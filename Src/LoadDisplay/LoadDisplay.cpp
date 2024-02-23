#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../BaseMecha/MechaParts.h"
#include"../Attack/Attack.h"

#include"LoadDisplay.h"

#ifndef LOAD_IMAGE_DIRECTORY
#define LOAD_IMAGE_DIRECTORY(current_path) TEXTURE_DIRECTORY("LoadDisplay/" current_path)
#endif

#define LOAD_DISPLAY_LEFT 200.0f
#define LOAD_DISPLAY_TOP 50.0f
#define LOAD_DISPLAY_WINDOW_WIDTH 880.0f
#define LOAD_DISPLAY_WINDOW_HEIGHT 520.0f

#define LOAD_DISPLAY_RIGHT LOAD_DISPLAY_LEFT + LOAD_DISPLAY_WINDOW_WIDTH
#define LOAD_DISPLAY_BOTTOM LOAD_DISPLAY_TOP + LOAD_DISPLAY_WINDOW_HEIGHT

#define MAIN_DISPAY_HEIGHT 420.0f
#define MAIN_DISPAY_BOTTOM LOAD_DISPLAY_TOP + MAIN_DISPAY_HEIGHT


#define MECHA_PREVIEW_WIDTH 150.0f
#define MECHA_PREVIEW_HEIGHT 220.0f
#define MECHA_PREVIEW_LEFT 8.0f
#define MECHA_PREVIEW_TOP 10.0f

#define MECHA_NAME_TEXT_WIDTH 150.0f
#define MECHA_NAME_TEXT_HEIGHT 80.0f
#define MECHA_NAME_TEXT_LEFT 8.0f
#define MECHA_NAME_TEXT_TOP 235.0f
#define MECHA_NAME_TEXT_FONT_SIZE 16.0f

#define UP_BUTTON_TOP 100.0f
#define UP_BUTTON_BOTTOM 320.0f + UP_BUTTON_TOP

#define SELECT_PANEL_WIDTH 166.0f
#define SELECT_PANEL_LEFT 340.0f
#define SELECT_PANEL_ALIGN 51.0f

#define SELECT_BUTTON_WIDTH 60.0f

#define SELECT_BUTTON_LEFT_LEFT 210.0f
#define SELECT_BUTTON_LEFT_RIGHT SELECT_BUTTON_LEFT_LEFT + SELECT_BUTTON_WIDTH

#define SELECT_BUTTON_RIGHT_LEFT 1010.0f
#define SELECT_BUTTON_RIGHT_RIGHT SELECT_BUTTON_RIGHT_LEFT + SELECT_BUTTON_WIDTH

#define BOTTOM_BUTTON_HEIGHT 50.0f
#define BOTTOM_BUTTON_TOP 492.0f
#define BOTTOM_BUTTON_BOTTOM BOTTOM_BUTTON_TOP + BOTTOM_BUTTON_HEIGHT

#define BOTTOM_BUTTON_WIDTH 200.0f

#define BOTTOM_BUTTON_LEFT_LEFT 340.0f
#define BOTTOM_BUTTON_LEFT_RIGHT BOTTOM_BUTTON_LEFT_LEFT + BOTTOM_BUTTON_WIDTH

#define BOTTOM_BUTTON_RIGHT_LEFT 740.0f
#define BOTTOM_BUTTON_RIGHT_RIGHT  BOTTOM_BUTTON_RIGHT_LEFT + BOTTOM_BUTTON_WIDTH

#define ROGRESS_CIRCLE_SIZE 450.0f
#define PROGRESS_CIRCLE_LEFT 215.0f
#define PROGRESS_CIRCLE_TOP 17.0f
#define PROGRESS_CIRCLE_RIGHT ROGRESS_CIRCLE_SIZE + PROGRESS_CIRCLE_LEFT
#define PROGRESS_CIRCLE_BOTTOM ROGRESS_CIRCLE_SIZE + PROGRESS_CIRCLE_TOP
//Degree Per Frame//
#define PROGRESS_CIRCLE_ROTATION_SPEED 20.0f

