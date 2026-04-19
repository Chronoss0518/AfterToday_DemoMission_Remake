
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../../Frames/GameFrame.h"

#include"../../Attack/Attack.h"
#include"../MechaPartsObject.h"

#include"CameraComponent.h"
#include"../BaseMecha.h"
#include"../MechaPartsData/CameraData.h"
#include"../MechaPartsData/ScopeData.h"

#define DEBUG_UP_CAMERA false

#if DEBUG_UP_CAMERA
#define DEBUG_UP_POS 50.0f
#endif

#define VIEW_ROTATE_POW_UPPER 16 / 9

#define UN_TARGET_LOOK_LEN 100.0f

void CameraComponent::Move()
{

	updateKeyFlg = false;

	CamVerticalRotateUpdate(InputName::CameraUpRotation, -cameraRotatePow);
	CamVerticalRotateUpdate(InputName::CameraDownRotation, cameraRotatePow);

	CamHorizontalRotateUpdate(InputName::CameraRightRotation, -cameraRotatePow * VIEW_ROTATE_POW_UPPER);
	CamHorizontalRotateUpdate(InputName::CameraLeftRotation, cameraRotatePow * VIEW_ROTATE_POW_UPPER);

	UpdateTargetLooker();

	UpdateViewMatrix();

	UpdateProjectionMatrix();

}

void CameraComponent::AddCameraData(CameraData* _data)
{
	if (cameraList.empty())
		nowFovy = _data->GetFovy();

	cameraList.push_back(_data);
}

void CameraComponent::SubCameraData(CameraData* _data)
{

}

void CameraComponent::CamVerticalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewVertical(_camRot / PhysicsMachine::GetFPS());

	updateKeyFlg = true;
}

void CameraComponent::CamHorizontalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewHorizontal(_camRot / PhysicsMachine::GetFPS());

	updateKeyFlg = true;
}

void CameraComponent::UpdateTargetLooker()
{

	if (IsPushFlg(InputName::UseTargetLooker))
		isTargetLooker = !isTargetLooker;

	SetTarget();

	SetRotateToTarget();

}

void CameraComponent::SetTarget()
{
	if (!isTargetLooker)
	{
		if (!lookTarget.expired())
			lookTarget.reset();

		return;
	}

	if (ChPtr::NullCheck(frame))return;

	auto&& baseMecha = GetBaseMecha();

	auto&& mechas = frame->GetMechas();

	if (mechas.empty())return;

	lookTarget.reset();
	ChVec3 test = ChVec3(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 0.0f);

	for (size_t i = 0; i < mechas.size(); i++)
	{
		auto&& mecha = mechas[i].lock();
		if (mecha == nullptr)continue;
		if (mecha.get() == baseMecha)continue;
		if (mecha->GetTeamNo() == baseMecha->GetTeamNo())continue;
		if (mecha->IsBreak())continue;

		ChVec4 pos = mecha->GetPosition();
		pos = viewMat.Transform(pos);

		if (pos.z < 0)continue;

		pos = proMat.Transform(pos);
		pos.x /= pos.w != 0.0f ? pos.w : 1.0f;
		pos.y /= pos.w != 0.0f ? pos.w : 1.0f;
		pos.z /= pos.w != 0.0f ? pos.w : 1.0f;

		if (pos.z > 1.0f)continue;
		if (pos.x > GameFrame::GetCenterProjectionWidth())continue;
		if (pos.y > GameFrame::GetCenterProjectionHeight())continue;

		ChVec2 testSize = test;
		ChVec2 posSize = pos;

		float testLen = testSize.GetLen();
		float posLen = posSize.GetLen();

		if (testLen < posLen)continue;

		test = pos;
		test.z = 0.0f;
		lookTarget = mechas[i];
	}
}

void CameraComponent::SetRotateToTarget()
{
	if (lookTarget.expired())return;
	if (updateKeyFlg)return;

	auto&& targetObj = lookTarget.lock();

	auto&& baseMecha = GetBaseMecha();

	ChVec3 dir = targetObj->GetPosition() - (centerPos + ChVec3(0.0f, CAMERA_Y_POS, 0.0f));
	dir.Normalize();

	auto rotate = GetRotationFromDir(dir);

	SetViewHorizontal(ChMath::ToDegree(rotate.xzRad));

	SetViewVertical(-ChMath::ToDegree(rotate.yRad));

}

ChVec3 CameraComponent::GetLookPosition()
{
	auto mat = CreateViewRotateMatrix();

	if (!lookTarget.expired())
	{
		auto target = lookTarget.lock();
		return target->GetPosition();
	}

	ChVec3 lookPos = GetViewPos(mat);

	ChVec3 res = lookDir;
	res.SetLen(UN_TARGET_LOOK_LEN);
	return res + lookPos;
}

bool CameraComponent::IsLookTarget(BaseMecha* _mecha)
{
	if (lookTarget.expired())return false;
	auto mecha = lookTarget.lock();
	return mecha.get() == _mecha;
}

ChLMat CameraComponent::CreateViewRotateMatrix()
{
	ChLMat camYMat, camXMat;

	camYMat.SetRotationYAxis(ChMath::ToRadian(viewHorizontal));
	camXMat.SetRotationXAxis(ChMath::ToRadian(viewVertical));
	camYMat = camXMat * camYMat;
	camYMat.SetPosition(centerPos + ChVec3(0.0f, CAMERA_Y_POS, 0.0f));

	return camYMat;
}

ChVec3 CameraComponent::GetViewPos(const ChLMat& _mat)
{
#if DEBUG_UP_CAMERA
	return centerPos + ChVec3(0.0f, DEBUG_UP_POS, 0.0f);

#endif

	return _mat.Transform(ChVec3(0.0f, 0.0f, -15.0f));
}

ChVec3 CameraComponent::GetViewLookPos(const ChLMat& _mat)
{

#if DEBUG_UP_CAMERA

	return centerPos;

#endif

	return _mat.Transform(ChVec3(0.0f, 0.0f, 5.0f));
}

ChVec3 CameraComponent::GetViewCrossDir(const ChLMat& _mat)
{
#if DEBUG_UP_CAMERA

	return ChVec3(1.0f,0.0f,0.0f);

#endif
	ChVec3 tmp = ChVec3();
	tmp.val.Set(_mat.m[0]);

	return tmp;
}

void CameraComponent::UpdateViewMatrix()
{
	auto&& lMat = CreateViewRotateMatrix();

	auto&& viewPos = GetViewPos(lMat);
	auto&& viewLookPos = GetViewLookPos(lMat);
	auto&& viewCrossDir = GetViewCrossDir(lMat);

	lookDir = viewLookPos - viewPos;
	lookDir.Normalize();
	ChVec3 up;
	up.SetCross(lookDir, viewCrossDir);

	ChMat_11 tmpMat;

#if DEBUG_UP_CAMERA
	tmpMat.CreateViewMatLookTarget(viewPos, viewLookPos, up);
#else
	tmpMat.CreateViewMatLookTarget(viewPos, viewLookPos, up);
#endif

	viewMat = tmpMat;
}

void CameraComponent::UpdateProjectionMatrix()
{

	ChMat_11 tmpProMat;
	tmpProMat.CreateProjectionMat(ChMath::ToRadian(nowFovy), GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, GAME_PROJECTION_NEAR, GAME_PROJECTION_FAR);
	proMat = tmpProMat;
}
