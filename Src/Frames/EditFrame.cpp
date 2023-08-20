
#include"../BaseIncluder.h"

#include"EditFrame.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"

#define EDIT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Edit/") current_path
#define EDIT_MESH_DIRECTORY(current_path) MESH_DIRECTORY("Edit/") current_path

#define LEFT_PANEL_TITLE_LOAD "Load My Mecha"


constexpr char const* initTitle = "non title";

void EditFrame::Init()
{
	auto&& windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();

	leftPanels.Create(LEFT_PANEL_TITLE_LOAD, 30, 30, 280, 660, windows->GethWnd());
	rightPanels.Create("Load File", 970, 30, 280, 660, windows->GethWnd());

	//leftPanelsTitle.CreateTexture(windows->GetInstance(), "");
	//rightPanelsTitle.CreateTexture(windows->GetInstance(), "");

}

void EditFrame::Release()
{

}

void EditFrame::UpdateFunction()
{

}

void EditFrame::DrawFunction()
{

}

void EditFrame::Update()
{

}

void EditFrame::CreateNewFile()
{
	editMecha = CreateMecha();
	
	fileName = initTitle;

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

	mecha->Create(ChVec2(GAME_WINDOW_WITDH, GAME_WINDOW_HEIGHT), meshDrawer, nullptr);

	return mecha;
}