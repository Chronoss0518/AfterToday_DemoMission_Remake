
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

	xRay.SetBaseDirection(ChVec3(-1.0f, 0.0f, 0.0f));
	yRay.SetBaseDirection(ChVec3(0.0f, -1.0f, 0.0f));
	zRay.SetBaseDirection(ChVec3(0.0f, 0.0f, -1.0f));


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

#if true

	sphereCollider.SetPosition(position);
	float moveSize = addMovePowerVector.GetLen();
	if (moveSize <= 0.0f)return;
	sphereCollider.SetSize(groundHeight + moveSize);

	auto&& fieldCollider = FieldCollider();

	if (!fieldCollider.IsHit(&sphereCollider))return;

	auto&& min = fieldCollider.GetHitMaxVector();
	auto&& max = fieldCollider.GetHitMinVector();

	min *= -1.0f;
	max *= -1.0f;

	ChVec3 tmpPos = position + addMovePowerVector;

	if (min.y > tmpPos.y)
	{
		addMovePowerVector.y = min.y;
		isGroundFlg = true;
	}

	if (min.x > tmpPos.x)
	{
		addMovePowerVector.x = min.x;
		isWallFlg = true;
	}

	if (min.z > tmpPos.z)
	{
		addMovePowerVector.z = min.z;
		isWallFlg = true;
	}

	if (max.y < tmpPos.y)
	{
		addMovePowerVector.y = max.y;
		isWallFlg = true;
	}

	if (max.x < tmpPos.x)
	{
		addMovePowerVector.x = max.x;
		isWallFlg = true;
	}

	if (max.z < tmpPos.z)
	{
		addMovePowerVector.z = max.z;
		isWallFlg = true;
	}

	return;
#endif

	ChVec3 testPowerPos = addMovePowerVector;

	testPowerPos.x = testPowerPos.x < 0.0f ? -1.0f : 1.0f;
	testPowerPos.y = testPowerPos.y < 0.0f ? -1.0f : 1.0f;
	testPowerPos.z = testPowerPos.z < 0.0f ? -1.0f : 1.0f;

	ChVec3 testPos = position + testPowerPos;

	xRay.SetInversFlg(addMovePowerVector.x < 0.0f);
	yRay.SetInversFlg(addMovePowerVector.y < 0.0f);
	zRay.SetInversFlg(addMovePowerVector.z < 0.0f);

	xRay.Start();
	yRay.Start();
	zRay.Start();

	float tmpX = 1000.0f, tmpY = 1000.0f, tmpZ = 1000.0f;
	bool tmpXFlg = false, tmpYFlg = false, tmpZFlg = false;


#if false
	//へこみ方向のチェック//
	xRay.SetPosition(ChVec3(testPos.x, position.y, position.z));
	yRay.SetPosition(ChVec3(position.x, yRay.IsInvers() ? testPos.y + groundHeight : testPos.y, position.z));
	zRay.SetPosition(ChVec3(position.x, position.y, testPos.z));

	yRay.IsHitField(FieldCollider());
	xRay.IsHitField(FieldCollider());
	zRay.IsHitField(FieldCollider());

	if (xRay.IsHit())
	{
		tmpX = xRay.GetVecSize();
		tmpXFlg = true;
	}

	if (yRay.IsHit())
	{
		tmpY = yRay.GetVecSize();
		tmpYFlg = true;
	}

	if (zRay.IsHit())
	{
		tmpZ = zRay.GetVecSize();
		tmpZFlg = true;
	}
#endif
	//突方向のチェック//

	xRay.SetPosition(ChVec3(testPos.x, testPos.y, testPos.z));
	yRay.SetPosition(ChVec3(testPos.x, yRay.IsInvers() ? testPos.y + groundHeight: testPos.y, testPos.z));
	zRay.SetPosition(ChVec3(testPos.x, testPos.y, testPos.z));

	yRay.IsHitField(FieldCollider());
	xRay.IsHitField(FieldCollider());
	zRay.IsHitField(FieldCollider());


	if (xRay.IsHit())
	{
		tmpX = tmpX < xRay.GetVecSize() ? tmpX : xRay.GetVecSize();
		tmpXFlg = true;
	}

	if (yRay.IsHit())
	{
		tmpY = tmpY < yRay.GetVecSize() ? tmpY : yRay.GetVecSize();
		tmpYFlg = true;
	}

	if (zRay.IsHit())
	{
		tmpZ = tmpZ < zRay.GetVecSize() ? tmpZ : zRay.GetVecSize();
		tmpZFlg = true;
	}

	xRay.End();
	yRay.End();
	zRay.End();

	if (tmpYFlg)
	{
		float tmpValue = std::abs(addMovePowerVector.y) - std::abs(tmpY);
		if (tmpValue > 0.0f)
		{
			addMovePowerVector.y -= addMovePowerVector.y < 0.0f ?  -tmpValue : tmpValue;
			isWallFlg = !yRay.IsInvers();
			isGroundFlg = yRay.IsInvers();
		}
	}

	if (tmpXFlg)
	{
		if (std::abs(xRay.GetVecSize() - testPowerPos.x) < std::abs(addMovePowerVector.x))
		{
			addMovePowerVector.x = 0.0f;
			isWallFlg = true;
		}
	}

	if (tmpZFlg)
	{
		if (std::abs(zRay.GetVecSize() - testPowerPos.z) < std::abs(addMovePowerVector.z))
		{
			addMovePowerVector.z = 0.0f;
			isWallFlg = true;
		}
	}

	return;


}

void PhysicsMachine::AddField(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _fieldModel)
{
	if (_fieldModel == nullptr)return;
	FieldObject().SetChild(_fieldModel);
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
	//ChVec3 testPos = position + addMovePowerVector;
	ChVec3 testPos = position;
	testPos.y += groundHeight;

	bool test = false;

	RangeTest(test,addMovePowerVector.x, testPos.x, FieldMaxSize().x, FieldMinSize().x);
	
	RangeTest(isGroundFlg,addMovePowerVector.y, testPos.y, FieldMaxSize().y, FieldMinSize().y);

	RangeTest(test,addMovePowerVector.z, testPos.z, FieldMaxSize().z, FieldMinSize().z);

}

void PhysicsMachine::RangeTest(bool& _isGroundOutSide,float& _addMovePowerVectorElement, const float _testPosElement, const float _fieldMaxElement, const float _fieldMinElement)
{
	if (_fieldMaxElement < _testPosElement + _addMovePowerVectorElement)
	{
		_addMovePowerVectorElement -= _testPosElement + _addMovePowerVectorElement - _fieldMaxElement;
		isOutSide = true;
	}

	if (_fieldMinElement > _testPosElement + _addMovePowerVectorElement)
	{
		_addMovePowerVectorElement -= (_testPosElement + _addMovePowerVectorElement) - _fieldMinElement;
 		isOutSide = true;
		_isGroundOutSide = true;
	}
}

void PhysicsMachine::HitTestEmptyModelAndField()
{
	if (!FieldObject().GetAllChildlen().empty())return;

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
	ray.SetMaxLength();
	vecSize = 1000.0f;
}

void PhysicsMachine::FieldHitTestRay::IsHitField(ChCpp::PolygonCollider<wchar_t>& _polygon)
{

	if (!_polygon.IsHit(&ray))return;

 	float hitLen = ray.GetHitVector().GetLen();

	if (isHit && hitLen > vecSize)return;

	vecSize = hitLen;
	isHit = true;
}
