
#include"Src/BaseIncluder.h"

#include"Src/BaseMecha/BaseMecha.h"
#include"Src/BaseMecha/MechaParts.h"

#include"Src/Frames/GameFrame.h"
#include"Src/Frames/TitleFrame.h"


#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY("RobotParts/" current_path)
#endif

ChPtr::Shared<MechaParts>head, weap;

ChD3D11::Shader::BaseDrawMesh11 meshDrawer;

void TestUpdate();

int WINAPI WinMain(
	HINSTANCE hInst
	, HINSTANCE hPrev
	, LPSTR lpszCmdParam
	, int nCmdshow)
{

	auto&& system = *ChSystem::SysManager().Init<ChSystem::Windows>();

	ChWin::WindClassObject windClass;

	windClass.RegistClass("ChGame");

	{

		auto s_screen = ChWin::GetScreenSize();

		{

			ChWin::WindCreater creater(hInst);

			creater.SetInitSize(s_screen);

			system.Init(creater,
				"AfterToday_DemoMission",
				windClass.GetWindClassName(),
				hInst,
				nCmdshow);

		}
		ChWin::MsgBox msg;
		msg.ClearDisplayButtonType();
		msg.AddDisplayButtonType(ChWin::MsgBox::DisplayButtonType::YesNo);
		bool fullScreenFlg = msg.Display(system.GethWnd(), "全画面確認", "全画面表示で行いますか?") == ChWin::MsgBox::PushButtonType::Yes;

		ChD3D11::D3D11API().Init(system.GethWnd(), fullScreenFlg, s_screen.w, s_screen.h);

		ChD3D11::Shader11().Init(ChD3D11::D3D11API(), s_screen.w, s_screen.h);

		ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 1.0f, 1.0f));

		ChMat_11 proMat, camMat;
		proMat.CreateProjectionMat(ChMath::ToRadian(60.0f), s_screen.w, s_screen.h, 0.1f, 1000.0f);
		camMat.CreateViewMatLookTarget(ChVec3(0.0f, 5.0f, -10.0f), ChVec3(0.0f, 0.0f, 0.0f), ChVec3(0.0f, 1.0f, 0.0f));

		meshDrawer.Init(ChD3D11::D3D11Device());
		meshDrawer.SetProjectionMatrix(proMat);
		meshDrawer.SetViewMatrix(camMat);

		ChD3D::XAudioManager().Init();

	}


	//ChCpp::FrameList frameList;

	//frameList.SetFrame<TitleFrame>();
	//frameList.SetFrame<GameFrame>();

#if 0
	auto head = ChPtr::Make_S<MechaParts>();

	auto camera = head->SetComponent<CameraData>();
	auto scope = head->SetComponent<ScopeData>();

	camera->SetCameraCount(1);
	camera->SetFovy(30.0f);

	scope->SetMaxFovy(45.0f);
	scope->SetMinFovy(10.0f);

	scope->SetFovySlideSpeed(5.0f);

	head->Save(PARTS_DIRECTORY("Head/Normal.mpd"));

	auto weap = ChPtr::Make_S<MechaParts>();

	auto gun = weap->SetComponent<GunData>();

	gun->SetBulletNum(30);
	gun->SetFireNum(1);
	gun->SetMagazineNum(3);
	gun->SetReloadTime(30);
	gun->SetWaitTime(3);

	weap->Save(PARTS_DIRECTORY("Weapon/Gun/MF-TPP1.mpd"));

#endif

	head = ChPtr::Make_S<MechaParts>();
	BaseMecha mechas;
	head->Load(mechas, ChD3D11::D3D11Device(), PARTS_DIRECTORY("Head/Normal.mpd"));
	weap = ChPtr::Make_S<MechaParts>();
	weap->Load(mechas, ChD3D11::D3D11Device(), PARTS_DIRECTORY("Weapon/Gun/MF-TPP1.mpd"));
	//load->Deserialize(mechas, nullptr, text);


	// ゲームに関する初期化処理 ---------------------------

	while (system.IsUpdate())
	{
		if (!ChSystem::SysManager().FPSProcess())continue;
		//frameList.Update();
		TestUpdate();
		ChD3D::XAudioManager().Update();
	}


	// ゲームに関する終了処理 ---------------------------

	return (int)system.GetReturnMassage()->wParam;
}

void TestUpdate()
{
	auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();
	if (windows->IsPushKey(VK_ESCAPE))
	{
		windows->Release();
		return;
	}

	ChD3D11::Shader11().DrawStart();

	ChD3D11::Shader11().DrawStart3D();

	ChLMat tmpMat;
	meshDrawer.DrawStart(ChD3D11::D3D11DC());

	meshDrawer.Draw(ChD3D11::D3D11DC(), weap->GetMesh());
	meshDrawer.DrawEnd();
	ChD3D11::Shader11().DrawEnd();

}