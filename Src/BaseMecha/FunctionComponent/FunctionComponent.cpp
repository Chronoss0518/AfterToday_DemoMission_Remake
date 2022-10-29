
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"FunctionComponent.h"


void FunctionComponent::Init()
{
	target = LookObj<BaseMecha>();

	if (!ChPtr::NullCheck(target))return;

	Destroy();
}
