#pragma once

#include"FunctionComponent.h"

class WalkData;
class CaterpillarData;

class MoveDataBase;

class MoveComponent :public FunctionComponent
{
public:

	class MoveObject
	{
	protected:

		using InputName = BaseMecha::InputName;

	public:

		inline void SetMoveComponent(MoveComponent* _com)
		{
			if (ChPtr::NullCheck(_com))return;
			if (ChPtr::NotNullCheck(component))return;
			component = _com;
		}

	protected:

		inline void SetMoveData(MoveDataBase* _data)
		{
			if (_data == nullptr)return;
			data = _data;
		}


	public:

		inline bool IsPushFlg(InputName _name) { return component->IsPushFlg(_name); }

		inline bool IsGround() { return component->IsGround(); }

		inline bool IsMoveData(MoveDataBase* _data)
		{
			return _data == data;
		}

	public:

		virtual void UpdateBegin(){}

		virtual void Update() = 0;

	public:

		inline void AddMoveVector(const ChVec3& _moveVecAdd) { component->AddMoveVector(_moveVecAdd); }

		inline void AddRotateVector(const ChVec3& _rotateVecAdd) { component->AddRotateVector(_rotateVecAdd); }

		inline void AddViewRotateVertical(const float& _viewRotate) { component->AddViewRotateVertical(_viewRotate); }

		inline void SetSelfViewRotateHorizontalFlg(const bool& _flg) { component->SetSelfViewRotateHorizontalFlg(_flg); }

		inline void AddViewRotateHorizontal(const float& _viewRotate) { component->AddViewRotateHorizontal(_viewRotate); }

	public:

		inline void SetPushFlg(InputName _name) { component->SetPushFlg(_name); }

		inline void RemovePushFlg(InputName _name) { component->RemovePushFlg(_name); }

	public:

		ChVec3 GetPosition() { return component->GetPosition(); }

		ChVec3 GetRotation() { return component->GetRotation(); }

		ChVec3 GetMoveVector() { return component->GetMoveVector(); }

		ChVec3 GetRotateVector() { return component->GetRotateVector(); }

	private:

		MoveComponent* component = nullptr;

		MoveDataBase* data = nullptr;
	};


public:

	void Update()override;

public:

	inline void AddMoveObject(ChPtr::Shared<MoveObject> _moveObject)
	{
		if (_moveObject == nullptr)return;
		_moveObject->SetMoveComponent(this);
		moveObjectList.push_back(_moveObject);
	}

public:

	void RemoveMoveObject(MoveDataBase* _moveObject);

public:

	void UpChangeMoveObject() 
	{
		nowMoveObject = (nowMoveObject + 1) % moveObjectList.size();
	}

	void DownChangeMoveObject()
	{
		nowMoveObject = nowMoveObject - 1;
		if (nowMoveObject < 0)nowMoveObject = moveObjectList.size() - 1;
	}

protected:

	void CamVerticalRotateUpdate(InputName _input, const float _camRot);

	void CamHorizontalRotateUpdate(InputName _input, const float _camRot);

protected:

	void IsAvoBoostTest(InputName _input, InputName _boost, InputName _avoid);

private:

	char nowMoveObject = 0;
	std::vector<ChPtr::Shared<MoveObject>>moveObjectList;

	float cameraRotatePow = 50.0f;
};


//êlå^ìãèÊï∫äÌ//
class WalkMoveObject :public MoveComponent::MoveObject
{
public:

	void UpdateBegin()override;

	void Update()override;

public:

	void SetWalkData(WalkData* _data);

private:

	void MoveUpdate(float _pow, BaseMecha::InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster);

	void RotateUpdate(float _pow, BaseMecha::InputName _input, const ChVec3& _direction);

	WalkData* data = nullptr;

};

//ëD//
class ShipMoveObject :public MoveComponent::MoveObject
{
public:

	void Update()override;

private:

	float movePow = 0.0f;
};

//êÌé‘//
class CaterpillarMoveObject :public MoveComponent::MoveObject
{
public:

	void Update()override;

public:

	void SetCaterpillarData(CaterpillarData* _data);

	inline void SetSideSize(float _size) { sideSize = _size; }

private:

	void FlagTest();

	void FrontBackTest(InputName _frontBack, InputName _right, InputName _left);

	void RightLeftTest(InputName _rightLeft, InputName _front, InputName _back);

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

private:

	CaterpillarData* data = nullptr;

	float sideSize = 0.0f;

};

//êÌì¨ã@//
class FighterMoveObject :public MoveComponent::MoveObject
{
public:

	void Update()override;

private:

	void MoveFront(float _pow, InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster);

	void MoveSide(float _pow, InputName _input, const ChVec3& _direction, const ChLMat& _nowTargetPoster);

	void RotateUpDown(float _pow, InputName _input, const ChVec3& _direction);

	void RotateZRightZLeft(float _pow, InputName _input, const ChVec3& _direction);

private:

	float accelePow = 0.0f;
	
	//êÇíºï˚å¸ÇÃâÒì]óÕ//
	float verticalRotatePower = 0.0f;

	//êÖïΩï˚å¸ÇÃâÒì]óÕ//
	float horyzontalRotatePower = 0.0f;
};
