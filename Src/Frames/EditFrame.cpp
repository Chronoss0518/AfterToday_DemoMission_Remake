
#include"../BaseIncluder.h"

#include"EditFrame.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"

#include"../LoadDisplay/LoadDisplay.h"

#include"../EditFrameDrawObject/PanelList/PanelList.h"

#define EDIT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Edit/") current_path
#define EDIT_MESH_DIRECTORY(current_path) MESH_DIRECTORY("Edit/") current_path

void EditFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	ChD3D11::Shader11().SetBackColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));

	controller.Init();

	MenuBase::InitMenu(&controller);

	auto&& device = ChD3D11::D3D11Device();
}

void EditFrame::Release()
{

}

void EditFrame::Update()
{
	UpdateFunction();

	DrawFunction();
}

void EditFrame::UpdateAction(ActionType _type)
{

}

void EditFrame::UpdateMouse()
{

}

void EditFrame::DrawFunction()
{
	ChD3D11::Shader11().DrawStart();



	ChD3D11::Shader11().DrawEnd();
}
