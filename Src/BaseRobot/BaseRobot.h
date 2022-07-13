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
		ChVec3_9 Pos;
		ChStd::FPOINT Rot;
		char Team;
		unsigned char RobotsNo;
		ChStd::Bool EnemyFlg;
	};

	//位置情報//
	struct MoveData
	{
		ChVec3_9 MoveV;//移動量//
		ChStd::FPOINT TmpRot;//仮の回転量//
		const float AvoDen = 0.8f;//回避時の減速する割合//
		float AvoMax = 5.0f;//回避時の最大回避速度//
		float AvoV = 0.0f;//回避時の速度//
		const float AvoMin = 0.0005f;//回避時の最低速度//
		ChVec3_9 AvoDir;//回避する向き//
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

	inline void SetPos(const ChVec3_9 *_Pos) { BaseD.Pos = *_Pos; }

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

	void SetLookView(ChPtr::Shared<ChCamera9> _Cam)
	{
		WpLook = _Cam;
	}

	inline void SetDData(
		const ChVec3_9* _Pos)
	{
		if (DData == nullptr)return;
		ChStd::Bool DFlg = ChStd::False;
		if (HP < 1)DFlg = ChStd::True;
		DData->SetDPos(_Pos, DFlg);
	}

	inline virtual const ChMat_9* GetCamPos() { return &CamPos; }

	inline const ChVec3_9* GetPos() { return &BaseD.Pos; }

	inline const ChVec3_9* GetMoveVec() { return &CamMoveVec; }



	inline ChStd::Bool GetIsKey(const ChStd::DataNo _Key)
	{
		return ButtonFlg[_Key];
	}

	inline const ChPtr::Shared<RobPartsList>GetRobParts()
	{
		return RobParts;
	}

	inline const ChMat_9* GetMat() { return &Mat; }

	inline const ChMat_9* GetDrawMat() { return &DrawMat; }

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
		if (DData == nullptr)return ChStd::False;
		if (HP > 0 || !DData->GetDDataEmpty())return ChStd::True;
		return ChStd::False;
	}

	inline ChStd::Bool IsLiveFlg()
	{
		if (HP > 0)return ChStd::True;
		return ChStd::False;
	}

	void Draw(const ChStd::Bool _EffectFlg);

	virtual void Input() {};

	void Move();
	
	BRobot& operator= (BRobot& _cm)
	{
		this->Mat = *_cm.GetMat();
		this->CamPos = *_cm.GetCamPos();
		this->BaseD.EnemyFlg = _cm.IsEnemy();
		return *this;
	}

protected:

	void Attack(const ChPtr::Shared<ChGame::WeapData> _Weap);

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
	ChStd::Bool ButtonFlg[KeyCount];
	ChPtr::Shared<ChMouCon>Mouse;

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
	ChMat_9 TargetPos;
	//ターゲットのウィンドウ上の位置から中央までの距離//
	float TWindOnPosLen = 0.0f;

	virtual void BInit(const unsigned short _PartsNum);

	void Change();

	void InputToMove();

	void PartsSet();

	void CreatLook();

	void CreateShotData(const ChStd::DataNo _TmpNo);

	void SetRobParts();

	float MoveS = 0.5f;
	float RaderLen = 300.0f;

	float HP = 100;
	float MaxHp;
	//float Enelgy = 10000;
	//float MaxEnelgy;

	unsigned short AddDamage = 0;


	ChStd::FPOINT MoveRot;

	float Sens = 1.0f;
	const float MouMoveP = 30.0f;

	ChStd::Bool BoostFlg = ChStd::False;
	float BoostV = 1.0f;
	//short BoostEnelgy = 100;

	BaseData BaseD;
	MoveData MoveD;

	ChPtr::Shared<RobPartsList>RobParts;


	const float MaxCamMoveSpeed = 10.0f;

	//Damage用//
	ChPtr::Shared<Damage> DData = nullptr;

	//DrawData//
	ChStd::COLOR1f Col;
	ChMat_9 Mat;
	ChMat_9 DrawMat;

	//CamData//

	void CamUpdate();

	ChPtr::Shared<ChCamera9>Look;
	ChStd::Bool LookFlg = ChStd::False;

	ChVec3_9 CamMoveVec;

	ChVec3_9 BaseCamPos = ChVec3_9(0.0f, 0.0f, 5.0f)
		, BaseCamLook = ChVec3_9(0.0f, 0.0f, 6.0f);

	void SetCamMat();

	void SetLook();

	ChMat_9 CamLookMat;

	static const unsigned char CamPosMatCnt = 10;

	ChVec3_9 CamOffPos = ChVec3_9(0.0f, 7.0f, -6.0f);
	ChMat_9 CamOffMat[CamPosMatCnt];

	ChMat_9 CamPos;
	std::vector<ChPtr::Shared<ChPhysicalBase9>> Physical;

	//LookAnotherRobots//
	ChPtr::Weak<BaseRobotsList> WpBRobotsList;

	ChPtr::Weak<HitPosList> WpHitPosList;

	ChPtr::Weak<ChCamera9>WpLook;

	//AvoHitAnotherRobotsData//

	void AvoHitRobots();

	const float Len = 2.0f;

};

