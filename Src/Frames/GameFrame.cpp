#include"../BaseIncluder.h"


#include"../AllStruct.h"
#include"../Damage/Damage.h"
#include"../BaseRobot/BaseRobot.h"
#include"GameFrame.h"

#include"../PlayerObj/PlayerObj.h"
#include"../EnemyObj/EnemyObj.h"

#include"../HitPosList/HitPosList.h"

#include"../CloudObj/CloudObj.h"


///////////////////////////////////////////////////////////////////////////////////////
//Gameメソッド
///////////////////////////////////////////////////////////////////////////////////////

void GameFrame::Init()
{

	ChDSound9.SetBGMSound("Battle", "TestBattle.wav", "Sound");
	ChDSound9.PlayBGM("Battle");
	ChDSound9.SetBGMSound("Boss", "MainBattle1.wav", "Sound");
	ChLMat tmpMat;

	HitPos = ChPtr::Make_S<HitPosList>();

	HitPos->Init();

	RobotsList = ChPtr::Make_S<BaseRobotsList>();

	Light =ChPtr::Make_S<ChLight9>(ChDevice.GetDevice());



	ChMeMa.SetSmpXFile("TestField.x", "Field", "Base");
	ChMeMa.SetSmpXFile("TestField2.x", "Field2", "Base");
	ChMeMa.SetSmpXFile("TestFieldHitBox.x", "FieldHit", "Base");

	ChMeMa.SetSmpXFile("Boost.x", "Boost", "Base");

	ChMeMa.SetSmpXFile("skysphya_Bluesky.x", "Sky", "Base");
	
	tmpMat.ScalingMode(MapScaling);
	for (auto Mate : ChMeMa.GetXFileMaterials("Field"))
	{
		Mate->Mat = tmpMat;
	}
	for (auto Mate : ChMeMa.GetXFileMaterials("Field2"))
	{
		Mate->Mat = tmpMat;
	}
	for (auto Mate : ChMeMa.GetXFileMaterials("FieldHit"))
	{
		Mate->Mat = tmpMat;
	}

	ChMeMa.CreateFaseNormal("Field", 0);

	ChMeMa.CreateFaseNormal("FieldHit", 0);

	tmpMat.ScalingMode(MapScaling * 2.0f);
	ChMeMa.GetXFileMaterials("Sky")[0]->Mat = tmpMat;

	Cloud = ChPtr::Make_S<CloudList>();
	Cloud->Init();
	RobotsList->Init();

	RobotsList->SetHitPosList(HitPos);

	RobotsList->SetRobots("", &ChVec3(100.0f, 200.0f, -150.0f),0);


	RobotsList->SetRobots2(ChStd::True, &ChVec3(-60.0f, 100.0f, 10.0f));
	RobotsList->SetRobots2(ChStd::True, &ChVec3(-30.0f, 100.0f, 30.0f));
	RobotsList->SetRobots2(ChStd::True, &ChVec3(30.0f, 100.0f, 30.0f));
	RobotsList->SetRobots2(ChStd::True, &ChVec3(50.0f, 100.0f, 10.0f));
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

void GameFrame::Release()
{

	ChDSound9.ClearBGM();
	HitPos = nullptr;
	RobotsList = nullptr;
	Camera = nullptr;
	Light = nullptr;
	//delete World;
}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Update()
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


	//tmpVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//tmpMat = tmpAngX * tmpAngY * tmpMat;

	//float tmpLen;

	CamUpdate();


	return NextFrame;
}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Update(void)
{

	Cloud->Update();

	ChVec3 tmpVec;

	RobotsList->Update();

	HitPos->UpDate();

	tmpVec = *RobotsList->GetMat(CamRobotNo);

	if (PPos.GetLen(&tmpVec) != 0.0f)
	{
		ChVec3 tmpMove;
		tmpMove = tmpVec - PPos;
		MapPos += tmpMove * MapMove;
		PPos = tmpVec;
	}



}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Render3D(void)
{

	ChLMat tmpMat;
	ChLMat tmpRot;
	ChLMat tmpScl;

	ChEffectDraw.SetCam(Camera);

	ChEffectDraw.LightCheck(FALSE);

	ChDevice.LightSetting(ChStd::False);
	ChEffectDraw.SetLight(Light);




	tmpMat.Identity();

	ChMeMa.DrawSmpXFile(&tmpMat, "Sky");

	ChDevice.LightSetting(ChStd::True);

	ChMeMa.DrawSmpXFile(&tmpMat, "Field");
	ChMeMa.DrawSmpXFile(&tmpMat, "Field2");

	Cloud->Draw();

	HitPos->Draw();

	Light->SetLight(TRUE);

	ChDevice.LightSetting(ChStd::True);
	ChEffectDraw.LightCheck(TRUE);
	ChEffectDraw.SetLight(Light);



	RobotsList->Draw();

}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::Render2D(void)
{
	RobotsList->DrawMarker();

}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::CamUpdate(void)
{
	ChVec3 tmpCamP,tmpVec;
	ChLMat tmpMat;
	tmpMat = *RobotsList->GetCamPos();

	tmpCamP.MatPos(&tmpMat, &BaseCamPos);

	Camera->SetCamPos(&tmpCamP);

	tmpCamP.MatPos(RobotsList->GetCamLookPos(), &BaseCamLook);
	
	Camera->SetCamLook(&tmpCamP);

	Camera->SetCamMat(RobotsList->GetCamLookPos());

	Camera->SetLookMaxLen(4000.0f);


}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::UpdateFunction()
{

	float tmpData = 0.0f;
	switch (Pattern)
	{
	case 0:
		if (RobotsList->IsEnemyLive())break;

		ChDSound9.PlayBGM("Boss");
		RobotsList->SetRobots(ChStd::True, &ChVec3(-150.0f, 200.0f, 100.0f), 1);
		RobotsList->SetRobots(ChStd::True, &ChVec3(-150.0f, 200.0f, 150.0f), 1);
		RobotsList->SetRobots(ChStd::True, &ChVec3(-100.0f, 200.0f, 100.0f), 1);
		RobotsList->SetRobots(ChStd::True, &ChVec3(-100.0f, 200.0f, 150.0f), 1);
		RobotsList->SetRobots(ChStd::False, &ChVec3(100.0f, 200.0f, -200.0f), 2);
		RobotsList->SetRobots(ChStd::False, &ChVec3(100.0f, 200.0f, -250.0f), 2);
		RobotsList->SetRobots(ChStd::False, &ChVec3(150.0f, 200.0f, -200.0f), 2);
		Pattern = 1;

		break;

	case 1:
		if (RobotsList->GetEnemyCnt() < 4)
		{
			EnemyBreakCnt++;
			RobotsList->SetRobots(ChStd::True, &ChVec3(-100.0f, 200.0f, 150.0f), 1);
		}

		if (RobotsList->GetMemberCnt() < 4)
		{
			RobotsList->SetRobots(ChStd::False, &ChVec3(100.0f, 200.0f, -200.0f), 2);
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
		unsigned char tmpData;
		tmpData = (FaidOutCnt <= 200 ? 55 + FaidOutCnt : 55 + 400 - FaidOutCnt);
		ChTexMa.SetBlendAlpha(tmpData, "Space");
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

void GameFrame::DrawFunction()
{
	ChLMat tmpMat;
	tmpMat.Identity();

	ChVec3 tmpVec;
	tmpVec = 0.0f;
	switch (Pattern)
	{
	case 2:

		tmpMat = ChVec3(ChWinAPID.GetWindWidth() / 2.0f, ChWinAPID.GetWindHeight() / 2.0f, 0.0f);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Black")
			, &tmpMat
			, &tmpVec 
			,nullptr);

		break;
	case 3:
		tmpMat = ChVec3(ChWinAPID.GetWindWidth() / 2.0f, ChWinAPID.GetWindHeight() / 2.0f, 0.0f);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Black" ), &tmpMat, &tmpVec, nullptr);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Continue"), &tmpMat, &tmpVec, nullptr);

		break;

	case 4:
		tmpMat = ChVec3(ChWinAPID.GetWindWidth() / 2.0f, ChWinAPID.GetWindHeight() / 2.0f, 0.0f);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Black"), &tmpMat, &tmpVec, nullptr);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Continue"), &tmpMat, &tmpVec, nullptr);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Space"), &tmpMat, &tmpVec, nullptr);

		break;

	case 5:
		tmpMat = ChVec3(ChWinAPID.GetWindWidth() / 2.0f, ChWinAPID.GetWindHeight() / 2.0f, 0.0f);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Black"), &tmpMat, &tmpVec, nullptr);

		ChSp9.DrawSprite(ChTexMa.GetTexture("Continue"), &tmpMat, &tmpVec, nullptr);

		break;
	default:

		break;

	}
}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::Draw()
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
