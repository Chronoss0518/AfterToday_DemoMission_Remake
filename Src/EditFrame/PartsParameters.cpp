
#include"../BaseIncluder.h"

#include"ParameterList.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"PartsParameters.h"

PartsParameters& PartsParameters::operator = (const PartsParameters& _parts)
{
	mainData.hardness = _parts.mainData.hardness;
	mainData.mass = _parts.mainData.mass;

	energyTankData.chargeEnergy = _parts.energyTankData.chargeEnergy;
	energyTankData.maxEnergy = _parts.energyTankData.maxEnergy;

	walkData.movePower = _parts.walkData.movePower;
	walkData.rotatePower = _parts.walkData.rotatePower;
	walkData.jumpPower = _parts.walkData.jumpPower;

	frontBoostData.boostPower = _parts.frontBoostData.boostPower;
	frontBoostData.boostUseEnergy = _parts.frontBoostData.boostUseEnergy;
	frontBoostData.avoidPower = _parts.frontBoostData.avoidPower;
	frontBoostData.avoidUseEnergy = _parts.frontBoostData.avoidUseEnergy;
	frontBoostData.avoidWait = _parts.frontBoostData.avoidWait;

	backBoostData.boostPower = _parts.backBoostData.boostPower;
	backBoostData.boostUseEnergy = _parts.backBoostData.boostUseEnergy;
	backBoostData.avoidPower = _parts.backBoostData.avoidPower;
	backBoostData.avoidUseEnergy = _parts.backBoostData.avoidUseEnergy;
	backBoostData.avoidWait = _parts.backBoostData.avoidWait;

	rightBoostData.boostPower = _parts.rightBoostData.boostPower;
	rightBoostData.boostUseEnergy = _parts.rightBoostData.boostUseEnergy;
	rightBoostData.avoidPower = _parts.rightBoostData.avoidPower;
	rightBoostData.avoidUseEnergy = _parts.rightBoostData.avoidUseEnergy;
	rightBoostData.avoidWait = _parts.rightBoostData.avoidWait;

	leftBoostData.boostPower = _parts.leftBoostData.boostPower;
	leftBoostData.boostUseEnergy = _parts.leftBoostData.boostUseEnergy;
	leftBoostData.avoidPower = _parts.leftBoostData.avoidPower;
	leftBoostData.avoidUseEnergy = _parts.leftBoostData.avoidUseEnergy;
	leftBoostData.avoidWait = _parts.leftBoostData.avoidWait;

	upBoostData.boostPower = _parts.upBoostData.boostPower;
	upBoostData.boostUseEnergy = _parts.upBoostData.boostUseEnergy;
	upBoostData.avoidPower = _parts.upBoostData.avoidPower;
	upBoostData.avoidUseEnergy = _parts.upBoostData.avoidUseEnergy;
	upBoostData.avoidWait = _parts.upBoostData.avoidWait;

	downBoostData.boostPower = _parts.downBoostData.boostPower;
	downBoostData.boostUseEnergy = _parts.downBoostData.boostUseEnergy;
	downBoostData.avoidPower = _parts.downBoostData.avoidPower;
	downBoostData.avoidUseEnergy = _parts.downBoostData.avoidUseEnergy;
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

	energyTankData.chargeEnergy += _parts.energyTankData.chargeEnergy;
	energyTankData.maxEnergy += _parts.energyTankData.maxEnergy;

	walkData.movePower += _parts.walkData.movePower;
	walkData.rotatePower += _parts.walkData.rotatePower;
	walkData.jumpPower += _parts.walkData.jumpPower;

	frontBoostData.boostPower += _parts.frontBoostData.boostPower;
	frontBoostData.boostUseEnergy += _parts.frontBoostData.boostUseEnergy;
	frontBoostData.avoidPower += _parts.frontBoostData.avoidPower;
	frontBoostData.avoidUseEnergy += _parts.frontBoostData.avoidUseEnergy;
	frontBoostData.avoidWait += _parts.frontBoostData.avoidWait;

	backBoostData.boostPower += _parts.backBoostData.boostPower;
	backBoostData.boostUseEnergy += _parts.backBoostData.boostUseEnergy;
	backBoostData.avoidPower += _parts.backBoostData.avoidPower;
	backBoostData.avoidUseEnergy += _parts.backBoostData.avoidUseEnergy;
	backBoostData.avoidWait += _parts.backBoostData.avoidWait;

	rightBoostData.boostPower += _parts.rightBoostData.boostPower;
	rightBoostData.boostUseEnergy += _parts.rightBoostData.boostUseEnergy;
	rightBoostData.avoidPower += _parts.rightBoostData.avoidPower;
	rightBoostData.avoidUseEnergy += _parts.rightBoostData.avoidUseEnergy;
	rightBoostData.avoidWait += _parts.rightBoostData.avoidWait;

	leftBoostData.boostPower += _parts.leftBoostData.boostPower;
	leftBoostData.boostUseEnergy += _parts.leftBoostData.boostUseEnergy;
	leftBoostData.avoidPower += _parts.leftBoostData.avoidPower;
	leftBoostData.avoidUseEnergy += _parts.leftBoostData.avoidUseEnergy;
	leftBoostData.avoidWait += _parts.leftBoostData.avoidWait;

	upBoostData.boostPower += _parts.upBoostData.boostPower;
	upBoostData.boostUseEnergy += _parts.upBoostData.boostUseEnergy;
	upBoostData.avoidPower += _parts.upBoostData.avoidPower;
	upBoostData.avoidUseEnergy += _parts.upBoostData.avoidUseEnergy;
	upBoostData.avoidWait += _parts.upBoostData.avoidWait;

	downBoostData.boostPower += _parts.downBoostData.boostPower;
	downBoostData.boostUseEnergy += _parts.downBoostData.boostUseEnergy;
	downBoostData.avoidPower += _parts.downBoostData.avoidPower;
	downBoostData.avoidUseEnergy += _parts.downBoostData.avoidUseEnergy;
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

	energyTankData.chargeEnergy -= _parts.energyTankData.chargeEnergy;
	energyTankData.maxEnergy -= _parts.energyTankData.maxEnergy;

	walkData.movePower -= _parts.walkData.movePower;
	walkData.rotatePower -= _parts.walkData.rotatePower;
	walkData.jumpPower -= _parts.walkData.jumpPower;

	frontBoostData.boostPower -= _parts.frontBoostData.boostPower;
	frontBoostData.boostUseEnergy -= _parts.frontBoostData.boostUseEnergy;
	frontBoostData.avoidPower -= _parts.frontBoostData.avoidPower;
	frontBoostData.avoidUseEnergy -= _parts.frontBoostData.avoidUseEnergy;
	frontBoostData.avoidWait -= _parts.frontBoostData.avoidWait;

	backBoostData.boostPower -= _parts.backBoostData.boostPower;
	backBoostData.boostUseEnergy -= _parts.backBoostData.boostUseEnergy;
	backBoostData.avoidPower -= _parts.backBoostData.avoidPower;
	backBoostData.avoidUseEnergy -= _parts.backBoostData.avoidUseEnergy;
	backBoostData.avoidWait -= _parts.backBoostData.avoidWait;

	rightBoostData.boostPower -= _parts.rightBoostData.boostPower;
	rightBoostData.boostUseEnergy -= _parts.rightBoostData.boostUseEnergy;
	rightBoostData.avoidPower -= _parts.rightBoostData.avoidPower;
	rightBoostData.avoidUseEnergy -= _parts.rightBoostData.avoidUseEnergy;
	rightBoostData.avoidWait -= _parts.rightBoostData.avoidWait;

	leftBoostData.boostPower -= _parts.leftBoostData.boostPower;
	leftBoostData.boostUseEnergy -= _parts.leftBoostData.boostUseEnergy;
	leftBoostData.avoidPower -= _parts.leftBoostData.avoidPower;
	leftBoostData.avoidUseEnergy -= _parts.leftBoostData.avoidUseEnergy;
	leftBoostData.avoidWait -= _parts.leftBoostData.avoidWait;

	upBoostData.boostPower -= _parts.upBoostData.boostPower;
	upBoostData.boostUseEnergy -= _parts.upBoostData.boostUseEnergy;
	upBoostData.avoidPower -= _parts.upBoostData.avoidPower;
	upBoostData.avoidUseEnergy -= _parts.upBoostData.avoidUseEnergy;
	upBoostData.avoidWait -= _parts.upBoostData.avoidWait;

	downBoostData.boostPower -= _parts.downBoostData.boostPower;
	downBoostData.boostUseEnergy -= _parts.downBoostData.boostUseEnergy;
	downBoostData.avoidPower -= _parts.downBoostData.avoidPower;
	downBoostData.avoidUseEnergy -= _parts.downBoostData.avoidUseEnergy;
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
