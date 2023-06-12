#pragma once

#include"CPUActionBase.h"

class CPUTargetSelect :public CPUActionBase
{
public:

	std::string Serialize()override;

	void Deserialize(const std::string& _text)override;

	unsigned long Update(CPUObjectLooker& _looker, GameFrame& _frame);

private:

	std::string GetValue(unsigned char _no);

};

class CPUTargetSelector : public FunctionsBase<CPUTargetSelect>
{
public:

	std::string Serialize()override;

	void Deserialize(const std::string& _text)override;

	void Update(CPUObjectLooker& _looker,GameFrame& _frame);

	inline bool IsLookTarget() { return lookTargetFlg; }

	inline unsigned long GetTargetNo() { return targetNo; }

private:

	bool lookTargetFlg = false;

	unsigned long targetNo = 0;

};