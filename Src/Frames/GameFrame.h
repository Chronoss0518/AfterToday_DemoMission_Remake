
#ifndef _GameFrame
#define _GameFrame

#ifndef BASE_FPS
#define BASE_FPS 60
#endif

#include"StageDataStructure.h"

class AttackObject;
class BaseMecha;
class GameScript;
class EffectObjectShader;
class EffectSpriteShader;
class WeaponComponent;

struct LookSquareValue;

class ShotEffectList;
class SmokeEffectList;

class GameInMessageBox;
class WeaponDataDrawUI;


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

	inline bool IsFriendryFireFlg() { return isFrendryFireFlg; }

public:

	void BreakMecha(BaseMecha* _mecha);

private:

	unsigned long GettargetNum(std::vector<std::string>& _args);

	void UpdateFunction();

	void DrawFunctionBegin();

	void DrawFunction();

	bool allControllFlg = true;

	void MissionStartAnimation();
	bool missionStartAnimationFlg = false;

	void SetAnimation(const std::string& _animationFilePath);
	void Aniamtion();
	bool animationFlg = false;

	void Success();
	bool successFlg = false;
	long successPauseCount = -1;

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

	ChPtr::Shared<BaseMecha> drawMecha = nullptr;
	ChD3D11::Shader::CircleCullingSprite11 gageDrawer;
	ChD3D11::Shader::BaseDrawSprite11 uiDrawer;
	ChD3D11::Shader::BasicBlurShader11 lightBloomeDrawer;
	ChD3D11::Sprite11 uiSprite;
	ImageSprite hitIcon;

	ChD3D11::Shader::BaseDrawPolygonBoard11 shotTargetDrawer;
	ChD3D11::PolygonBoard11 shotTargetBorad;
	ChD3D11::Texture11 shotTargetMarkerTex;
	ChLMat shotTargetdrawBaseMatrix;
	
	ChD3D11::Texture11 centerUITexture;
	ChD3D11::Texture11 enelgyUITexture;
	ChD3D11::Texture11 receveDamageUITexture;
	ChD3D11::Sprite11 centerUISprite;


	bool gameEndFlg = false;

	ChLMat projectionMat;

	ChPtr::Shared<ShotEffectList> shotEffectList = nullptr;
	ChPtr::Shared<SmokeEffectList> smokeEffectList = nullptr;

	ChPtr::Shared<EffectObjectShader> cloudEffectShader = nullptr;
	ChPtr::Shared<EffectObjectShader> waterSplashEffectShader = nullptr;
	ChPtr::Shared<EffectObjectShader> fireShader = nullptr;

	ChPtr::Shared<EffectSpriteShader> enemyMarkerShader = nullptr;

	ChD3D11::RenderTarget11 rt2D;
	ChD3D11::RenderTarget11 rt3D;
	ChD3D11::RenderTarget11 rtHighLightMap;
	ChD3D11::Texture11 fadeOutTexture;
	ChD3D11::DepthStencilTexture11 dsTex;

	ChD3D11::CB::CBLight11 light;
	ChD3D11::Shader::BaseDrawMesh11 meshDrawer;
	bool isFrendryFireFlg = false;

	ChPtr::Shared<GameInMessageBox> messageBox = nullptr;
	ChPtr::Shared<WeaponDataDrawUI> weaponDataDrawer = nullptr;


	ChD3D11::Sprite11 testTextureSprite;

	// 3D•`‰æ //
	void Render3D(void);

	// 2D•`‰æ //
	void Render2D(void);

	ChPtr::Shared<GameScript> script = nullptr;
	bool scriptPauseFlg = false;
	bool scriptPauseOnMessageFlg = false;
	bool endDrawKeyFlg = false;

	ChPtr::Shared<ResultStructure>resultData = nullptr;

	ChWin::TextBox box;

	bool isFrameUpdate = false;

};

#endif