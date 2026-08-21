#pragma once

class WeaponFunction;

#include"../BaseMecha/WeaponHandType.h"

class BaseMecha;


class WeaponPaletteUIBase : public ChCp::Initializer
{
public:

	virtual void Init(ID3D11Device* _device);

	void Release();

public:

	void SetSelectWeaponNo(WeaponHandType _type, unsigned char _no)
	{
		selectWeaponNo[ChStd::EnumCast(_type)] = _no;
	}

public:

	unsigned char GetSelectWeaponNo(WeaponHandType _type)
	{
		return selectWeaponNo[ChStd::EnumCast(_type)];
	}

protected:

	virtual ChVec4 GetPaletteColor(
		unsigned char _drawNo,
		WeaponHandType _type);

public:

	void Update(BaseMecha* _targetMecha);

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _uiDrawer);

private:

	void DrawPalette(
		ChD3D11::Shader::BaseDrawSprite11& _uiDrawer,
		WeaponHandType _type);

protected:

	float weapomPaletteTop = 0.0f;
	float weapomPaletteWidth = 0.0f;
	float weapomPaletteHeight = 0.0f;

	float leftWeapomPaletteLeft = 0.0f;
	float rightWeapomPaletteLeft = 0.0f;

	float weaponPaletteTopAline = 0.0f;
	float weaponPaletteSideAline = 0.0f;

	float paletteWidth = 0.0f;
	float paletteHeight = 0.0f;

private:

	ID3D11Device* device = nullptr;

	ChD3D11::Texture11* targetFunction[DRAW_TYPE_COUNT][PALETTE_COUNT];
	unsigned char selectWeaponNo[DRAW_TYPE_COUNT] = { 0,0 };
	ChD3D11::Texture11 backGround;
	ChD3D11::Texture11 palette;
	ChD3D11::Sprite11 useSprite;

};