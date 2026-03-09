
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "NextPosBase.h"

void NextPosBase::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto target = LookObj<MechaParts>();

	if (ChPtr::NullCheck(target))return;

	auto&& posObjects = target->GetMesh().GetAllChildlenForName<ChCpp::FrameObject<wchar_t>>(nextPosName);

	if (posObjects.empty())return;

	ChPtr::Shared<ChCpp::FrameObject<wchar_t>> posObject = posObjects[0].lock();

	SetObjectPos(_base, _parts, posObject);
}
