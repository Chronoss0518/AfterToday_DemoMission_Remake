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
		Battle,
		Edit,
		Setting
	};
	static constexpr int NEXT_BUTTON_TYPE_COUNT = 3;


	ChD3D11::Texture11 backGround;
	ChD3D11::Sprite11 descriptionWindowSprite;
	
	ChD3D11::Texture11 selectEdge;
	ChD3D11::Sprite11 selectSprite[NEXT_BUTTON_TYPE_COUNT];
	

	ChD3D11::Texture11 toBattle;
	ChD3D11::Sprite11 toBattleButtonSprite;
	
	ChD3D11::Texture11 toEdit;
	ChD3D11::Sprite11 toEditButtonSprite;

	ChD3D11::Texture11 toSetting;
	ChD3D11::Sprite11 toSettingButtonSprite;

	ChD3D11::Texture11 description[NEXT_BUTTON_TYPE_COUNT];

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	int nowSelect = ChStd::EnumCast(NextButtonType::Battle);
};
