
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../MechaParts.h"
#include"../MechaPartsObject.h"
#include"MoveComponent.h"


void MoveComponent::Init()
{
	target = LookObj<BaseMecha>();

	if (!ChPtr::NullCheck(target))return;

	Destroy();
}
