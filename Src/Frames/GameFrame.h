
#ifndef _GameFrame
#define _GameFrame

class HitPosList;
class BaseRobotsList;
class CloudList;
class GameScript;

struct MapObject
{
	ChD3D11::Mesh11 model;
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

	void LoadScript(const std::string& _text);

	void LoadMechas();

	void LoadBGMs();

	void LoadMaps();

	void Release()override;

	void Update()override;

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

	std::vector<ChPtr::Shared<ChD3D::AudioObject>>audios;

	ChStd::Bool startFlg = false;
	
	ChCpp::ObjectList mechaList;
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

};

#endif