#pragma once

#include"ControllerBase.h"

class GameFrame;

class PlayerController :public ControllerBase
{
public:

	void Init()override;

	void Release()override;

	void UpdateBegin()override;

	void Save(const std::wstring& _fileName);

	void Load(const std::wstring& _fileName);

	void SetGameFrame(GameFrame* _frame) { frame = _frame; }

	void ChangeKey(const InputName _inputType);

	void ChangeControllerKey(const InputName _inputType);

private:

	enum class XInputTypeNames : unsigned char
	{
		A, B, X, Y,
		Up, Down, Left, Right,
		L1, L2, L3, R1, R2, R3,
		Start, Back,
		LTop, LLeft, LDown, LRight,
		RTop, RLeft, RDown, RRight,
		None
	};

	enum class AxisTypeName : unsigned char {
		Up, Down, Left, Right, None
	};

	struct AxisTypeInput
	{
		InputName inputNames[ChStd::EnumCast(AxisTypeName::None)];
	};

private:

	void XInputUpdate();

	void CursolUpdate();

	void AxisFunction(float& _value, float _minSize, float _removePar,const AxisTypeName _plus, const AxisTypeName _minus);

	void SetXInputFlg(bool _flg,const XInputTypeNames _xinputType);

	void SetPushFlg(InputName _input);

private:

	bool controllerPushFlg = false;

	std::map<unsigned char, InputName> keyTypes;
	std::map<XInputTypeNames, InputName>controllerTypes;

	InputName cursolInput[ChStd::EnumCast(AxisTypeName::None)] = { InputName::None,InputName::None,InputName::None,InputName::None };
	InputName wheelInput[ChStd::EnumCast(AxisTypeName::None)]{ InputName::None,InputName::None,InputName::None,InputName::None };

	ChWin::MouseController* mouse = &ChWin::Mouse();
	float moveSensitivility = 1.0f;
	ChVec2 nowPos = ChVec2();
	ChVec2 windSize = ChVec2();

	GameFrame* frame = nullptr;
};
