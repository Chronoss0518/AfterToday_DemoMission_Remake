
#include"../BaseIncluder.h"

#include"TitleFrame.h"

#define TITLE_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Title/") current_path
#define TITLE_MESH_DIRECTORY(current_path) MESH_DIRECTORY("Title/") current_path

void TitleFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	ChCpp::ModelLoader::XFile xfileLoader;

	xfileLoader.SetMaxBoneNum(100);
	auto&& device = ChD3D11::D3D11Device();
	msd->Init(device);
	xfileLoader.CreateModel(msd,TITLE_MESH_DIRECTORY("MSD.x"));
	
	desk->Init(device);
	xfileLoader.CreateModel(desk,TITLE_MESH_DIRECTORY("Desk.x"));

	room->Init(device);
	xfileLoader.CreateModel(room,TITLE_MESH_DIRECTORY("Room.x"));

	meshDrawer.Init(device);

	ChMat_11 proMat,viewMat;
	proMat.CreateProjectionMat(60.0f,GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, GAME_PROJECTION_NEAR, GAME_PROJECTION_FAR);

	meshDrawer.SetCullMode(D3D11_CULL_NONE);
	ChVec3 camPos = ChVec3(0.0f, 10.0f, 0.0f);

	ChLMat tmpMat;
	tmpMat.SetPosition(0.0f, -10.0f, -10.0f);
	camPos = tmpMat.Transform(camPos);

	viewMat.CreateViewMat(camPos, ChVec3(0.0f, 0.0f, 1.0f), ChVec3(0.0f, 1.0f, 0.0f));

	meshDrawer.SetProjectionMatrix(proMat);
	meshDrawer.SetViewMatrix(viewMat);

}

void TitleFrame::Release()
{
	meshDrawer.Release();

	ReleaseMesh11(msd);
	ReleaseMesh11(desk);
	ReleaseMesh11(room);
}

void TitleFrame::Update()
{
	UpdateFunction();

	DrawFunction();
}

void TitleFrame::SetScript()
{

}

void TitleFrame::DrawFunction()
{
	ChD3D11::Shader11().DrawStart();

	Draw3D();

	Draw2D();

	ChD3D11::Shader11().DrawEnd();
}

void TitleFrame::UpdateFunction()
{

}

void TitleFrame::Draw3D()
{
	auto&& deviceContext = ChD3D11::D3D11DC();

	meshDrawer.DrawStart(deviceContext);

	ChLMat tmpMat;
	tmpMat.SetScalling(10.0f);

	tmpMat.SetPosition(0.0f, -23.0f, 10.0f);
	meshDrawer.Draw(*room, (ChMat_11)tmpMat);

	tmpMat.Identity();
	tmpMat.SetPosition(0.0f, -8.0f, 10.0f);

	meshDrawer.Draw(*desk, (ChMat_11)tmpMat);

	tmpMat.Identity();
	tmpMat.SetRotationXAxis(ChMath::ToRadian(-10.0f));
	tmpMat.SetPosition(ChVec3(0.0f, 0.0f, 5.0f));

	meshDrawer.Draw(*msd, (ChMat_11)tmpMat);

	meshDrawer.DrawEnd();
}

void TitleFrame::Draw2D()
{

}
