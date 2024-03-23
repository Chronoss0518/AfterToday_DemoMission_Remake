
#include"../BaseIncluder.h"

#include"StageSelectFrame.h"
#include"../StageSelectFrame/StageData/StageData.h"

#include"../LoadDisplay/LoadDisplay.h"

#include"../StageSelectFrame/StageSelectFrameDisplay/StageSelectDisplay.h"
#include"../StageSelectFrame/StageSelectFrameDisplay/StageDetailedDisplay.h"

#include"../LoadDisplay/LoadDisplay.h"

#define PANEL_TEXT_SIDE_PADDING 5.0f
#define PANEL_TEXT_WIDTH STAGE_SELECT_DESCRIPTION_WIDTH - (PANEL_TEXT_SIDE_PADDING * 2)
#define PANEL_TEXT_HEIGHT STAGE_SELECT_PANEL_HEIGHT * 0.5f

#define DESCRIPTION_TEXT_HEIGHT STAGE_SELECT_DESCRIPTION_HEIGHT * 0.5f

void StageSelectFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	ChD3D11::Shader11().SetBackColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));

	controller.Init();
	MenuBase::InitMenu(&controller);

	auto&& device = ChD3D11::D3D11Device();
	spriteShader.Init(device);

	loadDisplay = ChPtr::Make_S<LoadDisplay>();
	loadDisplay->Init(device, &controller);

	stageSelectFrameDisplay[ChStd::EnumCast(DisplayType::Select)] = ChPtr::Make_S<StageSelectDisplay>();
	stageSelectFrameDisplay[ChStd::EnumCast(DisplayType::Detailed)] = ChPtr::Make_S<StageDetailedDisplay>();
	
	controller.Init();
	controller.Update();

	notImageTexture.CreateColorTexture(device, ChVec4::FromColor(0.7f, 0.7f, 0.7f, 1.0f), 1, 1);

	InitStageDataList();

	for (auto&& display : stageSelectFrameDisplay)
	{
		display->SetStageSelectFrame(this);
		display->Init();
	}

	if (_sendData == nullptr)return;
	auto&& beforeData = ChPtr::SharedSafeCast<FromStageSelectFrameData>(_sendData);
	if (beforeData == nullptr)return;

	nowSelectStage = beforeData->selectStage;
	for (auto&& display : stageSelectFrameDisplay)
	{
		display->SetSelectData(*beforeData);
	}

}

void StageSelectFrame::InitStageDataList()
{
	auto&& device = ChD3D11::D3D11Device();

	ChD3D::DirectFontFromWICBitmap stagePanelTextDrawer;

	unsigned long stageSelectPanelWidth = static_cast<unsigned long>(STAGE_SELECT_PANEL_WIDTH);
	unsigned long stageSelectPanelHeight = static_cast<unsigned long>(STAGE_SELECT_PANEL_HEIGHT);

	ChD3D::WICBitmapObject stagePanelBitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(stageSelectPanelWidth, stageSelectPanelHeight);
	stagePanelTextDrawer.Init(stageSelectPanelWidth, stageSelectPanelHeight, stagePanelBitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	stagePanelTextDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	stagePanelTextDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject stagePanelBrush = stagePanelTextDrawer.CreateBrush(ChVec4::FromColor(0.0f, 0.0f, 0.0f,1.0f));

	ChD3D::TextFormatObject stagePanelNameFormat = stagePanelTextDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.0f);
	stagePanelNameFormat.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	stagePanelNameFormat.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	ChD3D::TextFormatObject stagePanelFeeFormat = stagePanelTextDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15.0f);
	stagePanelFeeFormat.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	stagePanelFeeFormat.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);



	ChD3D::DirectFontFromWICBitmap descriptionTextDrawer;

	unsigned long stageSelectDescriptionWidth = static_cast<unsigned long>(STAGE_SELECT_DESCRIPTION_WIDTH);
	unsigned long stageSelectDescriptionHeight = static_cast<unsigned long>(STAGE_SELECT_DESCRIPTION_HEIGHT);

	ChD3D::WICBitmapObject descriptionBitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(stageSelectDescriptionWidth, stageSelectDescriptionHeight);
	descriptionTextDrawer.Init(stageSelectDescriptionWidth, stageSelectDescriptionHeight, descriptionBitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	descriptionTextDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	descriptionTextDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject stageDescriptionBrush = descriptionTextDrawer.CreateBrush(ChVec4(1.0f));

	ChD3D::TextFormatObject stageDescriptionFormat = descriptionTextDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40.0f);
	stageDescriptionFormat.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	stageDescriptionFormat.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);



	for (auto&& file : std::filesystem::directory_iterator(STAGE_DATA_DIRECTORY("")))
	{
		std::string path = ChStr::UTF8ToString(file.path().c_str());
		auto stageData = ChPtr::Make_S<StageData>();
		stageData->LoadData(path);

		std::wstring successFee = L"$" + CreateMoneyText(std::to_wstring(stageData->stageDatas->drawSuccessFee));

		std::wstring location =
			L"作戦地域|" + ChStr::UTF8ToWString(stageData->stageDatas->stageLocationName);

		std::wstring hh = std::to_wstring(static_cast<unsigned long>(stageData->stageDatas->missionTimeSeccond / 60 / 60));
		if (hh.length() < 2)hh += L"0";
		std::wstring mm = std::to_wstring(static_cast<unsigned long>(stageData->stageDatas->missionTimeSeccond / 60 % 60));
		if (mm.length() < 2)mm += L"0";
		std::wstring ss = std::to_wstring(static_cast<unsigned long>(stageData->stageDatas->missionTimeSeccond % 60));
		if (ss.length() < 2)ss += L"0";

		std::wstring missionTime =
			L"作戦時間|" + hh + L":" + mm + L":" + ss;

		stagePanelTextDrawer.DrawStart();

		stagePanelTextDrawer.DrawToScreen(
			ChStr::UTF8ToWString(stageData->stageDatas->stageName), 
			stagePanelNameFormat, 
			stagePanelBrush, 
			ChVec4::FromRect(PANEL_TEXT_SIDE_PADDING, 0.0f, PANEL_TEXT_WIDTH, PANEL_TEXT_HEIGHT));

		stagePanelTextDrawer.DrawToScreen(
			successFee,
			stagePanelFeeFormat,
			stagePanelBrush,
			ChVec4::FromRect(PANEL_TEXT_SIDE_PADDING, PANEL_TEXT_HEIGHT, STAGE_SELECT_PANEL_WIDTH - 10.0f, PANEL_TEXT_HEIGHT + PANEL_TEXT_HEIGHT));
 
		stagePanelTextDrawer.DrawEnd();

		stageData->selectPanel.CreateColorTexture(device, stagePanelBitmap.GetBitmap());

		descriptionTextDrawer.DrawStart();

		descriptionTextDrawer.DrawToScreen(
			location,
			stageDescriptionFormat,
			stageDescriptionBrush,
			ChVec4::FromRect(0.0f, 0.0f, STAGE_SELECT_DESCRIPTION_WIDTH, DESCRIPTION_TEXT_HEIGHT));

		descriptionTextDrawer.DrawToScreen(
			missionTime,
			stageDescriptionFormat,
			stageDescriptionBrush,
			ChVec4::FromRect(0.0f, DESCRIPTION_TEXT_HEIGHT, STAGE_SELECT_DESCRIPTION_WIDTH, DESCRIPTION_TEXT_HEIGHT + DESCRIPTION_TEXT_HEIGHT));

		descriptionTextDrawer.DrawEnd();

		stageData->description.CreateColorTexture(device, descriptionBitmap.GetBitmap());

		stageDataList.push_back(stageData);
	}

	stagePanelTextDrawer.Release();

	stagePanelBitmap.Release();


	descriptionTextDrawer.Release();

	descriptionBitmap.Release();
}

