#include"../BaseIncluder.h"

#include"EditFrame.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../Attack/Attack.h"

#include"../EditFrame/ParameterList.h"

#include"../LoadDisplay/LoadDisplay.h"

#include"../SelectList/SelectList.h"

#include"../Application/Application.h"

#define EDIT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY(L"Edit/") current_path

#define MECHA_ROTATION_SPEED 1.0f

#define PARTS_PANEL_LIST_X 30.0f
#define PARTS_PANEL_LIST_Y 141.0f

#define EDIT_CONTROL_BUTTON_PARTS_PANEL_LIST_X 360.0f
#define EDIT_CONTROL_BUTTON_PARTS_PANEL_LIST_Y 186.0f

#define PANEL_COUNT 4
#define EDIT_CONTROL_BUTTON_USE_SELECT_PANEL_COUNT 4
#define EDIT_CONTROL_BUTTON_UN_USE_SELECT_PANEL_COUNT 3

#define PANEL_SIZE_W 280.0f
#define PANEL_SIZE_H 102.0f

#define EDIT_CONTROL_BUTTON_PANEL_SIZE_W 183.0f
#define EDIT_CONTROL_BUTTON_PANEL_SIZE_H 102.0f

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
	std::wstring partsPosName = L"";
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

	void Init()
	{
		auto&& device = AppIns().GetDirect3D11().GetDevice();

		sprite.Init();
		SelectListBase::Init();

		SetDrawCount(PANEL_COUNT);
		SetMoveDiraction(MoveDiraction::Vertical);
		SetPanelSize(ChVec2::FromSize(PANEL_SIZE_W, PANEL_SIZE_H));
		SetStartPosition(PARTS_PANEL_LIST_X, PARTS_PANEL_LIST_Y);
		SetAlighSize(0.0f, PANEL_SIZE_H);
		background.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PartsPanel.png"), device);
		selectImage.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PartsPanelSelect.png"), device);
	}

public:

	ChD3D11::Texture11* GetSelectImage() { return &selectImage; }

public:

	void DrawPanel(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect, ChPtr::Shared<SelectListItemBase> _drawItem, size_t _itemNo, bool _isSelectPanel)override
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

class SelectPartsListItem : public SelectListItemBase
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
	std::wstring partsPath = L"";
};

class SelectPartsList :public SelectListBase
{
public:

	void Init()override
	{
		auto&& device = AppIns().GetDirect3D11().GetDevice();

		sprite.Init();
		SelectListBase::Init();

		SetDrawCount(PANEL_COUNT);
		SetMoveDiraction(MoveDiraction::Vertical);
		SetPanelSize(ChVec2::FromSize(PANEL_SIZE_W, PANEL_SIZE_H));
		SetStartPosition(PARTS_PANEL_LIST_X, PARTS_PANEL_LIST_Y);
		SetAlighSize(0.0f, PANEL_SIZE_H);
		background.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PartsPanel.png"), device);
		selectImage.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PartsPanelSelect.png"), device);
	}

public:

	ChD3D11::Texture11* GetSelectImage() { return &selectImage; }

public:

	void DrawPanel(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect, ChPtr::Shared<SelectListItemBase> _drawItem, size_t _itemNo, bool _isSelectPanel)override
	{
		auto&& item = ChPtr::SharedSafeCast<SelectPartsListItem>(_drawItem);
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

class EditControlListItem : public SelectListItemBase
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
	EditFrame::EditControlType type = EditFrame::EditControlType::Select;
};

class EditControlList :public SelectListBase
{
public:

