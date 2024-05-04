
#include"../BaseIncluder.h"

#include"ParameterList.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"PartsParameters.h"

PartsParameters& PartsParameters::operator = (const PartsParameters& _parts)
{
	mainData.hardness = _parts.mainData.hardness;
	mainData.mass = _parts.mainData.mass;

	enelgyTankData.chargeEnelgy = _parts.enelgyTankData.chargeEnelgy;
	enelgyTankData.maxEnelgy = _parts.enelgyTankData.maxEnelgy;

	walkData.movePower = _parts.walkData.movePower;
	walkData.rotatePower = _parts.walkData.rotatePower;
	walkData.jumpPower = _parts.walkData.jumpPower;

	frontBoostData.boostPower = _parts.frontBoostData.boostPower;
	frontBoostData.boostUseEnelgy = _parts.frontBoostData.boostUseEnelgy;
	frontBoostData.avoidPower = _parts.frontBoostData.avoidPower;
	frontBoostData.avoidUseEnelgy = _parts.frontBoostData.avoidUseEnelgy;
	frontBoostData.avoidWait = _parts.frontBoostData.avoidWait;

	backBoostData.boostPower = _parts.backBoostData.boostPower;
	backBoostData.boostUseEnelgy = _parts.backBoostData.boostUseEnelgy;
	backBoostData.avoidPower = _parts.backBoostData.avoidPower;
	backBoostData.avoidUseEnelgy = _parts.backBoostData.avoidUseEnelgy;
	backBoostData.avoidWait = _parts.backBoostData.avoidWait;

	rightBoostData.boostPower = _parts.rightBoostData.boostPower;
	rightBoostData.boostUseEnelgy = _parts.rightBoostData.boostUseEnelgy;
	rightBoostData.avoidPower = _parts.rightBoostData.avoidPower;
	rightBoostData.avoidUseEnelgy = _parts.rightBoostData.avoidUseEnelgy;
	rightBoostData.avoidWait = _parts.rightBoostData.avoidWait;

	leftBoostData.boostPower = _parts.leftBoostData.boostPower;
	leftBoostData.boostUseEnelgy = _parts.leftBoostData.boostUseEnelgy;
	leftBoostData.avoidPower = _parts.leftBoostData.avoidPower;
	leftBoostData.avoidUseEnelgy = _parts.leftBoostData.avoidUseEnelgy;
	leftBoostData.avoidWait = _parts.leftBoostData.avoidWait;

	upBoostData.boostPower = _parts.upBoostData.boostPower;
	upBoostData.boostUseEnelgy = _parts.upBoostData.boostUseEnelgy;
	upBoostData.avoidPower = _parts.upBoostData.avoidPower;
	upBoostData.avoidUseEnelgy = _parts.upBoostData.avoidUseEnelgy;
	upBoostData.avoidWait = _parts.upBoostData.avoidWait;

	downBoostData.boostPower = _parts.downBoostData.boostPower;
	downBoostData.boostUseEnelgy = _parts.downBoostData.boostUseEnelgy;
	downBoostData.avoidPower = _parts.downBoostData.avoidPower;
	downBoostData.avoidUseEnelgy = _parts.downBoostData.avoidUseEnelgy;
	downBoostData.avoidWait = _parts.downBoostData.avoidWait;

	if (!weaponData.empty())
		weaponData.clear();

	for (auto&& weapon : _parts.weaponData)
	{
		weaponData.push_back(weapon);
	}

	return *this;
}

PartsParameters& PartsParameters::operator += (const PartsParameters& _parts)
{
	mainData.hardness += _parts.mainData.hardness;
	mainData.mass += _parts.mainData.mass;

	enelgyTankData.chargeEnelgy += _parts.enelgyTankData.chargeEnelgy;
	enelgyTankData.maxEnelgy += _parts.enelgyTankData.maxEnelgy;

	walkData.movePower += _parts.walkData.movePower;
	walkData.rotatePower += _parts.walkData.rotatePower;
	walkData.jumpPower += _parts.walkData.jumpPower;

	frontBoostData.boostPower += _parts.frontBoostData.boostPower;
	frontBoostData.boostUseEnelgy += _parts.frontBoostData.boostUseEnelgy;
	frontBoostData.avoidPower += _parts.frontBoostData.avoidPower;
	frontBoostData.avoidUseEnelgy += _parts.frontBoostData.avoidUseEnelgy;
	frontBoostData.avoidWait += _parts.frontBoostData.avoidWait;

	backBoostData.boostPower += _parts.backBoostData.boostPower;
	backBoostData.boostUseEnelgy += _parts.backBoostData.boostUseEnelgy;
	backBoostData.avoidPower += _parts.backBoostData.avoidPower;
	backBoostData.avoidUseEnelgy += _parts.backBoostData.avoidUseEnelgy;
	backBoostData.avoidWait += _parts.backBoostData.avoidWait;

	rightBoostData.boostPower += _parts.rightBoostData.boostPower;
	rightBoostData.boostUseEnelgy += _parts.rightBoostData.boostUseEnelgy;
	rightBoostData.avoidPower += _parts.rightBoostData.avoidPower;
	rightBoostData.avoidUseEnelgy += _parts.rightBoostData.avoidUseEnelgy;
	rightBoostData.avoidWait += _parts.rightBoostData.avoidWait;

	leftBoostData.boostPower += _parts.leftBoostData.boostPower;
	leftBoostData.boostUseEnelgy += _parts.leftBoostData.boostUseEnelgy;
	leftBoostData.avoidPower += _parts.leftBoostData.avoidPower;
	leftBoostData.avoidUseEnelgy += _parts.leftBoostData.avoidUseEnelgy;
	leftBoostData.avoidWait += _parts.leftBoostData.avoidWait;

	upBoostData.boostPower += _parts.upBoostData.boostPower;
	upBoostData.boostUseEnelgy += _parts.upBoostData.boostUseEnelgy;
	upBoostData.avoidPower += _parts.upBoostData.avoidPower;
	upBoostData.avoidUseEnelgy += _parts.upBoostData.avoidUseEnelgy;
	upBoostData.avoidWait += _parts.upBoostData.avoidWait;

	downBoostData.boostPower += _parts.downBoostData.boostPower;
	downBoostData.boostUseEnelgy += _parts.downBoostData.boostUseEnelgy;
	downBoostData.avoidPower += _parts.downBoostData.avoidPower;
	downBoostData.avoidUseEnelgy += _parts.downBoostData.avoidUseEnelgy;
	downBoostData.avoidWait += _parts.downBoostData.avoidWait;

	for (auto&& weapon : _parts.weaponData)
	{
		weaponData.push_back(weapon);
	}

	return *this;
}

