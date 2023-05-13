#pragma once

#include"FunctionComponent.h"

class MoveComponentBase :public FunctionComponent
{
protected:

	enum class UsingCameraRotateAxisType
	{
		Horizontal,//êÖïΩ//
		Vertical//êÇíº//
	};

public:

	void Update()override;

public:

	void SetMovePow(const float _movePow) { movePow = _movePow; }

	void SetRotatePow(const float _rotatePow) { rotatePow = _rotatePow; }

	void SetCameraRotatePow(const float _rotatePow) { cameraRotatePow = _rotatePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

	void SetUsingCameraRotateAxisFlg(bool _flg, UsingCameraRotateAxisType _type);

protected:

	virtual void MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid, const ChVec3& _direction, const ChLMat& _nowTargetPoster) = 0;

	virtual void RotateUpdate(float _pow, InputName _input, const ChVec3& _direction) = 0;

	void CamVerticalRotateUpdate(InputName _input, const float _camRot);

	void CamHorizontalRotateUpdate(InputName _input, const float _camRot);

private:

	float movePow = 0.0f;
	float jumpPow = 0.0f;
	float rotatePow = 0.0f;
	float cameraRotatePow = 50.0f;

	ChCpp::BitBool usingCameraRotateAxisFlg;
};

class BaseMechaMoveComponent :public MoveComponentBase
{
public:

	void Init()override;

private:

	void MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid,const ChVec3& _direction,const ChLMat& _nowTargetPoster)override;
	
	void RotateUpdate(float _pow, InputName _input, const ChVec3& _direction)override;

};