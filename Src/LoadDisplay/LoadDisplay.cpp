#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../BaseMecha/MechaParts.h"
#include"../Attack/Attack.h"

#include"LoadDisplay.h"
#include"../SelectList/SelectList.h"

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

#define RROGRESS_CIRCLE_SIZE 450.0f
//#define PROGRESS_CIRCLE_LEFT 215.0f
//#define PROGRESS_CIRCLE_TOP 17.0f
#define PROGRESS_CIRCLE_LEFT (GAME_SPRITE_WIDTH * 0.5f) - (RROGRESS_CIRCLE_SIZE * 0.5f)
#define PROGRESS_CIRCLE_TOP (GAME_SPRITE_HEIGHT * 0.5f) - (RROGRESS_CIRCLE_SIZE * 0.5f)
#define PROGRESS_CIRCLE_RIGHT PROGRESS_CIRCLE_LEFT + RROGRESS_CIRCLE_SIZE
#define PROGRESS_CIRCLE_BOTTOM PROGRESS_CIRCLE_TOP + RROGRESS_CIRCLE_SIZE

#define PROGRESS_CIRCLE_CENTER_X (415.0f + (RROGRESS_CIRCLE_SIZE * 0.5f) - (GAME_SPRITE_WIDTH * 0.5f)) / (GAME_SPRITE_WIDTH * 0.5f)
#define PROGRESS_CIRCLE_CENTER_Y (67.0f + (RROGRESS_CIRCLE_SIZE * 0.5f) - (GAME_SPRITE_HEIGHT * 0.5f)) / (GAME_SPRITE_HEIGHT * -0.5f)
//Degree Per Frame//
#define PROGRESS_CIRCLE_ROTATION_SPEED 5.0f

class LoadItem : public SelectListItemBase
{
public:

	ChPtr::Shared<BaseMecha>mecha = nullptr;
	ChD3D11::RenderTarget11 mechaTexture;
	ChD3D11::Texture11 panelName;

};

class LoadPanelList : public SelectListBase
{
public:

	LoadPanelList()
	{
		sprite.Init();
		mechaPreviewSprite.Init();
		mechaNameSprite.Init();
	}

public:

	void SetSelectImage(ChD3D11::Texture11* _selectImage)
	{
		if (ChPtr::NullCheck(_selectImage))return;
		selectImage = _selectImage;
	}

	void SetBackGroundImage(ChD3D11::Texture11* _backGroundImage)
	{
		if (ChPtr::NullCheck(_backGroundImage))return;
		backGroundImage = _backGroundImage;
	}

public:

	void DrawSelect(ChD3D11::Shader::BaseDrawSprite11& _drawer, bool _isSelectPanel)
	{
		if (!_isSelectPanel)return;
		if (ChPtr::NullCheck(selectImage))return;

		_drawer.Draw(*selectImage, sprite);
	}

public:

	void DrawPanel(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect, ChPtr::Shared<SelectListItemBase> _drawItem, unsigned long _itemNo, bool _isSelectPanel)override
	{
		auto&& item = ChPtr::SharedSafeCast<LoadItem>(_drawItem);
		if (item == nullptr)return;
		if (ChPtr::NullCheck(backGroundImage))return;

		sprite.SetPosRect(RectToGameWindow(_rect));
		ChVec4 tmpRect = _rect;
		tmpRect.left += MECHA_PREVIEW_LEFT;
		tmpRect.top += MECHA_PREVIEW_TOP;

		tmpRect.right = tmpRect.left + MECHA_PREVIEW_WIDTH;
		tmpRect.bottom = tmpRect.top + MECHA_PREVIEW_HEIGHT;

		mechaPreviewSprite.SetPosRect(RectToGameWindow(tmpRect));
		tmpRect = _rect;
		tmpRect.left += MECHA_NAME_TEXT_LEFT;
		tmpRect.top += MECHA_NAME_TEXT_TOP;

		tmpRect.right = tmpRect.left + MECHA_NAME_TEXT_WIDTH;
		tmpRect.bottom = tmpRect.top + MECHA_NAME_TEXT_HEIGHT;
		mechaNameSprite.SetPosRect(RectToGameWindow(tmpRect));

		_drawer.Draw(*backGroundImage, sprite);
		_drawer.Draw(item->mechaTexture, mechaPreviewSprite);
		_drawer.Draw(item->panelName, mechaNameSprite);

		DrawSelect(_drawer, _isSelectPanel);

	}

private:

	ChD3D11::Sprite11 sprite;
	ChD3D11::Sprite11 mechaPreviewSprite;
	ChD3D11::Sprite11 mechaNameSprite;

	ChD3D11::Texture11* selectImage = nullptr;
	ChD3D11::Texture11* backGroundImage = nullptr;

	

};

