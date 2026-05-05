#include"../BaseIncluder.h"

#include"../Application/Application.h"

#include"FieldManager.h"

#include"../PhysicsMachineTest/PhysicsMachine.h"

#define FIELD_MODEL_LIST_PARAM L"FieldModelList"
#define FIELD_POSITIONS_LIST_PARAM L"Positions"
#define FIELD_POINT_NAME_LIST_PARAM L"PointNames"

#define FIELD_MODEL_NAME_PARAM L"ModelName"
#define FIELD_MODEL_POSITION_PARAM L"Position"
#define FIELD_MODEL_ROTATION_PARAM L"Rotation"
#define FIELD_MODEL_SCALE_PARAM L"Scale"
#define FIELD_MODEL_COLLISION_HAND_TYPE_PARAM L"HandType"
#define FIELD_MODEL_HIT_FLG_PARAM L"HitFlg"

#define VECTOR_X_PARAM L"X"
#define VECTOR_Y_PARAM L"Y"
#define VECTOR_Z_PARAM L"Z"

#define FIELD_MODEL_COLLISION_HAND_TYPE_LHAND L"LHand"
#define FIELD_MODEL_COLLISION_HAND_TYPE_RHAND L"RHand"

#define FIELD_POSITION_TARGET_MODEL_PARAM L"TargetModelNo"
#define FIELD_POSITION_NAMES_PARAM L"PositionNames"

#define FIELD_POINT_NAME_PARAM L"PointName"
#define FIELD_POINT_VALUE_PARAM L"PointValue"

#define FIELD_DIRECTORY(current_path) MESH_DIRECTORY(L"Field/" current_path)
#define FIELD_DATA_DIRECTORY(current_path) TARGET_DIRECTORY(L"FieldData/" current_path)

void FieldManager::Init(const std::basic_string<wchar_t>& _fieldFile)
{
	Release();

	std::wstring fieldJsom = L"";

	{
		ChCpp::CharFile file;
		file.FileOpen(FIELD_DATA_DIRECTORY(+_fieldFile), false);
		fieldJsom = ChStr::GetUTF16FromUTF8(file.FileRead());
		fieldJsom = &fieldJsom[1];

		file.FileClose();
	}

	if (fieldJsom == L"")return;

	auto json = ChPtr::SharedSafeCast<ChCpp::JsonObject<wchar_t>>(ChCpp::JsonBaseType<wchar_t>::GetParameter(fieldJsom));

	if (json == nullptr)return;

	CreateFieldModel(json);

	if (fieldModels.empty())return;

	CreatePositions(json);

	if (positions.empty())return;

	CreatePointNames(json);
}

void FieldManager::Release()
{
	if (!fieldModels.empty())fieldModels.clear();
	if (!positions.empty())positions.clear();
	if (!pointNames.empty())pointNames.clear();
}

void FieldManager::CreateFieldModel(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>_object)
{
	auto fieldList = _object->GetJsonArray(FIELD_MODEL_LIST_PARAM);

	if (fieldList == nullptr)return;
	size_t fieldModelCount = fieldList->GetCount();
	if (fieldModelCount <= 0)return;

	ChPtr::Shared<ChCpp::JsonString<wchar_t>>handType = nullptr;
	ChPtr::Shared<ChCpp::JsonBoolean<wchar_t>>hitFlg = nullptr;
	ChLMat baseMat;

	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> model = nullptr;

	for (size_t i = 0; i < fieldModelCount; i++)
	{
		auto fieldModel = fieldList->GetJsonObject(i);
		model = nullptr;
		auto modelName = fieldModel->GetJsonString(FIELD_MODEL_NAME_PARAM);
		if (modelName != nullptr)continue;
		model = CreateModel(modelName->GetString());

		if (model == nullptr)continue;
		if (model->IsInit())continue;

		baseMat.Identity();

		baseMat.SetRotationYPR(CreateVectorValues(fieldModel->GetJsonObject(FIELD_MODEL_ROTATION_PARAM), ChVec3()));
		baseMat.SetScalling(CreateVectorValues(fieldModel->GetJsonObject(FIELD_MODEL_SCALE_PARAM), ChVec3(1.0f)));
		baseMat.SetPosition(CreateVectorValues(fieldModel->GetJsonObject(FIELD_MODEL_POSITION_PARAM), ChVec3()));

		model->SetOutSideTransform(baseMat);

		model->UpdateDrawTransform();

		hitFlg = nullptr;
		hitFlg = fieldModel->GetJsonBoolean(FIELD_MODEL_HIT_FLG_PARAM);

		if (hitFlg != nullptr && hitFlg->IsFlg())
		{
			handType = nullptr;
			handType = fieldModel->GetJsonString(FIELD_MODEL_COLLISION_HAND_TYPE_PARAM);

			PhysicsMachine::AddField(model, handType == nullptr ?
				ChCpp::PanelColliderBase::UseHandType::LeftHand :
				handType->GetString() == FIELD_MODEL_COLLISION_HAND_TYPE_LHAND ?
				ChCpp::PanelColliderBase::UseHandType::LeftHand :
				ChCpp::PanelColliderBase::UseHandType::RightHand);
		}

		fieldModels.push_back(model);
	}

}

