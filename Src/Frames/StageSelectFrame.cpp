
#include"../BaseIncluder.h"

#include"StageSelectFrame.h"
#include"../StageSelectFrame/StageData/StageData.h"

#define SELECT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("StageImage/") current_path

#define STAGE_PRE_IMAGE_TOP SCREEN_TO_PROJECTION_Y(580)
#define STAGE_PRE_IMAGE_BOTTOM SCREEN_TO_PROJECTION_Y(580 + 100)

#define SRTAGE_PRE_IMAGE_WIDTH 200

#define STAGE_PRE_IMAGE_LEFT0 SCREEN_TO_PROJECTION_X(120)
#define STAGE_PRE_IMAGE_RIGHT0 SCREEN_TO_PROJECTION_X(120 + SRTAGE_PRE_IMAGE_WIDTH)
#define STAGE_PRE_IMAGE_LEFT1 SCREEN_TO_PROJECTION_X(330)
#define STAGE_PRE_IMAGE_RIGHT1 SCREEN_TO_PROJECTION_X(330 + SRTAGE_PRE_IMAGE_WIDTH)
#define STAGE_PRE_IMAGE_LEFT2 SCREEN_TO_PROJECTION_X(540)
#define STAGE_PRE_IMAGE_RIGHT2 SCREEN_TO_PROJECTION_X(540 + SRTAGE_PRE_IMAGE_WIDTH)
#define STAGE_PRE_IMAGE_LEFT3 SCREEN_TO_PROJECTION_X(750)
#define STAGE_PRE_IMAGE_RIGHT3 SCREEN_TO_PROJECTION_X(750 + SRTAGE_PRE_IMAGE_WIDTH)
#define STAGE_PRE_IMAGE_LEFT4 SCREEN_TO_PROJECTION_X(960)
#define STAGE_PRE_IMAGE_RIGHT4 SCREEN_TO_PROJECTION_X(960 + SRTAGE_PRE_IMAGE_WIDTH)

#define STAGE_PRE_IMAGE_LEFT {STAGE_PRE_IMAGE_LEFT0,STAGE_PRE_IMAGE_LEFT1,STAGE_PRE_IMAGE_LEFT2,STAGE_PRE_IMAGE_LEFT3,STAGE_PRE_IMAGE_LEFT4}
#define STAGE_PRE_IMAGE_RIGHT {STAGE_PRE_IMAGE_RIGHT0,STAGE_PRE_IMAGE_RIGHT1,STAGE_PRE_IMAGE_RIGHT2,STAGE_PRE_IMAGE_RIGHT3,STAGE_PRE_IMAGE_RIGHT4}

#define TARGET_IMAGE_WIDTH 140
#define TAREGT_IMAGE_HEIGHT 80

#define TARGET_IMAGE_TOP0 SCREEN_TO_PROJECTION_Y(280)
#define TARGET_IMAGE_BOTTOM0 SCREEN_TO_PROJECTION_Y(280 + TAREGT_IMAGE_HEIGHT)
#define TARGET_IMAGE_TOP1 SCREEN_TO_PROJECTION_Y(370)
#define TARGET_IMAGE_BOTTOM1 SCREEN_TO_PROJECTION_Y(370 + TAREGT_IMAGE_HEIGHT)
#define TARGET_IMAGE_TOP2 SCREEN_TO_PROJECTION_Y(460)
#define TARGET_IMAGE_BOTTOM2 SCREEN_TO_PROJECTION_Y(460 + TAREGT_IMAGE_HEIGHT)

#define TARGET_IMAGE_LEFT0 SCREEN_TO_PROJECTION_X(790)
#define TARGET_IMAGE_RIGHT0 SCREEN_TO_PROJECTION_X(790 + TARGET_IMAGE_WIDTH)
#define TARGET_IMAGE_LEFT1 SCREEN_TO_PROJECTION_X(940)
#define TARGET_IMAGE_RIGHT1 SCREEN_TO_PROJECTION_X(940 + TARGET_IMAGE_WIDTH)
#define TARGET_IMAGE_LEFT2 SCREEN_TO_PROJECTION_X(1090)
#define TARGET_IMAGE_RIGHT2 SCREEN_TO_PROJECTION_X(1090 + TARGET_IMAGE_WIDTH)

