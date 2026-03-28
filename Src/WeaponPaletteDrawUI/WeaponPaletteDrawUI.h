#pragma once

class WeaponFunction;

#include"../BaseMecha/WeaponHandType.h"

class WeaponPaletteDrawUI
{
private:

	struct DrawData
	{
		WeaponFunction* targetFunction = nullptr;
		ImageSprite palette;
		ChD3D11::Sprite11 drawPosition;
	};

public:

	void Init(ID3D11Device* _device);

	void Release();

	void CreateTextDrawer(TextDrawerWICBitmap& _drawer, unsigned long _w, unsigned long _h, float _fontSize);

	void CreateTextImage(ChD3D11::Texture11& _outImage, TextDrawerWICBitmap& _drawer, const std::wstring& _text, float _w, float _h);

public:

	void Update();

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _uiDrawer);

private:

	ID3D11Device* device = nullptr;

	DrawData brawData[DRAW_TYPE_COUNT];
	ChD3D11::Texture11 backGround;
	ChD3D11::Texture11 palette;



};