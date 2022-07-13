#include"ChDXLibrary/ChGameIncludeFile.h"
#include"TitleFrame.h"

#include"AllStruct.h"

///////////////////////////////////////////////////////////////////////////////////////
//TiTleメソッド
///////////////////////////////////////////////////////////////////////////////////////


Title::Title()
{

}

///////////////////////////////////////////////////////////////////////////////////

Title::~Title()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////////

void Title::Init()
{
	fps = ChStd::GetTime();
	Light = ChPtr::Make_S<ChLight9>(ChDevice.GetDevice());
	Camera = ChPtr::Make_S<ChCamera9>(ChDevice.GetDevice(), ChWinAPID);

	ChTexMa.SetTexture("Title.png","Title", "Base", 1280, 720, 1280, 720);
	ChTexMa.SetTexture("TitleName.png", "TitleName", "Base", 1280, 720, 1280, 720);
	
	ChDSound9.SetBGMSound("Op", "OP.wav", "Sound");
	ChDSound9.PlayBGM("Op");

	ChTexMa.SetBlendAlpha(255, "Black");
	ChTexMa.SetBlendAlpha(0, "Space");
	ChTexMa.SetBlendAlpha(255, "Title");
	ChTexMa.SetBlendAlpha(0, "TitleName");
}

///////////////////////////////////////////////////////////////////////////////////

void Title::Release()
{
	ChDSound9.ClearBGM();
	ChTexMa.ClearData("Title");
	ChTexMa.ClearData("TitleName");

	Light = nullptr;
	Camera = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////

void Title::Render3D()
{
	ChMatrix_9 TmpMat;
	TmpMat.Identity();
}

///////////////////////////////////////////////////////////////////////////////////

void Title::Render2D()
{
	SqriptDraw();
}

///////////////////////////////////////////////////////////////////////////////////

void Title::Update()
{


	ChWinAPID.SetKeyCode();

	SqriptUpdate();

	if(ChWinAPID.IsPushKeyNoHold(VK_SPACE)&& Pattern < 3)
	{
		AniCnt = 0;
		Pattern = 3;
	}

}

///////////////////////////////////////////////////////////////////////////////////

void Title::SqriptUpdate()
{


	switch (Pattern)
	{
	case 0:

		ChTexMa.SetBlendAlpha(255 - AniCnt, "Black");
		AniCnt++;
		if (AniCnt >= 255) {
			AniCnt = 0;
			Pattern = 1;
		}
		break;

	case 1:
		
		AniCnt++;
		ChTexMa.SetBlendAlpha((unsigned char)AniCnt, "TitleName");

		if (AniCnt >= 255) {
			AniCnt = 0;
			ChTexMa.SetBlendAlpha(255, "TitleName");
			Pattern = 2;
		}
		break;

	case 2:
		unsigned char TmpData;
		TmpData = (AniCnt <= 200 ? 55 + AniCnt : 55 + 400 - AniCnt);
		ChTexMa.SetBlendAlpha(TmpData, "Space");
		AniCnt+= 4;
		
		
		AniCnt %= 400;

		break;

	case 3:

		AniCnt+= 3;
		ChTexMa.SetBlendAlpha((unsigned char)AniCnt, "Black");
		if (AniCnt >= 255) {
			AniCnt = 0;
			NextFrame = 'G';
		}

		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////

void Title::SqriptDraw()
{
	ChMat_9 TmpMat;
	TmpMat = ChVec3_9(ChWinAPID.GetWindWidth() / 2.0f, ChWinAPID.GetWindHeight() / 2.0f, 0.0f);

	ChSp9.DrawSprite(ChTexMa.GetTexture("Title"), &TmpMat, &ChVec3_9(), nullptr);



	switch (Pattern)
	{
	case 0:
		ChSp9.DrawSprite(ChTexMa.GetTexture("Black"), &TmpMat, &ChVec3_9(), nullptr);

		break;
	case 1:

		ChSp9.DrawSprite(ChTexMa.GetTexture("TitleName"), &TmpMat, &ChVec3_9(), nullptr);


		break;
	case 2:

		ChSp9.DrawSprite(ChTexMa.GetTexture("Space"), &TmpMat, &ChVec3_9(), nullptr);

		ChSp9.DrawSprite(ChTexMa.GetTexture("TitleName"), &TmpMat, &ChVec3_9(), nullptr);

		break;

	case 3:

		ChSp9.DrawSprite(ChTexMa.GetTexture("Black"), &TmpMat, &ChVec3_9(), nullptr);

	default:
		break;
	}

}

///////////////////////////////////////////////////////////////////////////////////

char Title::Frame(void)
{
	if (ChStd::GetTime() - fps < 1000 / ChWinAPID.GetFPSCnt())return NextFrame;
	fps = ChStd::GetTime();

	if (ChWinAPID.IsPushKeyNoHold(VK_ESCAPE))PostQuitMessage(0);

	Update();

	// バックバッファと Z バッファをクリア
	ChDevice.DrawStart(D3DCOLOR_XRGB(0, 0, 0));

	Camera->SetCamPos(&D3DXVECTOR3(0.0f, 5.0f, -5.0f));
	Camera->SetCamLook(&D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	Camera->SetView();

	Render3D();

	Render2D();


	ChDevice.DrawEnd();

	return NextFrame;
}

void Script1(void)
{

}

void Script2(void)
{

}

void Script3(void)
{

}

void Script4(void)
{

}
