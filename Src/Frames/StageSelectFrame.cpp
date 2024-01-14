
#include"../BaseIncluder.h"

#include"StageSelectFrame.h"
#include"../StageSelectFrame/StageData/StageData.h"

#define STAGE_IMAGE_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("StageImage/") current_path
#define SELECT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("StageSelect/") current_path


#define SELECT_PANEL_LEFT 45.0f

#define SELECT_PANEL_RIGHT 350.0f + SELECT_PANEL_LEFT
#define SELECT_PANEL_BUTTON_BOTTOM(TOP) TOP + 91.0f

#define SELECT_UP_BUTTON_TOP 32.0f
#define SELECT_DOWN_BUTTON_TOP 555.0f

#define SELECT_PANEL_LIST_TOP 134.0f
#define SELECT_PANEL_LIST_BOTTOM 408.0f + SELECT_PANEL_LIST_TOP

#define STAGE_NAME_X 285.0f
#define STAGE_NAME_Y 43.0f
#define STAGE_NAME_WIDTH 440.0f + STAGE_NAME_X
#define STAGE_NAME_HEIGHT 53.0f + STAGE_NAME_Y

#define STAGE_IMAGE_X 285.0f
#define STAGE_IMAGE_Y 113.0f
#define STAGE_IMAGE_WIDTH 440.0f + STAGE_IMAGE_X
#define STAGE_IMAGE_HEIGHT 328.0f + STAGE_IMAGE_Y

#define DISPLAY_MAP_POSITION_X 435.0f
#define DISPLAY_MAP_POSITION_Y 32.0f
#define DISPLAY_MAP_WIDTH 435.0f
#define DISPLAY_MAP_HEIGHT 32.0f

void StageSelectFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	ChD3D11::Shader11().SetBackColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));

	auto&& device = ChD3D11::D3D11Device();
	spriteShader.Init(device);

	InitStageSelectDisplay();

	SPRITE_INIT(stageImage, RectToGameWindow(ChVec4::FromRect(STAGE_IMAGE_X, STAGE_IMAGE_Y, STAGE_IMAGE_WIDTH, STAGE_IMAGE_HEIGHT)));

	SPRITE_INIT(stageName.sprite, RectToGameWindow(ChVec4::FromRect(STAGE_NAME_X, STAGE_NAME_Y, STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT)));

	notImageTexture.CreateColorTexture(device, ChVec4::FromColor(0.7f, 0.7f, 0.7f, 1.0f), 1, 1);

	stagePanelBackground.CreateTexture(SELECT_TEXTURE_DIRECTORY("StageNamePanel.png"), device);

	InitStageDataList();
}

void StageSelectFrame::InitStageDataList()
{
	ChD3D::DirectFontFromWICBitmap stagePanelTextDrawer;
	ChD3D::DirectFontFromWICBitmap descriptionTextDrawer;

	ChD3D::WICBitmapObject stagePanelBitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT);
	stagePanelTextDrawer.Init(STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT, stagePanelBitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	stagePanelTextDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	stagePanelTextDrawer.SetClearDisplayFlg(true);

	ChD3D::WICBitmapObject descriptionBitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT);
	descriptionTextDrawer.Init(STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT, descriptionBitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	descriptionTextDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	descriptionTextDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject stagePanelBrush = stagePanelTextDrawer.CreateBrush(ChVec4(1.0f));

	ChD3D::TextFormatObject stagePanelNameFormat = stagePanelTextDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.0f);
	stagePanelNameFormat.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	ChD3D::TextFormatObject stagePanelFeeFormat = stagePanelTextDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15.0f);
	stagePanelFeeFormat.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	ChD3D::BrushObject stageDescriptionBrush = stagePanelTextDrawer.CreateBrush(ChVec4(1.0f));

	ChD3D::TextFormatObject stageDescriptionFormat = stagePanelTextDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.0f);
	stageDescriptionFormat.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);


	ChPtr::Shared<ChD3D11::Texture11>stagePanelText = nullptr;

	ChPtr::Shared<ChD3D11::Texture11>drawSuccessFeeText = nullptr;

	for (auto&& file : std::filesystem::directory_iterator(STAGE_DATA_DIRECTORY("")))
	{
		stagePanelText = nullptr;
		drawSuccessFeeText = nullptr;

		std::string path = ChStr::UTF8ToString(file.path().c_str());
		auto stageData = ChPtr::Make_S<StageData>();
		stageData->LoadData(path);

		stagePanelTextDrawer.DrawStart();

		stagePanelTextDrawer.DrawToScreen(ChStr::UTF8ToWString(stageData->stageDatas->stageName), stagePanelNameFormat, stagePanelBrush, ChVec4::FromRect(0.0f, 0.0f, STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT));

		stagePanelTextDrawer.DrawEnd();

		stagePanelText = ChPtr::Make_S<ChD3D11::Texture11>();

		stagePanelText->CreateColorTexture(stagePanelBitmap.GetBitmap());

		stageDataList.push_back(stageData);
	}

	stagePanelTextDrawer.Release();

	stagePanelBitmap.Release();
}

