#pragma once

class BaseMecha;

struct LoaderPanel
{
	ChPtr::Shared<BaseMecha>mecha = nullptr;
	ChD3D11::RenderTarget11 mechaTexture;
	ChD3D11::Texture11 panelName;
	std::string path = "";
};

class LoadDisplay
{

public:

	void Init(ID3D11Device* _device);

	void Release();

public:

	std::string GetSelectMechaPath();

public:

	bool Update();

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader);

public:

	inline bool IsOpenLoader() { return openFlg; }

public:

	void Open(const std::string& _nowSelectPath, ID3D11DeviceContext* _dc, bool _releaseFlg);

	void Close();

	void Select();

private:

	enum class ActionType
	{
		LeftSelect,
		RightSelect,
		UpSelect,
		DownSelect,
		Decision,
		Cancel
	};

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

	static constexpr int PANEL_DRAW_COUNT = 3;

	bool openFlg = false;
	
	std::vector<ChPtr::Shared<LoaderPanel>>loadMechaList;
	SelectSpriteType selectSpriteType = SelectSpriteType::LoadPanel;
	unsigned long drawNowSelect = 0;
	unsigned long selectPanel = 0;

	ImageSprite window;
	ImageSprite mainWindow;

	LoadPanelSprite loadPanelSpriteList[PANEL_DRAW_COUNT];
	ChD3D11::Texture11 loadPanelImage;
	ChD3D11::Texture11 selectLoadPanelImage;

	ImageSprite panelSelectButton[ChStd::EnumCast(SelectButtonType::None)];
	ChD3D11::Texture11 selectPanelSelectButtonImage;
	SelectButtonType selectButton = SelectButtonType::None;

	ImageSprite bottomButton[ChStd::EnumCast(BottomButtonType::None)];
	ChD3D11::Texture11 selectBottomButtonImage;
	BottomButtonType selectBottomButton = BottomButtonType::None;

	ChVec4 backColor = ChVec4::FromColor(0.2f, 0.2f, 0.5f, 1.0f);
	ID3D11Device* device = nullptr;
	ChD3D11::DepthStencilTexture11 dsTex;

	ChD3D11::Shader::BaseDrawMesh11 meshDrawer;
	ChD3D11::CB::CBLight11 light;

	ChD3D11::Sprite11 testSprite;
};