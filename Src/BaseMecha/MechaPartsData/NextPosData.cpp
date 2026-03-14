
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "NextPosData.h"

void NextPosData::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	auto&& mechaParts = LookObj<MechaParts>();

	ChVec3 cross = ChVec3::GetCross(connectionRotateNormal, connectionRotateUp);

	ChLMat lmat;
	lmat.m[0].Set(cross.val);
	lmat.m[1].Set(connectionRotateUp.val);
	lmat.m[2].Set(connectionRotateNormal.val);

	mechaParts->AddPosition(nextPosName, _targetObject, lmat, type, maxWeight);

}
