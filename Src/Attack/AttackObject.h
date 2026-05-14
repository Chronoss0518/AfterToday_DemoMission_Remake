#pragma once

class PhysicsMachine;
class Attack;
class BulletData;
class BoostBulletData;
class HighExplosiveBulletData;
class MissileData;
class GameFrame;
class ControllerBase;

#ifndef ATTACK_OBJECT_DEBUG_FLG
#define ATTACK_OBJECT_DEBUG_FLG false
#endif

#ifndef ATTACK_OBJECT_DEBUG_SIZE
#define ATTACK_OBJECT_DEBUG_SIZE 100.0f
#endif

class AttackObject :public ChCpp::BaseObject<wchar_t>
{
public:
	
	friend Attack;
	friend BulletData;
	friend BoostBulletData;
	friend HighExplosiveBulletData;
	friend MissileData;

public:

	void Init(const ChLMat& _startMat);

public:

	virtual void SetBulletData(Attack* _data) { data = _data; }

	inline void SetIsHitTrue() { hitFlg = true; }

	inline void SetFrame(GameFrame* _frame) { frame = _frame; }

	inline void SetBaseMechaNo(size_t _no) { mechasNo = _no; }

	inline void SetTeamNo(size_t _no) { teamNo = _no; }

	void SetPosition(const ChVec3& _pos);

	void SetRotation(const ChVec3& _rot);

	void SetMovePower(const ChVec3 _pow);

public:

	ChVec3 GetPosition();

	ChVec3 GetRotation();

	ChVec3 GetMovePower();

	unsigned long GetPenetration();

	float GetHitSize();

	inline GameFrame& GetFrame() { return *frame; }

	inline ChCpp::SphereCollider& GetCollider() { return collider; }

public:

	bool IsHit() { return hitFlg; }

	bool IsUseMechaTest(size_t _no) { return mechasNo == _no; }

	bool IsUseMechaTeamTest(size_t _no) { return teamNo == _no; }

public:

	void Update()override;

	void Move()override;

	void Draw3D()override;

public:

	void UpdateHit();

protected:

	ChPtr::Shared<PhysicsMachine>physics = nullptr;

private:

	ChCpp::SphereCollider collider;

	GameFrame* frame = nullptr;
	size_t teamNo = -1;
	size_t mechasNo = -1;
	Attack* data = nullptr;
	bool hitFlg = false;
	unsigned long nowCreateSmokeCount = 0;

	float dispersalPower = 0.25f;
	unsigned long createSmokeCount = 0;
};