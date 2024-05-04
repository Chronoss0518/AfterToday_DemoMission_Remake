#include"../BaseIncluder.h"

#include"EditFrame.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../Attack/Attack.h"

#include"../EditFrame/ParameterList.h"

#include"../LoadDisplay/LoadDisplay.h"

#include"../SelectList/SelectList.h"

#define EDIT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Edit/") current_path

#define MECHA_ROTATION_SPEED 1.0f

#define PARTS_PANEL_LIST_X 30.0f
#define PARTS_PANEL_LIST_Y 141.0f

#define PANEL_COUNT 4

#define PANEL_SIZE_W 280.0f
#define PANEL_SIZE_H 102.0f

#define UP_BUTTON_PANEL_Y 30.0f
#define DOWN_BUTTON_PANEL_Y 558.0f

#define TEXT_ALIGN 8.0f

#define PANEL_TEXT_X TEXT_ALIGN
#define PANEL_TEXT_WIDTH PANEL_SIZE_W - (TEXT_ALIGN * 2.0f)

#define PANEL_TITLE_Y 26.0f
#define PANEL_TITLE_HEIGHT 45.0f
#define PANEL_TITLE_TEXT_SIZE 24.0f

#define PANEL_POS_TITLE_Y 17.0f
#define PANEL_POS_TITLE_HEIGHT 25.0f
#define PANEL_POS_TITLE_TEXT_SIZE 16.0f

#define PANEL_POS_PARTS_Y 42.0f
#define PANEL_POS_PARTS_HEIGHT 45.0f
#define PANEL_POS_PARTS_TEXT_SIZE 24.0f

#define TMP_CAMERA_POS ChVec3(0.0f, 2.5f, 10.0f)

#define LOAD_PARTS_COUNT 1

#define NOW_LOADING_ANIMATION_MOVE_SPEED 0.5f
#define NOW_LOADING_ANIMATION_WAIT_TIME 0.2f

class EditListItem : public SelectListItemBase
{
public:

	inline virtual void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect, ChD3D11::Sprite11& _sprite)
	{
		ChVec4 rect = _rect;
		rect.left += TEXT_ALIGN;
		rect.right -= TEXT_ALIGN;

		rect.top = _rect.top + PANEL_TITLE_Y;
		rect.bottom = rect.top + PANEL_TITLE_HEIGHT;

		_sprite.SetPosRect(RectToGameWindow(rect));
		_drawer.Draw(*positionNameTexture, _sprite);
	}

	ChPtr::Shared<ChD3D11::Texture11> positionNameTexture = nullptr;
};

class EditListPartsItem : public EditListItem
{
public:

	inline void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect, ChD3D11::Sprite11& _sprite)override
	{
		ChVec4 rect = _rect;
		rect.left += TEXT_ALIGN;
		rect.right -= TEXT_ALIGN;

		rect.top = _rect.top + PANEL_POS_TITLE_Y;
		rect.bottom = rect.top + PANEL_POS_TITLE_HEIGHT;

		_sprite.SetPosRect(RectToGameWindow(rect));
		_drawer.Draw(*positionNameTexture, _sprite);

		rect.top = _rect.top + PANEL_POS_PARTS_Y;
		rect.bottom = rect.top + PANEL_POS_PARTS_HEIGHT;

		_sprite.SetPosRect(RectToGameWindow(rect));
		_drawer.Draw(*partsNameTexture, _sprite);
	}

	ChPtr::Shared<ChD3D11::Texture11>  partsNameTexture = nullptr;
	ChPtr::Shared<MechaPartsObject> targetParts = nullptr;
};

class EditList :public SelectListBase
{
public:

	EditList()
	{
		sprite.Init();
	}

public:

	void CreateSelectImage(const std::string& _fileName, ID3D11Device* _device)
	{
		if (_fileName.empty())return;
		selectImage.CreateTexture(_fileName, _device);
	}

	void CreatePanelBackGround(const std::string& _fileName,ID3D11Device* _device)
	{
		if (_fileName.empty())return;
		background.CreateTexture(_fileName, _device);
	}

public:

	ChD3D11::Texture11* GetSelectImage() { return &selectImage; }

public:

