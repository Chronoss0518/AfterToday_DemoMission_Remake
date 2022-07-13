#ifndef _BRobParts
#define _BRobParts

class RobPartsList;
class HitPosList;

class RobParts
{
public:

	virtual ~RobParts();

	void Init(
		ChPtr::Shared<RobPartsList> _RobPartsList
		, std::string _PartsName);

	virtual void SetDitailsData(const std::string _DataName) {};

	virtual void SetPColor(const ChStd::COLOR1f* _Color);

	inline void SetXList(ChPtr::Shared<ChSmpXList9> _XList)
	{
		if (_XList == nullptr)return;
		if (WpXList.lock() != nullptr)return;
		WpXList = _XList;
	}

	virtual void SetOffsetPos() {};

	//virtual void SetParts(std::string _PartsName) {};

	inline unsigned char GetPType() { return PType; }

	inline ChPtr::Shared<ChGame::WeapData> GetAttackData()
	{
		if (Weap == nullptr)return nullptr;
		return Weap;
	}

	inline const ChMat_9* GetBasePos() { return &BaseMat; }

	inline const ChB_Box9* GetBBox() { return &BBox; }

	inline ChStd::Bool IsBreak()
	{
		if (HP <= 0.0f)return ChStd::True;
		return ChStd::False;
	}

	virtual void Update() {}

	virtual void Draw(const ChMat_9 *_Mat);

protected:

	virtual void SetStandardParts() {};

	virtual void SetParts(const std::string _PlDate);

	float HP = 100.0f;
	struct PartsData
	{
		ChStd::DataNo _PartsNo;
		ChMat_9* OffsetMat;
	};

	ChMat_9 BaseMat;
	unsigned char PType;//パーツの種類//

	std::string XFilePath;
	std::string DataPath;

	std::vector<ChPtr::Shared<PartsData>> PartsList;

	void SetAirBoost();

	void SetNormalBoost();

	//当たり判定時処理用変数//
	ChB_Box9 BBox;
	float DefPow = 0.0f;

	ChPtr::Weak<RobPartsList> WpRobParts;
	ChPtr::Weak<ChSmpXList9> WpXList;

	ChPtr::Shared<ChGame::APosData> APos = nullptr;
	ChPtr::Shared<ChGame::WeapData> Weap = nullptr;
};

class RPFoot :public RobParts
{
public:

	RPFoot();

	~RPFoot();

	void SetPColor(const ChStd::COLOR1f* _Color)override;

private:

	void SetStandardParts()override;

	const enum class PartsName :unsigned char
	{
		Waist, RFoot, LFoot
	};

};

class RPBody :public RobParts
{
public:

	RPBody();

	~RPBody();

	const ChVec3_9* GetArmOffPos() { return &ArmOffPos; }

	const ChVec3_9* GetHeadOffPos() { return &HeadOffPos; }

	const ChVec3_9* GetBoostOffPos() { return &BoostOffPos; }
private:

	ChVec3_9 ArmOffPos = ChVec3_9(0.0f,1.6f,0.0f);
	ChVec3_9 HeadOffPos = ChVec3_9(0.0f, 2.1f, 0.0f);
	ChVec3_9 BoostOffPos = ChVec3_9(0.0f, 1.8f, 0.0f);

	void SetStandardParts()override;

	const enum class PartsName :unsigned char
	{
		RArm, LArm, Head, Boost
	};

};

class RPHead :public RobParts
{
public:

	RPHead(const ChStd::Bool _WeapFlg);

	~RPHead();

	inline const ChPtr::Shared<ChGame::WeapData> GetWeapDate()
	{
		if (WeakCnt < WeakFreshCnt)return nullptr;
		WeakCnt = 0;
		return Weap;
	}

	void Update()override;

private:

	void SetStandardParts()override;

	unsigned char WeakCnt;
	unsigned char WeakFreshCnt;
};

class RPArm :public RobParts
{
public:

	RPArm();

	~RPArm();

	void SetPColor(const ChStd::COLOR1f* _Color);

private:

	void SetStandardParts()override;

	const enum class PartsName :unsigned char
	{
		RArm, LArm, RWeap, LWeap
	};

};

class RPBoost :public RobParts
{
public:

	RPBoost();

	~RPBoost();

	void Draw(const ChMat_9 *_Mat)override;

private:

	std::map<std::string, float>BoostPowList
	{
		std::pair<std::string, float>("FBoost", 0.0f)
		,std::pair<std::string, float>("UBoost", 0.0f)
		,std::pair<std::string, float>("RBoost", 0.0f)
		,std::pair<std::string, float>("LBoost", 0.0f)
	};



	void IsBoost();

	void SetParts(std::string _PartsName)override;

	void SetStandardParts()override;

	void SetBoost();

	std::string Boost = "Boost";

};

class RPWeapon :public RobParts
{
public:

	RPWeapon();

	~RPWeapon();

	inline const ChPtr::Shared<ChGame::WeapData> GetWeapDate()
	{
		if (WeakCnt < WeakFreshCnt)return nullptr;
		WeakCnt = 0;
		return Weap; 
	}

	void Update()override;

private:

	void SetParts(std::string _PartsName)override;

	void SetStandardParts()override;

	unsigned char WeakCnt;
	unsigned char WeakFreshCnt;
};

class RobPartsList :public std::enable_shared_from_this<RobPartsList>
{
public:

	const enum class BodyPartsListName :unsigned char
	{
		Foot, Body, Arm
		, Head, Boost
		, MWeap, SWeap
	};

	RobPartsList() {};

	~RobPartsList() {};

	void Init(ChPtr::Shared<BRobot>_BRobot, unsigned short _RobotNum);

	void Init(ChPtr::Shared<BRobot>_BRobot);

	void SetColor(
		const ChStd::DataNo _PartsNo
		, const ChStd::COLOR1f* _Color);

	float SetFootParts(std::string _PartsName);

	float SetBodyParts(std::string _PartsName);

	float SetHeadParts(std::string _PartsName);

	void SetArmParts(std::string _PartsName);

	float SetBoostParts(std::string _PartsName);

	void SetMWeapParts(std::string _PartsName);

	void SetSWeapParts(std::string _PartsName);

	inline const ChB_Box9* GetBBox(const ChStd::DataNo _DateNo)
	{
		if (PartsList.find(_DateNo) == PartsList.end())return nullptr;
		return PartsList[_DateNo]->GetBBox();
	}

	inline ChPtr::Shared<BRobot>GetBRobot() { return WpSetRobot.lock(); }

	ChStd::Bool IsPosHit(
		const ChGame::APosData* _AData);

	inline ChStd::Bool IsParts()
	{
		//if (PartsList.find())
	}

	const static unsigned char PartsCnt = 10;

	ChPtr::Shared<ChGame::WeapData> GetMWeap();

	ChPtr::Shared<ChGame::WeapData> GetSWeap();

	void Draw();

	void Updata();

private:

	void SetStanderdParts();

	//void SetParts(std::string _PartsName);

	ChPtr::Shared<ChSmpXList9> XList = nullptr;
	ChPtr::Shared<ChTexList9> TexList = nullptr;

	std::map<ChStd::DataNo ,ChPtr::Shared<RobParts>>PartsList;

	float Enelgy = 1000.0f;
	float MaxEnelgy = 1000.0f;

	ChPtr::Weak<BRobot>WpSetRobot;

	ChPtr::Weak<RPHead>WpHeadWeapon;
	ChPtr::Weak<RPWeapon> WpMWeapons;
	ChPtr::Weak<RPWeapon> WpSWeapons;

	std::vector<ChPtr::Shared<std::string>>TestDate;

};

#endif
