
#include"../../BaseIncluder.h"

#include"StageDetailedDisplay.h"
#include"../StageData/StageData.h"


#define STAGE_NAME_LEFT 285.0f
#define STAGE_NAME_TOP 43.0f
#define STAGE_NAME_WIDTH 440.0f
#define STAGE_NAME_HEIGHT 53.0f

#define STAGE_DESCRIPTION_LEFT 284.0f
#define STAGE_DESCRIPTION_TOP 492.0f
#define STAGE_DESCRIPTION_TEXT_WIDTH 440.0f
#define STAGE_DESCRIPTION_TEXT_HEIGHT 51.0f
#define STAGE_DESCRIPTION_TEXT_LINE_BETWEEN 26.0f
#define STAGE_DESCRIPTION_RIGHT STAGE_DESCRIPTION_TEXT_WIDTH + STAGE_DESCRIPTION_LEFT
#define STAGE_DESCRIPTION_BOTTOM STAGE_DESCRIPTION_TOP + STAGE_DESCRIPTION_TEXT_HEIGHT * 2 + STAGE_DESCRIPTION_TEXT_LINE_BETWEEN

#define STAGE_IMAGE_LEFT 285.0f
#define STAGE_IMAGE_TOP 113.0f
#define STAGE_IMAGE_WIDTH 440.0f
#define STAGE_IMAGE_HEIGHT 328.0f

#define MENU_PANEL_LEFT 45.0f
#define MENU_PANEL_RIGHT MENU_PANEL_LEFT + 179.0f
#define MENU_PANEL_TOP 43.0f
#define MENU_PANEL_HEIGHT 100.0f

#define STAGE_STRATEGY_OVERVIEW_LEFT 783.0f
#define STAGE_STRATEGY_OVERVIEW_TOP 32.0f
#define STAGE_STRATEGY_OVERVIEW_RIGHT 452.0f + STAGE_STRATEGY_OVERVIEW_LEFT
#define STAGE_STRATEGY_OVERVIEW_BOTTOM 588.0f + STAGE_STRATEGY_OVERVIEW_TOP

#define STAGE_STRATEGY_OVERVIEW_TEXT_TOP 105.0f
#define STAGE_STRATEGY_OVERVIEW_TEXT_BOTTOM 515.0f + STAGE_STRATEGY_OVERVIEW_TOP

void StageDetailedDisplay::Init()
{

	auto&& device = ChD3D11::D3D11Device();

	SPRITE_INIT(stageImage, RectToGameWindow(ChVec4::FromRect(+STAGE_IMAGE_LEFT, +STAGE_IMAGE_TOP, STAGE_IMAGE_WIDTH + STAGE_IMAGE_LEFT, STAGE_IMAGE_HEIGHT + STAGE_IMAGE_TOP)));

	SPRITE_INIT(stageName.sprite, RectToGameWindow(ChVec4::FromRect(STAGE_NAME_LEFT, STAGE_NAME_TOP, STAGE_NAME_WIDTH + STAGE_NAME_LEFT, STAGE_NAME_HEIGHT + STAGE_NAME_TOP)));

	SPRITE_INIT(stageDescription.sprite, RectToGameWindow(ChVec4::FromRect(STAGE_DESCRIPTION_LEFT, STAGE_DESCRIPTION_TOP, STAGE_DESCRIPTION_RIGHT , STAGE_DESCRIPTION_BOTTOM)));
	
	SPRITE_INIT(strategyOverview.sprite, RectToGameWindow(ChVec4::FromRect(STAGE_STRATEGY_OVERVIEW_LEFT, STAGE_STRATEGY_OVERVIEW_TOP, STAGE_STRATEGY_OVERVIEW_RIGHT, STAGE_STRATEGY_OVERVIEW_BOTTOM)));
	
	strategyOverview.image.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("StrategyOverview.png"), device);
	SPRITE_INIT(strategyOverviewText.sprite, RectToGameWindow(ChVec4::FromRect(STAGE_STRATEGY_OVERVIEW_LEFT, STAGE_STRATEGY_OVERVIEW_TEXT_TOP, STAGE_STRATEGY_OVERVIEW_RIGHT, STAGE_STRATEGY_OVERVIEW_TEXT_BOTTOM)));


	button[ChStd::EnumCast(MenuButtonType::Start)].image.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("Start.png"), device);
	button[ChStd::EnumCast(MenuButtonType::Edit)].image.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("Edit.png"), device);
	button[ChStd::EnumCast(MenuButtonType::Load)].image.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("Load.png"), device);
	button[ChStd::EnumCast(MenuButtonType::Cancel)].image.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("Cancel.png"), device);

	buttonSelect.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("MenuSelect.png"), device);
	comingSoonImage.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("Coming Soon.png"), device);
	unUsedMethodImage.CreateTexture(STAGE_SELECT_TEXTURE_DIRECTORY("UnUsedText.png"), device);

	float top = MENU_PANEL_TOP;
	float bottom = top + MENU_PANEL_HEIGHT;

	for (unsigned char i = 0; i < MENU_BUTTON_TYPE; i++)
	{
		SPRITE_INIT(button[i].sprite, RectToGameWindow(ChVec4::FromRect(MENU_PANEL_LEFT, top, MENU_PANEL_RIGHT, bottom)));
		top += MENU_PANEL_HEIGHT;
		bottom = top + MENU_PANEL_HEIGHT;
	}

}

