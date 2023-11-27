#pragma once

class PanelObject
{
public:

	void Init(ChD3D11::Texture11* _texture, const ChVec2& _size, float _padding, std::function<void()>& _pushAction);

	inline void SetText(const std::wstring& _text) { drawText = _text; }

	void Draw3D(ChD3D11::Shader::BaseDrawSprite11& _spriteShader, const ChVec2& _position, ChD3D11::Sprite11& _drawSprite);

	void Draw2D(ChD3D::DirectFontBase& _textDrawer, ChD3D::TextFormatObject& _textFormat, ChD3D::BrushObject& _brush, const ChVec2& _position);

	inline ChVec2 GetSize() { return size; }

	inline float GetPadding() { return padding; }

	bool IsOnPanel(const ChVec2& _drawPosition, const ChVec2& _position);

	void PushAction();

private:

	std::function<void()>pushAction;
	ChD3D11::Texture11* texture;
	std::wstring drawText = L"";
	ChVec2 size = ChVec2();
	float padding = 0.0f;
};