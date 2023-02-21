#include"../../../BaseIncluder.h"

#include"../../BaseMecha.h"

#include"../../../Frames/GameFrame.h"

#include"CPUFunctions.h"

ChStd::Bool CPUAttackFunction::Update(CPUController& _controller)
{
	return IsUnStopFlg() == false;
}

ChStd::Bool CPUAvoidFunction::Update(CPUController& _controller)
{
	return IsUnStopFlg() == false;;
}

ChStd::Bool CPUMoveFunction::Update(CPUController& _controller)
{
	return IsUnStopFlg() == false;
}

ChStd::Bool CPUBoostFunction::Update(CPUController& _controller)
{
	return IsUnStopFlg() == false;;
}
