
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"MoveComponent.h"

#include"../MechaPartsData/WalkData.h"
#include"../MechaPartsData/CaterpillarData.h"

#define ADD_ROTATE_MAGNIFICATION 3.0f

void MoveComponent::RemoveMoveObject(MoveDataBase* _moveObject)
{
	if (_moveObject == nullptr)return;
	for (int i = 0; i < moveObjectList.size(); i++)
	{
		
		if (!moveObjectList[i]->IsMoveData(_moveObject))continue;

		moveObjectList.erase(moveObjectList.begin() + i);
		return;
	}
}

void MoveComponent::IsAvoBoostTest(InputName _input, InputName _boost, InputName _avoid)
{
	if (!IsPushFlg(_input))return;

	if (IsPushFlg(InputName::Avo)) SetPushFlg(_avoid);

	if (IsPushFlg(InputName::Boost)) SetPushFlg(_boost);
}

void MoveComponent::Update()
{
	if (IsPushFlg(InputName::MoveUpChange))
		UpChangeMoveObject();

	if (IsPushFlg(InputName::MoveDownChange))
		DownChangeMoveObject();


	if (moveObjectList.size() <= nowMoveObject)return;
	auto&& moveObject = moveObjectList[nowMoveObject];

	if (moveObject == nullptr)return;

	moveObject->UpdateBegin();
	

	IsAvoBoostTest(InputName::Front, InputName::FrontBoost, InputName::FrontAvo);
	IsAvoBoostTest(InputName::Back, InputName::BackBoost, InputName::BackAvo);
	IsAvoBoostTest(InputName::Left, InputName::LeftBoost, InputName::LeftAvo);
	IsAvoBoostTest(InputName::Right, InputName::RightBoost, InputName::RightAvo);
	IsAvoBoostTest(InputName::Up, InputName::UpBoost, InputName::UpAvo);
	IsAvoBoostTest(InputName::Down, InputName::DownBoost, InputName::DownAvo);


	moveObject->Update();
}

void WalkMoveObject::SetWalkData(WalkData* _data)
{
	if (ChPtr::NullCheck(_data))return;

	SetMoveData(_data);
	data = _data;
}

void WalkMoveObject::UpdateBegin()
{
	SetSelfViewRotateHorizontalFlg(false);

	if (IsPushFlg(InputName::CameraRightRotation))
	{
		SetPushFlg(InputName::RightRotation);
		RemovePushFlg(InputName::CameraRightRotation);
	}

	if (IsPushFlg(InputName::CameraLeftRotation))
	{
		SetPushFlg(InputName::LeftRotation);
		RemovePushFlg(InputName::CameraLeftRotation);
	}

}

void WalkMoveObject::Update()
{
	if (data == nullptr)return;

	ChLMat tmp;

	//tmp.SetRotationXAxis(ChMath::ToRadian(GetRotation().x));
	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));

	MoveUpdate(data->GetMovePower(), InputName::Front, ChVec3(0.0f, 0.0f, 1.0f), tmp);
	MoveUpdate(data->GetMovePower(), InputName::Back, ChVec3(0.0f, 0.0f, -1.0f), tmp);
	MoveUpdate(data->GetMovePower(), InputName::Left, ChVec3(-1.0f, 0.0f, 0.0f), tmp);
	MoveUpdate(data->GetMovePower(), InputName::Right, ChVec3(1.0f, 0.0f, 0.0f), tmp);
	MoveUpdate(data->GetJumpPower(), InputName::Up, ChVec3(0.0f, 1.0f, 0.0f), tmp);

	RotateUpdate(data->GetRotatePower() * ADD_ROTATE_MAGNIFICATION, InputName::RightRotation, ChVec3(0.0f, -1.0f, 0.0f));
	RotateUpdate(data->GetRotatePower() * ADD_ROTATE_MAGNIFICATION, InputName::LeftRotation, ChVec3(0.0f, 1.0f, 0.0f));

}

void WalkMoveObject::MoveUpdate(float _pow, InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{
	if (!IsPushFlg(_input))return;
	if (!IsGround())return;

	AddMoveVector(_nowTargetPoster.TransformCoord(_direction) * _pow);

}

void WalkMoveObject::RotateUpdate(float _pow, InputName _input, const ChVec3& _direction)
{
	if (!IsPushFlg(_input))return;

	auto vector = _direction;

	vector *= _pow;

	AddRotateVector(vector);
}

void ShipMoveObject::Update()
{
	SetSelfViewRotateHorizontalFlg(true);

	if (!IsGround())return;

	ChLMat tmp;

	//tmp.SetRotationXAxis(ChMath::ToRadian(GetRotation().x));
	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));
}

void CaterpillarMoveObject::SetCaterpillarData(CaterpillarData* _data)
{
	if (ChPtr::NullCheck(_data))return;
	SetMoveData(_data);
	data = _data;
}

