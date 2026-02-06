#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../BaseMecha.h"

#include"../FunctionComponent/WeaponComponents.h"

#include"../../Frames/GameFrame.h"
#include"../../Attack/Attack.h"

#include"CPUController.h"

#include"../CPU/CPULooker.h"

#include"CPUAction/CPUActionBase.h"
#include"CPUAction/CPUWeaponSelector.h"
#include"CPUAction/CPUTargetSelector.h"
#include"CPUAction/CPUMovePositionSelector.h"
#include"CPUAction/CPUAttack.h"
#include"CPUAction/CPUMoveInput.h"


std::function<bool(const float _targetValue, const float _testValue)> comparisonOperation[5] =
{
	[](const float _targetValue, const float _testValue)->bool {return _targetValue > _testValue; },
	[](const float _targetValue, const float _testValue)->bool {return _targetValue >= _testValue; },
	[](const float _targetValue, const float _testValue)->bool {return _targetValue == _testValue; },
	[](const float _targetValue, const float _testValue)->bool {return _targetValue <= _testValue; },
	[](const float _targetValue, const float _testValue)->bool {return _targetValue < _testValue; }
};

const std::wstring CPUController::cutChar = L",";

void CPUController::Init()
{
	cpuTargetSelect = ChPtr::Make_S<CPUTargetSelector>();

	cpuMovePositionSelect = ChPtr::Make_S<CPUMovePositionSelector>();

	cpuWeaponSelect = ChPtr::Make_S<CPUWeaponSelector>();

	cpuAttack = ChPtr::Make_S<CPUAttack>();

	//cpuAvoidTest = ChPtr::Make_S<CPUAvoidTest>();

	cpuMoveInput = ChPtr::Make_S<CPUMoveInput>();


	cpuAttack->SetRightWeapons(LookObj()->GetComponent<RightWeaponComponent>());
	cpuAttack->SetLeftWeapons(LookObj()->GetComponent<LeftWeaponComponent>());

	//SampleCreate();
}

void CPUController::UpdateBegin()
{
	if (ChPtr::NullCheck(frame))
	{
		LookObj()->Destroy();
		return;
	}

	auto mecha = LookObj<BaseMecha>();

	if (ChPtr::NullCheck(mecha))return;

	if (mecha->IsBreak())return;

	if (nowActionMoveTime <= endActionMoveTime)
	{
		nowActionMoveTime++;

		for (auto&& input : inputDataParFrame)
		{
			Input(input);
		}

		return;
	}

	nowActionMoveTime = 0;
	inputDataParFrame.clear();

	UpdateEndActionMoveTime();

	auto looker = mecha->GetComponent<CPUObjectLooker>();

	if (looker == nullptr)return;
#if USE_CPU_THREAD

	while (!looker->IsEndUpdate()) {};

#endif
	cpuTargetSelect->Update(*looker, *frame, *this);

	cpuMovePositionSelect->Update(*cpuTargetSelect, *frame, *this);

	cpuWeaponSelect->Update(*cpuTargetSelect, *frame, *cpuAttack, *this);

	cpuAttack->Update(*cpuWeaponSelect, *cpuTargetSelect, *this, *frame);

	cpuMoveInput->Update(*this, *cpuTargetSelect, *cpuMovePositionSelect, *frame);
}

void CPUController::Input(InputName _input, bool _addInputDataParFrameFlg)
{
	if (_addInputDataParFrameFlg)inputDataParFrame.push_back(_input);
	Input(_input);
}

void CPUController::SaveCPUData(const std::wstring& _fileName)
{
	if (!saveFlg)return;

	if (sampleFlg)saveFlg = false;

	auto&& saveData = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();

	saveData->Set(L"SaveFlg", ChCpp::JsonBoolean<wchar_t>::CreateObject(saveFlg));
	saveData->Set(L"ActionMoveMaxTime", ChCpp::JsonNumber<wchar_t>::CreateObject(actionMoveMaxTime));
	saveData->Set(L"ActionMoveMinTime", ChCpp::JsonNumber<wchar_t>::CreateObject(actionMoveMinTime));

	saveData->Set(L"TargetSelector", cpuTargetSelect->Serialize());
	saveData->Set(L"MovePositionSelector", cpuMovePositionSelect->Serialize());
	saveData->Set(L"WeaponSelector", cpuWeaponSelect->Serialize());
	saveData->Set(L"MoveInput", cpuMoveInput->Serialize());
	saveData->Set(L"Attack", cpuAttack->Serialize());


	ChCpp::CharFile file;
	file.FileOpen(CPU_DIRECTORY(+_fileName), true);
	file.FileWrite(ChStr::GetUTF8FromUTF16(ChCpp::JsonBaseType<wchar_t>::FormatDocument(saveData->GetRawData())));
	file.FileClose();

}