	void Init()override
	{
		auto&& device = AppIns().GetDirect3D11().GetDevice();

		sprite.Init();
		SelectListBase::Init();

		SetMoveDiraction(MoveDiraction::Vertical);
		SetPanelSize(ChVec2::FromSize(EDIT_CONTROL_BUTTON_PANEL_SIZE_W, EDIT_CONTROL_BUTTON_PANEL_SIZE_H));
		SetStartPosition(EDIT_CONTROL_BUTTON_PARTS_PANEL_LIST_X, EDIT_CONTROL_BUTTON_PARTS_PANEL_LIST_Y);
		SetAlighSize(0.0f, EDIT_CONTROL_BUTTON_PANEL_SIZE_H);
		background.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PartsPanel.png"), device);
		selectImage.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PartsPanelSelect.png"), device);
	}

public:

	ChD3D11::Texture11* GetSelectImage() { return &selectImage; }

public:

	void DrawPanel(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect, ChPtr::Shared<SelectListItemBase> _drawItem, size_t _itemNo, bool _isSelectPanel)override
	{
		auto&& item = ChPtr::SharedSafeCast<EditControlListItem>(_drawItem);
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
	auto&& device = AppIns().GetDirect3D11().GetDevice();

	AppIns().GetDirect3D11().SetBackColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));

	MenuBase::InitMenu();

	spriteShader.Init(device);
	rtView.CreateRenderTarget(device, GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	dsView.CreateDepthBuffer(device, GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);

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
	nowLoading.CreateTexture(TEXTURE_DIRECTORY(L"NowLoading.png"), device);
	InitNowLoadingRect();


	light.Init(device);
	light.SetUseLightFlg(true);
	light.SetCamPos(TMP_CAMERA_POS);
	auto&& direction = ChVec3(3.0f, -2.0f, -4.0f);
	direction.Normalize();
	light.SetDirectionLightData(true,ChVec3(1.0f,1.0f,1.0f), direction, 0.4f);

	InitTextDrawer(panelTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_TITLE_HEIGHT), PANEL_TITLE_TEXT_SIZE,true);
	InitTextDrawer(panelPosTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_TITLE_HEIGHT), PANEL_POS_TITLE_TEXT_SIZE,false);
	InitTextDrawer(panelPosPartsDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_PARTS_HEIGHT), PANEL_POS_PARTS_TEXT_SIZE,true);

	loadDisplay = ChPtr::Make_S<LoadDisplay>();
	loadDisplay->Init();

	editMecha = ChPtr::Make_S<BaseMecha>();

	parameterList = ChPtr::Make_S<ParameterList>();
	AddSpecialKey('Z');
	AddSpecialKey('X');
	AddSpecialKeyMask(XINPUT_GAMEPAD_X | XINPUT_GAMEPAD_Y);

	partsList = ChPtr::Make_S<EditList>();
	partsList->Init();

	selectPartsList = ChPtr::Make_S<SelectPartsList>();
	selectPartsList->Init();

	{
		auto&& panel = ChPtr::Make_S<SelectPartsListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Cancel");

		selectPartsList->AddItem(panel);
	}

	editControlButtons = ChPtr::Make_S<EditControlList>();
	editControlButtons->Init();

	selectButton[ChStd::EnumCast(SelectButtonType::Up)].image.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"UPButton.png"), device);
	SPRITE_INIT(selectButton[ChStd::EnumCast(SelectButtonType::Up)].sprite,
		RectToGameWindow(ChVec4::FromRect(PARTS_PANEL_LIST_X, UP_BUTTON_PANEL_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, UP_BUTTON_PANEL_Y + PANEL_SIZE_H)));

	selectButton[ChStd::EnumCast(SelectButtonType::Down)].image.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"DownButton.png"), device);
	SPRITE_INIT(selectButton[ChStd::EnumCast(SelectButtonType::Down)].sprite,
		RectToGameWindow(ChVec4::FromRect(PARTS_PANEL_LIST_X, DOWN_BUTTON_PANEL_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, DOWN_BUTTON_PANEL_Y + PANEL_SIZE_H)));

	selectFlg = false;

	rightPanelBackGround.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PanelList.png"), device);
	leftPanelBackGround.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PanelList.png"), device);
	
	Load();

	sendData = ChPtr::SharedSafeCast<FromStageSelectFrameData>(_sendData);
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

		editMecha->DestroyCoreTest();

		auto&& rotate = editMecha->GetRotation();

		rotate += ChVec3(0.0f, MECHA_ROTATION_SPEED, 0.0f);

		editMecha->SetRotation(rotate);

		UpdateSelectPartsSetter();
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
	animationMoveSpeed = 1.0f / (NOW_LOADING_ANIMATION_MOVE_SPEED * static_cast<float>(AppIns().GetFPS()));
	animationWaitTime = 1.0f / (NOW_LOADING_ANIMATION_WAIT_TIME * static_cast<float>(AppIns().GetFPS()));
	nowLoadingPosRect = ChVec4::FromRect(-1.0f, 1.0f, -1.0f, -1.0f);
	nowLoadingUVRect = ChVec4::FromRect(0.0f, 0.0f, 0.0f, 1.0f);
	nowAnimationWaitTime = 1.0f;
	upFlg = true;
}