void LoadDisplay::Init(ID3D11Device* _device, ChD3D::XInputController* _controller)
{
	MenuBase::InitMenu(_controller);

	device = _device;
	dsTex.CreateDepthBuffer(device, static_cast<unsigned long>(GAME_SPRITE_WIDTH), static_cast<unsigned long>(GAME_SPRITE_HEIGHT));

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

	loadPanelImage.CreateTexture(LOAD_IMAGE_DIRECTORY("LoadMechaPanel.png"), device);
	selectLoadPanelImage.CreateTexture(LOAD_IMAGE_DIRECTORY("PanelSelect.png"), device);

	selectList = ChPtr::Make_S<LoadPanelList>();
	selectList->Init();
	selectList->SetStartPosition(SELECT_PANEL_LEFT, UP_BUTTON_TOP);
	selectList->SetPanelSize(SELECT_PANEL_WIDTH, (UP_BUTTON_BOTTOM - UP_BUTTON_TOP));
	selectList->SetAlighSize((SELECT_PANEL_WIDTH + SELECT_PANEL_ALIGN), 0.0f);
	selectList->SetDrawCount(PANEL_DRAW_COUNT);
	selectList->SetMoveDiraction(MoveDiraction::Horizontal);
	selectList->SetBackGroundImage(&loadPanelImage);
	selectList->SetSelectImage(&selectLoadPanelImage);

	//SPRITE_INIT(progressCircleTexturePosition, RectToGameWindow(ChVec4::FromRect(PROGRESS_CIRCLE_LEFT, PROGRESS_CIRCLE_TOP, PROGRESS_CIRCLE_RIGHT, PROGRESS_CIRCLE_BOTTOM)));
	SPRITE_INIT(progressCircleTexturePosition, ChVec4::FromRect(-0.5f, 0.5f, 0.5f, -0.5f));
	progressCircleTexture.CreateTexture(TEXTURE_DIRECTORY("ProgressCircleMask.png"));

	{
		auto&& color = ColorTextToColorVector3(L"66",L"F6",L"FF");

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
		L"ƒƒCƒŠƒI",
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
	selectList->Release();
	selectList = nullptr;
}

bool LoadDisplay::Update()
{
	if (LoadFile(selectList->ItemCount()))return true;

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

	selectList->UpdateAction(_type);
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

	if (selectList->UpdateMouse())
	{
		selectBottomButton = ChStd::EnumCast(BottomButtonType::Decision);
	}

}

void LoadDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)
{

	if (loadFileList != nullptr)
	{
		if (loadFileList->GetCount() > selectList->ItemCount())
		{
			DrawBase(_spriteShader);

			ChLMat rotateMat;
			rotateMat.SetRotationZAxis(ChMath::ToRadian(progressCircleRoutate));
			rotateMat.SetPosition(PROGRESS_CIRCLE_CENTER_X, PROGRESS_CIRCLE_CENTER_Y, 0.0f);

			_spriteShader.Draw(progressCircleTexture, progressCircleTexturePosition, progressCircleColor, (ChMat_11)(rotateMat));
			progressCircleRoutate += PROGRESS_CIRCLE_ROTATION_SPEED;
		}
	}

	if (!openFlg)return;

	DrawBase(_spriteShader);

	selectList->Draw(_spriteShader);
}

void LoadDisplay::DrawBase(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)
{
	_spriteShader.Draw(window.image, window.sprite);
	_spriteShader.Draw(mainWindow.image, mainWindow.sprite);

	for (unsigned char i = 0; i < ChStd::EnumCast(SelectButtonType::None); i++)
	{
		_spriteShader.Draw(panelSelectButton[i].image, panelSelectButton[i].sprite);
	}

	for (unsigned char i = 0; i < ChStd::EnumCast(SelectButtonType::None); i++)
	{
		_spriteShader.Draw(panelSelectButton[i].image, panelSelectButton[i].sprite);
		if (ChStd::EnumCast(selectButton) != i)continue;
		_spriteShader.Draw(selectPanelSelectButtonImage, panelSelectButton[i].sprite);
	}

	for (unsigned char i = 0; i < ChStd::EnumCast(BottomButtonType::None); i++)
	{
		_spriteShader.Draw(bottomButton[i].image, bottomButton[i].sprite);
		if (selectBottomButton != i)continue;
		_spriteShader.Draw(selectBottomButtonImage, bottomButton[i].sprite);
	}

}

void LoadDisplay::Open(ID3D11DeviceContext* _dc)
{
	if (_dc == nullptr)return;

	progressCircleRoutate = 0.0f;

	dc = _dc;

	ChCpp::WCharFile file;
	file.SetLocaleName("Japanese");
	file.FileOpen(PLAYER_MECHA_PATH);
	std::wstring fileText = file.FileReadText();
	file.FileClose();

	loadFileList = ChPtr::Make_S<ChCpp::JsonArray<wchar_t>>();

	loadFileList->SetRawData(fileText);
}

void LoadDisplay::Close()
{
	selectList->Release();

	loadFileList = nullptr;

	openFlg = false;
}

void LoadDisplay::Load()
{
	auto&& item = selectList->GetSelectItem(selectList->GetNowSelect());
	if (item == nullptr)return;
	auto&& mecha = ChPtr::SharedSafeCast<LoadItem>(item);
	if (mecha == nullptr)return;
	mecha->mecha->Save(PLAYER_USE_MECHA_PATH);
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

	//return true;

	auto&& assembleMechaFrame = loadFileList->GetJsonObject(_openNumber);
	auto&& loadMecha = ChPtr::Make_S<LoadItem>();

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
		loadMecha->mecha->GetMechaName(),
		textDrawer.format,
		textDrawer.brush,
		ChVec4::FromRect(0.0f, 0.0f, MECHA_NAME_TEXT_WIDTH, MECHA_NAME_TEXT_HEIGHT)
	);

	textDrawer.drawer.DrawEnd();

	loadMecha->panelName.CreateColorTexture(device, textDrawer.bitmap.GetBitmap());

	selectList->AddItem(loadMecha);

	return true;
}
