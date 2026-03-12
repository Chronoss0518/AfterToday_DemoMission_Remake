#pragma once

#include"FunctionComponent.h"


class EnergyComponent;
class BoostBrust;

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

	void UpdateBoost(Data& _data, const ChLMat& _nowTargetPoster, EnergyComponent& _energy);

	void UpdateAvoid(Data& _data, const ChLMat& _nowTargetPoster, EnergyComponent& _energy);

public:

	void SetBoostAvoidWait(const unsigned long _avoidWait, InputName _avoidType);

private:

	unsigned long GetUseEnergy(AvoidData& _data);

	unsigned long GetUseEnergy(BoostData& _data, bool _avoidFlg);

public:

	void AddBoostWhereAvoidName(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _boost, InputName _avoidType);

	void AddBoostWhereBoostName(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _boost, InputName _boostType);

	void AddBoostPow(const float _boostPow, InputName _boostType);

	void AddBoostUseEnergy(const unsigned long _boostUseEnergy, InputName _boostType);

	void AddBoostAvoidPow(const float _avoidPow, InputName _avoidType);

	void AddBoostAvoidUseEnergy(const unsigned long _avoidUseEnergy, InputName _avoidType);

public:

	void SubBoostWhereAvoidName(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _boost, InputName _avoidType);
		 
	void SubBoostWhereBoostName(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _boost, InputName _boostType);
		 
	void SubBoostPow(const float _boostPow, InputName _boostType);
		 
	void SubBoostUseEnergy(const unsigned long _boostUseEnergy, InputName _boostType);
		 
	void SubBoostAvoidPow(const float _avoidPow, InputName _avoidType);
		 
	void SubBoostAvoidUseEnergy(const unsigned long _avoidUseEnergy, InputName _avoidType);

private:

	static float& GetBoostUpPowerRegister() { static float regist = 0.3f; return regist; }

	enum class BoostDirection : unsigned char
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
		unsigned long useEnergy = 0;

		bool testUseFlg = false;

	};

	struct AvoidData
	{

		InputName name;
		float pow = 0.0f;
		unsigned long useEnergy = 0;
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

		std::vector<ChPtr::Shared<ChCpp::FrameObject<wchar_t>>>boostObject;

	};

	float boostRotation = 0.0f;

	Data data[6];

	std::vector<ChPtr::Shared<BoostBrust>>boostBrustList;
};
