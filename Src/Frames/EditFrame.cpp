
#include"../BaseIncluder.h"

#include"EditFrame.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"

#include"../EditFrame/ParameterList.h"

#include"../LoadDisplay/LoadDisplay.h"

#include"../SelectList/SelectList.h"

#define EDIT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Edit/") current_path

#define MECHA_ROTATION_SPEED 1.0f

#define PANEL_SIZE_W 0.0f
#define PANEL_SIZE_H 0.0f

#define PARTS_TITLE_POSITION_X 0.0f
#define PARTS_TITLE_POSITION_Y 0.0f
#define PARTS_TITLE_POSITION_WIDTH 0.0f
#define PARTS_TITLE_POSITION_HEIGHT 0.0f

#define LEFT_PANEL_WIDTH 0.0f

#define TMP_CAMERA_POS ChVec3(0.0f, 2.5f, 10.0f)

class EditListItem : public SelectListItemBase
{
public:

	ChD3D11::Texture11 positionNameTexture;
	ChD3D11::Texture11 partsNameTexture;
};

class EditList :public SelectListBase
{
public:

	EditList()
	{
		sprite.Init();
	}

public:

	void Init()override
	{

	}

public:

	void SetSelectImage(ChD3D11::Texture11* _selectImage)
	{
		if (ChPtr::NullCheck(_selectImage))return;
		selectImage = _selectImage;
	}

public:

	void DrawSelect(ChD3D11::Shader::BaseDrawSprite11& _drawer, bool _isSelectPanel)
	{
		if (!_isSelectPanel)return;
		if (ChPtr::NullCheck(selectImage))return;

		_drawer.Draw(*selectImage, sprite);
	}

	void DrawPanel(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec4& _rect, ChPtr::Shared<SelectListItemBase> _drawItem, unsigned long _itemNo, bool _isSelectPanel)override
	{
		auto&& item = ChPtr::SharedSafeCast<EditListItem>(_drawItem);
		if (item == nullptr)return;

		sprite.SetPosRect(_rect);

		_drawer.Draw(item->positionNameTexture, sprite);
		_drawer.Draw(item->partsNameTexture, sprite);

		DrawSelect(_drawer, _isSelectPanel);
	}

private:

	ChD3D11::Texture11* selectImage = nullptr;
	ChD3D11::Texture11* background = nullptr;
	ChD3D11::Sprite11 sprite;
};

void EditFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	ChD3D11::Shader11().SetBackColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));

	controller.Init();

	MenuBase::InitMenu(&controller);

	auto&& device = ChD3D11::D3D11Device();
	spriteShader.Init(device);
	spriteShader.SetAlphaBlendFlg(true);
	rtView.CreateRenderTarget(GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	dsView.CreateDepthBuffer(GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);

	backgroundSprite.Init();
	backgroundSprite.SetInitPosition();
	meshDrawer.Init(device);
	
	{
		ChMat_11 mat;
		mat.CreateProjectionMat(ChMath::ToRadian(60.0f), GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 0.1f, 1000.0f);
		meshDrawer.SetProjectionMatrix(mat);
		mat.CreateViewMatLookTarget(TMP_CAMERA_POS, ChVec3(0.0f, 2.0f, 0.0f), ChVec3(0.0f, 1.0f, 0.0f));

		meshDrawer.SetViewMatrix(mat);
	}

	light.Init(device);
	light.SetUseLightFlg(true);
	light.SetCamPos(TMP_CAMERA_POS);
	auto&& direction = ChVec3(3.0f, -2.0f, -4.0f);
	direction.Normalize();
	light.SetLightDir(direction);


	loadDisplay = ChPtr::Make_S<LoadDisplay>();
	loadDisplay->Init(device, &controller);

	editMecha = ChPtr::Make_S<BaseMecha>();

	parameterList = ChPtr::Make_S<ParameterList>();

	partsList = ChPtr::Make_S<EditList>();

	selectFlg = false;

	rightPanelBackGroud.CreateTexture(EDIT_TEXTURE_DIRECTORY("PanelList.png"), device);
	leftPanelBackGround.CreateTexture(EDIT_TEXTURE_DIRECTORY("PanelList.png"), device);
	
	Load();
}

void EditFrame::Release()
{
	loadDisplay = nullptr;
}

void EditFrame::Update()
{
	loadEndFlg = LoadPart();

	if (loadEndFlg)
	{
		MenuBase::UpdateFunction();

		auto&& rotate = editMecha->GetRotation();

		editMecha->SetRotation(rotate + ChVec3(0.0f, MECHA_ROTATION_SPEED, 0.0f));
	}


	DrawFunction();
}

void EditFrame::UpdateAction(ActionType _type)
{

	parameterList->Update(_type);

	partsList->UpdateAction(_type);

	if (_type == ActionType::Cancel)
	{
		ChangeFrame(ChStd::EnumCast(FrameNo::Select));
		return;
	}

	if (_type == ActionType::Decision)
	{

	}

}

void EditFrame::UpdateMouse()
{

}

void EditFrame::DrawFunction()
{
	auto&& dc = ChD3D11::D3D11DC();

	rtView.SetBackColor(dc, ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	dsView.ClearDepthBuffer(dc);

	ChD3D11::Shader11().DrawStart();

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

	spriteShader.DrawStart(dc);

	DrawEndLoading();

	DrawNowLoading();

	loadDisplay->Draw(spriteShader);

	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd(rtView);
}

void EditFrame::DrawNowLoading()
{
	if (loadEndFlg)return;

}

void EditFrame::DrawEndLoading()
{
	ChVec2 pos = parameterList->GetDrawStartPosition();
	ChVec2 size = parameterList->GetDrawPanelSize();
	ChVec4 rect = ChVec4::FromRect(pos.x, pos.y, pos.x + size.w,pos.y + size.h);

	backgroundSprite.SetPosRect(RectToGameWindow(rect));

	spriteShader.Draw(rightPanelBackGroud, backgroundSprite);

	parameterList->Draw(spriteShader);
}

void EditFrame::Load()
{

	for (auto&& file : std::filesystem::directory_iterator(PARTS_DIRECTORY("")))
	{
		std::wstring path = file.path().c_str();

		unsigned long len = path.rfind(L".");

		if (len == std::wstring::npos)continue;

		std::wstring extensionName = path.substr(len);

		if (extensionName == L".lst")continue;

 		pathList.push_back(ChStr::UTF8ToString(path));
	}

}

bool EditFrame::LoadPart()
{
	if (pathList.size() <= loadCount)return true;

	auto&& parts = MechaParts::LoadParts(*editMecha, ChD3D11::D3D11Device(), &meshDrawer, nullptr, pathList[loadCount]);

	parts->GetBaseObject()->SetParameters();

	loadCount++;
	
	parts = nullptr;

	if (pathList.size() > loadCount)return false;

	editMecha->Create(ChVec2(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), meshDrawer,nullptr);
	editMecha->Load(ChD3D11::D3D11Device(), PLAYER_USE_MECHA_PATH);

	parameterList->Init(ChD3D11::D3D11Device(), editMecha);

	return true;
}