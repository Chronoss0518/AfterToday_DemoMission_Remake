#ifndef _BaseRobot
#define _BaseRobot

class MechaPartsObject;
class CameraObject;
class WeaponObject;
class FunctionComponent;

class PhysicsMachine;

class BaseMecha :public ChCpp::BaseObject
{
public://Inner Struct Class Enum//

	friend FunctionComponent;

	BaseMecha();

	~BaseMecha();

	enum class InputName : unsigned char
	{
		Front, Back, Left, Right, Jump,
		LeftRotation, RightRotation,
		CameraUpRotation, CameraDownRotation, CameraLeftRotation, CameraRightRotation,
		Avo, FrontAvo, BackAvo, LeftAvo, RightAvo, UpAvo, DownAvo,
		Boost, FrontBoost, BackBoost, LeftBoost, RightBoost, UpBoost, DownBoost,
		Attack, RAttack, LAttack, 
		AttackTypeChange, RATChange, LATChange,
		Reload, RReload, LReload, CReload,
		WeaponChange, LWChange, RWChange,
		MagnificationUp, MagnificationDown,
		OverBoost, Release, OnSubKey, SetCameraCenter,
		MapOnOff ,None
	};

	struct WeaponData
	{
		unsigned long useWeaponNo = 0;
		std::vector<ChPtr::Shared<MechaPartsObject>>weapon;
	};

	enum class PartsPosNames : unsigned char
	{
		Body ,Head, Foot, RArm, LArm, Boost, Holder, Weapons, None
	};

public://Override Functions//

	void Release()override;

	void UpdateEnd()override;

	void Move()override;

	void Draw2D()override;

	void Draw3D()override;

public:

	void BaseMove();

public://SerializeDeserialize//

	void Deserialize(const std::string& _fileName);

	std::string Serialize();
	
public://Create Function//

	void Create(const ChVec2& _viewSize,MeshDrawer& _drawer);

public:

	void Load(ID3D11Device* _device, const std::string& _fileName);

	void LoadPartsList(ID3D11Device* _device, const ChCpp::TextObject& _textObject);

	void Save(const std::string& _fileName);

	std::string SavePartsList();

public:

	inline void AddMoveVector(const ChVec3& _moveVecAdd) { moveVec += _moveVecAdd; }

	inline void AddRotateVector(const ChVec3& _rotateVecAdd) { rotateVec += _rotateVecAdd; }

	inline void AddCamera(ChPtr::Shared<CameraObject> _camera)
	{
		cameraList.push_back(_camera); 
	}

	inline void AddLeftWeappon(ChPtr::Shared<MechaPartsObject> _weapon)
	{
		leftWeapon.weapon.push_back(_weapon); 
	}

	inline void AddRightWeappon(ChPtr::Shared<MechaPartsObject> _weapon)
	{
		rightWeapon.weapon.push_back(_weapon); 
	}

	inline void AddMechaParts(ChPtr::Shared<MechaPartsObject> _obj)
	{
		mechaParts.push_back(_obj);
	}

	inline void AddMechaPartsPos(ChPtr::Shared<ChCpp::FrameObject> _posObject, const PartsPosNames _name)
	{
		positions[ChStd::EnumCast(_name)].push_back(_posObject);
	}

	void AddMass(const float _mass) { mass += _mass; }

	void AddMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy += _maxEnelgy; }

	void AddChargeEnelgy(const unsigned long _chargeEnelgy) { chargeEnelgy += _chargeEnelgy; }

public:

	void SubMass(const float _mass) { mass -= _mass; }

	void SubMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy -= _maxEnelgy; }

	void SubChargeEnelgy(const unsigned long _chargeEnelgy) { chargeEnelgy -= _chargeEnelgy; }

public://Set Function//

	void SetTeam(const unsigned char _team) { team = _team; }

	void SetPosition(const ChVec3& _pos) { pos = _pos; }

	void SetRotation(const ChVec3& _rot) { rot = _rot; }

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

	inline ChVec3 GetPosition() { return pos; }

	inline ChVec3 GetRotation() { return rot; }

	inline static BaseMecha& GetCameraFromMecha()
	{
		return *GetList()[GetMechaCamNo()];
	}

	inline std::vector<BaseMecha*>& GetMechaList()
	{
		return GetList();
	}

	inline std::vector<ChPtr::Shared<MechaPartsObject>> GetMechaPartsList()
	{
		return mechaParts;
	}

	inline std::vector<ChPtr::Shared<ChCpp::FrameObject>> GetMechaPartsPosList(PartsPosNames _name)
	{
		
		if (_name == PartsPosNames::None)return std::vector<ChPtr::Shared<ChCpp::FrameObject>>();
		
		return positions[ChStd::EnumCast(_name)];
	}

	inline unsigned long GetNowEnelgy() { return nowEnelgy; }

	inline unsigned long GetMechaNo() { return mechasNo; }

protected:

	inline static long& GetMechaCamNo()
	{
		static long mechaCamNo = 0;
		return mechaCamNo;
	}

	inline static std::vector<BaseMecha*>& GetList()
	{
		static std::vector<BaseMecha*>list;
		return list;
	}

	ChVec2 viewSize;

	ChVec3 pos;
	ChVec3 rot;

	ChVec3 rotateVec;
	ChVec3 moveVec;//ˆÚ“®—Í//

	ChPtr::Shared<PhysicsMachine>physics = ChPtr::Make_S<PhysicsMachine>();

	float mass = 1.0f;

	unsigned char team = 0;
	
	unsigned long mechasNo = 0;

	unsigned long maxEnelgy = 0;
	unsigned long chargeEnelgy = 0;

	unsigned long nowEnelgy = 0;

	float groundDistance = 0.0f;
	
	ChCpp::BitBool inputFlgs = ChCpp::BitBool(6);

	std::vector<ChPtr::Shared<MechaPartsObject>>mechaParts;
	std::vector<ChPtr::Shared<ChCpp::FrameObject>> positions[ChStd::EnumCast(PartsPosNames::None)];

	unsigned long useCameraNo = 0;
	std::vector<ChPtr::Shared<CameraObject>>cameraList;

	WeaponData rightWeapon;
	WeaponData leftWeapon;

	std::string mechaName = "";

	MeshDrawer* drawer = nullptr;

};

#endif