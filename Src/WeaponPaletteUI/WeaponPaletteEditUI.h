#pragma once

#include"WeaponPaletteUIBase.h"

#include"../Frames/MenuBase.h"

#ifndef NOT_WEAPON_PALETTE_SELECT_NO
#define NOT_WEAPON_PALETTE_SELECT_NO 0xff
#endif

class WeaponPaletteEditUI : public WeaponPaletteUIBase
{
public:

	void Init(ID3D11Device* _device)override;

public:

	void Open();

public:

	void Update(MenuBase::ActionType _type);

	void UpdateMouse();

	void UpdateSelect(ChPtr::Shared<BaseMecha>_editMecha);

public:

	void SetWeaponNo(WeaponHandType _type,unsigned char _weaponNo);

public:

	inline void AllDown()
	{
		for (unsigned char i = 0; i < DRAW_TYPE_COUNT; i++)
		{
			nowSetWeaponMask->SetAllDownFlg();
		}
	}


protected:

	ChVec4 GetPaletteColor(
		unsigned char _drawNo,
		WeaponHandType _type)override;

public:

	void GetNowSelectWeaponData(WeaponHandType& _type, unsigned char& _drawNo);

private:

	unsigned char nowSelectWeaponNo = 0;
	ChCpp::BitBool nowSetWeaponMask[DRAW_TYPE_COUNT] = { ChCpp::BitBool((PALETTE_COUNT/8) + 1) ,ChCpp::BitBool((PALETTE_COUNT / 8) + 1) };
};