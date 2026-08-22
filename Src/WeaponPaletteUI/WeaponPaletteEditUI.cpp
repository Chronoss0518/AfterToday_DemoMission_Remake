#include"../BaseIncluder.h"

#include"../BaseMecha/BaseMecha.h"

#include"../BaseMecha/FunctionComponent/WeaponComponent.h"
#include"../BaseMecha/MechaPartsObjectFunction/WeaponFunction.h"

#include"WeaponPaletteEditUI.h"


#define WEAPON_PALETTE_TOP 590.0f
#define WEAPON_PALETTE_WIDTH 580.0f
#define WEAPON_PALETTE_HEIGHT 100.0f

#define LEFT_WEAPON_PALETTE_LEFT 20.0f
#define RIGHT_WEAPON_PALETTE_LEFT 680.0f

#define WEAPON_PALETTE_TOP_ALINE 10.0f
#define WEAPON_PALETTE_SIDE_ALINE 20.0f

#define PALETTE_WIDTH 92.0f
#define PALETTE_HEIGHT 80.0f

#define WEAPON_SELECT_COLOR ChVec4::FromColor(1.0f,1.0f,0.0f,1.0f)
#define WEAPON_UNSELECT_COLOR ChVec4::FromColor(1.0f,1.0f,1.0f,1.0f)
#define WEAPON_SET_COLOR ChVec4::FromColor(0.0f,1.0f,1.0f,1.0f)

void WeaponPaletteEditUI::Init(ID3D11Device* _device)
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

void WeaponPaletteEditUI::Open()
{
	nowSelectWeaponNo = 0;
}

void WeaponPaletteEditUI::Update(MenuBase::ActionType _type)
{
	if (_type == MenuBase::ActionType::Left)
		nowSelectWeaponNo = (nowSelectWeaponNo + PALETTE_COUNT - 1) % PALETTE_COUNT;

	if (_type == MenuBase::ActionType::Right)
		nowSelectWeaponNo = (nowSelectWeaponNo + 1) % PALETTE_COUNT;
}

void WeaponPaletteEditUI::UpdateMouse()
{
	ChVec2 leftTop;
	leftTop.y = weapomPaletteTop;

	leftTop.y += weaponPaletteTopAline;

	for (unsigned char i = 0; i < DRAW_TYPE_COUNT; i++)
	{
		leftTop.x = i == ChStd::EnumCast(WeaponHandType::Left) ? leftWeapomPaletteLeft : rightWeapomPaletteLeft;
		for (unsigned char j = 0; j < PALETTE_COUNT; j++)
		{
			leftTop.x += weaponPaletteSideAline;

			if (IsMoucePosOnRect(RectToGameWindow(ChVec4::FromRect(
				leftTop.x,
				leftTop.y,
				leftTop.x + paletteWidth,
				leftTop.y + paletteHeight))))
			{
				nowSelectWeaponNo = j + (i * PALETTE_COUNT);
				return;
			}

			leftTop.x += paletteWidth;
		}
	}

}

void WeaponPaletteEditUI::UpdateSelect(ChPtr::Shared<BaseMecha>_editMecha)
{
	if (_editMecha == nullptr)return;

	WeaponPaletteUIBase::Update(_editMecha.get());


	if (nowSelectWeaponNo < PALETTE_COUNT)
	{
		WeaponPaletteUIBase::SetSelectWeaponNo(WeaponHandType::Left, PALETTE_COUNT - nowSelectWeaponNo - 1);
		WeaponPaletteUIBase::SetSelectWeaponNo(WeaponHandType::Right, NOT_WEAPON_PALETTE_SELECT_NO);
	}
	else
	{
		WeaponPaletteUIBase::SetSelectWeaponNo(WeaponHandType::Left, NOT_WEAPON_PALETTE_SELECT_NO);
		WeaponPaletteUIBase::SetSelectWeaponNo(WeaponHandType::Right, nowSelectWeaponNo - PALETTE_COUNT);
	}

}

void WeaponPaletteEditUI::SetWeaponNo(WeaponHandType _type, unsigned char _weaponNo)
{
	if (PALETTE_COUNT <= _weaponNo)return;
	nowSetWeaponMask[ChStd::EnumCast(_type)].SetBitTrue(_weaponNo);
}

ChVec4 WeaponPaletteEditUI::GetPaletteColor(
	unsigned char _drawNo,
	WeaponHandType _type)
{
	ChVec4 color = nowSetWeaponMask[ChStd::EnumCast(_type)].GetBitFlg(_drawNo) ?
		WEAPON_SET_COLOR :
		WEAPON_UNSELECT_COLOR;

	return GetSelectWeaponNo(_type) == _drawNo ?
		WEAPON_SELECT_COLOR :
		color;
}

void WeaponPaletteEditUI::GetNowSelectWeaponData(WeaponHandType& _type, unsigned char& _drawNo)
{
	if (nowSelectWeaponNo < PALETTE_COUNT)
	{
		_type = WeaponHandType::Left;
		_drawNo = PALETTE_COUNT - nowSelectWeaponNo - 1;
	}
	else
	{
		_type = WeaponHandType::Right;
		_drawNo = nowSelectWeaponNo - PALETTE_COUNT;
	}
}