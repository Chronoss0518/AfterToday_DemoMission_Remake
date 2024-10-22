#pragma once

#include"FunctionComponent.h"

class WeaponFunction;

class WeaponComponent:public FunctionComponent
{
public:

	void Update()override;

public:

	inline size_t GetUseWeaponNo() { return useWeaponNo; }
	
	inline size_t GetRegistWeaponCount() { return weapon.size(); }

	std::vector<ChPtr::Weak<WeaponFunction>> GetRegistWeaponList();

	std::vector<ChPtr::Weak<MechaPartsObject>> GetWeaponMechaPartsList();

	std::wstring GetPartsName();

	std::wstring GetWeaponName();

	std::wstring GetNowBulletNum();

	std::wstring GetReloadCount();

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

	std::wstring weaponName = L"";
	size_t useWeaponNo = 0;
	std::vector<ChPtr::Shared<MechaPartsObject>>weapon;

};
