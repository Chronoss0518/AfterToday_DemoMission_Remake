
#include"../ChDXLibrary2/ChGameIncludeFile.h"

#include"Title.h"

void TitleFrame::Init()
{
	Cam = ChPtr::Make_S<ChCamera9>(ChD3D9::D3D9API().GetDevice(), &ChSystem::WinAPI());

	//SetCamAnimation//
	{
		ChMat_9 TmpMat;

		TmpMat.RotYPR(-30.0f, 0.0f, 0.0f);
		TmpMat = ChVec3_9(0.0f, -10.0f, -10.0f);
		Animations.SetAniObject(TmpMat);

		TmpMat.RotYPR(0.0f, 0.0f, 0.0f);
		TmpMat = ChVec3_9(0.0f, -10.0f, -10.0f);
		Animations.SetAniObject(TmpMat);

		Animations.Play();
		Animations.SetOneFrameTime(60.0f * 5);
	}


	Light = ChPtr::Make_S<ChLight9>(ChD3D9::D3D9API().GetDevice());

	Mouse = ChPtr::Make_S<ChMouCon>(true, &ChSystem::WinAPI());

	Light->SetDir(ChVec3_9(1.0f, -1.0f, 1.0f));
	Light->SetLightDif(1.0f, 1.0f, 1.0f, 1.0f);
	Light->SetLightSpe(true, 1.0f, 1.0f, 1.0f);

	TexList.Init(ChD3D9::D3D9API().GetDevice(), "../ŽÀs/data/Texture");

	TexList.SetTexture("TitleName.png"
		, ChStd::EnumCast(Tex::TitleName), 1280, 720);

	TexList.SetTexture("Title.png"
		, ChStd::EnumCast(Tex::Title1), 1280, 720);

	TexList.SetTexture("TmpTitle.png"
		, ChStd::EnumCast(Tex::Title2), 1280, 720);

	TexList.SetTexture("Push_Space.png"
		, ChStd::EnumCast(Tex::Push_Space), 1280, 720);

	MeshList.Init(ChD3D9::D3D9API().GetDevice(), "../ŽÀs/data/XFile/Title");

	MeshList.SetMesh("MSD.x", ChStd::EnumCast(Mesh::MSD));

	//MeshList.SetMesh("Device.x", ChStd::EnumCast(Mesh::MSD));

	MeshList.SetMesh("Desk.x", ChStd::EnumCast(Mesh::Desk));

	MeshList.SetMesh("Room.x", ChStd::EnumCast(Mesh::Room));

	RTList.Init(
		ChD3D9::D3D9API().GetDevice()
		, ChSystem::WinAPI().GetWindWidth()
		, ChSystem::WinAPI().GetWindHeight());



	RTList.SetClearColor(D3DCOLOR_ARGB(255, 0, 0, 0));

	RTList.CreateRT(0);

	{
		unsigned long Tmp = 0;

		for (auto&& Mate : MeshList.GetMeshMaterials(ChStd::EnumCast(Mesh::MSD)))
		{
			if (Mate->Name.find("Display") != Mate->Name.npos) {

				Mate->Diffuse.a = 2.0f;
				Mate->Diffuse.r = 2.0f;
				Mate->Diffuse.g = 2.0f;
				Mate->Diffuse.b = 2.0f;

				MeshList.SetTexture(ChStd::EnumCast(Mesh::MSD)
					, Tmp, RTList.GetRTTexture(0));

				break;
			}
			Tmp++;
		}

	}

	BackCol = 255;

	ChTex::TexManager9().SetBlendAlpha(255, "BrackTex");

	SetScript();


}

void TitleFrame::Frame()
{
	if (!ChStd::FPSProcess(ChSystem::WinAPI().GetFPSCnt()))return;

	Update();

	Draw();
}

void TitleFrame::Release()
{
	Cam = nullptr;
	Mouse = nullptr;
	Light = nullptr;
	RTList.Release();
	Script.Release();
	Animations.Release();
	TexList.Release();
	MeshList.Release();
}

