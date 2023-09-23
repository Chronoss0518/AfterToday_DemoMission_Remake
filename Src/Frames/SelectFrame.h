#pragma once

class SelectFrame :public ChCpp::BaseFrame
{
public:

	void Init()override;

	void Release()override;

	void Update()override;

private:

	void SetScript();

	void DrawFunction();

	void UpdateFunction();

	enum class NextButtonType
	{

	};

	ChD3D11::Texture11 backGround;
	ChD3D11::Sprite11 descriptionWindowSprite;
	
	ChD3D11::Texture11 toBattle;
	ChD3D11::Sprite11 toBattleButtonSprite;
	ChD3D11::Texture11 descriptionBattle;
	
	ChD3D11::Texture11 toEdit;
	ChD3D11::Sprite11 toEditButtonSprite;
	ChD3D11::Texture11 descriptionEdit;

	ChD3D11::Texture11 toSetting;
	ChD3D11::Sprite11 toSettingButtonSprite;
	ChD3D11::Texture11 descriptionSetting;

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;
};
