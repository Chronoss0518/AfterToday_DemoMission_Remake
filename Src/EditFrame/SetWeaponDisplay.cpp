#include"../BaseIncluder.h"

#include"../Frames/EditFrame.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../Attack/Attack.h"
#include"../WeaponPaletteUI/WeaponPaletteEditUI.h"

#include"../SelectList/SelectList.h"

#include"SetWeaponDisplay.h"

#include"../Application/Application.h"

#define TEXT_ALIGN 8.0f

#define PANEL_COUNT 3

#define PANEL_TITLE_Y 26.0f
#define PANEL_TITLE_HEIGHT 45.0f
#define PANEL_TITLE_HEIGHT 45.0f
#define PANEL_TITLE_TEXT_SIZE 24.0

#define PANEL_SIZE_W 280.0f
#define PANEL_SIZE_H 102.0f

#define PARTS_PANEL_LIST_X 30.0f
#define PARTS_PANEL_LIST_Y 141.0f

class AttackListItem : public SelectListItemBase
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
	ChPtr::Shared<MechaPartsObject>targetPartsObject = nullptr;
	ChPtr::Shared<WeaponFunction>targetWeaponFunction = nullptr;
};

class AttackList :public SelectListBase
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
		auto&& item = ChPtr::SharedSafeCast<AttackListItem>(_drawItem);
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

void SetWeaponDisplay::Init(EditFrame* _frame)
{
	EditFrameDisplayBase::Init(_frame);

	auto device = AppIns().GetDirect3D11().GetDevice();

	attackList = ChPtr::Make_S<AttackList>();
	attackList->Init();

	cancelPanel = ChPtr::Make_S<AttackListItem>();
	cancelPanel->texture = CreatePanelTitleTexture(L"Cancel");

	backGroundTexture.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PanelList.png"), device);
	backgroundSprite.Init();
	backgroundSprite.SetInitPosition();

	auto rect = ChVec4::FromRect(PARTS_PANEL_LIST_X, PARTS_PANEL_LIST_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, PARTS_PANEL_LIST_Y + PANEL_SIZE_H * PANEL_COUNT);
	backgroundSprite.SetPosRect(RectToGameWindow(rect));

	weaponPaletteUI = ChPtr::Make_S<WeaponPaletteEditUI>();
	weaponPaletteUI->Init(device);
}

void SetWeaponDisplay::Update(MenuBase::ActionType _type)
{
	size_t nowSelect = attackList->GetNowSelect();

	attackList->UpdateAction(_type);
	weaponPaletteUI->Update(_type);

	if (_type == MenuBase::ActionType::Decision)
	{
		CloseChangeParts();
		return;
	}

	if (_type == MenuBase::ActionType::Cancel)
	{
		CloseChangeParts();
		return;
	}

	if (nowSelect == attackList->GetNowSelect())return;

}

void SetWeaponDisplay::UpdateMouse()
{
	size_t nowSelect = attackList->GetNowSelect();

	attackList->UpdateMouse();
	weaponPaletteUI->UpdateMouse();

	if (nowSelect == attackList->GetNowSelect())return;

}

void SetWeaponDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)
{
	weaponPaletteUI->UpdateSelect(GetEditMecha());

	_spriteShader.Draw(backGroundTexture, backgroundSprite);

	attackList->Draw(_spriteShader);
}

void SetWeaponDisplay::InitAttackList()
{
	weaponPaletteUI->Open();
}

void SetWeaponDisplay::ReleaseAttackList()
{

}