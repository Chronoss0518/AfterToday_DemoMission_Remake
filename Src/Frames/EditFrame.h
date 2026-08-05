#pragma once

#include"FromStageSelectFrameData.h"

#ifndef EDIT_TEXTURE_DIRECTORY
#define EDIT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY(L"Edit/") current_path
#endif

#ifndef ACTIVE_COLOR
#define ACTIVE_COLOR ChVec4::FromColor(1.0f,1.0f,0.0f,1.0f)
#endif

#ifndef NOT_ACTIVE_COLOR
#define NOT_ACTIVE_COLOR ChVec4::FromColor(0.0f,1.0f,1.0f,1.0f)
#endif

class BaseMecha;
class MechaParts;
class MechaPartsObject;
class LoadDisplay;

class ParameterList;

class NowLoadingUpdater;

class PartsSelectDisplay;
class PartsChangeDisplay;
class PartsAddWeaponPaletteDisplay;

#include"MenuBase.h"

class EditFrame :public ChCpp::BaseFrame, public MenuBase
{
public:

	enum class EditType
	{
		SelectParts,
		ChangeParts,
		SetWeapon
	};

private:

	enum class SelectButtonType
	{
		Up, Down, None
	};

public:

	class EditFrameDisplayBase
	{
	public:

		virtual void Init(EditFrame* _frame) { frame = _frame; }

		virtual void Update(ActionType _type) = 0;

		virtual void UpdateMouse() {};

		virtual void Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader) = 0;

	protected:

		inline ChPtr::Shared<ChD3D11::Texture11>CreatePanelTitleTexture(const std::wstring& _str) { return frame->CreatePanelTitleTexture(_str); }

		inline ChPtr::Shared<ChD3D11::Texture11>CreatePanelPosTitleTexture(const std::wstring& _str) { return frame->CreatePanelPosTitleTexture(_str); }

		inline ChPtr::Shared<ChD3D11::Texture11>CreatePanelPosPartsTexture(const std::wstring& _str) { return frame->CreatePanelPosPartsTexture(_str); }

		inline ChPtr::Shared<ChD3D11::Texture11>CreatePanelTexture(const std::wstring& _str, TextDrawerWICBitmap& _drawer, const ChVec2& _size) { return frame->CreatePanelTexture(_str, _drawer, _size); }

	protected:

		inline void AddActionType(MenuBase::ActionType _type) { frame->AddActionType(_type); }

	protected:

		inline bool IsNotSelectButtons() { return frame->selectType == SelectButtonType::None; }

	protected:

		void SetBaseParts(ChPtr::Shared<MechaPartsObject> _parts);

		void SetNextParts(ChPtr::Shared<MechaPartsObject> _parts);

	protected:

		inline ChPtr::Shared<BaseMecha> GetEditMecha() { return frame->editMecha; }

		inline ChD3D11::Shader::BaseDrawMesh11<wchar_t>& GetMeshDrawer() { return frame->meshDrawer; }

	protected:

		void OpenChangeParts();

		void CloseChangeParts();

		void RefreshMechaParameter();

		inline void ReturnFrame() { frame->returnFrameFlg = true; }

	private:

		EditFrame* frame = nullptr;

	};

public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void InitTextDrawer(TextDrawerWICBitmap& _initDrawer, const ChVec2& _textureSize, float _fontSize,bool _boldFlg);

private:

	void UpdateAction(ActionType _type)override;

	void UpdateMouse()override;

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

	void ReturnFrame();

	void Load();

	bool LoadPart();

private:

	ChPtr::Shared<LoadDisplay>loadDisplay = nullptr;

	ChD3D11::RenderTarget11 rtView;
	ChD3D11::DepthStencilTexture11 dsView;

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;
	ChD3D11::Texture11 rightPanelBackGround, leftPanelBackGround;
	ChD3D11::Sprite11 backgroundSprite;

	ChD3D11::Shader::BaseDrawMesh11<wchar_t> meshDrawer;
	ChD3D11::CB::CBLight11 light;

	ImageSprite selectButton[ChStd::EnumCast(SelectButtonType::None)];
	ChD3D11::Texture11 selectImage;
	SelectButtonType selectType = SelectButtonType::None;

	ChPtr::Shared<PartsSelectDisplay>partsSelectDisplay = nullptr;
	ChPtr::Shared<PartsChangeDisplay>partsChangeDisplay = nullptr;

	ChPtr::Shared<EditFrameDisplayBase>nowDisplay = nullptr;

	ChPtr::Shared<ParameterList> parameterList = nullptr;

	bool returnFrameFlg = false;

	ChPtr::Shared<NowLoadingUpdater>nowLoadingUpdater = nullptr;

	std::vector<std::wstring> pathList;
	unsigned long loadCount = 0;
	bool loadEndFlg = false;

	TextDrawerWICBitmap panelTitleDrawer;//パネル全体でPositionNameを描画する
	TextDrawerWICBitmap panelPosTitleDrawer;//パーツネームが確認できた際にPosition名を描画する
	TextDrawerWICBitmap panelPosPartsDrawer;//パーツ名を描画する

	ChPtr::Shared<BaseMecha> editMecha = nullptr;

	ChPtr::Shared<FromStageSelectFrameData> sendData = nullptr;

};
