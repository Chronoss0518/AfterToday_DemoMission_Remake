#pragma once

#include"../MechaPartsObject.h"

class BoostFunction :public ChCpp::BaseComponent
{
public:

	void Update()override;

	void DrawBegin()override;

	void DrawEnd()override;

public:

	inline void SetTargetBoostObject(ChPtr::Shared<ChCpp::TransformObject<wchar_t>> _targetObject)
	{
		if (_targetObject == nullptr)return;
		targetBoostObject = _targetObject;
	}

	void SetBoostScaling();

	void SetAvoidScaling();

private:

	ChPtr::Shared<ChCpp::TransformObject<wchar_t>>targetBoostObject = nullptr;

	float nowScaling = 0.0f;
	float boostRotation = 0.0f;
};