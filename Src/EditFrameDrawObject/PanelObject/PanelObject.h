#pragma once

class PanelObject
{
public:

	void Init(ChD3D11::Texture11* _texture,const ChVec2& _size, float _padding, std::function<void()>& _pushAction);

private:

	std::function<void()>pushAction;
	ChD3D11::Texture11 texture;
	ChVec2 size = ChVec2();
	float padding = 0.0f;
};