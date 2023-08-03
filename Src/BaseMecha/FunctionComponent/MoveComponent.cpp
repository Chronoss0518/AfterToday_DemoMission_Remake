
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
	MoveUpdate(jumpPow, InputName::Up, InputName::UpBoost, InputName::UpAvo, ChVec3(0.0f, 1.0f, 0.0f), tmp);
	MoveUpdate(jumpPow, InputName::Down, InputName::DownBoost, InputName::DownAvo, ChVec3(0.0f, -1.0f, 0.0f), tmp);

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

	if (_input == InputName::Down)return;
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

void ShipMoveComponent::MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{
	if (_input == InputName::Right || _input == InputName::Left)return;
	if (_input == InputName::Up || _input == InputName::Down)return;

	if (!IsPushFlg(_input))return;

	if (IsPushFlg(InputName::Avo)) SetPushFlg(_avoid);

	else if (IsPushFlg(InputName::Boost)) SetPushFlg(_boost);

	if (!IsGround())return;

	AddMoveVector(_nowTargetPoster.TransformCoord(_direction) * _pow);

}

void ShipMoveComponent::RotateUpdate(float _pow, InputName _input, const ChVec3& _direction)
{
	if (_input == InputName::UpRotation || _input == InputName::DownRotation)return;

	if (!IsPushFlg(_input))return;

	auto vector = _direction;

	vector *= _pow;

	AddRotateVector(vector);
}

void ShipMoveComponent::CamVerticalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewRotateVertical(_camRot / PhysicsMachine::GetFPS());
}

void ShipMoveComponent::CamHorizontalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewRotateHorizontal(_camRot / PhysicsMachine::GetFPS());
}

void TankMoveComponent::MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{
	if (_input == InputName::Right || _input == InputName::Left)return;
	if (_input == InputName::Up || _input == InputName::Down)return;

	if (!IsPushFlg(_input))return;

	if (IsPushFlg(InputName::Avo)) SetPushFlg(_avoid);

	else if (IsPushFlg(InputName::Boost)) SetPushFlg(_boost);

	if (!IsGround())return;

	AddMoveVector(_nowTargetPoster.TransformCoord(_direction) * _pow);
}

void TankMoveComponent::RotateUpdate(float _pow, InputName _input, const ChVec3& _direction)
{
	if (_input == InputName::UpRotation || _input == InputName::DownRotation)return;

	if (!IsPushFlg(_input))return;

	auto vector = _direction;

	vector *= _pow;

	AddRotateVector(vector);
}

void TankMoveComponent::CamVerticalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewRotateVertical(_camRot / PhysicsMachine::GetFPS());
}

void TankMoveComponent::CamHorizontalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewRotateHorizontal(_camRot / PhysicsMachine::GetFPS());
}

void FighterMoveComponent::MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{
	if (!IsPushFlg(_input))return;

	if (IsPushFlg(InputName::Avo)) SetPushFlg(_avoid);

	else if (IsPushFlg(InputName::Boost)) SetPushFlg(_boost);


}

void FighterMoveComponent::RotateUpdate(float _pow, InputName _input, const ChVec3& _direction)
{
	if (!IsPushFlg(_input))return;

	if (_input == InputName::UpRotation || _input == InputName::DownRotation)return;

	ChVec3 vector;

	//yŽ²‰ñ“]‚©‚çzŽ²‰ñ“]‚Ö//
	vector.z = _direction.y;

	vector *= _pow;

	AddRotateVector(vector);
}

void FighterMoveComponent::CamVerticalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewRotateVertical(_camRot / PhysicsMachine::GetFPS());
}

void FighterMoveComponent::CamHorizontalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewRotateHorizontal(_camRot / PhysicsMachine::GetFPS());
}

void FighterMoveComponent::MoveFront(float _pow, InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{
	float movePow = _pow;

	if (_input == InputName::Back)
	{
		float movePower = GetMoveVector().Len();

		if (movePower > movePow)movePower = movePow;
	}

	AddMoveVector(_nowTargetPoster.TransformCoord(_direction) * movePow);
}

void FighterMoveComponent::MoveSide(float _pow, InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{

	if (IsGround())return;
	if (_input == InputName::Front || _input == InputName::Back)return;
	if (_input == InputName::Up || _input == InputName::Down)return;
	
	ChVec3 rotate;
	//xŽ²ˆÚ“®‚©‚çyŽ²‰ñ“]‚Ö//
	rotate.y = _direction.x;
	rotate.Normalize();

	AddRotateVector(_nowTargetPoster.TransformCoord(rotate) * _pow);
}

void FighterMoveComponent::RotateUpDown(float _pow, InputName _input, const ChVec3& _direction)
{
	if (_input == InputName::RightRotation || _input == InputName::LeftRotation)return;

	ChVec3 vector = _direction.y;

	vector *= _pow;

	AddRotateVector(vector);
}

void FighterMoveComponent::RotateZRightZLeft(float _pow, InputName _input, const ChVec3& _direction)
{

	if (_input == InputName::UpRotation || _input == InputName::DownRotation)return;

	ChVec3 vector;

	//yŽ²‰ñ“]‚©‚çzŽ²‰ñ“]‚Ö//
	vector.z = _direction.y;

	vector *= _pow;

	AddRotateVector(vector);
}