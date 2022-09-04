
#include"../BaseIncluder.h"

#include"TitleFrame.h"


#define TITLE_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Title/") current_path
#define TITLE_MESH_DIRECTORY(current_path) MESH_DIRECTORY("Title/") current_path

void TitleFrame::Init()
{
	//SetCamAnimation//
#if 0
	{
		ChLMat tmpMat;

		tmpMat.RotYPR(-30.0f, 0.0f, 0.0f);
		tmpMat = ChVec3_11(0.0f, -10.0f, -10.0f);
		Animations.SetAniObject(tmpMat);

		tmpMat.RotYPR(0.0f, 0.0f, 0.0f);
		tmpMat = ChVec3_11(0.0f, -10.0f, -10.0f);
		Animations.SetAniObject(tmpMat);

		Animations.Play();
		Animations.SetOneFrameTime(60.0f * 5);
	}

	Light = ChPtr::Make_S<ChLight9>(ChD3D9::D3D9API().GetDevice());

	Mouse = ChPtr::Make_S<ChMouCon>(true, &ChSystem::WinAPI());

	Light->SetDir(ChVec3(1.0f, -1.0f, 1.0f));
	Light->SetLightDif(1.0f, 1.0f, 1.0f, 1.0f);
	Light->SetLightSpe(true, 1.0f, 1.0f, 1.0f);

#endif

	title_name.CreateTexture(TITLE_TEXTURE_DIRECTORY("TitleName.png"));
	title.CreateTexture(TITLE_TEXTURE_DIRECTORY("Title.png"));
	tmpTitle.CreateTexture(TITLE_TEXTURE_DIRECTORY("TmpTitle.png"));
	push_space.CreateTexture(TITLE_TEXTURE_DIRECTORY("Push_Space.png"));

	{
		ChCpp::ModelObject tmpModel;

		ChCpp::ModelLoader::XFile loader;

		loader.CreateModel(msd, TITLE_MESH_DIRECTORY("MSD.x"));
		loader.CreateModel(desk, TITLE_MESH_DIRECTORY("Desk.x"));
		loader.CreateModel(room, TITLE_MESH_DIRECTORY("Room.x"));

	}
	
	{
		auto system = ChSystem::SysManager().GetSystem<ChSystem::Windows>();

		auto windSize = system->GetWindObject().GetWindSize();

		renderTarget->CreateRenderTarget(
			ChD3D11::D3D11Device()
			, windSize.w
			, windSize.h);


		renderTarget->SetBackColor(ChD3D11::D3D11DC(), ChVec4(0, 0, 0, 1));
	}

	{
		unsigned long tmp = 0;

		auto disp = msd->GetFrame("Display");

		auto&& mate = disp->primitiveDatas[0]->mate;

		mate->material.diffuse.a = 2.0f;
		mate->material.diffuse.r = 2.0f;
		mate->material.diffuse.g = 2.0f;
		mate->material.diffuse.b = 2.0f;

		mate->textures[Ch3D::TextureType::Diffuse] = renderTarget;


	}

	blendPow = 255;

	SetScript();


}

void TitleFrame::Update()
{
	if (!ChSystem::SysManager().FPSProcess())return;

	UpdateFunction();

	DrawFunction();
}

void TitleFrame::Release()
{
	Mouse = nullptr;
	Light = nullptr;
	Script.Release();
	renderTarget = nullptr;
	Animations.Release();
}

