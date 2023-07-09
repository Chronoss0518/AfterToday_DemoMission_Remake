#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../../Frames/GameFrame.h"

#include"../../CPU/CPULooker.h"

#include"CPUMovePositionSelector.h"
#include"CPUTargetSelector.h"
#include"CPUMoveInput.h"

std::string CPUMovePositionSelect::Serialize()
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

void CPUMovePositionSelect::Deserialize(const std::string& _text)
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

std::string CPUMovePositionSelect::GetValue(unsigned char _no)
{
	return "";
}

float CPUMovePositionSelect::GetPointLength(const ChVec3& _position)
{
	outPoint = point;

	if (axisFlg.GetBitFlg(ChStd::EnumCast(IgnoreAxisFlg::X)))outPoint.x = _position.x;
	if (axisFlg.GetBitFlg(ChStd::EnumCast(IgnoreAxisFlg::Y)))outPoint.y = _position.y;
	if (axisFlg.GetBitFlg(ChStd::EnumCast(IgnoreAxisFlg::Z)))outPoint.z = _position.z;


	return ChVec3::GetLen(outPoint,_position);
}

bool CPUMovePositionSelect::Update(const ChVec3& _position)
{
	return GetPointLength(_position) < distance;
}

void CPUMovePositionSelector::SetInitPosition(CPUController& _controller, float _distance, bool _xIgnore, bool _yIgnore, bool _zIgnore)
{
	auto point = ChPtr::Make_S<CPUMovePositionSelect>();

	auto&& mecha = _controller.GetBaseMecha();

	if (ChPtr::NullCheck(mecha))return;

	point->SetOperationPoint(mecha->GetPosition());
	point->SetOperationPointDistance(_distance);

	point->SetIgnoreFlg(_xIgnore, CPUMovePositionSelect::IgnoreAxisFlg::X);
	point->SetIgnoreFlg(_yIgnore, CPUMovePositionSelect::IgnoreAxisFlg::Y);
	point->SetIgnoreFlg(_zIgnore, CPUMovePositionSelect::IgnoreAxisFlg::Z);

	Add(point);
}

std::string CPUMovePositionSelector::Serialize()
{
	return "";
}

void CPUMovePositionSelector::Deserialize(const std::string& _text)
{

}

void CPUMovePositionSelector::Update(CPUTargetSelector& _selector, GameFrame& _frame, CPUController& _controller)
{
	//OutputDebugString("CPUMovePositionSelector Update Start\n");

	runUpdateLookTargetFlg = false;

	UpdateLookTarget(_selector, _frame);

	UpdateUnLookTarget(_controller);

	//OutputDebugString("CPUMovePositionSelector Update End\n");
}

void CPUMovePositionSelector::UpdateLookTarget(CPUTargetSelector& _selector, GameFrame& _frame)
{
	if (!_selector.IsLookTarget())return;

	auto&& mechaList = _frame.GetMechaList().GetObjectList<BaseMecha>();
	if (mechaList.size() <= _selector.GetTargetNo())return;
	targetMecha = mechaList[_selector.GetTargetNo()];
	auto&& mecha = targetMecha.lock();
	if (mecha == nullptr)return;

	point = mecha->GetPosition();

	float tmpLength = functions[moveOperationPointCount]->GetPointLength(point);

	for (unsigned long i = 0; i < functions.size(); i++)
	{
		if (i == moveOperationPointCount)continue;

		float testLength = functions[i]->GetPointLength(point);

		if (testLength > tmpLength)continue;

		tmpLength = testLength;
		moveOperationPointCount = i;
	}

	lastLookPoint = point;
	isBattleFlg = true;
	runUpdateLookTargetFlg = true;
}

void CPUMovePositionSelector::UpdateUnLookTarget(CPUController& _controller)
{
	if (runUpdateLookTargetFlg)return;
	
	auto&& mecha = _controller.GetBaseMecha();

	if (ChPtr::NullCheck(mecha))return;

	if (mecha->IsBreak())return;

	if (!isBattleFlg && mecha->GetDamageDir() != ChVec3())
	{
		isBattleFlg = true;
		lastLookPoint = mecha->GetDamageDir() * (isTargetPositionInAreaLength + 0.1f);
	}

	if (targetMecha.expired())isBattleFlg = false;

	if (isBattleFlg)
	{
		float testLookPosLength = (lastLookPoint - mecha->GetPosition()).Len();

		if (testLookPosLength < isTargetPositionInAreaLength)isBattleFlg = false;

		point = lastLookPoint;
		OutputDebugString(("Last Look Point : x[" + std::to_string(point.x) + "] y[" + std::to_string(point.y) + "] z[" + std::to_string(point.z) + "]\n").c_str());
		OutputDebugString(("Last Look Point Length :"+std::to_string(testLookPosLength) + "\n").c_str());

		return;
	}

	if (functions.empty())return;

	if (!functions[moveOperationPointCount]->Update(mecha->GetPosition())) 
	{

		point = functions[moveOperationPointCount]->GetOperationPoint();
		return;
	}
	moveOperationPointCount = (moveOperationPointCount + 1) % functions.size();

	point = functions[moveOperationPointCount]->GetOperationPoint();

	OutputDebugString(("Operation Point : x[" + std::to_string(point.x) + "] y[" + std::to_string(point.y) + "] z[" + std::to_string(point.z) + "]\n").c_str());
}