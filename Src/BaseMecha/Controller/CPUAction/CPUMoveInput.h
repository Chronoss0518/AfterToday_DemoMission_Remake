#pragma once

#include"../ControllerBase.h"

class CPUTargetSelector;
class CPUMovePositionSelector;
class GameFrame;

class CPUMoveInput
{
public:

	inline float GetNearTestLength() { return nearTestLength; }

public:

	void Update(
		CPUController& _controller, 
		CPUTargetSelector& _targetSelector,
		CPUMovePositionSelector& _movePositionSelector,
		GameFrame& _frame);

private:
	
	void MoveBattleTarget(CPUController& _controller,bool _isNearThePositionFlg, const ChVec3& _dir);

	void MoveUnButtleTarget(CPUController& _controller, bool _isFarThePositionFlg, const ChVec3& _dir);

private:

	float nearTestLength = 1.0f;

	unsigned long targetNo = -1;
	bool battleFlg = false;

	float battleTargetNearLength = 10.0f;
	
	float useBoostLengthFromBattle = 100.0f;

	float useBoostLengthFromUnBattle = 50.0f;

};