void StageSelectFrame::Release()
{
	loadDisplay->Release();
	for (unsigned char i = 0; i < 2; i++)
	{
		stageSelectFrameDisplay[i]->Release();
		stageSelectFrameDisplay[i] = nullptr;
	}

	controller.Release();
}

void StageSelectFrame::DrawFunction()
{
	
	auto&& dc = ChD3D11::D3D11DC();
	ChD3D11::Shader11().DrawStart();

	dc->OMGetRenderTargets(1, &rtView, nullptr);

	spriteShader.DrawStart(dc);

	stageSelectFrameDisplay[ChStd::EnumCast(type)]->Draw(spriteShader);

	spriteShader.DrawEnd();

	dc->OMSetRenderTargets(1, &rtView, nullptr);

	spriteShader.DrawStart(dc);

	loadDisplay->Draw(spriteShader);

	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();

}

void StageSelectFrame::UpdateAction(ActionType _type)
{
	DisplayType beforeDisplayType = type;

	if (_type == ActionType::Cancel)
	{
		Cancel();
		SetLoopBreakTrue();
	}

	stageSelectFrameDisplay[ChStd::EnumCast(type)]->UpdateAction(_type);

	if (type != beforeDisplayType)
	{
		stageSelectFrameDisplay[ChStd::EnumCast(type)]->OnDisplay();
		SetLoopBreakTrue();
	}
}

void StageSelectFrame::UpdateMouse()
{
	stageSelectFrameDisplay[ChStd::EnumCast(type)]->Update();
}

void StageSelectFrame::SetGameFrame()
{
	SendData(stageDataList[nowSelectStage]->stageDatas);
	ChangeFrame(ChStd::EnumCast(FrameNo::Game));
}

void StageSelectFrame::SetEditFrame()
{
	auto&& stageSelectData = ChPtr::Make_S<FromStageSelectFrameData>();

	stageSelectData->selectStage = nowSelectStage;

	for (auto&& display : stageSelectFrameDisplay)
	{
		display->GetSelectData(*stageSelectData);
	}

	SendData(stageSelectData);
	//ChangeFrame(ChStd::EnumCast(FrameNo::Edit));
}

void StageSelectFrame::Update()
{

	if (!loadDisplay->Update())
	{
		UpdateFunction();
	}

	DrawFunction();
}

void StageSelectFrame::Cancel()
{
	if (type == DisplayType::Select)
		BaseFrame::ChangeFrame(ChStd::EnumCast(FrameNo::Select));

	type = DisplayType::Select;
}

void StageSelectFrame::OpenLoadDisplay()
{
	auto&& dc = ChD3D11::D3D11DC();
	auto&& playerData = ChPtr::SharedSafeCast<PlayerData>(GetData());
	loadDisplay->Open(dc);
}

void StageSelectFrame::StageSelectFrameDisplay::OpenLoadDisplay()
{
	frame->OpenLoadDisplay();
}

