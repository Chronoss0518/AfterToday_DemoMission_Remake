
#include"../BaseIncluder.h"
#include"../AllStruct.h"

#include"PhysicsMachine.h"
#include"Physics_Gravity.h"
#include"Physics_AirPower.h"
#include"Physics_WaterPower.h"
#include"Physics_GroundPower.h"

#include<CPP/ChHitTestObject/ChHitTestRay.h>


void PhysicsFunction::Init(PhysicsMachine* _machine)
{
	machine = _machine;
}

void PhysicsFunction::AddMovePowerVector(const ChVec3& _addMove)
{
	machine->addMovePowerVector += _addMove;
}

void PhysicsFunction::AddRegistMovePowerVector(const ChVec3& _regist)
{
	machine->registMovePowerVector += _regist;
}

void PhysicsFunction::AddRotatePowerVector(const ChVec3& _addMove)
{
	machine->addRotatePowerVector += _addMove;
}

void PhysicsFunction::AddRegistRotatePowerVector(const ChVec3& _regist)
{
	machine->registRotatePowerVector += _regist;
}

ChVec3 PhysicsFunction::GetPosition()
{
	return machine->position;
}

ChVec3 PhysicsFunction::GetRotation()
{
	return machine->rotation;
}

ChVec3 PhysicsFunction::GetScalling()
{
	return machine->scalling;
}

ChLMat PhysicsFunction::GetTransform()
{
	return machine->nowTransform;
}

std::vector<ChPtr::Shared<PhysicsMachine::ModelData>>PhysicsFunction::GetFieldList()
{
	return PhysicsMachine::FieldList();
}

ChVec3 PhysicsFunction::GetAddMovePowerVector()
{
	return machine->addMovePowerVector;
}


ChVec3 PhysicsFunction::GetAddRotatePowerVector()
{
	return machine->addRotatePowerVector;
}

float PhysicsFunction::GetMass()
{
	return machine->mass;
}

unsigned long PhysicsFunction::GetFPS()
{
	unsigned long fps = PhysicsMachine::FreamPerSeccond();
	return fps != 0 ? fps : 1;
}

float PhysicsFunction::GetElevation()
{
	float elevation = PhysicsMachine::Elevation();
	return elevation > 0.0f ? elevation : 1.0f;
}

float PhysicsFunction::GetGravityAcceleration()
{
	return PhysicsMachine::GravityAcceleration();
}

float PhysicsFunction::GetAirRegist()
{
	return PhysicsMachine::AirRegist();
}

std::vector<ChPtr::Shared<ChVec3>>PhysicsFunction::GetBrustPosList()
{
	return PhysicsMachine::BrustPosList();
}

ChStd::Bool PhysicsFunction::IsGround()
{
	return machine->isGroundFlg;
}

void PhysicsMachine::Init()
{
	physicsFunctionFlg.SetValue(0xff);
	for (unsigned long i = 0; i < ChStd::EnumCast(PhysicsMachine::PhysicsFunctionList::None); i++)
	{
		functionList[i] = CreatePhysics(i);
		functionList[i]->Init(this);
	}
}

void PhysicsMachine::Update()
{
	nowTransform.Identity();

	nowTransform.SetPosition(position);
	nowTransform.SetRotation(rotation);
	nowTransform.SetScalling(scalling);

	for (unsigned char i = 0;i< ChStd::EnumCast(PhysicsMachine::PhysicsFunctionList::None);i++)
	{
		if (!physicsFunctionFlg.GetBitFlg(i))continue;
		functionList[i]->AddMove();
	}

	for (unsigned char i = 0; i < ChStd::EnumCast(PhysicsMachine::PhysicsFunctionList::None); i++)
	{
		if (!physicsFunctionFlg.GetBitFlg(i))continue;
		functionList[i]->AddRegist();
	}

	addMovePowerVector += registMovePowerVector;

	registMovePowerVector = 0.0f;

	addRotatePowerVector += registRotatePowerVector;

	registRotatePowerVector = 0.0f;

	UpdateHitTest();

}