void EditFrame::CreateEditControlButtonUseSelectItems()
{
	if(editControlButtons->GetCount() == EDIT_CONTROL_BUTTON_USE_SELECT_PANEL_COUNT)return ;

	editControlButtons->ClearItem();
	editControlButtons->SetDrawCount(EDIT_CONTROL_BUTTON_USE_SELECT_PANEL_COUNT);
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Select");
		panel->type = EditControlType::Select;

		editControlButtons->AddItem(panel);
	}
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Change");
		panel->type = EditControlType::Change;

		editControlButtons->AddItem(panel);
	}
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Remove");
		panel->type = EditControlType::Remove;

		editControlButtons->AddItem(panel);
	}
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Cancel");
		panel->type = EditControlType::Cancel;

		editControlButtons->AddItem(panel);
	}

	useSelectButtonFlg = true;

}

void EditFrame::CreateEditControlButtonUnUseSelectItems()
{
	if (editControlButtons->GetCount() == EDIT_CONTROL_BUTTON_UN_USE_SELECT_PANEL_COUNT)return;

	editControlButtons->ClearItem();
	editControlButtons->SetDrawCount(EDIT_CONTROL_BUTTON_UN_USE_SELECT_PANEL_COUNT);

	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Change");
		panel->type = EditControlType::Change;

		editControlButtons->AddItem(panel);
	}
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Remove");
		panel->type = EditControlType::Remove;

		editControlButtons->AddItem(panel);
	}
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Cancel");
		panel->type = EditControlType::Cancel;

		editControlButtons->AddItem(panel);
	}

	useSelectButtonFlg = false;
}

void EditFrame::SetPartsList(MechaPartsObject* _parts)
{
	partsList->ClearItem();

	{
		auto&& panel = ChPtr::Make_S<EditListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Back");

		partsList->AddItem(panel);
	}

	if (ChPtr::NullCheck(_parts))
	{

		auto&& panel = ChPtr::Make_S<EditListItem>();
		panel->positionNameTexture = CreatePanelTitleTexture(L"+ " LOAD_JSON_CORE_PARAM_NAME);
		panel->partsPosName = LOAD_JSON_CORE_PARAM_NAME;

		partsList->AddItem(panel);
		return;
	}

	auto&& base = _parts->GetBaseObject();

	auto&& selectPartsPanel = ChPtr::Make_S<EditListPartsItem>();

	selectPartsPanel->positionNameTexture = CreatePanelPosTitleTexture(L"Select Parts");

	selectPartsPanel->partsNameTexture = CreatePanelPosPartsTexture(base->GetMyName());

	selectPartsPanel->partsPosName = _parts->GetPartsPosName();

	partsList->AddItem(selectPartsPanel);

	for (auto&& position : base->GetPositionList())
	{
		auto&& child = _parts->GetChildParts(position.first);

		ChPtr::Shared<EditListItem>item = nullptr;

		SetPanelItem(item, child, position.first);
		
		SetPanelPartsItem(item, child, position.first);

		partsList->AddItem(item);
	}

}

