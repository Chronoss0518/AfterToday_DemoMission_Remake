#pragma once

class StageData;

class StageSelectFrame :public ChCpp::BaseFrame
{
public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void InitStagePreImageSpritePosition(ID3D11Device* _device);

	void InitTargetPreImageSpritePosition(ID3D11Device* _device);

	void InitStageData();

	void DrawFunction();

	void DrawStageData(const StageData& _stageData);

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

	std::vector<ActionType> inputDataList;

	std::vector<ChPtr::Shared<StageData>>stageDataList;

	ChD3D11::Sprite11 stagePreImageSprite[5];

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	int nowSelect = 0;

	ChD3D11::Texture11 notImageTexture;

	ChD3D11::Sprite11 stageNameSprite;

	ChD3D11::Sprite11 stageImageSprite;

	ChD3D11::Sprite11 targetNameSprite;

	ChD3D11::Sprite11 targetImageSprite[10];

};
