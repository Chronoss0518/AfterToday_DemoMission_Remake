
#include<Windows.h>

#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>


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
