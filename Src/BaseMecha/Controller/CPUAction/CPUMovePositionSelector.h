#pragma once

#include"../ControllerBase.h"

class CPUTargetSelector;
class CPUMoveInput;

class CPUMovePositionSelect
{
public:

	enum class IgnoreAxisFlg
	{
		X, Y, Z
	};

public:

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>> Serialize();

	void Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject);

	void SetOperationPoint(const ChVec3& _point) { point = _point; }

	inline void SetIgnoreFlg(const bool _flg, IgnoreAxisFlg _axis)
	{
		axisFlg.SetBitFlg(ChStd::EnumCast(_axis), _flg);
	}

	void SetOperationPointDistance(const float _distance) { if (_distance > 0.0f) distance = _distance; }

	ChVec3 GetOperationPoint() { return outPoint; }

	float GetOperationPointDistance() { return distance; }

	float GetPointLength(const ChVec3& position);

	bool Update(const ChVec3& _position);

private:

	ChVec3 point = 0.0f;
	ChVec3 outPoint = 0.0f;
	float distance = 10.0f;

	ChCpp::BitBool axisFlg;

};

class CPUMovePositionSelector : public FunctionsBase<CPUMovePositionSelect>
{
public:

	void SetInitPosition(CPUController& _controller, float _distance, bool _xIgnore, bool _yIgnore, bool _zIgnore);

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>> Serialize()override;

	void Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)override;

	void Update(CPUTargetSelector& _selector, GameFrame& _frame,CPUController& _controller);

	inline ChVec3 GetMovePoint() { return point; }

	inline bool IsBattleFlg() { return isBattleFlg; }

private:

	void UpdateLookTarget(CPUTargetSelector& _selector, GameFrame& _frame);

	void UpdateUnLookTarget(CPUController& _controller);

private:
	
	float isTargetPositionInAreaLength = 20.0f;

	bool runUpdateLookTargetFlg = false;
	ChVec3 point = 0.0f;

	ChVec3 lastLookPoint = 0.0f;
	bool isBattleFlg = false;
	bool isDamageFlg = false;

	unsigned long combatModeOffTime = 2000;
	unsigned long combatModeOffTimeNow = 0;

	ChPtr::Weak<BaseMecha>targetMecha;

	unsigned long moveOperationPointCount = 0;
};