#pragma once

class BaseMecha;
class MechaParts;
class MechaPartsObject;
class LoadDisplay;

class EditList;

class ParameterList;


#include"MenuBase.h"

class EditFrame :public ChCpp::BaseFrame, public MenuBase
{
public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void DrawFunction();

	void DrawNowLoading();

	void DrawEndLoading();

	void UpdateAction(ActionType _type)override;

	void UpdateMouse()override;

private:

	void Load();

private:

	bool LoadPart();

private:

	ChD3D::XInputController controller;

	ChPtr::Shared<LoadDisplay>loadDisplay = nullptr;

	ChD3D11::RenderTarget11 rtView;
	ChD3D11::DepthStencilTexture11 dsView;

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;
	ChD3D11::Texture11 rightPanelBackGroud, leftPanelBackGround;
	ChD3D11::Sprite11 backgroundSprite;

	ChD3D11::Shader::BaseDrawMesh11 meshDrawer;
	ChD3D11::CB::CBLight11 light;
	ChPtr::Shared<ParameterList> parameterList = nullptr;

	ChPtr::Shared<EditList>partsList = nullptr;

	std::vector<std::string> pathList;
	unsigned long loadCount = 0;
	bool loadEndFlg = false;

	TextDrawerWICBitmap panelAllDrawer;//パネル全体でPositionNameを描画する
	TextDrawerWICBitmap positionNameDrawer;//パーツネームが確認できた際にPosition名を描画する
	TextDrawerWICBitmap partsNameDrawer;//パーツ名を描画する

	std::vector<std::string>coreList;

	ChPtr::Shared<BaseMecha> editMecha = nullptr;
	bool selectFlg = false;

};
