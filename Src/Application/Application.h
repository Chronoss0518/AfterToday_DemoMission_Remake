#pragma once

#ifndef	APPLICATION_THREAD_MAX_COUNT
#define	APPLICATION_THREAD_MAX_COUNT 3
#endif

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

	inline ChWin::WindObjectW& GetWindow() { return window; }

	inline ChD3D11::DirectX3D11& GetDirect3D11() { return d3dApi; }

	inline ChD3D::XAudioList& GetAudioManager() { return audioList; }

	inline ChWin::WinKeyInput& GetKeyInput() { return keyInput; }

	inline ChD3D::XInputController& GetXInputController() { return controller; }

#if USE_THREAD
	inline ChCpp::ThreadObjectList& GetThreadList() { return threadList; }
#endif

public:

#if USE_THREAD
	inline void SetCPUThreadCount() { cpuThreadListCount = 0; }

	void AddCPUThread(ChPtr::Shared<ChCpp::ThreadObject> _obj);
#endif

private:

	ChWin::WindObjectW window;
	ChWin::WindClassObjectW windClass;

	ChD3D11::DirectX3D11 d3dApi;
	ChCpp::FrameList frameList;

	ChD3D::XAudioList audioList;

	ChWin::WinKeyInput keyInput;
	ChD3D::XInputController controller;

	ChCpp::FPSController fpsController;

#if USE_THREAD
	ChCpp::ThreadObjectList threadList;
	ChCpp::ThreadObjectList cpuThreadList[APPLICATION_THREAD_MAX_COUNT];
	unsigned long cpuThreadListCount = 0;
#endif

	bool inUpdateFlg = false;

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