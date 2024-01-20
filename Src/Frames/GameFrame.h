
#ifndef _GameFrame
#define _GameFrame

#include"StageDataStructure.h"

class AttackObject;
class BaseMecha;
class GameScript;
class EffectObjectShader;

struct LookSquareValue;

class ShotEffectList;
class SmokeEffectList;

class GameInMessageBox;

struct MapObject : public ChCpp::BaseObject
{
	ChPtr::Shared<ChD3D11::Mesh11> model = ChPtr::Make_S<ChD3D11::Mesh11>();
	ChLMat mat;
};

class MapCollider :public ChCpp::BaseComponent
{
public:

	void SetMatrix(ChLMat& _mat)
	{
		collider.SetMatrix(_mat);
	}

	void SetPolygon(ChCpp::FrameObject& _frame)
	{
		collider.SetModel(_frame);
	}

	ChCpp::PolygonCollider& GetCollider()
	{
		return collider;
	}

private:

	ChCpp::PolygonCollider collider;
	
};

class GameFrame:public ChCpp::BaseFrame
{
public:

	GameFrame() {};

	~GameFrame() { Release(); };

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void StartFrame();

	void InitScriptFunction();

	void SetHitMap(ChPtr::Shared<MapObject> _map);

	void LoadScript(const std::string& _text);

	void LoadStage(std::string& _stageScriptName);

public:

	void AddMecha(const std::string& _text);

	void AddField(const std::string& _text);

	void AddSkyObject(const std::string& _text);

	void AddBGM(const std::string& _text);

	void AddBullet(ChPtr::Shared<AttackObject> _bullet);

	void AddShotEffectObject(const ChVec3& _pos);

	void AddSmokeEffectObject(const ChVec3& _pos,const ChVec3& _moveVector);

	void AddSmokeEffectObject(const ChVec3& _pos,const ChVec3& _moveVector, const float _initDispersalpower);

	void AddSmokeEffectObject(const ChVec3& _pos,const ChVec3& _moveVector, const float _initDispersalpower, const float _initAlphaPow);

public:

	ChCpp::ObjectList& GetMechaList() { return mechaList; }

	std::vector<ChPtr::Weak<BaseMecha>>& GetMechas() { return mechas; }

	ChCpp::ObjectList& GetBulletList() { return bulletList; }

	ChCpp::ObjectList& GetMapList() { return mapList; }

	const std::vector<ChPtr::Shared<MapObject>>& GetHitMapList() { return hitMapList; }

	std::vector<ChPtr::Shared<LookSquareValue>> GetLookSquareValuesFromMap(const ChLMat& _viewMatrix, const ChLMat& _projectionMatrix);

	std::vector<ChPtr::Shared<LookSquareValue>> GetLookSquareValuesFromMap(const ChLMat& _vpMatrix);

public:

	void BreakMecha(BaseMecha* _mecha);

private:

	unsigned long GettargetNum(std::vector<std::string>& _args);

	void UpdateFunction();

	void DrawFunction();

	bool allControllFlg = true;

	void MissionStartAnimation();
	bool missionStartAnimationFlg = false;

	void SetAnimation(const std::string& _animationFilePath);
	void Aniamtion();
	bool animationFlg = false;

	void Success();
	bool successFlg = false;

	void Failed();
	bool failedFlg = false;

	bool initFlg = false;

	ChMat_11 viewMat;
	ChMat_11 proMat;

	enum class DrawEffect:unsigned short
	{
		Effect,Obj
	};

	void CamUpdate();

	ChCpp::ObjectList mapList;

	std::vector<ChPtr::Shared<MapObject>> hitMapList;

	ChPtr::Shared<ChD3D11::Mesh11> skySphere = ChPtr::Make_S<ChD3D11::Mesh11>();

	std::map<std::string,ChPtr::Shared<ChD3D::AudioObject>>audios;
	std::string nowPlayAudio = "";

	bool startFlg = false;
	
	ChCpp::ObjectList mechaList;
	std::vector<ChPtr::Weak<BaseMecha>>mechas;
	std::map<unsigned long, unsigned long>mechaPartyCounter;
	unsigned char playerParty = 0;
	unsigned long mechaView = 0;
	ChCpp::ObjectList bulletList;
	ChCpp::ObjectList mapObjects;

	ChPtr::Shared<BaseMecha>drawMecha = nullptr;
	ChD3D11::Shader::CircleCullingSprite11 gageDrawer;
	ChD3D11::Shader::BaseDrawSprite11 uiDrawer;

	ChPtr::Shared<ChD3D11::Texture11> centerUITexture = ChPtr::Make_S<ChD3D11::Texture11>();
	ChPtr::Shared<ChD3D11::Texture11> enelgyUITexture = ChPtr::Make_S<ChD3D11::Texture11>();
	ChPtr::Shared<ChD3D11::Texture11> receveDamageUITexture = ChPtr::Make_S<ChD3D11::Texture11>();
	ChD3D11::Sprite11 centerUISprite;

	ChPtr::Shared<ChD3D11::Texture11>enemyMarkerTexture = ChPtr::Make_S<ChD3D11::Texture11>();
	bool gameEndFlg = false;

	ChLMat projectionMat;

	ChPtr::Shared<ShotEffectList> shotEffectList = nullptr;
	ChPtr::Shared<SmokeEffectList> smokeEffectList = nullptr;

	ChPtr::Shared<EffectObjectShader> cloudEffectShader = nullptr;
	ChPtr::Shared<EffectObjectShader> waterSplashEffectShader = nullptr;
	ChPtr::Shared<EffectObjectShader> enemyMarkerShader = nullptr;

	ChD3D11::RenderTarget11 enemyMarker;
	ChD3D11::RenderTarget11 baseMarker;

	ChD3D11::CB::CBLight11 light;
	MeshDrawer meshDrawer;

	ChPtr::Shared<GameInMessageBox> messageBox = nullptr;

	// 3D�`�� //
	void Render3D(void);

	// 2D�`�� //
	void Render2D(void);

	ChPtr::Shared<GameScript> script = nullptr;
	bool scriptPauseFlg = false;

	ChWin::TextBox box;

	bool isFrameUpdate = false;

};

#endif