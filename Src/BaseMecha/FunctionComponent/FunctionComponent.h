#pragma once

#include"../BaseMecha.h"

class FunctionComponent :public ChCpp::BaseComponent
{
public:

	using InputName = BaseMecha::InputName;

public://override function//

	void Init()override;

public:

	inline bool IsPushFlg(InputName _name) { return target->inputFlgs.GetBitFlg(ChStd::EnumCast(_name)); }

	bool IsGround();

public:

	inline void AddMoveVector(const ChVec3& _moveVecAdd) { target->physics->AddMovePowerVector(_moveVecAdd); }
	
	inline void AddRotateVector(const ChVec3& _rotateVecAdd) { target->physics->AddRotatePowerVector(_rotateVecAdd); }

	inline void AddViewRotateVertical(const float& _viewRotate) { target->AddViewVertical(_viewRotate); }

	inline void AddViewRotateHorizontal(const float& _viewRotate) { target->AddViewHorizontal(_viewRotate); }

public:

	inline void SubNowEnelgy(unsigned long _use) { target->nowEnelgy -= (target->nowEnelgy - _use) > 0 ? _use : target->nowEnelgy; }

public:

	inline void SetPushFlg(InputName _name){ target->inputFlgs.SetBitTrue(ChStd::EnumCast(_name)); }

public:

	ChVec3 GetPosition() { return target->GetPosition(); }

	ChVec3 GetRotation() { return target->GetRotation(); }

	ChVec3 GetMoveVector() { return target->physics->GetAddMovePowerVector(); }

	ChVec3 GetRotateVector() { return target->physics->GetAddRotatePowerVector(); }

	unsigned long GetNowEnelgy() { return target->nowEnelgy; }


private:

	BaseMecha* target = nullptr;

};