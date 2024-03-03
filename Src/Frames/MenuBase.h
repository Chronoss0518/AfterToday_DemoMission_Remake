#pragma once

class MenuBase
{
public:

	static constexpr unsigned char ACTION_TYPE_COUNT = 6;
	enum class ActionType : unsigned char
	{
		Decision,
		Cancel,
		Up,
		Down,
		Left,
		Right,
	};

protected:

	void InitMenu(ChD3D::XInputController* _controller);

protected:

	inline void SetLoopBreakTrue() { loopBreakFlg = true; }

protected:

	void UpdateFunction();

	virtual void UpdateAction(ActionType _action) = 0;

	virtual void UpdateMouse() = 0;

private:

	void UpdateKeyboard();

	void UpdateController();

protected:

	void AddActionType(ActionType _action);

protected:

	void InputTest(ActionType _action, bool _inputFlg);

private:
 
	ChCpp::BitBool beforeInputMenuType;
	ChCpp::BitBool afterInputMenuType;
	ChCpp::BitBool inputMenuType;
	ChD3D::XInputController* controller = nullptr;

	bool isPushControllerFlg = false;

	bool loopBreakFlg = false;

};