#pragma once

#include"../../Frames/StageSelectFrame.h"

class StageDetailedDisplay : public StageSelectFrame::StageSelectFrameDisplay
{
public:

	void Init()override;

	void Release()override;

	void Update()override;

	void UpdateAction(const StageSelectFrame::ActionType& _action)override;

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)override;

	void OnDisplay()override;

private:

	enum class SelectSpriteType
	{
		UpButton,
		DownButton,
		StagePanel
	};

	enum class StageSelectButtonType
	{
		Up, Down, None
	};

private:

	void InitPositions();

private:

	void UpdateMouse();

	void UpdateKeyboard();

	void UpdateController();

private:

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
};