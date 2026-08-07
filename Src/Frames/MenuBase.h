#pragma once

class MenuBase
{
public:

	enum class ActionType : unsigned char
	{
		//特殊なアクション//
		Special1,
		Special2,
		Special3,
		//基本的なアクション//
		Decision,
		Cancel,
		Up,
		Down,
		Left,
		Right,
		//キー入力無し//
		None,
	};

protected:

	void InitMenu();

protected:

	inline void SetLoopBreakTrue() { loopBreakFlg = true; }

protected:

	virtual void UpdateFunction();

	virtual void UpdateAction(ActionType _action) = 0;

	virtual void UpdateMouse() = 0;

private:

	void UpdateKeyboard();

	void UpdateController();

public:

	void SetDefaultActionTest();

	void SetUseKeyInputActionTestFlg(bool _flg) { useKeyInputActionTestFlg = _flg; }

	void SetActionTest(std::function<bool(ChWin::WinKeyInput&)> _keyInputTest, ActionType _type);

	void SetUseControllerActionTestFlg(bool _flg) { useControllerActionTestFlg = _flg; }

	void SetActionTest(std::function<bool(ChD3D::XInputController&)> _controllerInputTest, ActionType _type);

protected:

	void AddActionType(ActionType _action);

public:

	void InputTest(ActionType _action, bool _inputFlg);

private:
 
	ChCpp::BitBool beforeInputMenuType = ChCpp::BitBool((ChStd::EnumCast(ActionType::None) / 8) + 1);
	ChCpp::BitBool afterInputMenuType = ChCpp::BitBool((ChStd::EnumCast(ActionType::None) / 8) + 1);
	ChCpp::BitBool inputMenuType = ChCpp::BitBool((ChStd::EnumCast(ActionType::None) / 8) + 1);

	bool useKeyInputActionTestFlg = true;
	std::function<bool(ChWin::WinKeyInput&)>keyInputActionTest[ChStd::EnumCast(ActionType::None)];

	bool useControllerActionTestFlg = true;
	std::function<bool(ChD3D::XInputController&)>controllerActionTest[ChStd::EnumCast(ActionType::None)];

	bool isPushControllerFlg = false;

	bool loopBreakFlg = false;
};