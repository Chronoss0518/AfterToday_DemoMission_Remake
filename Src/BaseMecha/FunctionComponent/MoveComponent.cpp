
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"MoveComponent.h"

void MoveComponentBase::Update()
{
	ChLMat tmp;

	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));
	tmp.SetRotationXAxis(ChMath::ToRadian(GetRotation().x));

	MoveUpdate(movePow, InputName::Front, InputName::FrontBoost, InputName::FrontAvo, ChVec3(0.0f, 0.0f, 1.0f), tmp);
	MoveUpdate(movePow, InputName::Back, InputName::BackBoost, InputName::BackAvo, ChVec3(0.0f, 0.0f, -1.0f), tmp);
	MoveUpdate(movePow, InputName::Left, InputName::LeftBoost, InputName::LeftAvo, ChVec3(-1.0f, 0.0f, 0.0f), tmp);
	MoveUpdate(movePow, InputName::Right, InputName::RightBoost, InputName::RightAvo, ChVec3(1.0f, 0.0f, 0.0f), tmp);
	MoveUpdate(jumpPow, InputName::Jump, InputName::UpBoost, InputName::UpAvo, ChVec3(0.0f, 1.0f, 0.0f), tmp);

	RotateUpdate(rotatePow, InputName::UpRotation, ChVec3(1.0f, 0.0f, 0.0f));
	RotateUpdate(rotatePow, InputName::DownRotation, ChVec3(-1.0f, 0.0f, 0.0f));
	RotateUpdate(rotatePow, InputName::RightRotation, ChVec3(0.0f, -1.0f, 0.0f));
	RotateUpdate(rotatePow, InputName::LeftRotation, ChVec3(0.0f, 1.0f, 0.0f));

	CamVerticalRotateUpdate(InputName::CameraUpRotation, cameraRotatePow);
	CamVerticalRotateUpdate(InputName::CameraDownRotation, -cameraRotatePow);

	CamHorizontalRotateUpdate(InputName::CameraRightRotation, cameraRotatePow);
	CamHorizontalRotateUpdate(InputName::CameraLeftRotation, -cameraRotatePow);
}

void MoveComponentBase::SetUsingCameraRotateAxisFlg(bool _flg, MoveComponentBase::UsingCameraRotateAxisType _type)
{
	usingCameraRotateAxisFlg.SetBitFlg(ChStd::EnumCast(_type), _flg);
}

void MoveComponentBase::CamVerticalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;
	
	if (!usingCameraRotateAxisFlg.GetBitFlg(ChStd::EnumCast(UsingCameraRotateAxisType::Vertical)))return;

	AddViewRotateVertical(_camRot / PhysicsMachine::GetFPS());
}

void MoveComponentBase::CamHorizontalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	if (!usingCameraRotateAxisFlg.GetBitFlg(ChStd::EnumCast(UsingCameraRotateAxisType::Horizontal)))return;

	AddViewRotateHorizontal(_camRot / PhysicsMachine::GetFPS());
}


void BaseMechaMoveComponent::Init()
{
	FunctionComponent::Init();

	SetUsingCameraRotateAxisFlg(false, UsingCameraRotateAxisType::Horizontal);
	SetUsingCameraRotateAxisFlg(true, UsingCameraRotateAxisType::Vertical);
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

	AddRotateVector(_direction * _pow);
}
