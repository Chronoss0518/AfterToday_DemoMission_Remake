
#ifndef _GameFrame
#define _GameFrame

class HitPosList;
class BaseRobotsList;
class CloudList;

class GameFrame:public ChCpp::BaseFrame
{
public:

	GameFrame() {};

	~GameFrame() { Release(); };

	void Init()override;

	void Release()override;

	void Update()override;

private:

	void UpdateFunction();

	void DrawFunction();

	enum class DrawEffect:ChStd::DataNo
	{
		Effect,Obj
	};

	void CamUpdate();

	//XFileóp
	const enum { Map, Sky };

	//Soundóp
	const enum{BGM};

	//PolygonBordAnimationóp
	const enum{VTop,VLeft,VBottom,VRight};

	const float MapScaling = 50.0f;
	ChVec3 MapPos;
	const float MapMove = 0.8f;

	
	CamMat;
	char CurrentPath[256];

	ChStd::Bool StartFlg = false;
	unsigned long fps;
	//OpenWorldMap *World;
	
	const ChStd::DataNo RTNo = 0;

	unsigned char Pattern = 0;
	unsigned short FOCUpDateCnt = 0;
	unsigned short FaidOutCnt = 0;
	unsigned short EnemyBreakCnt = 0;

	//ChObject
	ChPtr::Shared<HitPosList> HitPos;

	ChPtr::Shared<CloudList>Cloud;

	ChPtr::Shared<BaseRobotsList> RobotsList;
	ChVec3 PPos;

	//Viewïœêî//
	ChVec3 BaseCamPos = ChVec3(0.0f, 6.0f, -12.0f)
		, BaseCamLook = ChVec3(0.0f, 4.0f, 0.0f);

	DWORD CamRobotNo;
	ChGame::Camera Camera;
	ChD3D11::CB::CBLight11 Light;
	unsigned long TestFps;




	//LpChFileCon FileCon;

	// 3Dï`âÊ
	void Render3D(void);

	// 2Dï`âÊ
	void Render2D(void);

	//ï`âÊóp//
	ChStd::Bool DFlg;
	void Draw();

};

#endif