	void DrawPanel(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect, ChPtr::Shared<SelectListItemBase> _drawItem, unsigned long _itemNo, bool _isSelectPanel)override
	{
		auto&& item = ChPtr::SharedSafeCast<EditListItem>(_drawItem);
		if (item == nullptr)return;

		if (background.IsTex())
		{
			sprite.SetPosRect(RectToGameWindow(_rect));
			_drawer.Draw(background, sprite);
		}

		if (selectImage.IsTex() && _isSelectPanel)
		{
			sprite.SetPosRect(RectToGameWindow(_rect));
			_drawer.Draw(selectImage, sprite);
		}

		item->Draw(_drawer, _rect, sprite);

	}

private:

	ChD3D11::Texture11 selectImage;
	ChD3D11::Texture11 background;
	ChD3D11::Sprite11 sprite;
};

void EditFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	ChD3D11::Shader11().SetBackColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));

	controller.Init();

	MenuBase::InitMenu(&controller);

	auto&& device = ChD3D11::D3D11Device();
	spriteShader.Init(device);
	rtView.CreateRenderTarget(GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	dsView.CreateDepthBuffer(GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);

	backgroundSprite.Init();
	backgroundSprite.SetInitPosition();
	meshDrawer.Init(device);
	
	{
		ChMat_11 mat;
		mat.CreateProjectionMat(ChMath::ToRadian(60.0f), GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 0.1f, 1000.0f);
		meshDrawer.SetProjectionMatrix(mat);
		mat.CreateViewMatLookTarget(TMP_CAMERA_POS, ChVec3(0.0f, 2.0f, 0.0f), ChVec3(0.0f, 1.0f, 0.0f));

		meshDrawer.SetViewMatrix(mat);
	}

	nowLoadingSprite.Init();
	nowLoadingSprite.SetInitPosition();
	nowLoading.CreateTexture(TEXTURE_DIRECTORY("NowLoading.png"), device);
	InitNowLoadingRect();


	light.Init(device);
	light.SetUseLightFlg(true);
	light.SetCamPos(TMP_CAMERA_POS);
	auto&& direction = ChVec3(3.0f, -2.0f, -4.0f);
	direction.Normalize();
	light.SetLightDir(direction);

	InitTextDrawer(panelTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_TITLE_HEIGHT), PANEL_TITLE_TEXT_SIZE,true);
	InitTextDrawer(panelPosTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_TITLE_HEIGHT), PANEL_POS_TITLE_TEXT_SIZE,false);
	InitTextDrawer(panelPosPartsDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_PARTS_HEIGHT), PANEL_POS_PARTS_TEXT_SIZE,true);


	loadDisplay = ChPtr::Make_S<LoadDisplay>();
	loadDisplay->Init(device, &controller);

	editMecha = ChPtr::Make_S<BaseMecha>();

	parameterList = ChPtr::Make_S<ParameterList>();

	partsList = ChPtr::Make_S<EditList>();
	partsList->SetDrawCount(PANEL_COUNT);
	partsList->SetMoveDiraction(MoveDiraction::Vertical);
	partsList->SetPanelSize(ChVec2::FromSize(PANEL_SIZE_W, PANEL_SIZE_H));
	partsList->SetStartPosition(PARTS_PANEL_LIST_X, PARTS_PANEL_LIST_Y);
	partsList->SetAlighSize(0.0f, PANEL_SIZE_H);
	partsList->CreatePanelBackGround(EDIT_TEXTURE_DIRECTORY("PartsPanel.png"), device);
	partsList->CreateSelectImage(EDIT_TEXTURE_DIRECTORY("PartsPanelSelect.png"), device);

	selectButton[ChStd::EnumCast(SelectButtonType::Up)].image.CreateTexture(EDIT_TEXTURE_DIRECTORY("UPButton.png"), device);
	SPRITE_INIT(selectButton[ChStd::EnumCast(SelectButtonType::Up)].sprite,
		RectToGameWindow(ChVec4::FromRect(PARTS_PANEL_LIST_X, UP_BUTTON_PANEL_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, UP_BUTTON_PANEL_Y + PANEL_SIZE_H)));

	selectButton[ChStd::EnumCast(SelectButtonType::Down)].image.CreateTexture(EDIT_TEXTURE_DIRECTORY("DownButton.png"), device);
	SPRITE_INIT(selectButton[ChStd::EnumCast(SelectButtonType::Down)].sprite,
		RectToGameWindow(ChVec4::FromRect(PARTS_PANEL_LIST_X, DOWN_BUTTON_PANEL_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, DOWN_BUTTON_PANEL_Y + PANEL_SIZE_H)));

	selectFlg = false;

	rightPanelBackGround.CreateTexture(EDIT_TEXTURE_DIRECTORY("PanelList.png"), device);
	leftPanelBackGround.CreateTexture(EDIT_TEXTURE_DIRECTORY("PanelList.png"), device);
	
	Load();
}

