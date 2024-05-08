#pragma once

class CameraObject {

public:

	inline void SetCameraPositionObject(ChPtr::Shared<MechaPartsObject>& _cameraPos)
	{
		cameraPos = _cameraPos;
	}

private:

	ChPtr::Shared<MechaPartsObject>cameraPos = nullptr;


};