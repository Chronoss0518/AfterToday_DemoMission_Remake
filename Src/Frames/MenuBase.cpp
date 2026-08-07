
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

	auto&& keyInput = AppIns().GetKeyInput();

	for (unsigned char i = 0; i < ChStd::EnumCast(ActionType::None); i++)
	{
		if (!keyInputActionTest[i])continue;
		bool flg = keyInputActionTest[i](keyInput);
		InputTest((ActionType)i, flg);
	}

	return;

	pushSpecialKey.clear();

	for (size_t i = 0; i < specialKeyType.size(); i++)
	{
		bool flg = keyInput.IsPushKey(specialKeyType[i]);
		InputTest(ActionType::Special1, flg);
		if (!flg)continue;
		pushSpecialKey.push_back(specialKeyType[i]);
		return;
	}

	InputTest(ActionType::Decision,!keyInput.IsPushKey(VK_SHIFT) && (keyInput.IsPushKey(VK_RETURN) || keyInput.IsPushKey(VK_SPACE)));

	InputTest(ActionType::Cancel, keyInput.IsPushKey(VK_SHIFT) && (keyInput.IsPushKey(VK_RETURN) || keyInput.IsPushKey(VK_SPACE)));

	InputTest(ActionType::Up, keyInput.IsPushKey(VK_UP) || keyInput.IsPushKey('W'));

	InputTest(ActionType::Down, keyInput.IsPushKey(VK_DOWN) || keyInput.IsPushKey('S'));

	InputTest(ActionType::Left, keyInput.IsPushKey(VK_LEFT) || keyInput.IsPushKey('A'));

	InputTest(ActionType::Right, keyInput.IsPushKey(VK_RIGHT) || keyInput.IsPushKey('D'));
}

void MenuBase::UpdateController()
{
	isPushControllerFlg = false;

	auto&& controller = AppIns().GetXInputController();

	for (unsigned char i = 0; i < ChStd::EnumCast(ActionType::None); i++)
	{
		if (!controllerActionTest[i])continue;
		bool flg = controllerActionTest[i](controller);
		InputTest((ActionType)i, flg);
		if (!flg)continue;
		isPushControllerFlg = true;
	}

	return;

	if (specialKeyMask > 0)
	{
		bool flg = controller.GetFlgs(specialKeyMask);
		InputTest(ActionType::Special1, flg);
		if (flg)return;
	}

	InputTest(ActionType::Decision, controller.GetAFlg());
	isPushControllerFlg = isPushControllerFlg || controller.GetAFlg();

	InputTest(ActionType::Cancel, controller.GetBFlg());
	isPushControllerFlg = isPushControllerFlg || controller.GetBFlg();

	InputTest(ActionType::Up, controller.GetUpFlg() || controller.GetLYStick() > STICK_INPUT_SIZE);
	isPushControllerFlg = isPushControllerFlg || controller.GetUpFlg() || controller.GetLYStick() > STICK_INPUT_SIZE;
	
	InputTest(ActionType::Down, controller.GetDownFlg() || controller.GetLYStick() < -STICK_INPUT_SIZE);
	isPushControllerFlg = isPushControllerFlg || controller.GetDownFlg() || controller.GetLYStick() < -STICK_INPUT_SIZE;
	
	InputTest(ActionType::Left, controller.GetLeftFlg() || controller.GetLXStick() < -STICK_INPUT_SIZE);
	isPushControllerFlg = isPushControllerFlg || controller.GetLeftFlg() || controller.GetLXStick() < -STICK_INPUT_SIZE;
	
	InputTest(ActionType::Right, controller.GetRightFlg() || controller.GetLXStick() > STICK_INPUT_SIZE);
	isPushControllerFlg = isPushControllerFlg || controller.GetRightFlg() || controller.GetLXStick() > STICK_INPUT_SIZE;

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
	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetUpFlg() || _input.GetLYStick() > STICK_INPUT_SIZE); }, ActionType::Up);
	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetDownFlg() || _input.GetLYStick() < -STICK_INPUT_SIZE); }, ActionType::Down);
	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetLeftFlg() || _input.GetLXStick() < -STICK_INPUT_SIZE); }, ActionType::Left);
	SetActionTest([](ChD3D::XInputController& _input)->bool {return _input.GetRightFlg() || _input.GetLXStick() > STICK_INPUT_SIZE); }, ActionType::Right);

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

void MenuBase::AddSpecialKey(unsigned char _key)
{
	for (size_t i = 0; i < specialKeyType.size(); i++)
	{
		if (specialKeyType[i] == _key)
			return;
	}

	specialKeyType.push_back(_key);
}

void MenuBase::SubSpecialKey(unsigned char _key)
{
	for (size_t i = 0; i < specialKeyType.size(); i++)
	{
		if (specialKeyType[i] != _key)
			continue;

		specialKeyType.erase(specialKeyType.begin() + i);
		return;
	}
}

bool MenuBase::IsPushSpecialKey(unsigned char _key)
{
	for (size_t i = 0; i < pushSpecialKey.size(); i++)
	{
		if (specialKeyType[i] == _key)
			return true;
	}

	return false;
}

void MenuBase::InputTest(ActionType _action, bool _inputFlg)
{
	if (!_inputFlg)return;

	beforeInputMenuType.SetBitTrue(ChStd::EnumCast(_action));

	if (afterInputMenuType.GetBitFlg(ChStd::EnumCast(_action)))return;

	AddActionType(_action);
}