#pragma once

class PanelList;

class PanelListObject
{
public:

	void Init(ID3D11Device* _device);

public:

	inline void SetTitlePanel(ChD3D11::Texture11* _titlePanel, const ChVec4& _titleAnkor)
	{
		if (ChPtr::NullCheck(_titlePanel))return;
		titlePanel = _titlePanel;
	}

	inline void SetPanelListBackGround(ChD3D11::Texture11* _texture, const ChVec4& _backGroundAnkor)
	{
		if (ChPtr::NullCheck(_texture))return;
		panelListBackGround = _texture;
	}

	inline void SetPanelBaseSize(const ChVec2& _size) { panelSize = _size; }

	inline void SetPanelBasePadding(float _padding) { padding = _padding; }

	inline void SetHiddenFlg(bool _flg) { drawFlg = !_flg; }

public:

	void AddPanel(ChD3D11::Texture11* _texture, std::function<void()>& _pushAction);

	void AddPanel(ChD3D11::Texture11* _texture, const ChVec2& _size, std::function<void()>& _pushAction);

	void AddPanel(ChD3D11::Texture11* _texture, float _padding, std::function<void()>& _pushAction);

	void AddPanel(ChD3D11::Texture11* _texture, const ChVec2& _size, float _padding, std::function<void()>& _pushAction);

public:

	void Draw(ID3D11DeviceContext* _dc);

private:

	bool drawFlg = false;

	ChD3D11::Texture11* titlePanel = nullptr;

	ChD3D11::Sprite11 titlePanelSprite;

	std::vector<ChPtr::Shared<PanelObject>>panelList;
	ChD3D11::Sprite11 panelSprite;
	ChVec2 panelSize;
	float padding = 0.0f;

	ChD3D11::Texture11* panelListBackGround;
	ChD3D11::Sprite11 oanelListBackGroundSprite;
};