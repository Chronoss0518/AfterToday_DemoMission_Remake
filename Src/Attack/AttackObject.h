#pragma once

class Attack;
class BulletData;
class BoostBulletData;
class HighExplosiveBulletData;
class MissileData;
class GameFrame;
class ControllerBase;

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

	inline void SetBaseMechaNo(unsigned long _no) { mechasNo = _no; }

	inline void SetTeamNo(unsigned long _no) { teamNo = _no; }

	inline void SetPosition(const ChVec3& _pos) { physics->SetPosition(_pos); }

	inline void SetRotation(const ChVec3& _rot) { physics->SetRotation(_rot); }

	inline void SetMovePower(const ChVec3 _pow) { physics->SetAddMovePowerVector(_pow); }

public:

	inline ChVec3 GetPosition() { return physics->GetPosition(); }

	inline ChVec3 GetRotation() { return physics->GetRotation(); }

	inline ChVec3 GetMovePower() { return physics->GetAddMovePowerVector(); }

	unsigned long GetPenetration();

	float GetHitSize();

	inline GameFrame& GetFrame() { return *frame; }

	inline ChCpp::SphereCollider& GetCollider() { return collider; }

public:

	bool IsHit() { return hitFlg; }

	bool IsUseMechaTest(unsigned long _no) { return mechasNo == _no; }

	bool IsUseMechaTeamTest(unsigned long _no) { return teamNo == _no; }

public:

	void Update()override;

	void Move()override;

	void Draw3D()override;

public:

	void UpdateHit();

protected:

	ChPtr::Unique<PhysicsMachine>physics = ChPtr::Make_U<PhysicsMachine>();

private:

	ChCpp::SphereCollider collider;

	GameFrame* frame = nullptr;
	unsigned long teamNo = -1;
	unsigned long mechasNo = -1;
	Attack* data = nullptr;
	bool hitFlg = false;
	unsigned long nowCreateSmokeCount = 0;

	float dispersalPower = 0.5f;
	unsigned long createSmokeCount = 0;
};