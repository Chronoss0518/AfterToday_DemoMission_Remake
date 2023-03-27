#ifndef _BaseRobot
#define _BaseRobot

class GameFrame;
class MechaPartsObject;
class CameraObject;
class WeaponObject;
class FunctionComponent;
class BulletObject;

class PhysicsMachine;

struct PositionObject
{
	ChPtr::Shared<ChCpp::FrameObject>positionObject = nullptr;
	MechaPartsObject* haveObject = nullptr;
};

class BaseMecha :public ChCpp::BaseObject
{
public://Inner Struct Class Enum//

	friend FunctionComponent;

	BaseMecha();

	~BaseMecha();

	enum class InputName : unsigned char
	{
		Front, Back, Right, Left, Jump,
		RightRotation, LeftRotation,
		CameraUpRotation, CameraDownRotation, CameraLeftRotation, CameraRightRotation,
		Avo, FrontAvo, BackAvo, RightAvo, LeftAvo, UpAvo, DownAvo,
		Boost, FrontBoost, BackBoost, RightBoost, LeftBoost, UpBoost, DownBoost,
		Attack, RAttack, LAttack,
		AttackTypeUpChange, RATUChange, LATUChange,
		AttackTypeDownChange, RATDChange, LATDChange,
		Reload, RReload, LReload,
		WeaponUpChange, RWUChange, LWUChange,
		WeaponDownChange, RWDChange, LWDChange,
		MagnificationUp, MagnificationDown,
		OverBoost, Release, OnSubKey, SetCameraCenter,
		MapOnOff, None
	};

	enum class PartsPosNames : unsigned char
	{
		Body, Head, Foot, RArm, LArm, Boost, Holder, Weapons, None
	};

public://Override Functions//

	void Release()override;

	void Update()override;

	void UpdateEnd()override;

	void Move()override;

	void MoveEnd()override;

	void Draw2D()override;

	void Draw3D()override;

public:

	void BaseMove();

public://SerializeDeserialize//

	void Deserialize(const std::string& _fileName);

	std::string Serialize();

public://Create Function//

	void Create(const ChVec2& _viewSize, MeshDrawer& _drawer, GameFrame* _frame);

public:

	void Load(ID3D11Device* _device, const std::string& _fileName);

	void LoadPartsList(ID3D11Device* _device, const ChCpp::TextObject& _textObject);

	void Save(const std::string& _fileName);

	std::string SavePartsList();

public:

	void AddMoveVector(const ChVec3& _moveVecAdd);

	void AddRotateVector(const ChVec3& _rotateVecAdd);

	inline void AddCamera(ChPtr::Shared<CameraObject> _camera)
	{
		cameraList.push_back(_camera);
	}

	inline void AddMechaParts(ChPtr::Shared<MechaPartsObject> _obj)
	{
		mechaParts.push_back(_obj);
	}

	inline void AddMechaPartsPos(ChPtr::Shared<PositionObject> _posObject, const PartsPosNames _name)
	{
		positions[ChStd::EnumCast(_name)].push_back(_posObject);
	}

	inline void AddViewVertical(const float& _x) { if (std::abs(viewVertical) < maxViewVertical)viewVertical += _x; }

	void AddMass(const float _mass) { mass += _mass; }

	void AddMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy += _maxEnelgy; }

	void AddChargeEnelgy(const unsigned long _chargeEnelgy) { chargeEnelgy += _chargeEnelgy; }

public:

	void SubMass(const float _mass) { mass -= _mass; }

	void SubMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy -= _maxEnelgy; }

	void SubChargeEnelgy(const unsigned long _chargeEnelgy) { chargeEnelgy -= _chargeEnelgy; }

public://Set Function//

	void SetTeamNo(const unsigned long _team);

	void SetPosition(const ChVec3& _pos);

	void SetRotation(const ChVec3& _rot);

	void SetMass(const float _mass) { mass = _mass; }

	void SetMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy = _maxEnelgy; }

	void SetChargeEnelgy(const unsigned long _chargeEnelgy) { chargeEnelgy = _chargeEnelgy; }

	inline void SetPushFlg(const InputName _inputFlgName)
	{
		inputFlgs.SetBitTrue(ChStd::EnumCast(_inputFlgName));
	}

	void SetGroundHeight(const float _height);

	void SetPartsPos(MechaPartsObject& _parts, const PartsPosNames _name, unsigned long _no);

public://Get Function//

	ChVec3 GetPosition();

	ChVec3 GetRotation();

	ChCpp::ObjectList& GetMechaList();

	inline std::vector<ChPtr::Shared<MechaPartsObject>> GetMechaPartsList()
	{
		return mechaParts;
	}

	inline std::vector<ChPtr::Shared<PositionObject>> GetMechaPartsPosList(PartsPosNames _name)
	{
		if (_name == PartsPosNames::None)return std::vector<ChPtr::Shared<PositionObject>>();

		return positions[ChStd::EnumCast(_name)];
	}

	ChLMat GetViewMat() { return viewMat; }

	ChVec3 GetViewPos();
	
	ChVec3 GetViewLookPos();

	inline float GetMass() { return mass; }

	inline unsigned long GetNowEnelgy() { return nowEnelgy; }

	inline unsigned long GetMechaNo() { return mechasNo; }

	unsigned long GetTeamNo();

	inline float GetDamage() { return durable - nowDurable <= 0 ? durable : durable - nowDurable; }

public:

	void TestBulletHit(BulletObject& _obj);

public:

	void Break();

private:

	template<class T>
	ChPtr::Shared<T> GetComponentObject()
	{
		auto&& ComList = GetComponents<T>();
		if (!ComList.empty())
		{
			return ComList[0];
		}

		return SetComponent<T>();
	}

protected:

	ChLMat viewMat;

	ChVec2 viewSize;

	ChPtr::Unique<PhysicsMachine>physics = ChPtr::Make_U<PhysicsMachine>();

	float mass = 1.0f;

	unsigned long mechasNo = 0;

	unsigned long maxEnelgy = 0;
	unsigned long chargeEnelgy = 0;

	unsigned long nowEnelgy = 0;

	float groundDistance = 0.0f;

	float durable = 100.0f;
	float nowDurable = 100.0f;

	ChCpp::BitBool inputFlgs = ChCpp::BitBool(6);

	std::vector<ChPtr::Shared<MechaPartsObject>>mechaParts;
	std::vector<ChPtr::Shared<PositionObject>> positions[ChStd::EnumCast(PartsPosNames::None)];

	unsigned long useCameraNo = 0;
	std::vector<ChPtr::Shared<CameraObject>>cameraList;

	std::string mechaName = "";

	MeshDrawer* drawer = nullptr;
	GameFrame* frame = nullptr;

	ChVec3 centerPos;
	float viewVertical = 0.0f;
	float maxViewVertical = 85.0f;

	ChStd::Bool breakFlg = false;
};

#endif