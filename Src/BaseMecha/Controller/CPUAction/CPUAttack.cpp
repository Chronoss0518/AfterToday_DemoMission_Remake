#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../FunctionComponent/WeaponComponents.h"

#include"../../../Frames/GameFrame.h"
#include"../../MechaPartsObjectFunction/WeaponFunction.h"
#include"../../../Attack/Attack.h"
#include"../../CPU/CPULooker.h"

#include"CPUAttack.h"
#include"CPUWeaponSelector.h"
#include"CPUTargetSelector.h"


ChPtr::Shared<ChCpp::JsonObject> CPUAttack::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject>();

	res->SetObject("CenterLength", ChCpp::JsonNumber::CreateObject(centerLength));
	
	res->SetObject("PriorityWeaponType", ChCpp::JsonNumber::CreateObject(ChStd::EnumCast(priorityWeaponType)));
	
	res->SetObject("AttackCount", ChCpp::JsonNumber::CreateObject(attackCount));

	res->SetObject("AttackType", ChCpp::JsonNumber::CreateObject(attackType.GetValue()));

	return res;
}

void CPUAttack::Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& centerLengthObject = _jsonObject->GetJsonNumber("CenterLength");
	if (centerLengthObject != nullptr)centerLength = *centerLengthObject;
	
	auto&& priorityWeaponTypeObject = _jsonObject->GetJsonNumber("PriorityWeaponType");
	if (priorityWeaponTypeObject != nullptr)priorityWeaponType = static_cast<PriorityWeaponType>((unsigned char)*priorityWeaponTypeObject);

	auto&& attackCountObject = _jsonObject->GetJsonNumber("AttackCount");
	if (attackCountObject != nullptr)attackCount = *attackCountObject;

	auto&& attackTypeObject = _jsonObject->GetJsonNumber("AttackType");
	if (attackTypeObject != nullptr)attackType.SetValue(*attackTypeObject);

}

void CPUAttack::SetRightWeapons(const ChPtr::Shared<RightWeaponComponent>& _weapons)
{
	rightWeaponFunctions = _weapons;

	if (rightWeaponFunctions == nullptr)return;

	SetAttackTypeFlgs(*rightWeaponFunctions);
}

void CPUAttack::SetLeftWeapons(const ChPtr::Shared<LeftWeaponComponent>& _weapons)
{
	leftWeaponFunctions = _weapons;

	if (leftWeaponFunctions == nullptr)return;

	SetAttackTypeFlgs(*leftWeaponFunctions);
}

void CPUAttack::Update(
	CPUWeaponSelector& _weaponSelector,
	CPUTargetSelector& _targetSelector,
	CPUController& _controller,
	GameFrame& _frame)
{
	if (!_targetSelector.IsLookTarget())return;

	if (selectWeapon == nullptr)selectWeapon = _weaponSelector.GetSelectWeapon();

	if (selectWeapon == nullptr)return;

	//OutputDebugString("CPUAttack Update Start\n");

	FindRightPriorityAttack();

	FindLeftPriorityAttack();

	AttackFunction(_controller, leftCommandList);

	AttackFunction(_controller, rightCommandList);

	//OutputDebugString("CPUAttack Update End\n");
}

void CPUAttack::SetAttackTypeFlgs(WeaponComponent& _weaponFunction)
{

	for (auto&& weapon : _weaponFunction.GetRegistWeaponList())
	{
		auto&& function = weapon.lock();

		auto&& attackData = function->GetAttackData();

		unsigned char attackTypeValue = attackData->GetAttackType().GetValue();

		attackTypeValue |= attackType.GetValue();

		attackType.SetValue(attackTypeValue);

		if (attackTypeValue == 31)break;
	}
}

void CPUAttack::FindRightPriorityAttack()
{
	if (!leftCommandList.empty())return;
	if (!rightCommandList.empty())return;
	if (priorityWeaponType != PriorityWeaponType::Right)return;

	FindAttackFunction(
		*rightWeaponFunctions,
		rightCommandList,
		ControllerBase::InputName::RAttack,
		ControllerBase::InputName::RWUChange,
		ControllerBase::InputName::RWDChange,
		ControllerBase::InputName::RATUChange,
		ControllerBase::InputName::RATDChange);

	FindAttackFunction(
		*leftWeaponFunctions,
		leftCommandList,
		ControllerBase::InputName::LAttack,
		ControllerBase::InputName::LWUChange,
		ControllerBase::InputName::LWDChange,
		ControllerBase::InputName::LATUChange,
		ControllerBase::InputName::LATDChange);
}

