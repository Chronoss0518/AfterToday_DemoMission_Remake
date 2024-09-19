#pragma once

class CPUObjectLooker;
class RightWeaponComponent;
class LeftWeaponComponent;

class CPUActionBase;
class CPUWeaponSelector;
class CPUTargetSelector;
class CPUMovePositionSelector;
class CPUAttack;
class CPUAvoidTest;
class CPUMoveInput;

class ControllerBase:public ChCpp::BaseComponent
{
public:

	using InputName = BaseMecha::InputName;

public:

	virtual void Input(const InputName _inputFlgName);

	inline void SetTeamNo(unsigned long _team) { teamNo = _team; }

	inline unsigned long GetTeamNo() { return teamNo; }

	inline BaseMecha* GetBaseMecha() { return LookObj<BaseMecha>(); }

private:

	unsigned long teamNo = 0;

};

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

	InputName cursolInput[ChStd::EnumCast(CursolMoveTypeName::None)] = {InputName::None,InputName::None,InputName::None,InputName::None};
	InputName wheelInput[2]{ InputName::None,InputName::None };
	ChWin::MouseController* mouse = &ChWin::Mouse();
	float moveSensitivility = 1.0f;
	ChVec2 nowPos = ChVec2();
	ChVec2 windSize = ChVec2();
	float baseLen = 0.0f;
};

class CPUController :public ControllerBase
{
public:
	
	struct VoiceData
	{
		float size = 0.0f;
		ChVec3 position = 0.0f;
	};


	enum class ComparisonOperation : unsigned char
	{
		Greater,	//target > test//
		More,		//target >= test//
		Equal,		//target == test//
		Less,		//target <= test//
		Smaller		//target < test//
	};

public:

	void Init()override;

	void UpdateBegin()override;

	void Input(InputName _input, bool _addInputDataParFrameFlg);

public:

	inline void SetGameFrame(GameFrame* _frame) { frame = _frame; }

	inline void SetSaveFlg(const bool _flg) { saveFlg = _flg; }

	inline void SetActionMoveMaxTime(unsigned long _count) { actionMoveMaxTime = _count; }

	inline void SetActionMoveMinTime(unsigned long _count) { actionMoveMinTime = _count; }

	inline static void SetVoiceData(const VoiceData& _voice)
	{
		auto voiceData = ChPtr::Make_S<VoiceData>();
		voiceData->position = _voice.position;
		voiceData->size = _voice.size;
		GetVoiceData().push_back(voiceData);
	}

public:

	inline BaseMecha* GetBaseMecha() { return LookObj<BaseMecha>(); }

public:

	void AddCPUFunction();

public:

	void SaveCPUData(const std::wstring& _fileName);

	void LoadCPUData(const std::wstring& _fileName);

	void SampleCreate();

public:

	inline void CrearVoiceData()
	{
		GetVoiceData().clear();
	}

private:

	void UpdateEndActionMoveTime();

	void Input(InputName _input)override
	{
		ControllerBase::Input(_input);
	}

	inline static std::vector<ChPtr::Shared<VoiceData>>& GetVoiceData()
	{
		static std::vector<ChPtr::Shared<VoiceData>> voicePos;
		return voicePos;
	};

private:

	ChPtr::Shared<CPUTargetSelector> cpuTargetSelect = nullptr;
	ChPtr::Shared<CPUMovePositionSelector> cpuMovePositionSelect = nullptr;
	ChPtr::Shared<CPUWeaponSelector> cpuWeaponSelect = nullptr;
	ChPtr::Shared<CPUAttack> cpuAttack = nullptr;
	ChPtr::Shared<CPUAvoidTest> cpuAvoidTest = nullptr;
	ChPtr::Shared<CPUMoveInput>cpuMoveInput = nullptr;

	static const std::wstring cutChar;

	unsigned long actionMoveMaxTime = 5;
	unsigned long actionMoveMinTime = 2;

	unsigned long nowActionMoveTime = actionMoveMaxTime;
	unsigned long endActionMoveTime = actionMoveMaxTime;

	std::vector<InputName>inputDataParFrame;

	bool saveFlg = false;
	bool sampleFlg = false;

	GameFrame* frame = nullptr;
};

template<class CPUFunction>
class FunctionsBase
{
public:

	virtual ChPtr::Shared<ChCpp::JsonObject<wchar_t>> Serialize() = 0;

	virtual void Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject) = 0;

	inline void Add(ChPtr::Shared<CPUFunction> _target)
	{
		if (_target == nullptr)return;
		functions.push_back(_target);
	}

	inline void Remove(unsigned long _num)
	{
		if (_num >= functions.size())return;
		functions.erase(functions.begin() + _num);
	}

	inline void TargetFunctionMovePosition(unsigned long _targetNo, unsigned long _moveNo)
	{
		if (_targetNo >= functions.size() || _moveNo >= functions.size())return;
		if (_targetNo == _moveNo)return;

		bool countUpFlg = _targetNo > _moveNo;

		unsigned long loopCount = countUpFlg ? _moveNo - _targetNo : _targetNo - _moveNo;
		
		long countMoves = countUpFlg ? 1 : -1;

		ChPtr::Shared<CPUFunction> target = functions[_targetNo];

		for (unsigned long i = 0; i < loopCount; i++)
		{
			ChPtr::Shared<CPUFunction> tmp = functions[_moveNo + (i * countMoves) + countMoves];
			functions[_moveNo + (i * countMoves) + countMoves] = nullptr;
			functions[_moveNo + (i * countMoves) + countMoves] = functions[_moveNo + (i * countMoves)];
			functions[_moveNo + (i * countMoves)] = nullptr;
			functions[_moveNo + (i * countMoves)] = tmp;
		}
		functions[_moveNo] = nullptr;
		functions[_moveNo] = target;
	}

	inline void TargetFunctionChangePosition(unsigned long _targetNo, unsigned long _changeNo)
	{
		if (_targetNo >= functions.size() || _changeNo >= functions.size())return;
		if (_targetNo == _changeNo)return;

		ChPtr::Shared<CPUFunction> target = functions[_targetNo];
		functions[_targetNo] = nullptr;
		functions[_targetNo] = functions[_changeNo];
		functions[_changeNo] = nullptr;
		functions[_changeNo] = target;
	}

protected:

	std::vector<ChPtr::Shared<CPUFunction>> functions;

};