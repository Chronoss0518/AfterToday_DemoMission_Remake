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

public:

	inline void AddMoveVector(const ChVec3& _moveVecAdd) { target->AddMoveVector(_moveVecAdd); }
	
	inline void AddRotateVector(const ChVec3& _rotateVecAdd) { target->AddRotateVector(_rotateVecAdd); }

public:

	inline void SetPushFlg(InputName _name){ target->inputFlgs.SetBitTrue(ChStd::EnumCast(_name)); }

public:

	ChVec3 GetPosition() { return target->pos; }

	ChVec3 GetRotation() { return target->rot; }

	unsigned long GetNowEnelgy() { return target->nowEnelgy; }

private:

	BaseMecha* target = nullptr;

};