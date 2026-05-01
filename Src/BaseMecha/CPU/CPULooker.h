#pragma once

class GameFrame;
class BaseMecha;

class CPUObjectLooker:public ChCpp::BaseComponent
{
private:

	class CPUObjectLookerThreadUpdate : public ChCpp::ThreadObject
	{
	public:

		inline void SetCPUObjectLooker(CPUObjectLooker* _looker)
		{
			looker = _looker;
		}

	public:

		void Update()override;

	private:

		CPUObjectLooker* looker = nullptr;
	};

public:

	virtual ~CPUObjectLooker() { Release(); }

public:

	struct UseSquareValues
	{
		std::vector<ChPtr::Shared<LookSquareValue>> values;
		ChPtr::Weak<BaseMecha> otherMecha;
		float nearDistance = 1e+38f;
	};

	enum class MemberType : unsigned char
	{
		Member,
		Enemy,
		Other,
	};

	static const unsigned char MEMBER_TYPE_COUNT = 3;

	enum class DistanceType : unsigned char
	{
		Near,
		Far
	};

	static const unsigned char DISTANCE_TYPE_COUNT = 2;

	enum class DamageSizeType : unsigned char
	{
		Many,
		Few
	};

	static const unsigned char DAMAGE_SIZE_TYPE_COUNT = 2;

public:

	void Init()override;

	void DrawBegin()override;

	void Draw2D()override;

	void DrawEnd()override;

	void Release()override;

public:

	inline void SetGameFrame(GameFrame* _frame) { frame = _frame; }

	inline void UpdateCount(const unsigned long _updateCount) { updateCount = _updateCount; }

public:

	unsigned long GetLookTypeMechas(MemberType _member, DistanceType _distance, DamageSizeType _damageSize);

	std::vector<unsigned long> GetLookMechaList() { return lookMechaList; }

public:
	
	bool IsEndUpdate() { return !updateFlg; }

public:

	void FindMecha();

private:

	void MenyDamageTest(unsigned long& _base, unsigned long _target, std::vector<ChPtr::Weak<BaseMecha>>& _mechas);

	void FewDamageTest(unsigned long& _base, unsigned long _target,std::vector<ChPtr::Weak<BaseMecha>>& _mechas);

	GameFrame* frame = nullptr;

	ChCpp::HitTestRay ray;

	bool updateFlg = false;
	ChPtr::Shared<CPUObjectLookerThreadUpdate>threadObject = nullptr;

	unsigned long updateCount = 1;
	unsigned long nowUpdateCount = 0;

	unsigned long lookMechaTypes[MEMBER_TYPE_COUNT][DISTANCE_TYPE_COUNT][DAMAGE_SIZE_TYPE_COUNT];
	
	std::vector<ChPtr::Shared<UseSquareValues>>lookMecas;
	std::vector<ChPtr::Shared<LookSquareValue>>lookMaps;
	ChD3D11::Texture11 whiteTexture;

	std::vector<ChPtr::Shared<ChVec2>>targetPosition;

	float circleSize = 0.19f;

	ChD3D11::Sprite11 sprite;

	ChD3D11::Shader::BaseDrawSprite11 spriteDrawer;

	std::vector<unsigned long>lookMechaList;

};