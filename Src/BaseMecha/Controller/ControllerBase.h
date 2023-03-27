#pragma once

class CPUObjectLooker;

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

class CPUController :public ControllerBase
{
public:

	enum class ComparisonOperation : unsigned char
	{
		Greater,	//1 > 2//
		More,		//1 >= 2//
		Equal,		//1 == 2//
		Less,		//1 <= 2//
		Smaller		//1 < 2//
	};

public:

	void Init()override;

	void UpdateBegin()override;

public:

	inline void SetGameFrame(GameFrame* _frame) { frame = _frame; }

	inline void SetSaveFlg(const ChStd::Bool _flg) { saveFlg = _flg; }

public:

	void SaveCPUData(const std::string& _fileName);

	void LoadCPUData(const std::string& _fileName);

private:

	void SelectRunFunction(CPUObjectLooker& _looker);

	void RunUpdate();

private:

	class CPUFunctionBase;

	std::vector<ChPtr::Shared<CPUFunctionBase>>cpuFunctions;

	ChPtr::Shared<CPUFunctionBase>action;

	unsigned long actionMoveTime = 100;
	unsigned long nowActionMoveTime = actionMoveTime;

	ChVec3 lastPos = ChVec3();

	std::vector<ChPtr::Shared<ChVec3>>voicePos;

	ChStd::Bool saveFlg = false;

	GameFrame* frame = nullptr;
	BaseMecha* target = nullptr;
};
