#pragma once

class MenuBase
{
public:

	enum class ActionType : unsigned char
	{
		Decision,
		Cancel,
		Up,
		Down,
		Left,
		Right,
		//キー不特定//
		Special,
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

protected:

	void AddActionType(ActionType _action);

public:

	void AddSpecialKey(unsigned char _key);

	void SubSpecialKey(unsigned char _key);

	bool IsPushSpecialKey(unsigned char _key);

protected:

	void InputTest(ActionType _action, bool _inputFlg);

private:
 
	ChCpp::BitBool beforeInputMenuType = ChCpp::BitBool((ChStd::EnumCast(ActionType::None) / 8) + 1);
	ChCpp::BitBool afterInputMenuType = ChCpp::BitBool((ChStd::EnumCast(ActionType::None) / 8) + 1);
	ChCpp::BitBool inputMenuType = ChCpp::BitBool((ChStd::EnumCast(ActionType::None) / 8) + 1);

	bool isPushControllerFlg = false;

	bool loopBreakFlg = false;

	std::vector<unsigned char>specialKeyType;
	std::vector<unsigned char>pushSpecialKey;
};