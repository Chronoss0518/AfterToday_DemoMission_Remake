
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include"PostureData.h"

void PostureData::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	auto&& pos = _targetObject->GetComponent<PostureController>();
	if (pos != nullptr)return;
	pos = _targetObject->SetComponent<PostureController>();
	pos->Set(posture);

	auto&& baseParts = LookObj<MechaParts>();
	if (baseParts == nullptr)return;
	baseParts->AddPosture(pos);
}
