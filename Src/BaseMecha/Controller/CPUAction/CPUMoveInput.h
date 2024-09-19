#pragma once

#include"../ControllerBase.h"

class CPUTargetSelector;
class CPUMovePositionSelector;
class GameFrame;

class CPUMoveInput
{
public:

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>> Serialize();

	void Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject);

	void Update(
		CPUController& _controller, 
		CPUTargetSelector& _targetSelector,
		CPUMovePositionSelector& _movePositionSelector,
		GameFrame& _frame);

private:
	
	void MoveBattleTarget(CPUController& _controller,bool _isBattleFlg, const ChVec3& _dir, bool _isLookTarget);

	void MoveUnButtleTarget(CPUController& _controller, bool _isBattleFlg, const ChVec3& _dir);

private:

	unsigned long targetNo = -1;
	bool battleFlg = false;

	ControllerBase::InputName beforeCameraRotationInputName;

	float lookTweakSize = 0.8f;

	float targetPositionLength = 100.0f;
	
	float useBoostLengthFromBattle = 200.0f;

	float useBoostLengthFromUnBattle = 400.0f;

};