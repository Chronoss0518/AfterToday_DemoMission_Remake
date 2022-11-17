#pragma once

#include"FunctionComponent.h"

class BoostComponent:public FunctionComponent
{
public:

	void Update()override;

	void Move()override;

	void DrawEnd()override;

private:

	void UpdateBoost(const ChLMat& _nowTargetPoster);

	void UpdateAvoid(const ChLMat& _nowTargetPoster);

public:

	inline void SetInputBoostName(const InputName _inputBoost) { boostName = _inputBoost; }

	inline void SetInputAvoidName(const InputName _inputAvoid) { avoidName = _inputAvoid; }

	void SetDirection(const ChVec3& _dir) { direction = _dir; }

	inline void SetBoostAvoidWait(const unsigned long _avoidWait)
	{
		avoidWait = avoidWait < _avoidWait ? _avoidWait : avoidWait;
	}

public:

	inline void AddBoost(ChPtr::Shared<ChCpp::FrameObject> _boost)
	{
		boostObject.push_back(_boost);
	}

	inline void AddBoostPow(const float _boostPow)
	{
		boostPow += _boostPow;
	}

	inline void AddBoostUseEnelgy(const unsigned long _boostUseEnelgy)
	{
		useBoostEnelgy += _boostUseEnelgy;
	}

	inline void AddBoostAvoidPow(const float _avoidPow)
	{
		avoidPow += _avoidPow;
	}

	inline void AddBoostAvoidUseEnelgy(const unsigned long _avoidUseEnelgy)
	{
		useAvoidEnelgy += _avoidUseEnelgy;
	}

private:

	ChVec3 direction;

	float boostPow = 0.0f;
	unsigned long useBoostEnelgy = 0;
	float avoidPow = 0.0f;
	unsigned long useAvoidEnelgy = 0;
	unsigned long avoidWait = 0;

	bool boostStartFlg = false;
	bool boostUseFlg = false;

	bool avoidUseFlg = false;
	bool avoidStartFlg = false;
	unsigned long nowAvoidWaitTime = 0;

	float nowBoostPow = 0.0f;
	float boostRotation = 0.0f;

	std::vector<ChPtr::Shared<ChCpp::FrameObject>>boostObject;

	InputName boostName = InputName::None;
	InputName avoidName = InputName::None;
};
