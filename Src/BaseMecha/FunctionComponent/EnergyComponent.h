#pragma once

#include"FunctionComponent.h"

class EnergyComponent :public FunctionComponent
{
public:

	void Update()override;

public:

	inline void AddMaxEnergy(const unsigned long _maxEnelgy) { maxEnergy += _maxEnelgy; nowEnergy = maxEnergy; }

	inline void AddChargeEnergy(const unsigned long _chargeEnelgy) { chargeEnergy += _chargeEnelgy; }

public:

	inline void SubNowEnergy(unsigned long _use) { nowEnergy -= (nowEnergy - _use) > 0 ? _use : nowEnergy; }

	inline void SubMaxEnergy(const unsigned long _maxEnelgy) { maxEnergy -= _maxEnelgy; }

	inline void SubChargeEnergy(const unsigned long _chargeEnelgy) { chargeEnergy -= _chargeEnelgy; }

public:

	inline void SetMaxEnergy(const unsigned long _maxEnelgy) { maxEnergy = _maxEnelgy; }

	inline void SetChargeEnergy(const unsigned long _chargeEnelgy) { chargeEnergy = _chargeEnelgy; }

public:

	inline unsigned long GetNowEnergy() { return nowEnergy; }

	inline unsigned long GetMaxEnergy() { return maxEnergy; }

private:

	unsigned long maxEnergy = 0;
	unsigned long chargeEnergy = 0;
	unsigned long nowEnergy = 0;
};