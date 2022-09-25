#ifndef _BaseRobot
#define _BaseRobot

class RobParts;
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
	float mass;
	float fovy;
	float scopeFovy;
	char team;
	unsigned char mechasNo;

	MoveData move;
	
	ChCpp::BitBool inputFlgs;

	std::vector<ChPtr::Shared<Weapon>>rightWeapom;
	std::vector<ChPtr::Shared<Weapon>>leftWeapom;

	ChCpp::ObjectList partsList;

};

#endif