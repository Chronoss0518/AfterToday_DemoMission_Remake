#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"
#include "MechaPartsData/EnergyTankData.h"
#include "MechaPartsData/CameraData.h"
#include "MechaPartsData/ScopeData.h"
#include "MechaPartsData/AerodynamicsData.h"
#include "MechaPartsData/WalkData.h"
#include "MechaPartsData/CaterpillarData.h"
#include "MechaPartsData/NextPosData.h"
#include "MechaPartsData/BoostBrustData.h"
#include "MechaPartsData/SwordData.h"
#include "MechaPartsData/GunData.h"

#include"../EditFrame/PartsParameters.h"

#include"FunctionComponent/WeaponComponent.h"

#ifndef PARTS_DATA_CREATER
#define PARTS_DATA_CREATER(class_type) {GET_CLASS_NAME(class_type),[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<class_type>(); }}
#endif

ChCpp::ModelController::XFile<wchar_t> xfileLoader;
ChCpp::ModelController::ObjFile<wchar_t> objLoader;

std::map<std::wstring, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>MechaParts::createFunctions
{
	PARTS_DATA_CREATER(EnergyTankData),
	PARTS_DATA_CREATER(CameraData),
	PARTS_DATA_CREATER(ScopeData),
	PARTS_DATA_CREATER(WalkData),
	PARTS_DATA_CREATER(CaterpillarData),
	PARTS_DATA_CREATER(AerodynamicsData),
	PARTS_DATA_CREATER(NextPosData),
	PARTS_DATA_CREATER(BoostBrust),
	PARTS_DATA_CREATER(SwordData),
	PARTS_DATA_CREATER(GunData),
};

ChPtr::Shared<MechaPartsObject> MechaParts::LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, GameFrame* _frame, const std::wstring& _partsFilePath)
{
	auto&& jsonObject = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();
	jsonObject->Set(JSON_PROPEATY_PARTS_NAME, ChCpp::JsonString<wchar_t>::CreateObject(_partsFilePath));

	return LoadParts(_base, _device, _drawer, _frame, jsonObject);
}

ChPtr::Shared<MechaPartsObject> MechaParts::LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject, const std::wstring& _positionObjectType, ChPtr::Shared<MechaPartsObject> _parent)
{
	auto&& partsName = _jsonObject->GetJsonString(JSON_PROPEATY_PARTS_NAME);

	auto&& loadPartss = LoadPartsList();
	auto it = loadPartss.find(*partsName);
	if (it != loadPartss.end())
	{
		it->second->SetMeshDrawer(_drawer);

		auto&& partsObject = (*it).second->SetParameters(_base, _frame, _jsonObject);

		if (_parent != nullptr)
		{
			_parent->AddChildObject(_positionObjectType, partsObject);
		}

		(*it).second->CreateChild(partsObject, _base, _device, _drawer, _frame, _jsonObject);

		partsObject->CreateEnd();

		return partsObject;
	}

	auto mechaParts = ChPtr::Make_S<MechaParts>();
	mechaParts->Load(_base, _device, *partsName);

	if (mechaParts->GetThisFileName().empty())return nullptr;

	loadPartss[*partsName] = mechaParts;
	mechaParts->SetMeshDrawer(_drawer);

	auto&& partsObject = mechaParts->SetParameters(_base, _frame, _jsonObject);

	if (_parent != nullptr)
	{
		_parent->AddChildObject(_positionObjectType, partsObject);
	}

	mechaParts->CreateChild(partsObject,_base, _device, _drawer, _frame, _jsonObject);

	partsObject->CreateEnd();

	return partsObject;
}

void MechaParts::Load(BaseMecha& _base, ID3D11Device* _device, const std::wstring& _fileName)
{
	std::wstring text = L"";

	{
		ChCpp::CharFile file;
		file.FileOpen(_fileName, false);
		text = ChStr::GetUTF16FromUTF8(file.FileRead());
	}

	if (text.empty())return;

	thisFilePath = _fileName;
	
	if (thisFilePath.find(L"/"))
	{
		thisFileName = thisFilePath.substr(thisFilePath.rfind(L"/") + 1);
	}

	if (thisFileName.find(L"."))
	{
		SetMyName(thisFileName.substr(0, thisFileName.find(L".")));
	}
	Deserialize(_base, _device, text);
}

void MechaParts::Deserialize(BaseMecha& _base, ID3D11Device* _device, const std::wstring& _text)
{
	ChCpp::TextObject<wchar_t> textObject;
	textObject.SetCutChar(L"\n");
	textObject.SetText(_text.c_str());

	size_t lineCount = textObject.LineCount();

	LoadModel(_device, textObject.GetTextLine(0));

	hardness = ChStr::GetNumFromText<unsigned long>(textObject.GetTextLine(1).c_str());
	mass = ChStr::GetNumFromText<float>(textObject.GetTextLine(2).c_str());

	for (unsigned long i = 3; i < lineCount; i++)
	{
		i = CreateDatas(_base, textObject, i);
	}
}

void MechaParts::LoadModel(ID3D11Device* _device, const std::wstring& _fileName)
{
	ChCpp::ModelController::XFile<wchar_t> loader;

	model->Init(_device);
	loader.LoadModel(_fileName);
	loader.CreateModel(model);
	if (model->GetMyName() == L"Root")
	{
		defaultFrameMat = model->GetFrameTransformLMat();
	}

	float test = model->GetInitAllFrameMinPos().y;
	if (groundHeight > test)
	{
		groundHeight = test;
	}

	
}

