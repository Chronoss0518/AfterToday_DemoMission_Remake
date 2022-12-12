
#ifndef _GameFrame
#define _GameFrame

class BulletObject;
class BaseMecha;
class CloudList;
class GameScript;

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

	void InitScriptFunction();

	void SetHitMap(ChPtr::Shared<MapObject> _map);

	void LoadScript(const std::string& _text);

	void LoadMechas();

	void LoadBGMs();

	void LoadMaps();

	void Release()override;

	void Update()override;

public:

	void AddMecha(ChPtr::Shared<BaseMecha> _mecha);

	void AddBullet(ChPtr::Shared<BulletObject> _bullet);

public:

	ChCpp::ObjectList& GetMechaList() { return mechaList; }

	ChCpp::ObjectList& GetBulletList() { return bulletList; }

private:

	void UpdateFunction();

	void DrawFunction();

	ChMat_11 viewMat;
	ChMat_11 proMat;

	ChVec2 windSize;

	enum class DrawEffect:ChStd::DataNo
	{
		Effect,Obj
	};

	void CamUpdate();

	void TestUpdate();

	std::vector<ChPtr::Shared<MapObject>> map;

	ChPtr::Shared<ChD3D11::Mesh11> sky_sphere = ChPtr::Make_S<ChD3D11::Mesh11>();

	std::map<std::string,ChPtr::Shared<ChD3D::AudioObject>>audios;
	std::string nowPlayAudio = "";

	ChStd::Bool startFlg = false;
	
	ChCpp::ObjectList mechaList;
	std::map<unsigned char, unsigned long>mechaPartyCounter;
	unsigned char playerParty = 0;
	ChCpp::ObjectList bulletList;
	ChCpp::ObjectList mapObjects;

	ChPtr::Shared<ChD3D11::Texture11>enemyMarkerTexture = ChPtr::Make_S<ChD3D11::Texture11>();
	ChPtr::Shared<ChD3D11::Texture11>baseMarkerTexture = ChPtr::Make_S<ChD3D11::Texture11>();

	ChD3D11::RenderTarget11 enemyMarker;
	ChD3D11::RenderTarget11 baseMarker;

	ChD3D11::CB::CBLight11 light;
	MeshDrawer meshDrawer;

	// 3D•`‰æ
	void Render3D(void);

	// 2D•`‰æ
	void Render2D(void);

	ChPtr::Shared<GameScript> script = nullptr;
	std::map<std::string, unsigned long>loopPos;


	ChWin::TextBox box;

};

#endif