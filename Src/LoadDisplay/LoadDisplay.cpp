#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../BaseMecha/MechaParts.h"

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

void LoadDisplay::Init(ID3D11Device* _device)
{
	device = _device;
	dsTex.CreateDepthBuffer(device, static_cast<unsigned long>(GAME_WINDOW_WIDTH), static_cast<unsigned long>(GAME_WINDOW_HEIGHT));

	meshDrawer.Init(_device);
	meshDrawer.SetCullMode(D3D11_CULL_MODE::D3D11_CULL_BACK);
	meshDrawer.SetAlphaBlendFlg(false);

	testSprite.Init();
	testSprite.SetPosRect(ChVec4::FromRect(-1.0f,1.0f,1.0f,-1.0f ));

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

		panelLeft = panelRight + SELECT_PANEL_ALIGN;
		panelRight = panelLeft + SELECT_PANEL_WIDTH;
	}

	loadPanelImage.CreateTexture(LOAD_IMAGE_DIRECTORY("LoadMechaPanel.png"), device);
	selectLoadPanelImage.CreateTexture(LOAD_IMAGE_DIRECTORY("PanelSelect.png"), device);

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
	loadMechaList.clear();
	MechaParts::ClearPartsList();
}

std::string LoadDisplay::GetSelectMechaPath()
{
	return loadMechaList[selectPanel]->path;
}

bool LoadDisplay::Update()
{
	if (!openFlg)return false;

	return true;
}

void LoadDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)
{
	if (!openFlg)return;

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
		if (ChStd::EnumCast(selectBottomButton) != i)continue;
		_spriteShader.Draw(selectBottomButtonImage, bottomButton[i].sprite);
	}

	for (unsigned long i = 0; i < PANEL_DRAW_COUNT && i < loadMechaList.size(); i++)
	{
		_spriteShader.Draw(loadPanelImage, loadPanelSpriteList[i].backGround);
		_spriteShader.Draw(loadMechaList[(i + drawNowSelect) % loadMechaList.size()]->mechaTexture, loadPanelSpriteList[i].mechaPreview);
	}

	//_spriteShader.Draw(loadMechaList[0]->mechaTexture, testSprite);
}

void LoadDisplay::Open(const std::string& _nowSelectPath, ID3D11DeviceContext* _dc, bool _releaseFlg)
{
	drawNowSelect = 0;

	unsigned long loopCount = 0;

	for (auto&& file : std::filesystem::directory_iterator(PLAYER_MECHA_PATH("")))
	{
		std::string path = ChStr::UTF8ToString((file.path().c_str()));

		selectPanel = _nowSelectPath != path ? selectPanel : loopCount;

		auto&& loadMecha = ChPtr::Make_S<LoaderPanel>();
		loadMecha->mecha = ChPtr::Make_S<BaseMecha>();
		loadMecha->mecha->Create(ChVec2(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), meshDrawer, nullptr);
		loadMecha->mecha->SetPosition(ChVec3(0.0f));
		loadMecha->mecha->SetRotation(ChVec3(0.0f));

		loadMecha->mecha->Load(device, path);

		loadMecha->mechaTexture.CreateRenderTarget(device, static_cast<unsigned long>(GAME_WINDOW_WIDTH), static_cast<unsigned long>(GAME_WINDOW_HEIGHT));
		loadMecha->mechaTexture.SetBackColor(_dc, backColor);

		ID3D11RenderTargetView* rtView = loadMecha->mechaTexture.GetRTView();
		dsTex.ClearDepthBuffer(_dc);

		_dc->OMSetRenderTargets(1, &rtView, dsTex.GetDSView());

		light.SetDrawData(_dc);
		meshDrawer.DrawStart(_dc);

		loadMecha->mecha->Draw3D();

		meshDrawer.DrawEnd();

		loadMechaList.push_back(loadMecha);

		loopCount++;

		if (!_releaseFlg)continue;

		loadMecha->mecha = nullptr;
	}

	
	openFlg = true;

}

void LoadDisplay::Close()
{
	if (loadMechaList.empty())
		loadMechaList.clear();

	openFlg = false;
}

void LoadDisplay::Select()
{

}