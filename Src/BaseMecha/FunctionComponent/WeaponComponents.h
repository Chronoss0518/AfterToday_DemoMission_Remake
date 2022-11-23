#pragma once

#include"WeaponComponent.h"

class RightWeaponComponent :public WeaponComponent
{
public:

	RightWeaponComponent() {
		attack = InputName::RAttack;
		msChange = InputName::RATChange;
		mwChange = InputName::RWChange;
	}
};

class LeftWeaponComponent :public WeaponComponent
{
	LeftWeaponComponent() {
		attack = InputName::LAttack;
		msChange = InputName::LATChange;
		mwChange = InputName::LWChange;
	}
};
