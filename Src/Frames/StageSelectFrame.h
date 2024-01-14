#pragma once

class StageData;

class StageSelectFrame :public ChCpp::BaseFrame
{
public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	enum class DisplayType
	{
		Select,//ëIëâÊñ //
		Detalied//è⁄ç◊âÊñ 
	};

	enum class ActionType
	{
		UpSelect,
		DownSelect,
		Decision,
		Cancel
	};

	enum class SelectSpriteType
	{
		UpButton,
		DownButton,
		StagePanel
	};

private:

	void InitStageData(const StageData& _stageData);

	void InitStageDataList();

	void InitStageSelectDisplay();

	void DrawFunction();

	void DrawStageData();

	void DrawStageSelect();

	void UpdateFunction();

	void UpdateStageData(const ActionType& _type);

	void UpdateStageSelect(const ActionType& _type);

	void UpdateMouse();

	void UpdateKeyboard();

	void UpdateController();

	DisplayType type = DisplayType::Select;

	std::vector<ActionType> inputDataList;

	std::vector<ChPtr::Shared<StageData>>stageDataList;

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	int nowSelectStage = 0;
	SelectSpriteType selectSpriteType = SelectSpriteType::StagePanel;

	ChD3D11::Texture11 notImageTexture;

	ChD3D11::Texture11 stagePanelBackground;

	//StageSelectDisplay//
	SelectImageSprite stageSelectUpButton;
	SelectImageSprite stageSelectDownButton;
	ImageSprite stageSelectPanelList;
	ImageSprite displayMap;
	ChD3D11::Sprite11 description;

	//StageDetailedDisplay//
	SelectImageSprite startButton;
	SelectImageSprite editButton;
	SelectImageSprite loadButton;
	SelectImageSprite cancelButton;
	TextSprite stageName;
	ChD3D11::Sprite11 stageImage;
	TextSprite drawSuccessFee;
	TextSprite missionTime_d;
	SelectImageSprite stageDescription;
	TextSprite stageDescriptionText;

	bool firstFlg = true;
};
