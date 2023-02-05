
#ifndef _GameFrame
#define _GameFrame

class BulletObject;
class BaseMecha;
class CloudList;
class GameScript;
class EffectObjectShader;

class ShotEffectList;
class SmokeEffectList;

struct MapObject
{
	ChPtr::Shared<ChD3D11::Mesh11> model = ChPtr::Make_S<ChD3D11::Mesh11>();
	ChVec3 position;
	ChVec3 rotation;
	ChVec3 scalling;
};

class GameFrame:public ChCpp::BaseFrame
{
public:

	GameFrame() {};

	~GameFrame() { Release(); };

	void Init()override;

	void Release()override;

	void Update()override;

private:

	void InitScriptFunction();

	void SetHitMap(ChPtr::Shared<MapObject> _map);

	void LoadScript(const std::string& _text);

	void LoadStage();

	void LoadMechas();

	void LoadBGMs();

	void LoadMaps();

public:

	void AddMecha(const std::string& _text);

	void AddField(const std::string& _text);

	void AddSkyObject(const std::string& _text);

	void AddBGM(const std::string& _text);

	void AddSE(const std::string& _text);

	void AddBullet(ChPtr::Shared<BulletObject> _bullet);

	void AddShotEffectObject(const ChVec3& _pos);

	void AddSmokeEffectObject(const ChVec3& _pos,const ChVec3& _moveVector);

public:

	ChCpp::ObjectList& GetMechaList() { return mechaList; }

	ChCpp::ObjectList& GetBulletList() { return bulletList; }

public:

	void BreakMecha(BaseMecha* _mecha);

private:

	void UpdateFunction();

	void DrawFunction();

	ChStd::Bool initFlg = false;

	ChMat_11 viewMat;
	ChMat_11 proMat;

	ChVec2 windSize;

	enum class DrawEffect:ChStd::DataNo
	{
		Effect,Obj
	};

	void CamUpdate();

	std::vector<ChPtr::Shared<MapObject>> map;

	ChPtr::Shared<ChD3D11::Mesh11> skySphere = ChPtr::Make_S<ChD3D11::Mesh11>();

	std::map<std::string,ChPtr::Shared<ChD3D::AudioObject>>audios;
	std::string nowPlayAudio = "";

	ChStd::Bool startFlg = false;
	
	ChCpp::ObjectList mechaList;
	std::map<unsigned char, unsigned long>mechaPartyCounter;
	unsigned char playerParty = 0;
	unsigned long mechaView = 0;
	ChCpp::ObjectList bulletList;
	ChCpp::ObjectList mapObjects;

	ChPtr::Shared<ChD3D11::Texture11>enemyMarkerTexture = ChPtr::Make_S<ChD3D11::Texture11>();
	ChPtr::Shared<ChD3D11::Texture11>baseMarkerTexture = ChPtr::Make_S<ChD3D11::Texture11>();

	ChStd::Bool gameEndFlg = false;

	ChLMat projectionMat;

	ChPtr::Shared<ShotEffectList> shotEffectList = nullptr;
	ChPtr::Shared<SmokeEffectList> smokeEffectList = nullptr;

	ChPtr::Shared<EffectObjectShader> cloudEffectShader = nullptr;
	ChPtr::Shared<EffectObjectShader> waterSplashEffectShader = nullptr;

	ChD3D11::RenderTarget11 enemyMarker;
	ChD3D11::RenderTarget11 baseMarker;

	ChD3D11::CB::CBLight11 light;
	MeshDrawer meshDrawer;

	// 3D•`‰æ
	void Render3D(void);

	// 2D•`‰æ
	void Render2D(void);

	ChPtr::Shared<GameScript> script = nullptr;

	ChWin::TextBox box;

};

#endif