#pragma once

#include "../MechaParts.h"

class CameraData : public PartsDataBase
{

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	virtual std::wstring Serialize()override;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

	void SetFovy(const float _fovy) { fovy = _fovy; }

	void SetCameraObjectName(const std::wstring& _cameraObject) { cameraObject = _cameraObject; }

	void SetCameraCount(const unsigned long _cameraCount) { cameraCount = _cameraCount; }

public://Get Functions//

	virtual std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(CameraData); }

	float GetFovy()const { return fovy; }

protected:

	//カメラの視野角//
	float fovy = 60.0f;

	//3DModelのカメラの位置
	std::wstring cameraObject = L"";

	//機体内に表示されるカメラの数
	unsigned long cameraCount = 1;

};