void EditFrame::SetPanelItem(ChPtr::Shared<EditListItem>& _res, ChPtr::Shared<MechaPartsObject>& _parts, const std::wstring& _positionName)
{
	if (_parts != nullptr)return;
	auto&& res = ChPtr::Make_S<EditListItem>();

	res->positionNameTexture = CreatePanelTitleTexture(L"+ " + _positionName);
	res->partsPosName = _positionName;

	_res = res;
}

void EditFrame::SetPanelPartsItem(ChPtr::Shared<EditListItem>& _res, ChPtr::Shared<MechaPartsObject>& _parts, const std::wstring& _positionName)
{
	if (_parts == nullptr)return;
	auto&& res = ChPtr::Make_S<EditListPartsItem>();

	res->positionNameTexture = CreatePanelPosTitleTexture(_positionName);

	auto&& base = _parts->GetBaseObject();

	res->partsNameTexture = CreatePanelPosPartsTexture(base->GetMyName());

	res->partsPosName = _positionName;
	res->targetParts = _parts;

	_res = res;
}

void EditFrame::UpdateAction(ActionType _type)
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();

	parameterList->Update(_type);

	if (editControlButtons->GetCount() <= 0)
	{
		partsSelectFlg ?
			selectPartsList->UpdateAction(_type) :
			partsList->UpdateAction(_type);
	}
	else
	{
		editControlButtons->UpdateAction(_type);
	}

	if (_type == ActionType::Cancel)
	{
		if (partsSelectFlg)
		{
			selectPartsList->SetNowSelect(0);
			selectPartsList->SetDrawPosition(0);

			UpdateSelectPartsSetter();

			RefreshPartsList();
			partsSelectFlg = false;
			return;
		}

		if (selectParts == nullptr)
		{
			if (sendData == nullptr)
			{
				ChangeFrame(ChStd::EnumCast(FrameNo::Select));
			}
			else
			{
				SendData(sendData);
				ChangeFrame(ChStd::EnumCast(FrameNo::SelectStage));
			}
			return;
		}

		if (selectParts->GetParent() == nullptr)
		{
			if (sendData == nullptr)
			{
				ChangeFrame(ChStd::EnumCast(FrameNo::Select));
			}
			else
			{
				SendData(sendData);
				ChangeFrame(ChStd::EnumCast(FrameNo::SelectStage));
			}
			return;
		}

		UpdateButtonRemove();

	}

	if (_type != ActionType::Decision)return;

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

	if (editControlButtons->GetCount() <= 0)
	{
		partsSelectFlg ?
			UpdateSelectPartsListAction(_type) :
			UpdatePartsListAction(_type);

		return;
	}

	auto buttonItem = ChPtr::SharedSafeCast<EditControlListItem>(editControlButtons->GetSelectItem(editControlButtons->GetNowSelect()));

	if (buttonItem->type == EditControlType::Select)
		UpdateButtonSelect();

	if (buttonItem->type == EditControlType::Change)
		UpdateButtonChange();

	if (buttonItem->type == EditControlType::Remove)
	{
		if(selectParts != nullptr)selectParts->Destroy();
		selectParts = nullptr;
		UpdateButtonRemove();
	}

	if (buttonItem->type == EditControlType::Cancel)
		UpdateButtonCancel();

}

