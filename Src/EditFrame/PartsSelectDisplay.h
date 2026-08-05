#pragma once

#include"../Frames/EditFrame.h"

class EditList;
class EditListItem;

class EditControlList;
class EditControlListItem;

class PartsSelectDisplay : public EditFrame::EditFrameDisplayBase
{
public:

	enum class EditControlType
	{
		Select,
		Change,
		Remove,
		Cancel,
		SetWeapon,
		None
	};

public:

	void Init(EditFrame* _frame)override;

	void Update(MenuBase::ActionType _type)override;

	void UpdateMouse()override;

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)override;

private:

	void CreateEditControlButtonItems();

public:

	inline ChPtr::Shared<MechaPartsObject> GetEditParts() { return editSelectParts; }

	inline ChPtr::Shared<MechaPartsObject> GetSelectParts() { return selectParts; }

public:

	void InitPartsList(ChPtr::Shared<MechaPartsObject> _parts);

	void SetPartsList(ChPtr::Shared<MechaPartsObject> _parts);

private:

	void SetPanelItem(ChPtr::Shared<EditListItem>& _res, ChPtr::Shared<MechaPartsObject>& _parts, const std::wstring& _positionName);
	
	void SetPanelPartsItem(ChPtr::Shared<EditListItem>& _res, ChPtr::Shared<MechaPartsObject>& _parts, const std::wstring& _positionName);

	bool UpdatePanelList(MenuBase::ActionType _type);

	void UpdateDecision(MenuBase::ActionType _type);

	void UpdateCancel(MenuBase::ActionType _type);

private:

	bool UpdateEditButton(MenuBase::ActionType _type);

	void UpdateButtonSelect();

	void UpdateButtonChange();

	void UpdateButtonRemove();

	void UpdateButtonSetWeapon();

	void UpdateButtonCancel();

private:

	ChPtr::Shared<EditList>partsList = nullptr;
	ChPtr::Shared<EditControlList>editControlButtons = nullptr;
	bool useSelectButtonFlg = false;

	ChPtr::Shared<MechaPartsObject>selectParts = nullptr;
	ChPtr::Shared<MechaPartsObject>editSelectParts = nullptr;

	ChPtr::Shared<EditControlListItem>editControllButtonItems[ChStd::EnumCast(EditControlType::None)];

};
