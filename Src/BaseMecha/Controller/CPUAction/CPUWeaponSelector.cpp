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

ChPtr::Shared<ChCpp::JsonObject<wchar_t>> CPUWeaponSelect::Serialize()
{
	auto&& res = CPUActionBase::Serialize();

	res->Set(L"AttackType", ChCpp::JsonNumber<wchar_t>::CreateObject(ChStd::EnumCast(attackType)));

	res->Set(L"UnStopFlg", ChCpp::JsonBoolean<wchar_t>::CreateObject(unStopFlg));

	return res;
}

void CPUWeaponSelect::Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)
{

	if (_jsonObject == nullptr)return;

	CPUActionBase::Deserialize(_jsonObject);

	auto&& attackTypeObject = _jsonObject->GetJsonNumber(L"AttackType");
	if (attackTypeObject != nullptr)attackType = static_cast<AttackType>((unsigned char)*attackTypeObject);

	auto&& unStopFlgObject = _jsonObject->GetJsonBoolean(L"UnStopFlg");
	if (unStopFlgObject != nullptr)unStopFlg = *unStopFlgObject;

}

bool CPUWeaponSelect::Update(unsigned long _lookTarget, GameFrame& _frame, CPUAttack& _attack, CPUController& _controller)
{
	if(!IsRunTest(_lookTarget, _frame, _controller))return false;

	if (!_attack.IsHaveAttackType(attackType))return false;

	return true;
}

ChPtr::Shared<ChCpp::JsonObject<wchar_t>> CPUWeaponSelector::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();

	auto&& functionArray = ChPtr::Make_S<ChCpp::JsonArray<wchar_t>>();

	for (auto&& function : functions)
	{
		functionArray->Add(function->Serialize());
	}

	res->Set(L"WeaponSelectorFunctions", functionArray);

	return res;
}

void CPUWeaponSelector::Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& functionArray = _jsonObject->GetJsonArray(L"WeaponSelectorFunctions");

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

	OutputDebugString(L"CPUWeaponSelector Update Start\n");

	for (auto&& func : functions)
	{
		if (!func->Update(beforeTarget, _frame, _attack, _controller))continue;
		selectWeaponsQueue.push_back(func);
	}

	OutputDebugString(L"CPUWeaponSelector Update End\n");
}