void LoadDisplay::Init(ID3D11Device* _device, ChD3D::XInputController* _controller)
{
	MenuBase::InitMenu(_controller);

	device = _device;
	dsTex.CreateDepthBuffer(device, static_cast<unsigned long>(GAME_WINDOW_WIDTH), static_cast<unsigned long>(GAME_WINDOW_HEIGHT));

	meshDrawer.Init(_device);
	meshDrawer.SetCullMode(D3D11_CULL_MODE::D3D11_CULL_BACK);
	meshDrawer.SetAlphaBlendFlg(false);

	light.Init(ChD3D11::D3D11Device());
	light.SetUseLightFlg(true);
	ChVec3 dir = ChVec3(0.5f, -1.0f, 0.0f);
	dir.Normalize();
	light.SetDirectionLightData(true, ChVec3(1.0f), dir, 0.3f);

	ChVec4 rect = ChVec4::FromRect(LOAD_DISPLAY_LEFT, LOAD_DISPLAY_TOP, LOAD_DISPLAY_RIGHT, LOAD_DISPLAY_BOTTOM);
	SPRITE_INIT(window.sprite, RectToGameWindow(rect));
	window.image.CreateTexture(LOAD_IMAGE_DIRECTORY("Window.png"), device);

	rect.bottom = MAIN_DISPAY_BOTTOM;
	SPRITE_INIT(mainWindow.sprite, RectToGameWindow(rect));
	mainWindow.image.CreateTexture(LOAD_IMAGE_DIRECTORY("SelectDisplay.png"), device);


	rect = ChVec4::FromRect(SELECT_BUTTON_LEFT_LEFT, UP_BUTTON_TOP, SELECT_BUTTON_LEFT_RIGHT, UP_BUTTON_BOTTOM);
	SPRITE_INIT(panelSelectButton[ChStd::EnumCast(SelectButtonType::Left)].sprite, RectToGameWindow(rect));
	panelSelectButton[ChStd::EnumCast(SelectButtonType::Left)].image.CreateTexture(LOAD_IMAGE_DIRECTORY("LeftButton.png"), device);

	rect.left = SELECT_BUTTON_RIGHT_LEFT;
	rect.right = SELECT_BUTTON_RIGHT_RIGHT;
	SPRITE_INIT(panelSelectButton[ChStd::EnumCast(SelectButtonType::Right)].sprite, RectToGameWindow(rect));
	panelSelectButton[ChStd::EnumCast(SelectButtonType::Right)].image.CreateTexture(LOAD_IMAGE_DIRECTORY("RightButton.png"), device);
	selectPanelSelectButtonImage.CreateTexture(LOAD_IMAGE_DIRECTORY("ButtonSelect.png"), device);

	rect = ChVec4::FromRect(BOTTOM_BUTTON_LEFT_LEFT, BOTTOM_BUTTON_TOP, BOTTOM_BUTTON_LEFT_RIGHT, BOTTOM_BUTTON_BOTTOM);
	SPRITE_INIT(bottomButton[ChStd::EnumCast(BottomButtonType::Decision)].sprite, RectToGameWindow(rect));
	bottomButton[ChStd::EnumCast(BottomButtonType::Decision)].image.CreateTexture(LOAD_IMAGE_DIRECTORY("DecisionButton.png"), device);

	rect.left = BOTTOM_BUTTON_RIGHT_LEFT;
	rect.right = BOTTOM_BUTTON_RIGHT_RIGHT;
	SPRITE_INIT(bottomButton[ChStd::EnumCast(BottomButtonType::Cancel)].sprite, RectToGameWindow(rect));
	bottomButton[ChStd::EnumCast(BottomButtonType::Cancel)].image.CreateTexture(LOAD_IMAGE_DIRECTORY("CancelButton.png"), device);
	selectBottomButtonImage.CreateTexture(LOAD_IMAGE_DIRECTORY("BottomButtonSelect.png"), device);


	float panelLeft = SELECT_PANEL_LEFT;
	float panelRight = panelLeft + SELECT_PANEL_WIDTH;

	for (unsigned char i = 0; i < PANEL_DRAW_COUNT; i++)
	{

		rect = ChVec4::FromRect(panelLeft, UP_BUTTON_TOP, panelRight, UP_BUTTON_BOTTOM);
		SPRITE_INIT(loadPanelSpriteList[i].backGround, RectToGameWindow(rect));

		rect = ChVec4::FromRect(panelLeft + MECHA_PREVIEW_LEFT, UP_BUTTON_TOP + MECHA_PREVIEW_TOP, panelLeft + MECHA_PREVIEW_LEFT + MECHA_PREVIEW_WIDTH, UP_BUTTON_TOP + MECHA_PREVIEW_TOP + MECHA_PREVIEW_HEIGHT);
		SPRITE_INIT(loadPanelSpriteList[i].mechaPreview, RectToGameWindow(rect));

		rect = ChVec4::FromRect(panelLeft + MECHA_NAME_TEXT_LEFT, UP_BUTTON_TOP + MECHA_NAME_TEXT_TOP, panelLeft + MECHA_NAME_TEXT_LEFT + MECHA_NAME_TEXT_WIDTH, UP_BUTTON_TOP + MECHA_NAME_TEXT_TOP + MECHA_NAME_TEXT_HEIGHT);
		SPRITE_INIT(loadPanelSpriteList[i].mechaName, RectToGameWindow(rect));

		panelLeft = panelRight + SELECT_PANEL_ALIGN;
		panelRight = panelLeft + SELECT_PANEL_WIDTH;
	}

	loadPanelImage.CreateTexture(LOAD_IMAGE_DIRECTORY("LoadMechaPanel.png"), device);
	selectLoadPanelImage.CreateTexture(LOAD_IMAGE_DIRECTORY("PanelSelect.png"), device);

	SPRITE_INIT(progressCircleTexturePosition, RectToGameWindow(ChVec4::FromRect(PROGRESS_CIRCLE_LEFT, PROGRESS_CIRCLE_TOP, PROGRESS_CIRCLE_RIGHT, PROGRESS_CIRCLE_BOTTOM)));
	progressCircleTexture.CreateTexture(TEXTURE_DIRECTORY("ProgressCircleMask.png"));

	{
		auto&& color = ColorTextToColorVector3("66","F6","FF");

		progressCircleColor = ChVec4::FromColor(
			color.r,
			color.g,
			color.b,
			1.0f
		);
	}

	textDrawer.bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(
		static_cast<unsigned long>(MECHA_NAME_TEXT_WIDTH),
		static_cast<unsigned long>(MECHA_NAME_TEXT_HEIGHT));

	textDrawer.drawer.Init(
		static_cast<unsigned long>(MECHA_NAME_TEXT_WIDTH),
		static_cast<unsigned long>(MECHA_NAME_TEXT_HEIGHT),
		textDrawer.bitmap,
		ChD3D::DirectFontBase::LocaleNameId::Japanese);
	textDrawer.drawer.SetClearDisplayColor(ChVec4(0.0f));
	textDrawer.drawer.SetClearDisplayFlg(true);

	textDrawer.brush = textDrawer.drawer.CreateBrush(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));
	textDrawer.format = textDrawer.drawer.CreateTextFormat(
		L"ÉÅÉCÉäÉI",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		MECHA_NAME_TEXT_FONT_SIZE
	);

	textDrawer.format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	textDrawer.format.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);


	{
		ChMat_11 tmpMat;

		tmpMat.CreateProjectionMat(ChMath::ToRadian(60.0f),GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 0.1f, 1000.0f);
		meshDrawer.SetProjectionMatrix(tmpMat);
		
		tmpMat.CreateViewMatLookTarget(ChVec3(5.0f, 8.0f, 8.0f), ChVec3(0.0f, -2.0f, 0.0f), ChVec3(0.0f, 1.0f, 0.0f));
		meshDrawer.SetViewMatrix(tmpMat);
	}
}

