#ifndef _EnemyObj
#define _EnemyObj

const enum class EnemyType :char
{
	Normal,Captain,Boss
};

class EnemyObj :public BRobot
{
public:

	EnemyObj(const char _Type, ChStd::Bool _Enemy);

	void Input()override;

private:

	//void KeyBordInput();

	//void CPUInput();

	void Init(unsigned short _PartsNum)override;

	std::function<void(void)> InputDate;

	void EInit(const char _Type);

	//EnemyType//
	char EType = GetEnumCNum(EnemyType, Normal);

	std::vector<ChPtr::Shared<std::string>>TestDate;
};


class EnemyObj2 :public BRobot
{
public:

	EnemyObj2(ChStd::Bool _Enemy);

	void Input()override;

private:

	void BInit(const unsigned short _PartsNum)override;

	//void KeyBordInput();

	//void CPUInput();

	void Init(unsigned short _PartsNum)override;

	std::function<void(void)> InputDate;

	//�ڕW���ւ̕���//
	ChVec3_9 Dir;

	//EnemyBattleLen
	float BLen = 5.0f;


	std::vector<ChPtr::Shared<std::string>>TestDate;
};

#endif
