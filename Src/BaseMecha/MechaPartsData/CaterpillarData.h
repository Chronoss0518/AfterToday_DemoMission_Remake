#pragma once

#include"MoveDataBase.h"

class TankMoveComponent;

class CaterpillarData :public MoveDataBase
{

public:

	void RemoveParameter(BaseMecha& _base)override;

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	virtual std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	void SetMovePow(const float _movePow) { movePow = _movePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

public://Get Functions//

	float GetGroundNearPos() { return 0.0f; };

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(CaterpillarData); }

	float GetMovePower()const { return movePow; }

	float GetJumpPower()const { return jumpPow; }

protected:

	//ˆÚ“®—Í//
	float movePow = 0.0f;
	//”òãÄ—Í//
	float jumpPow = 0.0f;

	ChPtr::Shared<TankMoveComponent>moveObject = nullptr;

};
