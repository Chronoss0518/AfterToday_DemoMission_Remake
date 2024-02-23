
#include"../BaseIncluder.h"

#include"MenuBase.h"

#define STICK_INPUT_SIZE 0.6f

void MenuBase::InitMenu(ChD3D::XInputController* _controller)
{
	afterInputMenuType.SetBitTrue(ChStd::EnumCast(ActionType::Decision));
	afterInputMenuType.SetBitTrue(ChStd::EnumCast(ActionType::Cancel));

	controller = _controller;
}

void MenuBase::UpdateFunction()
{
	UpdateController();
	UpdateKeyboard();
	UpdateMouse();

	for (unsigned char i = 0; i < ACTION_TYPE_COUNT; i++)
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

	auto&& manager = ChSystem::SysManager();

	InputTest(ActionType::Decision, manager.IsPushKey(VK_RETURN) || manager.IsPushKey(VK_SPACE));

	InputTest(ActionType::Up, manager.IsPushKey(VK_UP) || manager.IsPushKey('W'));

	InputTest(ActionType::Down, manager.IsPushKey(VK_DOWN) || manager.IsPushKey('S'));

	InputTest(ActionType::Left, manager.IsPushKey(VK_LEFT) || manager.IsPushKey('A'));

	InputTest(ActionType::Right, manager.IsPushKey(VK_RIGHT) || manager.IsPushKey('D'));
}

void MenuBase::UpdateController()
{
	if (ChPtr::NullCheck(controller))return;
	controller->Update();

	InputTest(ActionType::Decision, controller->GetAFlg());

	InputTest(ActionType::Cancel, controller->GetBFlg());
	
	InputTest(ActionType::Up, controller->GetUpFlg() || controller->GetLYStick() > STICK_INPUT_SIZE);
	
	InputTest(ActionType::Down, controller->GetDownFlg() || controller->GetLYStick() < -STICK_INPUT_SIZE);
	
	InputTest(ActionType::Left, controller->GetLeftFlg() || controller->GetLXStick() < -STICK_INPUT_SIZE);
	
	InputTest(ActionType::Right, controller->GetRightFlg() || controller->GetLXStick() > STICK_INPUT_SIZE);
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