#pragma once

#include"FunctionComponent.h"

class WeaponComponent:public FunctionComponent
{
public:

	void Update()override;

public:

	inline void AddWeapon(ChPtr::Shared<MechaPartsObject> _weapon)
	{
		weapon.push_back(_weapon);
	}

	void Attack();

	void StartSubFunction();

	void AttackTypeUpChange();

	void AttackTypeDownChange();

	void WeaponUpChange();

	void WeaponDownChange();

protected:

	InputName attack = InputName::None;
	
	InputName attackTypeUpChange = InputName::None;
	InputName attackTypeDownChange = InputName::None;

	InputName weaponUpChange = InputName::None;
	InputName weaponDownChange = InputName::None;

private:

	unsigned long useWeaponNo = 0;
	std::vector<ChPtr::Shared<MechaPartsObject>>weapon;

};