#pragma once

#include"../Frames/MenuBase.h"

class BaseMecha;
class LoadPanelList;

class LoadItem;

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

	void DrawBase(ChD3D11::Shader::BaseDrawSprite11& _spriteShader);

public:

	inline bool IsOpenLoader() { return openFlg; }

public:

	void Open(ID3D11DeviceContext* _dc);

	void Close();

private:

	void Load();

	bool LoadFile(unsigned long _openNumber);

private:

	enum class SelectSpriteType:unsigned char
	{
		LeftButton,
		RightButton,
		LoadPanel
	};

	enum class BottomButtonType :unsigned char
	{
		Decision,
		Cancel,
		None,
	};

	enum class SelectButtonType :unsigned char
	{
		Left, Right, None
	};

private:

	static constexpr unsigned long PANEL_DRAW_COUNT = 3;

	bool openFlg = false;

	SelectSpriteType selectSpriteType = SelectSpriteType::LoadPanel;

	ImageSprite window;
	ImageSprite mainWindow;

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

	ChD3D11::Shader::BaseDrawMesh11<wchar_t> meshDrawer;
	ChD3D11::CB::CBLight11 light;

	ChPtr::Shared<LoadPanelList> selectList = nullptr;

	ChPtr::Shared<ChCpp::JsonArray<wchar_t>> loadFileList;
	ChD3D11::Texture11 progressCircleTexture;
	ChD3D11::Sprite11 progressCircleTexturePosition;
	ChVec4 progressCircleColor = ChVec4();
	float progressCircleRoutate = 0.0f;
	ID3D11DeviceContext* dc = nullptr;

};