void CPUController::LoadCPUData(const std::wstring& _fileName)
{
	std::wstring textData;

	{
		ChCpp::CharFile file;
		file.FileOpen(CPU_DIRECTORY(+_fileName), false);
		textData = ChStr::GetUTF16FromUTF8(file.FileRead());
		file.FileClose();
	}

	auto&& json = ChCpp::JsonBaseType<wchar_t>::GetParameter(textData);

	if (json == nullptr)return;

	auto&& jsonObject = ChPtr::SharedSafeCast<ChCpp::JsonObject<wchar_t>>(json);

	if (jsonObject == nullptr)return;

	auto&& saveFlgObject = jsonObject->GetJsonBoolean(L"SaveFlg");
	if (saveFlgObject != nullptr)saveFlg = *saveFlgObject;

	auto&& actionMoveMaxTimeObject = jsonObject->GetJsonNumber(L"ActionMoveMaxTime");
	if (actionMoveMaxTimeObject != nullptr)actionMoveMaxTime = *actionMoveMaxTimeObject;

	auto&& actionMoveMinTimeObject = jsonObject->GetJsonNumber(L"ActionMoveMinTime");
	if (actionMoveMinTimeObject != nullptr)actionMoveMinTime = *actionMoveMinTimeObject;

	cpuTargetSelect->Deserialize(jsonObject->GetJsonObject(L"TargetSelector"));
	cpuMovePositionSelect->Deserialize(jsonObject->GetJsonObject(L"MovePositionSelector"));
	cpuWeaponSelect->Deserialize(jsonObject->GetJsonObject(L"WeaponSelector"));
	cpuMoveInput->Deserialize(jsonObject->GetJsonObject(L"MoveInput"));
	cpuAttack->Deserialize(jsonObject->GetJsonObject(L"Attack"));

	endActionMoveTime = actionMoveMaxTime;

	UpdateEndActionMoveTime();

	nowActionMoveTime = endActionMoveTime;
}

void CPUController::SampleCreate()
{
	sampleFlg = true;

	saveFlg = true;
	actionMoveMaxTime = 10;
	actionMoveMinTime = 5;

	auto function = ChPtr::Make_S<CPUWeaponSelect>();
	function->SetActiveFlg(true);
	function->SetMemberType(CPUObjectLooker::MemberType::Enemy);
	function->SetDistanceType(CPUObjectLooker::DistanceType::Near);
	function->SetTestDistance(1000.0f);
	function->SetDistanceComparison(ComparisonOperation::Less);
	function->SetDamageType(CPUObjectLooker::DamageSizeType::Many);
	function->SetTestDamage(0.0f);
	function->SetDamageComparison(ComparisonOperation::More);
	function->SetAttackType(AttackType::Bullet);

	cpuWeaponSelect->Add(function);

	auto targetSelector = ChPtr::Make_S<CPUTargetSelect>();
	targetSelector->SetActiveFlg(true);
	targetSelector->SetMemberType(CPUObjectLooker::MemberType::Enemy);
	targetSelector->SetDistanceType(CPUObjectLooker::DistanceType::Near);
	targetSelector->SetTestDistance(1000.0f);
	targetSelector->SetDistanceComparison(ComparisonOperation::Less);
	targetSelector->SetDamageType(CPUObjectLooker::DamageSizeType::Many);
	targetSelector->SetTestDamage(0.0f);
	targetSelector->SetDamageComparison(ComparisonOperation::More);

	cpuTargetSelect->Add(targetSelector);

	cpuMovePositionSelect->SetInitPosition(*this, 20.0f, false, false, false);


	endActionMoveTime = actionMoveMaxTime;

	UpdateEndActionMoveTime();

	nowActionMoveTime = endActionMoveTime;
	SaveCPUData(L"mob.cpu");
}

void CPUController::UpdateEndActionMoveTime()
{

	unsigned long tmpActionTime = actionMoveMaxTime - actionMoveMinTime;

	if (tmpActionTime <= 0)tmpActionTime = 1;

	endActionMoveTime = std::rand() % tmpActionTime;
	endActionMoveTime += actionMoveMinTime;

	if (endActionMoveTime <= 0)endActionMoveTime = 1;
}

