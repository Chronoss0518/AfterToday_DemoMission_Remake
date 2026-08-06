#include"../BaseIncluder.h"

#include"EditFrame.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../Attack/Attack.h"

#include"../EditFrame/ParameterList.h"

#include"../LoadDisplay/LoadDisplay.h"
#include"../NowLoadingUpdater/NowLoadingUpdater.h"

#include"../SelectList/SelectList.h"

#include"../Application/Application.h"

#include"../EditFrame/PartsSelectDisplay.h"
#include"../EditFrame/PartsChangeDisplay.h"

#define MECHA_ROTATION_SPEED 1.0f

#define PARTS_PANEL_LIST_X 30.0f
#define PARTS_PANEL_LIST_Y 141.0f

#define PANEL_SIZE_W 280.0f
#define PANEL_SIZE_H 102.0f

#define UP_BUTTON_PANEL_Y 30.0f
#define DOWN_BUTTON_PANEL_Y 558.0f

#define TEXT_ALIGN 8.0f

#define PANEL_TEXT_X TEXT_ALIGN
#define PANEL_TEXT_WIDTH PANEL_SIZE_W - (TEXT_ALIGN * 2.0f)

#define PANEL_TITLE_Y 26.0f
#define PANEL_TITLE_HEIGHT 45.0f
#define PANEL_TITLE_TEXT_SIZE 24.0f

#define PANEL_POS_TITLE_Y 17.0f
#define PANEL_POS_TITLE_HEIGHT 25.0f
#define PANEL_POS_TITLE_TEXT_SIZE 16.0f

#define PANEL_POS_PARTS_Y 42.0f
#define PANEL_POS_PARTS_HEIGHT 45.0f
#define PANEL_POS_PARTS_TEXT_SIZE 24.0f

#define TMP_CAMERA_POS ChVec3(0.0f, 2.5f, 10.0f)


void EditFrame::EditFrameDisplayBase::SetBaseParts(ChPtr::Shared<MechaPartsObject> _parts)
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();
	frame->parameterList->SetBaseParts(device, _parts);
}

void EditFrame::EditFrameDisplayBase::SetNextParts(ChPtr::Shared<MechaPartsObject> _parts)
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();
	frame->parameterList->SetNextParts(device, _parts);
}

void EditFrame::EditFrameDisplayBase::OpenChangeParts()
{
	frame->nowDisplay = frame->partsChangeDisplay;

	frame->parameterList->SetUseSpecialActionFlg(false);

	auto parts = frame->partsSelectDisplay->GetEditParts();
	if(parts != nullptr)
		frame->partsChangeDisplay->InitChangeData(parts);
	else
		frame->partsChangeDisplay->InitChangeData(frame->partsSelectDisplay->GetSelectParts(), frame->partsSelectDisplay->GetTargetPartsPosName());

}

void EditFrame::EditFrameDisplayBase::CloseChangeParts()
{
	frame->nowDisplay = frame->partsSelectDisplay;

	frame->partsSelectDisplay->InitPartsList(frame->partsChangeDisplay->GetResultParts());

	frame->parameterList->SetUseSpecialActionFlg(true);

	frame->partsChangeDisplay->Close();
}

void EditFrame::EditFrameDisplayBase::RefreshMechaParameter()
{
	frame->parameterList->RefreshMechaParameter(frame->editMecha);
}

void EditFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();

	AppIns().GetDirect3D11().SetBackColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));

	MenuBase::InitMenu();

	spriteShader.Init(device);
	rtView.CreateRenderTarget(device, GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	dsView.CreateDepthBuffer(device, GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);

	meshDrawer.Init(device);
	
	{
		ChMat_11 mat;
		mat.CreateProjectionMat(ChMath::ToRadian(60.0f), GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 0.1f, 1000.0f);
		meshDrawer.SetProjectionMatrix(mat);
		mat.CreateViewMatLookTarget(TMP_CAMERA_POS, ChVec3(0.0f, 2.0f, 0.0f), ChVec3(0.0f, 1.0f, 0.0f));

		meshDrawer.SetViewMatrix(mat);
	}

	nowLoadingUpdater = ChPtr::Make_S<NowLoadingUpdater>();
	nowLoadingUpdater->Init();

	light.Init(device);
	light.SetUseLightFlg(true);
	light.SetCamPos(TMP_CAMERA_POS);
	auto&& direction = ChVec3(3.0f, -2.0f, -4.0f);
	direction.Normalize();
	light.SetDirectionLightData(true,ChVec3(1.0f,1.0f,1.0f), direction, 0.4f);

	InitTextDrawer(panelTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_TITLE_HEIGHT), PANEL_TITLE_TEXT_SIZE,true);
	InitTextDrawer(panelPosTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_TITLE_HEIGHT), PANEL_POS_TITLE_TEXT_SIZE,false);
	InitTextDrawer(panelPosPartsDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_PARTS_HEIGHT), PANEL_POS_PARTS_TEXT_SIZE,true);

	loadDisplay = ChPtr::Make_S<LoadDisplay>();
	loadDisplay->Init();

	editMecha = ChPtr::Make_S<BaseMecha>();

	parameterList = ChPtr::Make_S<ParameterList>();
	AddSpecialKey('Z');
	AddSpecialKey('X');
	AddSpecialKeyMask(XINPUT_GAMEPAD_X | XINPUT_GAMEPAD_Y);

	partsSelectDisplay = ChPtr::Make_S<PartsSelectDisplay>();
	partsSelectDisplay->Init(this);

	partsChangeDisplay = ChPtr::Make_S<PartsChangeDisplay>();
	partsChangeDisplay->Init(this);
	
