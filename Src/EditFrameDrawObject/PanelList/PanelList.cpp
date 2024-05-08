
#include"../../BaseIncluder.h"

#include"PanelList.h"
#include"../PanelObject/PanelObject.h"


void PanelList::Init(ID3D11Device* _device, ChD3D::DirectFontBase& _textDrawer)
{
	if (ChPtr::NullCheck(_device))return;
	titlePanelSprite.Init();
	panelListBackGroundSprite.Init();

	panelTextFormat = _textDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f);

	panelTextBursh = _textDrawer.CreateBrush(ChVec4(0.0f, 0.0f, 0.0f, 1.0f));

	titleTextFormat = _textDrawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f);

	titleTextBursh = _textDrawer.CreateBrush(ChVec4(0.0f, 0.0f, 0.0f, 1.0f));

}

void PanelList::Draw(ChD3D11::Shader::BaseDrawSprite11& _shader,ChD3D::DirectFontBase& _textDrawer)
{
	if (ChPtr::NullCheck(panelListBackGround))return;
	//if (ChPtr::NullCheck(titlePanel))return;

	_shader.Draw(*panelListBackGround, panelListBackGroundSprite);
	//_shader.Draw(*titlePanel, titlePanelSprite);

	ChVec4 anchor;

	anchor.left = titlePanelSprite.GetPos(ChD3D11::SpritePositionName::LeftTop).x;
	anchor.top = titlePanelSprite.GetPos(ChD3D11::SpritePositionName::LeftTop).y;
	anchor.right = titlePanelSprite.GetPos(ChD3D11::SpritePositionName::RightBottom).x;
	anchor.bottom = titlePanelSprite.GetPos(ChD3D11::SpritePositionName::RightBottom).y;

	//_textDrawer.Draw(titleText, titleTextFormat, titleTextBursh, anchor);

	ChVec2 startPos = titlePanelSprite.GetPos(3);

	for (auto&& panel : panelList)
	{
		panel->Draw3D(_shader, startPos, panelSprite);
		panel->Draw2D(_textDrawer,panelTextFormat, panelTextBursh, startPos);
		startPos.y += panel->GetSize().y + panel->GetPadding();
	}

}

void PanelList::SetTitlePanel(ChD3D11::Texture11* _titlePanel, const ChVec4& _titleAnkor)
{
	if (ChPtr::NullCheck(_titlePanel))return;
	titlePanel = _titlePanel;

	titlePanelSprite.SetInitPosition();

	titlePanelSprite.SetPos(0,ChVec2(_titleAnkor.left, _titleAnkor.top));
	titlePanelSprite.SetPos(1,ChVec2(_titleAnkor.right, _titleAnkor.top));
	titlePanelSprite.SetPos(2,ChVec2(_titleAnkor.right, _titleAnkor.bottom));
	titlePanelSprite.SetPos(3,ChVec2(_titleAnkor.left, _titleAnkor.bottom));

}

void PanelList::SetPanelListBackGround(ChD3D11::Texture11* _texture, const ChVec4& _backGroundAnkor)
{
	if (ChPtr::NullCheck(_texture))return;
	panelListBackGround = _texture;

	panelListBackGroundSprite.SetInitPosition();

	panelListBackGroundSprite.SetPosRect(_backGroundAnkor);
}

void PanelList::AddPanel(ChD3D11::Texture11* _texture, const std::wstring& _text, std::function<void()>& _pushAction)
{
	AddPanel(_texture, _text, panelSize, padding, _pushAction);
}

void PanelList::AddPanel(ChD3D11::Texture11* _texture, const std::wstring& _text, const ChVec2& _size, std::function<void()>& _pushAction)
{
	AddPanel(_texture, _text, _size, padding, _pushAction);
}

void PanelList::AddPanel(ChD3D11::Texture11* _texture, const std::wstring& _text, float _padding, std::function<void()>& _pushAction)
{
	AddPanel(_texture, _text, panelSize, _padding, _pushAction);
}

void PanelList::AddPanel(ChD3D11::Texture11* _texture, const std::wstring& _text, const ChVec2& _size, float _padding, std::function<void()>& _pushAction)
{
	if (ChPtr::NullCheck(_texture))return;
	if (_pushAction == nullptr)return;

	auto panel = ChPtr::Make_S<PanelObject>();
	panel->Init(_texture, _size, _padding, _pushAction);
	panel->SetText(_text);
	panelList.push_back(panel);
}
