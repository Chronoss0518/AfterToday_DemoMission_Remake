#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../BaseMecha.h"

#include"../../Frames/GameFrame.h"

#include"ControllerBase.h"

#include"CPUFunction/CPUFunctions.h"
#include"../CPU/CPULooker.h"

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

	for (auto&& function : cpuFunctions)
	{
		if (nowActionMoveTime > 0)continue;
		if (function->Update(*this))break;
	}

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

	}

}