void EditFrame::Release()
{
	loadDisplay = nullptr;

	MechaParts::ClearPartsList();
	Attack::AllRelease();
}

void EditFrame::Update()
{
	loadEndFlg = LoadPart();

	if (loadEndFlg)
	{
		MenuBase::UpdateFunction();

		auto&& rotate = editMecha->GetRotation();

		rotate += ChVec3(0.0f, MECHA_ROTATION_SPEED, 0.0f);

		editMecha->SetRotation(rotate);
	}

	UpdateNowLoadingRect();

	DrawFunction();
}

void EditFrame::InitTextDrawer(TextDrawerWICBitmap& _initDrawer, const ChVec2& _textureSize, const float _fontSize, bool _boldFlg)
{
	unsigned long w = static_cast<unsigned long>(_textureSize.w);
	unsigned long h = static_cast<unsigned long>(_textureSize.h);

	_initDrawer.bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(w, h);

	_initDrawer.drawer.Init(w, h, _initDrawer.bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);
	_initDrawer.drawer.SetClearDisplayFlg(true);
	_initDrawer.drawer.SetClearDisplayColor(ChVec4(0.0f));

	_initDrawer.brush = _initDrawer.drawer.CreateBrush(ChVec4::FromColor(0.0f,0.0f,0.0f,1.0f));

	_initDrawer.format = _initDrawer.drawer.CreateTextFormat(L"ÉSÉVÉbÉN",nullptr, _boldFlg ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,_fontSize);
}

void EditFrame::InitNowLoadingRect()
{
	animationMoveSpeed = 1.0f / (NOW_LOADING_ANIMATION_MOVE_SPEED * static_cast<float>(ChSystem::SysManager().GetFPS()));
	animationWaitTime = 1.0f / (NOW_LOADING_ANIMATION_WAIT_TIME * static_cast<float>(ChSystem::SysManager().GetFPS()));
	nowLoadingPosRect = ChVec4::FromRect(-1.0f, 1.0f, -1.0f, -1.0f);
	nowLoadingUVRect = ChVec4::FromRect(0.0f, 0.0f, 0.0f, 1.0f);
	nowAnimationWaitTime = 1.0f;
	upFlg = true;
}

void EditFrame::SetPartsList(MechaPartsObject& _parts)
{
	auto&& base = _parts.GetBaseObject();

	auto&& selectPartsPanel = ChPtr::Make_S<EditListPartsItem>();

	selectPartsPanel->positionNameTexture = CreatePanelPosTitleTexture(L"Select Parts");

	selectPartsPanel->partsNameTexture = CreatePanelPosPartsTexture(ChStr::UTF8ToWString(base->GetMyName()));

	partsList->AddItem(selectPartsPanel);

	for (auto&& position : base->GetPositionList())
	{
		auto&& child = _parts.GetChildParts(position.first);

		ChPtr::Shared<EditListItem>item = nullptr;

		SetPanelItem(item, child, position.first);
		
		SetPanelPartsItem(item, child, position.first);

		partsList->AddItem(item);
	}

}

void EditFrame::SetPanelItem(ChPtr::Shared<EditListItem>& _res, ChPtr::Shared<MechaPartsObject>& _parts, const std::string& _positionName)
{
	if (_parts != nullptr)return;
	auto&& res = ChPtr::Make_S<EditListItem>();

	res->positionNameTexture = CreatePanelTitleTexture(ChStr::UTF8ToWString(_positionName));

	_res = res;
}

void EditFrame::SetPanelPartsItem(ChPtr::Shared<EditListItem>& _res, ChPtr::Shared<MechaPartsObject>& _parts, const std::string& _positionName)
{
	if (_parts == nullptr)return;
	auto&& res = ChPtr::Make_S<EditListPartsItem>();

	res->positionNameTexture = CreatePanelPosTitleTexture(ChStr::UTF8ToWString(_positionName));

	auto&& base = _parts->GetBaseObject();

	res->partsNameTexture = CreatePanelPosPartsTexture(ChStr::UTF8ToWString(base->GetMyName()));

	res->targetParts = _parts;

	_res = res;
}

