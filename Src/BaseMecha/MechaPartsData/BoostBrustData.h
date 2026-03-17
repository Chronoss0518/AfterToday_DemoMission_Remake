#pragma once

#include"NextPosBase.h"

#include"../BoostDirection.h"

class BoostBrust :public PartsDataBase
{
public:

	void RemoveParameter(BaseMecha& _base)override;

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	void SetBoostData(BoostDirection _direction, PartsParameterStruct::BoostData& _boost);

	void SetParamName(const std::wstring& _objectName) { objectName = _objectName; }

	void SetUseEnergy(const unsigned long _useEnergy) { useEnergy = _useEnergy; }

	void SetBoostPower(const float _boostPower) { boostPower = _boostPower; }

	void SetAvoidUseEnergy(const unsigned long _avoidUseEnergy) { avoidUseEnergy = _avoidUseEnergy; }

	void SetAvoidPower(const float _avoidPow) { avoidPow = _avoidPow; }

	void SetAvoidWait(const unsigned long _avoidWait) { avoidWait = _avoidWait; }

public:

	void AddBoostDirection(BoostDirection _direction) { directionFlgs.SetBitTrue(ChStd::EnumCast(_direction)); }

public://Get Functions//

	inline std::wstring GetObjectNameList() { return objectName; }

	inline unsigned long GetBoostUseEnergy()const { return useEnergy; }

	inline float GetBoostPower()const { return boostPower; }

	inline unsigned long GetAvoidUseEnergy()const { return avoidUseEnergy; }

	inline float GetAvoidPower()const { return avoidPow; }

	inline unsigned long GetAvoidWait()const { return avoidWait; }

	inline bool GetBoostDirectionFlg(BoostDirection _direction) { return directionFlgs.GetBitFlg(ChStd::EnumCast(_direction)); }

	ChPtr::Shared<ChCpp::FrameObject<wchar_t>> GetFrame(BaseMecha& _base);

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(BoostBrust); }

protected:

	std::wstring objectName = L"";

	ChCpp::BitBool directionFlgs = ChCpp::BitBool(1);

	//Boost使用時のエネルギー消費量//
	unsigned long useEnergy = 0;
	//Boost使用時における移動力//
	float boostPower = 0.0f;
	//回避時のエネルギー消費量//
	unsigned long avoidUseEnergy = 0;
	//回避する際の移動力//
	float avoidPow = 0.0f;
	//回避する際の再使用時間//
	unsigned long avoidWait = 0;

};
