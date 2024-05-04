#pragma once

class BaseMecha;
class MechaParts;
class MechaPartsObject;
class LoadDisplay;

class EditList;
class EditListItem;

class ParameterList;


#include"MenuBase.h"

class EditFrame :public ChCpp::BaseFrame, public MenuBase
{
private:

	enum class SelectButtonType
	{
		Up, Down, None
	};

public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void InitTextDrawer(TextDrawerWICBitmap& _initDrawer, const ChVec2& _textureSize, float _fontSize,bool _boldFlg);

	void InitNowLoadingRect();

	void SetPartsList(MechaPartsObject& _parts);

	void SetPanelItem(ChPtr::Shared<EditListItem>& _res,ChPtr::Shared<MechaPartsObject>& _parts, const std::string& _positionName);

	void SetPanelPartsItem(ChPtr::Shared<EditListItem>& _res,ChPtr::Shared<MechaPartsObject>& _parts, const std::string& _positionName);

	void UpdateAction(ActionType _type)override;

	void UpdateMouse()override;

	void UpdateNowLoadingRect();

	void DrawFunction();

	void DrawNowLoading(ID3D11DeviceContext* _dc);

	void DrawEndLoading();

private:

	ChPtr::Shared<ChD3D11::Texture11>CreatePanelTitleTexture(const std::wstring& _str);

	ChPtr::Shared<ChD3D11::Texture11>CreatePanelPosTitleTexture(const std::wstring& _str);

	ChPtr::Shared<ChD3D11::Texture11>CreatePanelPosPartsTexture(const std::wstring& _str);

	ChPtr::Shared<ChD3D11::Texture11>CreatePanelTexture(const std::wstring& _str, TextDrawerWICBitmap& _drawer,const ChVec2& _size);

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
	ChD3D11::Texture11 rightPanelBackGround, leftPanelBackGround;
	ChD3D11::Sprite11 backgroundSprite;

	ChD3D11::Sprite11 nowLoadingSprite;
	ChD3D11::Texture11 nowLoading;
	ChVec4 nowLoadingPosRect;
	ChVec4 nowLoadingUVRect;
	float animationMoveSpeed = 0.0f;
	float animationWaitTime = 0.0f;
	float nowAnimationWaitTime = 1.0f;
	bool upFlg = true;

	ChD3D11::Shader::BaseDrawMesh11 meshDrawer;
	ChD3D11::CB::CBLight11 light;
	ChPtr::Shared<ParameterList> parameterList = nullptr;

	ImageSprite selectButton[ChStd::EnumCast(SelectButtonType::None)];
	SelectButtonType selectType = SelectButtonType::None;

	ChPtr::Shared<EditList>partsList = nullptr;
	ChPtr::Shared<MechaPartsObject>selectParts = nullptr;
	std::vector<ChPtr::Shared<MechaPartsObject>>selectStack;

	std::vector<std::string> pathList;
	unsigned long loadCount = 0;
	bool loadEndFlg = false;

	TextDrawerWICBitmap panelTitleDrawer;//パネル全体でPositionNameを描画する
	TextDrawerWICBitmap panelPosTitleDrawer;//パーツネームが確認できた際にPosition名を描画する
	TextDrawerWICBitmap panelPosPartsDrawer;//パーツ名を描画する

	std::vector<std::string>coreList;

	ChPtr::Shared<BaseMecha> editMecha = nullptr;
	bool selectFlg = false;

};
