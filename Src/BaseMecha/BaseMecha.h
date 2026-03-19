#ifndef _BaseRobot
#define _BaseRobot

class GameFrame;
class MechaPartsObject;
class WeaponObject;
class FunctionComponent;
class AttackObject;
class WeaponComponent;

class ControllerBase;

class PhysicsMachine;

class BaseMecha :public ChCpp::BaseObject<wchar_t>
{
public://Inner Struct Class Enum//

	friend FunctionComponent;

	BaseMecha();

	~BaseMecha();

	enum class InputName : unsigned char
	{
		Front, Back, Right, Left, Up, Down,
		FrontRight,FrontLeft,BackRight,BackLeft,
		UpRotation, DownRotation, RightRotation, LeftRotation,
		CameraUpRotation, CameraDownRotation, CameraRightRotation, CameraLeftRotation,
		Avo, FrontAvo, BackAvo, RightAvo, LeftAvo, UpAvo, DownAvo,
		Boost, FrontBoost, BackBoost, RightBoost, LeftBoost, UpBoost, DownBoost,
		Attack, RAttack, LAttack,
		AttackTypeUpChange, RATUChange, LATUChange,
		AttackTypeDownChange, RATDChange, LATDChange,
		Reload, RReload, LReload,
		WeaponUpChange, RWUChange, LWUChange,
		WeaponDownChange, RWDChange, LWDChange,
		MoveUpChange, MoveDownChange,
		ScopeMagnificationUp, ScopeMagnificationDown,
		OverBoost, Release, OnSubKey, SetCameraCenter,
		MapOnOff, UseTargetLooker, None
	};

public://Override Functions//

	void Release()override;

	void Update()override;

	void UpdateEnd()override;

	void Move()override;

	void MoveEnd()override;

	void Draw3D()override;

	void Draw2D()override;

	void DrawEnd()override;

private:

	void BaseMove();

public://SerializeDeserialize//

	void Deserialize(const std::wstring& _fileName);

	std::wstring Serialize();

public://Create Function//

	void Create(const ChVec2& _viewSize, ChD3D11::Shader::BaseDrawMesh11<wchar_t>& _drawer, GameFrame* _frame);

public:

	void Load(ID3D11Device* _device, const std::wstring& _fileName);

	void LoadPartsList(ID3D11Device* _device, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject);

	void LoadEnd();

	void Save(const std::wstring& _fileName);

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>> SavePartsList();

public:

	void AddMoveVector(const ChVec3& _moveVecAdd);

	void AddRotateVector(const ChVec3& _rotateVecAdd);

	void AddMass(const float _mass) { mass += _mass; }

	void AddAnchorData(const ChVec3& _size, const ChLMat& _drawMat);

	void AddLeftWeaponData(ChPtr::Shared<MechaPartsObject>_partsObject);

	void AddRightWeaponData(ChPtr::Shared<MechaPartsObject>_partsObject);

public:

	void SubMass(const float _mass) { mass -= _mass; }

public://Set Function//

	void SetTeamNo(const unsigned long _team);

	void SetPosition(const ChVec3& _pos);

	void SetRotation(const ChVec3& _rot);

	inline void  SetSelfViewHorizontalFlg(const bool& _flg) { isSelfViewHorizontalFlg = _flg; }

	void SetMass(const float _mass) { mass = _mass; }

	inline void SetPushFlg(const InputName _inputFlgName)
	{
		inputFlgs.SetBitTrue(ChStd::EnumCast(_inputFlgName));
	}

	void SetGroundHeight(const float _height);

	void SetHitEffectDrawFrame();

public://Get Function//

	ChVec3 GetPosition();

	ChVec3 GetRotation();

	ChLMat GetViewMat();

	ChVec3 GetDamageDir() { return damageDir; }

	inline float GetMass() { return mass; }

	inline size_t GetMechaNo() { return mechasNo; }

	size_t GetTeamNo();

	inline float GetMaxDamageGage() { return durable; }

	inline float GetDamage() { return nowDurable; }

	inline long GetHitEffectDrawFrame() { return hitEffectDrawFrame; }

	long GetHitEffectDrawStartFrame();

	inline std::wstring GetMechaName() { return mechaName; }

	size_t GetAnchorRegistNum();

	ChPtr::Shared<MechaPartsObject> GetCoreParts() { return core; }

public:

	inline bool IsBreak() { return breakFlg; }

public:

	void UpdateAnchor(size_t _no, const ChLMat& _drawMat);

public:

	void SetTestHitSize(const ChVec3& _hitSize);

	void TestBulletHit(AttackObject& _obj);

public:

	void Break();

public:

	template<class T>
	ChPtr::Shared<T> GetComponentObject()
	{
		auto&& com = GetComponent<T>();
		if (com != nullptr)
		{
			return com;
		}

		return SetComponent<T>();
	}

protected:

	ChVec2 viewSize;

	ChPtr::Shared<ControllerBase>controller = nullptr;

	ChPtr::Unique<PhysicsMachine>physics = ChPtr::Make_U<PhysicsMachine>();

	float mass = 1.0f;

	size_t mechasNo = 0;

	float groundDistance = 0.0f;

	float durable = 100.0f;
	float nowDurable = 100.0f;

	ChCpp::BitBool inputFlgs = ChCpp::BitBool(((unsigned char)InputName::None / 8) + 1);

	ChPtr::Shared<MechaPartsObject> core = nullptr;

	ChVec3 damageDir = ChVec3();

	float baseHitSize = 0.0f;

	ChCpp::SphereCollider testCollider;
	ChCpp::SphereCollider testAttackCollider;

	std::wstring mechaName = L"";

	ChD3D11::Shader::BaseDrawMesh11<wchar_t>* drawer = nullptr;
	GameFrame* frame = nullptr;

	ChVec3 centerPos;

	long hitEffectDrawFrame = -1;

	bool breakFlg = false;

	bool isSelfViewHorizontalFlg = true;
};

#endif