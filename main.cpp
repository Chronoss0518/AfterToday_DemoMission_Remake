
#include"Src/BaseIncluder.h"

#include"Src/BaseMecha/BaseMecha.h"
#include"Src/BaseMecha/MechaParts.h"

#include"Src/Frames/GameFrame.h"
#include"Src/Frames/TitleFrame.h"

#include"Src/BaseMecha/Controller/ControllerBase.h"
//#include"TestGameLoop.h"

int WINAPI WinMain(
	HINSTANCE hInst
	, HINSTANCE hPrev
	, LPSTR lpszCmdParam
	, int nCmdshow)
{

	//Test::CreateSampleFile();

	//return 0;

	auto&& system = *ChSystem::SysManager().Init<ChSystem::Windows>();

	ChWin::WindClassObject windClass;
	windClass.RegistClass("ChGame");

	auto s_screen = ChWin::GetScreenSize();
	{
		{
			ChWin::WindCreater creater(hInst);

			{
				ChWin::WindStyle style;
				style.AddOverlappedWindow();
				creater.SetWindStyle(&style);
			}

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

		ChD3D::XAudioManager().Init();

	}

	//Test::Init();

	ChCpp::FrameList frameList;

	//frameList.SetFrame<TitleFrame>();
	frameList.SetFrame<GameFrame>();

	// ゲームに関する初期化処理 ---------------------------

	while (system.IsUpdate())
	{
		if (!ChSystem::SysManager().FPSProcess())continue;
		frameList.Update();
		//Test::Update();
		ChD3D::XAudioManager().Update();
	}

	// ゲームに関する終了処理 ---------------------------

	frameList.Release();
	ChD3D11::Shader11().Release();
	ChD3D11::D3D11API().Release();

	return (int)system.GetReturnMassage()->wParam;
}