void LoadDisplay::Release()
{
	Close();

	MechaParts::ClearPartsList();
	Attack::AllRelease();
}

bool LoadDisplay::Update()
{
	if (LoadFile(loadMechaList.size()))return true;

	if (!openFlg)return false;

	UpdateFunction();

	return true;
}

void LoadDisplay::UpdateAction(ActionType _type)
{

	if (_type == ActionType::Decision)
	{
		if (selectButton == SelectButtonType::Left)
		{
			AddActionType(ActionType::Left);
			return;
		}

		if (selectButton == SelectButtonType::Right)
		{
			AddActionType(ActionType::Right);
			return;
		}

		if (static_cast<BottomButtonType>(selectBottomButton) == BottomButtonType::None)return;

		if (static_cast<BottomButtonType>(selectBottomButton) == BottomButtonType::Cancel)
		{
			AddActionType(ActionType::Cancel);
			return;
		}

		Load();
		Close();
		SetLoopBreakTrue();
		return;
	}

	if (_type == ActionType::Cancel)
	{
		Close();
		SetLoopBreakTrue();
		return;
	}

	if (_type == ActionType::Up || _type == ActionType::Down)
	{
		selectBottomButton = ChStd::EnumCast(static_cast<BottomButtonType>(selectBottomButton) == BottomButtonType::None ? BottomButtonType::Decision : BottomButtonType::None);
		return;
	}

	if (static_cast<BottomButtonType>(selectBottomButton) != BottomButtonType::None)
	{
		if (_type == ActionType::Left || _type == ActionType::Right)
		{
			selectBottomButton = (selectBottomButton + 1) % ChStd::EnumCast(BottomButtonType::None);
		}
		return;
	}

	if (_type == ActionType::Right)
	{
		if (PANEL_DRAW_COUNT < loadMechaList.size())
		{
			if (selectPanelNo == ((drawNowSelect + PANEL_DRAW_COUNT - 1) % loadMechaList.size()))
				drawNowSelect = (drawNowSelect + 1) % loadMechaList.size();
		}

		selectPanelNo = (selectPanelNo + 1) % loadMechaList.size();
	}

	if (_type == ActionType::Left)
	{
		if (PANEL_DRAW_COUNT < loadMechaList.size())
		{
			if (selectPanelNo == drawNowSelect)
				drawNowSelect = (drawNowSelect + loadMechaList.size() - 1) % loadMechaList.size();
		}

		selectPanelNo = (selectPanelNo + loadMechaList.size() - 1) % loadMechaList.size();
	}
}

