#pragma once

class SelectFrame :public ChCpp::BaseFrame
{
public:

	void Init()override;

	void Release()override;

	void Update()override;

private:

	void SetScript();

	void DrawFunction();

	void UpdateFunction();

	ChPtr::Shared<ChD3D11::Mesh11> msd = ChPtr::Make_S<ChD3D11::Mesh11>();
	ChPtr::Shared<ChD3D11::Mesh11> desk = ChPtr::Make_S<ChD3D11::Mesh11>();
	ChPtr::Shared<ChD3D11::Mesh11> room = ChPtr::Make_S<ChD3D11::Mesh11>();

	ChPtr::Shared<ChD3D11::RenderTarget11>msdDisplay = ChPtr::Make_S<ChD3D11::RenderTarget11>();

	ChD3D11::Texture11 backGround;
	ChD3D11::Texture11 descriptionWindow;
	
	ChD3D11::Texture11 toBattle;
	ChD3D11::Texture11 descriptionBattle;
	
	ChD3D11::Texture11 toEdit;
	ChD3D11::Texture11 descriptionEdit;

	ChD3D11::Texture11 toSetting;
	ChD3D11::Texture11 descriptionSetting;

	ChD3D11::CB::CBLight11 light;
};
