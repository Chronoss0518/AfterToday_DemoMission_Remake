#include"../BaseIncluder.h"

#include"../Frames/EditFrame.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../Attack/Attack.h"

#include"../SelectList/SelectList.h"

#include"PartsSelectDisplay.h"

#include"../Application/Application.h"

#define PANEL_COUNT 4

#define TEXT_ALIGN 8.0f

#define PANEL_TITLE_Y 26.0f
#define PANEL_TITLE_HEIGHT 45.0f

#define PANEL_POS_TITLE_Y 17.0f
#define PANEL_POS_TITLE_HEIGHT 25.0f

#define PANEL_POS_PARTS_Y 42.0f
#define PANEL_POS_PARTS_HEIGHT 45.0f

#define PANEL_SIZE_W 280.0f
#define PANEL_SIZE_H 102.0f

#define PARTS_PANEL_LIST_X 30.0f
#define PARTS_PANEL_LIST_Y 141.0f


#define EDIT_CONTROL_BUTTON_PARTS_PANEL_LIST_X 360.0f
#define EDIT_CONTROL_BUTTON_PARTS_PANEL_LIST_Y 186.0f

#define EDIT_CONTROL_BUTTON_PANEL_SIZE_W 183.0f
#define EDIT_CONTROL_BUTTON_PANEL_SIZE_H 80.0f

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

	void SetActiveFlg(bool _active) { notActiveFlg = !_active; }

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
			_drawer.Draw(selectImage, sprite,notActiveFlg ? NOT_ACTIVE_COLOR : ACTIVE_COLOR);
		}

		item->Draw(_drawer, _rect, sprite);

	}

private:

	bool notActiveFlg = false;
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
	PartsSelectDisplay::EditControlType type = PartsSelectDisplay::EditControlType::None;
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
			_drawer.Draw(selectImage, sprite, ACTIVE_COLOR);
		}

		item->Draw(_drawer, _rect, sprite);

	}

private:

	ChD3D11::Texture11 selectImage;
	ChD3D11::Texture11 background;
	ChD3D11::Sprite11 sprite;
};

void PartsSelectDisplay::Init(EditFrame* _frame)
{
	EditFrame::EditFrameDisplayBase::Init(_frame);

	auto device = AppIns().GetDirect3D11().GetDevice();

	partsList = ChPtr::Make_S<EditList>();
	partsList->Init();

	backPanel = ChPtr::Make_S<EditListItem>();
	backPanel->positionNameTexture = CreatePanelTitleTexture(L"Back");

	selectPartsTexture = CreatePanelPosTitleTexture(L"Select Parts");

	editControlButtons = ChPtr::Make_S<EditControlList>();
	editControlButtons->Init();


	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Select");
		panel->type = EditControlType::Select;

		editControllButtonItems[ChStd::EnumCast(panel->type)] = panel;
	}
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Change");
		panel->type = EditControlType::Change;

		editControllButtonItems[ChStd::EnumCast(panel->type)] = panel;
	}
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Remove");
		panel->type = EditControlType::Remove;

		editControllButtonItems[ChStd::EnumCast(panel->type)] = panel;
	}
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"SetWeapon");
		panel->type = EditControlType::SetWeapon;

		editControllButtonItems[ChStd::EnumCast(panel->type)] = panel;
	}
	{
		auto&& panel = ChPtr::Make_S<EditControlListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Cancel");
		panel->type = EditControlType::Cancel;

		editControllButtonItems[ChStd::EnumCast(panel->type)] = panel;
	}

	backGroundTexture.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PanelList.png"), device);
	backgroundSprite.Init();
	backgroundSprite.SetInitPosition();

	auto rect = ChVec4::FromRect(PARTS_PANEL_LIST_X, PARTS_PANEL_LIST_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, PARTS_PANEL_LIST_Y + PANEL_SIZE_H * PANEL_COUNT);
	backgroundSprite.SetPosRect(RectToGameWindow(rect));

}

void PartsSelectDisplay::Update(MenuBase::ActionType _type)
{
	if(UpdateEditButton(_type))return;

	if(UpdatePanelList(_type))return;
}

