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

	inline void SetProjectionMatrix(const ChLMat& _mat) { proMat = _mat; }

	inline void SetViewMatrix(const ChLMat& _mat) { viewMat = _mat; }

public:
	//一番近い敵//
	ChPtr::Weak<BaseMecha> FindNearEnemyMecha() { return nearEnemy; }

	//一番近くダメージが多い敵//
	ChPtr::Weak<BaseMecha> FindNearManyDamageEnemyMecha() { return nearManyDamageEnemy; }

	//一番近くダメージが少ない敵//
	ChPtr::Weak<BaseMecha> FindNearFewDamageEnemyMecha() { return nearFewDamageEnemy; }

	//一番遠い敵//
	ChPtr::Weak<BaseMecha> FindFarEnemyMecha() { return farEnemy; }

	//一番遠くダメージが多い敵//
	ChPtr::Weak<BaseMecha> FindFarManyDamageEnemyMecha() { return farManyDamageEnemy; }

	//一番遠くダメージが少ない敵//
	ChPtr::Weak<BaseMecha> FindFarFewDamageEnemyMecha() { return farFewDamageEnemy; }

public:

	void LoadCPUData(const std::string& _fileName);

private:

	void FindMecha();

	void MenyDamageTest(ChPtr::Weak<BaseMecha>& _base, ChPtr::Weak<BaseMecha>& _target);

	void FewDamageTest(ChPtr::Weak<BaseMecha>& _base, ChPtr::Weak<BaseMecha>& _target);

	std::vector<ChPtr::Shared<CPUFunctionBase>>cpuFunctions;
	ChPtr::Weak<CPUFunctionBase>action;

	unsigned long actionMoveTime = 100;
	unsigned long nowActionMoveTime = actionMoveTime;

	ChLMat viewMat;
	ChLMat proMat;

	ChPtr::Weak<BaseMecha> nearEnemy;
	ChPtr::Weak<BaseMecha> nearManyDamageEnemy;
	ChPtr::Weak<BaseMecha> nearFewDamageEnemy;
	ChPtr::Weak<BaseMecha> farEnemy;
	ChPtr::Weak<BaseMecha> farManyDamageEnemy;
	ChPtr::Weak<BaseMecha> farFewDamageEnemy;
	ChPtr::Weak<BaseMecha> leaderMecha;
	ChPtr::Weak<BaseMecha> nearMember;

	std::vector<ChPtr::Shared<ChVec3>>voicePos;

	GameFrame* frame = nullptr;
	BaseMecha* target = nullptr;
};
