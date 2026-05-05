#include<Windows.h>

#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

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

	auto res = GetAddMovePowerVector();

	res.y = res.y > 0.0f ? 0.0f : res.y;

	float speed = res.GetLen();

	res = res * -1.0f;

	res.Normalize();

	float groundRegist = GetGroundRegist();

	res *= speed * groundRegist;

	AddRegistMovePowerVector(res);
}

void Physics_GroundPower::AddRotateRegist()
{
	if (!IsGround())return;

	auto&& res = GetAddRotatePowerVector();

	float speed = res.GetLen();

	res = res * -1.0f;

	res.Normalize();

	float groundRegist = GetGroundRegist();

	res *= speed * groundRegist;

	AddRegistRotatePowerVector(res);
}
