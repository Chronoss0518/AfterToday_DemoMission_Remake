#pragma once

#include"ControllerBase.h"

class PlayerController :public ControllerBase
{
public:

	void Init()override;

	void UpdateBegin()override;

	void Save(const std::wstring& _fileName);

	void Load(const std::wstring& _fileName);

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
		RTop, RLeft, RDown, RRight
	};

	enum class CursolMoveTypeName : unsigned char {
		Up, Down, Left, Right, None
	};

	void XInputUpdate();

	void CursolUpdate();

	void CursolFunction(float& _value, const CursolMoveTypeName _plus, const CursolMoveTypeName _minus);

	void SetXInputFlg(const XInputTypeNames _xinputType);

	bool controllerPushFlg = false;

	std::map<unsigned long, InputName> keyTypes;
	std::map<XInputTypeNames, InputName>controllerTypes;
	ChD3D::XInputController controller;

	InputName cursolInput[ChStd::EnumCast(CursolMoveTypeName::None)] = { InputName::None,InputName::None,InputName::None,InputName::None };
	InputName wheelInput[2]{ InputName::None,InputName::None };
	ChWin::MouseController* mouse = &ChWin::Mouse();
	float moveSensitivility = 1.0f;
	ChVec2 nowPos = ChVec2();
	ChVec2 windSize = ChVec2();
	float baseLen = 0.0f;
};
