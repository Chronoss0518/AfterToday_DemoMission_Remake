#include"../BaseIncluder.h"

#include"../Frames/EditFrame.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../Attack/Attack.h"

#include"../SelectList/SelectList.h"

#include"PartsChangeDisplay.h"

#include"../Application/Application.h"

#define TEXT_ALIGN 8.0f

#define PANEL_COUNT 4

#define PANEL_TITLE_Y 26.0f
#define PANEL_TITLE_HEIGHT 45.0f
#define PANEL_TITLE_TEXT_SIZE 24.0

#define PANEL_SIZE_W 280.0f
#define PANEL_SIZE_H 102.0f

#define PARTS_PANEL_LIST_X 30.0f
#define PARTS_PANEL_LIST_Y 141.0f

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
		_drawer.Draw(*texture, _sprite);
	}

	ChPtr::Shared<ChD3D11::Texture11> texture = nullptr;
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
			_drawer.Draw(selectImage, sprite, ACTIVE_COLOR);
		}

		item->Draw(_drawer, _rect, sprite);

	}

private:

	ChD3D11::Texture11 selectImage;
	ChD3D11::Texture11 background;
	ChD3D11::Sprite11 sprite;
};

void PartsChangeDisplay::Init(EditFrame* _frame)
{
	EditFrameDisplayBase::Init(_frame);

	auto device = AppIns().GetDirect3D11().GetDevice();

	selectPartsList = ChPtr::Make_S<SelectPartsList>();
	selectPartsList->Init();

	cancelPanel = ChPtr::Make_S<SelectPartsListItem>();

	cancelPanel->texture = CreatePanelTitleTexture(L"Cancel");
	cancelPanel->partsPath = L"";

	backGroundTexture.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PanelList.png"), device);
	backgroundSprite.Init();
	backgroundSprite.SetInitPosition();

	auto rect = ChVec4::FromRect(PARTS_PANEL_LIST_X, PARTS_PANEL_LIST_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, PARTS_PANEL_LIST_Y + PANEL_SIZE_H * PANEL_COUNT);
	backgroundSprite.SetPosRect(RectToGameWindow(rect));

}

void PartsChangeDisplay::Update(MenuBase::ActionType _type)
{
	size_t nowSelect = selectPartsList->GetNowSelect();

	selectPartsList->UpdateAction(_type);

	if (_type == MenuBase::ActionType::Decision)
	{
		CloseChangeParts();
		return;
	}

	if (_type == MenuBase::ActionType::Cancel)
	{
		ResetBeforeParts();
		CloseChangeParts();
		return;
	}

	if (nowSelect == selectPartsList->GetNowSelect())return;

	UpdateCoreParts();

	UpdateNotCoreParts();
}

void PartsChangeDisplay::UpdateCoreParts()
{
	if (nowChangeTargetPartsParent != nullptr)return;

	auto mecha = GetEditMecha();

	if (mecha == nullptr)return;

	auto panel = ChPtr::SharedSafeCast<SelectPartsListItem>(selectPartsList->GetSelectItem(selectPartsList->GetNowSelect()));

	auto tmpJson = BaseMecha::CreateBaseMechaCoreData(beforePartsJson);

	if (panel->partsPath != L"")
	{
		tmpJson = BaseMecha::CreateBaseMechaData(panel->partsPath);
	}

	mecha->LoadCore(tmpJson);

	resultParts = mecha->GetCoreParts();

	RefreshMechaParameter();
}

void PartsChangeDisplay::UpdateNotCoreParts()
{
	if (nowChangeTargetPartsParent == nullptr)return;

	auto mecha = GetEditMecha();

	if (mecha == nullptr)return;

	nowChangeTargetPartsParent->RemoveChildObject(nowChangeTargetPartsPosName);

	auto panel = ChPtr::SharedSafeCast<SelectPartsListItem>(selectPartsList->GetSelectItem(selectPartsList->GetNowSelect()));

	auto&& meshDrawer = GetMeshDrawer();

	if (panel->partsPath != L"")
		resultParts = MechaParts::LoadParts(*mecha, &meshDrawer, nullptr, panel->partsPath, nowChangeTargetPartsPosName, nowChangeTargetPartsParent);
	else
		resultParts = MechaParts::LoadParts(*mecha, &meshDrawer, nullptr, beforePartsJson, nowChangeTargetPartsPosName, nowChangeTargetPartsParent);

	RefreshMechaParameter();
}