void MechaParts::RemoveParameter(BaseMecha& _base)
{
	_base.SubMass(mass);

	for (auto&& partsData : GetComponents<PartsDataBase>())
	{
		partsData->RemoveParameter(_base);
	}
}

unsigned long MechaParts::CreateDatas(BaseMecha& _base, ChCpp::TextObject<wchar_t>& _textObject, unsigned long _linePos)
{

	std::wstring typeName = _textObject.GetTextLine(_linePos);
	if (typeName.length() <= 0)return _linePos + 1;
	typeName.pop_back();
	typeName.pop_back();
	auto&& createFunction = createFunctions.find(typeName);
	if (createFunction == createFunctions.end())return _linePos + 1;
	auto&& parts = (*createFunction).second(*this);
	unsigned long linePos = parts->Deserialize(_textObject, _linePos + 1);
	return linePos;
}

void MechaParts::CreateChild(ChPtr::Shared<MechaPartsObject> _partsObject, BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject)
{
	for (auto&& posData : positions)
	{
		auto&& jsonObject = _jsonObject->GetJsonObject(posData.first);
		if (jsonObject == nullptr)continue;

		auto&& childParts = LoadParts(_base, _device, drawer, _frame, jsonObject, posData.first,_partsObject);


	}
	_partsObject->SetHitSize();
}

ChPtr::Shared<MechaPartsObject> MechaParts::SetParameters(BaseMecha& _base, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject)
{

	auto&& parts = SetPartsParameter(_base);
	parts->SetFrame(_frame);
	parts->SetBaseMecha(&_base);

	for (auto&& com : GetComponents<PartsDataBase>())
	{
		com->SetPartsParameter(_base, *parts, _frame);
	}

	AddWeaponData(parts, _base, _jsonObject);

	parts->CreateAnchor();

	return parts;
}

ChPtr::Shared<MechaPartsObject> MechaParts::SetPartsParameter(BaseMecha& _base)
{
	auto partsObject = ChPtr::Make_S<MechaPartsObject>();

	partsObject->baseParts = this;

	_base.AddMass(mass);

	_base.SetGroundHeight(groundHeight);

	return partsObject;
}

void MechaParts::SetParameters(MechaPartsObject& _parts)
{
	auto partsParameter = ChPtr::Make_S<PartsParameters>();

	partsParameter->mainData.mass = mass;

	partsParameter->mainData.hardness = hardness;

	for (auto&& com : GetComponents<PartsDataBase>())
	{
		com->SetPartsParameter(*partsParameter, _parts);
	}

	_parts.SetPartsParameters(partsParameter);
}

std::wstring MechaParts::Save(const std::wstring& _fileName)
{
	std::wstring res = Serialize();

	{
		ChCpp::CharFile file;
		file.FileOpen(_fileName, true);
		file.FileWrite(ChStr::GetUTF8FromUTF16(res));
		file.FileClose();
	}

	return res;
}

std::wstring MechaParts::Serialize()
{
	std::wstring res;

	//モデル名//
	res += model->GetModelName() + L"\n";

	//パーツの硬さ//
	res += std::to_wstring(hardness) + L"\n";

	//パーツの重さ//
	res += std::to_wstring(mass) + L"\n";

	for (auto&& com : GetComponents<PartsDataBase>())
	{
		res += com->GetPartsTypeTag() + L":{\n";
		res += com->Serialize();
		res += L"}\n";
	}

	return res;
}

void MechaParts::AddWeaponData(ChPtr::Shared<MechaPartsObject> _partsObject, BaseMecha& _base, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject)
{
	SetWeaponFunction(_partsObject, _base, _jsonObject, JSON_PROPEATY_RIGHT_WEAPON, WeaponHandType::Right);

	SetWeaponFunction(_partsObject, _base, _jsonObject, JSON_PROPEATY_LEFT_WEAPON, WeaponHandType::Left);
}

void MechaParts::SetWeaponFunction(ChPtr::Shared<MechaPartsObject> _partsObject, BaseMecha& _base, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject,const std::wstring& _jsonPropertyName, WeaponHandType _type)
{
	auto&& jsonArray = _jsonObject->GetJsonArray(_jsonPropertyName);
	if (jsonArray == nullptr)return;

	auto&& com = _base.GetComponentObject<WeaponComponent>();
	auto&& weaponFunctions = _partsObject->GetWeaponFunctions();

	for (int i = 0; i < jsonArray->GetCount(); i++)
	{
		auto&& jsonNum = jsonArray->GetJsonNumber(i);
		if (jsonNum == nullptr)continue;
		auto&& num = static_cast<char>(jsonNum->GetIntValue());
		_partsObject->AddWeaponPaletteCounter(num, _type);
		if (PALETTE_COUNT <= num || num < 0)continue;
		com->SetWeapon(_type, num, weaponFunctions[i]);
	}

}

void MechaParts::Draw(const ChMat_11& _mat)
{
	if (model == nullptr)return;
	drawer->Draw(*model, _mat);
}

ChD3D11::Mesh11<wchar_t>& PartsDataBase::GetModel(MechaPartsObject& _base)
{
	return *_base.GetBaseObject()->model;
}
