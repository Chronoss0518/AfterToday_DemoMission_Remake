
#include"../BaseIncluder.h"
#include"../AllStruct.h"

#include"PhysicsMachine.h"
#include"Physics_MaterialPower.h"

void Physics_MaterialPower::AddMove()
{
	CreateMap();

	Test();

	AddPower();
}

void Physics_MaterialPower::AddRegist()
{
	AddMoveRegist();

	AddRotateRegist();
}