#if false
	partsList = ChPtr::Make_S<EditList>();
	partsList->Init();

	selectPartsList = ChPtr::Make_S<SelectPartsList>();
	selectPartsList->Init();

	{
		auto&& panel = ChPtr::Make_S<SelectPartsListItem>();

		panel->positionNameTexture = CreatePanelTitleTexture(L"Cancel");

		selectPartsList->AddItem(panel);
	}

#endif

	selectButton[ChStd::EnumCast(SelectButtonType::Up)].image.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"UPButton.png"), device);
	SPRITE_INIT(selectButton[ChStd::EnumCast(SelectButtonType::Up)].sprite,
		RectToGameWindow(ChVec4::FromRect(PARTS_PANEL_LIST_X, UP_BUTTON_PANEL_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, UP_BUTTON_PANEL_Y + PANEL_SIZE_H)));

	selectButton[ChStd::EnumCast(SelectButtonType::Down)].image.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"DownButton.png"), device);
	SPRITE_INIT(selectButton[ChStd::EnumCast(SelectButtonType::Down)].sprite,
		RectToGameWindow(ChVec4::FromRect(PARTS_PANEL_LIST_X, DOWN_BUTTON_PANEL_Y, PARTS_PANEL_LIST_X + PANEL_SIZE_W, DOWN_BUTTON_PANEL_Y + PANEL_SIZE_H)));

	selectImage.CreateTexture(EDIT_TEXTURE_DIRECTORY(L"PartsPanelSelect.png"), device);

	Load();

	sendData = ChPtr::SharedSafeCast<FromStageSelectFrameData>(_sendData);
}

void EditFrame::Release()
{
	loadDisplay = nullptr;

	MechaParts::ClearPartsList();
	Attack::AllRelease();
}

void EditFrame::Update()
{
	loadEndFlg = LoadPart();

	if (loadEndFlg)
	{
		MenuBase::UpdateFunction();

		ReturnFrame();

		editMecha->DestroyCoreTest();

		auto&& rotate = editMecha->GetRotation();

		rotate += ChVec3(0.0f, MECHA_ROTATION_SPEED, 0.0f);

		editMecha->SetRotation(rotate);
	}
	else
		nowLoadingUpdater->Update();

	DrawFunction();
}

void EditFrame::InitTextDrawer(TextDrawerWICBitmap& _initDrawer, const ChVec2& _textureSize, const float _fontSize, bool _boldFlg)
{
	unsigned long w = static_cast<unsigned long>(_textureSize.w);
	unsigned long h = static_cast<unsigned long>(_textureSize.h);

	_initDrawer.bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(w, h);

	_initDrawer.drawer.Init(w, h, _initDrawer.bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);
	_initDrawer.drawer.SetClearDisplayFlg(true);
	_initDrawer.drawer.SetClearDisplayColor(ChVec4(0.0f));

	_initDrawer.brush = _initDrawer.drawer.CreateBrush(ChVec4::FromColor(0.0f,0.0f,0.0f,1.0f));

	_initDrawer.format = _initDrawer.drawer.CreateTextFormat(L"ƒSƒVƒbƒN",nullptr, _boldFlg ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,_fontSize);
}

void EditFrame::UpdateAction(ActionType _type)
{
	if (returnFrameFlg)return;

	auto&& device = AppIns().GetDirect3D11().GetDevice();

	parameterList->Update(_type);

	if (_type == ActionType::Decision)
	{
		if (selectType == SelectButtonType::Up)
		{
			AddActionType(ActionType::Up);
			return;
		}

		if (selectType == SelectButtonType::Down)
		{
			AddActionType(ActionType::Down);
			return;
		}
	}

	if(nowDisplay != nullptr)nowDisplay->Update(_type);

}

void EditFrame::UpdateMouse()
{
	auto&& keyInput = AppIns().GetKeyInput();

	InputTest(MenuBase::ActionType::Decision, keyInput.IsPushKeyNoHold(VK_LBUTTON));

	InputTest(MenuBase::ActionType::Cancel, keyInput.IsPushKeyNoHold(VK_RBUTTON));

	auto&& mouse = ChWin::Mouse();
	mouse.Update();

	auto&& mouseMove = mouse.GetMoveValue();

	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

	for (unsigned char i = 0; i < ChStd::EnumCast(SelectButtonType::None); i++)
	{
		if (!IsMoucePosOnSprite(selectButton[i].sprite))continue;
		selectType = static_cast<SelectButtonType>(i);
		return;
	}

	selectType = SelectButtonType::None;

	if (nowDisplay != nullptr)nowDisplay->UpdateMouse();

}

