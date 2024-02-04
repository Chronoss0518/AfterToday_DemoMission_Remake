
#include"../../BaseIncluder.h"

#include"StageSelectDisplay.h"
#include"../StageData/StageData.h"


#define SELECT_PANEL_LEFT 45.0f

#define SELECT_PANEL_RIGHT STAGE_SELECT_PANEL_WIDTH + SELECT_PANEL_LEFT
#define SELECT_PANEL_BUTTON_BOTTOM(TOP) TOP + 91.0f

#define SELECT_UP_BUTTON_TOP 32.0f
#define SELECT_DOWN_BUTTON_TOP 555.0f

#define SELECT_PANEL_LIST_TOP 134.0f
#define SELECT_PANEL_LIST_BOTTOM (STAGE_SELECT_PANEL_HEIGHT * PANEL_DRAW_COUNT) + SELECT_PANEL_LIST_TOP

#define DISPLAY_MAP_LEFT 435.0f
#define DISPLAY_MAP_TOP 32.0f
#define DISPLAY_MAP_RIGHT 800.0f + DISPLAY_MAP_LEFT
#define DISPLAY_MAP_BOTTOM 409.0f + DISPLAY_MAP_TOP

#define DESCRIPTION_LEFT 440.0f
#define DESCRIPTION_TOP 447.0f
#define DESCRIPTION_RIGHT STAGE_SELECT_DESCRIPTION_WIDTH + DESCRIPTION_LEFT
#define DESCRIPTION_BOTTOM STAGE_SELECT_DESCRIPTION_HEIGHT + DESCRIPTION_TOP

void StageSelectDisplay::Init()
{

	auto&& device = ChD3D11::D3D11Device();

	stagePanelBackground.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("StageNamePanel.png"), device);

	SPRITE_INIT(
		stageSelectButton[ChStd::EnumCast(StageSelectButtonType::Up)].sprite,
		RectToGameWindow(ChVec4::FromRect(SELECT_PANEL_LEFT, SELECT_UP_BUTTON_TOP, SELECT_PANEL_RIGHT, SELECT_PANEL_BUTTON_BOTTOM(SELECT_UP_BUTTON_TOP))));
	stageSelectButton[ChStd::EnumCast(StageSelectButtonType::Up)].image.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("UPButton.png"), device);

	SPRITE_INIT(
		stageSelectButton[ChStd::EnumCast(StageSelectButtonType::Down)].sprite,
		RectToGameWindow(ChVec4::FromRect(SELECT_PANEL_LEFT, SELECT_DOWN_BUTTON_TOP, SELECT_PANEL_RIGHT, SELECT_PANEL_BUTTON_BOTTOM(SELECT_DOWN_BUTTON_TOP))));
	stageSelectButton[ChStd::EnumCast(StageSelectButtonType::Down)].image.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("DownButton.png"), device);
	buttonSelectImage.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("ButtonSelect.png"), device);

	SPRITE_INIT(displayMap.sprite, RectToGameWindow(ChVec4::FromRect(DISPLAY_MAP_LEFT, DISPLAY_MAP_TOP, DISPLAY_MAP_RIGHT, DISPLAY_MAP_BOTTOM)));
	displayMap.image.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("Map(NoImage).png"), device);

	SPRITE_INIT(description, RectToGameWindow(ChVec4::FromRect(DESCRIPTION_LEFT, DESCRIPTION_TOP, DESCRIPTION_RIGHT, DESCRIPTION_BOTTOM)));

	SPRITE_INIT(stageSelectPanelList.sprite, RectToGameWindow(ChVec4::FromRect(SELECT_PANEL_LEFT, SELECT_PANEL_LIST_TOP, SELECT_PANEL_RIGHT, SELECT_PANEL_LIST_BOTTOM)));
	stageSelectPanelList.image.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("SelectRange.png"), device);
	stagePanelBackground.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("StageNamePanel.png"), device);
	panelSelectImage.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("PanelSelect.png"), device);

	float top = SELECT_PANEL_LIST_TOP;
	float bottom = SELECT_PANEL_LIST_TOP + STAGE_SELECT_PANEL_HEIGHT;

	for (unsigned char i = 0; i < PANEL_DRAW_COUNT; i++)
	{

		SPRITE_INIT(selectStageSprite[i], RectToGameWindow(ChVec4::FromRect(SELECT_PANEL_LEFT, top, SELECT_PANEL_RIGHT, bottom)));

		top += STAGE_SELECT_PANEL_HEIGHT;
		bottom = top + STAGE_SELECT_PANEL_HEIGHT;
	}

}

