#pragma once

class SelectFrame :public ChCpp::BaseFrame
{
public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void SetScript();

	void DrawFunction();

	void UpdateFunction();

	void UpdateMouse();

	void UpdateKeyboard();

	void UpdateController();

	enum class ActionType
	{
		UpSelect,
		DownSelect,
		Decision
	};

	enum class NextButtonType
	{
		Battle,
		Edit,
		Setting
	};

	static constexpr int NEXT_BUTTON_TYPE_COUNT = 3;
	std::vector<ActionType> inputDataList;
	std::map<NextButtonType,std::function<void(void)>>nextFrameFunction;

	ChD3D11::Texture11 backGround;
	ChD3D11::Sprite11 descriptionWindowSprite;
	
	ChD3D11::Texture11 selectEdge;
	ChD3D11::Texture11 comingSoonTexture;
	
	ImageSprite toButton[NEXT_BUTTON_TYPE_COUNT];
	
	ChD3D11::Texture11 description[NEXT_BUTTON_TYPE_COUNT];

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	ChD3D::XInputController controller;
	bool beforePushFlg = false;

	int nowSelect = ChStd::EnumCast(NextButtonType::Battle);

	bool firstFlg = true;
};
