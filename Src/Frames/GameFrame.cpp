#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>


#include"AllStruct.h"
#include"Damage/Damage.h"
#include"BaseRobot/BaseRobot.h"
#include"GameFrame.h"

#include"PlayerObj/PlayerObj.h"
#include"EnemyObj/EnemyObj.h"

#include"HitPosList/HitPosList.h"

#include"CloudObj/CloudObj.h"


///////////////////////////////////////////////////////////////////////////////////////
//Gameメソッド
///////////////////////////////////////////////////////////////////////////////////////

void Game::Init()
{

	ChDSound9.SetBGMSound("Battle", "TestBattle.wav", "Sound");
	ChDSound9.PlayBGM("Battle");
	ChDSound9.SetBGMSound("Boss", "MainBattle1.wav", "Sound");
	ChMat_9 TmpMat;

	HitPos = ChPtr::Make_S<HitPosList>();

	HitPos->Init();

	RobotsList = ChPtr::Make_S<BaseRobotsList>();

	Camera =ChPtr::Make_S<ChCamera9>(ChDevice.GetDevice(), ChWinAPID);

	Light =ChPtr::Make_S<ChLight9>(ChDevice.GetDevice());



	ChMeMa.SetSmpXFile("TestField.x", "Field", "Base");
	ChMeMa.SetSmpXFile("TestField2.x", "Field2", "Base");
	ChMeMa.SetSmpXFile("TestFieldHitBox.x", "FieldHit", "Base");

	ChMeMa.SetSmpXFile("Boost.x", "Boost", "Base");

	ChMeMa.SetSmpXFile("skysphya_Bluesky.x", "Sky", "Base");
	
	TmpMat.ScalingMode(MapScaling);
	for (auto Mate : ChMeMa.GetXFileMaterials("Field"))
	{
		Mate->Mat = TmpMat;
	}
	for (auto Mate : ChMeMa.GetXFileMaterials("Field2"))
	{
		Mate->Mat = TmpMat;
	}
	for (auto Mate : ChMeMa.GetXFileMaterials("FieldHit"))
	{
		Mate->Mat = TmpMat;
	}

	ChMeMa.CreateFaseNormal("Field", 0);

	ChMeMa.CreateFaseNormal("FieldHit", 0);

	TmpMat.ScalingMode(MapScaling * 2.0f);
	ChMeMa.GetXFileMaterials("Sky")[0]->Mat = TmpMat;

	Cloud = ChPtr::Make_S<CloudList>();
	Cloud->Init();
	RobotsList->Init();

	RobotsList->SetHitPosList(HitPos);

	RobotsList->SetRobots("", &ChVec3_9(100.0f, 200.0f, -150.0f),0);


	RobotsList->SetRobots2(ChStd::True, &ChVec3_9(-60.0f, 100.0f, 10.0f));
	RobotsList->SetRobots2(ChStd::True, &ChVec3_9(-30.0f, 100.0f, 30.0f));
	RobotsList->SetRobots2(ChStd::True, &ChVec3_9(30.0f, 100.0f, 30.0f));
	RobotsList->SetRobots2(ChStd::True, &ChVec3_9(50.0f, 100.0f, 10.0f));
	RobotsList->SetCamRobot(0);

	Light->SetDir(&D3DXVECTOR3(1.0f, -1.0f, 0.0f));
	Light->SetLightDif(0.0f, 1.0f, 1.0f, 1.0f);
	Light->SetLightSpe(ChStd::True, 1.0f, 1.0f, 1.0f);
	ChDevice.SetCullMode(ChCULL(CCW));


	PPos = *RobotsList->GetMat(0);

	ChTexMa.SetBlendAlpha(0, "Black");
	ChTexMa.SetBlendAlpha(0, "Continue");
	ChTexMa.SetBlendAlpha(0, "Space");
}

///////////////////////////////////////////////////////////////////////////////////

void Game::Release()
{

	ChDSound9.ClearBGM();
	HitPos = nullptr;
	RobotsList = nullptr;
	Camera = nullptr;
	Light = nullptr;
	//delete World;
}

///////////////////////////////////////////////////////////////////////////////////