void StageDetailedDisplay::Release()
{

}

void StageDetailedDisplay::Update()
{
	UpdateMouse();
}

void StageDetailedDisplay::UpdateAction(MenuBase::ActionType _type)
{

	if (_type == MenuBase::ActionType::Decision)
	{
		if (nowSelect == ChStd::EnumCast(MenuButtonType::Start))
			SetGameFrame();

		if (nowSelect == ChStd::EnumCast(MenuButtonType::Edit))
			Edit();

		if (nowSelect == ChStd::EnumCast(MenuButtonType::Load))
			Load();

		if (nowSelect == ChStd::EnumCast(MenuButtonType::Cancel))
			Cancel();
	}

	if (_type == MenuBase::ActionType::Up)
	{
		nowSelect = (nowSelect + MENU_BUTTON_TYPE - 1) % MENU_BUTTON_TYPE;
	}

	if (_type == MenuBase::ActionType::Down)
	{
		nowSelect = (nowSelect + 1) % MENU_BUTTON_TYPE;
	}

}

void StageDetailedDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)
{

	auto&& selectStageData = GetNowSelectStageData();

	ChD3D11::TextureBase11* testImage = &selectStageData.stageImage;
	if (!testImage->IsTex())testImage = &GetNonImage();

	_drawer.Draw(*testImage, stageImage);

	_drawer.Draw(stageName.image, stageName.sprite);

	_drawer.Draw(stageDescription.image, stageDescription.sprite);

	_drawer.Draw(strategyOverviewText.image, strategyOverviewText.sprite);

	_drawer.Draw(strategyOverview.image, strategyOverview.sprite);

	_drawer.Draw(buttonSelect, button[nowSelect].sprite);

	for (unsigned char i = 0; i < MENU_BUTTON_TYPE; i++)
	{
		//if (i == ChStd::EnumCast(MenuButtonType::Edit))_drawer.Draw(comingSoonImage, button[i].sprite);

		if ((i == ChStd::EnumCast(MenuButtonType::Edit) ||
			i == ChStd::EnumCast(MenuButtonType::Load)) && 
			!selectStageData.stageDatas->selectModelFlg)
			_drawer.Draw(unUsedMethodImage, button[i].sprite);

		_drawer.Draw(button[i].image, button[i].sprite);
	}
}

void StageDetailedDisplay::SetSelectData(const FromStageSelectFrameData& _selectData)
{
	nowSelect = _selectData.selectMenu;
}

void StageDetailedDisplay::GetSelectData(FromStageSelectFrameData& _selectData)
{
	_selectData.selectMenu = nowSelect;
}

void StageDetailedDisplay::OnDisplay()
{
	auto&& stageData = GetNowSelectStageData();

	CreateStageNameTextImage(stageData);
	CreateStageDescriptionTextImage(stageData);
	CreateStageStrategyOverviewTextImage(stageData);

	useLoadAndEdit = stageData.stageDatas->selectModelFlg;
}

void StageDetailedDisplay::UpdateMouse()
{
	auto&& manager = ChSystem::SysManager();

	AddAction(StageSelectFrame::ActionType::Decision, manager.IsPushKeyNoHold(VK_LBUTTON));

	AddAction(StageSelectFrame::ActionType::Cancel, manager.IsPushKeyNoHold(VK_RBUTTON));


	auto&& mouce = ChWin::Mouse();
	mouce.Update();

	auto&& mouseMove = mouce.GetMoveValue();

	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

	for (unsigned char i = 0; i < MENU_BUTTON_TYPE; i++)
	{
		if (!IsMoucePosOnSprite(button[i].sprite))continue;

		nowSelect = i;
		break;
	}

}

