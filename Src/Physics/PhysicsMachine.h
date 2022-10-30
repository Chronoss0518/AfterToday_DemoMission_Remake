#pragma once

class PhysicsMachine;

class PhysicsFunction
{
public:

	void Init();

	virtual void Update() = 0;

	virtual void UpdateEnd(){}

	void AddEffectMove(const ChVec3& _addMove);

	std::vector<ChPtr::Weak<ChCpp::FrameObject>>GetFieldList();

private:

	PhysicsMachine* machine = nullptr;

};

class PhysicsMachine:public ChCp::Initializer
{
public:

	friend PhysicsFunction;

public:

	enum class PhysicsFunctionList
	{
		Gravity,
		AirPower,
		WaterPower,
		GroundPower,
		HitTest,
		None
	};

public:

	void Init();

public:

	void Update();

	void UpdateGravity();

	void UpdateAirPower();

	void UpdateRegistance();

public:

	inline void AddField(ChPtr::Shared<ChCpp::FrameObject> _fieldModel)
	{
		if (_fieldModel == nullptr)return;
		FieldList().push_back(_fieldModel);
	}

public:

	inline void SetGravityAcceleration(float _acceleration)
	{
		GetGravityAcceleration() = _acceleration;
	}

public:

	inline ChVec3 GetEffectMoveVector() { return effectMove; }

public:


	ChPtr::Shared<PhysicsFunction> functionList[ChStd::EnumCast(PhysicsFunctionList::None)];

	ChCpp::BitBool physicsFunctionFlg = ChCpp::BitBool((ChStd::EnumCast(PhysicsFunctionList::None) / 8) + 1);

	inline void ClearFieldList() 
	{
		auto fieldList = FieldList();
		if (fieldList.empty())return;
		fieldList.clear();
	}

private:

	ChVec3 effectMove;

	static inline float& GetGravityAcceleration()
	{
		static float ins;
		return ins;
	}

	inline static std::vector<ChPtr::Weak<ChCpp::FrameObject>>& FieldList()
	{
		static std::vector<ChPtr::Weak<ChCpp::FrameObject>> ins;
		return ins;
	}
};
