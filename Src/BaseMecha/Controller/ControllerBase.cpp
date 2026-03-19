#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../BaseMecha.h"

#include"ControllerBase.h"


void ControllerBase::Input(const InputName _inputFlgName)
{
	auto targetMecha = GetBaseMecha();
	if (ChPtr::NullCheck(targetMecha))Destroy();

	targetMecha->SetPushFlg(_inputFlgName);
}