#define TARGET_IMAGE_LEFT {TARGET_IMAGE_LEFT0,TARGET_IMAGE_LEFT1,TARGET_IMAGE_LEFT2}
#define TARGET_IMAGE_RIGHT {TARGET_IMAGE_RIGHT0,TARGET_IMAGE_RIGHT1,TARGET_IMAGE_RIGHT2}
#define TARGET_IMAGE_TOP {TARGET_IMAGE_TOP0,TARGET_IMAGE_TOP1,TARGET_IMAGE_TOP2}
#define TARGET_IMAGE_BOTTOM {TARGET_IMAGE_BOTTOM0,TARGET_IMAGE_BOTTOM1,TARGET_IMAGE_BOTTOM2}

#define TO_FLUAT(num) num##.0f

#define STAGE_NAME_DRAW_WIDTH 540
#define STAGE_NAME_DRAW_HEIGHT 53
#define STAGE_NAME_DRAW_X 200
#define STAGE_NAME_DRAW_Y 23

#define TARGET_NAME_DRAW_WIDTH 300
#define TARGET_NAME_DRAW_HEIGHT 38
#define TARGET_NAME_DRAW_X 860
#define TARGET_NAME_DRAW_Y 52


void StageSelectFrame::Init()
{
	ChD3D11::Shader11().SetBackColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));

	auto&& device = ChD3D11::D3D11Device();
	spriteShader.Init(device);

	stageImageSprite.Init(device);
	stageImageSprite.SetInitPosition();

	stageImageSprite.SetPos(ChD3D11::SpritePositionName::LeftTop, ChVec2(SCREEN_TO_PROJECTION_X(200), SCREEN_TO_PROJECTION_Y(100)));
	stageImageSprite.SetPos(ChD3D11::SpritePositionName::RightTop, ChVec2(SCREEN_TO_PROJECTION_X(200 + 540), SCREEN_TO_PROJECTION_Y(100)));
	stageImageSprite.SetPos(ChD3D11::SpritePositionName::LeftBottom, ChVec2(SCREEN_TO_PROJECTION_X(200), SCREEN_TO_PROJECTION_Y(100 + 400)));
	stageImageSprite.SetPos(ChD3D11::SpritePositionName::RightBottom, ChVec2(SCREEN_TO_PROJECTION_X(200 + 540), SCREEN_TO_PROJECTION_Y(100 + 400)));


	stageNameSprite.Init(device);
	stageNameSprite.SetInitPosition();

	stageNameSprite.SetPos(ChD3D11::SpritePositionName::LeftTop, ChVec2(SCREEN_TO_PROJECTION_X(200), SCREEN_TO_PROJECTION_Y(23)));
	stageNameSprite.SetPos(ChD3D11::SpritePositionName::RightTop, ChVec2(SCREEN_TO_PROJECTION_X(200 + 540), SCREEN_TO_PROJECTION_Y(23)));
	stageNameSprite.SetPos(ChD3D11::SpritePositionName::LeftBottom, ChVec2(SCREEN_TO_PROJECTION_X(200), SCREEN_TO_PROJECTION_Y(23 + 53)));
	stageNameSprite.SetPos(ChD3D11::SpritePositionName::RightBottom, ChVec2(SCREEN_TO_PROJECTION_X(200 + 540), SCREEN_TO_PROJECTION_Y(23 + 53)));


	targetNameSprite.Init(device);
	targetNameSprite.SetInitPosition();

	targetNameSprite.SetPos(ChD3D11::SpritePositionName::LeftTop, ChVec2(SCREEN_TO_PROJECTION_X(860), SCREEN_TO_PROJECTION_Y(52)));
	targetNameSprite.SetPos(ChD3D11::SpritePositionName::RightTop, ChVec2(SCREEN_TO_PROJECTION_X(860 + 300), SCREEN_TO_PROJECTION_Y(52)));
	targetNameSprite.SetPos(ChD3D11::SpritePositionName::LeftBottom, ChVec2(SCREEN_TO_PROJECTION_X(860), SCREEN_TO_PROJECTION_Y(52 + 38)));
	targetNameSprite.SetPos(ChD3D11::SpritePositionName::RightBottom, ChVec2(SCREEN_TO_PROJECTION_X(860 + 300), SCREEN_TO_PROJECTION_Y(52 + 38)));

	notImageTexture.CreateColorTexture(device, ChVec4::FromColor(0.7f, 0.7f, 0.7f, 1.0f), 1, 1);

	InitStagePreImageSpritePosition(device);
	InitTargetPreImageSpritePosition(device);

	InitStageData();

}

