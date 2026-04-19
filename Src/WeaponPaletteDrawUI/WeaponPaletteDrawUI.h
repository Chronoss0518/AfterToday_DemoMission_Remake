#pragma once

class WeaponFunction;

#include"../BaseMecha/WeaponHandType.h"

class BaseMecha;

class WeaponPaletteDrawUI : public ChCp::Initializer
{
public:

	void Init(ID3D11Device* _device);

	void Release();

public:

	void Update(BaseMecha* _targetMecha);

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _uiDrawer);

private:

	void DrawPalette(
		ChD3D11::Shader::BaseDrawSprite11& _uiDrawer,
		WeaponHandType _type);

private:

	ID3D11Device* device = nullptr;

	ChD3D11::Texture11* targetFunction[DRAW_TYPE_COUNT][PALETTE_COUNT];
	unsigned char selectWeaponNo[DRAW_TYPE_COUNT] = { 0,0 };
	ChD3D11::Texture11 backGround;
	ChD3D11::Texture11 palette;
	ChD3D11::Sprite11 useSprite;



};