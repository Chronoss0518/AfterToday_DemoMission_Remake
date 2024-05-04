#pragma once

#include"../../Frames/StageSelectFrame.h"

class StageSelectList;

class StageSelectDisplay : public StageSelectFrame::StageSelectFrameDisplay 
{
public:

	void Init()override;

	void Release()override;

	void Update()override;

	void UpdateAction(MenuBase::ActionType _action)override;

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)override;

	void SetSelectData(const FromStageSelectFrameData& _selectData)override;

	void GetSelectData(FromStageSelectFrameData& _selectData)override;

private:

	enum class StageSelectButtonType
	{
		Up, Down, None
	};

private:

	void UpdateMouse();

private:

	static constexpr int PANEL_DRAW_COUNT = 4;

	unsigned long drawNowSelect = 0;

	ImageSprite stageSelectButton[ChStd::EnumCast(StageSelectButtonType::None)];
	ChD3D11::Texture11 buttonSelectImage;

	StageSelectButtonType selectType = StageSelectButtonType::None;

	ImageSprite stageSelectPanelList;
	ImageSprite displayMap;
	ChD3D11::Sprite11 description;

	ChD3D11::Texture11 stagePanelBackground;

	ChD3D11::Texture11 panelSelectImage;
	ChPtr::Shared<StageSelectList> selectList = nullptr;
};