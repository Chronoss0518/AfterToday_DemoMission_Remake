#pragma once

#include"../ControllerBase.h"

class CPUTargetSelector;
class CPUMoveInput;

class CPUMovePositionSelect
{
public:

	std::string Serialize();

	void Deserialize(const std::string& _text);

	void SetOperationPoint(const ChVec3& _point) { point = _point; }

	void SetOperationPointDistance(const float _distance) { if (_distance > 0.0f) distance = _distance; }

	ChVec3 GetOperationPoint() { return point; }

	float GetOperationPointDistance() { return distance; }

	float GetPointLength(const ChVec3& position);

	bool Update(const ChVec3& _position);

private:

	std::string GetValue(unsigned char _no);

	ChVec3 point = 0.0f;
	float distance = 10.0f;

};

class CPUMovePositionSelector : public FunctionsBase<CPUMovePositionSelect>
{
public:

	void Init(bool _insertStartPointFlg, CPUController& _controller,float _distance);

	std::string Serialize()override;

	void Deserialize(const std::string& _text)override;

	void Update(CPUTargetSelector& _selector, GameFrame& _frame, CPUMoveInput& _moveInput,CPUController& _controller);

	inline ChVec3 GetMovePoint() { return point; }

private:

	void UpdateLookTarget(CPUTargetSelector& _selector, GameFrame& _frame);

	void UpdateUnLookTarget(CPUController& _controller, float _nearTestLength);

private:
	
	bool runUpdateLookTargetFlg = false;
	ChVec3 point = 0.0f;

	ChVec3 lastLookPoint = 0.0f;
	bool isButtleFlg = false;

	unsigned long moveOperationPointCount = 0;
};