void EditFrame::UpdateMouse()
{

	auto&& keyInput = AppIns().GetKeyInput();


	InputTest(MenuBase::ActionType::Decision, keyInput.IsPushKeyNoHold(VK_LBUTTON));

	InputTest(MenuBase::ActionType::Cancel, keyInput.IsPushKeyNoHold(VK_RBUTTON));

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

	if (editControlButtons->GetCount() <= 0)
	{
		partsSelectFlg ?
			selectPartsList->UpdateMouse() :
			partsList->UpdateMouse();
	}
	else
	{
		editControlButtons->UpdateMouse();
	}

	selectType = SelectButtonType::None;

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

void EditFrame::UpdatePartsListAction(ActionType _type)
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();

	auto&& partsPanel = ChPtr::SharedSafeCast<EditListPartsItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

	if (partsPanel == nullptr)
	{
		auto panel = ChPtr::SharedSafeCast<EditListItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

		if (panel->partsPosName == L"")
		{
			AddActionType(ActionType::Cancel);

			return;
		}

		useSelectButtonFlg = true;

		OpenPartsSelectList();


		return;
	}

	if (partsPanel->targetParts == nullptr)
	{
		//AddActionType(ActionType::Cancel);
		CreateEditControlButtonUnUseSelectItems();
		return;
	}

	CreateEditControlButtonUseSelectItems();

}

void EditFrame::RefreshPartsList()
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();

	partsList->SetDrawPosition(0);

	partsList->ClearItem();
	SetPartsList(selectParts.get());

	parameterList->SetBaseParts(device, selectParts);

}

void EditFrame::OpenPartsSelectList()
{
	auto&& partsPanel = ChPtr::SharedSafeCast<EditListPartsItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

	if (partsPanel == nullptr)
	{
		auto panel = ChPtr::SharedSafeCast<EditListItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

		changePartsPosName = panel->partsPosName;

		nowChangeTargetPartsParent = ChPtr::SharedSafeCast<MechaPartsObject>(!useSelectButtonFlg ? selectParts->GetParent() : selectParts);

		beforePartsJson = selectParts != nullptr ? selectParts->Serialize() : ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();
	}
	else
	{

		changePartsPosName = partsPanel->partsPosName;
		
		nowChangeTargetPartsParent = ChPtr::SharedSafeCast<MechaPartsObject>(selectParts->GetParent());

		beforePartsJson = selectParts->Serialize();
	}


	if (changePartsPosName == LOAD_JSON_CORE_PARAM_NAME)changePartsPosName = L"";

	partsSelectFlg = true;

}

void EditFrame::UpdateSelectPartsListAction(ActionType _type)
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();

	auto&& partsPanel = ChPtr::SharedSafeCast<SelectPartsListItem>(selectPartsList->GetSelectItem(selectPartsList->GetNowSelect()));

	if (partsPanel == nullptr)return;

	UpdateSelectPartsSetter();

	if (!useSelectButtonFlg)
	{
		selectParts = tmpSelectParts;
	}

	tmpSelectParts = nullptr;

	RefreshPartsList();

	partsSelectFlg = false;

	if (partsPanel->partsPath == L"")
	{
		selectPartsList->SetDrawPosition(0);
		return;
	}

	parameterList->SetNextParts(device, selectParts);


}

void EditFrame::UpdateSelectPartsSetter()
{
	if (!partsSelectFlg)return;

	auto&& partsPanel = ChPtr::SharedSafeCast<SelectPartsListItem>(selectPartsList->GetSelectItem(selectPartsList->GetNowSelect()));

	if (partsPanel == nullptr)return;

	if (partsPanel->partsPath == nowChangeTargetPartsName)return;
	nowChangeTargetPartsName = partsPanel->partsPath;

	if (nowChangeTargetPartsParent != nullptr)
	{
		nowChangeTargetPartsParent->RemoveChildObject(changePartsPosName);

		if (nowChangeTargetPartsName != L"")
			tmpSelectParts = MechaParts::LoadParts(*editMecha, &meshDrawer, nullptr, nowChangeTargetPartsName, changePartsPosName, nowChangeTargetPartsParent);
		else
			tmpSelectParts = MechaParts::LoadParts(*editMecha, &meshDrawer, nullptr, beforePartsJson, changePartsPosName, nowChangeTargetPartsParent);

		parameterList->RefreshMechaParameter(editMecha);
		return;
	}

	editMecha->RemoveCore();

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>> json = nullptr;

	if (nowChangeTargetPartsName != L"")
		json = BaseMecha::CreateBaseMechaData(nowChangeTargetPartsName);
	else
		json = BaseMecha::CreateBaseMechaCoreData(beforePartsJson);

	editMecha->LoadCore(json);

	parameterList->RefreshMechaParameter(editMecha);


}

