
#ifndef _GameFrame
#define _GameFrame

class HitPosList;
class BaseRobotsList;
class CloudList;

class Game:public ChCpp::BaseFrame
{
public:

	Game() {};

	~Game() { Release(); };

	void Init();

	void Release();

	char Frame(void);

private:

	void SqriptUpdate();

	void SqriptDraw();

	enum class DrawEffect:ChStd::DataNo{Effect,Obj};

	void CamUpdate();

	//XFile�p
	const enum { Map, Sky };

	//Sound�p
	const enum{BGM};

	//PolygonBordAnimation�p
	const enum{VTop,VLeft,VBottom,VRight};

	const float MapScaling = 50.0f;
	ChVec3_11 MapPos;
	const float MapMove = 0.8f;

	ChMat_11 CamMat;
	char CurrentPath[256];

	char NextFrame = 'G';
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
	ChVec3_11 PPos;

	//View�ϐ�//
	ChVec3_11 BaseCamPos = ChVec3_11(0.0f, 6.0f, -12.0f)
		, BaseCamLook = ChVec3_11(0.0f, 4.0f, 0.0f);

	DWORD CamRobotNo;
	ChPtr::Shared<ChCamera9>Camera;
	ChPtr::Shared<ChLight9>Light;
	unsigned long TestFps;


	//LpChFileCon FileCon;

	// �X�V����
	void Update(void);

	// 3D�`��
	void Render3D(void);

	// 2D�`��
	void Render2D(void);

	//�`��p//
	ChStd::Bool DFlg;
	void Draw();

};

#endif