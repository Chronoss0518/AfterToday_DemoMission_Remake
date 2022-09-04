#ifndef _BaseRobot
#define _BaseRobot

class HitPosList;
class BaseRobotsList;
class RobParts;
class RobPartsList;

class BRobot :public std::enable_shared_from_this<BRobot>
{
public:

	//RobotData//
	struct BaseData
	{
		ChVec3 Pos;
		ChVec4 Rot;
		char Team;
		unsigned char RobotsNo;
		ChStd::Bool EnemyFlg;
	};

	//位置情報//
	struct MoveData
	{
		ChVec3 MoveV;//移動量//
		ChVec4 tmpRot;//仮の回転量//
		const float AvoDen = 0.8f;//回避時の減速する割合//
		float AvoMax = 5.0f;//回避時の最大回避速度//
		float AvoV = 0.0f;//回避時の速度//
		const float AvoMin = 0.0005f;//回避時の最低速度//
		ChVec3 AvoDir;//回避する向き//
	};

	enum class KeyName :ChStd::DataNo
	{
		Up, Down, Left, Right, Jump
		, UAvo, JAvo, RAvo, LAvo
		, MAttack, SAttack
		, OverBoost
	};

	virtual void Init(unsigned short _PartsNum) {};

	virtual ~BRobot();

	inline void SetPos(const ChVec3& _Pos) { BaseD.Pos = _Pos; }

	void SetHitPosList(const ChPtr::Shared<HitPosList> _HitPosList)
	{
		WpHitPosList = _HitPosList;
	}

	void SetRobotsList(const ChPtr::Shared<BaseRobotsList> _RobotsList)
	{
		WpBRobotsList = _RobotsList;
	}

	inline void SharedDamageData(ChPtr::Shared<Damage> _D)
	{
		if (_D == nullptr)return;
		if (DData != nullptr)return;
		DData = _D;
	}

	inline void SetDData(
		const ChVec3* _Pos)
	{
		if (DData == nullptr)return;
		ChStd::Bool DFlg = false;
		if (HP < 1)DFlg = true;
		DData->SetDPos(_Pos, DFlg);
	}

	inline const ChVec3 GetPos() { return BaseD.Pos; }

	inline ChStd::Bool GetIsKey(const ChStd::DataNo _Key)
	{
		return ButtonFlg.GetBitFlg(_Key);
	}

	inline const ChPtr::Shared<RobPartsList>GetRobParts()
	{
		return RobParts;
	}

	inline const ChLMat GetMat() { return Mat; }

	inline const ChLMat GetDrawMat() { return DrawMat; }

	inline float GetPraHp() { return HP > 0 ? (float)(HP / MaxHp) : 0.0f; }

	inline const MoveData& GetMoveData()
	{
		return MoveD;
	}

	//inline float GetPraEnelgy() { return (float)(Enelgy / MaxEnelgy); }

	void IsDamage(const short _D);

	inline ChStd::Bool IsEnemy() { return BaseD.EnemyFlg; }

	inline ChStd::Bool IsDorpOutFlg()
	{
		if (DData == nullptr)return false;
		if (HP > 0 || !DData->GetDDataEmpty())return true;
		return false;
	}

	inline ChStd::Bool IsLiveFlg()
	{
		if (HP > 0)return true;
		return false;
	}

	void Draw(const ChStd::Bool _EffectFlg);

	virtual void Input() {};

	void Move();
	
	BRobot& operator= (BRobot& _cm)
	{
		this->Mat = _cm.GetMat();
		this->BaseD.EnemyFlg = _cm.IsEnemy();
		return *this;
	}

protected:

	void Attack(const ChPtr::Shared<WeapData> _Weap);

	void InputKeyData();

	void InputCPUData();

	virtual void AIMoveKeyInput();

	virtual void AIMoveMouInput();

	//移動キー保持//
	static const unsigned char MoveKeyCnt = 2;
	char MoveKey[MoveKeyCnt];
	char KeyCode[5] = { 'W','A','S','D','T' };

	//inputData//

	static const unsigned char KeyCount = 12;
	ChCpp::BitBool ButtonFlg = ChCpp::BitBool(2);

	//行動を変える割合(最大100%)//
	unsigned char SpNow = 95;


	void CurrsolMove(const float _MoveX, const float _MoveY);

	//Aim速度//
	float AAS = 50.0f;

	//AimDownSight以降速度//
	float ADS = 10.0f;

	//AttackViewLen//
	float ViweLen = 100.0f;

