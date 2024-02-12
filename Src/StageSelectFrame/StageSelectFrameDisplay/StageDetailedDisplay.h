#pragma once

#include"../../Frames/StageSelectFrame.h"

class StageDetailedDisplay : public StageSelectFrame::StageSelectFrameDisplay
{
public:

	void Init()override;

	void Release()override;

	void Update()override;

	void UpdateAction(MenuBase::ActionType _action)override;

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)override;

	void OnDisplay()override;

	void SetSelectData(const FromStageSelectFrameData& _selectData)override;

	void GetSelectData(FromStageSelectFrameData& _selectData)override;

public:

	enum class MenuButtonType
	{
		Start,
		Edit,
		Load,
		Cancel
	};

public:

	inline void SetMenuType(const MenuButtonType& _type)
	{
		nowSelect = ChStd::EnumCast(_type);
	}

private:

	void UpdateMouse();

private:

	void CreateStageNameTextImage(StageData& _stageData);

	void CreateStageDescriptionTextImage(StageData& _stageData);

	void CreateStageStrategyOverviewTextImage(StageData& _stageData);

private:

	void Load();

	void Edit();

private:

	static constexpr int MENU_BUTTON_TYPE = 4;
	int nowSelect = ChStd::EnumCast(MenuButtonType::Start);

	ImageSprite button[MENU_BUTTON_TYPE];
	ChD3D11::Texture11 buttonSelect;
	ChD3D11::Texture11 comingSoonImage;

	ImageSprite stageName;
	ChD3D11::Sprite11 stageImage;
	ImageSprite stageDescription;

	ImageSprite strategyOverview;
	ImageSprite strategyOverviewText;

	bool useLoadAndEdit = true;
};