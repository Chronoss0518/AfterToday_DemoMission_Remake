
#include"../../BaseIncluder.h"

#include"PanelList.h"
#include"../PanelObject/PanelObject.h"


void PanelListObject::Init(ID3D11Device* _device)
{
	if (ChPtr::NullCheck(_device))return;
	titlePanelSprite.Init(_device);
	oanelListBackGroundSprite.Init(_device);
}

void PanelListObject::Draw(ID3D11DeviceContext* _dc)
{

}

void AddPanel(ChD3D11::Texture11* _texture, std::function<void()>& _pushAction)
{

}

void AddPanel(ChD3D11::Texture11* _texture, const ChVec2& _size, std::function<void()>& _pushAction)
{

}

void AddPanel(ChD3D11::Texture11* _texture, float _padding, std::function<void()>& _pushAction)
{

}

void AddPanel(ChD3D11::Texture11* _texture, const ChVec2& _size, float _padding, std::function<void()>& _pushAction)
{

}