void LoadDisplay::UpdateMouse()
{

	auto&& manager = ChSystem::SysManager();

	InputTest(ActionType::Decision, manager.IsPushKeyNoHold(VK_LBUTTON));

	auto&& mouce = ChWin::Mouse();
	mouce.Update();

	auto&& mouseMove = mouce.GetMoveValue();

	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

	selectButton = SelectButtonType::None;
	selectBottomButton = ChStd::EnumCast(BottomButtonType::None);

	for (int i = 0; i < ChStd::EnumCast(SelectButtonType::None); i++)
	{
		if (!IsMoucePosOnSprite(panelSelectButton[i].sprite))continue;
		selectButton = static_cast<SelectButtonType>(i);
		return;
	}

	for (int i = 0; i < ChStd::EnumCast(BottomButtonType::None); i++)
	{
		if (!IsMoucePosOnSprite(bottomButton[i].sprite))continue;
		selectBottomButton = i;
		return;
	}

	for (int i = 0; i < PANEL_DRAW_COUNT; i++)
	{
		if (!IsMoucePosOnSprite(loadPanelSpriteList[i].backGround))continue;
		selectPanelNo = (i + drawNowSelect) % loadMechaList.size();
		selectBottomButton = ChStd::EnumCast(BottomButtonType::Decision);
		return;
	}
}

void LoadDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)
{

	if (loadFileList != nullptr)
	{
		if (loadFileList->GetCount() > loadMechaList.size())
		{
			DrawBase(_spriteShader);

			ChLMat rotateMat;
			rotateMat.SetRotationZAxis(ChMath::ToRadian(progressCircleRoutate));

			_spriteShader.Draw(progressCircleTexture, progressCircleTexturePosition, progressCircleColor, (ChMat_11)rotateMat);
			progressCircleRoutate += PROGRESS_CIRCLE_ROTATION_SPEED;
		}
	}

	if (!openFlg)return;

	DrawBase(_spriteShader);

	for (unsigned long i = 0; i < PANEL_DRAW_COUNT && i < loadMechaList.size(); i++)
	{
		_spriteShader.Draw(loadPanelImage, loadPanelSpriteList[i].backGround);
		_spriteShader.Draw(loadMechaList[(i + drawNowSelect) % loadMechaList.size()]->mechaTexture, loadPanelSpriteList[i].mechaPreview);
		_spriteShader.Draw(loadMechaList[(i + drawNowSelect) % loadMechaList.size()]->panelName, loadPanelSpriteList[i].mechaName);

		if (((i + drawNowSelect) % loadMechaList.size()) != selectPanelNo)continue;
		_spriteShader.Draw(selectLoadPanelImage, loadPanelSpriteList[i].backGround);
	}
}

