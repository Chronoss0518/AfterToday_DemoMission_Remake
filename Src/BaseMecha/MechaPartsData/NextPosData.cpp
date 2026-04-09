
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "NextPosData.h"

void NextPosData::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	auto&& mechaParts = LookObj<MechaParts>();

	ChVec4 tmpUp = connectionRotateUp;
	tmpUp.w = 0.0f;
	tmpUp.Normalize();
	ChVec4 tmpNormal = connectionRotateNormal;
	tmpNormal.w = 0.0f;
	tmpNormal.Normalize();

	ChVec4 cross = ChVec3::GetCross(tmpUp, tmpNormal);
	cross.w = 0.0f;
	ChLMat lmat;
	lmat.m[0].Set(cross.val);
	lmat.m[1].Set(tmpUp.val);
	lmat.m[2].Set(tmpNormal.val);

	mechaParts->AddPosition(connectionName, _targetObject, lmat, type, maxWeight);
}
