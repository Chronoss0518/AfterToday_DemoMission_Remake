#pragma once

#include"../BaseMecha.h"

class FunctionComponent :public ChCpp::BaseComponent
{
public:

	using InputName = BaseMecha::InputName;

public://override function//

	void Init()override;

public:

	inline bool IsPushFlg(InputName _name) { return target->IsPushFlg(_name); }

	bool IsGround();

protected:

	inline void AddMoveVector(const ChVec3& _moveVecAdd) { target->AddMoveVector(_moveVecAdd); }
	
	inline void AddRotateVector(const ChVec3& _rotateVecAdd) { target->AddRotateVector(_rotateVecAdd); }

	inline void SetSelfViewRotateHorizontalFlg(const bool& _flg) { target->SetSelfViewHorizontalFlg(_flg); }

protected:

	inline void SetPushFlg(InputName _name){ target->SetPushFlg(_name); }

	inline void RemovePushFlg(InputName _name) { target->inputFlgs.SetBitFalse(ChStd::EnumCast(_name)); }

protected:

	ChVec3 GetPosition() { return target->GetPosition(); }

	ChVec3 GetRotation() { return target->GetRotation(); }

	ChVec3 GetMoveVector() { return target->GetMoveVector(); }

	ChVec3 GetRotateVector() { return target->GetRotateVector(); }

	inline BaseMecha* GetBaseMecha() { return target; }

protected:

	template<class T>
	ChPtr::Shared<T> GetComponent()
	{
		if (ChPtr::NullCheck(target))return nullptr;
		return target->GetComponentObject<T>();
	}

private:

	BaseMecha* target = nullptr;

};