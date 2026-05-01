#pragma once

#include"ControllerBase.h"

class CPUActionBase;
class CPUWeaponSelector;
class CPUTargetSelector;
class CPUMovePositionSelector;
class CPUAttack;
class CPUAvoidTest;
class CPUMoveInput;

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