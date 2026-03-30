#include"../../BaseIncluder.h"

#include"../../AllStruct.h"
#include"../../Frames/GameFrame.h"
#include"../BaseMecha.h"

#include"PlayerController.h"

#include"../../Application/Application.h"

#define MIN_CURSOL_LEN_PARCEC 0.01f
#define DEFAULT_CONTROLLER_MOVE_SIZE 0.3f

void PlayerController::Init()
{
	auto&& windows = *ChSystem::SysManager().GetSystem<ChSystem::WindowsW>();
	windSize.h = static_cast<float>(windows.GetWindHeight());
	windSize.w = static_cast<float>(windows.GetWindWidth());

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

	//cursolInput[ChStd::EnumCast(CursolMoveTypeName::Left)] = InputName::LeftRotation;
	cursolInput[ChStd::EnumCast(AxisTypeName::Left)] = InputName::CameraLeftRotation;

	//cursolInput[ChStd::EnumCast(CursolMoveTypeName::Right)] = InputName::RightRotation;
	cursolInput[ChStd::EnumCast(AxisTypeName::Right)] = InputName::CameraRightRotation;


	cursolInput[ChStd::EnumCast(AxisTypeName::Up)] = InputName::CameraUpRotation;
	cursolInput[ChStd::EnumCast(AxisTypeName::Down)] = InputName::CameraDownRotation;


	keyTypes[VK_LBUTTON] = InputName::LAttack;
	keyTypes[VK_RBUTTON] = InputName::RAttack;

	//keyTypes[VK_CONTROL] = InputName::MSChange;
	keyTypes[VK_MBUTTON] = InputName::WeaponDownChange;
	//keyTypes[VK_SCROLL] = InputName::ScopeMagnificationUp;

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
	//controllerTypes[XInputTypeNames::RLeft] = InputName::CameraLeftRotation;

	controllerTypes[XInputTypeNames::RDown] = InputName::CameraUpRotation;
	controllerTypes[XInputTypeNames::RRight] = InputName::RightRotation;
	//controllerTypes[XInputTypeNames::RRight] = InputName::CameraRightRotation;

	controllerTypes[XInputTypeNames::L1] = InputName::LAttack;
	controllerTypes[XInputTypeNames::L2] = InputName::LWDChange;
	controllerTypes[XInputTypeNames::L3] = InputName::LReload;
	controllerTypes[XInputTypeNames::R1] = InputName::RAttack;
	controllerTypes[XInputTypeNames::R2] = InputName::RWDChange;
	controllerTypes[XInputTypeNames::R3] = InputName::RReload;

}

void PlayerController::Release()
{
	if (ChPtr::NullCheck(frame))return;
	frame->PlayerLost();
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
		auto&& keyInput = AppIns().GetKeyInput();

		for (auto&& key : keyTypes)
		{
			if (!keyInput.IsPushKey(key.first))continue;
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

	ChVec2 vector = mouse->GetNowProPosToChVec2();
	vector.y = -vector.y;

	if (vector.x > 1.0f)vector.x = 1.0f;
	if (vector.x < -1.0f)vector.x = -1.0f;
	if (vector.y > 1.0f)vector.y = 1.0f;
	if (vector.y < -1.0f)vector.y = -1.0f;

	vector *= moveSensitivility;

	nowPos += vector;

	CursolFunction(vector.x, MIN_CURSOL_LEN_PARCEC, AxisTypeName::Right, AxisTypeName::Left);
	CursolFunction(vector.y, MIN_CURSOL_LEN_PARCEC, AxisTypeName::Up, AxisTypeName::Down);

}

void PlayerController::CursolFunction(float& _value, float _removeSize, const AxisTypeName _plus, const AxisTypeName _minus)
{
	if (controllerPushFlg)return;

	if (std::abs(_value) < _removeSize)
	{
		_value = 0.0f;
		return;
	}

	auto targetMecha = GetBaseMecha();
	if (_value > _removeSize)
	{
		targetMecha->SetPushFlg(cursolInput[ChStd::EnumCast(_plus)]);
		_value -= _removeSize;
	}

	if (_value < -_removeSize)
	{
		targetMecha->SetPushFlg(cursolInput[ChStd::EnumCast(_minus)]);
		_value += _removeSize;
	}
}

void PlayerController::SetXInputFlg(const XInputTypeNames _xinputType)
{
	auto targetMecha = GetBaseMecha();
	controllerPushFlg = true;
	targetMecha->SetPushFlg(controllerTypes[_xinputType]);
}
