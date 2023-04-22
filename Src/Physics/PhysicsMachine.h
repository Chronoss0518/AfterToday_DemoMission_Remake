#pragma once

class PhysicsFunction;

class PhysicsMachine:public ChCp::Initializer
{
public:

	friend PhysicsFunction;

public:

	enum class PhysicsFunctionList:unsigned char
	{
		Gravity,
		AirPower,
		WaterPower,
		GroundPower,
		None
	};

	struct ModelData
	{
		ChPtr::Weak<ChCpp::FrameObject> model;
		ChCpp::PolygonCollider collider;
		ChLMat transform;
	};

public:

	void Init();

public:

	void Update();

private:

	void UpdateHitTest();

public:

	inline void AddMovePowerVector(const ChVec3& _addPowerVector) { addMovePowerVector += _addPowerVector / (CastFloat(GetFPS()) / CastFloat(BaseSpeed())); }

	inline void AddRotatePowerVector(const ChVec3& _addPowerVector) { addRotatePowerVector += _addPowerVector / (CastFloat(GetFPS()) / CastFloat(BaseSpeed())); }

	static void AddField(ChPtr::Shared<ChCpp::FrameObject> _fieldModel, const ChLMat& _mat);

	inline void AddBaseModel(ChPtr::Shared<ChCpp::FrameObject>_model,const ChLMat& _mat)
	{
		if (_model == nullptr)return;
		auto model = ChPtr::Make_S<ModelData>();
		model->model = _model;
		model->transform = _mat;
		baseModels.push_back(model);
	}

	static inline void AddBrustPos(const ChVec3& _pos)
	{
		auto pos = ChPtr::Make_S<ChVec3>();
		*pos = _pos;
		BrustPosList().push_back(pos);
	}

public:

	static inline void SetGravityAcceleration(float _acceleration)
	{
		GravityAcceleration() = _acceleration;
	}

	inline void SetMass(float _mass) { mass = _mass; }

	inline void SetAddMovePowerVector(const ChVec3& _addPowerVector) { addMovePowerVector = _addPowerVector; }

	inline void SetAddRotatePowerVector(const ChVec3& _addPowerVector) { addRotatePowerVector = _addPowerVector; }

	inline void SetPosition(const ChVec3& _position) { position = _position; }

	inline void SetRotation(const ChVec3& _rotation) { rotation = _rotation; }

	inline void SetScalling(const ChVec3& _scalling) { scalling = _scalling; }

	inline void SetGroundHeight(const float _height) { groundHeight = _height; }

	inline void SetLeftRightWallLen(const float _len) { if(_len > 0.0f)leftRightWallLen = _len; }

	inline void SetFrontBackWallLen(const float _len) { if (_len > 0.0f)frontBackWallLen = _len; }

	static inline void SetFPS(unsigned long _fps) { if(_fps > 0)FreamPerSeccond() = _fps; }

	static inline void SetBaseSpeed(unsigned long _baseSpeed) { if (_baseSpeed > 0)BaseSpeed() = _baseSpeed; }

	static inline void SetElevation(const float _elevation) { Elevation() = _elevation; }

	static inline void SetAirRegist(const float _regist) { AirRegist() = _regist; }

	static inline void SetFieldSize(const ChVec3& _size) { FieldSize() = _size; }

public:

	inline ChVec3 GetAddMovePowerVector() { return addMovePowerVector; }

	inline ChVec3 GetAddRotatePowerVector() { return addRotatePowerVector; }

	inline ChVec3 GetPosition() { return position; }

	inline ChVec3 GetRotation() { return rotation; }

	inline float GetMass() { return mass; }

	inline float GetGroundHeight() { return groundHeight; }

	static inline unsigned long GetFPS() { return FreamPerSeccond(); }

	static inline unsigned long GetBaseSpeed() { return BaseSpeed(); }

	static inline float GetElevation() { return Elevation(); }

	static inline float GetAirRegist() { return AirRegist(); }

	static inline ChVec3 GetFieldSize() { return FieldSize(); }

public:

	inline bool IsGround() { return isGroundFlg; }

	inline bool IsWall() { return isWallFlg; }

	inline bool IsOutSide() { return isOutSide; }

public:

	static inline void ClearFieldList()
	{
		auto fieldList = FieldList();
		if (fieldList.empty())return;
		fieldList.clear();
	}

	inline void ClearBaseModel()
	{
		baseModels.clear();
	}

private:

	inline float CastFloat(unsigned long _val) { return static_cast<float>(_val); }

	void TestFieldRange();

