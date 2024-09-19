#pragma once

namespace PartsParameterStruct
{
	struct MainData
	{
		unsigned long hardness = 0;
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
		float boostPower = 0.0f;
		unsigned long boostUseEnelgy = 0;
		float avoidPower = 0.0f;
		unsigned long avoidUseEnelgy = 0;
		unsigned long avoidWait = 0;
	};

	struct AttackBase
	{
		virtual ~AttackBase(){}
	};

	struct BulletData:AttackBase
	{
		//初速//
		float firstSpeed = 0.0f;
	};

	struct BoostBulletData :AttackBase
	{
		//ブースト点火までの時間//
		unsigned long startBoostTime = 0;

		//ブースト時間//
		unsigned long useBoostTime = 0;

		//ブーストのパワー//
		float boostPow = 0.0f;
	};

	struct HighExplosiveBulletData :AttackBase
	{
		//爆発範囲//
		float blastRange = 0.0f;
	};

	struct MissileData :AttackBase
	{
		//回転速度//
		float rotateSpeed = 0.0f;

		//見失う範囲//
		float lostRange = 0.0f;
	};

	struct AttackData
	{
		std::wstring objectName = L"";

		//威力//
		unsigned long penetration = 0;

		//衝突判定用//
		float hitSize = 0.0f;

		std::vector<ChPtr::Shared<AttackBase>>attackDataBase;
	};

	struct WeaponData
	{
		virtual ~WeaponData(){}

		std::wstring partsName = L"";
		std::wstring weaponName = L"";

		unsigned long waitTime = 0;
		std::vector<ChPtr::Shared<AttackData>>attackData;
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
		unsigned long range = 0;
	};

}

struct PartsParameters:public ChCpp::BaseComponent
{
	PartsParameters& operator = (const PartsParameters& _parts);

	PartsParameters& operator += (const PartsParameters& _parts);
	PartsParameters operator + (const PartsParameters& _parts)const;
	PartsParameters& operator -= (const PartsParameters& _parts);
	PartsParameters operator - (const PartsParameters& _parts)const;

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

