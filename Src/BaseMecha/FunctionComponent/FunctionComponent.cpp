
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"
#include"../../Physics/PhysicsMachine.h"
#include"FunctionComponent.h"


void FunctionComponent::Init()
{
	target = LookObj<BaseMecha>();

	if (ChPtr::NotNullCheck(target))return;

	Destroy();
}

bool FunctionComponent::IsGround()
{
	target = LookObj<BaseMecha>();

	if (ChPtr::NullCheck(target))return false;

	return target->physics->IsGround();
}
