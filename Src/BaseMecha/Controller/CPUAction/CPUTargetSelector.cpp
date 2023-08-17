#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../FunctionComponent/WeaponComponents.h"

#include"../../../Frames/GameFrame.h"

#include"../../CPU/CPULooker.h"

#include"CPUTargetSelector.h"

ChPtr::Shared<ChCpp::JsonObject> CPUTargetSelect::Serialize()
{
	return CPUActionBase::Serialize();
}

void CPUTargetSelect::Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)
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

ChPtr::Shared<ChCpp::JsonObject> CPUTargetSelector::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject>();

	auto&& functionArray = ChPtr::Make_S<ChCpp::JsonArray>();

	for (auto&& function : functions)
	{
		functionArray->AddObject(function->Serialize());
	}

	res->SetObject("TargetSelectorFunctions", functionArray);

	return res;
}

void CPUTargetSelector::Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& functionArray = _jsonObject->GetJsonArray("TargetSelectorFunctions");

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
	for (auto&& selectFunction : functions)
	{
		unsigned long test = selectFunction->Update(_looker, _frame, _controller);

		//OutputDebugString(("TestTargetNo :" + std::to_string(test) + "\n").c_str());
		if (test >= _frame.GetMechaList().GetObjectCount())continue;
		targetNo = test;
		lookTargetFlg = true;
		break;
	}

}