class BaseRobotsList :public std::enable_shared_from_this<BaseRobotsList>
{
public:

	BaseRobotsList() {}

	~BaseRobotsList() { RobotsList.clear(); }

	void Init();

	inline void SetHitPosList(const ChPtr::Shared<HitPosList> _HitPosList)
	{
		WpHitPosList = _HitPosList;
	}

	void SetRobots(std::string _PlayerData, const ChVec3_9 *_StartPos,const unsigned short _PartsNum);

	void SetRobots(const ChStd::Bool _EneFlg, ChVec3_9 *_StartPos,const unsigned short _PartsNum);

	void SetRobots2(const ChStd::Bool _EneFlg, ChVec3_9 *_StartPos);

	void SetCamRobot(const DWORD _No)
	{
		if (RobotsList.size() <= _No)return;
		WpCamRobot = RobotsList[_No];
	}

	void SetDData(
		const DWORD _No
		, const ChVec3_9* _Pos)
	{
		if (_No >= RobotsList.size())return;
		RobotsList[_No]->SetDData(_Pos);
	}

	const ChMat_9* GetCamPos();

	const ChMat_9* GetCamLookPos();

	inline const DWORD GetRobotsCnt() { return  RobotsList.size(); }

	const ChMat_9* GetMat(const DWORD _No);

	inline ChStd::Bool GetEnemyFlg(const DWORD _No)
	{
		if (RobotsList.empty()
			|| RobotsList.size() <= _No)return ChStd::False;
		return RobotsList[_No]->IsEnemy();
	}

	inline unsigned char GetEnemyCnt() { return EnemyCnt; }

	inline unsigned char GetMemberCnt() { return MemberCnt; }

	ChStd::Bool IsDFlg() { return DFlg; }

	void DownDFlg() { DFlg = ChStd::False; }

	void Draw();

	void Update();

	void CamUpdate();

	ChStd::Bool IsMemberLive() { return MemberCnt > 0 ? ChStd::True : ChStd::False; }

	ChStd::Bool IsEnemyLive() { return EnemyCnt > 0 ? ChStd::True : ChStd::False; }

	void DrawMarker();

private:

	void MoveCamRobot();

	ChMat_9 TmpMat;

	std::vector<ChPtr::Shared<BRobot>> RobotsList;

	ChPtr::Shared<BRobot> EndCamRobot;

	ChPtr::Weak<BRobot> WpCamRobot;

	ChPtr::Weak<HitPosList> WpHitPosList;

	//View変数//
	ChVector3_9 BaseCamPos = ChVec3_9(0.0f, 5.0f, -20.0f)
		, BaseCamLook = ChVec3_9(0.0f, 0.0f, 6.0f);

	ChStd::Bool DFlg = ChStd::True;

	char PartsData[3];

	unsigned char EnemyCnt = 0;
	unsigned char MemberCnt = 0;

	ChPtr::Shared<DDataBase>DBase;

	ChPtr::Shared<ChCamera9> Camera;


};

#endif