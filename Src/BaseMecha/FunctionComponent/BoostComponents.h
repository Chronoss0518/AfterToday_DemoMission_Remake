#pragma once

#include"BoostComponent.h"

class FrontBoostComponent :public BoostComponent
{
public:

	inline FrontBoostComponent()
	{
		SetInputBoostName(BaseMecha::InputName::FrontBoost);
		SetInputAvoidName(BaseMecha::InputName::FrontAvo);

		SetDirection(ChVec3(0.0f, 0.0f, 1.0f));
	}

};

class BackBoostComponent :public BoostComponent
{
public:

	inline BackBoostComponent()
	{
		SetInputBoostName(BaseMecha::InputName::BackBoost);
		SetInputAvoidName(BaseMecha::InputName::BackAvo);


		SetDirection(ChVec3(0.0f, 0.0f, -1.0f));
	}

};

class LeftBoostComponent :public BoostComponent
{
public:

	inline LeftBoostComponent()
	{
		SetInputBoostName(BaseMecha::InputName::LeftBoost);
		SetInputAvoidName(BaseMecha::InputName::LeftAvo);

		SetDirection(ChVec3(-1.0f, 0.0f, 0.0f));
	}

};

class RightBoostComponent :public BoostComponent
{
public:

	inline RightBoostComponent()
	{
		SetInputBoostName(BaseMecha::InputName::RightBoost);
		SetInputAvoidName(BaseMecha::InputName::RightAvo);

		SetDirection(ChVec3(1.0f, 0.0f, 0.0f));
	}

};

class UpBoostComponent :public BoostComponent
{
public:

	inline UpBoostComponent()
	{
		SetInputBoostName(BaseMecha::InputName::UpBoost);
		SetInputAvoidName(BaseMecha::InputName::UpAvo);

		SetDirection(ChVec3(0.0f, 1.0f, 0.0f));
	}

};

class DownBoostComponent :public BoostComponent
{
public:

	inline DownBoostComponent()
	{
		SetInputBoostName(BaseMecha::InputName::DownBoost);
		SetInputAvoidName(BaseMecha::InputName::DownAvo);

		SetDirection(ChVec3(0.0f, -1.0f, 0.0f));
	}

};