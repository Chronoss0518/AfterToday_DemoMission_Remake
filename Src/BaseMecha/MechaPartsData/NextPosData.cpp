
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "NextPosData.h"

void NextPosData::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	auto&& mechaParts = LookObj<MechaParts>();

	mechaParts->AddPosition(nextPosName, _targetObject);

}
