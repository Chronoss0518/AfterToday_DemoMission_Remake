#pragma once

#include"FunctionComponent.h"

class CameraData;

class CameraComponent : public FunctionComponent
{
public:

	static constexpr float CAMERA_Y_POS = 4.0f;

public:

	void Move()override;

public:

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();

public:

	void SetGameFrame(GameFrame* _frame) { frame = _frame; }

	void AddCameraData(CameraData* _data);

	void SubCameraData(CameraData* _data);

private:

	void CamVerticalRotateUpdate(InputName _input, const float _camRot);

	void CamHorizontalRotateUpdate(InputName _input, const float _camRot);

private:

	void UpdateTargetLooker();

	void SetTarget();

	void SetRotateToTarget();

public:

	inline void SetCenterPos(const ChVec3& _center) { centerPos = _center; }

public:

	inline void AddViewVertical(const float& _x) { viewVertical = std::abs(viewVertical + _x) < maxViewVertical ? viewVertical + _x : viewVertical; }

	inline void AddViewHorizontal(const float& _y) { viewHorizontal += _y; }

	inline void SetViewVertical(const float& _x) { viewVertical = std::abs(_x) < maxViewVertical ? _x : maxViewVertical; }

	inline void SetViewHorizontal(const float& _y) { viewHorizontal = _y; }

public:

	inline ChLMat GetViewMat() { return viewMat; }

	inline ChLMat GetProMat() { return proMat; }

	inline ChVec3 GetViewLookDir() { return lookDir; }

	inline float GetViewVertical() { return viewVertical; }

	inline float GetViewHorizontal() { return viewHorizontal; }

public:

	bool IsLookTarget(BaseMecha* _mecha);

private:

	ChLMat CreateViewMatrix();

private:

	ChVec3 GetViewPos(const ChLMat& _mat);

	ChVec3 GetViewLookPos(const ChLMat& _mat);

	ChVec3 GetViewCrossDir(const ChLMat& _mat);

private:

	size_t useCameraNo = 0;
	std::vector<CameraData*>cameraList;

	//ÉJÉÅÉâÇÃéãñÏäp//
	float nowFovy = 30.0f;

	ChVec3 centerPos = ChVec3();

	float viewVertical = 0.0f;
	float maxViewVertical = 85.0f;

	float viewHorizontal = 0.0f;

	float cameraRotatePow = 50.0f;

	ChLMat viewMat = ChLMat();
	ChLMat proMat = ChLMat();

	ChVec3 lookDir = ChVec3(0.0f, 0.0f, 1.0f);

	GameFrame* frame = nullptr;
	ChPtr::Weak<BaseMecha>lookTarget;
	bool isTargetLooker = false;

	bool updateKeyFlg = false;
};