#pragma once

#include"FunctionComponent.h"

class MoveComponentBase :public FunctionComponent
{
protected:

public:

	void Update()override;

public:

	void SetMovePow(const float _movePow) { movePow = _movePow; }

	void SetRotatePow(const float _rotatePow) { rotatePow = _rotatePow; }

	void SetCameraRotatePow(const float _rotatePow) { cameraRotatePow = _rotatePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

protected:

	virtual void MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid, const ChVec3& _direction, const ChLMat& _nowTargetPoster) = 0;

	virtual void RotateUpdate(float _pow, InputName _input, const ChVec3& _direction) = 0;

	virtual void CamVerticalRotateUpdate(InputName _input, const float _camRot) = 0;

	virtual void CamHorizontalRotateUpdate(InputName _input, const float _camRot) = 0;

private:

	float movePow = 0.0f;
	float jumpPow = 0.0f;
	float rotatePow = 0.0f;
	float cameraRotatePow = 50.0f;

};

class BaseMechaMoveComponent :public MoveComponentBase
{

private:

	void MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid, const ChVec3& _direction, const ChLMat& _nowTargetPoster)override;

	void RotateUpdate(float _pow, InputName _input, const ChVec3& _direction)override;

	void CamVerticalRotateUpdate(InputName _input, const float _camRot)override;

	void CamHorizontalRotateUpdate(InputName _input, const float _camRot)override;


};