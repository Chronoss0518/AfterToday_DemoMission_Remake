#include"../BaseIncluder.h"

#include"../BaseMecha/BaseMecha.h"

#include"../BaseMecha/FunctionComponent/WeaponComponent.h"
#include"../BaseMecha/MechaPartsObjectFunction/WeaponFunction.h"

#include"WeaponPaletteEditUI.h"


#define WEAPON_PALETTE_TOP 560.0f
#define WEAPON_PALETTE_WIDTH 290.0f
#define WEAPON_PALETTE_HEIGHT 50.0f

#define LEFT_WEAPON_PALETTE_LEFT 153.0f
#define RIGHT_WEAPON_PALETTE_LEFT 836.0f

#define WEAPON_PALETTE_TOP_ALINE 5.0f
#define WEAPON_PALETTE_SIDE_ALINE 10.0f

#define PALETTE_WIDTH 46.0f
#define PALETTE_HEIGHT 40.0f

#define WEAPON_SELECT_COLOR ChVec4::FromColor(1.0f,1.0f,0.0f,1.0f)
#define WEAPON_UNSELECT_COLOR ChVec4::FromColor(1.0f,1.0f,1.0f,1.0f)
#define WEAPON_SET_COLOR ChVec4::FromColor(0.0f,1.0f,1.0f,1.0f)

void WeaponPaletteEditUI::Init(ID3D11Device* _device)
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

void WeaponPaletteEditUI::Update(MenuBase::ActionType _type)
{

}

void WeaponPaletteEditUI::UpdateMouse(ChWin::MouseController& _mouse)
{

}

ChVec4 WeaponPaletteEditUI::GetPaletteColor(
	unsigned char _drawNo,
	WeaponHandType _type)
{
	ChVec4 color = GetSelectWeaponNo(_type) == _drawNo ?
		WEAPON_SELECT_COLOR :
		WEAPON_UNSELECT_COLOR;
}
