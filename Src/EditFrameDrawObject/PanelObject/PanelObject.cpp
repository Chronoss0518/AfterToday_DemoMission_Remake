#include"../../BaseIncluder.h"
#include"PanelObject.h"


void PanelObject::Init(ChD3D11::Texture11* _texture, const ChVec2& _size, float _padding, std::function<void()>& _pushAction)
{
	if (ChPtr::NullCheck(_texture))return;
	if (_pushAction == nullptr)return;

	texture = _texture;
	size = _size;
	padding = _padding;
	pushAction = _pushAction;
}

void PanelObject::Draw3D(ChD3D11::Shader::BaseDrawSprite11& _spriteShader, const ChVec2& _position, ChD3D11::Sprite11& _drawSprite)
{
	if (!_spriteShader.IsInit())return;
	_drawSprite.SetPos(0, ChVec2(_position.x, _position.y));
	_drawSprite.SetPos(1, ChVec2(_position.x + size.x, _position.y));
	_drawSprite.SetPos(2, ChVec2(_position.x + size.x, _position.y - size.y));
	_drawSprite.SetPos(3, ChVec2(_position.x, _position.y - size.y));

	_spriteShader.Draw(*texture, _drawSprite);

}

void PanelObject::Draw2D(ChD3D::DirectFontBase& _textDrawer, ChD3D::TextFormatObject& _textFormat, ChD3D::BrushObject& _brush, const ChVec2& _position)
{
	ChVec4 anchor;

	anchor.top = _position.y;
	anchor.right = _position.x + size.x;
	anchor.bottom = _position.y - size.y;
	anchor.left = _position.x;

	_textDrawer.DrawToProjection(drawText, _textFormat, _brush, anchor);
}

bool PanelObject::IsOnPanel(const ChVec2& _drawPosition, const ChVec2& _position)
{
	ChVec2 drawPosition = _drawPosition;
	drawPosition.Abs();

	ChVec2 mouseDownPosition = _position;
	mouseDownPosition.Abs();

	if (mouseDownPosition.x < drawPosition.x || mouseDownPosition.x > drawPosition.x + size.x)return false;
	if (mouseDownPosition.y < drawPosition.y || mouseDownPosition.y > drawPosition.y + size.y)return false;

	return true;

}

void PanelObject::PushAction()
{
	if (pushAction == nullptr)return;
	pushAction();
}
