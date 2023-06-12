#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../FunctionComponent/WeaponComponents.h"

#include"../../../Frames/GameFrame.h"

#include"../../CPU/CPULooker.h"

#include"CPUTargetSelector.h"

std::string CPUTargetSelect::Serialize()
{
	std::string res = "";
#if 0
	for (unsigned char i = 0; i < ChStd::EnumCast(SerializeNo::None); i++)
	{
		res += GetValue(i) + cutChar;
	}

	res.pop_back();
#endif
	return res;
}

void CPUTargetSelect::Deserialize(const std::string& _text)
{
	if (_text.empty())return;
#if 0
	auto&& textList = ChStr::Split(_text, cutChar);

	activeFlg = textList[ChStd::EnumCast(SerializeNo::ActiveFlg)] == "1";

	memberType = (CPUObjectLooker::MemberType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::MemberType)]);

	distanceType = (CPUObjectLooker::DistanceType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DistanceType)]);
	testDistance = ChStr::GetFloatingFromText<float>(textList[ChStd::EnumCast(SerializeNo::TestDistance)]);
	distanceComparison = (ComparisonOperation)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DistanceComparison)]);

	damageType = (CPUObjectLooker::DamageSizeType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DamageType)]);
	testDamage = ChStr::GetFloatingFromText<float>(textList[ChStd::EnumCast(SerializeNo::TestDamage)]);
	damageComparison = (ComparisonOperation)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DamageComparison)]);
#endif
}

unsigned long CPUTargetSelect::Update(CPUObjectLooker& _lookTarget, GameFrame& _frame)
{
	unsigned long lookTargetNo = GetLookTypeMechas(_lookTarget);

	if (!IsRunTest(lookTargetNo, _frame))return -1;

	return lookTargetNo;
}

std::string CPUTargetSelect::GetValue(unsigned char _no)
{

}

std::string CPUTargetSelector::Serialize()
{
	return "";
}

void CPUTargetSelector::Deserialize(const std::string& _text)
{

}

void CPUTargetSelector::Update(CPUObjectLooker& _looker, GameFrame& _frame)
{
	if (functions.empty())return;

	//OutputDebugString("CPUTargetSelector Update Start\n");

	lookTargetFlg = false;
	targetNo = -1;
	for (auto&& selectFunction : functions)
	{
		unsigned long test = selectFunction->Update(_looker, _frame);
		if (test < 0)continue;
		targetNo = test;
		lookTargetFlg = true;
		break;
	}

	//OutputDebugString("CPUTargetSelector Update End\n");

}