	//EnemyBattleLen
	float BLen = 5.0f;

	ChStd::Bool SetTargetPos();
	ChLMat TargetPos;
	//ターゲットのウィンドウ上の位置から中央までの距離//
	float TWindOnPosLen = 0.0f;

	virtual void BInit(const unsigned short _PartsNum);

	void Change();

	void InputToMove();

	void PartsSet();

	void CreatLook();

	void CreateShotData(const ChStd::DataNo _tmpNo);

	void SetRobParts();

	float MoveS = 0.5f;
	float RaderLen = 300.0f;

	float HP = 100;
	float MaxHp;
	//float Enelgy = 10000;
	//float MaxEnelgy;

	unsigned short AddDamage = 0;


	ChVec4 MoveRot;

	float Sens = 1.0f;
	const float MouMoveP = 30.0f;

	ChStd::Bool BoostFlg = false;
	float BoostV = 1.0f;
	//short BoostEnelgy = 100;

	BaseData BaseD;
	MoveData MoveD;

	ChPtr::Shared<RobPartsList>RobParts;


	const float MaxCamMoveSpeed = 10.0f;

	//Damage用//
	ChPtr::Shared<Damage> DData = nullptr;

	//DrawData//
	ChVec4 Col;
	ChLMat Mat;
	ChLMat DrawMat;

	//CamData//

	void CamUpdate();


	ChLMat CamMoveVec;

	ChVec3 BaseCamPos = ChVec3(0.0f, 0.0f, 5.0f)
		, BaseCamLook = ChVec3(0.0f, 0.0f, 6.0f);

	void SetCamMat();

	void SetLook();

	ChLMat CamLookMat;

	static const unsigned char CamPosMatCnt = 10;

	ChVec3 CamOffPos = ChVec3(0.0f, 7.0f, -6.0f);
	ChLMat CamOffMat[CamPosMatCnt];

	ChLMat CamPos;
	std::vector<ChPtr::Shared<ChPhysicalBase9>> Physical;

	//LookAnotherRobots//
	ChPtr::Weak<BaseRobotsList> WpBRobotsList;

	ChPtr::Weak<HitPosList> WpHitPosList;

	//AvoHitAnotherRobotsData//

	void AvoHitRobots();

	const float Len = 2.0f;

};

class BaseRobotsList :public std::enable_shared_from_this<BaseRobotsList>
{
public:

	BaseRobotsList() {}

	~BaseRobotsList() { robotsList.clear(); }

	void Init();

	void SetRobots(std::string _PlayerData, const ChVec3 *_StartPos,const unsigned short _PartsNum);

	void SetRobots(const ChStd::Bool _EneFlg, ChVec3 *_StartPos,const unsigned short _PartsNum);

	void SetRobots2(const ChStd::Bool _EneFlg, ChVec3 *_StartPos);

	void SetDData(
		const DWORD _No
		, const ChVec3* _Pos)
	{
		if (_No >= robotsList.size())return;
		robotsList[_No]->SetDData(_Pos);
	}

	inline const DWORD GetRobotsCnt() { return  robotsList.size(); }

	inline ChStd::Bool GetEnemyFlg(const DWORD _No)
	{
		if (robotsList.empty()
			|| robotsList.size() <= _No)return false;
		return robotsList[_No]->IsEnemy();
	}

	inline unsigned char GetEnemyCnt() { return enemyCnt; }

	inline unsigned char GetMemberCnt() { return memberCnt; }

	ChStd::Bool IsDFlg() { return dFlg; }

	void DownDFlg() { dFlg = false; }

	void Draw();

	void Update();

	void CamUpdate();

	ChStd::Bool IsMemberLive() { return memberCnt > 0 ? true : false; }

	ChStd::Bool IsEnemyLive() { return enemyCnt > 0 ? true : false; }

	void DrawMarker();

private:

	void MoveCamRobot();

	ChLMat tmpMat;

	std::vector<ChPtr::Shared<BRobot>> robotsList;

	ChPtr::Shared<BRobot> endCamRobot;

	//View変数//
	ChVec3 baseCamPos = ChVec3(0.0f, 5.0f, -20.0f)
		, baseCamLook = ChVec3(0.0f, 0.0f, 6.0f);

	ChStd::Bool dFlg = true;

	char partsData[3];

	unsigned char enemyCnt = 0;
	unsigned char memberCnt = 0;

	ChPtr::Shared<DDataBase>DBase;

	ChLMat camera;


};

#endif