void StageSelectFrame::InitStageData(const StageData& _stageData)
{
	ChD3D::DirectFontFromWICBitmap stageNameTextDrawer;

	ChD3D::WICBitmapObject stageNameBitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT);
	stageNameTextDrawer.Init(STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT, stageNameBitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	stageNameTextDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	stageNameTextDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject stageNameBrush = stageNameTextDrawer.CreateBrush(ChVec4(1.0f));
	ChD3D::TextFormatObject stageNameFormat = stageNameTextDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 38.0f);
	stageNameFormat.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	ChPtr::Shared<ChD3D11::Texture11>stageNameText = nullptr;

	ChPtr::Shared<ChD3D11::Texture11>drawSuccessFeeText = nullptr;

	stageNameTextDrawer.DrawStart();

	stageNameTextDrawer.DrawToScreen(ChStr::UTF8ToWString(_stageData.stageDatas->stageName), stageNameFormat, stageNameBrush, ChVec4::FromRect(0.0f, 0.0f, STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT));

	stageNameTextDrawer.DrawEnd();

	stageNameText = ChPtr::Make_S<ChD3D11::Texture11>();

	stageNameText->CreateColorTexture(stageNameBitmap.GetBitmap());

	stageNameTextDrawer.Release();

	stageNameBitmap.Release();

}

void StageSelectFrame::InitStageSelectDisplay()
{
	auto&& device = ChD3D11::D3D11Device();

	SPRITE_INIT(stageSelectUpButton.sprite, RectToGameWindow(ChVec4::FromRect(SELECT_PANEL_LEFT, SELECT_UP_BUTTON_TOP, SELECT_PANEL_RIGHT, SELECT_PANEL_BUTTON_BOTTOM(SELECT_UP_BUTTON_TOP))));
	stageSelectUpButton.image.CreateTexture(SELECT_TEXTURE_DIRECTORY("UPButton.png"), device);

	SPRITE_INIT(stageSelectDownButton.sprite, RectToGameWindow(ChVec4::FromRect(SELECT_PANEL_LEFT, SELECT_DOWN_BUTTON_TOP, SELECT_PANEL_RIGHT, SELECT_PANEL_BUTTON_BOTTOM(SELECT_DOWN_BUTTON_TOP))));
	stageSelectDownButton.image.CreateTexture(SELECT_TEXTURE_DIRECTORY("DownButton.png"), device);

	SPRITE_INIT(stageSelectPanelList.sprite, RectToGameWindow(ChVec4::FromRect(SELECT_PANEL_LEFT, SELECT_PANEL_LIST_TOP, SELECT_PANEL_RIGHT, SELECT_PANEL_LIST_BOTTOM)));

	SPRITE_INIT(displayMap.sprite, RectToGameWindow(ChVec4::FromRect(STAGE_IMAGE_X, STAGE_IMAGE_Y, STAGE_IMAGE_WIDTH, STAGE_IMAGE_HEIGHT)));
	displayMap.image.CreateTexture(SELECT_TEXTURE_DIRECTORY("Map(NoImage).png"), device);

	SPRITE_INIT(description, RectToGameWindow(ChVec4::FromRect(STAGE_IMAGE_X, STAGE_IMAGE_Y, STAGE_IMAGE_WIDTH, STAGE_IMAGE_HEIGHT)));
}

void StageSelectFrame::Release()
{

}

void StageSelectFrame::DrawFunction()
{
	auto&& dc = ChD3D11::D3D11DC();

	ChD3D11::Shader11().DrawStart();

	spriteShader.DrawStart(dc);

	DrawStageSelect();

	DrawStageData();

	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();

}