ChPtr::Shared<ChCpp::JsonObject<wchar_t>> CPUActionBase::Serialize()
{

	auto&& res = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();

	res->Set(L"MemberType", ChCpp::JsonNumber<wchar_t>::CreateObject(ChStd::EnumCast(memberType)));

	res->Set(L"DistanceType", ChCpp::JsonNumber<wchar_t>::CreateObject(ChStd::EnumCast(distanceType)));
	res->Set(L"TestDistance", ChCpp::JsonNumber<wchar_t>::CreateObject(testDistance));
	res->Set(L"DistanceComparison", ChCpp::JsonNumber<wchar_t>::CreateObject(ChStd::EnumCast(distanceComparison)));


	res->Set(L"DamageSizeType", ChCpp::JsonNumber<wchar_t>::CreateObject(ChStd::EnumCast(damageType)));
	res->Set(L"TestDamage", ChCpp::JsonNumber<wchar_t>::CreateObject(testDamage));
	res->Set(L"DamageComparison", ChCpp::JsonNumber<wchar_t>::CreateObject(ChStd::EnumCast(damageComparison)));

	res->Set(L"ActiveFlg", ChCpp::JsonBoolean<wchar_t>::CreateObject(activeFlg));

	return res;
}

void CPUActionBase::Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& memberTypeObject = _jsonObject->GetJsonNumber(L"MemberType");
	if (memberTypeObject != nullptr)memberType = static_cast<CPUObjectLooker::MemberType>((unsigned char)*memberTypeObject);


	auto&& distanceTypeObject = _jsonObject->GetJsonNumber(L"DistanceType");
	if (distanceTypeObject != nullptr)distanceType = static_cast<CPUObjectLooker::DistanceType>((unsigned char)*distanceTypeObject);

	auto&& testDistanceObject = _jsonObject->GetJsonNumber(L"TestDistance");
	if (testDistanceObject != nullptr)testDistance = *testDistanceObject;

	auto&& distanceComparisonObject = _jsonObject->GetJsonNumber(L"DistanceComparison");
	if (distanceComparisonObject != nullptr)distanceComparison = static_cast<CPUController::ComparisonOperation>((unsigned char)*distanceComparisonObject);


	auto&& damageTypeObject = _jsonObject->GetJsonNumber(L"DamageSizeType");
	if (damageTypeObject != nullptr)damageType = static_cast<CPUObjectLooker::DamageSizeType>((unsigned char)*damageTypeObject);

	auto&& testDamageObject = _jsonObject->GetJsonNumber(L"TestDamage");
	if (testDamageObject != nullptr)testDamage = *testDamageObject;

	auto&& damageComparisonObject = _jsonObject->GetJsonNumber(L"DamageComparison");
	if (damageComparisonObject != nullptr)damageComparison = static_cast<CPUController::ComparisonOperation>((unsigned char)*damageComparisonObject);


	auto&& activeFlgObject = _jsonObject->GetJsonBoolean(L"ActiveFlg");
	if (activeFlgObject != nullptr)activeFlg = *activeFlgObject;

}

unsigned long CPUActionBase::GetLookTypeMechas(CPUObjectLooker& _lookTarget)
{
	return _lookTarget.GetLookTypeMechas(memberType, distanceType, damageType);
}

bool CPUActionBase::IsRunTest(unsigned long _lookTarget, GameFrame& _frame, CPUController& _controller)
{
	if (!activeFlg)return false;

	auto&& thisMecha = _controller.GetBaseMecha();

	if (ChPtr::NullCheck(thisMecha))return false;

	if (thisMecha->IsBreak())return false;

	auto&& mechas = _frame.GetMechas();

	if (mechas.size() <= _lookTarget)return false;

	auto target = mechas[_lookTarget];

	if (target.expired())return false;

	auto mecha = target.lock();

	if (mecha == nullptr)return false;

	if (mecha->IsBreak())return false;

	OutputDebugStringW(L"MechaTest Success\n");

	if (!comparisonOperation[ChStd::EnumCast(damageComparison)](mecha->GetDamage(), testDamage))return false;

	OutputDebugStringW(L"DamageTest Success\n");

	float length = ChVec3::GetLen(thisMecha->GetPosition(), mecha->GetPosition());

	if (!comparisonOperation[ChStd::EnumCast(distanceComparison)](length, testDistance))return false;

	OutputDebugStringW(L"DistanceTest Success\n");

	return true;
}