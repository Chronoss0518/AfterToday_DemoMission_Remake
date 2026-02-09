#pragma once

class CPUObjectLooker;
class RightWeaponComponent;
class LeftWeaponComponent;

class CPUActionBase;
class CPUWeaponSelector;
class CPUTargetSelector;
class CPUMovePositionSelector;
class CPUAttack;
class CPUAvoidTest;
class CPUMoveInput;

class ControllerBase:public ChCpp::BaseComponent
{
public:

	using InputName = BaseMecha::InputName;

public:

	static constexpr unsigned long EMPTY_TEAM_NO = 0xffffffff;

public:

	virtual void Input(const InputName _inputFlgName);

	inline void SetTeamNo(unsigned long _team) { teamNo = _team; }

	inline unsigned long GetTeamNo() { return teamNo; }

	inline BaseMecha* GetBaseMecha() { return LookObj<BaseMecha>(); }

private:

	unsigned long teamNo = EMPTY_TEAM_NO;

};

