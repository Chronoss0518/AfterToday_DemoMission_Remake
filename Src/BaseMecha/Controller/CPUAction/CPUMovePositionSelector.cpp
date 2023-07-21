#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../../Frames/GameFrame.h"

#include"../../CPU/CPULooker.h"

#include"CPUMovePositionSelector.h"
#include"CPUTargetSelector.h"
#include"CPUMoveInput.h"

ChPtr::Shared<ChCpp::JsonObject> CPUMovePositionSelect::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject>();

	auto&& operationPoint = ChPtr::Make_S<ChCpp::JsonObject>();

	operationPoint->SetObject("X", ChCpp::JsonNumber::CreateObject(point.x));

	operationPoint->SetObject("Y", ChCpp::JsonNumber::CreateObject(point.y));

	operationPoint->SetObject("Z", ChCpp::JsonNumber::CreateObject(point.z));

	res->SetObject("OperationPoint", operationPoint);

	res->SetObject("Distance", ChCpp::JsonNumber::CreateObject(distance));

	res->SetObject("AxisFlg", ChCpp::JsonNumber::CreateObject(axisFlg.GetValue()));

	return res;
}

void CPUMovePositionSelect::Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& operationPoint = _jsonObject->GetJsonObject("OperationPoint");

	if (operationPoint != nullptr)
	{
		auto&& x = operationPoint->GetJsonNumber("X");
		if (x != nullptr)point.x = *x;

		auto&& y = operationPoint->GetJsonNumber("Y");
		if (y != nullptr)point.y = *y;

		auto&& z = operationPoint->GetJsonNumber("Z");
		if (z != nullptr)point.z = *z;
	}

	auto&& distanceObject = _jsonObject->GetJsonNumber("Distance");
	if (distanceObject != nullptr)distance = *distanceObject;

	auto&& axisFlgObject = _jsonObject->GetJsonNumber("AxisFlg");
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

ChPtr::Shared<ChCpp::JsonObject> CPUMovePositionSelector::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject>();

	auto&& operationPointArray = ChPtr::Make_S<ChCpp::JsonArray>();

	for (auto&& positions : functions)
	{
		operationPointArray->AddObject(positions->Serialize());
	}

	res->SetObject("OperatorPositions", operationPointArray);

	return res;
}

void CPUMovePositionSelector::Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& operationPointArray = _jsonObject->GetJsonArray("OperatorPositions");

	if (operationPointArray == nullptr)return;

	for (unsigned long i = 0; i < operationPointArray->GetCount(); i++)
	{
		auto&& operationPoint = ChPtr::Make_S<CPUMovePositionSelect>();

		operationPoint->Deserialize(operationPointArray->GetJsonObject(i));

		Add(operationPoint);
	}
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