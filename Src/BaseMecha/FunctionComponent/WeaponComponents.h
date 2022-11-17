#pragma once

#include"WeaponComponent.h"

class RightWeaponComponent :public WeaponComponent
{
public:

	RightWeaponComponent() {
		attack = InputName::RAttack;
		msChange = InputName::RMSChange;
		mwChange = InputName::RWChange;
	}
};

class LeftWeaponComponent :public WeaponComponent
{
	LeftWeaponComponent() {
		attack = InputName::LAttack;
		msChange = InputName::LMSChange;
		mwChange = InputName::LWChange;
	}
};
