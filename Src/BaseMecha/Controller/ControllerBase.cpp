#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../BaseMecha.h"

#include"../FunctionComponent/WeaponComponents.h"

#include"../../Frames/GameFrame.h"
#include"../../Attack/Attack.h"

#include"ControllerBase.h"
#include"../CPU/CPULooker.h"

#include"CPUAction/CPUActionBase.h"
#include"CPUAction/CPUWeaponSelector.h"
#include"CPUAction/CPUTargetSelector.h"
#include"CPUAction/CPUMovePositionSelector.h"
#include"CPUAction/CPUAttack.h"
#include"CPUAction/CPUMoveInput.h"

#define MIN_CURSOL_LEN_PARCEC 0.01f
#define DEFAULT_CONTROLLER_MOVE_SIZE 0.3f

void ControllerBase::Input(const InputName _inputFlgName)
{
	auto targetMecha = LookObj<BaseMecha>();
	if (ChPtr::NullCheck(targetMecha))Destroy();

	targetMecha->SetPushFlg(_inputFlgName);
}

void PlayerController::Init()
{
	auto&& windows = *ChSystem::SysManager().GetSystem<ChSystem::Windows>();
	windSize.h = static_cast<float>(windows.GetWindHeight());
	windSize.w = static_cast<float>(windows.GetWindWidth());
	baseLen = (windSize.h < windSize.w ? windSize.h : windSize.w);


	keyTypes[VK_SPACE] = InputName::Up;
	keyTypes[VK_SHIFT] = InputName::Avo;
	keyTypes[VK_CONTROL] = InputName::Boost;

	keyTypes['Q'] = InputName::SetCameraCenter;
	keyTypes['E'] = InputName::MapOnOff;
	keyTypes['W'] = InputName::Front;
	keyTypes['A'] = InputName::Left;
	keyTypes['D'] = InputName::Right;
	keyTypes['S'] = InputName::Back;

	keyTypes[VK_LEFT] = InputName::LeftRotation;
	keyTypes[VK_RIGHT] = InputName::RightRotation;
	
	cursolInput[ChStd::EnumCast(CursolMoveTypeName::Left)] = InputName::LeftRotation;
	cursolInput[ChStd::EnumCast(CursolMoveTypeName::Right)] = InputName::RightRotation;

	cursolInput[ChStd::EnumCast(CursolMoveTypeName::Up)] = InputName::CameraUpRotation;
	cursolInput[ChStd::EnumCast(CursolMoveTypeName::Down)] = InputName::CameraDownRotation;

	keyTypes[VK_LBUTTON] = InputName::LAttack;
	keyTypes[VK_RBUTTON] = InputName::RAttack;

	//keyTypes[VK_CONTROL] = InputName::MSChange;
	keyTypes[VK_MBUTTON] = InputName::WeaponDownChange;
	keyTypes[VK_SCROLL] = InputName::MagnificationUp;
	
	controller.Init();

	controllerTypes[XInputTypeNames::A] = InputName::Up;
	controllerTypes[XInputTypeNames::B] = InputName::Boost;
	controllerTypes[XInputTypeNames::X] = InputName::Avo;
	controllerTypes[XInputTypeNames::Y] = InputName::MapOnOff;
	controllerTypes[XInputTypeNames::LTop] = InputName::Front;
	controllerTypes[XInputTypeNames::LLeft] = InputName::Left;
	controllerTypes[XInputTypeNames::LDown] = InputName::Back;
	controllerTypes[XInputTypeNames::LRight] = InputName::Right;
	controllerTypes[XInputTypeNames::RTop] = InputName::CameraDownRotation;
	controllerTypes[XInputTypeNames::RLeft] = InputName::LeftRotation;
	controllerTypes[XInputTypeNames::RDown] = InputName::CameraUpRotation;
	controllerTypes[XInputTypeNames::RRight] = InputName::RightRotation;
	controllerTypes[XInputTypeNames::L1] = InputName::LAttack;
	controllerTypes[XInputTypeNames::L2] = InputName::LWDChange;
	controllerTypes[XInputTypeNames::L3] = InputName::LReload;
	controllerTypes[XInputTypeNames::R1] = InputName::RAttack;
	controllerTypes[XInputTypeNames::R2] = InputName::RWDChange;
	controllerTypes[XInputTypeNames::R3] = InputName::RReload;

}

void PlayerController::UpdateBegin()
{
	auto targetMecha = GetBaseMecha();
	if (ChPtr::NullCheck(targetMecha))
	{
		Destroy();
		return;
	}

	XInputUpdate();

	if (!controllerPushFlg)
	{
		for (auto&& key : keyTypes)
		{
			if (!ChSystem::SysManager().IsPushKey(key.first))continue;
			targetMecha->SetPushFlg(key.second);
		}

	}

	CursolUpdate();
}

