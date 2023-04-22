#pragma once

class CameraObject {

public:

	inline void SetCameraPositionObject(ChPtr::Shared<ChCpp::FrameObject>& _cameraPos)
	{
		cameraPos = _cameraPos;
	}

private:

	ChPtr::Shared<ChCpp::FrameObject>cameraPos = nullptr;

};