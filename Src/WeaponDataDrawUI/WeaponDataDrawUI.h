#pragma once


class WeaponDataDrawUI
{
public:

	static constexpr unsigned long DRAW_TYPE_COUNT = 2;

	enum class DRAW_TYPE
	{
		Left, Right
	};

private:

	struct DrawData
	{
		std::wstring nowBulletNum = L"0";
		bool isUpdateBulletNum = true;
		ImageSprite bulletNumIS;
		std::wstring reloadCount = L"0";
		bool isUpdateReloadCount = true;
		ImageSprite reloadIS;
		std::wstring weaponName = L"-";
		bool isUpdateWeaponName = true;
		ImageSprite weaponIS;
		ChD3D11::Sprite11 drawPosition;
	};

public:

	void Init(ID3D11Device* _device);

	void Release();

	void CreateTextDrawer(TextDrawerWICBitmap& _drawer,unsigned long _w,unsigned long _h,float _fontSize);

	void CreateTextImage(ChD3D11::Texture11& _outImage, TextDrawerWICBitmap& _drawer, const std::wstring& _text, float _w, float _h);

public:

	void SetNowBulletNum(const std::wstring& _num, DRAW_TYPE _type);
	
	void SetReloadCount(const std::wstring& _count, DRAW_TYPE _type);

	void SetWeaponName(const std::wstring& _name, DRAW_TYPE _type);

public:

	void Update();

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _uiDrawer);

private:

	TextDrawerWICBitmap bulletDrawer;
	TextDrawerWICBitmap reloadDrawer;
	TextDrawerWICBitmap weaponNameDrawer;

	ID3D11Device* device = nullptr;

	DrawData brawData[DRAW_TYPE_COUNT];
	ChD3D11::Texture11 backGround;



};