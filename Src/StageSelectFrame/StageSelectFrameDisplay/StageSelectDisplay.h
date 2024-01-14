#pragma once

#include"../../Frames/StageSelectFrame.h"

#ifndef PANEL_DRAW_COUNT
#define PANEL_DRAW_COUNT 4
#endif

class StageSelectDisplay : public StageSelectFrame::StageSelectFrameDisplay 
{
public:

	void Init()override;

	void Release()override;

	void Update()override;

	void UpdateAction(const StageSelectFrame::ActionType& _action)override;

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)override;

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

	void UpdateMouse();

	void UpdateKeyboard();

	void UpdateController();

private:

	SelectSpriteType selectSpriteType = SelectSpriteType::StagePanel;

	int drawNowSelect = 0;

	SelectImageSprite stageSelectButton[ChStd::EnumCast(StageSelectButtonType::None)];
	ChD3D11::Texture11 buttonSelectImage;

	StageSelectButtonType selectType = StageSelectButtonType::None;

	ImageSprite stageSelectPanelList;
	ImageSprite displayMap;
	ChD3D11::Sprite11 description;

	ChD3D11::Sprite11 selectStageSprite[PANEL_DRAW_COUNT];
	ChD3D11::Texture11 panelSelectImage;

	ChD3D11::Texture11 stagePanelBackground;
};