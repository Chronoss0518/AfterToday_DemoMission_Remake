#ifndef _HitPos
#define _HitPos

class BRobot;
class RobPartsList;
class Damage;
class DDataBase;

class Bullet
{
public:

	void BInit();

	//生存フラグ更新//
	inline void SetHitFlg(ChStd::Bool _Flg) { BreakFlg = _Flg; }

	inline ChStd::Bool SetAData(const ChPtr::Shared<ChGame::APosData>_AData)
	{
		if (_AData == nullptr)return ChStd::False;
		Pos = *_AData;
		StartPos = Pos.Pos;
		return ChStd::True;
	}

	//使用するTexture、MeshのNo//
	ChStd::DataNo GetObjNum() { return TextureNo; }

	//ベースとなるアタック時のデータ//
	ChGame::APosData* GetAPosData() { return &Pos; }

	//生存確認//
	ChStd::Bool IsArive() { return !BreakFlg; }

	ChStd::Bool IsClear() { return ClearFlg; }

	//更新//
	virtual void Update(const ChVec3_9* _BPos,const float _ClearLen);

protected:

	ChVec3_9 StartPos;
	std::vector<ChPtr::Shared<ChPhysicalBase9>> Physical;

	ChStd::Bool BreakFlg = ChStd::False;
	ChStd::Bool ClearFlg = ChStd::False;

	ChGame::APosData Pos;
	ChStd::DataNo TextureNo;

};

class Missaile :public Bullet
{

};

class Roket :public Bullet
{

};

class HitPosList
{
public:

	HitPosList();

	~HitPosList();

	void Init();

	inline void SetBasePos(ChVec3_9* _Pos)
	{
		if (ChPtr::NullCheck(_Pos))return;
		BasePos = _Pos;
	}

	void SetAData(const ChPtr::Shared<ChGame::APosData>_RData);

	inline DWORD GetAPosNum()
	{
		return BList.size();
	}

	inline ChStd::Bool IsHaveBullet()
	{
		return !BList.empty();
	}

	void IsHitTest(ChPtr::Shared<RobPartsList> _BRob);

	void Draw();

	void UpDate();

private:

	ChVec3_9* BasePos;

	const unsigned short MaxAPos = 100;

	const float mSclCnt = 0.1f;
	
	const float LenForClear = 250.0f;

	std::vector<ChPtr::Shared<Bullet>>BList;

	ChPtr::Shared<Damage>DData;

	ChPtr::Shared<DDataBase>DDataList;

	//ChPtr::Weak<BaseRobotsList> WpBRobotsList;

};

#endif 