#include"../BaseIncluder.h"
#include"WeaponPaletteDrawUI.h"

#define PANEL_TOP 475.0f
#define PANEL_BOTTOM PANEL_TOP + 70.0f
#define PANEL_WIDTH 134.0f

#define LEFT_PANEL_LEFT 309.0f
#define LEFT_PANEL_RIGHT LEFT_PANEL_LEFT + PANEL_WIDTH

#define RIGHT_PANEL_LEFT 836.0f
#define RIGHT_PANEL_RIGHT RIGHT_PANEL_LEFT + PANEL_WIDTH

#define NOW_BULLET_NUM_TOP  505.0f - PANEL_TOP
#define NOW_BULLET_NUM_LEFT 310.0f - LEFT_PANEL_LEFT
#define NOW_BULLET_NUM_WIDTH 80.0f
#define NOW_BULLET_NUM_HEIGHT 40.0f

#define RELOAD_COUNT_TOP 505.0f - PANEL_TOP
#define RELOAD_COUNT_LEFT 390.0f - LEFT_PANEL_LEFT
#define RELOAD_COUNT_WIDTH 53.0f
#define RELOAD_COUNT_HEIGHT 40.0f

#define WEAPON_NAME_LEFT 314.0f - LEFT_PANEL_LEFT
#define WEAPON_NAME_WIDTH 124.0f

#define WEAPON_PARTS_NAME_TOP 475.0f - PANEL_TOP
#define WEAPON_PARTS_NAME_HEIGHT 12.0f

#define WEAPON_NAME_TOP WEAPON_PARTS_NAME_HEIGHT + WEAPON_PARTS_NAME_TOP
#define WEAPON_NAME_HEIGHT 16.0f

#define NOW_BULLET_NUM_TEXT_SIZE 16.0f
#define RELOAD_COUNT_TEXT_SIZE 24.0f
#define WEAPON_PARTS_NAME_TEXT_SIZE 10.0f
#define WEAPON_NAME_TEXT_SIZE 12.0f


void WeaponPaletteDrawUI::Init(ID3D11Device* _device)
{
	device = _device;

	backGround.CreateTexture(TEXTURE_DIRECTORY(L"WeaponPalette.png"), device);


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
