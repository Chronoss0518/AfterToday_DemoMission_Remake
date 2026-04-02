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

public:

	ChVec2 GetDrawStartPosition()override;

	ChVec2 GetDrawPanelSize()override;

public:

	void Up()override;

	void Down()override;

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)override;

private:


};

class ParameterEntireDisplay : public ParameterDisplay
{
public:

	void Init(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)override;

public:

	ChVec2 GetDrawStartPosition()override;

	ChVec2 GetDrawPanelSize()override;

public:

	void Up()override;

	void Down()override;

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)override;

private:

};
