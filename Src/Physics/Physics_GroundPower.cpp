
#include"../BaseIncluder.h"
#include"../AllStruct.h"

#include"PhysicsMachine.h"
#include"Physics_GroundPower.h"

void Physics_GroundPower::CreateMap()
{

}

void Physics_GroundPower::Test()
{

}

void Physics_GroundPower::AddPower()
{

}

void Physics_GroundPower::AddMoveRegist()
{
	if (!IsGround())return;

	auto vec = GetAddMovePowerVector();
	vec.y = 0.0f;
	vec *= -0.2f;

	AddRegistMovePowerVector(vec);
}

void Physics_GroundPower::AddRotateRegist()
{

}
