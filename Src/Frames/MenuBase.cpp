
#include"../BaseIncluder.h"

#include"MenuBase.h"

#include"../Application/Application.h"

#define STICK_INPUT_SIZE 0.6f

void MenuBase::InitMenu()
{
	afterInputMenuType.SetBitTrue(ChStd::EnumCast(ActionType::Decision));
	afterInputMenuType.SetBitTrue(ChStd::EnumCast(ActionType::Cancel));
}

void MenuBase::UpdateFunction()
{
	UpdateController();

	if (!isPushControllerFlg)
	{
		UpdateKeyboard();
		UpdateMouse();
	}

	for (unsigned char i = 0; i < ChStd::EnumCast(ActionType::None); i++)
	{
		if (loopBreakFlg)break;
		if (!inputMenuType.GetBitFlg(i))continue;
		UpdateAction(static_cast<ActionType>(i));
	}

	loopBreakFlg = false;

	afterInputMenuType.SetValue(beforeInputMenuType.GetValue());
	beforeInputMenuType.SetAllDownFlg();

	inputMenuType.SetAllDownFlg();
}

void MenuBase::UpdateKeyboard()
{
	if (!useActionTestFlg)return;

	auto&& keyInput = AppIns().GetKeyInput();

	for (unsigned char i = 0; i < ChStd::EnumCast(ActionType::None); i++)
	{
		if (!keyInputActionTest[i])continue;
		bool flg = keyInputActionTest[i](keyInput);
		InputTest((ActionType)i, flg);
	}
}

void MenuBase::UpdateController()
{
	isPushControllerFlg = false;

	if (!useActionTestFlg)return;

	auto&& controller = AppIns().GetXInputController();

	for (unsigned char i = 0; i < ChStd::EnumCast(ActionType::None); i++)
	{
		if (!controllerActionTest[i])continue;
		bool flg = controllerActionTest[i](controller);
		InputTest((ActionType)i, flg);
		if (!flg)continue;
		isPushControllerFlg = true;
	}
}

void MenuBase::SetDefaultActionTest()
{

	SetActionTest([](ChWin::WinKeyInput& _input)->bool {return !_input.IsPushKey(VK_SHIFT) && (_input.IsPushKey(VK_RETURN) || _input.IsPushKey(VK_SPACE)); }, ActionType::Decision);
	SetActionTest([](ChWin::WinKeyInput& _input)->bool {return _input.IsPushKey(VK_SHIFT) && (_input.IsPushKey(VK_RETURN) || _input.IsPushKey(VK_SPACE)); }, ActionType::Cancel);
	SetActionTest([](ChWin::WinKeyInput& _input)->bool {return _input.IsPushKey(VK_UP) || _input.IsPushKey('W'); }, ActionType::Up);
	SetActionTest([](ChWin::WinKeyInput& _input)->bool {return _input.IsPushKey(VK_DOWN) || _input.IsPushKey('S'); }, ActionType::Down);
	SetActionTest([](ChWin::WinKeyInput& _input)->bool {return _input.IsPushKey(VK_LEFT) || _input.IsPushKey('A'); }, ActionType::Left);
	SetActionTest([](ChWin::WinKeyInput& _input)->bool {return _input.IsPushKey(VK_RIGHT) || _input.IsPushKey('D'); }, ActionType::Right);

	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetAFlg(); }, ActionType::Decision);
	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetBFlg(); }, ActionType::Cancel);
	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetUpFlg() || _input.GetLYStick() > STICK_INPUT_SIZE; }, ActionType::Up);
	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetDownFlg() || _input.GetLYStick() < -STICK_INPUT_SIZE; }, ActionType::Down);
	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetLeftFlg() || _input.GetLXStick() < -STICK_INPUT_SIZE; }, ActionType::Left);
	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetRightFlg() || _input.GetLXStick() > STICK_INPUT_SIZE; }, ActionType::Right);

}

void MenuBase::SetActionTest(std::function<bool(ChWin::WinKeyInput&)> _keyInputTest, ActionType _type)
{
	keyInputActionTest[ChStd::EnumCast(_type)] = _keyInputTest;
}

void MenuBase::SetActionTest(std::function<bool(ChD3D::XInputController&)> _controllerInputTest, ActionType _type)
{
	controllerActionTest[ChStd::EnumCast(_type)] = _controllerInputTest;
}

void MenuBase::AddActionType(ActionType _action)
{
	inputMenuType.SetBitTrue(ChStd::EnumCast(_action));
}

void MenuBase::InputTest(ActionType _action, bool _inputFlg)
{
	if (!_inputFlg)return;

	beforeInputMenuType.SetBitTrue(ChStd::EnumCast(_action));

	if (afterInputMenuType.GetBitFlg(ChStd::EnumCast(_action)))return;

	AddActionType(_action);
}