char Game::Frame(void)
{

	if (DFlg)Draw();

	if (!ChStd::FPSProcess(ChWinAPID.GetFPSCnt()))return NextFrame;

	if(!RobotsList->IsMemberLive())NextFrame = 'T';
	ChWinAPID.SetKeyCode();
	if (ChWinAPID.IsPushKeyNoHold(VK_ESCAPE))NextFrame = 'T';
	DFlg = ChStd::True;

	SqriptUpdate();
	if (Pattern > 2)return NextFrame;

	// バックバッファと Z バッファをクリア
	ChDevice.GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	

	Update();


	//TmpVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//TmpMat = TmpAngX * TmpAngY * TmpMat;

	//float TmpLen;

	CamUpdate();


	return NextFrame;
}

///////////////////////////////////////////////////////////////////////////////////

void Game::Update(void)
{

	Cloud->Update();

	ChVec3_9 TmpVec;

	RobotsList->Update();

	HitPos->UpDate();

	TmpVec = *RobotsList->GetMat(CamRobotNo);

	if (PPos.GetLen(&TmpVec) != 0.0f)
	{
		ChVec3_9 TmpMove;
		TmpMove = TmpVec - PPos;
		MapPos += TmpMove * MapMove;
		PPos = TmpVec;
	}



}

///////////////////////////////////////////////////////////////////////////////////

void Game::Render3D(void)
{

	ChMatrix_9 TmpMat;
	ChMatrix_9 TmpRot;
	ChMatrix_9 TmpScl;

	ChEffectDraw.SetCam(Camera);

	ChEffectDraw.LightCheck(FALSE);

	ChDevice.LightSetting(ChStd::False);
	ChEffectDraw.SetLight(Light);




	TmpMat.Identity();

	ChMeMa.DrawSmpXFile(&TmpMat, "Sky");

	ChDevice.LightSetting(ChStd::True);

	ChMeMa.DrawSmpXFile(&TmpMat, "Field");
	ChMeMa.DrawSmpXFile(&TmpMat, "Field2");

	Cloud->Draw();

	HitPos->Draw();

	Light->SetLight(TRUE);

	ChDevice.LightSetting(ChStd::True);
	ChEffectDraw.LightCheck(TRUE);
	ChEffectDraw.SetLight(Light);



	RobotsList->Draw();

}

//////////////////////////////////////////////////////////////////////////////////

void Game::Render2D(void)
{
	RobotsList->DrawMarker();

}

//////////////////////////////////////////////////////////////////////////////////

void Game::CamUpdate(void)
{
	ChVector3_9 TmpCamP,TmpVec;
	ChMat_9 TmpMat;
	TmpMat = *RobotsList->GetCamPos();

	TmpCamP.MatPos(&TmpMat, &BaseCamPos);

	Camera->SetCamPos(&TmpCamP);

	TmpCamP.MatPos(RobotsList->GetCamLookPos(), &BaseCamLook);
	
	Camera->SetCamLook(&TmpCamP);

	Camera->SetCamMat(RobotsList->GetCamLookPos());

	Camera->SetLookMaxLen(4000.0f);


}

//////////////////////////////////////////////////////////////////////////////////

