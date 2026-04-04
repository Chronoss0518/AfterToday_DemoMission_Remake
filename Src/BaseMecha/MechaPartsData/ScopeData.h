#pragma once

#include"CameraData.h"

class ScopeData : public CameraData
{

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base, MechaPartsObject& _parts)override {};

	void SetMinFovy(const float _minFovy) { minFovy = _minFovy; }

	void SetMaxFovy(const float _maxFovy) { maxFovy = _maxFovy; }

	void SetFovySlideSpeed(const float _fovySlideSpeed) { fovySlideSpeed = _fovySlideSpeed; }

public://Get Functions//

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(ScopeData); }

	float GetMaxFovy()const { return minFovy; }

	float GetMinFovy()const { return maxFovy; }

	float GetFovySlideSpeed()const { return fovySlideSpeed; }

protected:

	//Å¬‚Ì‹–ìŠp//
	float minFovy = 10.0f;

	//Å‘å‚Ì‹–ìŠp//
	float maxFovy = 90.0f;

	//‹–ìŠp‚ğ•Ï“®‚³‚¹‚éƒXƒs[ƒh//
	float fovySlideSpeed = 1.0f;

};
