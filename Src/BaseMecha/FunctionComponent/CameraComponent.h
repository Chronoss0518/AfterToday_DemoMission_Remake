#pragma once

#include"FunctionComponent.h"

class CmaeraData;

class CameraComponent : public FunctionComponent
{
public:

	static constexpr float CAMERA_Y_POS = 4.0f;

public:

	void Update()override;

public:

	void UpdateCamera();

private:

	void CamVerticalRotateUpdate(InputName _input, const float _camRot);

	void CamHorizontalRotateUpdate(InputName _input, const float _camRot);

public:

	inline void SetViewVerticial(bool _updateFlg, float _rotation) { if (_updateFlg)viewHorizontal = _rotation; }

	inline void SetCenterPos(const ChVec3& _center) { centerPos = _center; }

public:

	inline void AddViewVertical(const float& _x) { viewVertical = std::abs(viewVertical + _x) < maxViewVertical ? viewVertical + _x : viewVertical; }

	inline void AddViewHorizontal(const float& _y) { viewHorizontal += _y; }

public:

	inline ChLMat GetViewMat() { return viewMat; }

public:

	ChVec3 GetViewPos();

	ChVec3 GetViewLookPos();

private:

	size_t useCameraNo = 0;
	std::vector<CmaeraData*>cameraList;

	ChVec3 centerPos = ChVec3();

	float viewVertical = 0.0f;
	float maxViewVertical = 85.0f;

	float viewHorizontal = 0.0f;

	float cameraRotatePow = 50.0f;

	ChLMat viewMat;

};