#pragma once

#include"../Frames/MenuBase.h"

class BaseMecha;

struct LoaderPanel
{
	ChPtr::Shared<BaseMecha>mecha = nullptr;
	ChD3D11::RenderTarget11 mechaTexture;
	ChD3D11::Texture11 panelName;
	std::string path = "";
};

class LoadDisplay :public MenuBase
{

public:

	void Init(ID3D11Device* _device, ChD3D::XInputController* _controller);

	void Release();

public:

	bool Update();

	void UpdateAction(ActionType _type);

	void UpdateMouse()override;

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader);

public:

	inline bool IsOpenLoader() { return openFlg; }

public:

	void Open(ID3D11DeviceContext* _dc, bool _releaseFlg);

	void Close();

private:

	void Load();

private:

	enum class SelectSpriteType
	{
		LeftButton,
		RightButton,
		LoadPanel
	};

	enum class BottomButtonType
	{
		Decision,
		Cancel,
		None,
	};

	enum class SelectButtonType
	{
		Left, Right, None
	};

	struct LoadPanelSprite
	{
		ChD3D11::Sprite11 backGround;
		ChD3D11::Sprite11 mechaPreview;
		ChD3D11::Sprite11 mechaName;
	};

private:

	static constexpr unsigned long PANEL_DRAW_COUNT = 3;

	bool openFlg = false;

	std::vector<ChPtr::Shared<LoaderPanel>>loadMechaList;
	SelectSpriteType selectSpriteType = SelectSpriteType::LoadPanel;
	unsigned long drawNowSelect = 0;
	unsigned long selectPanelNo = 0;

	ImageSprite window;
	ImageSprite mainWindow;

	LoadPanelSprite loadPanelSpriteList[PANEL_DRAW_COUNT];
	ChD3D11::Texture11 loadPanelImage;
	ChD3D11::Texture11 selectLoadPanelImage;

	TextDrawerWICBitmap textDrawer;

	ImageSprite panelSelectButton[ChStd::EnumCast(SelectButtonType::None)];
	ChD3D11::Texture11 selectPanelSelectButtonImage;
	SelectButtonType selectButton = SelectButtonType::None;

	ImageSprite bottomButton[ChStd::EnumCast(BottomButtonType::None)];
	ChD3D11::Texture11 selectBottomButtonImage;
	unsigned long selectBottomButton = ChStd::EnumCast(BottomButtonType::None);

	ChVec4 backColor = ChVec4::FromColor(0.2f, 0.2f, 0.5f, 1.0f);
	ID3D11Device* device = nullptr;
	ChD3D11::DepthStencilTexture11 dsTex;

	ChD3D11::Shader::BaseDrawMesh11 meshDrawer;
	ChD3D11::CB::CBLight11 light;
};