void Game::SqriptUpdate()
{

	float TmpData = 0.0f;
	switch (Pattern)
	{
	case 0:
		if (RobotsList->IsEnemyLive())break;

		ChDSound9.PlayBGM("Boss");
		RobotsList->SetRobots(ChStd::True, &ChVec3_9(-150.0f, 200.0f, 100.0f), 1);
		RobotsList->SetRobots(ChStd::True, &ChVec3_9(-150.0f, 200.0f, 150.0f), 1);
		RobotsList->SetRobots(ChStd::True, &ChVec3_9(-100.0f, 200.0f, 100.0f), 1);
		RobotsList->SetRobots(ChStd::True, &ChVec3_9(-100.0f, 200.0f, 150.0f), 1);
		RobotsList->SetRobots(ChStd::False, &ChVec3_9(100.0f, 200.0f, -200.0f), 2);
		RobotsList->SetRobots(ChStd::False, &ChVec3_9(100.0f, 200.0f, -250.0f), 2);
		RobotsList->SetRobots(ChStd::False, &ChVec3_9(150.0f, 200.0f, -200.0f), 2);
		Pattern = 1;

		break;

	case 1:
		if (RobotsList->GetEnemyCnt() < 4)
		{
			EnemyBreakCnt++;
			RobotsList->SetRobots(ChStd::True, &ChVec3_9(-100.0f, 200.0f, 150.0f), 1);
		}

		if (RobotsList->GetMemberCnt() < 4)
		{
			RobotsList->SetRobots(ChStd::False, &ChVec3_9(100.0f, 200.0f, -200.0f), 2);
		}

		if (EnemyBreakCnt > 2) {
			RobotsList->DownDFlg();
			Pattern = 2; 
			FaidOutCnt = 0;
		}

		break;

	case 2:

		FOCUpDateCnt += 1;
		FOCUpDateCnt %= 2;

		if (FOCUpDateCnt == 0)FaidOutCnt += 1;

		ChTexMa.SetBlendAlpha((unsigned char)FaidOutCnt, "Black");

		if (FaidOutCnt >= 255)
		{
			ChTexMa.SetBlendAlpha(255, "Black");
			FaidOutCnt = 0;
			Pattern = 3;
		}
		break;
	case 3:
		FaidOutCnt += 1;

		ChTexMa.SetBlendAlpha((unsigned char)FaidOutCnt, "Continue");
		if (FaidOutCnt >= 255)
		{
			ChTexMa.SetBlendAlpha(255, "Continue");
			FaidOutCnt = 0;
			Pattern = 4;
		}
		break;
	case 4:

		if(ChWinAPID.IsPushKeyNoHold(VK_SPACE))Pattern = 5;
		unsigned char TmpData;
		TmpData = (FaidOutCnt <= 200 ? 55 + FaidOutCnt : 55 + 400 - FaidOutCnt);
		ChTexMa.SetBlendAlpha(TmpData, "Space");
		FaidOutCnt += 4;


		FaidOutCnt %= 400;

		break;

	case 5:
		ChTexMa.SetBlendAlpha(255 - FaidOutCnt, "Continue");
		FaidOutCnt += 3;
		if (FaidOutCnt >= 255) {
			NextFrame = 'T';
		}
		break;
	default:
		break;

	}
}

//////////////////////////////////////////////////////////////////////////////////

void Game::SqriptDraw()
{
	ChMat_9 TmpMat;
	TmpMat.Identity();

	ChVec3_9 TmpVec;
	TmpVec = 0.0f;
	switch (Pattern)
	{
	case 2:

		TmpMat = ChVec3_9(ChWinAPID.GetWindWidth() / 2.0f, ChWinAPID.GetWindHeight() / 2.0f, 0.0f);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Black")
			, &TmpMat
			, &TmpVec 
			,nullptr);

		break;
	case 3:
		TmpMat = ChVec3_9(ChWinAPID.GetWindWidth() / 2.0f, ChWinAPID.GetWindHeight() / 2.0f, 0.0f);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Black" ), &TmpMat, &TmpVec, nullptr);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Continue"), &TmpMat, &TmpVec, nullptr);

		break;

	case 4:
		TmpMat = ChVec3_9(ChWinAPID.GetWindWidth() / 2.0f, ChWinAPID.GetWindHeight() / 2.0f, 0.0f);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Black"), &TmpMat, &TmpVec, nullptr);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Continue"), &TmpMat, &TmpVec, nullptr);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Space"), &TmpMat, &TmpVec, nullptr);

		break;

	case 5:
		TmpMat = ChVec3_9(ChWinAPID.GetWindWidth() / 2.0f, ChWinAPID.GetWindHeight() / 2.0f, 0.0f);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Black"), &TmpMat, &TmpVec, nullptr);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Continue"), &TmpMat, &TmpVec, nullptr);

		break;
	default:

		break;

	}
}

//////////////////////////////////////////////////////////////////////////////////

void Game::Draw()
{


	ChDevice.DrawStart(D3DCOLOR_XRGB(0, 0, 0));

	Camera->SetView();

	ChEffectDraw.SetCam(Camera);
	
	if (Pattern < 3)
	{
		Render3D();

		Render2D();

	}
	SqriptDraw();


	ChDevice.DrawEnd();

	DFlg = ChStd::False;

}
