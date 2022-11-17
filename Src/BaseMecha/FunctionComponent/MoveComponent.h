#pragma once

#include"FunctionComponent.h"

class MoveComponent :public FunctionComponent
{
public:

	void Update()override;

public:

	void SetMovePow(const float _movePow) { movePow = _movePow; }

	void SetRotatePow(const float _rotatePow) { rotatePow = _rotatePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

private:

	void MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid,const ChVec3& _direction,const ChLMat& _nowTargetPoster);
	
	void RotateUpdate(InputName _input, const ChVec3& _direction);

private:

	float movePow = 0.0f;
	float jumpPow = 0.0f;
	float rotatePow = 0.0f;

	InputName inputName = InputName::None;
	InputName boostName = InputName::None;
	InputName avoidName = InputName::None;
};