
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"MoveComponent.h"

void MoveComponent::Update()
{
	ChLMat tmp;

	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));

	MoveUpdate(movePow, InputName::Front, InputName::FrontBoost, InputName::FrontAvo, ChVec3(0.0f, 0.0f, 1.0f), tmp);
	MoveUpdate(movePow, InputName::Back, InputName::BackBoost, InputName::BackAvo, ChVec3(0.0f, 0.0f, -1.0f), tmp);
	MoveUpdate(movePow, InputName::Left, InputName::LeftBoost, InputName::LeftAvo, ChVec3(-1.0f, 0.0f, 0.0f), tmp);
	MoveUpdate(movePow, InputName::Right, InputName::RightBoost, InputName::RightAvo, ChVec3(1.0f, 0.0f, 0.0f), tmp);
	MoveUpdate(jumpPow, InputName::Jump, InputName::UpBoost, InputName::UpAvo, ChVec3(0.0f, 1.0f, 0.0f), tmp);

	RotateUpdate(InputName::LeftRotation, ChVec3(0.0f, -1.0f, 0.0f));
	RotateUpdate(InputName::RightRotation, ChVec3(0.0f, 1.0f, 0.0f));

}

void MoveComponent::MoveUpdate(float _pow, InputName _input, InputName _boost, InputName _avoid, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{

	if (!IsPushFlg(_input))return;

	if (IsPushFlg(InputName::Avo)) SetPushFlg(_avoid);

	else if (IsPushFlg(InputName::Boost)) SetPushFlg(_boost);

	if (!IsGround())return;

	unsigned long fps = ChSystem::SysManager().GetFPS();

	AddMoveVector(_nowTargetPoster.TransformCoord(_direction) * _pow / fps);

}

void MoveComponent::RotateUpdate(InputName _input, const ChVec3& _direction)
{
	if (!IsPushFlg(_input))return;

	unsigned long fps = ChSystem::SysManager().GetFPS();

	AddRotateVector(_direction * rotatePow / fps);
}
