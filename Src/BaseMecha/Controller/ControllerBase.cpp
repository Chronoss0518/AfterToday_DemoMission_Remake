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

#define DEFAULT_CURSOL_MOVE_SIZE 0.2f

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
	mouse->Init(windows);
	
	mouse->SetCenterFixedFlg(true);
	mouse->SetVisibleFlg(true);

	keyTypes[VK_SPACE] = InputName::Jump;
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

	controllerTypes[XInputTypeNames::A] = InputName::Attack;
	controllerTypes[XInputTypeNames::B] = InputName::Avo;
	controllerTypes[XInputTypeNames::X] = InputName::MapOnOff;
	controllerTypes[XInputTypeNames::Y] = InputName::Boost;
	controllerTypes[XInputTypeNames::LTop] = InputName::Front;
	controllerTypes[XInputTypeNames::LLeft] = InputName::Left;
	controllerTypes[XInputTypeNames::LDown] = InputName::Back;
	controllerTypes[XInputTypeNames::LRight] = InputName::Right;
	controllerTypes[XInputTypeNames::RTop] = InputName::None;
	controllerTypes[XInputTypeNames::RLeft] = InputName::None;
	controllerTypes[XInputTypeNames::RDown] = InputName::None;
	controllerTypes[XInputTypeNames::RRight] = InputName::None;
	controllerTypes[XInputTypeNames::L1] = InputName::LAttack;
	controllerTypes[XInputTypeNames::L2] = InputName::LWDChange;
	controllerTypes[XInputTypeNames::L3] = InputName::LReload;
	controllerTypes[XInputTypeNames::R1] = InputName::RAttack;
	controllerTypes[XInputTypeNames::R2] = InputName::RWDChange;
	controllerTypes[XInputTypeNames::R3] = InputName::RReload;

}

void PlayerController::UpdateBegin()
{
	auto targetMecha = LookObj<BaseMecha>();
	if (ChPtr::NullCheck(targetMecha))
	{
		Destroy();
		return;
	}

	XInputUpdate();
	
	for (auto&& key : keyTypes)
	{
		if (!ChSystem::SysManager().IsPushKey(key.first))continue;
		targetMecha->SetPushFlg(key.second);
	}

	CursolUpdate();
}

void PlayerController::XInputUpdate()
{
	auto targetMecha = LookObj<BaseMecha>();
	controller.Update();

	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetBFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::B]);
	if (controller.GetXFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::X]);
	if (controller.GetYFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::Y]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	

}

void PlayerController::CursolUpdate()
{
	mouse->Update();
	
	nowPos += mouse->GetMoveValueToChVec2();
	
	ChVec2 vector = nowPos;

	CursolFunction(vector.x, windSize.w, CursolMoveTypeName::Right, CursolMoveTypeName::Left);
	CursolFunction(vector.y, windSize.h, CursolMoveTypeName::Up, CursolMoveTypeName::Down);

	nowPos -= vector;
}

void PlayerController::CursolFunction(float& _value, const float _windSize, const CursolMoveTypeName _plus, const CursolMoveTypeName _minus)
{
	float mathMoveSencsitivility = 1.0f - moveSensitivility;

	float mathWindSize = _windSize * mathMoveSencsitivility * DEFAULT_CURSOL_MOVE_SIZE;

	auto targetMecha = LookObj<BaseMecha>();
	if (std::abs(_value) > mathWindSize)
	{
		if (_value > 0)
		{
			targetMecha->SetPushFlg(cursolInput[ChStd::EnumCast(_plus)]);
			_value -= mathWindSize;
		}
		else
		{
			targetMecha->SetPushFlg(cursolInput[ChStd::EnumCast(_minus)]);
			_value += mathWindSize;
		}
	}
}

