#include"../BaseIncluder.h"
#include"WeaponPaletteDrawUI.h"


#ifndef PALETTE_TEXTURE_DIRECTORY
#define PALETTE_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY(L"WeaponPalette/" current_path) 
#endif


void WeaponPaletteDrawUI::Init(ID3D11Device* _device)
{
	device = _device;

	backGround.CreateTexture(PALETTE_TEXTURE_DIRECTORY(L"WeaponPalette.png"), device);
	palette.CreateTexture(PALETTE_TEXTURE_DIRECTORY(L"Palette.png"), device);
}

void WeaponPaletteDrawUI::Release()
{

}

void WeaponPaletteDrawUI::CreateTextDrawer(TextDrawerWICBitmap& _drawer, unsigned long _w, unsigned long _h, float _fontSize)
{
	_drawer.bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(_w, _h);
	_drawer.drawer.Init(_w, _h, _drawer.bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);
	_drawer.drawer.SetClearDisplayColor(ChVec4(0.0f));
	_drawer.drawer.SetClearDisplayFlg(true);

	_drawer.brush = _drawer.drawer.CreateBrush(ChVec4(1.0f));
	_drawer.format = _drawer.drawer.CreateTextFormat(L"ÉÅÉCÉäÉI", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, _fontSize);
}

void WeaponPaletteDrawUI::CreateTextImage(ChD3D11::Texture11& _outImage, TextDrawerWICBitmap& _drawer, const std::wstring& _text, float _w, float _h)
{
	_outImage.Release();

	_drawer.drawer.DrawStart();

	_drawer.drawer.DrawToScreen(_text, _drawer.format, _drawer.brush, ChVec4::FromRect(0.0f, 0.0f, _w, _h));

	_drawer.drawer.DrawEnd();

	_outImage.CreateColorTexture(device, _drawer.bitmap.GetBitmap());
}

void WeaponPaletteDrawUI::Update()
{

}

void WeaponPaletteDrawUI::Draw(ChD3D11::Shader::BaseDrawSprite11& _uiDrawer)
{

}