void PhysicsMachine::UpdateHitTest()
{
	isGroundFlg = false;
	isWallFlg = false;
	
	TestFieldRange();
	
	HitTestEmptyModelAndField();

	if (baseModels.empty())
	{
		std::vector<ChPtr::Shared<ChCpp::ChMultiThread>>threadList;

		ChVec3 testPos = position + addMovePowerVector;
		testPos.y += groundHeight;
		

		{
			xRay.SetInversFlg(addMovePowerVector.x < 0.0f);
			xRay.SetBaseDirection(ChVec3(-1.0f, 0.0f, 0.0f));
			xRay.SetMaxLength(std::abs(addMovePowerVector.x) + leftRightWallLen);
			xRay.SetPosition(ChVec3(testPos.x + (addMovePowerVector.x < 0.0f ? -leftRightWallLen : leftRightWallLen), position.y, testPos.z));
		}

		{
			yRay.SetInversFlg(addMovePowerVector.y < 0.0f);
			yRay.SetBaseDirection(ChVec3(0.0f, -1.0f, 0.0f));
			yRay.SetMaxLength(std::abs(addMovePowerVector.y) + 5.0f);
			yRay.SetPosition(ChVec3(position.x, testPos.y, position.z));
		}

		{
			zRay.SetInversFlg(addMovePowerVector.z < 0.0f);
			zRay.SetBaseDirection(ChVec3(0.0f, 0.0f, -1.0f));
			zRay.SetMaxLength(std::abs(addMovePowerVector.z) + frontBackWallLen);
			zRay.SetPosition(ChVec3(testPos.x, position.y, testPos.z + (addMovePowerVector.z < 0.0f ? -frontBackWallLen : frontBackWallLen)));

		}

		xRay.Start();
		yRay.Start();
		zRay.Start();

		for (auto&& model : FieldList())
		{
			auto  tmpThread = ChPtr::Make_S< ChCpp::ChMultiThread>();

			tmpThread->Init([&]() {
				auto obj = model->model.lock();
				if (obj == nullptr)return;

				ChCpp::PolygonCollider polygon;
				polygon.SetModel(*obj);
				polygon.SetMatrix(model->transform);
				polygon.SetRightHandFlg();

				yRay.IsHitField(polygon);
				xRay.IsHitField(polygon);
				zRay.IsHitField(polygon);
				
			});
			threadList.push_back(tmpThread);
		}

		for (auto&& thread : threadList)
		{
			thread->Join();
		}

		if (yRay.IsHit())
		{
			addMovePowerVector.y -= (!yRay.IsInvers() ? yRay.GetVecSize() : -yRay.GetVecSize());
			isWallFlg = true;
			isGroundFlg = yRay.IsInvers();
		}

		if (xRay.IsHit())
		{
			addMovePowerVector.x -= (!xRay.IsInvers() ? xRay.GetVecSize() : -xRay.GetVecSize());
			isWallFlg = true;
		}

		if (zRay.IsHit())
		{
			addMovePowerVector.z -= (!zRay.IsInvers() ? zRay.GetVecSize() : -zRay.GetVecSize());
			isWallFlg = true;
		}

		xRay.End();
		yRay.End();
		zRay.End();

		return;

	}

	std::vector<ChPtr::Shared<ChCpp::ChMultiThread>>threadList;

	for (auto&& partsPtr : baseModels)
	{

		auto thread = ChPtr::Make_S<ChCpp::ChMultiThread>();

		thread->Init([&] {

			if (partsPtr->model.expired())return;

			auto parts = partsPtr->model.lock();

			auto comList = parts->GetComponents<ChCpp::FrameComponent>();

			if (comList.empty())return;

			});
	}
}

ChPtr::Shared<PhysicsFunction> PhysicsMachine::CreatePhysics(unsigned long _physicsFunctionlistName)
{
	static std::function<ChPtr::Shared<PhysicsFunction>()>creater[]
	{
		[]()->ChPtr::Shared<PhysicsFunction> {return ChPtr::Make_S<Physics_Gravity>(); },
		[]()->ChPtr::Shared<PhysicsFunction> {return ChPtr::Make_S<Physics_AirPower>(); },
		[]()->ChPtr::Shared<PhysicsFunction> {return ChPtr::Make_S<Physics_WaterPower>(); },
		[]()->ChPtr::Shared<PhysicsFunction> {return ChPtr::Make_S<Physics_GroundPower>(); }
	};

	return creater[_physicsFunctionlistName]();
}

void PhysicsMachine::TestFieldRange()
{

	if (FieldSize().ElementsLen() <= 0.0f)return;

	ChVec3 testPos = position + addMovePowerVector;
	testPos.y += groundHeight;

	RangeTest(addMovePowerVector.x, testPos.x, FieldSize().x);
	
	RangeTest(addMovePowerVector.y, testPos.y, FieldSize().y);

	RangeTest(addMovePowerVector.z, testPos.z, FieldSize().z);

}

void PhysicsMachine::RangeTest(float& _addMovePowerVectorElement, const float _testPosElement, const float _fieldSizeElement)
{
	float testFieldSizeElement = std::abs(_fieldSizeElement);
	float testPosElement = std::abs(_testPosElement);

	if (std::abs(_fieldSizeElement) < std::abs(testPosElement))
	{
		_addMovePowerVectorElement += _addMovePowerVectorElement > 0.0f ?
			-(testPosElement - testFieldSizeElement) :
			(testPosElement - testFieldSizeElement);
	}
}

void PhysicsMachine::HitTestEmptyModelAndField()
{
	if (!baseModels.empty())return;
	if (!FieldList().empty())return;

	float testPos = position.y + addMovePowerVector.y;
	if (testPos < groundHeight)
	{
		addMovePowerVector.y = addMovePowerVector.y - testPos;
		isGroundFlg = true;
	}

}

void PhysicsMachine::FieldHitTestRay::Start()
{
	isHit = false;
	ray.SetRayDir((!inversFlg ? baseDir : baseDir * -1.0f));
	vecSize = 0.0f;
}

void PhysicsMachine::FieldHitTestRay::IsHitField(ChCpp::PolygonCollider& _polygon)
{

	if (!_polygon.IsHit(&ray))return;

	float hitLen = ray.GetHitVectol().Len();

	if (hitLen < vecSize)return;

	vecSize = hitLen;
	isHit = true;
}