void StageSelectFrame::DrawStageData()
{
	if (type != DisplayType::Detalied)return;

	auto&& selectStageData = *stageDataList[nowSelectStage];

	ChD3D11::TextureBase11* testImage = &selectStageData.stageImage;
	if (!testImage->IsTex())testImage = &notImageTexture;

	spriteShader.Draw(*testImage, stageImage);

	if (stageName.textImage != nullptr)
	{
		spriteShader.Draw(*stageName.textImage, stageName.sprite);
	}

}

void StageSelectFrame::DrawStageSelect()
{
	if (type != DisplayType::Select)return;


	spriteShader.Draw(stageSelectUpButton.image, stageSelectUpButton.sprite);

	spriteShader.Draw(stageSelectDownButton.image, stageSelectDownButton.sprite);

	spriteShader.Draw(stageSelectDownButton.image, stageSelectDownButton.sprite);
	
	spriteShader.Draw(stageSelectDownButton.image, stageSelectDownButton.sprite);

	spriteShader.Draw(stageSelectDownButton.image, stageSelectDownButton.sprite);

}

void StageSelectFrame::UpdateFunction()
{

	UpdateKeyboard();
	UpdateMouse();

	for (unsigned long i = 0;i < inputDataList.size(); i++)
	{
		auto&& inputData = inputDataList[i];
		if (inputData == ActionType::Cancel)
		{
			if (type == DisplayType::Select)
				BaseFrame::ChangeFrame(ChStd::EnumCast(FrameNo::Select));

			type = DisplayType::Select;
			break;
		}

		UpdateStageSelect(inputData);
		UpdateStageData(inputData);

	}

	inputDataList.clear();
}

void StageSelectFrame::UpdateStageData(const ActionType& _type)
{
	if (type != DisplayType::Detalied)return;
}

void StageSelectFrame::UpdateStageSelect(const ActionType& _type)
{
	if (type != DisplayType::Select)return;

	if (_type == ActionType::Decision)
	{
		if (selectSpriteType == SelectSpriteType::UpButton)
		{
			inputDataList.push_back(ActionType::UpSelect);
			return;
		}

		if (selectSpriteType == SelectSpriteType::DownButton)
		{
			inputDataList.push_back(ActionType::UpSelect);
			return;
		}

		type = DisplayType::Detalied;
		InitStageData(*stageDataList[nowSelectStage]);
		return;
	}

	if (_type == ActionType::UpSelect)
	{
		nowSelectStage--;
	}

	if (_type == ActionType::DownSelect)
	{
		nowSelectStage++;
	}

	nowSelectStage = (nowSelectStage + stageDataList.size()) % stageDataList.size();
}

void StageSelectFrame::Update()
{
	if (firstFlg)
	{
		inputDataList.clear();
		firstFlg = false;
		return;
	}

	UpdateFunction();

	DrawFunction();
}

void StageSelectFrame::UpdateMouse()
{

	auto&& manager = ChSystem::SysManager();

	if (manager.IsPushKeyNoHold(VK_LBUTTON))
	{
		inputDataList.push_back(ActionType::Decision);
	}

	auto&& mouce = ChWin::Mouse();
	mouce.Update();

	auto&& mouseMove = mouce.GetMoveValue();

	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

	if (IsMoucePosOnSprite(stageSelectUpButton.sprite))
	{
		selectSpriteType = SelectSpriteType::UpButton;
		return;
	}

	if (IsMoucePosOnSprite(stageSelectDownButton.sprite))
	{
		selectSpriteType = SelectSpriteType::DownButton;
		return;
	}

	for (auto&& stageData : stageDataList)
	{
		if (IsMoucePosOnSprite(stageSelectDownButton.sprite))
		{

		}
	}
}

void StageSelectFrame::UpdateKeyboard()
{

	auto&& manager = ChSystem::SysManager();

	if (manager.IsPushKeyNoHold(VK_RETURN))
	{
		inputDataList.push_back(ActionType::Decision);
	}

	if (manager.IsPushKeyNoHold(VK_UP))
	{
		inputDataList.push_back(ActionType::UpSelect);
	}

	if (manager.IsPushKeyNoHold(VK_DOWN))
	{
		inputDataList.push_back(ActionType::DownSelect);
	}
}

void StageSelectFrame::UpdateController()
{

}