void TitleFrame::Draw()
{

	ChMat_9 TmpMat;

	Light->SetLight(true);
	Light->SetDir(ChVec3_9(0.0f, -0.8f, 0.2f));
	ChD3D9::D3D9API().LightUseFlg(0, true);

	ChD3D9::D3D9API().AlphaBlendSetting();

	Cam->SetView();

	ChD3D9::D3D9API().DrawStart(D3DCOLOR_ARGB(255, 0, 0, 0));

	TmpMat = ChVec3_9(0.0f, -8.0f, 10.0f);

	MeshList.DrawMesh(TmpMat, ChStd::EnumCast(Mesh::Desk));

	TmpMat.Identity();
	TmpMat.ScalingMode(10.0f);

	TmpMat = ChVec3_9(0.0f, -23.0f, 10.0f);

	MeshList.DrawMesh(TmpMat, ChStd::EnumCast(Mesh::Room));

	Script.Draw();

	/*
		DrawClass::TShader().DrawPolygonBord(
			TexList.GetTex(ChStd::EnumCast(Tex::TitleName))->Tex
			, TmpPos
			, TmpMat);

		ChangeFrame("");
	*/

	ChD3D9::D3D9API().DrawEnd();
}

void TitleFrame::Update()
{

	if (ChSystem::WinAPI().IsPushKey(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	ChStd::FPOINT TmpP;
	Mouse->GetMovePos(&TmpP);

	MousePos.y += TmpP.x;
	MousePos.x += TmpP.y;

	Script.Update();

	ChMat_9 TmpMat;

	RTList.ReturnRT();

	DeviceTexChenge++;
	DeviceTexChenge %= ChengeCount;

	Cam->SetCamLook(ChVec3_9(0.0f, 0.0f, 0.0f));

	{

		ChMat_9 Tmp;
		ChVec3_9 TmpVec;

		Tmp.RotWorld(MousePos.x, MousePos.y, 0.0f);
		TmpVec.MatNormal(Tmp, ChVec3_9(0.0f, 0.0f, -1.0f));

		TmpVec *= 20.0f;

		//TmpVec += ChVec3_9(0.0f, 10.0f, 0.0f);

		//TmpVec = ChVec3_9(0.0f, 0.0f, -20.0f);


		TmpVec = ChVec3_9(0.0f, 10.0f, 0.0f);
		TmpVec.MatPos(Animations.Update(), TmpVec);


		Cam->SetCamPos(TmpVec);

	}


	//Cam->SetCamPos(ChVec3_9(0.0f, 0.0f, -20.0f));




}

void TitleFrame::SetScript()
{

	///////////////////////////////////////////////////////////////////////////////////
	//0//
	Script.SetScript
	([this]()
	{
		if (ChSystem::WinAPI().IsPushKeyNoHold(VK_SPACE))
		{
			Script.ChangeScript(3);
			TexList.SetBlendAlpha(255, ChStd::EnumCast(Tex::TitleName));
			ChTex::TexManager9().SetBlendAlpha(0, "BrackTex");
		}

		BackCol -= MoveCol;

		ChTex::TexManager9().SetBlendAlpha(BackCol, "BrackTex");
		short Test = BackCol;
		Test -= MoveCol;


		ChMat_9 TmpMat;

		TmpMat = ChVec3_9(1280 / 2.0f, 720 / 2.0f, 0.0f);

		RTList.SetRT(0);

		if (DeviceTexChenge % 6 == 0 || DeviceTexChenge % 7 == 0)
		{
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title2)), TmpMat
				, ChVec3_9());
		}
		else {
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title1)), TmpMat
				, ChVec3_9());
		}

		if (Test > 0)return;
		Script.ChangeScript();

		BackCol = 0;
		TexList.SetBlendAlpha(BackCol, ChStd::EnumCast(Tex::TitleName));
		ChTex::TexManager9().SetBlendAlpha(0, "BrackTex");
	},
		[this]()
	{

		ChMat_9 TmpMat;
		ChVec3_9 TmpVec;

		TmpMat.RotYPR(10.0f, 0.0f, 0.0f);

		TmpMat = ChVec3_9(0.0f, 0.0f, 5.0f);

		TmpVec = TmpMat;

		MeshList.DrawMesh(TmpMat, ChStd::EnumCast(Mesh::MSD));

		TmpMat.Identity();

		TmpMat = ChVec3_9(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);


		TmpMat.Identity();
		TmpMat.ScalingMode(ChVec3_9(1280.0f, 720.0f, 0.0f));
		TmpMat = ChVec3_9(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);

		ChTex::DrawSp9().DrawSprite(
			ChTex::TexManager9().GetTexture("BrackTex")
			, TmpMat, ChVec3_9());

	}
	);

	///////////////////////////////////////////////////////////////////////////////////
	//1//
	Script.SetScript
	([this]()
	{
		if (ChSystem::WinAPI().IsPushKeyNoHold(VK_SPACE))
		{
			Script.ChangeScript(3);
			TexList.SetBlendAlpha(255, ChStd::EnumCast(Tex::TitleName));
		}


		BackCol += MoveCol;

		TexList.SetBlendAlpha(BackCol, ChStd::EnumCast(Tex::TitleName));

		short Test = BackCol;
		Test += MoveCol;

		ChMat_9 TmpMat;

		TmpMat = ChVec3_9(1280 / 2.0f, 720 / 2.0f, 0.0f);

		RTList.SetRT(0);

		if (DeviceTexChenge % 6 == 0 || DeviceTexChenge % 7 == 0)
		{
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title2)), TmpMat
				, ChVec3_9());
		}
		else {
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title1)), TmpMat
				, ChVec3_9());
		}

		if (Test <= UCHAR_MAX)return;

		TexList.SetBlendAlpha(255, ChStd::EnumCast(Tex::TitleName));
		Script.ChangeScript();
		BackCol = 0;

	},
		[this]()
	{

		ChMat_9 TmpMat;
		ChVec3_9 TmpVec;

		TmpMat.RotYPR(10.0f, 0.0f, 0.0f);

		TmpMat = ChVec3_9(0.0f, 0.0f, 5.0f);

		TmpVec = TmpMat;

		MeshList.DrawMesh(TmpMat, ChStd::EnumCast(Mesh::MSD));

		TmpMat.Identity();
		TmpMat = ChVec3_9(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);

		ChTex::DrawSp9().DrawSprite(
			TexList.GetTex(ChStd::EnumCast(Tex::TitleName))
			, TmpMat, ChVec3_9());



	}
	);

	///////////////////////////////////////////////////////////////////////////////////
	//2//
	Script.SetScript
	([this]()
	{
		if (ChSystem::WinAPI().IsPushKeyNoHold(VK_SPACE))
		{
			Script.ChangeScript(3);
		}

		ChMat_9 TmpMat;

		TmpMat = ChVec3_9(1280 / 2.0f, 720 / 2.0f, 0.0f);

		RTList.SetRT(0);

		if (DeviceTexChenge % 6 == 0 || DeviceTexChenge % 7 == 0)
		{
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title2)), TmpMat
				, ChVec3_9());
		}
		else {
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title1)), TmpMat
				, ChVec3_9());
		}


	},
		[this]()
	{

		ChMat_9 TmpMat;
		ChVec3_9 TmpVec;

		TmpMat.RotYPR(10.0f, 0.0f, 0.0f);

		TmpMat = ChVec3_9(0.0f, 0.0f, 5.0f);

		TmpVec = TmpMat;

		MeshList.DrawMesh(TmpMat, ChStd::EnumCast(Mesh::MSD));

		TmpMat.Identity();
		TmpMat = ChVec3_9(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);

		ChTex::DrawSp9().DrawSprite(
			TexList.GetTex(ChStd::EnumCast(Tex::TitleName))
			, TmpMat, ChVec3_9());


	}
	);

	///////////////////////////////////////////////////////////////////////////////////
	//3//
	Script.SetScript
	([this]()
	{



	},
		[this]()
	{

		ChMat_9 TmpMat;
		ChVec3_9 TmpVec;

		TmpMat.RotYPR(10.0f, 0.0f, 0.0f);

		TmpMat = ChVec3_9(0.0f, 0.0f, 5.0f);

		TmpVec = TmpMat;

		MeshList.DrawMesh(TmpMat, ChStd::EnumCast(Mesh::MSD));

		TmpMat.Identity();
		TmpMat = ChVec3_9(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);

		ChTex::DrawSp9().DrawSprite(
			TexList.GetTex(ChStd::EnumCast(Tex::TitleName))
			, TmpMat, ChVec3_9());


	}
	);
}
