
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"MoveComponent.h"

void MoveComponentBase::Update()
{
	ChLMat tmp;

	//tmp.SetRotationXAxis(ChMath::ToRadian(GetRotation().x));
	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));

	MoveUpdate(movePow, InputName::Front, InputName::FrontBoost, InputName::FrontAvo, ChVec3(0.0f, 0.0f, 1.0f), tmp);
	MoveUpdate(movePow, InputName::Back, InputName::BackBoost, InputName::BackAvo, ChVec3(0.0f, 0.0f, -1.0f), tmp);
	MoveUpdate(movePow, InputName::Left, InputName::LeftBoost, InputName::LeftAvo, ChVec3(-1.0f, 0.0f, 0.0f), tmp);
	MoveUpdate(movePow, InputName::Right, InputName::RightBoost, InputName::RightAvo, ChVec3(1.0f, 0.0f, 0.0f), tmp);
	MoveUpdate(jumpPow, InputName::Jump, InputName::UpBoost, InputName::UpAvo, ChVec3(0.0f, 1.0f, 0.0f), tmp);

	CamVerticalRotateUpdate(InputName::CameraUpRotation, cameraRotatePow);
	CamVerticalRotateUpdate(InputName::CameraDownRotation, -cameraRotatePow);

	CamHorizontalRotateUpdate(InputName::CameraRightRotation, cameraRotatePow);
	CamHorizontalRotateUpdate(InputName::CameraLeftRotation, -cameraRotatePow);

	RotateUpdate(rotatePow, InputName::UpRotation, ChVec3(1.0f, 0.0f, 0.0f));
	RotateUpdate(rotatePow, InputName::DownRotation, ChVec3(-1.0f, 0.0f, 0.0f));
	RotateUpdate(rotatePow, InputName::RightRotation, ChVec3(0.0f, -1.0f, 0.0f));
	RotateUpdate(rotatePow, InputName::LeftRotation, ChVec3(0.0f, 1.0f, 0.0f));

}

void BaseMechaMoveComponent::MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{

	if (!IsPushFlg(_input))return;

	if (IsPushFlg(InputName::Avo)) SetPushFlg(_avoid);

	else if (IsPushFlg(InputName::Boost)) SetPushFlg(_boost);

	if (!IsGround())return;

	AddMoveVector(_nowTargetPoster.TransformCoord(_direction) * _pow);

}

void BaseMechaMoveComponent::RotateUpdate(float _pow, InputName _input, const ChVec3& _direction)
{
	if (_input == InputName::UpRotation || _input == InputName::DownRotation)return;

	if (!IsPushFlg(_input))return;

	auto vector = _direction;

	vector *= _pow;

	AddRotateVector(vector);
}

void BaseMechaMoveComponent::CamVerticalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewRotateVertical(_camRot / PhysicsMachine::GetFPS());
}

void BaseMechaMoveComponent::CamHorizontalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	if (_input == InputName::CameraRightRotation)SetPushFlg(InputName::RightRotation);

	if (_input == InputName::CameraLeftRotation)SetPushFlg(InputName::LeftRotation);
}
