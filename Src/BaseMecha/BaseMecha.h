#ifndef _BaseRobot
#define _BaseRobot

class MechaPartsObject;
class CameraObject;
class WeaponObject;
class MoveComponent;


class BaseMecha :public ChCpp::BaseObject
{
public://Inner Struct Class Enum//

	friend MoveComponent;

	BaseMecha();

	~BaseMecha();

	enum class InputName : unsigned char
	{
		Front, Back, Left, Right, Jump,
		LeftRotation, RightRotation,
		CameraUpRotation, CameraDownRotation, CameraLeftRotation, CameraRightRotation,
		Avo, FrontAvo, BackAvo, LeftAvo, RightAvo, UpAvo, DownAvo,
		Boost, FrontBoost, BackBoost, LeftBoost, RightBoost, UpBoost, DownBoost,
		Attack, RAttack, LAttack, MSChange,
		Reload, RReload, LReload,
		WeaponChange, LWChange, RWChange,
		MagnificationUp, MagnificationDown,
		OverBoost, Release, OnSubKey, SetCameraCenter,
		MapOnOff ,None
	};

	struct WeaponData
	{
		unsigned long useWeaponNo = 0;
		std::vector<ChPtr::Shared<WeaponObject>>weapon;
	};

	enum class PartsPosNames : unsigned char
	{
		Head, RArm, LArm, Foot, Boost, RWeapons, LWeapons, None
	};

public://Override Functions//

	void Release()override;

	void UpdateEnd()override;

	void Move()override;

	void Draw2D()override;

	void Draw3D()override;

public:

	void NormalMoveUpdate(unsigned char _inputName, float _movePow,const ChLMat& _nowPosMatrix);

	void BaseMove();

public://SerializeDeserialize//

	void Deserialize(const std::string& _fileName);

	std::string Serialize();
	
public://Create Function//

	void Create(const ChVec2& _viewSize,MeshDrawer& _drawer);

public:

	void Load(ID3D11Device* _device, const std::string& _fileName);

	void LoadParts(ID3D11Device* _device, const std::string& _fileName);

	void Save(const std::string& _fileName);

public:

	inline void AddMoveVector(const ChVec3& _moveVecAdd) { moveVec += _moveVecAdd; }

	inline void AddRotateVector(const ChVec3& _rotateVecAdd) { rotateVec += _rotateVecAdd; }

	inline void AddCamera(ChPtr::Shared<CameraObject> _camera)
	{
		cameraList.push_back(_camera); 
	}

	inline void AddLeftWeappon(ChPtr::Shared<WeaponObject> _weapon)
	{
		leftWeapom.weapon.push_back(_weapon); 
	}

	inline void AddRightWeappon(ChPtr::Shared<WeaponObject> _weapon)
	{
		rightWeapon.weapon.push_back(_weapon); 
	}

	inline void AddMechaParts(ChPtr::Shared<MechaPartsObject> _obj)
	{
		mechaParts.push_back(_obj);
	}

	void AddMass(const float _mass) { mass += _mass; }

	void AddMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy += _maxEnelgy; }

	void AddChargeEnelgy(const unsigned long _chargeEnelgy) { chargeEnelgy += _chargeEnelgy; }

	inline bool AddWeaponPos(ChPtr::Shared<ChCpp::FrameObject> _posObject, PartsPosNames _name)
	{
		if (_name != PartsPosNames::LWeapons && _name != PartsPosNames::RWeapons)return false;
		unsigned long num = ChStd::EnumCast(_name) - ChStd::EnumCast(PartsPosNames::RWeapons);
		weaponsPoss[num].push_back(_posObject);
		return true;
	}


public:

	void SubMass(const float _mass) { mass -= _mass; }

	void SubMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy -= _maxEnelgy; }

	void SubChargeEnelgy(const unsigned long _chargeEnelgy) { chargeEnelgy -= _chargeEnelgy; }

public://Set Function//

	void SetTeam(const unsigned char _team) { team = _team; }

	void SetPosition(const ChVec3& _pos) { pos = _pos; }

	void SetRotation(const ChVec3& _rot) { rot = _rot; }

	void SetMass(const float _mass) { mass = _mass; }

	void SetMovePow(const float _movePow) { movePow = _movePow; }
	
	void SetRotatePow(const float _rotatePow) { rotatePow = _rotatePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

	void SetMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy = _maxEnelgy; }

	void SetChargeEnelgy(const unsigned long _chargeEnelgy) { chargeEnelgy = _chargeEnelgy; }

	inline void SetPartsPos(ChPtr::Shared<ChCpp::FrameObject> _posObject,PartsPosNames _name)
	{
		if (_name == PartsPosNames::None)return;
		if (AddWeaponPos(_posObject, _name))return;

		positions[ChStd::EnumCast(_name)] = _posObject; 
	}

	inline void SetPushFlg(const InputName _inputFlgName)
	{
		inputFlgs.SetBitTrue(ChStd::EnumCast(_inputFlgName));
	}

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

	inline ChPtr::Shared<ChCpp::FrameObject> GetWeaponPos(PartsPosNames _name)
	{
		if (_name != PartsPosNames::RWeapons && _name != PartsPosNames::LWeapons)return nullptr;
		return weaponsPoss[ChStd::EnumCast(_name)][0];
	}

	inline ChPtr::Shared<ChCpp::FrameObject> GetPartsPos(PartsPosNames _name)
	{
		if (_name == PartsPosNames::None)return nullptr;
		ChPtr::Shared<ChCpp::FrameObject> res = GetWeaponPos(_name);
		if (res == nullptr)
		{
			res = positions[ChStd::EnumCast(_name)];
		}
		return res;
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

	inline ChVec3 GetDirectionVector(unsigned long _num)
	{
		if (_num >= 6)return ChVec3();
		static ChVec3 direction[6]
		{
			ChVec3(0.0f,0.0f,1.0f),
			ChVec3(0.0f,0.0f,-1.0f),
			ChVec3(-1.0f,0.0f,0.0f),
			ChVec3(1.0f,0.0f,0.0f),
			ChVec3(0.0f,1.0f,0.0f),
			ChVec3(0.0f,-1.0f,0.0f)
		};

		return direction[_num];
	}

	ChVec2 viewSize;

	ChVec3 pos;
	ChVec3 rot;

	ChVec3 rotateVec;
	ChVec3 moveVec;//ˆÚ“®—Í//

	float movePow = 0.0f;
	float jumpPow = 0.0f;
	float rotatePow = 0.0f;

	float mass = 1.0f;

	unsigned char team = 0;
	
	unsigned long mechasNo = 0;

	unsigned long maxEnelgy = 0;
	unsigned long chargeEnelgy = 0;

	unsigned long nowEnelgy = 0;

	float groundDistance = 0.0f;
	
	ChCpp::BitBool inputFlgs = ChCpp::BitBool(6);

	std::vector<ChPtr::Shared<MechaPartsObject>>mechaParts;

	ChPtr::Shared<ChCpp::FrameObject> positions[ChStd::EnumCast(PartsPosNames::None)]{nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

	std::vector<ChPtr::Shared<ChCpp::FrameObject>>weaponsPoss[2];

	unsigned long useCameraNo = 0;
	std::vector<ChPtr::Shared<CameraObject>>cameraList;

	WeaponData rightWeapon;
	WeaponData leftWeapom;

	std::string mechaName = "";

	MeshDrawer* drawer = nullptr;

};

#endif