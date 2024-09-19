#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../FunctionComponent/WeaponComponents.h"

#include"../../../Frames/GameFrame.h"

#include"../../CPU/CPULooker.h"

#include"CPUTargetSelector.h"

ChPtr::Shared<ChCpp::JsonObject<wchar_t>> CPUTargetSelect::Serialize()
{
	return CPUActionBase::Serialize();
}

void CPUTargetSelect::Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	CPUActionBase::Deserialize(_jsonObject);
}

unsigned long CPUTargetSelect::Update(CPUObjectLooker& _lookTarget, GameFrame& _frame, CPUController& _controller)
{
	unsigned long lookTargetNo = GetLookTypeMechas(_lookTarget);

	if (!IsRunTest(lookTargetNo, _frame, _controller))return -1;

	return lookTargetNo;
}

ChPtr::Shared<ChCpp::JsonObject<wchar_t>> CPUTargetSelector::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();

	auto&& functionArray = ChPtr::Make_S<ChCpp::JsonArray<wchar_t>>();

	for (auto&& function : functions)
	{
		functionArray->Add(function->Serialize());
	}

	res->Set(L"TargetSelectorFunctions", functionArray);

	return res;
}

void CPUTargetSelector::Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& functionArray = _jsonObject->GetJsonArray(L"TargetSelectorFunctions");

	if (functionArray == nullptr)return;

	for (unsigned long i = 0; i < functionArray->GetCount(); i++)
	{
		auto&& function = ChPtr::Make_S<CPUTargetSelect>();

		function->Deserialize(functionArray->GetJsonObject(i));

		Add(function);
	}
}

void CPUTargetSelector::Update(CPUObjectLooker& _looker, GameFrame& _frame, CPUController& _controller)
{
	if (functions.empty())return;

	lookTargetFlg = false;
	targetNo = -1;

	auto&& mechaList = _frame.GetMechas();

	for (auto&& selectFunction : functions)
	{
		unsigned long test = selectFunction->Update(_looker, _frame, _controller);

		if (test >= mechaList.size())continue;
		auto&& testMecha = mechaList[test];
		if (testMecha.expired())continue;
		targetNo = test;
		lookTargetFlg = true;
		break;
	}

}