void EditFrame::UpdateAction(ActionType _type)
{

	parameterList->Update(_type);

	partsList->UpdateAction(_type);

	if (_type == ActionType::Cancel)
	{
		if (selectStack.empty())
		{
			ChangeFrame(ChStd::EnumCast(FrameNo::Select));
			return;
		}

		partsList->SetDrawPosition(0);
		selectParts = selectStack[selectStack.size() - 1];
		selectStack.pop_back();

		parameterList->SetBaseParts(ChD3D11::D3D11Device(),selectParts);

		partsList->ClearItem();
		SetPartsList(*selectParts);
	}

	if (_type == ActionType::Decision)
	{
		if (selectType == SelectButtonType::Up)
		{
			AddActionType(ActionType::Up);
			return;
		}

		if (selectType == SelectButtonType::Down)
		{
			AddActionType(ActionType::Down);
			return;
		}

		auto&& partsPanel = ChPtr::SharedSafeCast<EditListPartsItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

		if (partsPanel == nullptr)return;

		if (partsPanel->targetParts == nullptr)
		{
			AddActionType(ActionType::Cancel);

			return;
		}

		partsList->SetDrawPosition(0);
		selectStack.push_back(selectParts);
		selectParts = nullptr;
		selectParts = partsPanel->targetParts;

		parameterList->SetBaseParts(ChD3D11::D3D11Device(), selectParts);

		partsList->ClearItem();
		SetPartsList(*selectParts);

	}

}

void EditFrame::UpdateMouse()
{

	auto&& manager = ChSystem::SysManager();


	InputTest(MenuBase::ActionType::Decision, manager.IsPushKeyNoHold(VK_LBUTTON));

	InputTest(MenuBase::ActionType::Cancel, manager.IsPushKeyNoHold(VK_RBUTTON));

	auto&& mouse = ChWin::Mouse();
	mouse.Update();

	auto&& mouseMove = mouse.GetMoveValue();

	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

	for (unsigned char i = 0; i < ChStd::EnumCast(SelectButtonType::None); i++)
	{
		if (!IsMoucePosOnSprite(selectButton[i].sprite))continue;
		selectType = static_cast<SelectButtonType>(i);
		return;
	}

	selectType = SelectButtonType::None;
	
	partsList->UpdateMouse();
}

void EditFrame::UpdateNowLoadingRect()
{
	if (upFlg)
	{
		nowLoadingPosRect.right += animationMoveSpeed;
		nowLoadingUVRect.right += (animationMoveSpeed * 0.5f);

		if (nowLoadingPosRect.right < 1.0f)return;

		upFlg = false;
		nowLoadingPosRect.right = 1.0f;
		nowLoadingUVRect.right = 1.0f;
	}
	else
	{
		if ((nowAnimationWaitTime -= animationWaitTime) > 0.0f)return;

		nowLoadingPosRect.left += animationMoveSpeed;
		nowLoadingUVRect.left += (animationMoveSpeed * 0.5f);

		if (nowLoadingPosRect.left <= 1.0f)return;
		InitNowLoadingRect();
	}
}