void PartsSelectDisplay::UpdateMouse()
{
	if (editControlButtons->GetCount() <= 0)
	{
		partsList->UpdateMouse();
	}
	else
	{
		editControlButtons->UpdateMouse();
	}
}

void PartsSelectDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)
{
	_spriteShader.Draw(backGroundTexture, backgroundSprite);

	partsList->SetActiveFlg(editControlButtons->GetCount() <= 0);

	partsList->Draw(_spriteShader);

	if (editControlButtons->GetCount() > 0)
		editControlButtons->Draw(_spriteShader);
}

void PartsSelectDisplay::InitPartsList(ChPtr::Shared<MechaPartsObject> _parts)
{
	if(!useSelectButtonFlg)selectParts = _parts;

	SetPartsList(selectParts);

	partsList->SetDrawPosition(0);
	partsList->SetNowSelect(0);
}

void PartsSelectDisplay::SetPartsList(ChPtr::Shared<MechaPartsObject> _parts)
{
	partsList->ClearItem();

	partsList->AddItem(backPanel);

	if (_parts == nullptr)
	{
		auto&& panel = ChPtr::Make_S<EditListItem>();
		panel->positionNameTexture = CreatePanelTitleTexture(L"+ " LOAD_JSON_CORE_PARAM_NAME);
		panel->partsPosName = LOAD_JSON_CORE_PARAM_NAME;

		partsList->AddItem(panel);
		return;
	}

	auto&& base = _parts->GetBaseObject();

	auto&& selectPartsPanel = ChPtr::Make_S<EditListPartsItem>();

	selectPartsPanel->positionNameTexture = selectPartsTexture;
	selectPartsPanel->partsNameTexture = CreatePanelPosPartsTexture(base->GetMyName());
	selectPartsPanel->partsPosName = _parts->GetPartsPosName();
	selectPartsPanel->targetParts = _parts;

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

void PartsSelectDisplay::SetPanelItem(ChPtr::Shared<EditListItem>& _res, ChPtr::Shared<MechaPartsObject>& _parts, const std::wstring& _positionName)
{
	if (_parts != nullptr)return;
	auto&& res = ChPtr::Make_S<EditListItem>();

	res->positionNameTexture = CreatePanelTitleTexture(L"+ " + _positionName);
	res->partsPosName = _positionName;

	_res = res;
}

void PartsSelectDisplay::SetPanelPartsItem(ChPtr::Shared<EditListItem>& _res, ChPtr::Shared<MechaPartsObject>& _parts, const std::wstring& _positionName)
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

bool PartsSelectDisplay::UpdatePanelList(MenuBase::ActionType _type)
{
	if (editControlButtons->GetCount() > 0)return false;

	partsList->UpdateAction(_type);

	if (!IsNotSelectButtons())return true;

	UpdateCancel(_type);

	UpdateDecision(_type);

	return true;
}

void PartsSelectDisplay::UpdateDecision(MenuBase::ActionType _type)
{
	if (_type != MenuBase::ActionType::Decision)return;

	auto&& partsPanel = ChPtr::SharedSafeCast<EditListPartsItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

	if (partsPanel == nullptr)
	{
		auto panel = ChPtr::SharedSafeCast<EditListItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

		if (panel->partsPosName == L"")
		{
			AddActionType(MenuBase::ActionType::Cancel);

			return;
		}

		targetPartsPosName = panel->partsPosName;
		useSelectButtonFlg = selectParts != nullptr;

		editSelectParts = nullptr;
		OpenChangeParts();

		return;
	}

	CreateEditControlButtonItems();

}

void PartsSelectDisplay::UpdateCancel(MenuBase::ActionType _type)
{
	if (_type != MenuBase::ActionType::Cancel)return;

	if (selectParts == nullptr)
	{
		ReturnFrame();
		return;
	}

	if (selectParts->GetParent() == nullptr)
	{
		ReturnFrame();
		return;
	}

	partsList->SetDrawPosition(0);

	selectParts = ChPtr::SharedSafeCast<MechaPartsObject>(selectParts->GetParent());
	SetPartsList(selectParts);
	
	SetBaseParts(selectParts);

}

void PartsSelectDisplay::CreateEditControlButtonItems()
{
	bool weaponFlg = false;

	SetEditParts();

	if (!editSelectParts->GetWeaponFunctions().empty())
		weaponFlg = true;

	editControlButtons->ClearItem();

	for (unsigned char i = 0; i < ChStd::EnumCast(EditControlType::None); i++)
	{
		if ((EditControlType)(i) == EditControlType::Select && !useSelectButtonFlg)continue;
		if ((EditControlType)(i) == EditControlType::SetWeapon && !weaponFlg)continue;

		editControlButtons->AddItem(editControllButtonItems[i]);
	}

	editControlButtons->SetDrawCount(editControlButtons->GetCount());

}

void PartsSelectDisplay::SetEditParts()
{
	useSelectButtonFlg = false;

	auto&& partsPanel = ChPtr::SharedSafeCast<EditListPartsItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

	editSelectParts = selectParts;

	if (partsPanel != nullptr &&
		partsPanel->targetParts != nullptr)
	{
		useSelectButtonFlg = selectParts.get() != partsPanel->targetParts.get();
		editSelectParts = partsPanel->targetParts;
		return;
	}

	auto panel = ChPtr::SharedSafeCast<EditListItem>(partsList->GetSelectItem(partsList->GetNowSelect()));

	if (panel->partsPosName == LOAD_JSON_CORE_PARAM_NAME)
	{
		editSelectParts = nullptr;
	}

}

bool PartsSelectDisplay::UpdateEditButton(MenuBase::ActionType _type)
{
	if (editControlButtons->GetCount() <= 0)return false;

	editControlButtons->UpdateAction(_type);

	if (_type != MenuBase::ActionType::Decision)return true;

	auto buttonItem = ChPtr::SharedSafeCast<EditControlListItem>(editControlButtons->GetSelectItem(editControlButtons->GetNowSelect()));

	if (buttonItem->type == EditControlType::Select)
		UpdateButtonSelect();

	if (buttonItem->type == EditControlType::Change)
		UpdateButtonChange();

	if (buttonItem->type == EditControlType::Remove)
	{
		UpdateButtonRemove();
	}

	if (buttonItem->type == EditControlType::SetWeapon)
		UpdateButtonSetWeapon();

	if (buttonItem->type == EditControlType::Cancel)
		UpdateButtonCancel();

	return true;
}

void PartsSelectDisplay::UpdateButtonSelect()
{
	UpdateButtonCancel();

	auto&& device = AppIns().GetDirect3D11().GetDevice();

	selectParts = editSelectParts;

	SetBaseParts(selectParts);

	SetPartsList(selectParts);
	partsList->SetDrawPosition(0);
	partsList->SetNowSelect(0);
}

void PartsSelectDisplay::UpdateButtonChange()
{
	UpdateButtonCancel();



	OpenChangeParts();
}

void PartsSelectDisplay::UpdateButtonRemove()
{
	UpdateButtonCancel();

	if (editSelectParts != nullptr)
	{
		selectParts = ChPtr::SharedSafeCast<MechaPartsObject>(editSelectParts->GetParent());
		if(selectParts != nullptr)selectParts->RemoveChildObject(editSelectParts->GetPartsPosName());
		editSelectParts->Destroy();
		editSelectParts = nullptr;
	}

	auto&& device = AppIns().GetDirect3D11().GetDevice();

	partsList->SetDrawPosition(0);

	if (selectParts == nullptr)
	{
		GetEditMecha()->RemoveCore();
		SetPartsList(nullptr);
		return;
	}

	SetBaseParts(selectParts);

	partsList->ClearItem();
	SetPartsList(selectParts);
}

void PartsSelectDisplay::UpdateButtonSetWeapon()
{
	OpenSetWeapon();
}

void PartsSelectDisplay::UpdateButtonCancel()
{
	editControlButtons->ClearItem();
	editControlButtons->SetDrawCount(0);

}
