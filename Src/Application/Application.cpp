
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"Application.h"

Application* app = nullptr;



void Application::Init(HINSTANCE hInst, int nCmdshow)
{
	windClass.RegistClass("ChGame");

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