void EditFrame::UpdateButtonSelect()
{
	UpdateButtonCancel();

	auto&& device = AppIns().GetDirect3D11().GetDevice();

	auto&& partsPanel = ChPtr::SharedSafeCast<EditListPartsItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

	if (partsPanel == nullptr)return;

	partsList->SetDrawPosition(0);
	selectParts = partsPanel->targetParts;

	parameterList->SetBaseParts(device, selectParts);

	partsList->ClearItem();
	SetPartsList(selectParts.get());
}

void EditFrame::UpdateButtonChange()
{
	OpenPartsSelectList();
	UpdateButtonCancel();
}

void EditFrame::UpdateButtonRemove()
{
	UpdateButtonCancel();

	auto&& device = AppIns().GetDirect3D11().GetDevice();

	partsList->SetDrawPosition(0);
	
	if (selectParts == nullptr)
	{
		SetPartsList(nullptr);
		return;
	}

	if (selectParts->GetParent() == nullptr)
	{
		SetPartsList(selectParts.get());
		return;
	}

	selectParts = ChPtr::SharedSafeCast<MechaPartsObject>(selectParts->GetParent());

	parameterList->SetBaseParts(device, selectParts);

	partsList->ClearItem();
	SetPartsList(selectParts == nullptr ? nullptr : selectParts.get());
}

void EditFrame::UpdateButtonCancel()
{
	editControlButtons->ClearItem();
	editControlButtons->SetDrawCount(0);
}

void EditFrame::DrawFunction()
{
	auto&& dc = AppIns().GetDirect3D11().GetDC();

	rtView.SetBackColor(dc, ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	dsView.ClearDepthBuffer(dc);

	AppIns().GetDirect3D11().DrawStart();

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

	DrawNowLoading();

	loadDisplay->Draw(spriteShader);

	spriteShader.DrawEnd();

	AppIns().GetDirect3D11().DrawEnd(rtView);
}

void EditFrame::DrawNowLoading()
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

	partsSelectFlg ?
		selectPartsList->Draw(spriteShader) :
		partsList->Draw(spriteShader);

	if (editControlButtons->GetCount() > 0)
		editControlButtons->Draw(spriteShader);

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

	res->CreateColorTexture(AppIns().GetDirect3D11().GetDevice(), _drawer.bitmap.GetBitmap());

	return res;
}

void EditFrame::Load()
{

	for (auto&& file : std::filesystem::directory_iterator(PARTS_DIRECTORY(L"")))
	{
		std::wstring path = file.path().c_str();

		size_t len = path.rfind(L".");

		if (len == std::wstring::npos)continue;

		std::wstring extensionName = path.substr(len);

 		pathList.push_back(path);
	}

}

bool EditFrame::LoadPart()
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();

	if (pathList.size() <= loadCount)return true;

	auto&& parts = MechaParts::LoadParts(*editMecha, &meshDrawer, nullptr, pathList[loadCount]);

	parts->GetBaseObject()->SetParameters(*parts);

	{

		auto&& panel = ChPtr::Make_S<SelectPartsListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(parts->GetPartsName());
		panel->partsPath = pathList[loadCount];
		selectPartsList->AddItem(panel);

	}

	loadCount++;

	parts = nullptr;

	if (pathList.size() > loadCount)return false;

	editMecha->Create(ChVec2(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), meshDrawer, nullptr);
	editMecha->Load(PLAYER_USE_MECHA_PATH);

	selectParts = editMecha->GetCoreParts();

	parameterList->Init(device, editMecha);

	SetPartsList(selectParts.get());

	pathList.clear();

	return true;

}