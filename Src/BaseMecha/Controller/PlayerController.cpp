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
	auto&& windows = AppIns().GetWindow();
	windSize.h = static_cast<float>(windows.GetWindSize().h);
	windSize.w = static_cast<float>(windows.GetWindSize().w);

	keyTypes[VK_SPACE] = InputName::Up;
	keyTypes[VK_SHIFT] = InputName::Avo;
	keyTypes[VK_CONTROL] = InputName::Boost;

	keyTypes['Q'] = InputName::SetCameraCenter;
	keyTypes['E'] = InputName::MapOnOff;
	keyTypes['W'] = InputName::Front;
	keyTypes['A'] = InputName::Left;
	keyTypes['D'] = InputName::Right;
	keyTypes['S'] = InputName::Back;

	keyTypes['Z'] = InputName::WeaponUpChange;
	keyTypes['X'] = InputName::WeaponDownChange;

	keyTypes[VK_LEFT] = InputName::LeftRotation;
	keyTypes[VK_RIGHT] = InputName::RightRotation;

	//cursolInput[ChStd::EnumCast(CursolMoveTypeName::Left)] = InputName::LeftRotation;
	cursolInput[ChStd::EnumCast(AxisTypeName::Left)] = InputName::CameraLeftRotation;

	//cursolInput[ChStd::EnumCast(CursolMoveTypeName::Right)] = InputName::RightRotation;
	cursolInput[ChStd::EnumCast(AxisTypeName::Right)] = InputName::CameraRightRotation;


	cursolInput[ChStd::EnumCast(AxisTypeName::Up)] = InputName::CameraUpRotation;
	cursolInput[ChStd::EnumCast(AxisTypeName::Down)] = InputName::CameraDownRotation;


	keyTypes[VK_LBUTTON] = InputName::LWeaponAction;
	keyTypes[VK_RBUTTON] = InputName::RWeaponAction;

	keyTypes[VK_MBUTTON] = InputName::UseTargetLooker;

	//keyTypes[VK_CONTROL] = InputName::MSChange;
	//keyTypes[VK_MBUTTON] = InputName::WeaponDownChange;
	//keyTypes[VK_SCROLL] = InputName::ScopeMagnificationUp;

	controllerTypes[XInputTypeNames::A] = InputName::Up;
	controllerTypes[XInputTypeNames::B] = InputName::Boost;
	controllerTypes[XInputTypeNames::X] = InputName::Avo;
	controllerTypes[XInputTypeNames::Y] = InputName::MapOnOff;

	controllerTypes[XInputTypeNames::Up] = InputName::None;
	controllerTypes[XInputTypeNames::Down] = InputName::None;
	controllerTypes[XInputTypeNames::Left] = InputName::None;
	controllerTypes[XInputTypeNames::Right] = InputName::None;

	controllerTypes[XInputTypeNames::LTop] = InputName::Front;
	controllerTypes[XInputTypeNames::LLeft] = InputName::Left;
	controllerTypes[XInputTypeNames::LDown] = InputName::Back;
	controllerTypes[XInputTypeNames::LRight] = InputName::Right;
	controllerTypes[XInputTypeNames::RTop] = InputName::CameraDownRotation;
	controllerTypes[XInputTypeNames::RLeft] = InputName::CameraLeftRotation;
	//controllerTypes[XInputTypeNames::RLeft] = InputName::CameraLeftRotation;

	controllerTypes[XInputTypeNames::RDown] = InputName::CameraUpRotation;
	controllerTypes[XInputTypeNames::RRight] = InputName::CameraRightRotation;
	//controllerTypes[XInputTypeNames::RRight] = InputName::CameraRightRotation;

	controllerTypes[XInputTypeNames::L1] = InputName::LWeaponAction;
	controllerTypes[XInputTypeNames::L2] = InputName::LWSubFunction;
	controllerTypes[XInputTypeNames::L3] = InputName::None;
	controllerTypes[XInputTypeNames::R1] = InputName::RWeaponAction;
	controllerTypes[XInputTypeNames::R2] = InputName::RWSubFunction;
	controllerTypes[XInputTypeNames::R3] = InputName::UseTargetLooker;

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
			SetPushFlg(key.second);
		}

	}

	CursolUpdate();
}

void PlayerController::XInputUpdate()
{
	controllerPushFlg = false;
	auto&& controller = AppIns().GetXInputController();

	SetXInputFlg(controller.GetAFlg(), XInputTypeNames::A);
	SetXInputFlg(controller.GetBFlg(), XInputTypeNames::B);
	SetXInputFlg(controller.GetXFlg(), XInputTypeNames::X);
	SetXInputFlg(controller.GetYFlg(), XInputTypeNames::Y);

	SetXInputFlg(controller.GetUpFlg(), XInputTypeNames::Up);
	SetXInputFlg(controller.GetDownFlg(), XInputTypeNames::Down);
	SetXInputFlg(controller.GetLeftFlg(), XInputTypeNames::Left);
	SetXInputFlg(controller.GetRightFlg(), XInputTypeNames::Right);

	SetXInputFlg(controller.GetBackFlg(), XInputTypeNames::Back);
	SetXInputFlg(controller.GetStartFlg(), XInputTypeNames::Start);

	SetXInputFlg(controller.GetR1Flg(), XInputTypeNames::R1);
	SetXInputFlg(controller.GetR2Trigger() > DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::R2);
	SetXInputFlg(controller.GetR3Flg(), XInputTypeNames::R3);

	SetXInputFlg(controller.GetL1Flg(), XInputTypeNames::L1);
	SetXInputFlg(controller.GetL2Trigger() > DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::L2);
	SetXInputFlg(controller.GetL3Flg(), XInputTypeNames::L3);

	SetXInputFlg(controller.GetLXStick() > DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::LRight);
	SetXInputFlg(controller.GetLXStick() < -DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::LLeft);
	SetXInputFlg(controller.GetLYStick() < -DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::LDown);
	SetXInputFlg(controller.GetLYStick() > DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::LTop);

	SetXInputFlg(controller.GetRXStick() > DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::RRight);
	SetXInputFlg(controller.GetRXStick() < -DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::RLeft);
	SetXInputFlg(controller.GetRYStick() < -DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::RDown);
	SetXInputFlg(controller.GetRYStick() > DEFAULT_CONTROLLER_MOVE_SIZE, XInputTypeNames::RTop);

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

	if (_value > _removeSize)
	{
		SetPushFlg(cursolInput[ChStd::EnumCast(_plus)]);
		_value -= _removeSize;
	}

	if (_value < -_removeSize)
	{
		SetPushFlg(cursolInput[ChStd::EnumCast(_minus)]);
		_value += _removeSize;
	}
}

void PlayerController::SetXInputFlg(bool _flg,const XInputTypeNames _xinputType)
{
	if (!_flg)return;

	SetPushFlg(controllerTypes[_xinputType]);
}

void PlayerController::SetPushFlg(InputName _input)
{
	auto&& targetMecha = GetBaseMecha();

	targetMecha->SetPushFlg(_input);
}