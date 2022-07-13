#ifndef _Damage
#define _Damage

class Damage
{
public:

	Damage();

	~Damage();

	void Update();

	void SetDPos(const ChVec3_9 *_Pos, const ChStd::Bool _DFlg);

	ChStd::Bool GetDDataEmpty() { return DEffectList.empty(); }

	struct DData
	{
		ChVec3_9 Pos;
		float Rot;
		float Scl;
		float Alpha;
		std::string DataName;
	};

private:
	float MaxScl = 2.0f;
	const float TimeCnt = 20.0f;

	std::vector<ChPtr::Shared<DData>>DEffectList;

	std::string SEName = "SE_Burn1.wav";

public:

	inline std::vector<ChPtr::Shared<DData>>::iterator GetDData() 
	{ return DEffectList.begin(); }

	inline std::vector<ChPtr::Shared<DData>>::iterator GetEndDData()
	{ return DEffectList.end(); }

};

class DDataBase
{
public:

	void Init();

	void Release();

	inline void SharedDamageData(ChPtr::Shared<Damage> _D)
	{
		if (_D == nullptr)return;
		DList.push_back(_D);
	}

	void Update();

	void Draw();

private:




	std::vector<ChPtr::Shared<Damage>>DList;

	std::string XFile_Burn = "Burn";

};

#endif