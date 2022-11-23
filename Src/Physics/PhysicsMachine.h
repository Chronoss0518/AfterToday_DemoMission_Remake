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
		ChLMat transform;
	};

public:

	void Init();

public:

	void Update();

private:

	void UpdateHitTest();

public:

	static inline void AddField(ChPtr::Shared<ChCpp::FrameObject> _fieldModel,const ChLMat& _mat)
	{
		if (_fieldModel == nullptr)return;
		auto field = ChPtr::Make_S<ModelData>();
		field->model = _fieldModel;
		field->transform = _mat;
		FieldList().push_back(field);
	}

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

	static inline void SetFPS(unsigned long _fps) { FreamPerSeccond() = _fps; }

	inline void SetAddMovePowerVector(const ChVec3& _addPowerVector) { addMovePowerVector = _addPowerVector; }

	inline void SetAddRotatePowerVector(const ChVec3& _addPowerVector) { addRotatePowerVector = _addPowerVector; }

	inline void SetPosition(const ChVec3& _position) { position = _position; }

	inline void SetRotation(const ChVec3& _rotation) { rotation = _rotation; }

	inline void SetScalling(const ChVec3& _scalling) { scalling = _scalling; }

	inline void SetGroundHeight(const float _height) { groundHeight = _height; }

	inline void SetLeftRightWallLen(const float _len) { if(_len > 0.0f)leftRightWallLen = _len; }

	inline void SetFrontBackWallLen(const float _len) { if (_len > 0.0f)frontBackWallLen = _len; }

	static inline void SetElevation(const float _elevation) { Elevation() = _elevation; }

	static inline void SetAirRegist(const float _regist) { AirRegist() = _regist; }

	static inline void SetFieldSize(const ChVec3& _size) { FieldSize() = _size; }

public:

	inline ChVec3 GetAddMovePowerVector() { return addMovePowerVector; }

	inline ChVec3 GetAddRotatePowerVector() { return addRotatePowerVector; }

	inline float GetMass() { return mass; }

	inline float GetGroundHeight() { return groundHeight; }

public:

	inline ChStd::Bool IsGround() { return isGroundFlg; }

	inline ChStd::Bool IsWall() { return isWallFlg; }

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

	ChStd::Bool isGroundFlg = false;
	ChStd::Bool isWallFlg = false;

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
		static unsigned long ins = 0;
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

		inline void SetInversFlg(const ChStd::Bool _flg) { inversFlg = _flg; }

		inline void SetBaseDirection(const ChVec3& _dir) { baseDir = _dir; }

		inline void SetPosition(const ChVec3& _pos) { ray.SetPosition(_pos); }

		inline void SetMaxLength(const float _len) { ray.SetMaxLength(_len); }

	public:

		inline float GetVecSize() { return vecSize; }

	public:

		inline ChStd::Bool IsInvers() { return inversFlg; }

		inline ChStd::Bool IsHit() { return isHit; }

		void IsHitField(ChCpp::PolygonCollider& _polygon);

	private:

		ChCpp::HitTestRay ray;
		ChStd::Bool inversFlg = false;
		ChStd::Bool isHit = false;
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

	ChStd::Bool IsGround();

private:

	PhysicsMachine* machine = nullptr;

};