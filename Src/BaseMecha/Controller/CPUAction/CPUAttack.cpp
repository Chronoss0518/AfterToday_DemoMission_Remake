#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../FunctionComponent/WeaponComponent.h"

#include"../../../Frames/GameFrame.h"
#include"../../MechaPartsObjectFunction/WeaponFunction.h"
#include"../../../Attack/Attack.h"
#include"../../CPU/CPULooker.h"

#include"CPUAttack.h"
#include"CPUWeaponSelector.h"
#include"CPUTargetSelector.h"


ChPtr::Shared<ChCpp::JsonObject<wchar_t>> CPUAttack::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();

	res->Set(L"CenterLength", ChCpp::JsonNumber<wchar_t>::CreateObject(centerLength));
	
	res->Set(L"PriorityWeaponType", ChCpp::JsonNumber<wchar_t>::CreateObject(ChStd::EnumCast(priorityWeaponType)));
	
	res->Set(L"AttackCount", ChCpp::JsonNumber<wchar_t>::CreateObject(attackCount));

	res->Set(L"AttackType", ChCpp::JsonNumber<wchar_t>::CreateObject(attackType.GetValue()));

	return res;
}

void CPUAttack::Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& centerLengthObject = _jsonObject->GetJsonNumber(L"CenterLength");
	if (centerLengthObject != nullptr)centerLength = *centerLengthObject;
	
	auto&& priorityWeaponTypeObject = _jsonObject->GetJsonNumber(L"PriorityWeaponType");
	if (priorityWeaponTypeObject != nullptr)priorityWeaponType = static_cast<WeaponHandType>((unsigned char)*priorityWeaponTypeObject);

	auto&& attackCountObject = _jsonObject->GetJsonNumber(L"AttackCount");
	if (attackCountObject != nullptr)attackCount = *attackCountObject;

	auto&& attackTypeObject = _jsonObject->GetJsonNumber(L"AttackType");
	if (attackTypeObject != nullptr)attackType.SetValue(*attackTypeObject);

}

void CPUAttack::SetWeapons(const ChPtr::Shared<WeaponComponent>& _weapons)
{
	weaponFunctions = _weapons;
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

	//OutputDebugStringW(L"CPUAttack Update Start\n");

	FindRightPriorityAttack();

	FindLeftPriorityAttack();

	AttackFunction(_controller, leftCommandList);

	AttackFunction(_controller, rightCommandList);

	//OutputDebugStringW(L"CPUAttack Update End\n");
}


void CPUAttack::FindRightPriorityAttack()
{
	if (!leftCommandList.empty())return;
	if (!rightCommandList.empty())return;
	if (priorityWeaponType != WeaponHandType::Right)return;

	FindAttackFunction(
		WeaponHandType::Right,
		rightCommandList,
		ControllerBase::InputName::RAttack,
		ControllerBase::InputName::RWUChange,
		ControllerBase::InputName::RWDChange);

	FindAttackFunction(
		WeaponHandType::Left,
		leftCommandList,
		ControllerBase::InputName::LAttack,
		ControllerBase::InputName::LWUChange,
		ControllerBase::InputName::LWDChange);
}

void CPUAttack::FindLeftPriorityAttack()
{
	if (!leftCommandList.empty())return;
	if (!rightCommandList.empty())return;
	if (priorityWeaponType != WeaponHandType::Left)return;

	FindAttackFunction(
		WeaponHandType::Left,
		leftCommandList,
		ControllerBase::InputName::LAttack,
		ControllerBase::InputName::LWUChange,
		ControllerBase::InputName::LWDChange);

	FindAttackFunction(
		WeaponHandType::Right,
		rightCommandList,
		ControllerBase::InputName::RAttack,
		ControllerBase::InputName::RWUChange,
		ControllerBase::InputName::RWDChange);

}

void CPUAttack::FindAttackFunction(
	WeaponHandType _type,
	std::vector<ControllerBase::InputName>& _commandList,
	ControllerBase::InputName _attack,
	ControllerBase::InputName _weaponUpChange,
	ControllerBase::InputName _weaponDownChange)
{
	if (!_commandList.empty())return;
	if (weaponFunctions == nullptr)return;

	size_t nowSelectWeapon = weaponFunctions->GetUseWeaponNo(_type);
	size_t nowSelectAttack = -1;

	size_t targetWeapon = -1;

	ChPtr::Shared<WeaponFunction> weapon = nullptr;
	for (unsigned char i = 0; i < PALETTE_COUNT; i++)
	{
		weapon = weaponFunctions->GetWeaponFunction(_type, i);
		if (weapon == nullptr)continue;

		ChCpp::BitBool attackTypes = weapon->GetAttackData()->GetAttackType();
		if (!attackType.GetBitFlg(ChStd::EnumCast(selectWeapon->GetAttackType())))continue;
		targetWeapon = i;

		break;
	}

	if (targetWeapon > PALETTE_COUNT)return;

	for (size_t i = 0; i < attackCount; i++)
	{
		_commandList.push_back(_attack);
	}

	CreateAttackCommand(PALETTE_COUNT, nowSelectWeapon, targetWeapon, _commandList, _weaponUpChange, _weaponDownChange);

}

void CPUAttack::CreateAttackCommand(
	size_t maxSize,
	size_t nowSelectCount,
	size_t targetSelectCount,
	std::vector<ControllerBase::InputName>& _commandList,
	ControllerBase::InputName _upChange,
	ControllerBase::InputName _downChange)
{

	bool upFlg = true;
	size_t moveCount = (maxSize + nowSelectCount - targetSelectCount) % maxSize;

	size_t testMoveCount = ((maxSize * 2) - nowSelectCount - targetSelectCount) % maxSize;

	if (moveCount > testMoveCount)
	{
		moveCount = testMoveCount;
		upFlg = false;
	}

	for (size_t i = 0; i < moveCount; i++)
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