void EditFrame::DrawFunction()
{
	auto&& dc = AppIns().GetDirect3D11().GetDC();

	rtView.SetBackColor(dc, ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	dsView.ClearDepthBuffer(dc);

	AppIns().GetDirect3D11().DrawStart();

	ID3D11RenderTargetView* tmpView = rtView.GetRTView();

	dc->OMSetRenderTargets(1, &tmpView, dsView.GetDSView());

	if (loadEndFlg)
	{
		meshDrawer.DrawStart(dc);
		light.SetDrawData(dc);

		editMecha->Draw3D();

		meshDrawer.DrawEnd();
	}

	dc->OMSetRenderTargets(1, &tmpView, nullptr);

	spriteShader.SetAlphaBlendFlg(true);
	spriteShader.DrawStart(dc);

	DrawEndLoading();

	DrawNowLoading();

	loadDisplay->Draw(spriteShader);

	spriteShader.DrawEnd();

	AppIns().GetDirect3D11().DrawEnd(rtView);
}

void EditFrame::DrawNowLoading()
{
	if (loadEndFlg)return;

	nowLoadingUpdater->Draw(spriteShader);
}

void EditFrame::DrawEndLoading()
{
	if (!loadEndFlg)return;

	parameterList->Draw(spriteShader);

	if (nowDisplay != nullptr)nowDisplay->Draw(spriteShader);

	for (unsigned char i = 0; i < ChStd::EnumCast(SelectButtonType::None); i++)
	{
		spriteShader.Draw(selectButton[i].image, selectButton[i].sprite);

		if (ChStd::EnumCast(selectType) == i)
			spriteShader.Draw(selectImage, selectButton[i].sprite,ACTIVE_COLOR);
	}
}

ChPtr::Shared<ChD3D11::Texture11>EditFrame::CreatePanelTitleTexture(const std::wstring& _str)
{
	return CreatePanelTexture(_str, panelTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_TITLE_HEIGHT));
}

ChPtr::Shared<ChD3D11::Texture11>EditFrame::CreatePanelPosTitleTexture(const std::wstring& _str)
{
	return CreatePanelTexture(_str, panelPosTitleDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_TITLE_HEIGHT));
}

ChPtr::Shared<ChD3D11::Texture11>EditFrame::CreatePanelPosPartsTexture(const std::wstring& _str)
{
	return CreatePanelTexture(_str, panelPosPartsDrawer, ChVec2(PANEL_TEXT_WIDTH, PANEL_POS_PARTS_HEIGHT));
}

ChPtr::Shared<ChD3D11::Texture11>EditFrame::CreatePanelTexture(const std::wstring& _str, TextDrawerWICBitmap& _drawer, const ChVec2& _size)
{
	_drawer.drawer.DrawStart();

	_drawer.drawer.DrawToScreen(_str, _drawer.format, _drawer.brush, ChVec4::FromRect(0.0f, 0.0f, _size.w, _size.h));

	_drawer.drawer.DrawEnd();

	auto&& res = ChPtr::Make_S<ChD3D11::Texture11>();

	res->CreateColorTexture(AppIns().GetDirect3D11().GetDevice(), _drawer.bitmap.GetBitmap());

	return res;
}

void EditFrame::ReturnFrame()
{
	if (!returnFrameFlg)return;

	if (sendData == nullptr)
	{
		ChangeFrame(ChStd::EnumCast(FrameNo::Select));
		return;
	}

	SendData(sendData);
	ChangeFrame(ChStd::EnumCast(FrameNo::SelectStage));
}

void EditFrame::Load()
{

	for (auto&& file : std::filesystem::directory_iterator(PARTS_DIRECTORY(L"")))
	{
		std::wstring path = file.path().c_str();

		size_t len = path.rfind(L".");

		if (len == std::wstring::npos)continue;

		std::wstring extensionName = path.substr(len);

 		pathList.push_back(path);
	}

}

bool EditFrame::LoadPart()
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();

	if (pathList.size() <= loadCount)return true;

	auto&& parts = MechaParts::LoadParts(*editMecha, &meshDrawer, nullptr, pathList[loadCount]);

	parts->GetBaseObject()->SetParameters(*parts);

	partsChangeDisplay->AddMechaParts(parts);

	loadCount++;

	if (pathList.size() > loadCount)return false;

	editMecha->Create(ChVec2(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), meshDrawer, nullptr);
	editMecha->Load(PLAYER_USE_MECHA_PATH);

	partsSelectDisplay->InitPartsList(editMecha->GetCoreParts());
	nowDisplay = partsSelectDisplay;

	parameterList->Init(device, editMecha);

	pathList.clear();

	return true;

}