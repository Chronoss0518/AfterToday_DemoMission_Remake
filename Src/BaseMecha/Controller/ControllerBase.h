#pragma once

class ControllerBase:public ChCpp::BaseComponent
{
public:

	using InputName = BaseMecha::InputName;


public:

	void Input(const InputName _inputFlgName);

private:

};

class PlayerController :public ControllerBase
{
public:

	void Init()override;

	void Update()override;

	void XInputUpdate();

private:

	enum class XInputTypeNames
	{
		A, B, X, Y,
		Up, Down, Left, Right,
		L1, L2, L3, R1, R2, R3,
		Start, Back,
		LTop, LLeft, LDown, LRight,
		RTop, RLeft, RDown, RRight
	};

	std::map<unsigned long, InputName> keyTypes;
	std::map<XInputTypeNames, InputName>controllerTypes;

	ChD3D::XInputController controller;

};

class CPUController :public ControllerBase
{
public:

	void Update()override;

};