#pragma once

class ControllerBase:public ChCpp::BaseComponent
{
public:

	using InputName = BaseMecha::InputName;

public:

	void Input(const InputName _inputFlgName);

	inline void SetTeamNo(unsigned long _team) { teamNo = _team; }

	inline unsigned long GetTeamNo() { return teamNo; }


private:

	unsigned long teamNo = 0;

};

class PlayerController :public ControllerBase
{
public:

	void Init()override;

	void UpdateBegin()override;

	void Save(const std::string& _fileName);

	void Load(const std::string& _fileName);

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

	void CursolFunction(float& _value, const float _windSize, const CursolMoveTypeName _plus, const CursolMoveTypeName _minus);

	std::map<unsigned long, InputName> keyTypes;
	std::map<XInputTypeNames, InputName>controllerTypes;
	ChD3D::XInputController controller;

	InputName cursolInput[ChStd::EnumCast(CursolMoveTypeName::None)] = {InputName::None,InputName::None,InputName::None,InputName::None};
	InputName wheelInput[2]{ InputName::None,InputName::None };
	ChWin::MouseController* mouse = &ChWin::Mouse();
	float moveSensitivility = 0.3f;
	ChVec2 nowPos = ChVec2();
	ChVec2 windSize = ChVec2();
};

class CPUFunctionBase;

class CPUController :public ControllerBase
{
public:

	void Init()override;

	void UpdateBegin()override;

public:

	inline void SetGameFrame(GameFrame* _frame) { frame = _frame; }

public:

	void LoadCPUData(const std::string& _fileName);

private:

	std::vector<ChPtr::Shared<CPUFunctionBase>>cpuFunctions;
	ChPtr::Weak<CPUFunctionBase>action;

	unsigned long actionMoveTime = 100;
	unsigned long nowActionMoveTime = actionMoveTime;

	std::vector<ChPtr::Shared<ChVec3>>voicePos;

	GameFrame* frame = nullptr;
	BaseMecha* target = nullptr;
};