void LoadDisplay::DrawBase(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)
{
	_spriteShader.Draw(window.image, window.sprite);
	_spriteShader.Draw(mainWindow.image, mainWindow.sprite);

	for (unsigned long i = 0; i < ChStd::EnumCast(SelectButtonType::None); i++)
	{
		_spriteShader.Draw(panelSelectButton[i].image, panelSelectButton[i].sprite);
	}

	for (unsigned long i = 0; i < ChStd::EnumCast(SelectButtonType::None); i++)
	{
		_spriteShader.Draw(panelSelectButton[i].image, panelSelectButton[i].sprite);
		if (ChStd::EnumCast(selectButton) != i)continue;
		_spriteShader.Draw(selectPanelSelectButtonImage, panelSelectButton[i].sprite);
	}

	for (unsigned long i = 0; i < ChStd::EnumCast(BottomButtonType::None); i++)
	{
		_spriteShader.Draw(bottomButton[i].image, bottomButton[i].sprite);
		if (selectBottomButton != i)continue;
		_spriteShader.Draw(selectBottomButtonImage, bottomButton[i].sprite);
	}

}

void LoadDisplay::Open(ID3D11DeviceContext* _dc)
{
	if (_dc == nullptr)return;
	drawNowSelect = 0;
	selectPanelNo = 0;

	progressCircleRoutate = 0.0f;

	dc = _dc;

	ChCpp::CharFile file;
	file.FileOpen(PLAYER_MECHA_PATH);
	std::string fileText = file.FileReadText();
	file.FileClose();

	loadFileList = ChPtr::Make_S<ChCpp::JsonArray>();

	loadFileList->SetRawData(fileText);
	

}

void LoadDisplay::Close()
{
	if (!loadMechaList.empty())
		loadMechaList.clear();

	loadFileList = nullptr;


	openFlg = false;
}

void LoadDisplay::Load()
{
	std::string fileText = "";

	loadMechaList[selectPanelNo]->mecha->Save(PLAYER_USE_MECHA_PATH);

}

bool LoadDisplay::LoadFile(unsigned long _openNumber)
{
	if (dc == nullptr)return false;
	if (loadFileList == nullptr)return false;
	if (loadFileList->GetCount() <= _openNumber)
	{
		dc = nullptr;
		openFlg = true;
		return false;
	}

	auto&& assembleMechaFrame = loadFileList->GetJsonObject(_openNumber);

	auto&& loadMecha = ChPtr::Make_S<LoaderPanel>();

	loadMecha->mecha = ChPtr::Make_S<BaseMecha>();

	loadMecha->mecha->Create(ChVec2(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), meshDrawer, nullptr);

	loadMecha->mecha->SetPosition(ChVec3(0.0f));
	loadMecha->mecha->SetRotation(ChVec3(0.0f));

	loadMecha->mecha->LoadPartsList(device, assembleMechaFrame);

	loadMecha->mechaTexture.CreateRenderTarget(device, static_cast<unsigned long>(GAME_WINDOW_WIDTH), static_cast<unsigned long>(GAME_WINDOW_HEIGHT));
	loadMecha->mechaTexture.SetBackColor(dc, backColor);

	ID3D11RenderTargetView* rtView = loadMecha->mechaTexture.GetRTView();
	dsTex.ClearDepthBuffer(dc);

	dc->OMSetRenderTargets(1, &rtView, dsTex.GetDSView());

	light.SetDrawData(dc);
	meshDrawer.DrawStart(dc);

	loadMecha->mecha->Draw3D();

	meshDrawer.DrawEnd();

	textDrawer.drawer.DrawStart();

	textDrawer.drawer.DrawToScreen(
		ChStr::UTF8ToWString(loadMecha->mecha->GetMechaName()),
		textDrawer.format,
		textDrawer.brush,
		ChVec4::FromRect(0.0f, 0.0f, MECHA_NAME_TEXT_WIDTH, MECHA_NAME_TEXT_HEIGHT)
	);

	textDrawer.drawer.DrawEnd();

	loadMecha->panelName.CreateColorTexture(device, textDrawer.bitmap.GetBitmap());

	loadMechaList.push_back(loadMecha);

	return true;
}
