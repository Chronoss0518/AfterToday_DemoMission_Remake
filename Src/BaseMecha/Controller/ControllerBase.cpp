#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../BaseMecha.h"

#include"../FunctionComponent/WeaponComponents.h"

#include"../../Frames/GameFrame.h"

#include"ControllerBase.h"

#include"../CPU/CPULooker.h"


#define DEFAULT_CURSOL_MOVE_SIZE 0.2f

class CPUController::CPUFunctionBase
{
public:

	enum class HaveHandType
	{
		L,R
	};

	void Init();

	std::string Serialize();

	void Deserialize(const std::string& _text);

	void SetUnStopFlg(const ChStd::Bool _flg) { unStopFlg = _flg; }

	ChStd::Bool IsUnStopFlg() { return unStopFlg; }

	ChStd::Bool IsRunTest(CPUObjectLooker& _looker);

	ChStd::Bool IsRunEnd() { return isRunEndFlg; }

	void Update(CPUController& _controller);

private:

	ChPtr::Shared<CPUObjectLooker::UseSquareValues> target;

	CPUObjectLooker::MemberType memberType = CPUObjectLooker::MemberType::Enemy;
	CPUObjectLooker::DistanceType distanceType = CPUObjectLooker::DistanceType::None;
	CPUObjectLooker::DamageSizeType damageType = CPUObjectLooker::DamageSizeType::None;

	float testDamage = 0.0f;
	ComparisonOperation damageComparison = ComparisonOperation::Smaller;

	float testDistance = 0.0f;
	ComparisonOperation distanceComparison = ComparisonOperation::Smaller;

	std::function<ChStd::Bool(const float _targetValue, const float _testValue)>comparisonOperation[5];

	unsigned long weaponNo = 0;
	unsigned long attackType = 0;
	HaveHandType handType = HaveHandType::L;

	//この命令を行ってもスクリプトを抜けないフラグ//
	ChStd::Bool unStopFlg = false;

	ChStd::Bool isRunEndFlg = false;

};


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

void CPUController::Init()
{

}

void CPUController::UpdateBegin()
{
	if (ChPtr::NullCheck(frame))
	{
		LookObj()->Destroy();
		return;
	}
	
	auto looker = LookObj()->GetComponent<CPUObjectLooker>();

	if (looker == nullptr)
	{
		LookObj()->Destroy();
		return;
	}

	while (!looker->IsEndUpdate())continue;

	SelectRunFunction(*looker);
	RunUpdate();
}

void CPUController::SaveCPUData(const std::string& _fileName)
{

}

void CPUController::LoadCPUData(const std::string& _fileName)
{
	ChCpp::TextObject textData;
	
	{
		ChCpp::File<> file;
		file.FileOpen(CPU_DIRECTORY(+_fileName));
		textData.SetText(file.FileReadText());
		file.FileClose();
	}

	actionMoveTime = ChStr::GetIntegialFromText<unsigned long>(textData.GetTextLine(0));

	for (unsigned long i = 1; i < textData.Count(); i++)
	{
		auto function = ChPtr::Make_S<CPUController::CPUFunctionBase>();
	}

}

void CPUController::SelectRunFunction(CPUObjectLooker& _looker)
{
	if (action != nullptr)return;

	for (auto&& function : cpuFunctions)
	{
		if (nowActionMoveTime > 0)continue;
		if (function->IsRunTest(_looker) && !function->IsUnStopFlg())
		{
			action = function;
			break;
		}
	}
}

void CPUController::RunUpdate()
{
	if (action == nullptr)return;
	action->Update(*this);
	if (!action->IsRunEnd())return;
	action = nullptr;

}

void CPUController::CPUFunctionBase::Init()
{
	comparisonOperation[ChStd::EnumCast(CPUController::ComparisonOperation::Greater)] =
		[&](const float _targetValue, const float _testValue)->ChStd::Bool {return _targetValue > _testValue; };

	comparisonOperation[ChStd::EnumCast(CPUController::ComparisonOperation::More)] =
		[&](const float _targetValue, const float _testValue)->ChStd::Bool {return _targetValue >= _testValue; };

	comparisonOperation[ChStd::EnumCast(CPUController::ComparisonOperation::Equal)] =
		[&](const float _targetValue, const float _testValue)->ChStd::Bool {return _targetValue == _testValue; };

	comparisonOperation[ChStd::EnumCast(CPUController::ComparisonOperation::Less)] =
		[&](const float _targetValue, const float _testValue)->ChStd::Bool {return _targetValue <= _testValue; };

	comparisonOperation[ChStd::EnumCast(CPUController::ComparisonOperation::Smaller)] =
		[&](const float _targetValue, const float _testValue)->ChStd::Bool {return _targetValue < _testValue; };
}

std::string CPUController::CPUFunctionBase::Serialize()
{
	std::string res = "";

	return res;
}

void CPUController::CPUFunctionBase::Deserialize(const std::string& _text)
{

}

ChStd::Bool CPUController::CPUFunctionBase::IsRunTest(CPUObjectLooker& _looker)
{
	target = _looker.GetLookTypeMechas(memberType, distanceType, damageType);

	if (target == nullptr)return false;

	auto mecha = target->otherMecha.lock();

	if (mecha == nullptr)return false;

	if (!comparisonOperation[ChStd::EnumCast(damageComparison)](mecha->GetDamage(), testDamage))return false;

	if (!comparisonOperation[ChStd::EnumCast(distanceComparison)](mecha->GetDamage(), testDistance))return false;
	
	isRunEndFlg = false;
	
	return true;
}

void CPUController::CPUFunctionBase::Update(CPUController& _controller)
{


	auto mecha = _controller.LookObj();

	if (mecha == nullptr)return;
	
	auto weapon = mecha->GetComponents<RightWeaponComponent>();



	_controller.Input(InputName::LAttack);

	isRunEndFlg = true;
}