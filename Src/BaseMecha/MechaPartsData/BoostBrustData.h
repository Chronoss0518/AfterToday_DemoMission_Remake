#pragma once

#include"NextPosBase.h"

class BoostBrust :public PartsDataBase
{

public:

	void RemoveParameter(BaseMecha& _base)override;

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetBoostData(PartsParameterStruct::BoostData& _boost);

	inline void SetParamName(const std::wstring& _objectName) { objectName = _objectName; }

	void SetUseEnergy(const unsigned long _useEnergy) { useEnergy = _useEnergy; }

	void SetBoostPower(const float _boostPower) { boostPower = _boostPower; }

	void SetAvoidUseEnergy(const unsigned long _avoidUseEnergy) { avoidUseEnergy = _avoidUseEnergy; }

	void SetAvoidPower(const float _avoidPow) { avoidPow = _avoidPow; }

	void SetAvoidWait(const unsigned long _avoidWait) { avoidWait = _avoidWait; }

public://Get Functions//

	inline std::wstring GetObjectNameList() { return objectName; }

	unsigned long GetBoostUseEnergy()const { return useEnergy; }

	float GetBoostPower()const { return boostPower; }

	unsigned long GetAvoidUseEnergy()const { return avoidUseEnergy; }

	float GetAvoidPower()const { return avoidPow; }

	unsigned long GetAvoidWait()const { return avoidWait; }

	ChPtr::Shared<ChCpp::FrameObject<wchar_t>> GetFrame(BaseMecha& _base);

	virtual BaseMecha::InputName GetBoostInputName() = 0;

	virtual BaseMecha::InputName GetAvoidInputName() = 0;

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

class RightBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::RightBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::RightAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(RightBoostBrust); }

};

class LeftBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::LeftBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::LeftAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(LeftBoostBrust); }


};

class FrontBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::FrontBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::FrontAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(FrontBoostBrust); }

};

class BackBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::BackBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::BackAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(BackBoostBrust); }

};

class UpBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::UpBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::UpAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(UpBoostBrust); }

};

class DownBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::DownBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::DownAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(DownBoostBrust); }

};
