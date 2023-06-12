#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../FunctionComponent/WeaponComponents.h"

#include"../../../Frames/GameFrame.h"
#include"../../../Attack/Attack.h"

#include"../../CPU/CPULooker.h"

#include"CPUWeaponSelector.h"
#include"CPUTargetSelector.h"
#include"CPUAttack.h"

std::string CPUWeaponSelect::Serialize()
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

void CPUWeaponSelect::Deserialize(const std::string& _text)
{
	if (_text.empty())return;
#if 0
	auto&& textList = ChStr::Split(_text, cutChar);

	activeFlg = textList[ChStd::EnumCast(SerializeNo::ActiveFlg)] == "1";
	centerLength = ChStr::GetFloatingFromText<float>(textList[ChStd::EnumCast(SerializeNo::CenterLength)]);

	memberType = (CPUObjectLooker::MemberType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::MemberType)]);

	distanceType = (CPUObjectLooker::DistanceType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DistanceType)]);
	testDistance = ChStr::GetFloatingFromText<float>(textList[ChStd::EnumCast(SerializeNo::TestDistance)]);
	distanceComparison = (ComparisonOperation)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DistanceComparison)]);

	damageType = (CPUObjectLooker::DamageSizeType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DamageType)]);
	testDamage = ChStr::GetFloatingFromText<float>(textList[ChStd::EnumCast(SerializeNo::TestDamage)]);
	damageComparison = (ComparisonOperation)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::DamageComparison)]);

	attackType = (AttackType)ChStr::GetIntegialFromText<int>(textList[ChStd::EnumCast(SerializeNo::AttackType)]);

	unStopFlg = textList[ChStd::EnumCast(SerializeNo::UnStop)] == "1";
#endif
}

bool CPUWeaponSelect::Update(unsigned long _lookTarget, GameFrame& _frame, CPUAttack& _attack)
{
	if(!IsRunTest(_lookTarget, _frame))return false;

	if (!_attack.IsHaveAttackType(attackType))return false;

	return true;
}


std::string CPUWeaponSelect::GetValue(unsigned char _no)
{
	return "";
#if 0
	switch ((SerializeNo)_no)
	{
	case SerializeNo::ActiveFlg:
		return std::to_string(activeFlg ? 1 : 0) + cutChar;
	case SerializeNo::CenterLength:
		return std::to_string(centerLength) + cutChar;
	case SerializeNo::MemberType:
		return  std::to_string(ChStd::EnumCast(memberType)) + cutChar;
	case SerializeNo::DistanceType:
		return std::to_string(ChStd::EnumCast(distanceType)) + cutChar;
	case SerializeNo::TestDistance:
		return std::to_string(testDistance) + cutChar;
	case SerializeNo::DistanceComparison:
		return std::to_string(ChStd::EnumCast(distanceComparison)) + cutChar;
	case SerializeNo::DamageType:
		return std::to_string(ChStd::EnumCast(damageType)) + cutChar;
	case SerializeNo::TestDamage:
		return std::to_string(testDamage) + cutChar;
	case SerializeNo::DamageComparison:
		return std::to_string(ChStd::EnumCast(damageComparison)) + cutChar;
	case SerializeNo::AttackType:
		return std::to_string(ChStd::EnumCast(attackType)) + cutChar;
	case SerializeNo::UnStop:
		return std::to_string(unStopFlg ? 1 : 0) + cutChar;
	default:
		break;
	}
#endif
}

std::string CPUWeaponSelector::Serialize()
{
	return "";
}

void CPUWeaponSelector::Deserialize(const std::string& _text)
{

}

void CPUWeaponSelector::Update(CPUTargetSelector& _targetSelector, GameFrame& _frame, CPUAttack& _attack)
{
	if (!_targetSelector.IsLookTarget())
	{
		beforeTarget = -1;
		return;
	}

	if (beforeTarget != _targetSelector.GetTargetNo())
	{
		beforeTarget = _targetSelector.GetTargetNo();
		ActionCansel();
		_attack.AttackCancel();
	}

	if (!selectWeaponsQueue.empty())return;

	OutputDebugString("CPUWeaponSelector Update Start\n");

	for (auto&& func : functions)
	{
		if (!func->Update(beforeTarget, _frame, _attack))continue;
		selectWeaponsQueue.push_back(func);
	}

	OutputDebugString("CPUWeaponSelector Update End\n");
}
