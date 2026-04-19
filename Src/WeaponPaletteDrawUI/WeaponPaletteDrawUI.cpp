#include"../BaseIncluder.h"

#include"../BaseMecha/BaseMecha.h"

#include"../BaseMecha/FunctionComponent/WeaponComponent.h"
#include"../BaseMecha/MechaPartsObjectFunction/WeaponFunction.h"

#include"WeaponPaletteDrawUI.h"

#ifndef PALETTE_TEXTURE_DIRECTORY
#define PALETTE_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY(L"WeaponPalette/" current_path) 
#endif

#define WEAPON_PALETTE_TOP 560.0f
#define WEAPON_PALETTE_WIDTH 290.0f
#define WEAPON_PALETTE_HEIGHT 50.0f

#define WEAPON_PALETTE_TOPBOTTOM_ALINE 5.0f
#define WEAPON_PALETTE_SIDE_ALINE 10.0f

#define PALETTE_WIDTH 46.0f
#define PALETTE_HEIGHT 40.0f

#define LEFT_WEAPON_PALETTE_LEFT 153.0f

#define RIGHT_WEAPON_PALETTE_LEFT 836.0f

#define WEAPON_SELECT_COLOR ChVec4::FromColor(1.0f,1.0f,0.0f,1.0f)
#define WEAPON_UNSELECT_COLOR ChVec4::FromColor(1.0f,1.0f,1.0f,1.0f)

void WeaponPaletteDrawUI::Init(ID3D11Device* _device)
{
	device = _device;
	backGround.CreateTexture(PALETTE_TEXTURE_DIRECTORY(L"WeaponPalette.png"), device);
	palette.CreateTexture(PALETTE_TEXTURE_DIRECTORY(L"Palette.png"), device);

	useSprite.Init();

	for (size_t i = 0; i < DRAW_TYPE_COUNT; i++)
	{
		for (size_t j = 0; j < PALETTE_COUNT; j++)
		{
			targetFunction[i][j] = nullptr;
		}
	}
}

void WeaponPaletteDrawUI::Release()
{
	backGround.Release();
	palette.Release();
}

void WeaponPaletteDrawUI::Update(BaseMecha* _targetMecha)
{
	if (ChPtr::NullCheck(_targetMecha))return;

	auto&& com = _targetMecha->GetComponentObject<WeaponComponent>();

	for (size_t i = 0; i < DRAW_TYPE_COUNT; i++)
	{
		WeaponHandType type = static_cast<WeaponHandType>(i);
		selectWeaponNo[i] = com->GetUseWeaponNo(type);
		for (size_t j = 0; j < PALETTE_COUNT; j++)
		{
			targetFunction[i][j] = nullptr;
			auto&& func = com->GetWeaponFunction(type, j);

			if (func == nullptr)continue;

			targetFunction[i][j] = &func->GetPaletteImage();
		}
	}

}

void WeaponPaletteDrawUI::Draw(ChD3D11::Shader::BaseDrawSprite11& _uiDrawer)
{
	DrawPalette(_uiDrawer,WeaponHandType::Left);

	DrawPalette(_uiDrawer, WeaponHandType::Right);
}

void WeaponPaletteDrawUI::DrawPalette(
	ChD3D11::Shader::BaseDrawSprite11& _uiDrawer,
	WeaponHandType _type)
{
	bool leftFlg = _type == WeaponHandType::Left;

	ChVec2 leftTop;
	leftTop.x = leftFlg ? LEFT_WEAPON_PALETTE_LEFT : RIGHT_WEAPON_PALETTE_LEFT;
	leftTop.y = WEAPON_PALETTE_TOP;

	useSprite.SetPosRect(
		RectToGameWindow(
			ChVec4::FromRect(
				leftTop.x,
				leftTop.y,
				leftTop.x + WEAPON_PALETTE_WIDTH,
				leftTop.y + WEAPON_PALETTE_HEIGHT)
		)
	);

	_uiDrawer.Draw(backGround, useSprite);

	leftTop.y += WEAPON_PALETTE_TOPBOTTOM_ALINE;

	for (unsigned char i = 0; i < PALETTE_COUNT; i++)
	{
		leftTop.x += WEAPON_PALETTE_SIDE_ALINE;

		useSprite.SetPosRect(
			RectToGameWindow(
				ChVec4::FromRect(
					leftTop.x,
					leftTop.y,
					leftTop.x + PALETTE_WIDTH,
					leftTop.y + PALETTE_HEIGHT)
			)
		);

		unsigned char drawNo = leftFlg ? PALETTE_COUNT - i - 1 : i;

		auto drawFunction = targetFunction[ChStd::EnumCast(_type)][drawNo];

		ChVec4 color = selectWeaponNo[ChStd::EnumCast(_type)] == drawNo ?
			WEAPON_SELECT_COLOR :
			WEAPON_UNSELECT_COLOR;

		if (ChPtr::NotNullCheck(drawFunction))
			_uiDrawer.Draw(*drawFunction, useSprite);

		_uiDrawer.Draw(palette, useSprite, color);

		leftTop.x += PALETTE_WIDTH;

	}

}