void PlayerController::XInputUpdate()
{
	controllerPushFlg = false;
	controller.Update();

	if (controller.GetAFlg())SetXInputFlg(XInputTypeNames::A);
	
	if (controller.GetBFlg())SetXInputFlg(XInputTypeNames::B);
	
	if (controller.GetXFlg())SetXInputFlg(XInputTypeNames::X);
	
	if (controller.GetYFlg())SetXInputFlg(XInputTypeNames::Y);

	if (controller.GetUpFlg())SetXInputFlg(XInputTypeNames::Up);
	if (controller.GetDownFlg())SetXInputFlg(XInputTypeNames::Down);
	if (controller.GetLeftFlg())SetXInputFlg(XInputTypeNames::Left);
	if (controller.GetRightFlg())SetXInputFlg(XInputTypeNames::Right);

	if (controller.GetBackFlg())SetXInputFlg(XInputTypeNames::Back);
	if (controller.GetStartFlg())SetXInputFlg(XInputTypeNames::Start);

	if (controller.GetR1Flg())SetXInputFlg(XInputTypeNames::R1);
	if (controller.GetR2Trigger() > DEFAULT_CONTROLLER_MOVE_SIZE)SetXInputFlg(XInputTypeNames::R2);
	if (controller.GetR3Flg())SetXInputFlg(XInputTypeNames::R3);

	if (controller.GetL1Flg())SetXInputFlg(XInputTypeNames::L1);
	if (controller.GetL2Trigger() > DEFAULT_CONTROLLER_MOVE_SIZE)SetXInputFlg(XInputTypeNames::L2);
	if (controller.GetL3Flg())SetXInputFlg(XInputTypeNames::L3);


	if (controller.GetLXStick() > DEFAULT_CONTROLLER_MOVE_SIZE)
	{
		SetXInputFlg(XInputTypeNames::LRight);
	}

	if (controller.GetLXStick() < -DEFAULT_CONTROLLER_MOVE_SIZE)
	{
		SetXInputFlg(XInputTypeNames::LLeft);
	}

	if (controller.GetLYStick() < -DEFAULT_CONTROLLER_MOVE_SIZE)
	{
		SetXInputFlg(XInputTypeNames::LDown);
	}

	if (controller.GetLYStick() > DEFAULT_CONTROLLER_MOVE_SIZE)
	{
		SetXInputFlg(XInputTypeNames::LTop);
	}

	if (controller.GetRXStick() > DEFAULT_CONTROLLER_MOVE_SIZE)
	{
		SetXInputFlg(XInputTypeNames::RRight);
	}

	if (controller.GetRXStick() < -DEFAULT_CONTROLLER_MOVE_SIZE)
	{
		SetXInputFlg(XInputTypeNames::RLeft);
	}

	if (controller.GetRYStick() < -DEFAULT_CONTROLLER_MOVE_SIZE)
	{
		SetXInputFlg(XInputTypeNames::RDown);
	}

	if (controller.GetRYStick() > DEFAULT_CONTROLLER_MOVE_SIZE)
	{
		SetXInputFlg(XInputTypeNames::RTop);
	}

}

void PlayerController::CursolUpdate()
{
	mouse->Update();

	ChVec2 vector = mouse->GetNowPosToChVec2() -= (windSize / 2.0f);
	vector /= baseLen;

	if (vector.x > 1.0f)vector.x = 1.0f;
	if (vector.x < -1.0f)vector.x = -1.0f;
	if (vector.y > 1.0f)vector.y = 1.0f;
	if (vector.y < -1.0f)vector.y = -1.0f;

	vector *= moveSensitivility;
	
	nowPos += vector;

	CursolFunction(vector.x, CursolMoveTypeName::Right, CursolMoveTypeName::Left);
	CursolFunction(vector.y, CursolMoveTypeName::Up, CursolMoveTypeName::Down);

}

void PlayerController::CursolFunction(float& _value, const CursolMoveTypeName _plus, const CursolMoveTypeName _minus)
{

	if (controllerPushFlg)return;

	if (std::abs(_value) < MIN_CURSOL_LEN_PARCEC)
	{
		_value = 0.0f;
		return;
	}

	auto targetMecha = GetBaseMecha();
	if (_value > MIN_CURSOL_LEN_PARCEC)
	{
		targetMecha->SetPushFlg(cursolInput[ChStd::EnumCast(_plus)]);
		_value -= MIN_CURSOL_LEN_PARCEC;
	}

	if (_value <-MIN_CURSOL_LEN_PARCEC)
	{
		targetMecha->SetPushFlg(cursolInput[ChStd::EnumCast(_minus)]);
		_value += MIN_CURSOL_LEN_PARCEC;
	}
}

void PlayerController::SetXInputFlg(const XInputTypeNames _xinputType)
{
	auto targetMecha = GetBaseMecha();
	controllerPushFlg = true;
	targetMecha->SetPushFlg(controllerTypes[_xinputType]);
}

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

	cpuWeaponSelect->Update(*cpuTargetSelect, *frame,*cpuAttack,*this);

	cpuAttack->Update(*cpuWeaponSelect, *cpuTargetSelect, *this, *frame);

	cpuMoveInput->Update(*this,*cpuTargetSelect,*cpuMovePositionSelect,*frame);
}

void CPUController::Input(InputName _input, bool _addInputDataParFrameFlg)
{
	if(_addInputDataParFrameFlg)inputDataParFrame.push_back(_input);
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


	ChCpp::WCharFile file;
	file.FileOpen(CPU_DIRECTORY(+_fileName));
	file.FileWriteText(ChCpp::JsonBaseType<wchar_t>::FormatDocument(saveData->GetRawData()));
	file.FileClose();

}

void CPUController::LoadCPUData(const std::wstring& _fileName)
{
	std::wstring textData;

	{
		ChCpp::WCharFile file;
		file.FileOpen(CPU_DIRECTORY(+_fileName));
		textData = file.FileReadText();
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

	OutputDebugString(L"MechaTest Success\n");

	if (!comparisonOperation[ChStd::EnumCast(damageComparison)](mecha->GetDamage(), testDamage))return false;

	OutputDebugString(L"DamageTest Success\n");

	float length = ChVec3::GetLen(thisMecha->GetPosition(), mecha->GetPosition());

	if (!comparisonOperation[ChStd::EnumCast(distanceComparison)](length, testDistance))return false;

	OutputDebugString(L"DistanceTest Success\n");

	return true;
}
