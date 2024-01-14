
#include"../../BaseIncluder.h"

#include"StageDetailedDisplay.h"
#include"../StageData/StageData.h"

#define STAGE_NAME_X 285.0f
#define STAGE_NAME_Y 43.0f
#define STAGE_NAME_WIDTH 440.0f + STAGE_NAME_X
#define STAGE_NAME_HEIGHT 53.0f + STAGE_NAME_Y

#define STAGE_IMAGE_X 285.0f
#define STAGE_IMAGE_Y 113.0f
#define STAGE_IMAGE_WIDTH 440.0f + STAGE_IMAGE_X
#define STAGE_IMAGE_HEIGHT 328.0f + STAGE_IMAGE_Y

void StageDetailedDisplay::Init()
{
	SPRITE_INIT(stageImage, RectToGameWindow(ChVec4::FromRect(STAGE_IMAGE_X, STAGE_IMAGE_Y, STAGE_IMAGE_WIDTH, STAGE_IMAGE_HEIGHT)));

	SPRITE_INIT(stageName.sprite, RectToGameWindow(ChVec4::FromRect(STAGE_NAME_X, STAGE_NAME_Y, STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT)));
}

void StageDetailedDisplay::Release()
{

}

void StageDetailedDisplay::Update()
{
	UpdateMouse();
	UpdateKeyboard();
	UpdateController();
}

void StageDetailedDisplay::UpdateAction(const StageSelectFrame::ActionType& _type)
{

	if (_type == StageSelectFrame::ActionType::Decision)
	{

	}

	if (_type == StageSelectFrame::ActionType::UpSelect)
	{

	}

	if (_type == StageSelectFrame::ActionType::DownSelect)
	{

	}

}

void StageDetailedDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)
{

	auto&& selectStageData = GetNowSelectStageData();

	ChD3D11::TextureBase11* testImage = &selectStageData.stageImage;
	if (!testImage->IsTex())testImage = &GetNonImage();

	_drawer.Draw(*testImage, stageImage);

	if (stageName.textImage != nullptr)
	{
		_drawer.Draw(*stageName.textImage, stageName.sprite);
	}


}

void StageDetailedDisplay::OnDisplay()
{
	auto&& stageData = GetNowSelectStageData();

	ChD3D::DirectFontFromWICBitmap stageNameTextDrawer;

	ChD3D::WICBitmapObject stageNameBitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT);
	stageNameTextDrawer.Init(STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT, stageNameBitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	stageNameTextDrawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	stageNameTextDrawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject stageNameBrush = stageNameTextDrawer.CreateBrush(ChVec4(1.0f));
	ChD3D::TextFormatObject stageNameFormat = stageNameTextDrawer.CreateTextFormat(L"ÉÅÉCÉäÉI", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 38.0f);
	stageNameFormat.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	ChPtr::Shared<ChD3D11::Texture11>stageNameText = nullptr;

	ChPtr::Shared<ChD3D11::Texture11>drawSuccessFeeText = nullptr;

	stageNameTextDrawer.DrawStart();

	stageNameTextDrawer.DrawToScreen(ChStr::UTF8ToWString(stageData.stageDatas->stageName), stageNameFormat, stageNameBrush, ChVec4::FromRect(0.0f, 0.0f, STAGE_NAME_WIDTH, STAGE_NAME_HEIGHT));

	stageNameTextDrawer.DrawEnd();

	stageNameText = ChPtr::Make_S<ChD3D11::Texture11>();

	stageNameText->CreateColorTexture(stageNameBitmap.GetBitmap());

	stageNameTextDrawer.Release();

	stageNameBitmap.Release();
}

void StageDetailedDisplay::UpdateMouse()
{
	auto&& manager = ChSystem::SysManager();

	if (manager.IsPushKeyNoHold(VK_LBUTTON))
	{
		AddAction(StageSelectFrame::ActionType::Decision);
	}

	auto&& mouce = ChWin::Mouse();
	mouce.Update();

	auto&& mouseMove = mouce.GetMoveValue();

	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

}

void StageDetailedDisplay::UpdateKeyboard()
{

	auto&& manager = ChSystem::SysManager();

	if (manager.IsPushKeyNoHold(VK_RETURN))
	{
		AddAction(StageSelectFrame::ActionType::Decision);
	}

	if (manager.IsPushKeyNoHold(VK_UP))
	{
		AddAction(StageSelectFrame::ActionType::UpSelect);
	}

	if (manager.IsPushKeyNoHold(VK_DOWN))
	{
		AddAction(StageSelectFrame::ActionType::DownSelect);
	}
}

void StageDetailedDisplay::UpdateController()
{

}
