
#include<Windows.h>

#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

#include"Physics_Gravity.h"

void Physics_Gravity::AddMove()
{
	AddMovePowerVector(ChVec3(0.0f, -(GetGravityAcceleration() * GetMass() / GetFPS()), 0.0f));
}
