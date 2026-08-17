#pragma once

#include"WeaponPaletteUIBase.h"

#include"../Frames/MenuBase.h"

class WeaponPaletteEditUI : public WeaponPaletteUIBase
{
public:

	void Init(ID3D11Device* _device)override;

public:

	void Update(MenuBase::ActionType _type);

	void UpdateMouse(ChWin::MouseController& _mouse);

protected:

	ChVec4 GetPaletteColor(
		unsigned char _drawNo,
		WeaponHandType _type)override;

private:

	ChCpp::BitBool nowSetWeaponMask[DRAW_TYPE_COUNT] = { ChCpp::BitBool(PALETTE_COUNT) ,ChCpp::BitBool(PALETTE_COUNT) };
};