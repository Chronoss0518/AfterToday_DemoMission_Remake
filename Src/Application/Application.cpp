
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

#include"../BaseMecha/Controller/ControllerBase.h"
#include"../StageSelectFrame/StageData/StageData.h"
Application* app = nullptr;



void Application::Init(HINSTANCE hInst, int nCmdshow)
{
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

			creater.Create(&window,
				"AfterToday_DemoMission",
				windClass.GetWindClassName());

			window.SetWindProcedure(WM_DESTROY, [&](HWND _hWnd, UINT _msg, WPARAM _wPalam, LPARAM _lParam)->LRESULT {
				window.Release();
				return 0;
				});

		}
		ChWin::MsgBox msg;
		msg.ClearDisplayButtonType();
		msg.AddDisplayButtonType(ChWin::MsgBox::DisplayButtonType::YesNo);
		bool fullScreenFlg = msg.Display(window.GethWnd(), "全画面確認", "全画面表示で行いますか?") == ChWin::MsgBox::PushButtonType::Yes;

		d3dApi.Init(window.GethWnd(), fullScreenFlg, GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);

		ChD3D11::Shader11().Init(ChD3D11::D3D11API(), GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

		ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 1.0f, 1.0f));

		ChD3D::XAudioManager().Init();

		ChWin::Mouse().Init(window.GethWnd(), s_screen.w, s_screen.h);
	}

	PlayerData testData;

	testData.stageName = "stage1.chs";

	frameList.SaveData(&testData);

	ChD3D::WICBitmapCreatorObj().Init();

	frameList.SetFrame<SelectFrame>();
	frameList.SetFrame<TitleFrame>();
	frameList.SetFrame<StageSelectFrame>();
	frameList.SetFrame<EditFrame>();
	frameList.SetFrame<GameFrame>();
	auto gameFrame = frameList.GetNowFrame<GameFrame>();

}

int Application::Update()
{
	while (window.Update())
	{
		if (!ChSystem::SysManager().FPSProcess())continue;
		frameList.Update();
		ChD3D::XAudioManager().Update();
	}

	return (int)window.GetReturnMassage()->wParam;
}

void Application::Release()
{
	frameList.Release();
	ChD3D11::Shader11().Release();
	ChD3D11::D3D11API().Release();
}

static Application& getApp()
{
	if (ChPtr::NullCheck(app))
	{
		static Application baseApp;
		app = &baseApp;
	}

	return *app;
}

static void setApp(Application& _app)
{
	if (ChPtr::NullCheck(&_app))return;

	app = &_app;
}