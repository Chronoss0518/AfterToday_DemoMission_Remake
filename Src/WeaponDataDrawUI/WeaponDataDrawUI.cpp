#include"../BaseIncluder.h"
#include"WeaponDataDrawUI.h"

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

#define WEAPON_NAME_TOP 475.0f - PANEL_TOP
#define WEAPON_NAME_LEFT 314.0f - LEFT_PANEL_LEFT
#define WEAPON_NAME_WIDTH 124.0f
#define WEAPON_NAME_HEIGHT 30.0f


#define NOW_BULLET_NUM_TEXT_SIZE 16.0f
#define RELOAD_COUNT_TEXT_SIZE 24.0f
#define WEAPON_NAME_TEXT_SIZE 14.0f


void WeaponDataDrawUI::Init(ID3D11Device* _device)
{
	device = _device;

	backGround.CreateTexture(TEXTURE_DIRECTORY("WeaponData.png"), device);
	
	CreateTextDrawer(bulletDrawer, static_cast<unsigned long>(NOW_BULLET_NUM_WIDTH), static_cast<unsigned long>(NOW_BULLET_NUM_HEIGHT), NOW_BULLET_NUM_TEXT_SIZE);
	bulletDrawer.format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	bulletDrawer.format.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	CreateTextDrawer(reloadDrawer, static_cast<unsigned long>(RELOAD_COUNT_WIDTH), static_cast<unsigned long>(RELOAD_COUNT_HEIGHT), RELOAD_COUNT_TEXT_SIZE);
	reloadDrawer.format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	reloadDrawer.format.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	CreateTextDrawer(weaponNameDrawer, static_cast<unsigned long>(WEAPON_NAME_WIDTH), static_cast<unsigned long>(WEAPON_NAME_HEIGHT), WEAPON_NAME_TEXT_SIZE);
	weaponNameDrawer.format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	weaponNameDrawer.format.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	SPRITE_INIT(
		brawData[ChStd::EnumCast(DRAW_TYPE::Left)].drawPosition,
		RectToGameWindow(ChVec4::FromRect(LEFT_PANEL_LEFT,PANEL_TOP, LEFT_PANEL_RIGHT, PANEL_BOTTOM))
	);


	SPRITE_INIT(
		brawData[ChStd::EnumCast(DRAW_TYPE::Left)].bulletNumIS.sprite,
		RectToGameWindow(ChVec4::FromRect(NOW_BULLET_NUM_LEFT + LEFT_PANEL_LEFT, NOW_BULLET_NUM_TOP + PANEL_TOP, NOW_BULLET_NUM_LEFT + LEFT_PANEL_LEFT + NOW_BULLET_NUM_WIDTH, NOW_BULLET_NUM_TOP + PANEL_TOP + NOW_BULLET_NUM_HEIGHT))
	);

	SPRITE_INIT(
		brawData[ChStd::EnumCast(DRAW_TYPE::Left)].reloadIS.sprite,
		RectToGameWindow(ChVec4::FromRect(RELOAD_COUNT_LEFT + LEFT_PANEL_LEFT, RELOAD_COUNT_TOP + PANEL_TOP, RELOAD_COUNT_LEFT + LEFT_PANEL_LEFT + RELOAD_COUNT_WIDTH, RELOAD_COUNT_TOP + PANEL_TOP + RELOAD_COUNT_HEIGHT))
	);

	SPRITE_INIT(
		brawData[ChStd::EnumCast(DRAW_TYPE::Left)].weaponIS.sprite,
		RectToGameWindow(ChVec4::FromRect(WEAPON_NAME_LEFT + LEFT_PANEL_LEFT, WEAPON_NAME_TOP + PANEL_TOP, WEAPON_NAME_LEFT + LEFT_PANEL_LEFT + WEAPON_NAME_WIDTH, WEAPON_NAME_TOP + PANEL_TOP + WEAPON_NAME_HEIGHT))
	);


	SPRITE_INIT(
		brawData[ChStd::EnumCast(DRAW_TYPE::Right)].drawPosition,
		RectToGameWindow(ChVec4::FromRect(RIGHT_PANEL_LEFT, PANEL_TOP, RIGHT_PANEL_RIGHT, PANEL_BOTTOM))
	);

	SPRITE_INIT(
		brawData[ChStd::EnumCast(DRAW_TYPE::Right)].bulletNumIS.sprite,
		RectToGameWindow(ChVec4::FromRect(NOW_BULLET_NUM_LEFT + RIGHT_PANEL_LEFT, NOW_BULLET_NUM_TOP + PANEL_TOP, NOW_BULLET_NUM_LEFT + RIGHT_PANEL_LEFT + NOW_BULLET_NUM_WIDTH, NOW_BULLET_NUM_TOP + PANEL_TOP + NOW_BULLET_NUM_HEIGHT))
	);

	SPRITE_INIT(
		brawData[ChStd::EnumCast(DRAW_TYPE::Right)].reloadIS.sprite,
		RectToGameWindow(ChVec4::FromRect(RELOAD_COUNT_LEFT + RIGHT_PANEL_LEFT, RELOAD_COUNT_TOP + PANEL_TOP, RELOAD_COUNT_LEFT + RIGHT_PANEL_LEFT + RELOAD_COUNT_WIDTH, RELOAD_COUNT_TOP + PANEL_TOP + RELOAD_COUNT_HEIGHT))
	);

	SPRITE_INIT(
		brawData[ChStd::EnumCast(DRAW_TYPE::Right)].weaponIS.sprite,
		RectToGameWindow(ChVec4::FromRect(WEAPON_NAME_LEFT + RIGHT_PANEL_LEFT, WEAPON_NAME_TOP + PANEL_TOP, WEAPON_NAME_LEFT + RIGHT_PANEL_LEFT + WEAPON_NAME_WIDTH, WEAPON_NAME_TOP + PANEL_TOP + WEAPON_NAME_HEIGHT))
	);


}

