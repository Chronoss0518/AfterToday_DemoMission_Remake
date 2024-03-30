#pragma once

namespace PartsParameterStruct
{
	struct MainData
	{
		float hardness = 0.0f;
		float mass = 0.0f;
	};

	struct EnelgyTankData
	{
		unsigned long maxEnelgy = 0;
		unsigned long chargeEnelgy = 0;
	};

	struct WalkData
	{
		float movePower = 0.0f;
		float rotatePower = 0.0f;
		float jumpPower = 0.0f;
	};

	struct BoostData
	{
		unsigned long boostUseEnelgy = 0;
		float boostPower = 0.0f;
		unsigned long avoidUseEnelgy = 0;
		float avoidPower = 0.0f;
		unsigned long avoidWait = 0;
	};

	struct WeaponData
	{
		unsigned long waitTime = 0;
	};

	struct SwordData : public WeaponData
	{
		unsigned long damageParSpeed = 0;

		unsigned long attackTime = 0;
	};

	struct GunData : public WeaponData
	{
		unsigned long fireNum = 0;
		unsigned long bulletNum = 0;
		unsigned long magazineNum = 0;
		unsigned long reloadTime = 0;
		unsigned char range = 0;
	};

}

struct PartsParameters
{
	PartsParameterStruct::MainData mainData;

	PartsParameterStruct::EnelgyTankData enelgyTankData;

	PartsParameterStruct::WalkData walkData;

	PartsParameterStruct::BoostData frontBoostData;

	PartsParameterStruct::BoostData backBoostData;

	PartsParameterStruct::BoostData rightBoostData;

	PartsParameterStruct::BoostData leftBoostData;

	PartsParameterStruct::BoostData upBoostData;

	PartsParameterStruct::BoostData downBoostData;

	std::vector<ChPtr::Shared<PartsParameterStruct::WeaponData>>weaponData;
};