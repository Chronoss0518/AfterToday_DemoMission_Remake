#pragma once

class TitleFrame :public ChCpp::BaseFrame
{
public:

	void Init()override;

	void Release()override;

	void Update()override;

private:

	void SetScript();

	void DrawFunction();

	void UpdateFunction();

	unsigned char BackCol;
	const unsigned char MoveCol = 5;
	ChStd::Bool UpDownFlg = false;

	unsigned char DeviceTexChenge = 0;
	unsigned char ChengeCount = 20;

	ChCpp::ChScCon script;

	//ChAnimationObject9 Animations;

	ChPtr::Shared<ChD3D11::Mesh11> msd = ChPtr::Make_S<ChD3D11::Mesh11>();
	ChPtr::Shared<ChD3D11::Mesh11> desk = ChPtr::Make_S<ChD3D11::Mesh11>();
	ChPtr::Shared<ChD3D11::Mesh11> room = ChPtr::Make_S<ChD3D11::Mesh11>();

	ChD3D11::Texture11 title_name;
	ChD3D11::Texture11 title;
	ChD3D11::Texture11 tmpTitle;
	ChD3D11::Texture11 push_space;
	ChD3D11::Texture11 blackTex;
	float blendPow = 0;

	ChLMat startAniMatrix;
	ChLMat endAniMatrix;
	float animationTime = 0.0f;
	const float animationSpeed = 1.0f / 60.0f * 5;

	ChPtr::Shared<ChD3D11::RenderTarget11> renderTarget = ChPtr::Make_S<ChD3D11::RenderTarget11>();

	//ChGame::Camera Cam;

	ChD3D11::CB::CBLight11 light;

	ChVec2 mousePos;
};
