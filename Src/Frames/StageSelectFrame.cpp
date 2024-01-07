
#include"../BaseIncluder.h"

#include"StageSelectFrame.h"
#include"../StageSelectFrame/StageData/StageData.h"

#define SELECT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("StageImage/") current_path

#define TO_FLUAT(num) num##.0f

#define STAGE_NAME_DRAW_WIDTH 540
#define STAGE_NAME_DRAW_HEIGHT 53
#define STAGE_NAME_DRAW_X 200
#define STAGE_NAME_DRAW_Y 23


void StageSelectFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
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


	notImageTexture.CreateColorTexture(device, ChVec4::FromColor(0.7f, 0.7f, 0.7f, 1.0f), 1, 1);

	InitStageData();

}

void StageSelectFrame::InitStageData()
{
	ChD3D::DirectFontFromWICBitmap stageNameTextDrawer;
	ChD3D::DirectFontFromWICBitmap targetNameTextDrawer;

	ChD3D::WICBitmapObject stageNameBitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(STAGE_NAME_DRAW_WIDTH, STAGE_NAME_DRAW_HEIGHT);
	stageNameTextDrawer.Init(STAGE_NAME_DRAW_WIDTH, STAGE_NAME_DRAW_HEIGHT, stageNameBitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	stageNameTextDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	stageNameTextDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject stageNameBrush = stageNameTextDrawer.CreateBrush(ChVec4(1.0f));
	ChD3D::TextFormatObject stageNameFormat = stageNameTextDrawer.CreateTextFormat(L"ÉÅÉCÉäÉI", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 38.0f);
	stageNameFormat.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	for (auto&& file : std::filesystem::directory_iterator(STAGE_DATA_DIRECTORY("")))
	{
		std::string path = ChStr::UTF8ToString(file.path().c_str());
		auto stageData = ChPtr::Make_S<StageData>();
		stageData->LoadData(path);

		stageNameTextDrawer.DrawStart();

		stageNameTextDrawer.DrawToScreen(ChStr::UTF8ToWString(stageData->stageDatas->stageName), stageNameFormat, stageNameBrush, ChVec4::FromRect(0.0f, 0.0f, TO_FLUAT(STAGE_NAME_DRAW_WIDTH), TO_FLUAT(STAGE_NAME_DRAW_HEIGHT)));

		stageNameTextDrawer.DrawEnd();

		stageData->stageNameText = ChPtr::Make_S<ChD3D11::Texture11>();

		stageData->stageNameText->CreateColorTexture(stageNameBitmap.GetBitmap());

		stageDataList.push_back(stageData);
	}

	stageNameTextDrawer.Release();

	targetNameTextDrawer.Release();

	stageNameBitmap.Release();

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

	DrawStageData(*stageDataList[nowSelect]);

	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();

}

void StageSelectFrame::DrawStageData(const StageData& _stageData)
{
	ChD3D11::TextureBase11* testImage = const_cast<ChD3D11::Texture11*>(&_stageData.stageImage);
	if (!testImage->IsTex())testImage = &notImageTexture;

	spriteShader.Draw(*testImage, stageImageSprite);

	if (_stageData.stageNameText->IsTex())
	{
		spriteShader.Draw(*_stageData.stageNameText, stageNameSprite);
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