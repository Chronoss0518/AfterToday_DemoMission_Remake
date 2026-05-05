
#include<Windows.h>

#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

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
	machine->AddMovePowerVector(_addMove);
}

void PhysicsFunction::AddRegistMovePowerVector(const ChVec3& _regist)
{
	machine->registMovePowerVector += _regist;
}

void PhysicsFunction::AddRotatePowerVector(const ChVec3& _addMove)
{
	machine->AddRotatePowerVector(_addMove);
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

float PhysicsFunction::GetGroundRegist()
{
	return PhysicsMachine::GroundRegist();
}

std::vector<ChPtr::Shared<ChVec3>>PhysicsFunction::GetBrustPosList()
{
	return PhysicsMachine::BrustPosList();
}

bool PhysicsFunction::IsGround()
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
	nowTransform.SetRotationYPR(rotation);
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

	ChVec3 testPos = position + addMovePowerVector;

#if 0

	ChCpp::SphereCollider thisCol;
	thisCol.SetPosition(testPos.y);
	thisCol.SetRotation(rotation + addRotatePowerVector);
	thisCol.SetScalling(addMovePowerVector);
	for (auto field : FieldList())
	{
		if (!field->collider.IsHit(&thisCol))continue;

		ChVec3 vector = thisCol.GetHitVectol();

		addMovePowerVector -= vector;

		isWallFlg = true;
		if (vector.y != 0.0f)
		{
			isGroundFlg = true;
		}

	}

#else


	{
		xRay.SetInversFlg(addMovePowerVector.x < 0.0f);
		xRay.SetBaseDirection(ChVec3(-1.0f, 0.0f, 0.0f));
		xRay.SetPosition(ChVec3(testPos.x + addMovePowerVector.x, testPos.y, testPos.z));
	}

	{
		yRay.SetInversFlg(addMovePowerVector.y < 0.0f);
		yRay.SetBaseDirection(ChVec3(0.0f, -1.0f, 0.0f));
		yRay.SetPosition(ChVec3(testPos.x, testPos.y + groundHeight, testPos.z));
	}

	{
		zRay.SetInversFlg(addMovePowerVector.z < 0.0f);
		zRay.SetBaseDirection(ChVec3(0.0f, 0.0f, -1.0f));
		zRay.SetPosition(ChVec3(testPos.x, testPos.y, testPos.z + addMovePowerVector.z));

	}

	xRay.Start();
	yRay.Start();
	zRay.Start();

	for (auto&& model : FieldList())
	{
		auto obj = model->GetModel();
		if (obj == nullptr)return;

		yRay.IsHitField(*model);
		xRay.IsHitField(*model);
		zRay.IsHitField(*model);
	}

	if (yRay.IsHit())
	{
		if (std::abs(yRay.GetVecSize()) < std::abs(addMovePowerVector.y) + 5.0f)
		{
			addMovePowerVector.y -= (!yRay.IsInvers() ? yRay.GetVecSize() : -yRay.GetVecSize());
			isWallFlg = true;
			isGroundFlg = yRay.IsInvers();
		}
	}

	if (xRay.IsHit())
	{
		if (std::abs(xRay.GetVecSize()) < std::abs(addMovePowerVector.x) + leftRightWallLen)
		{
			addMovePowerVector.x -= (!xRay.IsInvers() ? xRay.GetVecSize() : -xRay.GetVecSize());
			isWallFlg = true;
		}
	}

	if (zRay.IsHit())
	{
		if (std::abs(zRay.GetVecSize()) < std::abs(addMovePowerVector.z) + frontBackWallLen)
		{
			addMovePowerVector.z -= (!zRay.IsInvers() ? zRay.GetVecSize() : -zRay.GetVecSize());
			isWallFlg = true;
		}
	}

	xRay.End();
	yRay.End();
	zRay.End();
#endif
	return;


}

void PhysicsMachine::AddField(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _fieldModel, ChCpp::PanelColliderBase::UseHandType _type)
{
	if (_fieldModel == nullptr)return;
	auto field = ChPtr::Make_S<ChCpp::PolygonCollider<wchar_t>>();
	field->SetModel(*_fieldModel);
	field->SetHandType(_type);
	FieldList().push_back(field);
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

	isOutSide = false;
	ChVec3 testPos = position + addMovePowerVector;
	testPos.y += groundHeight;

	RangeTest(addMovePowerVector.x, testPos.x, FieldMaxSize().x, FieldMinSize().x);
	
	RangeTest(addMovePowerVector.y, testPos.y, FieldMaxSize().y, FieldMinSize().y);

	RangeTest(addMovePowerVector.z, testPos.z, FieldMaxSize().z, FieldMinSize().z);

}

void PhysicsMachine::RangeTest(float& _addMovePowerVectorElement, const float _testPosElement, const float _fieldMaxElement, const float _fieldMinElement)
{
	return;
	if (_fieldMaxElement < _testPosElement + _addMovePowerVectorElement)
	{
		_addMovePowerVectorElement = _fieldMaxElement - (_testPosElement + _addMovePowerVectorElement);
		isOutSide = true;
	}

	if (_fieldMinElement > _testPosElement + _addMovePowerVectorElement)
	{
		_addMovePowerVectorElement = (_testPosElement + _addMovePowerVectorElement) - _fieldMinElement;
		isOutSide = true;
	}
}

void PhysicsMachine::HitTestEmptyModelAndField()
{
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

void PhysicsMachine::FieldHitTestRay::IsHitField(ChCpp::PolygonCollider<wchar_t>& _polygon)
{

	if (!_polygon.IsHit(&ray))return;

	float hitLen = ray.GetHitVectol().GetLen();

	if (hitLen < vecSize)return;

	vecSize = hitLen;
	isHit = true;
}
