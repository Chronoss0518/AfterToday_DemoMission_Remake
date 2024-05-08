#include"../BaseIncluder.h"

#include"SelectList.h"


void SelectListBase::Init()
{
	drawPos = 0;
	nowSelectPanel = 0;
}

void SelectListBase::Release()
{
	ClearItem();

	drawPos = 0;
	nowSelectPanel = 0;
}

void SelectListBase::SetStartPosition(float _x, float _y)
{
	SetStartPosition(ChVec2(_x, _y));
}

void SelectListBase::SetStartPosition(const ChVec2& _position)
{
	startPosition = _position;
}

void SelectListBase::SetPanelSize(float _x, float _y)
{
	SetPanelSize(ChVec2(_x, _y));
}

void SelectListBase::SetPanelSize(const ChVec2& _size)
{
	basePanelSize = _size;
}

void SelectListBase::SetAlighSize(float _x, float _y)
{
	SetAlighSize(ChVec2(_x, _y));
}

void SelectListBase::SetAlighSize(const ChVec2& _size)
{
	alighSize = _size;
}

bool SelectListBase::UpdateMouse()
{
	drawPos %= itemList.size();

	ChVec4 rect = GetStartRect();

	for (unsigned long i = drawPos; i < drawCount + drawPos; i++)
	{
		if (IsMoucePosOnRect(RectToGameWindow(rect)))
		{
			nowSelectPanel = i % itemList.size();
			return true;
		}

		rect = GetNextRect(rect);
	}

	return false;
}

void SelectListBase::UpdateAction(MenuBase::ActionType _type)
{
	UpdateActionVertical(_type);

	UpdateActionHorizontal(_type);

	return;
}

void SelectListBase::UpdateActionVertical(MenuBase::ActionType _type)
{
	if (moveDirection != MoveDiraction::Vertical)return;

	if (_type == MenuBase::ActionType::Up)
	{
		MoveSub();
	}

	if (_type == MenuBase::ActionType::Down)
	{
		MoveAdd();
	}
}

void SelectListBase::UpdateActionHorizontal(MenuBase::ActionType _type)
{
	if (moveDirection != MoveDiraction::Horizontal)return;

	if (_type == MenuBase::ActionType::Right)
	{
		MoveAdd();
	}

	if (_type == MenuBase::ActionType::Left)
	{
		MoveSub();
	}
}

void SelectListBase::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)
{
	if (itemList.empty())return;

	drawPos %= itemList.size();

	ChVec4 rect = GetStartRect();

	unsigned long drawUseCount = drawCount;

	if (itemList.size() <= drawUseCount)drawUseCount = itemList.size();

	for (unsigned long i = drawPos;i< drawUseCount + drawPos;i++)
	{
		unsigned long num = i % itemList.size();
		DrawPanel(_drawer, rect, itemList[num],i,nowSelectPanel == num);

		rect = GetNextRect(rect);
	}
}

void SelectListBase::AddItem(ChPtr::Shared<SelectListItemBase> _item)
{
	if (_item == nullptr)return;
	itemList.push_back(_item);
}

void SelectListBase::RemoveItem(ChPtr::Shared<SelectListItemBase> _item)
{
	if (_item == nullptr)return;

	auto&& it = std::find(itemList.begin(), itemList.end(), _item);
	if (it == itemList.end())return;

	itemList.erase(it);
}

void SelectListBase::RemoveItem(unsigned long _num)
{
	if (_num >= itemList.size())return;
	auto&& it = itemList.begin() + _num;
	itemList.erase(it);
}

void SelectListBase::ClearItem()
{
	if (itemList.empty())return;
	itemList.clear();
}

ChVec4 SelectListBase::GetStartRect()
{
	ChVec4 res;

	res.left = startPosition.x;
	res.top = startPosition.y;

	res = UpdateRightBottomInRect(res);

	return res;
}

ChVec4 SelectListBase::GetNextRect(const ChVec4& _rect)
{
	ChVec4 res = _rect;

	res.left += alighSize.x;
	res.top += alighSize.y;

	res = UpdateRightBottomInRect(res);

	return res;
}

ChVec4 SelectListBase::UpdateRightBottomInRect(const ChVec4& _rect)
{

	ChVec4 res = _rect;

	res.right = res.left + basePanelSize.x;
	res.bottom = res.top + basePanelSize.y;

	return res;
}

void SelectListBase::MoveAdd()
{
	if (!loopFlg && drawPos + drawCount == itemList.size())return;

	if (drawCount < itemList.size())
	{
		if (nowSelectPanel == ((drawPos + drawCount - 1) % itemList.size()))
			drawPos = (drawPos + 1) % itemList.size();
	}

	nowSelectPanel = (nowSelectPanel + 1) % itemList.size();
}

void SelectListBase::MoveSub()
{
	if (!loopFlg && 0 == drawPos)return;

	if (drawCount < itemList.size())
	{
		if (nowSelectPanel == drawPos)
			drawPos = (drawPos + itemList.size() - 1) % itemList.size();
	}

	nowSelectPanel = (nowSelectPanel + itemList.size() - 1) % itemList.size();
}