void EditFrame::DrawFunction()
{
	auto&& dc = ChD3D11::D3D11DC();

	rtView.SetBackColor(dc, ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	dsView.ClearDepthBuffer(dc);

	ChD3D11::Shader11().DrawStart();

	ID3D11RenderTargetView* tmpView = rtView.GetRTView();

	dc->OMSetRenderTargets(1, &tmpView, dsView.GetDSView());

	if (loadEndFlg)
	{
		meshDrawer.DrawStart(dc);
		light.SetDrawData(dc);

		editMecha->Draw3D();

		meshDrawer.DrawEnd();
	}

	dc->OMSetRenderTargets(1, &tmpView, nullptr);

	spriteShader.SetAlphaBlendFlg(true);
	spriteShader.DrawStart(dc);

	DrawEndLoading();

	DrawNowLoading(dc);

	loadDisplay->Draw(spriteShader);

	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd(rtView);
}

void EditFrame::DrawNowLoading(ID3D11DeviceContext* _dc)
{
	if (loadEndFlg)return;

	nowLoadingSprite.SetPosRect(nowLoadingPosRect);
	nowLoadingSprite.SetUVPosRect(nowLoadingUVRect);

	spriteShader.Draw(nowLoading, nowLoadingSprite);

}

void EditFrame::DrawEndLoading()
{
	if (!loadEndFlg)return;

	ChVec2 pos = parameterList->GetDrawStartPosition();
	ChVec2 size = parameterList->GetDrawPanelSize();
	ChVec4 rect = ChVec4::FromRect(pos.x, pos.y, pos.x + size.w,pos.y + size.h);

	backgroundSprite.SetPosRect(RectToGameWindow(rect));

	spriteShader.Draw(rightPanelBackGround, backgroundSprite);

	parameterList->Draw(spriteShader);

	rect = ChVec4::FromRect(PARTS_PANEL_LIST_X, PARTS_PANEL_LIST_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, PARTS_PANEL_LIST_Y + PANEL_SIZE_H * PANEL_COUNT);

	backgroundSprite.SetPosRect(RectToGameWindow(rect));

	spriteShader.Draw(leftPanelBackGround, backgroundSprite);

	partsList->Draw(spriteShader);


	for (unsigned char i = 0; i < ChStd::EnumCast(SelectButtonType::None); i++)
	{

		spriteShader.Draw(selectButton[i].image, selectButton[i].sprite);

		if (ChStd::EnumCast(selectType) == i)
			spriteShader.Draw(*partsList->GetSelectImage(), selectButton[i].sprite);
	}

}

ChPtr::Shared<ChD3D11::Texture11>EditFrame::CreatePanelTitleTexture(const std::wstring& _str)
{
	return CreatePanelTexture(_str, panelTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_TITLE_HEIGHT));
}

ChPtr::Shared<ChD3D11::Texture11>EditFrame::CreatePanelPosTitleTexture(const std::wstring& _str)
{
	return CreatePanelTexture(_str, panelPosTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_TITLE_HEIGHT));
}

ChPtr::Shared<ChD3D11::Texture11>EditFrame::CreatePanelPosPartsTexture(const std::wstring& _str)
{
	return CreatePanelTexture(_str, panelPosPartsDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_PARTS_HEIGHT));
}

ChPtr::Shared<ChD3D11::Texture11>EditFrame::CreatePanelTexture(const std::wstring& _str, TextDrawerWICBitmap& _drawer, const ChVec2& _size)
{
	_drawer.drawer.DrawStart();

	_drawer.drawer.DrawToScreen(_str, _drawer.format, _drawer.brush, ChVec4::FromRect(0.0f, 0.0f, _size.w, _size.h));

	_drawer.drawer.DrawEnd();

	auto&& res = ChPtr::Make_S<ChD3D11::Texture11>();

	res->CreateColorTexture(ChD3D11::D3D11Device(), _drawer.bitmap.GetBitmap());

	return res;
}

void EditFrame::Load()
{

	for (auto&& file : std::filesystem::directory_iterator(PARTS_DIRECTORY("")))
	{
		std::wstring path = file.path().c_str();

		unsigned long len = path.rfind(L".");

		if (len == std::wstring::npos)continue;

		std::wstring extensionName = path.substr(len);

		if (extensionName == L".lst")continue;

 		pathList.push_back(ChStr::UTF8ToString(path));
	}

}

bool EditFrame::LoadPart()
{
	for (unsigned long i = 0; i < LOAD_PARTS_COUNT; i++)
	{
		if (pathList.size() <= loadCount)return true;

		auto&& parts = MechaParts::LoadParts(*editMecha, ChD3D11::D3D11Device(), &meshDrawer, nullptr, pathList[loadCount]);

		parts->GetBaseObject()->SetParameters();

		loadCount++;

		parts = nullptr;

		if (pathList.size() > loadCount)continue;

		editMecha->Create(ChVec2(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), meshDrawer, nullptr);
		editMecha->Load(ChD3D11::D3D11Device(), PLAYER_USE_MECHA_PATH);

		selectParts = editMecha->GetCoreParts();

		parameterList->Init(ChD3D11::D3D11Device(), editMecha);

		SetPartsList(*selectParts);

		return true;
	}

	return false;

}