void StageSelectFrame::InitStagePreImageSpritePosition(ID3D11Device* _device)
{

	float leftPosition[] = STAGE_PRE_IMAGE_LEFT;
	float rightPosition[] = STAGE_PRE_IMAGE_RIGHT;

	ChVec4 rect;

	for (unsigned long i = 0; i < sizeof(rightPosition) / sizeof(float); i++)
	{
		stagePreImageSprite[i].Init(_device);

		stagePreImageSprite[i].SetPos(ChD3D11::SpritePositionName::LeftTop, ChVec2(leftPosition[i], STAGE_PRE_IMAGE_TOP));
		stagePreImageSprite[i].SetPos(ChD3D11::SpritePositionName::RightTop, ChVec2(rightPosition[i], STAGE_PRE_IMAGE_TOP));
		stagePreImageSprite[i].SetPos(ChD3D11::SpritePositionName::LeftBottom, ChVec2(leftPosition[i], STAGE_PRE_IMAGE_BOTTOM));
		stagePreImageSprite[i].SetPos(ChD3D11::SpritePositionName::RightBottom, ChVec2(rightPosition[i], STAGE_PRE_IMAGE_BOTTOM));
	}
}

void StageSelectFrame::InitTargetPreImageSpritePosition(ID3D11Device* _device)
{
	targetImageSprite[0].Init(_device);

	targetImageSprite[0].SetPos(ChD3D11::SpritePositionName::LeftTop, ChVec2(SCREEN_TO_PROJECTION_X(860), SCREEN_TO_PROJECTION_Y(100)));
	targetImageSprite[0].SetPos(ChD3D11::SpritePositionName::RightTop, ChVec2(SCREEN_TO_PROJECTION_X(860 + 300), SCREEN_TO_PROJECTION_Y(100)));
	targetImageSprite[0].SetPos(ChD3D11::SpritePositionName::LeftBottom, ChVec2(SCREEN_TO_PROJECTION_X(860), SCREEN_TO_PROJECTION_Y(100 + 160)));
	targetImageSprite[0].SetPos(ChD3D11::SpritePositionName::RightBottom, ChVec2(SCREEN_TO_PROJECTION_X(860 + 300), SCREEN_TO_PROJECTION_Y(100 + 160)));

	float leftPosition[] = TARGET_IMAGE_LEFT;
	float rightPosition[] = TARGET_IMAGE_RIGHT;
	float topPosition[] = TARGET_IMAGE_TOP;
	float bottomPosition[] = TARGET_IMAGE_BOTTOM;

	unsigned long wCount = sizeof(rightPosition) / sizeof(float);
	unsigned long hCount = sizeof(bottomPosition) / sizeof(float);

	for (unsigned long h = 0; h < hCount; h++)
	{
		for (unsigned long w = 0; w < wCount; w++)
		{
			unsigned long pos = w + (h * wCount) + 1;

			targetImageSprite[pos].Init(_device);

			targetImageSprite[pos].SetPos(ChD3D11::SpritePositionName::LeftTop, ChVec2(leftPosition[w], topPosition[h]));
			targetImageSprite[pos].SetPos(ChD3D11::SpritePositionName::RightTop, ChVec2(rightPosition[w], topPosition[h]));
			targetImageSprite[pos].SetPos(ChD3D11::SpritePositionName::LeftBottom, ChVec2(leftPosition[w], bottomPosition[h]));
			targetImageSprite[pos].SetPos(ChD3D11::SpritePositionName::RightBottom, ChVec2(rightPosition[w], bottomPosition[h]));
		}

	}

}

