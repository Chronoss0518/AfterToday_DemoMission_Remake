#include"../BaseIncluder.h"

#include"../BaseMecha/BaseMecha.h"

#include"../BaseMecha/FunctionComponent/WeaponComponent.h"
#include"../BaseMecha/MechaPartsObjectFunction/WeaponFunction.h"

#include"WeaponPaletteUIBase.h"

#ifndef PALETTE_TEXTURE_DIRECTORY
#define PALETTE_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY(L"WeaponPalette/" current_path) 
#endif

#define WEAPON_SELECT_COLOR ChVec4::FromColor(1.0f,1.0f,0.0f,1.0f)
#define WEAPON_UNSELECT_COLOR ChVec4::FromColor(1.0f,1.0f,1.0f,1.0f)

void WeaponPaletteUIBase::Init(ID3D11Device* _device)
{
	if (IsInit())return;

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

	SetInitFlg(true);
}

void WeaponPaletteUIBase::Release()
{
	if (!IsInit())return;

	backGround.Release();
	palette.Release();

	SetInitFlg(false);
}

void WeaponPaletteUIBase::Update(BaseMecha* _targetMecha)
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

void WeaponPaletteUIBase::Draw(ChD3D11::Shader::BaseDrawSprite11& _uiDrawer)
{
	DrawPalette(_uiDrawer,WeaponHandType::Left);

	DrawPalette(_uiDrawer, WeaponHandType::Right);
}

void WeaponPaletteUIBase::DrawPalette(
	ChD3D11::Shader::BaseDrawSprite11& _uiDrawer,
	WeaponHandType _type)
{
	bool leftFlg = _type == WeaponHandType::Left;

	ChVec2 leftTop;
	leftTop.x = leftFlg ? leftWeapomPaletteLeft : rightWeapomPaletteLeft;
	leftTop.y = weapomPaletteTop;

	useSprite.SetPosRect(
		RectToGameWindow(
			ChVec4::FromRect(
				leftTop.x,
				leftTop.y,
				leftTop.x + weapomPaletteWidth,
				leftTop.y + weapomPaletteHeight)
		)
	);

	_uiDrawer.Draw(backGround, useSprite);

	leftTop.y += weaponPaletteTopAline;

	for (unsigned char i = 0; i < PALETTE_COUNT; i++)
	{
		leftTop.x += weaponPaletteSideAline;

		useSprite.SetPosRect(
			RectToGameWindow(
				ChVec4::FromRect(
					leftTop.x,
					leftTop.y,
					leftTop.x + paletteWidth,
					leftTop.y + paletteHeight)
			)
		);

		unsigned char drawNo = leftFlg ? PALETTE_COUNT - i - 1 : i;

		auto drawFunction = targetFunction[ChStd::EnumCast(_type)][drawNo];

		if (ChPtr::NotNullCheck(drawFunction))
			_uiDrawer.Draw(*drawFunction, useSprite);

		ChVec4 color = GetPaletteColor(drawNo, _type);

		_uiDrawer.Draw(palette, useSprite, color);

		leftTop.x += paletteWidth;

	}

}

ChVec4 WeaponPaletteUIBase::GetPaletteColor(
	unsigned char _drawNo,
	WeaponHandType _type)
{
	return selectWeaponNo[ChStd::EnumCast(_type)] == _drawNo ?
		WEAPON_SELECT_COLOR :
		WEAPON_UNSELECT_COLOR;
}
