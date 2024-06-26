
#include"../../BaseIncluder.h"

#include"StageSelectDisplay.h"
#include"../StageData/StageData.h"
#include"../../SelectList/SelectList.h"

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

class StageSelectListItem : public SelectListItemBase
{
public:

	ChPtr::Shared<StageData> stageData = nullptr;
};

class StageSelectList :public SelectListBase
{
public:

	StageSelectList()
	{
		sprite.Init();
	}

public:

	void SetSelectImage(ChD3D11::Texture11* _selectImage)
	{
		if (ChPtr::NullCheck(_selectImage))return;
		selectImage = _selectImage;
	}

public:

	void DrawSelect(ChD3D11::Shader::BaseDrawSprite11& _drawer, bool _isSelectPanel)
	{
		if (!_isSelectPanel)return;
		if (ChPtr::NullCheck(selectImage))return;

		_drawer.Draw(*selectImage, sprite);

	}	

	void DrawPanel(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect, ChPtr::Shared<SelectListItemBase> _drawItem, unsigned long _itemNo, bool _isSelectPanel)override
	{
		auto&& item = ChPtr::SharedSafeCast<StageSelectListItem>(_drawItem);
		if (item == nullptr)return;

		sprite.SetPosRect(RectToGameWindow(_rect));

		DrawSelect(_drawer, _isSelectPanel);

		_drawer.Draw(item->stageData->selectPanel, sprite);

	}

private:

	ChD3D11::Texture11* selectImage = nullptr;
	ChD3D11::Sprite11 sprite;
};

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

	selectList = ChPtr::Make_S<StageSelectList>();
	selectList->Init();
	selectList->SetStartPosition(SELECT_PANEL_LEFT, SELECT_PANEL_LIST_TOP);
	selectList->SetPanelSize(STAGE_SELECT_PANEL_WIDTH, STAGE_SELECT_PANEL_HEIGHT);
	selectList->SetAlighSize(0.0f, STAGE_SELECT_PANEL_HEIGHT);
	selectList->SetDrawCount(PANEL_DRAW_COUNT);

	selectList->SetSelectImage(&panelSelectImage);

	auto&& stageList = GetStageDataList();

	for (unsigned long i = 0; i < stageList.size(); i++)
	{
		auto&& item = ChPtr::Make_S<StageSelectListItem>();
		item->stageData = stageList[i];
		selectList->AddItem(item);
	}
}

void StageSelectDisplay::Release()
{
	selectList->ClearItem();
	selectList = nullptr;
}

void StageSelectDisplay::Update()
{
	UpdateMouse();
}

void StageSelectDisplay::UpdateAction(MenuBase::ActionType _type)
{

	if (_type == MenuBase::ActionType::Decision)
	{
		if (selectType == StageSelectButtonType::Up)
		{
			AddAction(MenuBase::ActionType::Up);
			return;
		}

		if (selectType == StageSelectButtonType::Down)
		{
			AddAction(MenuBase::ActionType::Down);
			return;
		}
		if (GetNowSelectCount() >= GetStageDataList().size())return;
		SetDisplayType(StageSelectFrame::DisplayType::Detailed);
		return;
	}

	selectList->UpdateAction(_type);

	SetNowStageSelect(selectList->GetNowSelect());

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


	selectList->Draw(_drawer);

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


	AddAction(MenuBase::ActionType::Decision, manager.IsPushKeyNoHold(VK_LBUTTON));

	AddAction(MenuBase::ActionType::Cancel, manager.IsPushKeyNoHold(VK_RBUTTON));

	auto&& mouce = ChWin::Mouse();
	mouce.Update();

	auto&& point = mouce.GetWheelMove();

	if (point.y > 0)
	{
		point.y = 5;
	}

	auto&& mouseMove = mouce.GetMoveValue();

	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

	for (unsigned char i = 0; i < ChStd::EnumCast(StageSelectButtonType::None); i++)
	{
		if (!IsMoucePosOnSprite(stageSelectButton[i].sprite))continue;
		selectType = static_cast<StageSelectButtonType>(i);
		return;
	}

	selectType = StageSelectButtonType::None;

	if (selectList->UpdateMouse())
	{
		SetNowStageSelect(selectList->GetNowSelect());
	}
}