void StageSelectDisplay::Release()
{

}

void StageSelectDisplay::Update()
{

	UpdateMouse();
}

void StageSelectDisplay::UpdateAction(const StageSelectFrame::ActionType& _type)
{

	if (_type == StageSelectFrame::ActionType::Decision)
	{
		if (selectSpriteType == SelectSpriteType::UpButton)
		{
			AddAction(StageSelectFrame::ActionType::UpSelect);
			return;
		}

		if (selectSpriteType == SelectSpriteType::DownButton)
		{
			AddAction(StageSelectFrame::ActionType::DownSelect);
			return;
		}
		if (GetNowSelectCount() >= GetStageDataList().size())return;
		SetDisplayType(StageSelectFrame::DisplayType::Detailed);
		return;
	}

	if (_type == StageSelectFrame::ActionType::UpSelect)
	{
		UpNowSelectStage();
		
		if (GetNowSelectCount() < drawNowSelect || GetNowSelectCount() + drawNowSelect >= PANEL_DRAW_COUNT)
		{
			drawNowSelect = GetNowSelectCount();
		}
	}

	if (_type == StageSelectFrame::ActionType::DownSelect)
	{
		DownNowSelectStage();

		if (GetNowSelectCount() >= drawNowSelect + PANEL_DRAW_COUNT)
		{
			drawNowSelect = GetNowSelectCount() + PANEL_DRAW_COUNT - 1;
		}
	}



}

void StageSelectDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)
{
	auto&& stageDataList = GetStageDataList();
	unsigned long drawCount = stageDataList.size();
	drawCount = drawCount > PANEL_DRAW_COUNT ? PANEL_DRAW_COUNT : drawCount;


	auto&& selectStageData = GetNowSelectCount();

	for (unsigned char i = 0; i < ChStd::EnumCast(StageSelectButtonType::None); i++)
	{
		if(ChStd::EnumCast(selectType) == i)
			_drawer.Draw(buttonSelectImage, stageSelectButton[i].sprite);

		_drawer.Draw(stageSelectButton[i].image, stageSelectButton[i].sprite);
	}

	_drawer.Draw(displayMap.image, displayMap.sprite);

	_drawer.Draw(panelSelectImage, selectStageSprite[(selectStageData - drawNowSelect) % PANEL_DRAW_COUNT]);


	for (unsigned long i = 0; i < drawCount; i++)
	{
		_drawer.Draw(stageDataList[(drawNowSelect + i) % stageDataList.size()]->selectPanel, selectStageSprite[i]);
	}

	_drawer.Draw(stageSelectPanelList.image, stageSelectPanelList.sprite);

	if (stageDataList.size() <= selectStageData)return;
	_drawer.Draw(stageDataList[selectStageData]->description, description);

}

void StageSelectDisplay::SetSelectData(const FromStageSelectFrameData& _selectData)
{
	drawNowSelect = _selectData.selectDrawTop;
}

void StageSelectDisplay::GetSelectData(FromStageSelectFrameData& _selectData)
{
	_selectData.selectDrawTop = drawNowSelect;
}

void StageSelectDisplay::UpdateMouse()
{

	auto&& manager = ChSystem::SysManager();

	if (manager.IsPushKeyNoHold(VK_LBUTTON))
	{
		AddAction(StageSelectFrame::ActionType::Decision);
	}

	if (manager.IsPushKeyNoHold(VK_RBUTTON))
	{
		AddAction(StageSelectFrame::ActionType::Cancel);
	}

	auto&& mouce = ChWin::Mouse();
	mouce.Update();

	auto&& mouseMove = mouce.GetMoveValue();

	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

	for (unsigned char i = 0; i < ChStd::EnumCast(StageSelectButtonType::None); i++)
	{
		if (!IsMoucePosOnSprite(stageSelectButton[i].sprite))continue;
		selectSpriteType = static_cast<SelectSpriteType>(i);
		selectType = static_cast<StageSelectButtonType>(i);
		return;
	}

	selectType = StageSelectButtonType::None;
	selectSpriteType = SelectSpriteType::StagePanel;

	for (unsigned long i = 0;i< PANEL_DRAW_COUNT ; i++)
	{
		if (!IsMoucePosOnSprite(selectStageSprite[i]))continue;

		SetNowStageSelect(i + drawNowSelect);
		break;
	}
}
