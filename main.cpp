#include"Src/ChDXLibrary/ChGameIncludeFile.h"

#include"Src/GameFrame.h"
#include"Src/TitleFrame.h"

void(*Frame)(void);

ChPtr::Shared<Title>Ti;
ChPtr::Shared<Game>Ga;

void GameFrame(void);

void TitleFrame(void);

bool gameFullScreen;	// フルスクリーン（true,false)

int WINAPI WinMain(
	HINSTANCE hInst
	, HINSTANCE hPrev
	,LPSTR lpszCmdParam
	, int nCmdshow)
{
	
	ChWinAPID.Init(
		"AfterToday_DemoMission"
		, "ChGame"
		, 1280
		, 720
		, hInst
		, nCmdshow);

	gameFullScreen = false;	// ウィンドウモード

	ChStd::SetRand(ChStd::GetTime());
	Frame = TitleFrame;

	ChDSound9.Init(ChWinAPID.GethWnd());
	ChDSound9.SetUseDirectory("../実行/data/Sound", "Sound");

	ChDevice.Init(ChWinAPID.GethWnd(), ChStd::IsFullScrean(ChWinAPID.GethWnd()), ChWinAPID.GetWindWidth(), ChWinAPID.GetWindHeight());

	ChEffectDraw.Init("../実行/data/Model.cso", ChDevice.GetDevice());


	ChMeMa.Init(ChDevice.GetDevice());

	ChTexMa.Init(ChDevice.GetDevice());

	ChTexMa.SetDirectoryPath("../実行/data/Texture", "Base");

	ChTexMa.SetColorTex<UINT>(
		D3DCOLOR_ARGB(255, 0, 0, 0)
		, "Black"
		, ChWinAPID.GetWindWidth()
		, ChWinAPID.GetWindHeight());

	ChTexMa.SetTexture("Push_Space.png", "Space", "Base", 1280, 720, 1280, 720);
	ChTexMa.SetTexture("To_Be_Continue.png", "Continue", "Base", 1280, 720, 1280, 720);

	ChMeMa.SetDirectoryPath("../実行/data/XFile", "Base");
	ChMeMa.SetDirectoryPath("../実行/data/XFile/RobotsParts", "Robparts");
	ChMeMa.SetDirectoryPath("../実行/data/XFile/RobotsParts/Weapon", "Bullet");

	ChPoBo9.Init(ChDevice.GetDevice());

	ChSp9.Init(ChDevice.GetDevice());

	ChDFont9.Init(ChDevice.GetDevice());


	ChRT9.Init(ChDevice.GetDevice()
		, ChWinAPID.GetWindWidth()
		, ChWinAPID.GetWindHeight());

	ChRT9.CreateRT("RT");


	// ゲームに関する初期化処理 ---------------------------

	while (1) {
		if (PeekMessage(ChWinAPID.GetReturnMassage(), NULL, 0, 0, PM_NOREMOVE))
		{
			if (!ChWinAPID.CheckWinMessage())break;
		}
		if (ChStd::PauseProcess('Z'))continue;
		Frame();
		ChDSound9.Update();
	}


	// ゲームに関する終了処理 ---------------------------

	ChDFont9.Release();
	ChEffectDraw.Release();

	ChDevice.Release();
	ChMeMa.Release();

	ChSp9.Release();

	ChRT9.Release();

	timeEndPeriod(1);

	//std::function<int(void)> f = []() {return 0; };

	return (int)ChWinAPID.GetReturnMassage()->wParam;
}

void GameFrame(void)
{
	if(!ChDevice.CheckInstanse())
	{
		PostQuitMessage(0);
		return;
	}
	if(Ga == nullptr)
	{
		Ga =ChPtr::Make_S<Game>();
		Ga->Init();
	}
	
	switch(Ga->Frame())
	{
	case 'T':
		Ga = nullptr;
		Frame = &TitleFrame;
		break;
	default:
		return;
	}
}

void TitleFrame(void)
{
	if (!ChDevice.CheckInstanse())
	{
		PostQuitMessage(0);
		return;
	}
	if (Ti == nullptr)
	{
		Ti = ChPtr::Make_S<Title>();
		Ti->Init();
	}

	switch (Ti->Frame())
	{
	case 'G':
		Ti = nullptr;
		Frame = &GameFrame;
		break;
	default:
		return;
	}
}