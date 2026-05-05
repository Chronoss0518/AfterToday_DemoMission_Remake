
#include<Windows.h>

#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

#include"PhysicsMachine.h"
#include"Physics_AirPower.h"

void Physics_AirPower::CreateMap()
{

}

void Physics_AirPower::Test()
{

}

void Physics_AirPower::AddPower()
{

}

void Physics_AirPower::AddMoveRegist()
{
	auto res = GetAddMovePowerVector();

	float speed = res.GetLen();

	res = res * -1.0f;

	res.Normalize();

	float airRegist = GetAirRegist();

	res *= speed * airRegist;

	AddRegistMovePowerVector(res);

}

void Physics_AirPower::AddRotateRegist()
{
	auto res = GetAddRotatePowerVector();

	float speed = res.GetLen();

	res = res * -1.0f;

	res.Normalize();

	float airRegist = GetAirRegist();

	res *= speed * airRegist;

	AddRegistRotatePowerVector(res);

}