ChPtr::Shared<ChD3D11::Mesh11<wchar_t>>FieldManager::CreateModel(const std::basic_string<wchar_t>& _modelName)
{
	auto device = AppIns().GetDirect3D11().GetDevice();
	size_t pos = _modelName.find_last_of(L".");

	if (_modelName.substr(pos) == L".x") {
		auto res = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
		res->Init(device);
		ChCpp::ModelController::XFile<wchar_t> loader;
		loader.LoadModel(FIELD_DIRECTORY(+_modelName));
		loader.CreateModel(res);
		return res;
	}
	else if (_modelName.substr(pos) == L".obj") {
		auto res = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
		res->Init(device);
		ChCpp::ModelController::ObjFile<wchar_t> loader;
		loader.LoadModel(FIELD_DIRECTORY(+_modelName));
		loader.CreateModel(res);
		return res;
	}
	else
	{
		return nullptr;
	}
}

ChVec3 FieldManager::CreateVectorValues(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>_object, const ChVec3& _default)
{
	if (_object == nullptr)return _default;

	ChVec3 res = _default;

	ChPtr::Shared<ChCpp::JsonNumber<wchar_t>>number = nullptr;

	number = _object->GetJsonNumber(VECTOR_X_PARAM);
	res.x = number == nullptr ? res.x : static_cast<float>(number->GetFloatValue());

	number = nullptr;
	number = _object->GetJsonNumber(VECTOR_Y_PARAM);
	res.y = number == nullptr ? res.y : static_cast<float>(number->GetFloatValue());

	number = nullptr;
	number = _object->GetJsonNumber(VECTOR_Z_PARAM);
	res.z = number == nullptr ? res.z : static_cast<float>(number->GetFloatValue());

	return res;
}

void FieldManager::CreatePositions(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>_object)
{
	auto positionList = _object->GetJsonArray(FIELD_POSITIONS_LIST_PARAM);

	if (positionList == nullptr)return;

	size_t count = positionList->GetCount();

	for (size_t i = 0; i < count; i++)
	{
		auto positionObject = positionList->GetJsonObject(i);

		if (positionObject == nullptr)continue;

		auto targetNo = positionObject->GetJsonNumber(FIELD_POSITION_TARGET_MODEL_PARAM);

		if (targetNo == nullptr)continue;

		auto targetNoValue = targetNo->GetIntValue();

		if (targetNoValue < 0 || targetNoValue >= fieldModels.size())continue;

		auto positionNames = positionObject->GetJsonArray(FIELD_POSITION_NAMES_PARAM);

		if (positionNames == nullptr)continue;

		for (size_t j = 0; j < positionNames->GetCount(); j++)
		{
			auto positionName = positionNames->GetJsonString(j);

			if (positionName == nullptr)continue;

			auto positionNameValue = positionName->GetString();

			if (positionNameValue == L"")continue;

			auto childs = fieldModels[targetNoValue]->GetAllChildlenForName<ChCpp::FrameObject<wchar_t>>(positionNameValue);

			if (childs.empty())continue;
			if (childs[0].expired())continue;
			
			auto child = childs[0].lock();

			child->UpdateAllDrawTransform();

			auto pos = ChPtr::Make_S<ChVec3>();
			*pos = child->GetDrawLHandMatrix().Transform(ChVec3());
			
			positions.push_back(pos);

		}
	}
	
}

void FieldManager::CreatePointNames(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>_object)
{
	auto pointList = _object->GetJsonObject(FIELD_POINT_NAME_LIST_PARAM);

	if (pointList == nullptr)return;

	auto pointListKey = pointList->GetKeys();

	for (size_t i = 0; i < pointListKey.size(); i++)
	{
		auto points = pointList->GetJsonArray(pointListKey[i]);

		size_t count = points->GetCount();

		if (count <= 0)continue;

		for (size_t j = 0; j < count; j++)
		{
			auto num = points->GetJsonNumber(j);
			if (num == nullptr)continue;
			auto numValue = num->GetIntValue();

			if (numValue < 0 || numValue > positions.size())continue;

			pointNames[pointListKey[i]].push_back(static_cast<size_t>(numValue));
		}
	}
}

std::vector<ChPtr::Shared<ChVec3>> FieldManager::GetPositionFromName(const std::basic_string<wchar_t>& _pointName)
{
	std::vector<ChPtr::Shared<ChVec3>> res;

	auto pointName = pointNames.find(_pointName);

	if (pointName == pointNames.end())return res;

	for (size_t i = 0; i < pointName->second.size(); i++)
	{
		if (pointName->second[i] >= positions.size())continue;

		res.push_back(positions[pointName->second[i]]);
	}

	return res;
}

void FieldManager::Draw(ChD3D11::Shader::BaseDrawMesh11<wchar_t>& _meshDrawer, ChLMat _viewMat)
{
	for (size_t i = 0; i < fieldModels.size(); i++)
	{
		ChLMat drawMat = fieldModels[i]->GetDrawLHandMatrix();
		drawMat = drawMat * _viewMat;
		ChVec3 point = drawMat.Transform(ChVec3());

		if (point.GetLen() > maxDrawLen)continue;

		_meshDrawer.Draw(*fieldModels[i]);
	}
}
