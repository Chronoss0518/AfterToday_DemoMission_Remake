#pragma once

#include"FunctionComponent.h"

class BoostComponent:public FunctionComponent
{
private:

	struct Data;
	struct BoostData;
	struct AvoidData;

public:

	BoostComponent();

public:

	void ClearBoostAvoidWait(InputName _avoidType);

public:

	void Update()override;

	void BoostDrawBegin();

	void BoostDrawEnd();

private:

	void UpdateInputFunction(Data& _data);

	void UpdateModelFunction(Data& _data);

	void UpdateBoost(Data& _data, const ChLMat& _nowTargetPoster);

	void UpdateAvoid(Data& _data, const ChLMat& _nowTargetPoster);

public:

	void SetBoostAvoidWait(const unsigned long _avoidWait, InputName _avoidType);

public:

	float GetAvoidMovePow(InputName _avoidType);

	unsigned long GetAvoidUseEnelgy(InputName _avoidType);

	unsigned long GetAvoidUseWaitTime(InputName _avoidType);

	float GetBoostMovePow(InputName _avoidType);

	unsigned long GetBoostUseEnelgy(InputName _avoidType);

private:

	unsigned long GetUseEnelgy(AvoidData& _data);

	unsigned long GetUseEnelgy(BoostData& _data, bool _avoidFlg);

public:

	void AddBoostWhereAvoidName(ChPtr::Shared<ChCpp::FrameObject> _boost, InputName _avoidType);

	void AddBoostWhereBoostName(ChPtr::Shared<ChCpp::FrameObject> _boost, InputName _boostType);

	void AddBoostPow(const float _boostPow, InputName _boostType);

	void AddBoostUseEnelgy(const unsigned long _boostUseEnelgy, InputName _boostType);

	void AddBoostAvoidPow(const float _avoidPow, InputName _avoidType);

	void AddBoostAvoidUseEnelgy(const unsigned long _avoidUseEnelgy, InputName _avoidType);

public:

	void SubBoostWhereAvoidName(ChPtr::Shared<ChCpp::FrameObject> _boost, InputName _avoidType);
		 
	void SubBoostWhereBoostName(ChPtr::Shared<ChCpp::FrameObject> _boost, InputName _boostType);
		 
	void SubBoostPow(const float _boostPow, InputName _boostType);
		 
	void SubBoostUseEnelgy(const unsigned long _boostUseEnelgy, InputName _boostType);
		 
	void SubBoostAvoidPow(const float _avoidPow, InputName _avoidType);
		 
	void SubBoostAvoidUseEnelgy(const unsigned long _avoidUseEnelgy, InputName _avoidType);

private:

	static float& GetBoostUpPowerRegister() { static float regist = 0.3f; return regist; }

	enum class BoostDirection
	{
		Front,
		Back,
		Right,
		Left,
		Up,
		Down
	};

	struct BoostData
	{

		InputName name;
		float pow = 0.0f;
		unsigned long useEnelgy = 0;

		bool testUseFlg = false;

	};

	struct AvoidData
	{

		InputName name;
		float pow = 0.0f;
		unsigned long useEnelgy = 0;
		unsigned long wait = 0;

		bool useFlg = false;
		unsigned long nowWaitTime = 0;

		bool testUseFlg = false;

	};

	struct Data
	{
		ChVec3 direction;

		BoostData boost;
		AvoidData avoid;

		float nowBoostPow = 0.0f;

		std::vector<ChPtr::Shared<ChCpp::FrameObject>>boostObject;

	};

	float boostRotation = 0.0f;

	Data data[6];

};
