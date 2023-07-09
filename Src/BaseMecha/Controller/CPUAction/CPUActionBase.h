#pragma once

#include"../ControllerBase.h"

class CPUActionBase
{
public:

	virtual std::string Serialize();

	virtual void Deserialize(const std::string& _text);

	inline void SetActiveFlg(const bool _flg) { activeFlg = _flg; }

	inline void SetMemberType(CPUObjectLooker::MemberType _type) { memberType = _type; }

	inline void SetDistanceType(CPUObjectLooker::DistanceType _type) { distanceType = _type; }

	inline void SetDamageType(CPUObjectLooker::DamageSizeType _type) { damageType = _type; }

	inline void SetTestDistance(const float _distance) { testDistance = _distance; }

	inline void SetDistanceComparison(CPUController::ComparisonOperation _com) { distanceComparison = _com; }

	inline void SetTestDamage(const float _damage) { testDamage = _damage; }

	inline void SetDamageComparison(CPUController::ComparisonOperation _com) { damageComparison = _com; }

	unsigned long GetLookTypeMechas(CPUObjectLooker& _lookTarget);

	virtual bool IsRunTest(unsigned long _lookTarget, GameFrame& _frame,CPUController& _controller);

	inline bool IsActiveFlg() { return activeFlg; }

	inline CPUObjectLooker::MemberType GetMemberType() { return memberType; }

	inline CPUObjectLooker::DistanceType GetDistanceType() { return distanceType; }

	inline CPUObjectLooker::DamageSizeType GetDamageType() { return damageType; }

	inline float GetTestDistance() { return testDistance; }

	inline CPUController::ComparisonOperation GetDistanceComparison() { return distanceComparison; }

	inline float GetTestDamage() { return testDamage; }

	inline CPUController::ComparisonOperation GetDamageComparison() { return damageComparison; }

protected:

	float testDistance = 0.0f;
	CPUController::ComparisonOperation distanceComparison = CPUController::ComparisonOperation::Smaller;

	float testDamage = 0.0f;
	CPUController::ComparisonOperation damageComparison = CPUController::ComparisonOperation::Smaller;

private:

	bool activeFlg = true;

	CPUObjectLooker::MemberType memberType = CPUObjectLooker::MemberType::Enemy;
	CPUObjectLooker::DistanceType distanceType = CPUObjectLooker::DistanceType::None;
	CPUObjectLooker::DamageSizeType damageType = CPUObjectLooker::DamageSizeType::None;

};

