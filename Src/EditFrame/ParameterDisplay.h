#pragma once

#include"ParameterList.h"

namespace PartsParameterStruct
{
	struct WeaponData;
	struct AttackBase;
}

class ParameterPartialDisplay : public ParameterDisplay
{
public:

	void Init(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)override;

protected:

	void AddPanelList();

	void AddWeaponParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddSwordParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddGunParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddAttackParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::wstring& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddBoostBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::wstring& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);
	
	void AddHighExplosiveBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::wstring& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);
	
	void AddMissileParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::wstring& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::wstring& _title);

	void AddParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::wstring& _title, TextDrawerWICBitmap& _titleDrawer, const std::wstring& _itemName, TextDrawerWICBitmap& _valueDrawer, unsigned long* _baseValue, unsigned long* _nextValue, bool _inversOperatorFlg);

	void AddParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::wstring& _title, TextDrawerWICBitmap& _titleDrawer, const std::wstring& _itemName, TextDrawerWICBitmap& _valueDrawer, float* _baseValue, float* _nextValue, bool _inversOperatorFlg);

public:

	void Update(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)override;

	void UpdateCountPanels(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer);

public:

	ChVec2 GetDrawStartPosition()override;

	ChVec2 GetDrawPanelSize()override;

public:

	void Up()override;

	void Down()override;

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)override;

private:

	std::vector<ChPtr::Weak<ParameterTitlePanel>>countPanels;
	unsigned long countPanelsBeforNum = 0;
	std::vector<ChPtr::Shared<ParameterTitlePanel>>panels;
	unsigned long baseParameterCount = 0;
	ChPtr::Shared<ParameterTitlePanel> emptyPanel = nullptr;

};

class ParameterEntireDisplay : public ParameterDisplay
{
public:

	void Init(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)override;

public:

	void Update(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)override;

public:

	ChVec2 GetDrawStartPosition()override;

	ChVec2 GetDrawPanelSize()override;

public:

	void Up()override;

	void Down()override;

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)override;

private:

	ChD3D11::Texture11 panelTitle;
	ChD3D11::Sprite11 drawSprite;
};
