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

	enum class Mesh :ChStd::DataNo
	{
		MSD
		, Desk
		, Room
	};

	enum class Tex :ChStd::DataNo
	{
		Title1
		, Title2
		, TitleName
		, Push_Space
	};

	unsigned char BackCol;
	const unsigned char MoveCol = 5;
	ChStd::Bool UpDownFlg = false;

	unsigned char DeviceTexChenge = 0;
	unsigned char ChengeCount = 20;

	ChCpp::ChScCon Script;

	//ChAnimationObject9 Animations;

	ChPtr::Shared<ChD3D11::Mesh11> msd = nullptr;
	ChPtr::Shared<ChD3D11::Mesh11> desk = nullptr;
	ChPtr::Shared<ChD3D11::Mesh11> room = nullptr;

	ChD3D11::Texture11 title_name;
	ChD3D11::Texture11 title;
	ChD3D11::Texture11 tmpTitle;
	ChD3D11::Texture11 push_space;
	ChD3D11::Texture11 blackTex;
	float blendPow = 0;


	ChPtr::Shared<ChD3D11::RenderTarget11> renderTarget = ChPtr::Make_S<ChD3D11::RenderTarget11>();

	ChGame::Camera Cam;

	ChPtr::Shared<ChD3D11::CB::CBLight11> Light;


};
