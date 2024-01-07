#pragma once

class StageData;

class StageSelectFrame :public ChCpp::BaseFrame
{
public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void InitStageData();

	void DrawFunction();

	void DrawStageData(const StageData& _stageData);

	void DrawStageSelect();

	void UpdateFunction();

	void UpdateMouse();

	void UpdateKeyboard();

	void UpdateController();

	enum class DisplayType
	{
		Select,//ëIëâÊñ //
		Detalied//è⁄ç◊âÊñ 
	};

	enum class ActionType
	{
		UpSelect,
		DownSelect,
		Decision
	};

	DisplayType type = DisplayType::Select;

	std::vector<ActionType> inputDataList;

	std::vector<ChPtr::Shared<StageData>>stageDataList;

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	int nowSelect = 0;

	ChD3D11::Texture11 notImageTexture;

	ChD3D11::Sprite11 stageNameSprite;

	ChD3D11::Sprite11 stageImageSprite;

};