void WeaponDataDrawUI::Release()
{
	bulletDrawer.drawer.Release();
	bulletDrawer.bitmap.Release();
	reloadDrawer.drawer.Release();
	reloadDrawer.bitmap.Release();
	weaponNameDrawer.drawer.Release();
	weaponNameDrawer.bitmap.Release();
}

void WeaponDataDrawUI::CreateTextDrawer(TextDrawerWICBitmap& _drawer, unsigned long _w, unsigned long _h, float _fontSize)
{
	_drawer.bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(_w, _h);
	_drawer.drawer.Init(_w, _h, _drawer.bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);
	_drawer.drawer.SetClearDisplayColor(ChVec4(0.0f));
	_drawer.drawer.SetClearDisplayFlg(true);

	_drawer.brush = _drawer.drawer.CreateBrush(ChVec4(1.0f));
	_drawer.format = _drawer.drawer.CreateTextFormat(L"ÉÅÉCÉäÉI", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, _fontSize);
}

void WeaponDataDrawUI::CreateTextImage(ChD3D11::Texture11& _outImage, TextDrawerWICBitmap& _drawer, const std::wstring& _text, float _w, float _h)
{
	_outImage.Release();

	_drawer.drawer.DrawStart();

	_drawer.drawer.DrawToScreen(_text, _drawer.format, _drawer.brush, ChVec4::FromRect(0.0f, 0.0f, _w, _h));

	_drawer.drawer.DrawEnd();

	_outImage.CreateColorTexture(device,_drawer.bitmap.GetBitmap());
}

void WeaponDataDrawUI::SetNowBulletNum(const std::wstring& _num, DRAW_TYPE _type)
{
	if (brawData[ChStd::EnumCast(_type)].nowBulletNum == _num)return;
	brawData[ChStd::EnumCast(_type)].nowBulletNum = _num;
	brawData[ChStd::EnumCast(_type)].isUpdateBulletNum = true;
}

void WeaponDataDrawUI::SetReloadCount(const std::wstring& _count, DRAW_TYPE _type)
{
	if (brawData[ChStd::EnumCast(_type)].reloadCount == _count)return;
	brawData[ChStd::EnumCast(_type)].reloadCount = _count;
	brawData[ChStd::EnumCast(_type)].isUpdateReloadCount = true;
}

void WeaponDataDrawUI::SetWeaponName(const std::wstring& _name, DRAW_TYPE _type)
{
	if (brawData[ChStd::EnumCast(_type)].weaponName == _name)return;
	brawData[ChStd::EnumCast(_type)].weaponName = _name;
	brawData[ChStd::EnumCast(_type)].isUpdateWeaponName = true;
}

void WeaponDataDrawUI::Update()
{
	for (unsigned char i = 0; i < DRAW_TYPE_COUNT; i++)
	{
		auto&& draw = brawData[i];
		if (draw.isUpdateBulletNum)
		{
			CreateTextImage(draw.bulletNumIS.image, bulletDrawer, draw.nowBulletNum, NOW_BULLET_NUM_WIDTH, NOW_BULLET_NUM_HEIGHT);
			draw.isUpdateBulletNum = false;
		}

		if (draw.isUpdateReloadCount)
		{
			CreateTextImage(draw.reloadIS.image, reloadDrawer, draw.reloadCount, RELOAD_COUNT_WIDTH, RELOAD_COUNT_HEIGHT);
			draw.isUpdateReloadCount = false;
		}

		if (draw.isUpdateWeaponName)
		{
			CreateTextImage(draw.weaponIS.image, weaponNameDrawer, draw.weaponName, WEAPON_NAME_WIDTH, WEAPON_NAME_HEIGHT);
			draw.isUpdateWeaponName = false;
		}
	}


}

void WeaponDataDrawUI::Draw(ChD3D11::Shader::BaseDrawSprite11& _uiDrawer)
{

	for (unsigned char i = 0; i < DRAW_TYPE_COUNT; i++)
	{
		auto&& draw = brawData[i];
		_uiDrawer.Draw(backGround, draw.drawPosition);
		_uiDrawer.Draw(draw.bulletNumIS.image, draw.bulletNumIS.sprite);
		_uiDrawer.Draw(draw.reloadIS.image, draw.reloadIS.sprite);
		_uiDrawer.Draw(draw.weaponIS.image, draw.weaponIS.sprite);
	}

}