PartsParameters PartsParameters::operator + (const PartsParameters& _parts)const
{
	PartsParameters res = *this;
	res += _parts;
	return res;
}

PartsParameters& PartsParameters::operator -= (const PartsParameters& _parts)
{
	mainData.hardness -= _parts.mainData.hardness;
	mainData.mass -= _parts.mainData.mass;

	enelgyTankData.chargeEnelgy -= _parts.enelgyTankData.chargeEnelgy;
	enelgyTankData.maxEnelgy -= _parts.enelgyTankData.maxEnelgy;

	walkData.movePower -= _parts.walkData.movePower;
	walkData.rotatePower -= _parts.walkData.rotatePower;
	walkData.jumpPower -= _parts.walkData.jumpPower;

	frontBoostData.boostPower -= _parts.frontBoostData.boostPower;
	frontBoostData.boostUseEnelgy -= _parts.frontBoostData.boostUseEnelgy;
	frontBoostData.avoidPower -= _parts.frontBoostData.avoidPower;
	frontBoostData.avoidUseEnelgy -= _parts.frontBoostData.avoidUseEnelgy;
	frontBoostData.avoidWait -= _parts.frontBoostData.avoidWait;

	backBoostData.boostPower -= _parts.backBoostData.boostPower;
	backBoostData.boostUseEnelgy -= _parts.backBoostData.boostUseEnelgy;
	backBoostData.avoidPower -= _parts.backBoostData.avoidPower;
	backBoostData.avoidUseEnelgy -= _parts.backBoostData.avoidUseEnelgy;
	backBoostData.avoidWait -= _parts.backBoostData.avoidWait;

	rightBoostData.boostPower -= _parts.rightBoostData.boostPower;
	rightBoostData.boostUseEnelgy -= _parts.rightBoostData.boostUseEnelgy;
	rightBoostData.avoidPower -= _parts.rightBoostData.avoidPower;
	rightBoostData.avoidUseEnelgy -= _parts.rightBoostData.avoidUseEnelgy;
	rightBoostData.avoidWait -= _parts.rightBoostData.avoidWait;

	leftBoostData.boostPower -= _parts.leftBoostData.boostPower;
	leftBoostData.boostUseEnelgy -= _parts.leftBoostData.boostUseEnelgy;
	leftBoostData.avoidPower -= _parts.leftBoostData.avoidPower;
	leftBoostData.avoidUseEnelgy -= _parts.leftBoostData.avoidUseEnelgy;
	leftBoostData.avoidWait -= _parts.leftBoostData.avoidWait;

	upBoostData.boostPower -= _parts.upBoostData.boostPower;
	upBoostData.boostUseEnelgy -= _parts.upBoostData.boostUseEnelgy;
	upBoostData.avoidPower -= _parts.upBoostData.avoidPower;
	upBoostData.avoidUseEnelgy -= _parts.upBoostData.avoidUseEnelgy;
	upBoostData.avoidWait -= _parts.upBoostData.avoidWait;

	downBoostData.boostPower -= _parts.downBoostData.boostPower;
	downBoostData.boostUseEnelgy -= _parts.downBoostData.boostUseEnelgy;
	downBoostData.avoidPower -= _parts.downBoostData.avoidPower;
	downBoostData.avoidUseEnelgy -= _parts.downBoostData.avoidUseEnelgy;
	downBoostData.avoidWait -= _parts.downBoostData.avoidWait;

	if (!weaponData.empty())
		weaponData.clear();

	return *this;
}

PartsParameters PartsParameters::operator - (const PartsParameters& _parts)const
{
	PartsParameters res = *this;
	res -= _parts;
	return res;
}
