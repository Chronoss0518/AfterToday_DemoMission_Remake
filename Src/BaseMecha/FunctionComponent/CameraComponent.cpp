
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../../Attack/Attack.h"
#include"../MechaPartsObject.h"

#include"CameraComponent.h"
#include"../BaseMecha.h"

#define DEBUG_UP_CAMERA false

#if DEBUG_UP_CAMERA
#define DEBUG_UP_POS 50.0f
#endif

#define VIEW_ROTATE_POW_UPPER 16 / 9

void CameraComponent::Update()
{

	CamVerticalRotateUpdate(InputName::CameraUpRotation, cameraRotatePow);
	CamVerticalRotateUpdate(InputName::CameraDownRotation, -cameraRotatePow);

	CamHorizontalRotateUpdate(InputName::CameraRightRotation, -cameraRotatePow * VIEW_ROTATE_POW_UPPER);
	CamHorizontalRotateUpdate(InputName::CameraLeftRotation, cameraRotatePow * VIEW_ROTATE_POW_UPPER);

}

void CameraComponent::CamVerticalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewVertical(_camRot / PhysicsMachine::GetFPS());
}

void CameraComponent::CamHorizontalRotateUpdate(InputName _input, const float _camRot)
{
	if (!IsPushFlg(_input))return;

	AddViewHorizontal(_camRot / PhysicsMachine::GetFPS());
}

ChVec3 CameraComponent::GetViewPos()
{
#if DEBUG_UP_CAMERA
	return centerPos + ChVec3(0.0f, DEBUG_UP_POS, 0.0f);

#endif


	ChLMat camYMat, camXMat;

	camYMat.SetRotationYAxis(ChMath::ToRadian(viewHorizontal));
	camXMat.SetRotationXAxis(-ChMath::ToRadian(viewVertical));
	camYMat = camXMat * camYMat;
	camYMat.SetPosition(centerPos + ChVec3(0.0f, CAMERA_Y_POS, 0.0f));

	return camYMat.Transform(ChVec3(0.0f, 0.0f, -15.0f));
}

ChVec3 CameraComponent::GetViewLookPos()
{

#if DEBUG_UP_CAMERA

	return centerPos;

#endif
	ChLMat camYMat, camXMat;

	camYMat.SetRotationYAxis(ChMath::ToRadian(viewHorizontal));
	camXMat.SetRotationXAxis(-ChMath::ToRadian(viewVertical));
	camYMat = camXMat * camYMat;
	camYMat.SetPosition(centerPos + ChVec3(0.0f, CAMERA_Y_POS - 2.0f, 0.0f));

	return camYMat.Transform(ChVec3(0.0f, 0.0f, 5.0f));
}

void CameraComponent::UpdateCamera()
{
	auto viewPos = GetViewPos();
	auto viewLookPos = GetViewLookPos();

	ChMat_11 tmpMat;

#if DEBUG_UP_CAMERA
	tmpMat.CreateViewMatLookTarget(viewPos, viewLookPos, ChVec3(0.0f, 0.0f, 1.0f));
#else
	tmpMat.CreateViewMatLookTarget(viewPos, viewLookPos, ChVec3(0.0f, 1.0f, 0.0f));
#endif

	viewMat = tmpMat;
}