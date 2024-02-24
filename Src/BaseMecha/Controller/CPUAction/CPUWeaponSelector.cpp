#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../FunctionComponent/WeaponComponents.h"

#include"../../../Frames/GameFrame.h"
#include"../../../Attack/Attack.h"

#include"../../CPU/CPULooker.h"

#include"CPUWeaponSelector.h"
#include"CPUTargetSelector.h"
#include"CPUAttack.h"

ChPtr::Shared<ChCpp::JsonObject> CPUWeaponSelect::Serialize()
{
	auto&& res = CPUActionBase::Serialize();

	res->Set("AttackType", ChCpp::JsonNumber::CreateObject(ChStd::EnumCast(attackType)));

	res->Set("UnStopFlg", ChCpp::JsonBoolean::CreateObject(unStopFlg));

	return res;
}

void CPUWeaponSelect::Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)
{

	if (_jsonObject == nullptr)return;

	CPUActionBase::Deserialize(_jsonObject);

	auto&& attackTypeObject = _jsonObject->GetJsonNumber("AttackType");
	if (attackTypeObject != nullptr)attackType = static_cast<AttackType>((unsigned char)*attackTypeObject);

	auto&& unStopFlgObject = _jsonObject->GetJsonBoolean("UnStopFlg");
	if (unStopFlgObject != nullptr)unStopFlg = *unStopFlgObject;

}

bool CPUWeaponSelect::Update(unsigned long _lookTarget, GameFrame& _frame, CPUAttack& _attack, CPUController& _controller)
{
	if(!IsRunTest(_lookTarget, _frame, _controller))return false;

	if (!_attack.IsHaveAttackType(attackType))return false;

	return true;
}

ChPtr::Shared<ChCpp::JsonObject> CPUWeaponSelector::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject>();

	auto&& functionArray = ChPtr::Make_S<ChCpp::JsonArray>();

	for (auto&& function : functions)
	{
		functionArray->Add(function->Serialize());
	}

	res->Set("WeaponSelectorFunctions", functionArray);

	return res;
}

void CPUWeaponSelector::Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& functionArray = _jsonObject->GetJsonArray("WeaponSelectorFunctions");

	if (functionArray == nullptr)return;

	for (unsigned long i = 0; i < functionArray->GetCount(); i++)
	{
		auto&& function = ChPtr::Make_S<CPUWeaponSelect>();

		function->Deserialize(functionArray->GetJsonObject(i));

		Add(function);
	}
}

void CPUWeaponSelector::Update(CPUTargetSelector& _targetSelector, GameFrame& _frame, CPUAttack& _attack, CPUController& _controller)
{
	if (!_targetSelector.IsLookTarget())
	{
		beforeTarget = -1;
		return;
	}

	if (beforeTarget != _targetSelector.GetTargetNo())
	{
		beforeTarget = _targetSelector.GetTargetNo();
		ActionCansel();
		_attack.AttackCancel();
	}

	if (!selectWeaponsQueue.empty())return;

	OutputDebugString("CPUWeaponSelector Update Start\n");

	for (auto&& func : functions)
	{
		if (!func->Update(beforeTarget, _frame, _attack, _controller))continue;
		selectWeaponsQueue.push_back(func);
	}

	OutputDebugString("CPUWeaponSelector Update End\n");
}
