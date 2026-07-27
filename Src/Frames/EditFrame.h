#pragma once

#include"FromStageSelectFrameData.h"

class BaseMecha;
class MechaParts;
class MechaPartsObject;
class LoadDisplay;

class EditList;
class EditListItem;

class SelectPartsList;

class EditControlList;

class ParameterList;


#include"MenuBase.h"

class EditFrame :public ChCpp::BaseFrame, public MenuBase
{
public:

	enum class EditControlType
	{
		Select,
		Change,
		Remove,
		Cancel
	};

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

private:

	void CreateEditControlButtonUseSelectItems();

	void CreateEditControlButtonUnUseSelectItems();

private:

	void SetPartsList(ChPtr::Shared<MechaPartsObject> _parts);

	void SetPanelItem(ChPtr::Shared<EditListItem>& _res,ChPtr::Shared<MechaPartsObject>& _parts, const std::wstring& _positionName);

	void SetPanelPartsItem(ChPtr::Shared<EditListItem>& _res,ChPtr::Shared<MechaPartsObject>& _parts, const std::wstring& _positionName);

private:

	void UpdateAction(ActionType _type)override;

	void UpdateMouse()override;

	void UpdateNowLoadingRect();

private:

	void UpdatePartsListAction(ActionType _type);

	void RefreshPartsList();

private:

	void OpenPartsSelectList();

	void UpdateSelectPartsListAction(ActionType _type);

	void UpdateSelectPartsSetter();

private:

	void UpdateButtonSelect();

	void UpdateButtonChange();

	void UpdateButtonRemove();

	void UpdateButtonCancel();

private:

	void DrawFunction();

	void DrawNowLoading();

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

	ChD3D11::Shader::BaseDrawMesh11<wchar_t> meshDrawer;
	ChD3D11::CB::CBLight11 light;

	ImageSprite selectButton[ChStd::EnumCast(SelectButtonType::None)];
	SelectButtonType selectType = SelectButtonType::None;

	ChPtr::Shared<EditList>partsList = nullptr;
	ChPtr::Shared<SelectPartsList>selectPartsList = nullptr;
	bool partsSelectFlg = false;
	ChPtr::Shared<EditControlList>editControlButtons = nullptr;
	bool useSelectButtonFlg = false;

	ChPtr::Shared<MechaPartsObject>selectParts = nullptr;
	ChPtr::Shared<MechaPartsObject>tmpSelectParts = nullptr;
	ChPtr::Shared<ParameterList> parameterList = nullptr;
	//変更後のパーツ//
	ChPtr::Shared<MechaPartsObject>nowChangeTargetPartsParent = nullptr;
	std::wstring nowChangeTargetPartsName = L"";
	ChPtr::Shared<ChCpp::JsonObject<wchar_t>>beforePartsJson = nullptr;
	std::wstring changePartsPosName = L"";

	std::vector<std::wstring> pathList;
	unsigned long loadCount = 0;
	bool loadEndFlg = false;

	TextDrawerWICBitmap panelTitleDrawer;//パネル全体でPositionNameを描画する
	TextDrawerWICBitmap panelPosTitleDrawer;//パーツネームが確認できた際にPosition名を描画する
	TextDrawerWICBitmap panelPosPartsDrawer;//パーツ名を描画する

	ChPtr::Shared<BaseMecha> editMecha = nullptr;
	bool selectFlg = false;

	ChPtr::Shared<FromStageSelectFrameData> sendData = nullptr;

};
