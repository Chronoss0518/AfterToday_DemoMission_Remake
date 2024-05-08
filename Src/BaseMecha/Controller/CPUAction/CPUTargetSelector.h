#pragma once

#include"CPUActionBase.h"

class CPUTargetSelect :public CPUActionBase
{
public:

	ChPtr::Shared<ChCpp::JsonObject> Serialize()override;

	void Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)override;

	unsigned long Update(CPUObjectLooker& _looker, GameFrame& _frame, CPUController& _controller);

};

class CPUTargetSelector : public FunctionsBase<CPUTargetSelect>
{
public:

	ChPtr::Shared<ChCpp::JsonObject> Serialize()override;

	void Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)override;

	void Update(CPUObjectLooker& _looker, GameFrame& _frame, CPUController& _controller);

	inline bool IsLookTarget() { return lookTargetFlg; }

	inline unsigned long GetTargetNo() { return targetNo; }

private:

	bool lookTargetFlg = false;

	unsigned long targetNo = 0;

};