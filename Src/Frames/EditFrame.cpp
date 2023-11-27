
#include"../BaseIncluder.h"

#include"EditFrame.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"

#include"../EditFrameDrawObject/PanelList/PanelList.h"

#define EDIT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Edit/") current_path
#define EDIT_MESH_DIRECTORY(current_path) MESH_DIRECTORY("Edit/") current_path

#define LEFT_PANEL_TITLE_LOAD "Load My Mecha"

constexpr char const* initTitle = "non title";


void EditFrame::Init()
{
	ChD3D11::Shader11().SetBackColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));

	auto&& device = ChD3D11::D3D11Device();
	drawTexter.Init(ChD3D11::D3D11API().GetSurface());

	spriteShader.Init(device);
	partsTitlePanel.CreateTexture(EDIT_TEXTURE_DIRECTORY("PanelList.png"), device);

	addMecha.CreateTexture("", device);
	
	panelBackGround.CreateTexture(EDIT_TEXTURE_DIRECTORY("PanelList.png"), device);

	auto&& leftPos = ChVec2(30, 30);
	auto&& rightPos = ChVec2(970, 30);
	auto&& size = ChVec2(280, 660);

	auto&& leftPanelRect = RectToGameWindow(ChVec4::FromRect(leftPos.x, leftPos.y, leftPos.x + size.x, leftPos.y + size.y));
	auto&& rightPanelRect = RectToGameWindow(ChVec4::FromRect(rightPos.x, rightPos.y, rightPos.x + size.x, rightPos.y + size.y));

	leftPanel = ChPtr::Make_S<PanelList>();
	leftPanel->Init(ChD3D11::D3D11Device(), drawTexter);
	leftPanel->SetPanelListBackGround(&panelBackGround, leftPanelRect);

	rightPanel = ChPtr::Make_S<PanelList>();
	rightPanel->Init(ChD3D11::D3D11Device(), drawTexter);
	rightPanel->SetPanelListBackGround(&panelBackGround, rightPanelRect);

	panelSprite.Init(ChD3D11::D3D11Device());
	panelSprite.SetInitPosition();
}

void EditFrame::Release()
{

}

void EditFrame::UpdateFunction()
{
	auto&& windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();

	if (windows->IsPushKey(VK_ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}




	SetInitFlg(true);
}

void EditFrame::DrawFunction()
{
	ChD3D11::Shader11().DrawStart();

	spriteShader.DrawStart(ChD3D11::D3D11DC());

	//drawTexter.DrawStart();

	leftPanel->Draw(spriteShader, drawTexter);

	rightPanel->Draw(spriteShader, drawTexter);

	//drawTexter.DrawEnd();

	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();
}

void EditFrame::Update()
{
	UpdateFunction();

	DrawFunction();
}

void EditFrame::CreateNewFile()
{
	editMecha = CreateMecha();
	
	fileName = initTitle;

}

void EditFrame::CreateLoadFileList()
{

	for (auto&& file : std::filesystem::directory_iterator(SAVE_PATH("")))
	{
		std::wstring path = (file.path().c_str());
	}


}

void EditFrame::LoadFile(const std::string& _fileName)
{
	editMecha = CreateMecha();

	editMecha->Load(ChD3D11::D3D11Device(), _fileName);

	fileName = _fileName;
}

void EditFrame::SaveFile()
{
	if (editMecha == nullptr)return;

	editMecha->Save(fileName);
}

void EditFrame::AddPartsObject(BaseMecha::PartsPosNames _name, ChPtr::Shared<MechaPartsObject> _createParts)
{

}

void EditFrame::ChangePartsObejct(BaseMecha::PartsPosNames _name, ChPtr::Shared<MechaPartsObject> _createParts)
{

}

void EditFrame::RemovePartsObejct(BaseMecha::PartsPosNames _name, ChPtr::Shared<MechaPartsObject> _createParts)
{

}

ChPtr::Shared<BaseMecha> EditFrame::CreateMecha()
{
	auto&& mecha = ChPtr::Make_S<BaseMecha>();

	mecha->Create(ChVec2(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), meshDrawer, nullptr);

	return mecha;
}