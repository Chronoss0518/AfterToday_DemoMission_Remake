#pragma once

class PanelObject;

class PanelList
{
public:

	void Init(ID3D11Device* _device, ChD3D::DirectFontBase& _textDrawer);

public:

	void SetTitlePanel(ChD3D11::Texture11* _titlePanel, const ChVec4& _titleAnkor);

	inline void SetTitleName(const std::wstring& _title) { titleText = _title; }

	void SetPanelListBackGround(ChD3D11::Texture11* _texture, const ChVec4& _backGroundAnkor);

	inline void SetPanelBaseSize(const ChVec2& _size) { panelSize = _size; }

	inline void SetPanelBasePadding(float _padding) { padding = _padding; }

	inline void SetHiddenFlg(bool _flg) { drawFlg = !_flg; }

public:

	void AddPanel(ChD3D11::Texture11* _texture, const std::wstring& _text, std::function<void()>& _pushAction);

	void AddPanel(ChD3D11::Texture11* _texture, const std::wstring& _text, const ChVec2& _size, std::function<void()>& _pushAction);

	void AddPanel(ChD3D11::Texture11* _texture, const std::wstring& _text, float _padding, std::function<void()>& _pushAction);

	void AddPanel(ChD3D11::Texture11* _texture, const std::wstring& _text, const ChVec2& _size, float _padding, std::function<void()>& _pushAction);

public:

	inline ChVec2 GetSize() { return panelSize; }

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _shader, ChD3D::DirectFontBase& _textDrawer);

private:

	bool drawFlg = false;

	ChD3D11::Texture11* titlePanel = nullptr;
	ChD3D11::Sprite11 titlePanelSprite;

	std::wstring titleText = L"";
	ChD3D::TextFormatObject titleTextFormat;
	ChD3D::BrushObject titleTextBursh;

	std::vector<ChPtr::Shared<PanelObject>>panelList;
	ChD3D11::Sprite11 panelSprite;
	ChD3D::TextFormatObject panelTextFormat;
	ChD3D::BrushObject panelTextBursh;
	ChVec2 panelSize;
	float padding = 0.0f;

	ChD3D11::Texture11* panelListBackGround;
	ChD3D11::Sprite11 panelListBackGroundSprite;

};