std::function<bool(const float _targetValue, const float _testValue)> comparisonOperation[5] =
{
	[](const float _targetValue, const float _testValue)->bool {return _targetValue > _testValue; },
	[](const float _targetValue, const float _testValue)->bool {return _targetValue >= _testValue; },
	[](const float _targetValue, const float _testValue)->bool {return _targetValue == _testValue; },
	[](const float _targetValue, const float _testValue)->bool {return _targetValue <= _testValue; },
	[](const float _targetValue, const float _testValue)->bool {return _targetValue < _testValue; }
};

const std::string CPUController::cutChar = ",";

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

	SampleCreate();
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

void CPUController::SaveCPUData(const std::string& _fileName)
{
	if (!saveFlg)return;

	std::string textData = "";

	if (sampleFlg)saveFlg = false;
	textData += std::to_string(saveFlg ? 1 : 0) + "\n";
	textData += std::to_string(actionMoveMaxTime) + "\n";
	textData += std::to_string(actionMoveMinTime) + "\n";

	textData += cpuTargetSelect->Serialize();
	textData += cpuMovePositionSelect->Serialize();
	textData += cpuWeaponSelect->Serialize();


	ChCpp::File<> file;
	file.FileOpen(CPU_DIRECTORY(+_fileName));
	file.FileWriteText(textData);
	file.FileClose();

}

void CPUController::LoadCPUData(const std::string& _fileName)
{
	return;
	ChCpp::TextObject textData;

	{
		ChCpp::File<> file;
		file.FileOpen(CPU_DIRECTORY(+_fileName));
		textData.SetText(file.FileReadText());
		file.FileClose();
	}

	if (textData.LineCount() <= 1)
	{
		SampleCreate();
		SaveCPUData(_fileName);
		return;
	}

	saveFlg = ChStr::GetIntegialFromText<unsigned long>(textData.GetTextLine(0)) == 1;
	actionMoveMaxTime = ChStr::GetIntegialFromText<unsigned long>(textData.GetTextLine(1));
	actionMoveMinTime = ChStr::GetIntegialFromText<unsigned long>(textData.GetTextLine(2));
	
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
}

void CPUController::UpdateEndActionMoveTime()
{

	unsigned long tmpActionTime = actionMoveMaxTime - actionMoveMinTime;

	if (tmpActionTime <= 0)tmpActionTime = 1;

	endActionMoveTime = std::rand() % tmpActionTime;
	endActionMoveTime += actionMoveMinTime;

	if (endActionMoveTime <= 0)endActionMoveTime = 1;
}

std::string CPUActionBase::Serialize()
{
	std::string res = "";
#if 0
	for (unsigned char i = 0; i < ChStd::EnumCast(SerializeNo::None); i++)
	{
		res += GetValue(i) + cutChar;
	}

	res.pop_back();
#endif
	return res;
}

void CPUActionBase::Deserialize(const std::string& _text)
{
	if (_text.empty())return;

#if 0
	auto&& textList = ChStr::Split(_text, cutChar);

	activeFlg = textList[ChStd::EnumCast(SerializeNo::ActiveFlg)] == "1";

	memberType = (CPUObjectLooker::MemberType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::MemberType)]);

	distanceType = (CPUObjectLooker::DistanceType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DistanceType)]);
	testDistance = ChStr::GetFloatingFromText<float>(textList[ChStd::EnumCast(SerializeNo::TestDistance)]);
	distanceComparison = (ComparisonOperation)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DistanceComparison)]);

	damageType = (CPUObjectLooker::DamageSizeType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DamageType)]);
	testDamage = ChStr::GetFloatingFromText<float>(textList[ChStd::EnumCast(SerializeNo::TestDamage)]);
	damageComparison = (ComparisonOperation)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DamageComparison)]);
#endif
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

	OutputDebugString("MechaTest Success\n");

	if (!comparisonOperation[ChStd::EnumCast(damageComparison)](mecha->GetDamage(), testDamage))return false;

	OutputDebugString("DamageTest Success\n");

	float length = ChVec3::GetLen(thisMecha->GetPosition(), mecha->GetPosition());

	if (!comparisonOperation[ChStd::EnumCast(distanceComparison)](length, testDistance))return false;

	OutputDebugString("DistanceTest Success\n");

	return true;
}