void StageSelectFrame::InitStageData()
{
	ChD3D::DirectFontFromWICBitmap stageNameTextDrawer;
	ChD3D::DirectFontFromWICBitmap targetNameTextDrawer;

	ChD3D::WICBitmapObject stageNameBitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(STAGE_NAME_DRAW_WIDTH, STAGE_NAME_DRAW_HEIGHT);
	stageNameTextDrawer.Init(STAGE_NAME_DRAW_WIDTH, STAGE_NAME_DRAW_HEIGHT, stageNameBitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	ChD3D::WICBitmapObject targetNameBitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(TARGET_NAME_DRAW_WIDTH, TARGET_NAME_DRAW_HEIGHT);
	targetNameTextDrawer.Init(TARGET_NAME_DRAW_WIDTH, TARGET_NAME_DRAW_HEIGHT, targetNameBitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	stageNameTextDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	stageNameTextDrawer.SetClearDisplayFlg(true);

	targetNameTextDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	targetNameTextDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject stageNameBrush = stageNameTextDrawer.CreateBrush(ChVec4(1.0f));
	ChD3D::TextFormatObject stageNameFormat = stageNameTextDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 38.0f);

	ChD3D::BrushObject targetNameBrush = targetNameTextDrawer.CreateBrush(ChVec4(1.0f));
	ChD3D::TextFormatObject targetNameFormat = targetNameTextDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 26.0f);

	for (auto&& file : std::filesystem::directory_iterator(STAGE_DATA_DIRECTORY("")))
	{
		std::string path = ChStr::UTF8ToString(file.path().c_str());
		auto stageData = ChPtr::Make_S<StageData>();
		stageData->LoadData(path);

		stageNameTextDrawer.DrawStart();

		stageNameTextDrawer.DrawToScreen(stageData->stageName, stageNameFormat, stageNameBrush, ChVec4::FromRect(0.0f, 0.0f, TO_FLUAT(STAGE_NAME_DRAW_WIDTH), TO_FLUAT(STAGE_NAME_DRAW_HEIGHT)));

		stageNameTextDrawer.DrawEnd();

		stageData->stageNameText = ChPtr::Make_S<ChD3D11::Texture11>();

		stageData->stageNameText->CreateColorTexture(stageNameBitmap.GetBitmap());


		targetNameTextDrawer.DrawStart();

		targetNameTextDrawer.DrawToScreen(stageData->targetName, targetNameFormat, targetNameBrush, ChVec4::FromRect(0.0f, 0.0f, TO_FLUAT(TARGET_NAME_DRAW_WIDTH), TO_FLUAT(TARGET_NAME_DRAW_HEIGHT)));

		targetNameTextDrawer.DrawEnd();

		stageData->targetNameText = ChPtr::Make_S<ChD3D11::Texture11>();

		stageData->targetNameText->CreateColorTexture(targetNameBitmap.GetBitmap());


		stageDataList.push_back(stageData);
	}

	stageNameTextDrawer.Release();

	targetNameTextDrawer.Release();

}

void StageSelectFrame::Release()
{

}

void StageSelectFrame::DrawFunction()
{
	auto&& dc = ChD3D11::D3D11DC();

	ChD3D11::Shader11().DrawStart();

	std::vector<unsigned long>drawStageImages;

	spriteShader.DrawStart(dc);

	for (unsigned long i = 0; i <= 5; i++)
	{
		unsigned long testValue = (i + nowSelect) % stageDataList.size();

		auto drawTestValue = std::find(drawStageImages.begin(), drawStageImages.end(), testValue);

		//if (drawTestValue == drawStageImages.end())break;

		ChD3D11::TextureBase11* testImage = &stageDataList[testValue]->stageImage.image;
		if (!testImage->IsTex())testImage = &notImageTexture;

		spriteShader.Draw(*testImage, stagePreImageSprite[(i + 2) % 5]);
	}

	DrawStageData(*stageDataList[nowSelect]);

	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();

}

void StageSelectFrame::DrawStageData(const StageData& _stageData)
{
	ChD3D11::TextureBase11* testImage = const_cast<ChD3D11::Texture11*>(&_stageData.stageImage.image);
	if (!testImage->IsTex())testImage = &notImageTexture;

	spriteShader.Draw(*testImage, stageImageSprite);

	if (_stageData.stageNameText->IsTex())
	{
		spriteShader.Draw(*_stageData.stageNameText, stageNameSprite);
	}

	if (_stageData.targetNameText->IsTex())
	{
		spriteShader.Draw(*_stageData.targetNameText, targetNameSprite);
	}

	for (unsigned long i = 0; i < 10; i++)
	{
		testImage = const_cast<ChD3D11::Texture11*>(&_stageData.targetImages[i]->image);
		if (ChPtr::NullCheck(testImage))testImage = &notImageTexture;
		if (!testImage->IsTex())testImage = &notImageTexture;

		spriteShader.Draw(*testImage, targetImageSprite[i]);
	}

}

void StageSelectFrame::UpdateFunction()
{

	UpdateKeyboard();
	UpdateMouse();

	for (auto&& inputData : inputDataList)
	{
		if (inputData == ActionType::UpSelect)
		{
			nowSelect--;
		}

		if (inputData == ActionType::DownSelect)
		{
			nowSelect++;
		}

		nowSelect = (nowSelect + stageDataList.size()) % stageDataList.size();

		if (inputData == ActionType::Decision)
		{

		}

	}

	inputDataList.clear();
}

void StageSelectFrame::Update()
{
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

	//if (IsMoucePosOnSprite(toBattleButtonSprite))

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