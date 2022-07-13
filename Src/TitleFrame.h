#pragma once

class BRobot;

class Title
{
public:

	//ChBGM BGM;

	Title();

	///////////////////////////////////////////////////////////////////////////////////

	~Title();

	///////////////////////////////////////////////////////////////////////////////////

	void Init();

	void Release();

	///////////////////////////////////////////////////////////////////////////////////

	char Frame(void);

	///////////////////////////////////////////////////////////////////////////////////

private:

	void SqriptUpdate();

	void SqriptDraw();

	//XFile用定数
	const enum {TmpMap};

	//Texture用定数
	const enum { Logo, water};

	std::vector<ChPtr::Shared<std::function<void(void)>>>ScriptList;
	unsigned char Pattern = 0;

	char NextFrame = 'T';
	char celect = 0;
	char StartCheck = ChStd::True;
	unsigned long fps;
	unsigned short AniCnt = 0;

	ChPtr::Shared<ChCamera9> Camera;
	ChPtr::Shared<ChLight9> Light;

	ChPtr::Shared<BRobot> BaseRobot;

	ChStd::DataNo SENo = 0;
	//ChSkinMeshAnimationXFile SkinMesh;

	///////////////////////////////////////////////////////////////////////////////////

	// 更新処理
	void Update(void);

	///////////////////////////////////////////////////////////////////////////////////

	// 3D描画
	void Render3D(void);

	//////////////////////////////////////////////////////////////////////////////////

	// 2D描画
	void Render2D(void);

	void Script1(void);
	void Script2(void);
	void Script3(void);
	void Script4(void);
};