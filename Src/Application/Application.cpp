
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"Application.h"

#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaParts.h"

#include"../Frames/GameFrame.h"
#include"../Frames/TitleFrame.h"
#include"../Frames/SelectFrame.h"
#include"../Frames/EditFrame.h"
#include"../Frames/StageSelectFrame.h"
#include"../Frames/ResultFrame.h"

#include"../BaseMecha/Controller/ControllerBase.h"
#include"../StageSelectFrame/StageData/StageData.h"

#define TEST_STAGE_NAME L"stage1.chs"

void Application::Init(HINSTANCE hInst, int nCmdshow)
{
	if (IsInit())return;
	if (inUpdateFlg)return;

	windClass.RegistClass(L"ChGame-MX-64");

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
			creater.Create(window,
				//L"AfterToday_DemoMission",
				L"MechanizedWar TestProject",
				windClass.GetWindClassName());

			window.SetWindProcedure(WM_DESTROY, [&](HWND _hWnd, UINT _msg, WPARAM _wPalam, LPARAM _lParam)->LRESULT {
				window.Release();
				return 0;
				});

		}
		ChWin::MsgBox msg;
		msg.ClearDisplayButtonType();
		msg.AddDisplayButtonType(ChWin::MsgBox::DisplayButtonType::YesNo);
		bool fullScreenFlg = msg.DisplayW(window.GethWnd(), L"全画面確認", L"全画面表示で行いますか?") == ChWin::MsgBox::PushButtonType::Yes;

		d3dApi.Init(window.GethWnd(), fullScreenFlg, GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);

		ChD3D11::Shader11().Init(d3dApi, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

		ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 1.0f, 1.0f));

		audioList.Init();

		ChWin::Mouse().Init(window);

		controller.Init();
	}

	ChD3D::WICBitmapCreatorObj().Init();

	auto testData = ChPtr::Make_S<StageDataStructure>();

	testData->stageScriptPath = TEST_STAGE_NAME;

	frameList.SetSendData(testData);

	auto playerData = ChPtr::Make_S<PlayerData>();

	frameList.SaveData(playerData);

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

	fpsController.SetFPS(BASE_FPS);

	SetInitFlg(true);
}

int Application::Update()
{
	if (!IsInit())return 0;
	if (inUpdateFlg)return 0;

	inUpdateFlg = true;

	while (window.Update())
	{
		keyInput.Update();
		audioList.Update();
		if (!fpsController.Update(timeGetTime()))continue;
		controller.Update();

		if (keyInput.IsPushKeyNoHold(VK_ESCAPE))
		{
			PostQuitMessage(0);
			continue;
		}

		frameList.Update();
		keyInput.SetAllFlgDown();
	}
	inUpdateFlg = false;

	return (int)window.GetReturnMassage()->wParam;
}

void Application::Release()
{
	if (!IsInit())return;
	if (inUpdateFlg)return;

	frameList.Release();
	audioList.Release();
	controller.Release();

	ChD3D11::Shader11().Release();
	ChD3D11::D3D11API().Release();

	window.Release();
	windClass.Release();

	SetInitFlg(false);
}