	void RangeTest(float& _addMovePowerVectorElement,const float _testPosElement, const float _fieldSizeElement);

	void HitTestEmptyModelAndField();

	ChPtr::Shared<PhysicsFunction> functionList[ChStd::EnumCast(PhysicsFunctionList::None)];

	ChCpp::BitBool physicsFunctionFlg = ChCpp::BitBool((ChStd::EnumCast(PhysicsFunctionList::None) / 8) + 1);

	ChVec3 registMovePowerVector;
	ChVec3 addMovePowerVector;

	ChVec3 registRotatePowerVector;
	ChVec3 addRotatePowerVector;

	ChVec3 position;
	ChVec3 rotation;
	ChVec3 scalling = ChVec3(1.0f);

	float groundHeight = 0.0f;
	float leftRightWallLen = 5.0f;
	float frontBackWallLen = 5.0f;

	ChLMat nowTransform;

	bool isGroundFlg = false;
	bool isWallFlg = false;
	bool isOutSide = false;

	float mass = 0.0f;

	std::vector<ChPtr::Shared<ModelData>>baseModels;

	static inline float& GravityAcceleration()
	{
		static float ins = 9.8f;
		return ins;
	}

	inline static float& AirRegist()
	{
		static float ins = 0.0f;
		return ins;
	}

	static inline float& TestStartHeight()
	{
		static float ins = 0.0f;
		return ins;
	}

	inline static std::vector<ChPtr::Shared<ModelData>>& FieldList()
	{
		static std::vector<ChPtr::Shared<ModelData>> ins;
		return ins;
	}

	inline static std::vector<ChPtr::Shared<ChVec3>>&BrustPosList()
	{
		static std::vector<ChPtr::Shared<ChVec3>> ins;
		return ins;
	}

	inline static unsigned long& FreamPerSeccond()
	{
		static unsigned long ins = 1;
		return ins;
	}

	inline static unsigned long& BaseSpeed()
	{
		static unsigned long ins = 1;
		return ins;
	}

	inline static float& Elevation()
	{
		static float ins = 0.0f;
		return ins;
	}

	inline static float& DynamicFriction()
	{
		static float ins = 0.0f;
		return ins;
	}

	inline static float& StaticFriction()
	{
		static float ins = 0.0f;
		return ins;
	}

	inline static ChVec3& FieldSize()
	{
		static ChVec3 ins;
		return ins;
	}

	static ChPtr::Shared<PhysicsFunction> CreatePhysics(unsigned long _physicsFunctionlistName);

	class FieldHitTestRay
	{
	public:

		void Start();

		inline void End() {}

	public:

		inline void SetInversFlg(const bool _flg) { inversFlg = _flg; }

		inline void SetBaseDirection(const ChVec3& _dir) { baseDir = _dir; }

		inline void SetPosition(const ChVec3& _pos) { ray.SetPosition(_pos); }

		inline void SetMaxLength(const float _len) { ray.SetMaxLength(_len); }

	public:

		inline float GetVecSize() { return vecSize; }

	public:

		inline bool IsInvers() { return inversFlg; }

		inline bool IsHit() { return isHit; }

		void IsHitField(ChCpp::PolygonCollider& _polygon);

	private:

		ChCpp::HitTestRay ray;
		bool inversFlg = false;
		bool isHit = false;
		ChVec3 baseDir;

		float vecSize = 0.0f;
	};

	FieldHitTestRay yRay, xRay, zRay;
};

class PhysicsFunction
{
public:

	void Init(PhysicsMachine* _machine);

	virtual void AddMove() = 0;

	virtual void AddRegist() {};

public:

	void AddMovePowerVector(const ChVec3& _addMove);

	void AddRegistMovePowerVector(const ChVec3& _regist);

	void AddRotatePowerVector(const ChVec3& _addMove);

	void AddRegistRotatePowerVector(const ChVec3& _regist);

public:

	ChVec3 GetPosition();

	ChVec3 GetRotation();

	ChVec3 GetScalling();

	ChLMat GetTransform();

	std::vector<ChPtr::Shared<PhysicsMachine::ModelData>>GetFieldList();

	ChVec3 GetAddMovePowerVector();

	ChVec3 GetAddRotatePowerVector();

	float GetMass();

	unsigned long GetFPS();

	float GetGravityAcceleration();

	float GetElevation();

	float GetAirRegist();

	std::vector<ChPtr::Shared<ChVec3>>GetBrustPosList();

public:

	bool IsGround();

private:

	PhysicsMachine* machine = nullptr;

};