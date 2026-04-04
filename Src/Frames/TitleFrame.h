#pragma once

class TitleFrame :public ChCpp::BaseFrame
{
public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void SetScript();

	void DrawFunction();

	void UpdateFunction();

	void Draw3D();

	void Draw2D();

	unsigned char backCol;
	const unsigned char moveCol = 5;
	bool upDownFlg = false;

	unsigned char deviceTexChenge = 0;
	unsigned char chengeCount = 20;

	ChD3D11::Shader::BaseDrawMesh11<wchar_t> meshDrawer;

	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> msd = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> desk = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> room = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();

	ChD3D11::Texture11 title_name;
	ChD3D11::Texture11 title;
	ChD3D11::Texture11 push_space;
	ChD3D11::Texture11 blackTex;
	float blendPow = 0;

	ChLMat startAniMatrix;
	ChLMat endAniMatrix;
	float animationTime = 0.0f;
	const float animationSpeed = 1.0f / 60.0f * 5;

	ChPtr::Shared<ChD3D11::RenderTarget11> renderTarget = ChPtr::Make_S<ChD3D11::RenderTarget11>();

	ChD3D11::CB::CBLight11 light;

	ChWin::MouseController* mouse = &ChWin::Mouse();
};
