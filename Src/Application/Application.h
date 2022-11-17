#pragma once

class Application
{

public:

	void Init(HINSTANCE hInst, int nCmdshow);

public:

private:

	ChWin::WindObject window;
	ChWin::WindClassObject windClass;

	

	ChD3D11::DirectX3D11 d3dApi;

	ChCpp::FrameList frameList;

};

static Application& getApp();

static void setApp(Application& _app);