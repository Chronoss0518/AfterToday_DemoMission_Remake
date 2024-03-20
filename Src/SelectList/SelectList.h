#pragma once

class SelectListItemBase
{
public:

	virtual void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer);
};

enum class DrawDiraction
{
	LeftToRight,
	TopToDown,
};

class SelectListBase
{
public:

public:

	inline bool IsOnPanel() { return onPanelFlg; }

public:

	bool Update(ChWin::MouseController& _controller);

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer);

private:

	bool onPanelFlg = false;

	unsigned long drawCount = 0;
	unsigned long drawPos = 0;
	unsigned long nowSelectPanel = 0;

	ChVec2 startPosition;
	ChVec2 basePanel;
	ChVec2 alighSize;

	std::vector<ChPtr::Shared<SelectListItemBase>>itemList;


};