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

	void SetActionTest(std::function<bool(ChWin::WinKeyInput&)> _keyInputTest, ActionType _type);

	void SetActionTest(std::function<bool(ChD3D::XInputController&)> _controllerInputTest, ActionType _type);

protected:

	void AddActionType(ActionType _action);

public:

	void AddSpecialKey(unsigned char _key);

	inline void AddSpecialKeyMask(unsigned short _mask) { specialKeyMask |= _mask; }

	void SubSpecialKey(unsigned char _key);

	inline void SubSpecialKeyMask(unsigned short _mask) { specialKeyMask = (specialKeyMask & (0xffff - _mask)); }

	bool IsPushSpecialKey(unsigned char _key);

protected:

	void InputTest(ActionType _action, bool _inputFlg);

private:
 
	ChCpp::BitBool beforeInputMenuType = ChCpp::BitBool((ChStd::EnumCast(ActionType::None) / 8) + 1);
	ChCpp::BitBool afterInputMenuType = ChCpp::BitBool((ChStd::EnumCast(ActionType::None) / 8) + 1);
	ChCpp::BitBool inputMenuType = ChCpp::BitBool((ChStd::EnumCast(ActionType::None) / 8) + 1);

	std::function<bool(ChWin::WinKeyInput&)>keyInputActionTest[ChStd::EnumCast(ActionType::None)];
	std::function<bool(ChD3D::XInputController&)>controllerActionTest[ChStd::EnumCast(ActionType::None)];

	bool isPushControllerFlg = false;

	bool loopBreakFlg = false;

	std::vector<unsigned char>specialKeyType;
	std::vector<unsigned char>pushSpecialKey;

	unsigned short specialKeyMask = 0;
};