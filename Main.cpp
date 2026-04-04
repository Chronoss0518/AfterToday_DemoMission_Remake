
#include"Src/BaseIncluder.h"

#include"Src/Application/Application.h"


int WINAPI WinMain(
	HINSTANCE hInst,
	HINSTANCE hPrev,
	LPSTR lpszCmdParam,
	int nCmdshow)
{

	AppIns().Init(hInst, 1);

	int res = AppIns().Update();

	AppIns().Release();

	return res;
}
