
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

class ShotEffectList;
class SmokeEffectList;

class GameInMessageBox;
class WeaponDataDrawUI;
class WeaponPaletteDrawUI;


struct MapObject : public ChCpp::BaseObject<wchar_t>
{
	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> model = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
	ChLMat mat;
};

class MapCollider :public ChCpp::BaseComponent
{
public:

	void SetMatrix(ChLMat& _mat)
	{
		collider.SetMatrix(_mat);
	}

	void SetPolygon(ChCpp::FrameObject<wchar_t>& _frame)
	{
		collider.SetModel(_frame);
	}

	ChCpp::PolygonCollider<wchar_t>& GetCollider()
	{
		return collider;
	}

private:

	ChCpp::PolygonCollider<wchar_t> collider;
	
};

class GameFrame:public ChCpp::BaseFrame
{
private:

	enum class SelectMechaType
	{
		Up,Down,None
	};

public:

	GameFrame() {};

	~GameFrame() { Release(); };

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void InitScriptFunction();

	void SetHitMap(ChPtr::Shared<MapObject> _map);

	void LoadScript(const std::wstring& _text);

	void LoadStage(std::wstring& _stageScriptName);

public:

	void AddMecha(const std::wstring& _text);

	void AddField(const std::wstring& _text);

	void AddSkyObject(const std::wstring& _text);

	void AddBGM(const std::wstring& _text);

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

	static float GetCenterProjectionWidth();

	static float GetCenterProjectionHeight();

public:

	inline bool IsFriendryFireFlg() { return isFrendryFireFlg; }

public:

	void BreakMecha(BaseMecha* _mecha);

	inline void PlayerLost() { playerCount--; }

private:

	size_t GettargetNum(std::vector<std::wstring>& _args);

	void UpdateFunction();

	void UpdatePlayerLostKeys();

	void DrawFunctionBegin();

	void DrawFunction();

	bool allControllFlg = true;

	void MissionStartAnimation();
	bool missionStartAnimationFlg = false;

	void SetAnimation(const std::wstring& _animationFilePath);
	void Aniamtion();
	bool animationFlg = false;

	void Success();
	bool successFlg = false;
	long successPauseCount = -1;

	void Failed();
	bool failedFlg = false;

	bool initFlg = false;

	ChLMat proMat;
	ChLMat viewMat;

	enum class DrawEffect:unsigned short
	{
		Effect,Obj
	};

	ChCpp::ObjectList mapList;

	std::vector<ChPtr::Shared<MapObject>> hitMapList;

	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> skySphere = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();

	std::map<std::wstring,ChPtr::Shared<ChD3D::AudioObject>>audios;
	std::wstring nowPlayAudio = L"";

	bool startFlg = false;
	
	ChCpp::ObjectList mechaList;
	std::vector<ChPtr::Weak<BaseMecha>>mechas;
	std::map<size_t, size_t>mechaPartyCounter;
	unsigned char playerParty = 0;
	size_t mechaView = 0;
	size_t playerCount = 0;
	bool isUseChangeCameraFlg = false;

	ChCpp::ObjectList bulletList;

	ChPtr::Shared<BaseMecha> drawMecha = nullptr;
	ChD3D11::Shader::CircleCullingSprite11 gageDrawer;
	ChD3D11::Shader::BaseDrawSprite11 uiDrawer;
	ChD3D11::Shader::BasicHighlightShader11 lightBloomeDrawer;
	ChD3D11::Sprite11 uiSprite;
	ImageSprite hitIcon;

	ChD3D11::Texture11 centerUITexture;
	ChD3D11::Texture11 enelgyUITexture;
	ChD3D11::Texture11 receveDamageUITexture;
	ChD3D11::Sprite11 centerUISprite;

	ChD3D11::Texture11 targetLockOnImage;
	ChD3D11::Texture11 targetLockOffImage;
	ChD3D11::Sprite11 targetLockOnOffPanelSprite;

	bool gameEndFlg = false;

	ChPtr::Shared<ShotEffectList> shotEffectList = nullptr;
	ChPtr::Shared<SmokeEffectList> smokeEffectList = nullptr;

	ChPtr::Shared<EffectObjectShader> cloudEffectShader = nullptr;
	ChPtr::Shared<EffectObjectShader> waterSplashEffectShader = nullptr;
	ChPtr::Shared<EffectObjectShader> fireShader = nullptr;

	ChPtr::Shared<EffectSpriteShader> enemyMarkerShader = nullptr;

	ChD3D11::RenderTarget11 rt2D;
	ChD3D11::RenderTarget11 rt3D;
	ChD3D11::RenderTarget11 rtHighLightMap;
	ChD3D11::RenderTarget11 rtObjectLooker;
	ChD3D11::Texture11 fadeOutTexture;
	ChD3D11::DepthStencilTexture11 dsTex;

	ChD3D11::CB::CBLight11 light;
	ChD3D11::Shader::BaseDrawMesh11<wchar_t> meshDrawer;
	bool isFrendryFireFlg = false;

	ChPtr::Shared<GameInMessageBox> messageBox = nullptr;
	ChPtr::Shared<WeaponDataDrawUI> weaponDataDrawer = nullptr;
	ChPtr::Shared<WeaponPaletteDrawUI> weaponPaletteDrawer = nullptr;

	D3D11_BLEND_DESC uiBlending;

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