void CPUAttack::FindLeftPriorityAttack()
{
	if (!leftCommandList.empty())return;
	if (!rightCommandList.empty())return;
	if (priorityWeaponType != PriorityWeaponType::Left)return;

	FindAttackFunction(
		*leftWeaponFunctions,
		leftCommandList,
		ControllerBase::InputName::LAttack,
		ControllerBase::InputName::LWUChange,
		ControllerBase::InputName::LWDChange,
		ControllerBase::InputName::LATUChange,
		ControllerBase::InputName::LATDChange);

	FindAttackFunction(
		*rightWeaponFunctions,
		rightCommandList,
		ControllerBase::InputName::RAttack,
		ControllerBase::InputName::RWUChange,
		ControllerBase::InputName::RWDChange,
		ControllerBase::InputName::RATUChange,
		ControllerBase::InputName::RATDChange);

}

void CPUAttack::FindAttackFunction(
	WeaponComponent& _weaponFunction,
	std::vector<ControllerBase::InputName>& _commandList,
	ControllerBase::InputName _attack,
	ControllerBase::InputName _weaponUpChange,
	ControllerBase::InputName _weaponDownChange,
	ControllerBase::InputName _typeUpChange,
	ControllerBase::InputName _typeDownChange)
{
	if (ChPtr::NullCheck(&_weaponFunction))return;
	if (!_commandList.empty())return;

	unsigned long nowSelectWeapon = _weaponFunction.GetUseWeaponNo();
	unsigned long nowSelectAttack = -1;

	unsigned long targetWeapon = -1;
	unsigned long targetAttackType = -1;

	auto&& registWeaponList = _weaponFunction.GetWeaponMechaPartsList();

	unsigned long maxWeaponSize = registWeaponList.size();
	unsigned long maxAttackSize = 0;
	ChPtr::Shared<MechaPartsObject> weapon = nullptr;

	for (unsigned long i = 0; i < maxWeaponSize; i++)
	{
		weapon = registWeaponList[i].lock();
		if (weapon == nullptr)continue;

		auto&& weaponFunctions = weapon->GetWeaponFunctions();
		maxAttackSize = weaponFunctions.size();
		for (unsigned long j = 0; j < maxAttackSize; j++)
		{
			ChCpp::BitBool attackTypes = weaponFunctions[j]->GetAttackData()->GetAttackType();
			if (!attackType.GetBitFlg(ChStd::EnumCast(selectWeapon->GetAttackType())))continue;
			nowSelectAttack = weapon->GetUseAttackType();
			targetWeapon = i;
			targetAttackType = j;
			break;
		}

		if (targetWeapon > maxWeaponSize || targetAttackType > maxAttackSize)continue;

		break;

	}

	if (targetWeapon > maxWeaponSize || targetAttackType > maxAttackSize)return;

	for (unsigned long i = 0; i < attackCount; i++)
	{
		_commandList.push_back(_attack);
	}

	CreateAttackCommand(maxAttackSize, nowSelectAttack, targetAttackType, _commandList, _typeUpChange, _typeDownChange);

	CreateAttackCommand(maxWeaponSize, nowSelectWeapon, targetWeapon, _commandList, _weaponUpChange, _weaponDownChange);

}

void CPUAttack::CreateAttackCommand(
	unsigned long maxSize,
	unsigned long nowSelectCount,
	unsigned long targetSelectCount,
	std::vector<ControllerBase::InputName>& _commandList,
	ControllerBase::InputName _upChange,
	ControllerBase::InputName _downChange)
{

	bool upFlg = true;
	unsigned long moveCount = (maxSize + nowSelectCount - targetSelectCount) % maxSize;

	unsigned long testMoveCount = ((maxSize * 2) - nowSelectCount - targetSelectCount) % maxSize;

	if (moveCount > testMoveCount)
	{
		moveCount = testMoveCount;
		upFlg = false;
	}

	for (unsigned long i = 0; i < moveCount; i++)
	{
		_commandList.push_back(upFlg ? _upChange : _downChange);
	}
}

void CPUAttack::AttackFunction(CPUController& _controller,
	std::vector<ControllerBase::InputName>& _commandList)
{
	if (_commandList.empty())return;

	_controller.Input(_commandList[_commandList.size() - 1], false);

	_commandList.pop_back();
}