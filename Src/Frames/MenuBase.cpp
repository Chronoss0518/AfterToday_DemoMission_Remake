
#include"../BaseIncluder.h"

#include"MenuBase.h"

void MenuBase::InitMenu(ChD3D::XInputController* _controller)
{
	controllerInputData.SetBitTrue(ChStd::EnumCast(ActionType::Decision));
	keyboardInputData.SetBitTrue(ChStd::EnumCast(ActionType::Decision));
	mouseInputData.SetBitTrue(ChStd::EnumCast(ActionType::Decision));

	controller = _controller;
}

void MenuBase::UpdateFunction()
{
	UpdateKeyboard();
	UpdateMouse();
	UpdateController();

	for (unsigned char i = 0; i < ACTION_TYPE_COUNT; i++)
	{
		if (loopBreakFlg)break;
		if (!inputMenuType.GetBitFlg(i))continue;
		UpdateAction(static_cast<ActionType>(i));
	}

	loopBreakFlg = false;

	inputMenuType.SetAllDownFlg();
}

void MenuBase::UpdateKeyboard()
{

	auto&& manager = ChSystem::SysManager();

	KeyboardTest(ActionType::Decision, manager.IsPushKey(VK_RETURN) || manager.IsPushKey(VK_SPACE));

	KeyboardTest(ActionType::Up, manager.IsPushKey(VK_UP) || manager.IsPushKey('W'));

	KeyboardTest(ActionType::Down, manager.IsPushKey(VK_DOWN) || manager.IsPushKey('S'));

	KeyboardTest(ActionType::Left, manager.IsPushKey(VK_LEFT) || manager.IsPushKey('A'));

	KeyboardTest(ActionType::Right, manager.IsPushKey(VK_RIGHT) || manager.IsPushKey('D'));
}

void MenuBase::UpdateController()
{
	if (ChPtr::NullCheck(controller))return;
	controller->Update();

	ControllerTest(ActionType::Decision, controller->GetAFlg());

	ControllerTest(ActionType::Cancel, controller->GetBFlg());
	
	ControllerTest(ActionType::Up, controller->GetUpFlg() || controller->GetLYStick() > 0.3f);
	
	ControllerTest(ActionType::Down, controller->GetDownFlg() || controller->GetLYStick() < -0.3f);
	
	ControllerTest(ActionType::Left, controller->GetLeftFlg() || controller->GetLXStick() < -0.3f);
	
	ControllerTest(ActionType::Right, controller->GetRightFlg() || controller->GetLXStick() > 0.3f);

}

void MenuBase::AddActionType(ActionType _action)
{
	inputMenuType.SetBitTrue(ChStd::EnumCast(_action));
}

void MenuBase::ControllerTest(ActionType _action, bool _inputFlg)
{
	InputTest(_action, _inputFlg, controllerInputData);
}

void MenuBase::KeyboardTest(ActionType _action, bool _inputFlg)
{
	InputTest(_action, _inputFlg, keyboardInputData);
}

void MenuBase::MouseTest(ActionType _action, bool _inputFlg)
{
	InputTest(_action, _inputFlg, mouseInputData);
}

void MenuBase::InputTest(ActionType _action, bool _inputFlg, ChCpp::BitBool& _inputData)
{
	if (!_inputFlg)
	{
		_inputData.SetBitFalse(ChStd::EnumCast(_action));
		return;
	}

	if (_inputData.GetBitFlg(ChStd::EnumCast(_action)))return;

	AddActionType(_action);
	_inputData.SetBitTrue(ChStd::EnumCast(_action));
}