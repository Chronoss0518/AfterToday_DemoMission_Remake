#pragma once

#include"../Frames/MenuBase.h"

class SelectListItemBase
{
public:

	virtual ~SelectListItemBase(){}
};

enum class MoveDiraction
{
	//êÇíº//
	Vertical,
	//êÖïΩ//
	Horizontal
};

class SelectListBase
{
public:

	void Init();

	void Release();

public:

	void SetStartPosition(float _x, float _y);

	void SetStartPosition(const ChVec2& _position);

	void SetPanelSize(float _x, float _y);

	void SetPanelSize(const ChVec2& _size);
	
	//StartPositionÇ©ÇÁÇÃëäëŒà íu//
	void SetAlighSize(float _x, float _y);

	//StartPositionÇ©ÇÁÇÃëäëŒà íu//
	void SetAlighSize(const ChVec2& _size);

	inline void SetMoveDiraction(MoveDiraction _dir) { moveDirection = _dir; }

	inline void SetDrawCount(unsigned long _drawCount) { drawCount = _drawCount; }

public:

	unsigned long GetNowSelect() { return nowSelectPanel; }

	inline ChPtr::Shared<SelectListItemBase> GetSelectItem(unsigned long _num)
	{
		if (itemList.size() <= _num)return nullptr;
		return itemList[_num];
	}

public:

	bool UpdateMouse();

	void UpdateAction(MenuBase::ActionType _type);

private:

	void UpdateActionVertical(MenuBase::ActionType _type);

	void UpdateActionHorizontal(MenuBase::ActionType _type);

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer);

	virtual void DrawPanel(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect,ChPtr::Shared<SelectListItemBase> _drawItem, bool _isSelectPanel) = 0;

public:

	void AddItem(ChPtr::Shared<SelectListItemBase> _item);

	void RemoveItem(ChPtr::Shared<SelectListItemBase> _item);

	void RemoveItem(unsigned long _num);

	void ClearItem();

	inline unsigned long ItemCount() { return itemList.size(); }

private:

	ChVec4 GetStartRect();

	ChVec4 GetNextRect(const ChVec4& _rect);

	ChVec4 UpdateRightBottomInRect(const ChVec4& _rect);

private:

	void MoveAdd();

	void MoveSub();

private:

	unsigned long drawCount = 0;
	unsigned long drawPos = 0;
	unsigned long nowSelectPanel = 0;

	ChVec2 startPosition;
	ChVec2 basePanelSize;
	ChVec2 alighSize;

	MoveDiraction moveDirection = MoveDiraction::Vertical;

	std::vector<ChPtr::Shared<SelectListItemBase>>itemList;
};