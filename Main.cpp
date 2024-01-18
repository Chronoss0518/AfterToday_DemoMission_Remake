
#include"Src/BaseIncluder.h"

#include"Src/BaseMecha/BaseMecha.h"
#include"Src/BaseMecha/MechaParts.h"

#include"Src/Frames/GameFrame.h"
#include"Src/Frames/TitleFrame.h"
#include"Src/Frames/SelectFrame.h"
#include"Src/Frames/EditFrame.h"
#include"Src/Frames/StageSelectFrame.h"

#include"Src/BaseMecha/Controller/ControllerBase.h"
#include "Src/StageSelectFrame/StageData/StageData.h"
//#include"TestGameLoop.h"

int WINAPI WinMain(
	HINSTANCE hInst
	, HINSTANCE hPrev
	, LPSTR lpszCmdParam
	, int nCmdshow)
{

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
				style.AddClipChildren();
				creater.SetWindStyle(&style);
			}

			creater.SetInitSize(s_screen);

			system.Init(creater,
				"AfterToday_DemoMission",
				windClass.GetWindClassName(),
				hInst,
				nCmdshow);

			system.SetWinProcedure(WM_DESTROY, [&](HWND _hWnd, UINT _msg, WPARAM _wPalam, LPARAM _lParam)->LRESULT {
					system.Release();
					return 0;
				});

		}
		ChWin::MsgBox msg;
		msg.ClearDisplayButtonType();
		msg.AddDisplayButtonType(ChWin::MsgBox::DisplayButtonType::YesNo);
		bool fullScreenFlg = msg.Display(system.GethWnd(), "全画面確認", "全画面表示で行いますか?") == ChWin::MsgBox::PushButtonType::Yes;

		ChD3D11::D3D11API().Init(system.GethWnd(), fullScreenFlg, GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);

		ChD3D11::Shader11().Init(ChD3D11::D3D11API(), GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

		ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 1.0f, 1.0f));

		ChD3D::XAudioManager().Init();

		ChWin::Mouse().Init(system);
	}

	ChCpp::FrameList frameList;

	auto testData = ChPtr::Make_S<StageDataStructure>();

	testData->stageScriptPath = "stage1.chs";

	frameList.SetSendData(testData);

	ChD3D::WICBitmapCreatorObj().Init();

#if USE_TITLE_FRAME_FLG
	frameList.SetFrame<TitleFrame>();
#endif
#if USE_SELECT_FRAME_FLG
		frameList.SetFrame<SelectFrame>();
#endif
#if USE_STAGE_SELECT_FRAME_FLG
		frameList.SetFrame<StageSelectFrame>();
#endif
#if USE_GAME_FRAME_FLG
		frameList.SetFrame<GameFrame>();
#endif
#if USE_EDIT_FRAME_FLG
		frameList.SetFrame<EditFrame>();
#endif
#if USE_SETTING_FRAME_FLG
		frameList.SetFrame<SettingFrame>();
#endif
#if USE_RESULT_FRAME_FLG
		frameList.SetFrame<ResultFrame>();
#endif
	//auto gameFrame = frameList.GetNowFrame<GameFrame>();

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
	ChD3D::WICBitmapCreatorObj().Release();

	return (int)system.GetReturnMassage()->wParam;
}
