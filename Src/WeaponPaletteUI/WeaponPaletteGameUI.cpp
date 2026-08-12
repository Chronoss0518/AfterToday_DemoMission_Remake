#include"../BaseIncluder.h"

#include"../BaseMecha/BaseMecha.h"

#include"../BaseMecha/FunctionComponent/WeaponComponent.h"
#include"../BaseMecha/MechaPartsObjectFunction/WeaponFunction.h"

#include"WeaponPaletteGameUI.h"


#define WEAPON_PALETTE_TOP 560.0f
#define WEAPON_PALETTE_WIDTH 290.0f
#define WEAPON_PALETTE_HEIGHT 50.0f

#define LEFT_WEAPON_PALETTE_LEFT 153.0f
#define RIGHT_WEAPON_PALETTE_LEFT 836.0f

#define WEAPON_PALETTE_TOP_ALINE 5.0f
#define WEAPON_PALETTE_SIDE_ALINE 10.0f

#define PALETTE_WIDTH 46.0f
#define PALETTE_HEIGHT 40.0f

void WeaponPaletteGameUI::Init(ID3D11Device* _device)
{

	WeaponPaletteUIBase::Init(_device);

	SetWeaponPaletteTop(WEAPON_PALETTE_TOP);
	SetWeaponPaletteWidth(WEAPON_PALETTE_WIDTH);
	SetWeaponPaletteHeight(WEAPON_PALETTE_HEIGHT);

	SetLeftWeaponPaletteLeft(LEFT_WEAPON_PALETTE_LEFT);
	SetRightWeaponPaletteLeft(RIGHT_WEAPON_PALETTE_LEFT);

	SetWeaponPaletteTopAline(WEAPON_PALETTE_TOP_ALINE);
	SetWeaponPaletteSideAline(WEAPON_PALETTE_SIDE_ALINE);

	SetPaletteWidth(PALETTE_WIDTH);
	SetPaletteHeight(PALETTE_HEIGHT);
}