void PartsChangeDisplay::UpdateMouse()
{
	size_t nowSelect = selectPartsList->GetNowSelect();

	selectPartsList->UpdateMouse();

	if (nowSelect == selectPartsList->GetNowSelect())return;

	UpdateCoreParts();

	UpdateNotCoreParts();
}

void PartsChangeDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)
{
	_spriteShader.Draw(backGroundTexture, backgroundSprite);

	selectPartsList->Draw(_spriteShader);
}

void PartsChangeDisplay::AddMechaParts(ChPtr::Shared<MechaPartsObject> _parts)
{
	auto panel = ChPtr::Make_S<SelectPartsListItem>();

	panel->texture = CreatePanelTitleTexture(_parts->GetPartsName());
	panel->partsPath = _parts->GetBaseObject()->GetThisFilePath();

	partsPanelList.push_back(panel);
}

void PartsChangeDisplay::InitChangeData(ChPtr::Shared<MechaPartsObject>_targetParts)
{
	selectPartsList->AddItem(cancelPanel);

	for (size_t i = 0; i < partsPanelList.size(); i++)
	{
		selectPartsList->AddItem(partsPanelList[i]);
	}

	if (_targetParts == nullptr)
	{
		nowChangeTargetPartsPosName = LOAD_JSON_CORE_PARAM_NAME;
		return;
	}

	beforePartsJson = _targetParts->Serialize();

	nowChangeTargetPartsParent = ChPtr::SharedSafeCast<MechaPartsObject>(_targetParts->GetParent());

	if (nowChangeTargetPartsParent == nullptr)
	{
		nowChangeTargetPartsPosName = LOAD_JSON_CORE_PARAM_NAME;
		return;
	}

	nowChangeTargetPartsPosName = _targetParts->GetPartsPosName();

}


void PartsChangeDisplay::InitChangeData(ChPtr::Shared<MechaPartsObject>_parentParts, const std::wstring& _partsPos)
{
	selectPartsList->AddItem(cancelPanel);

	for (size_t i = 0; i < partsPanelList.size(); i++)
	{
		selectPartsList->AddItem(partsPanelList[i]);
	}

	beforePartsJson = nullptr;

	nowChangeTargetPartsParent = _parentParts;

	if (nowChangeTargetPartsParent == nullptr)
	{
		nowChangeTargetPartsPosName = LOAD_JSON_CORE_PARAM_NAME;
		return;
	}

	nowChangeTargetPartsPosName = _partsPos;

}

void PartsChangeDisplay::Close()
{
	selectPartsList->ClearItem();
	resultParts = nullptr;
	nowChangeTargetPartsParent = nullptr;
	nowChangeTargetPartsPosName = L"";
}

void PartsChangeDisplay::ResetBeforeParts()
{
	if (nowChangeTargetPartsParent == nullptr)return;

	auto mecha = GetEditMecha();

	if (mecha == nullptr)return;

	nowChangeTargetPartsParent->RemoveChildObject(nowChangeTargetPartsPosName);

	if (nowChangeTargetPartsParent != nullptr)
	{
		auto&& meshDrawer = GetMeshDrawer();
		resultParts = MechaParts::LoadParts(*mecha, &meshDrawer, nullptr, beforePartsJson, nowChangeTargetPartsPosName, nowChangeTargetPartsParent);
		RefreshMechaParameter();
		return;
	}

	mecha->LoadCore(BaseMecha::CreateBaseMechaCoreData(beforePartsJson));

	resultParts = mecha->GetCoreParts();

	RefreshMechaParameter();
}