void StageDetailedDisplay::CreateStageNameTextImage(StageData& _stageData)
{
	ChD3D::DirectFontFromWICBitmap textDrawer;

	unsigned long w = static_cast<unsigned long>(STAGE_NAME_WIDTH);
	unsigned long h = static_cast<unsigned long>(STAGE_NAME_HEIGHT);

	ChD3D::WICBitmapObject bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(w, h);
	textDrawer.Init(w, h, bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	textDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	textDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject brush = textDrawer.CreateBrush(ChVec4(1.0f));
	ChD3D::TextFormatObject format = textDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40.0f);
	format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	textDrawer.DrawStart();

	textDrawer.DrawToScreen(ChStr::UTF8ToWString(_stageData.stageDatas->stageName), format, brush, ChVec4::FromRect(0.0f, 0.0f, STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT));

	textDrawer.DrawEnd();

	stageName.image.CreateColorTexture(bitmap.GetBitmap());

	textDrawer.Release();

	bitmap.Release();


}

void StageDetailedDisplay::CreateStageDescriptionTextImage(StageData& _stageData)
{
	ChD3D::DirectFontFromWICBitmap textDrawer;

	unsigned long w = static_cast<unsigned long>(STAGE_DESCRIPTION_RIGHT - STAGE_DESCRIPTION_LEFT);
	unsigned long h = static_cast<unsigned long>(STAGE_DESCRIPTION_BOTTOM - STAGE_DESCRIPTION_TOP);

	ChD3D::WICBitmapObject bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(w, h);
	textDrawer.Init(w, h, bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	textDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	textDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject brush = textDrawer.CreateBrush(ChVec4(1.0f));
	ChD3D::TextFormatObject format = textDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40.0f);
	format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);;

	std::wstring successFee = L"成功報酬|" + CreateMoneyText(std::to_wstring(_stageData.stageDatas->drawSuccessFee));

	std::wstring hh = std::to_wstring(static_cast<unsigned long>(_stageData.stageDatas->missionTimeSeccond / 60 / 60));
	if (hh.length() < 2)hh += L"0";
	std::wstring mm = std::to_wstring(static_cast<unsigned long>(_stageData.stageDatas->missionTimeSeccond / 60 % 60));
	if (mm.length() < 2)mm += L"0";
	std::wstring ss = std::to_wstring(static_cast<unsigned long>(_stageData.stageDatas->missionTimeSeccond % 60));
	if (ss.length() < 2)ss += L"0";

	std::wstring missionTime =
		L"作戦時間|" + hh + L":" + mm + L":" + ss;

	textDrawer.DrawStart();

	textDrawer.DrawToScreen(successFee, format, brush, ChVec4::FromRect(0.0f, 0.0f, STAGE_DESCRIPTION_TEXT_WIDTH, STAGE_DESCRIPTION_TEXT_HEIGHT));

	textDrawer.DrawToScreen(missionTime, format, brush, ChVec4::FromRect(0.0f, STAGE_DESCRIPTION_TEXT_HEIGHT + STAGE_DESCRIPTION_TEXT_LINE_BETWEEN, STAGE_DESCRIPTION_TEXT_WIDTH, STAGE_DESCRIPTION_TEXT_HEIGHT + STAGE_DESCRIPTION_TEXT_HEIGHT + STAGE_DESCRIPTION_TEXT_LINE_BETWEEN));

	textDrawer.DrawEnd();

	stageDescription.image.CreateColorTexture(bitmap.GetBitmap());

	textDrawer.Release();

	bitmap.Release();
}

void StageDetailedDisplay::CreateStageStrategyOverviewTextImage(StageData& _stageData)
{
	ChD3D::DirectFontFromWICBitmap textDrawer;

	unsigned long w = static_cast<unsigned long>(STAGE_STRATEGY_OVERVIEW_RIGHT - STAGE_STRATEGY_OVERVIEW_LEFT);
	unsigned long h = static_cast<unsigned long>(STAGE_STRATEGY_OVERVIEW_TEXT_BOTTOM - STAGE_STRATEGY_OVERVIEW_TEXT_TOP);

	ChD3D::WICBitmapObject bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(w, h);
	textDrawer.Init(w, h, bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	textDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	textDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject brush = textDrawer.CreateBrush(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));
	ChD3D::TextFormatObject format = textDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30.0f);
	format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	textDrawer.DrawStart();

	textDrawer.DrawToScreen(ChStr::UTF8ToWString(_stageData.stageDatas->stageStrategyOverview), format, brush, ChVec4::FromRect(0.0f, 0.0f, STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT));

	textDrawer.DrawEnd();

	strategyOverviewText.image.CreateColorTexture(bitmap.GetBitmap());

	textDrawer.Release();

	bitmap.Release();


}

void StageDetailedDisplay::Load()
{
	if (!useLoadAndEdit)return;
	OpenLoadDisplay();
}

void StageDetailedDisplay::Edit()
{
	if (!useLoadAndEdit)return;
	SetEditFrame();
}
