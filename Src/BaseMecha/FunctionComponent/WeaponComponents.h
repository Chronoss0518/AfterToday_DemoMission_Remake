#pragma once

#include"WeaponComponent.h"

class RightWeaponComponent :public WeaponComponent
{
public:

	RightWeaponComponent() {
		attack = InputName::RAttack;

		attackTypeUpChange = InputName::RATUChange;
		attackTypeDownChange = InputName::RATDChange;

		weaponUpChange = InputName::RWUChange;
		weaponDownChange = InputName::RWDChange;
	}
};

class LeftWeaponComponent :public WeaponComponent
{
public:
	LeftWeaponComponent() {
		attack = InputName::LAttack;

		attackTypeUpChange = InputName::LATUChange;
		attackTypeDownChange = InputName::LATDChange;

		weaponUpChange = InputName::LWUChange;
		weaponDownChange = InputName::LWDChange;
	}
};
