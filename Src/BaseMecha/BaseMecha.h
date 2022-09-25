#ifndef _BaseRobot
#define _BaseRobot

class MechaParts;

class Scope;
class Weapon;

class BaseMecha :public ChCpp::BaseObject
{
public://Inner Struct Class Enum//

	BaseMecha();

	~BaseMecha();

	//à⁄ìÆèÓïÒ//
	struct MoveData
	{
		ChVec3 moveV;//à⁄ìÆó //
		ChVec3 tmpRot;//âºÇÃâÒì]ó //
		float movePow;//à⁄ìÆóÕ//
		
	};

	enum class InputName :ChStd::DataNo
	{
		Up, Down, Left, Right, Jump,
		Avo, RAttack, LAttack, MSChange, 
		MagnificationUp, MagnificationDown,
		OverBoost, Release
	};

public://Override Functions//

	void Release()override;

	void Update()override;

	void Move()override;

	void Draw2D()override;

	void Draw3D()override;

public://SerializeDeserialize//

	void Deserialize(const std::string& _desirialize);

	std::string Serialize();
	
public://Create Function//

	void Create(ID3D11Device* _device,unsigned long _w,unsigned long _h);

public://Set Function//

	void SetTeam(const unsigned char _team) { team = _team; }

	void SetPosition(const ChVec3& _pos) { pos = _pos; }

	void SetRotation(const ChVec3& _rot) { rot = _rot; }

	void SetMass(const float _mass) { mass = _mass; }

	void SetFovy(const float _fovy) { fovy = _fovy; }

	void SetMovePower(const float _movePow) { movePow = _movePow; }

	void SetBoostPower(const float _boostPow) { boostPow = _boostPow; }

	void SetAvoidancePower(const float _avoidPow) { avoidPow = _avoidPow; }

	void SetMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy = _maxEnelgy; }

	void SetChargeEnelgy(const unsigned long _chargeEnelgy) { chargeEnelgy = _chargeEnelgy; }

public://Get Function//

	static BaseMecha& GetCameraFromMecha()
	{
		return *GetList()[GetMechaCamNo()];
	}

	inline ChVec3 GetPosition() { return pos; }

	inline ChVec3 GetRotation() { return rot; }

	inline float GetFovy() { return fovy; }

	inline float GetScopeFovy() { return scopeFovy; }

protected:

	long mechaNo = 0;

	static long& GetMechaCamNo()
	{
		static long mechaCamNo = 0;
		return mechaCamNo;
	}

	static std::vector<BaseMecha*>& GetList()
	{
		static std::vector<BaseMecha*>list;
		return list;
	}

	ChVec3 pos;
	ChVec3 rot;
	float mass = 1.0f;
	float fovy = 60.0f;
	float movePow = 0.0f;
	float boostPow = 0.0f;
	float avoidPow = 0.0f;

	float scopeFovy = fovy;
	unsigned char team = 0;
	unsigned char mechasNo = 0;
	unsigned long maxEnelgy = 0;
	unsigned long chargeEnelgy = 0;
	unsigned long nowEnelgy = 0;

	MoveData move;
	
	ChCpp::BitBool inputFlgs;

	std::vector<ChPtr::Shared<Scope>>scopeList;
	std::vector<ChPtr::Shared<Weapon>>rightWeapom;
	std::vector<ChPtr::Shared<Weapon>>leftWeapom;

	ChCpp::ObjectList partsList;

};

#endif