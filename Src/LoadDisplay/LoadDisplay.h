#pragma once

class BaseMecha;

struct LoaderPanel
{
	ChPtr::Shared<BaseMecha>mecha = nullptr;
	ChD3D11::RenderTarget11 mechaTexture;
	ImageSprite panelName;
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

	void Open(ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11& _meshDrawer);

	void Close();

	void Select();

private:

	enum class ActionType
	{
		UpSelect,
		DownSelect,
		Decision,
		Cancel
	};

	enum class SelectSpriteType
	{
		UpButton,
		DownButton,
		LoadPanel
	};

	enum class SelectButtonType
	{
		Up, Down, None
	};


private:

	bool openFlg = false;
	
	std::vector<ChPtr::Shared<LoaderPanel>>loadMechaList;


	SelectSpriteType selectSpriteType = SelectSpriteType::LoadPanel;

	int drawNowSelect = 0;

	std::string loadPath = "";

	ImageSprite stageSelectButton[ChStd::EnumCast(SelectButtonType::None)];
	ChD3D11::Texture11 buttonSelectImage;

	SelectButtonType selectType = SelectButtonType::None;

	static constexpr int PANEL_DRAW_COUNT = 4;
	ChD3D11::Sprite11 selectMechaList[PANEL_DRAW_COUNT];
	ChD3D11::Texture11 panelSelectImage;

};