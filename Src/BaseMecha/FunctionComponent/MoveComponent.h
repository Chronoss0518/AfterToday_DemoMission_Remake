#pragma once

#include"FunctionComponent.h"

class MoveComponentBase :public FunctionComponent
{
public:

	void SetMovePow(const float _movePow) { movePow = _movePow; }

	void SetRotatePow(const float _rotatePow) { rotatePow = _rotatePow; }

	void SetCameraRotatePow(const float _rotatePow) { cameraRotatePow = _rotatePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

protected:

	inline float GetMovePow() { return movePow; }

	inline float GetRotatePow() { return rotatePow; }

	inline float GetCameraRotatePow() { return cameraRotatePow; }

	inline float GetJumpPow() { return jumpPow; }

protected:

	void CamVerticalRotateUpdate(InputName _input, const float _camRot);

	void CamHorizontalRotateUpdate(InputName _input, const float _camRot);

protected:

	void IsAvoBoostTest(InputName _input, InputName _boost, InputName _avoid);

private:

	float movePow = 0.0f;
	float jumpPow = 0.0f;
	float rotatePow = 0.0f;
	float cameraRotatePow = 50.0f;

};

//êlå^ìãèÊï∫äÌ//
class BaseMechaMoveComponent :public MoveComponentBase
{
public:

	void Update()override;

private:

	void MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid, const ChVec3& _direction, const ChLMat& _nowTargetPoster);

	void RotateUpdate(float _pow, InputName _input, const ChVec3& _direction);


};

//ëD//
class ShipMoveComponent :public MoveComponentBase
{
public:

	void Update()override;

private:
};

//êÌé‘//
class TankMoveComponent :public MoveComponentBase
{
public:

	void Update()override;

private:

	void FlagTest();

	void FrontBackTest(InputName _frontBack, InputName _right, InputName _left);

	void RemoveTest(InputName _front, InputName _back);

	void MoveUpdate(float _movePower, InputName _right,InputName _left,const ChVec3& _direction, const ChLMat& _nowTargetPoster);

	void OneSideMoveUpdate(
		float _rotatePower,
		InputName _move,
		InputName _oneSideFront,
		InputName _oneSideBack,
		const ChVec3& _moveDirection,
		const ChVec3& _rotateDirection,
		const ChLMat& _nowTargetPoster);

	void RotationUpdate(float _rotatePower, InputName _right, InputName _left, const ChVec3& _direction);
};

//êÌì¨ã@//
class FighterMoveComponent :public MoveComponentBase
{
public:

	void Update()override;

private:

	void MoveFront(float _pow, InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster);

	void MoveSide(float _pow, InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster);

	void RotateUpDown(float _pow, InputName _input, const ChVec3& _direction);

	void RotateZRightZLeft(float _pow, InputName _input, const ChVec3& _direction);

private:

};
