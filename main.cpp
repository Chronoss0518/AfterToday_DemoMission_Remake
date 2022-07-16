
#include"Src/BaseIncluder.h"

#include"Src/GameFrame.h"
#include"Src/TitleFrame.h"


int WINAPI WinMain(
	HINSTANCE hInst
	, HINSTANCE hPrev
	,LPSTR lpszCmdParam
	, int nCmdshow)
{
	
	auto system = ChSystem::SysManager().Init<ChSystem::Windows>();

	ChWin::WindClassObject windClass;

	windClass.RegistClass("ChGame");

	{

		auto s_screen = ChWin::GetScreenSize();

		{

			ChWin::WindCreater creater(hInst);

			creater.SetInitSize(s_screen);

			system->Init(creater,
				"AfterToday_DemoMission",
				windClass.GetWindClassName(),
				hInst,
				nCmdshow);

		}
		ChWin::MsgBox msg;
		msg.ClearDisplayButtonType();
		msg.AddDisplayButtonType(ChWin::MsgBox::DisplayButtonType::YesNo);
		bool fullScreenFlg = msg.Display(system->GethWnd(), "全画面確認", "全画面表示で行いますか?") == ChWin::MsgBox::PushButtonType::Yes;

		ChD3D11::D3D11API().Init(system->GethWnd(), fullScreenFlg, s_screen.w, s_screen.h);

		ChD3D::XAudioManager().Init();

	}

	ChCpp::FrameList frameList;

	frameList.SetFrame<Title>();
	frameList.SetFrame<Game>();


	// ゲームに関する初期化処理 ---------------------------

	while (system->IsUpdate()) 
	{
		if (!ChSystem::SysManager().FPSProcess())continue;
		frameList.Update();
		ChD3D::XAudioManager().Update();
	}


	// ゲームに関する終了処理 ---------------------------

	return (int)system->GetReturnMassage()->wParam;
}