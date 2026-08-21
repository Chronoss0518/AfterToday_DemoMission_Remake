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

	weapomPaletteTop = WEAPON_PALETTE_TOP;
	weapomPaletteWidth = WEAPON_PALETTE_WIDTH;
	weapomPaletteHeight = WEAPON_PALETTE_HEIGHT;

	leftWeapomPaletteLeft = LEFT_WEAPON_PALETTE_LEFT;
	rightWeapomPaletteLeft = RIGHT_WEAPON_PALETTE_LEFT;

	weaponPaletteTopAline = WEAPON_PALETTE_TOP_ALINE;
	weaponPaletteSideAline = WEAPON_PALETTE_SIDE_ALINE;

	paletteWidth = PALETTE_WIDTH;
	paletteHeight = PALETTE_HEIGHT;
}