void TitleFrame::DrawFunction()
{

	ChLMat tmpMat;

	Light->SetLight(true);
	Light->SetDir(ChVec3(0.0f, -0.8f, 0.2f));
	ChD3D9::D3D9API().LightUseFlg(0, true);

	ChD3D9::D3D9API().AlphaBlendSetting();

	Cam->SetView();

	ChD3D9::D3D9API().DrawStart(D3DCOLOR_ARGB(255, 0, 0, 0));

	tmpMat = ChVec3(0.0f, -8.0f, 10.0f);

	MeshList.DrawMesh(tmpMat, ChStd::EnumCast(Mesh::Desk));

	tmpMat.Identity();
	tmpMat.ScalingMode(10.0f);

	tmpMat = ChVec3(0.0f, -23.0f, 10.0f);

	MeshList.DrawMesh(tmpMat, ChStd::EnumCast(Mesh::Room));

	Script.Draw();

	/*
		DrawClass::TShader().DrawPolygonBord(
			TexList.GetTex(ChStd::EnumCast(Tex::TitleName))->Tex
			, tmpPos
			, tmpMat);

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

	ChStd::FPOINT tmpP;
	Mouse->GetMovePos(&tmpP);

	MousePos.y += tmpP.x;
	MousePos.x += tmpP.y;

	Script.Update();

	ChLMat tmpMat;

	RTList.ReturnRT();

	DeviceTexChenge++;
	DeviceTexChenge %= ChengeCount;

	Cam->SetCamLook(ChVec3(0.0f, 0.0f, 0.0f));

	{

		ChLMat tmp;
		ChVec3 tmpVec;

		tmp.RotWorld(MousePos.x, MousePos.y, 0.0f);
		tmpVec.MatNormal(tmp, ChVec3(0.0f, 0.0f, -1.0f));

		tmpVec *= 20.0f;

		//tmpVec += ChVec3(0.0f, 10.0f, 0.0f);

		//tmpVec = ChVec3(0.0f, 0.0f, -20.0f);


		tmpVec = ChVec3(0.0f, 10.0f, 0.0f);
		tmpVec.MatPos(Animations.Update(), tmpVec);


		Cam->SetCamPos(tmpVec);

	}


	//Cam->SetCamPos(ChVec3(0.0f, 0.0f, -20.0f));




}

void TitleFrame::SetScript()
{

	///////////////////////////////////////////////////////////////////////////////////
	//0//
	Script.SetScript
	([this]()
	{
		if (ChSystem::SysManager().IsPushKeyNoHold(VK_SPACE))
		{
			Script.ChangeScript(3);
			TexList.SetBlendAlpha(255, ChStd::EnumCast(Tex::TitleName));
			blendPow = 0;
		}

		BackCol -= MoveCol;

		ChTex::TexManager9().SetBlendAlpha(BackCol, "BrackTex");
		short Test = BackCol;
		Test -= MoveCol;


		ChLMat tmpMat;

		tmpMat = ChVec3(1280 / 2.0f, 720 / 2.0f, 0.0f);

		RTList.SetRT(0);

		if (DeviceTexChenge % 6 == 0 || DeviceTexChenge % 7 == 0)
		{
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title2)), tmpMat
				, ChVec3());
		}
		else {
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title1)), tmpMat
				, ChVec3());
		}

		if (Test > 0)return;
		Script.ChangeScript();

		BackCol = 0;
		TexList.SetBlendAlpha(BackCol, ChStd::EnumCast(Tex::TitleName));
		ChTex::TexManager9().SetBlendAlpha(0, "BrackTex");
	},
		[this]()
	{

		ChLMat tmpMat;
		ChVec3 tmpVec;

		tmpMat.RotYPR(10.0f, 0.0f, 0.0f);

		tmpMat = ChVec3(0.0f, 0.0f, 5.0f);

		tmpVec = tmpMat;

		MeshList.DrawMesh(tmpMat, ChStd::EnumCast(Mesh::MSD));

		tmpMat.Identity();

		tmpMat = ChVec3(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);


		tmpMat.Identity();
		tmpMat.ScalingMode(ChVec3(1280.0f, 720.0f, 0.0f));
		tmpMat = ChVec3(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);

		ChTex::DrawSp9().DrawSprite(
			ChTex::TexManager9().GetTexture("BrackTex")
			, tmpMat, ChVec3());

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

		ChLMat tmpMat;

		tmpMat = ChVec3(1280 / 2.0f, 720 / 2.0f, 0.0f);

		RTList.SetRT(0);

		if (DeviceTexChenge % 6 == 0 || DeviceTexChenge % 7 == 0)
		{
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title2)), tmpMat
				, ChVec3());
		}
		else {
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title1)), tmpMat
				, ChVec3());
		}

		if (Test <= UCHAR_MAX)return;

		TexList.SetBlendAlpha(255, ChStd::EnumCast(Tex::TitleName));
		Script.ChangeScript();
		BackCol = 0;

	},
		[this]()
	{

		ChLMat tmpMat;
		ChVec3 tmpVec;

		tmpMat.RotYPR(10.0f, 0.0f, 0.0f);

		tmpMat = ChVec3(0.0f, 0.0f, 5.0f);

		tmpVec = tmpMat;

		MeshList.DrawMesh(tmpMat, ChStd::EnumCast(Mesh::MSD));

		tmpMat.Identity();
		tmpMat = ChVec3(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);

		ChTex::DrawSp9().DrawSprite(
			TexList.GetTex(ChStd::EnumCast(Tex::TitleName))
			, tmpMat, ChVec3());



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

		ChLMat tmpMat;

		tmpMat = ChVec3(1280 / 2.0f, 720 / 2.0f, 0.0f);

		RTList.SetRT(0);

		if (DeviceTexChenge % 6 == 0 || DeviceTexChenge % 7 == 0)
		{
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title2)), tmpMat
				, ChVec3());
		}
		else {
			ChTex::DrawSp9().DrawSprite(
				TexList.GetTex(ChStd::EnumCast(Tex::Title1)), tmpMat
				, ChVec3());
		}


	},
		[this]()
	{

		ChLMat tmpMat;
		ChVec3 tmpVec;

		tmpMat.RotYPR(10.0f, 0.0f, 0.0f);

		tmpMat = ChVec3(0.0f, 0.0f, 5.0f);

		tmpVec = tmpMat;

		MeshList.DrawMesh(tmpMat, ChStd::EnumCast(Mesh::MSD));

		tmpMat.Identity();
		tmpMat = ChVec3(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);

		ChTex::DrawSp9().DrawSprite(
			TexList.GetTex(ChStd::EnumCast(Tex::TitleName))
			, tmpMat, ChVec3());


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

		ChLMat tmpMat;
		ChVec3 tmpVec;

		tmpMat.RotYPR(10.0f, 0.0f, 0.0f);

		tmpMat = ChVec3(0.0f, 0.0f, 5.0f);

		tmpVec = tmpMat;

		MeshList.DrawMesh(tmpMat, ChStd::EnumCast(Mesh::MSD));

		tmpMat.Identity();
		tmpMat = ChVec3(
			ChSystem::WinAPI().GetWindWidth() / 2.0f
			, ChSystem::WinAPI().GetWindHeight() / 2.0f
			, 0.0f);

		ChTex::DrawSp9().DrawSprite(
			TexList.GetTex(ChStd::EnumCast(Tex::TitleName))
			, tmpMat, ChVec3());


	}
	);
}
