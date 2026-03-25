#pragma once

#include"FunctionComponent.h"

#include"../WeaponHandType.h"

class WeaponFunction;

class WeaponComponent:public FunctionComponent
{
public:

	void Update()override;

public:

	inline void SetWeapon(WeaponHandType _type, unsigned char _paletteNo, ChPtr::Shared<WeaponFunction> _weapon)
	{
		if (_paletteNo < 0 || _paletteNo >= PALETTE_COUNT)return;
		weapons[ChStd::EnumCast(_type)][_paletteNo] = _weapon;
	}

public:

	ChPtr::Shared<WeaponFunction> GetWeaponFunction(WeaponHandType _type,unsigned char _num);

	inline size_t GetUseWeaponNo(WeaponHandType _type) { return useWeaponNo[ChStd::EnumCast(_type)]; }
	
	std::wstring GetPartsName(WeaponHandType _type);

	std::wstring GetWeaponName(WeaponHandType _type);

	std::wstring GetNowBulletNum(WeaponHandType _type);

	std::wstring GetReloadCount(WeaponHandType _type);

public:

	void Attack(WeaponHandType _type);

	void StartSubFunction(WeaponHandType _type);

	void WeaponUpChange(WeaponHandType _type);

	void WeaponDownChange(WeaponHandType _type);

private:

	size_t useWeaponNo[DRAW_TYPE_COUNT] = { 0,0 };
	ChPtr::Shared<WeaponFunction>weapons[DRAW_TYPE_COUNT][PALETTE_COUNT];

};
