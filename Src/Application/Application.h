#pragma once

class Application : public ChCp::Initializer
{
private:

	Application(){}

public:

	void Init(HINSTANCE hInst, int nCmdshow);

	int Update();

	void Release();

public:

	inline void SetFPS(unsigned long _fps) { fpsController.SetFPS(_fps); }

public:

	inline unsigned long GetFPS() { return fpsController.GetFPS(); }

public:

	ChWin::WindObjectW& GetWindow() { return window; }

	ChD3D11::DirectX3D11& GetDirect3D11() { return d3dApi; }

	ChD3D::XAudioList& GetAudioManager() { return audioList; }

private:

	ChWin::WindObjectW window;
	ChWin::WindClassObjectW windClass;

	ChD3D11::DirectX3D11 d3dApi;
	ChCpp::FrameList frameList;

	ChD3D::XAudioList audioList;

	ChWin::WinKeyInput keyInput;

	ChCpp::FPSController fpsController;

public:

	static inline Application& GetIns()
	{
		static Application ins;
		return ins;
	}

};

inline Application& AppIns()
{
	return Application::GetIns();
}