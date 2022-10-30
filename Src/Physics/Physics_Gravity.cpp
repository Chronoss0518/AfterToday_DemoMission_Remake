#include"../BaseIncluder.h"
#include"../AllStruct.h"

#include"Physics_Gravity.h"

void Physics_Gravity::Update()
{
	nowPower += acceleration;
}