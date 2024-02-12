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

	void ControllerTest(ActionType _action,bool _inputFlg);

	void KeyboardTest(ActionType _action,bool _inputFlg);

	void MouseTest(ActionType _action,bool _inputFlg);


private:

	void InputTest(ActionType _action, bool _inputFlg, ChCpp::BitBool& _inputData);

private:
 
	ChCpp::BitBool inputMenuType;
	ChCpp::BitBool controllerInputData;
	ChCpp::BitBool keyboardInputData;
	ChCpp::BitBool mouseInputData;
	ChD3D::XInputController* controller = nullptr;

	bool loopBreakFlg = false;

};