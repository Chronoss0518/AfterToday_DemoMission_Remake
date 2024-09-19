#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../../Frames/GameFrame.h"

#include"../../CPU/CPULooker.h"

#include"CPUMovePositionSelector.h"
#include"CPUTargetSelector.h"
#include"CPUMoveInput.h"

ChPtr::Shared<ChCpp::JsonObject<wchar_t>> CPUMovePositionSelect::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();

	auto&& operationPoint = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();

	operationPoint->Set(L"X", ChCpp::JsonNumber<wchar_t>::CreateObject(point.x));

	operationPoint->Set(L"Y", ChCpp::JsonNumber<wchar_t>::CreateObject(point.y));

	operationPoint->Set(L"Z", ChCpp::JsonNumber<wchar_t>::CreateObject(point.z));

	res->Set(L"OperationPoint", operationPoint);

	res->Set(L"Distance", ChCpp::JsonNumber<wchar_t>::CreateObject(distance));

	res->Set(L"AxisFlg", ChCpp::JsonNumber<wchar_t>::CreateObject(axisFlg.GetValue()));

	return res;
}

void CPUMovePositionSelect::Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& operationPoint = _jsonObject->GetJsonObject(L"OperationPoint");

	if (operationPoint != nullptr)
	{
		auto&& x = operationPoint->GetJsonNumber(L"X");
		if (x != nullptr)point.x = *x;

		auto&& y = operationPoint->GetJsonNumber(L"Y");
		if (y != nullptr)point.y = *y;

		auto&& z = operationPoint->GetJsonNumber(L"Z");
		if (z != nullptr)point.z = *z;
	}

	auto&& distanceObject = _jsonObject->GetJsonNumber(L"Distance");
	if (distanceObject != nullptr)distance = *distanceObject;

	auto&& axisFlgObject = _jsonObject->GetJsonNumber(L"AxisFlg");
	if (axisFlgObject != nullptr)axisFlg.SetValue(*axisFlgObject);
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

ChPtr::Shared<ChCpp::JsonObject<wchar_t>> CPUMovePositionSelector::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();

	auto&& operationPointArray = ChPtr::Make_S<ChCpp::JsonArray<wchar_t>>();

	for (auto&& positions : functions)
	{
		operationPointArray->Add(positions->Serialize());
	}

	res->Set(L"OperatorPositions", operationPointArray);

	res->Set(L"CombatModeOff", ChCpp::JsonNumber<wchar_t>::CreateObject(combatModeOffTime));

	res->Set(L"IsTargetPositionInAreaLength", ChCpp::JsonNumber<wchar_t>::CreateObject(isTargetPositionInAreaLength));

	return res;
}

void CPUMovePositionSelector::Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& operationPointArray = _jsonObject->GetJsonArray(L"OperatorPositions");

	if (operationPointArray != nullptr)
	{
		for (unsigned long i = 0; i < operationPointArray->GetCount(); i++)
		{
			auto&& operationPoint = ChPtr::Make_S<CPUMovePositionSelect>();

			operationPoint->Deserialize(operationPointArray->GetJsonObject(i));

			Add(operationPoint);
		}
	}

	auto&& combatModeOffTimeObject = _jsonObject->GetJsonNumber(L"CombatModeOff");
	if (combatModeOffTimeObject != nullptr)combatModeOffTime = *combatModeOffTimeObject;


	auto&& isTargetPositionInAreaLengthObject = _jsonObject->GetJsonNumber(L"IsTargetPositionInAreaLength");
	if (isTargetPositionInAreaLengthObject != nullptr)isTargetPositionInAreaLength = *isTargetPositionInAreaLengthObject;

}

void CPUMovePositionSelector::Update(CPUTargetSelector& _selector, GameFrame& _frame, CPUController& _controller)
{
	//OutputDebugString(L"CPUMovePositionSelector Update Start\n");

	runUpdateLookTargetFlg = false;

	UpdateLookTarget(_selector, _frame);

	UpdateUnLookTarget(_controller);

	//OutputDebugString(L"CPUMovePositionSelector Update End\n");
}

void CPUMovePositionSelector::UpdateLookTarget(CPUTargetSelector& _selector, GameFrame& _frame)
{
	if (!_selector.IsLookTarget())return;

	auto&& mechaList = _frame.GetMechaList().GetObjectList<BaseMecha>();
	if (mechaList.size() <= _selector.GetTargetNo())return;
	targetMecha = mechaList[_selector.GetTargetNo()];
	if (targetMecha.expired())return;
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

	combatModeOffTimeNow = 0;
}

void CPUMovePositionSelector::UpdateUnLookTarget(CPUController& _controller)
{
	if (runUpdateLookTargetFlg)return;
	
	auto&& mecha = _controller.GetBaseMecha();

	if (ChPtr::NullCheck(mecha))return;

	if (mecha->IsBreak())return;

	if (!isDamageFlg && targetMecha.expired())isBattleFlg = false;

	if (mecha->GetDamageDir() != ChVec3())
	{
		isDamageFlg = isBattleFlg = true;
		point = lastLookPoint = mecha->GetDamageDir() * (isTargetPositionInAreaLength + 100.0f);
	}

	if (isBattleFlg)
	{
		combatModeOffTimeNow++;
		if (combatModeOffTime < combatModeOffTimeNow)return;
		combatModeOffTimeNow = combatModeOffTime;

		float testLookPosLength = (lastLookPoint - mecha->GetPosition()).GetLen();

		if (testLookPosLength < isTargetPositionInAreaLength)isDamageFlg = isBattleFlg = false;

		point = lastLookPoint;

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

	//OutputDebugString((L"Operation Point : x[" + std::to_string(point.x) + "] y[" + std::to_string(point.y) + "] z[" + std::to_string(point.z) + "]\n").c_str());
}