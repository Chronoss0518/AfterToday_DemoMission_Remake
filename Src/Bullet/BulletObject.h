#pragma once

class BulletData;
class BoostBulletData;
class HighExplosiveBulletData;
class MissileData;
class GameFrame;

class BulletObject :public ChCpp::BaseObject
{
public:
	
	friend BulletData;
	friend BoostBulletData;
	friend HighExplosiveBulletData;
	friend MissileData;

public:

	void Init(const ChLMat& _startMat);

public:

	virtual void SetBulletData(BulletData* _data) { data = _data; }

	inline void SetIsHitTrue() { hitFlg = true; }

	inline void SetFrame(GameFrame* _frame) { frame = _frame; }

	inline void SetBaseMecha(BaseMecha* _mecha) { mecha = _mecha; }

	inline void SetPosition(const ChVec3& _pos) { physics->SetPosition(_pos); }

	inline void SetRotation(const ChVec3& _rot) { physics->SetRotation(_rot); }

public:

	inline ChVec3 GetPosition() { return physics->GetPosition(); }

	inline ChVec3 GetRotation() { return physics->GetRotation(); }

	inline ChVec3 GetMovePower() { return physics->GetAddMovePowerVector(); }

	unsigned long GetPenetration();

	BaseMecha* GetBaseMecha() { return mecha; }

	inline GameFrame& GetFrame() { return *frame; }

	inline ChCpp::SphereCollider& GetCollider() { return collider; }

public:

	void Update()override;

	void Move()override;

	void Draw3D()override;

protected:

	ChPtr::Unique<PhysicsMachine>physics = ChPtr::Make_U<PhysicsMachine>();

private:

	ChCpp::SphereCollider collider;

	GameFrame* frame = nullptr;
	BulletData* data = nullptr;
	ChStd::Bool hitFlg = false;
	BaseMecha* mecha = nullptr;

};