#include"../BaseIncluder.h"
#include"../AllStruct.h"

#include"Physics_Gravity.h"

void Physics_Gravity::AddMove()
{
	AddMovePowerVector(ChVec3(0.0f, -(GetGravityAcceleration() * GetMass())/ GetFPS(), 0.0f));
}