void CaterpillarMoveObject::Update()
{	

	SetSelfViewRotateHorizontalFlg(true);

	if (!IsGround())return;

	FlagTest();

	ChLMat tmp;

	//tmp.SetRotationXAxis(ChMath::ToRadian(GetRotation().x));
	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));


	MoveUpdate(data->GetMovePower(), InputName::FrontRight, InputName::FrontLeft, ChVec3(0.0f, 0.0f, 1.0f), tmp);
	MoveUpdate(data->GetMovePower(), InputName::BackRight, InputName::BackLeft, ChVec3(0.0f, 0.0f, -1.0f), tmp);

	OneSideMoveUpdate(data->GetMovePower() * 0.5f, InputName::FrontRight, InputName::FrontLeft, InputName::BackLeft, ChVec3(sideSize, 0.0f, 0.0f), ChVec3(0.0f, -1.0f, 0.0f), tmp);
	OneSideMoveUpdate(data->GetMovePower() * 0.5f, InputName::FrontLeft, InputName::FrontRight, InputName::BackRight, ChVec3(-sideSize, 0.0f, 0.0f), ChVec3(0.0f, 1.0f, 0.0f), tmp);

	OneSideMoveUpdate(data->GetMovePower() * 0.5f, InputName::BackRight, InputName::FrontLeft, InputName::BackLeft, ChVec3(-sideSize, 0.0f, 0.0f), ChVec3(0.0f, -1.0f, 0.0f), tmp);
	OneSideMoveUpdate(data->GetMovePower() * 0.5f, InputName::BackLeft, InputName::FrontRight, InputName::BackRight, ChVec3(sideSize, 0.0f, 0.0f), ChVec3(0.0f, 1.0f, 0.0f), tmp);

	RotationUpdate(data->GetMovePower(), InputName::FrontLeft, InputName::BackRight, ChVec3(0.0f, -1.0f, 0.0f));
	RotationUpdate(data->GetMovePower(), InputName::FrontRight,InputName::BackLeft, ChVec3(0.0f, 1.0f, 0.0f));

	if (IsPushFlg(InputName::Up))
		AddMoveVector(tmp.TransformCoord(ChVec3(0.0f, 1.0f, 0.0f)) * data->GetJumpPower());
}

void CaterpillarMoveObject::FlagTest()
{
	bool isFront = IsPushFlg(InputName::Front);
	bool isBack = IsPushFlg(InputName::Back);
	if (isFront && isBack)return;

	FrontBackTest(InputName::Front, InputName::FrontRight, InputName::FrontLeft);
	FrontBackTest(InputName::Back, InputName::BackRight, InputName::BackLeft);

	RightLeftTest(InputName::Right,InputName::FrontLeft,InputName::BackRight);
	RightLeftTest(InputName::Left,InputName::FrontRight,InputName::BackLeft);

	RemoveTest(InputName::FrontRight, InputName::BackRight);
	RemoveTest(InputName::FrontLeft, InputName::BackLeft);

}

void CaterpillarMoveObject::FrontBackTest(InputName _frontBack, InputName _right, InputName _left)
{
	if (!IsPushFlg(_frontBack))return;

	bool isRightLeftFlg = false;

	if (IsPushFlg(InputName::Right))
	{
		SetPushFlg(_right);
		isRightLeftFlg = true;
	}

	if (IsPushFlg(InputName::Left))
	{
		SetPushFlg(_left);
		isRightLeftFlg = true;
	}

	if (isRightLeftFlg)return;

	SetPushFlg(_right);
	SetPushFlg(_left);

}

void CaterpillarMoveObject::RightLeftTest(InputName _rightLeft, InputName _front, InputName _back)
{
	if (!IsPushFlg(_rightLeft))return;
	if (IsPushFlg(InputName::Front) || IsPushFlg(InputName::Back))return;

	SetPushFlg(_front);
	SetPushFlg(_back);
}

void CaterpillarMoveObject::RemoveTest(InputName _front, InputName _back)
{
	if (!IsPushFlg(_front) || !IsPushFlg(_back))return;

	RemovePushFlg(_front);
	RemovePushFlg(_back);
}

void CaterpillarMoveObject::MoveUpdate(float _movePower, InputName _right, InputName _left, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{
	if (!IsPushFlg(_right) || !IsPushFlg(_left))return;

	if (!IsGround())return;

	AddMoveVector(_nowTargetPoster.TransformCoord(_direction) * _movePower);
}

void CaterpillarMoveObject::OneSideMoveUpdate(
	float _rotatePower,
	InputName _move,
	InputName _oneSideFront,
	InputName _oneSideBack,
	const ChVec3& _moveDirection,
	const ChVec3& _rotateDirection,
	const ChLMat& _nowTargetPoster)
{
	if (!IsPushFlg(_move))return;

	if (IsPushFlg(_oneSideFront) || IsPushFlg(_oneSideBack))return;

	auto vector = _rotateDirection;

	vector *= _rotatePower;

	AddRotateVector(vector);

	ChLMat tmpMat;
	tmpMat.SetRotationYPR(vector);

	tmpMat = tmpMat * _nowTargetPoster;

	AddMoveVector(tmpMat.TransformCoord(_moveDirection));
}

void CaterpillarMoveObject::RotationUpdate(float _rotatePower, InputName _right, InputName _left, const ChVec3& _direction)
{
	if (!IsPushFlg(_right) || !IsPushFlg(_left))return;

	auto vector = _direction;

	vector *= _rotatePower;

	AddRotateVector(vector);
}

void FighterMoveObject::MoveFront(float _pow, InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
{
	float movePow = _pow;

	if (_input == InputName::Back)
	{
		float movePower = GetMoveVector().GetLen();

		if (movePower > movePow)movePower = movePow;
	}

	AddMoveVector(_nowTargetPoster.TransformCoord(_direction) * movePow);
}

void FighterMoveObject::MoveSide(float _pow, InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster)
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

void FighterMoveObject::RotateUpDown(float _pow, InputName _input, const ChVec3& _direction)
{
	if (_input == InputName::RightRotation || _input == InputName::LeftRotation)return;

	ChVec3 vector = _direction.y;

	vector *= _pow;

	AddRotateVector(vector);
}

void FighterMoveObject::RotateZRightZLeft(float _pow, InputName _input, const ChVec3& _direction)
{

	if (_input == InputName::UpRotation || _input == InputName::DownRotation)return;

	ChVec3 vector;

	//yŽ²‰ñ“]‚©‚çzŽ²‰ñ“]‚Ö//
	vector.z = _direction.y;

	vector *= _pow;

	AddRotateVector(vector);
}