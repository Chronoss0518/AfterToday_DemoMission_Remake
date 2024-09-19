#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"
#include"../EditFrame/PartsParameters.h"
#include"FunctionComponent/BoostComponent.h"
#include"FunctionComponent/MoveComponent.h"
#include"FunctionComponent/CameraComponent.h"
#include"../Attack/Attack.h"

#include"MechaPartsObjectFunction/WeaponFunction.h"

#ifndef PARTS_DATA_CREATER
#define PARTS_DATA_CREATER(class_type) {GET_CLASS_NAME(class_type),[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<class_type>(); }}
#endif

ChCpp::ModelLoader::XFile<wchar_t> xfileLoader;
ChCpp::ModelLoader::ObjFile<wchar_t> objLoader;

std::map<std::wstring, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>MechaParts::createFunctions
{
	PARTS_DATA_CREATER(EnelgyTankData),
	PARTS_DATA_CREATER(CameraData),
	PARTS_DATA_CREATER(ScopeData),
	PARTS_DATA_CREATER(WalkData),
	PARTS_DATA_CREATER(Aerodynamics),
	PARTS_DATA_CREATER(MoveAcceleration),
	PARTS_DATA_CREATER(NextPos),
	PARTS_DATA_CREATER(Posture),
	PARTS_DATA_CREATER(RightBoostBrust),
	PARTS_DATA_CREATER(LeftBoostBrust),
	PARTS_DATA_CREATER(FrontBoostBrust),
	PARTS_DATA_CREATER(BackBoostBrust),
	PARTS_DATA_CREATER(UpBoostBrust),
	PARTS_DATA_CREATER(DownBoostBrust),
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

	return partsObject;
}

void MechaParts::Load(BaseMecha& _base, ID3D11Device* _device, const std::wstring& _fileName)
{
	std::wstring text = L"";

	{
		ChCpp::WCharFile file;
		file.SetLocaleName("Japanese");
		file.FileOpen(_fileName);
		text = file.FileReadText();
	}

	if (text.empty())return;

	thisFilePath = _fileName;
	
	if (thisFilePath.find(L"/"))
	{
		thisFileName = thisFilePath.substr(thisFilePath.find_last_of(L"/") + 1);
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
	textObject.SetText(_text.c_str());

	unsigned long lineCount = textObject.LineCount();

	LoadModel(_device, textObject.GetTextLine(0));

	hardness = ChStr::GetNumFromText<unsigned long,wchar_t>(textObject.GetTextLine(1).c_str());
	mass = ChStr::GetNumFromText<float,wchar_t>(textObject.GetTextLine(2).c_str());

	for (unsigned long i = 3; i < lineCount; i++)
	{
		i = CreateDatas(_base, textObject, i);
	}

}

void MechaParts::LoadModel(ID3D11Device* _device, const std::wstring& _fileName)
{
	ChCpp::ModelLoader::XFile<wchar_t> loader;

	model->Init(_device);
	loader.CreateModel(model, _fileName);
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

ChPtr::Shared<MechaPartsObject>  MechaParts::SetParameters(BaseMecha& _base, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject)
{

	auto&& parts = SetPartsParameter(_base);
	parts->SetFrame(_frame);
	for (auto&& com : GetComponents<PartsDataBase>())
	{
		com->SetPartsParameter(_base, *parts, _frame);
	}

	AddWeaponData(parts, _base, _jsonObject);

	parts->SetFrame(_frame);
	parts->SetBaseMecha(&_base);

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

void MechaParts::SetParameters()
{
	if (GetComponent<PartsParameters>() != nullptr)return;

	auto&& parts = SetPartsParameter();
	for (auto&& com : GetComponents<PartsDataBase>())
	{
		com->SetPartsParameter(*parts);
	}
}

ChPtr::Shared<PartsParameters>  MechaParts::SetPartsParameter()
{
	auto partsObject = GetComponent<PartsParameters>();

	if (partsObject == nullptr)
	{
		partsObject = SetComponent<PartsParameters>();
	}

	partsObject->mainData.mass = mass;

	partsObject->mainData.hardness = hardness;

	return partsObject;
}


std::wstring MechaParts::Save(const std::wstring& _fileName)
{
	std::wstring res = Serialize();

	{
		ChCpp::WCharFile file;
		file.SetLocaleName("Japanese");
		file.FileOpen(_fileName);
		file.FileWriteText(res);
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
	auto&& jsonBool = _jsonObject->GetJsonBoolean(JSON_PROPEATY_RIGHT_WEAPON);
	if (jsonBool != nullptr)
		_base.AddRightWeaponData(_partsObject);

	jsonBool = _jsonObject->GetJsonBoolean(JSON_PROPEATY_LEFT_WEAPON);
	if (jsonBool != nullptr)
		_base.AddLeftWeaponData(_partsObject);
}

void MechaParts::Draw(const ChMat_11& _mat)
{
	if (model == nullptr)return;
	drawer->Draw(*model, _mat);
}

void EnelgyTankData::RemoveParameter(BaseMecha& _base)
{
	_base.SubMaxEnelgy(maxEnelgy);
	_base.SubChargeEnelgy(chargeEnelgy);
}

unsigned long EnelgyTankData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	maxEnelgy = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(_textPos).c_str());
	chargeEnelgy = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(_textPos + 1).c_str());

	return _textPos + 2;
}

std::wstring EnelgyTankData::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(maxEnelgy) + L"\n";
	res += std::to_wstring(chargeEnelgy) + L"\n";

	return res;
}

void EnelgyTankData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	_base.AddMaxEnelgy(maxEnelgy);
	_base.AddChargeEnelgy(chargeEnelgy);
}

void EnelgyTankData::SetPartsParameter(PartsParameters& _base)
{
	_base.enelgyTankData.maxEnelgy += maxEnelgy;
	_base.enelgyTankData.chargeEnelgy += chargeEnelgy;
}

unsigned long CameraData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	fovy = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_textPos).c_str());
	cameraCount = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(_textPos + 1).c_str());
	cameraObject = _text.GetTextLine(_textPos + 2);
	return _textPos + 3;
}

std::wstring CameraData::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(fovy) + L"\n";
	res += std::to_wstring(cameraCount) + L"\n";
	res += cameraObject + L"\n";
	return res;
}

void CameraData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	//auto camera = ChPtr::Make_S<CameraObject>();

	//_base.AddCamera(camera);
}

unsigned long ScopeData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = CameraData::Deserialize(_text, _textPos);
	minFovy = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(textPos).c_str());
	maxFovy = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(textPos + 1).c_str());
	fovySlideSpeed = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(textPos + 2).c_str());
	return textPos + 3;
}

std::wstring ScopeData::Serialize()
{
	std::wstring res = L"";
	res = CameraData::Serialize();
	res += std::to_wstring(minFovy) + L"\n";
	res += std::to_wstring(maxFovy) + L"\n";
	res += std::to_wstring(fovySlideSpeed) + L"\n";

	return res;
}

void ScopeData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	//auto camera = ChPtr::Make_S<CameraObject>();

	//_base.AddCamera(camera);
}

unsigned long Aerodynamics::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	upPowerParSpeed = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_textPos).c_str());
	return _textPos + 1;
}

std::wstring Aerodynamics::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(upPowerParSpeed) + L"\n";

	return res;
}

void Aerodynamics::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& moveAcceleration = GetComponent<Aerodynamics>(_base);

}

unsigned long MoveAcceleration::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	acceleration = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_textPos).c_str());
	deceleration = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_textPos + 1).c_str());
	return _textPos + 2;
}

std::wstring MoveAcceleration::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(acceleration) + L"\n";
	res += std::to_wstring(deceleration) + L"\n";

	return res;
}

void MoveAcceleration::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& moveAcceleration = GetComponent<MoveAcceleration>(_base);

}

void WalkData::RemoveParameter(BaseMecha& _base)
{
	auto&& walk = GetComponent<BaseMechaMoveComponent>(_base);

	walk->SetMovePow(0.0f);
	walk->SetRotatePow(0.0f);
	walk->SetJumpPow(0.0f);
}

unsigned long WalkData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	movePow = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_textPos).c_str());
	rotatePow = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_textPos + 1).c_str());
	jumpPow = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_textPos + 2).c_str());
	return _textPos + 3;
}

std::wstring WalkData::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(movePow) + L"\n";
	res += std::to_wstring(rotatePow) + L"\n";
	res += std::to_wstring(jumpPow) + L"\n";

	return res;
}

void WalkData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& walk = GetComponent<BaseMechaMoveComponent>(_base);

	walk->SetMovePow(movePow);
	walk->SetRotatePow(rotatePow);
	walk->SetJumpPow(jumpPow);
}

void WalkData::SetPartsParameter(PartsParameters& _base)
{
	_base.walkData.movePower = movePow;
	_base.walkData.rotatePower = rotatePow;
	_base.walkData.jumpPower = jumpPow;
}

void NextPosBase::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto target = LookObj<MechaParts>();

	if (ChPtr::NullCheck(target))return;

	auto&& posObjects = target->GetMesh().GetAllChildlenForName<ChCpp::FrameObject<wchar_t>>(nextPosName);

	if (posObjects.empty())return;

	ChPtr::Shared<ChCpp::FrameObject<wchar_t>> posObject = posObjects[0].lock();

	SetObjectPos(_base, _parts, posObject);
}

void NextPos::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	auto&& mechaParts = LookObj<MechaParts>();

	mechaParts->AddPosition(nextPosName, _targetObject);

}

void Posture::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	auto&& pos = _targetObject->GetComponent<PostureController>();
	if (pos != nullptr)return;
	pos = _targetObject->SetComponent<PostureController>();
	pos->Set(posture);

	auto&& baseParts = LookObj<MechaParts>();
	if (baseParts == nullptr)return;
	baseParts->AddPosture(pos);
}

void BoostBrust::RemoveParameter(BaseMecha& _base)
{
	auto&& obj = GetFrame(_base);

	if (obj == nullptr)return;

	auto&& com = GetComponent<BoostComponent>(_base);

	com->SubBoostPow(boostPower, GetBoostInputName());
	com->SubBoostUseEnelgy(useEnelgy, GetBoostInputName());
	com->SubBoostAvoidPow(avoidPow, GetAvoidInputName());
	com->SubBoostAvoidUseEnelgy(avoidUseEnelgy, GetAvoidInputName());
	com->SetBoostAvoidWait(avoidWait, GetAvoidInputName());

	com->AddBoostWhereBoostName(obj, GetBoostInputName());

}

unsigned long BoostBrust::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	objectName = _text.GetTextLine(_textPos);
	useEnelgy = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(_textPos + 1).c_str());
	boostPower = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_textPos + 2).c_str());
	avoidUseEnelgy = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(_textPos + 3).c_str());
	avoidPow = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_textPos + 4).c_str());
	avoidWait = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(_textPos + 5).c_str());
	return _textPos + 6;
}

std::wstring BoostBrust::Serialize()
{

	std::wstring res = L"";

	res = objectName;
	res += std::to_wstring(useEnelgy) + L"\n";
	res += std::to_wstring(boostPower) + L"\n";
	res += std::to_wstring(avoidUseEnelgy) + L"\n";
	res += std::to_wstring(avoidPow) + L"\n";

	return res;
}

void BoostBrust::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& obj = GetFrame(_base);

	if (obj == nullptr)return;

	auto&& com = GetComponent<BoostComponent>(_base);

	com->AddBoostPow(boostPower, GetBoostInputName());
	com->AddBoostUseEnelgy(useEnelgy, GetBoostInputName());
	com->AddBoostAvoidPow(avoidPow, GetAvoidInputName());
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy, GetAvoidInputName());
	com->SetBoostAvoidWait(avoidWait, GetAvoidInputName());

	com->AddBoostWhereBoostName(obj, GetBoostInputName());
}

void BoostBrust::SetBoostData(PartsParameterStruct::BoostData& _boost)
{
	_boost.boostPower = boostPower;
	_boost.boostUseEnelgy = useEnelgy;
	_boost.avoidPower = avoidPow;
	_boost.avoidUseEnelgy = avoidUseEnelgy;
	_boost.avoidWait = avoidWait;
}

void RightBoostBrust::SetPartsParameter(PartsParameters& _base)
{
	SetBoostData(_base.rightBoostData);
}

void LeftBoostBrust::SetPartsParameter(PartsParameters& _base)
{
	SetBoostData(_base.leftBoostData);
}

void FrontBoostBrust::SetPartsParameter(PartsParameters& _base)
{
	SetBoostData(_base.frontBoostData);
}

void BackBoostBrust::SetPartsParameter(PartsParameters& _base)
{
	SetBoostData(_base.backBoostData);
}

void UpBoostBrust::SetPartsParameter(PartsParameters& _base)
{
	SetBoostData(_base.upBoostData);
}

void DownBoostBrust::SetPartsParameter(PartsParameters& _base)
{
	SetBoostData(_base.downBoostData);
}

ChPtr::Shared<ChCpp::FrameObject<wchar_t>> BoostBrust::GetFrame(BaseMecha& _base)
{
	auto&& base = *LookObj<MechaParts>();
	if (ChPtr::NullCheck(&base))return nullptr;

	auto&& mesh = base.GetMesh();

	auto&& boostList = mesh.GetAllChildlenForName<ChCpp::FrameObject<wchar_t>>(objectName);

	if (boostList.empty())return nullptr;

	return boostList[0].lock();


}

unsigned long WeaponData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
	weaponName = _text.GetTextLine(textPos);
	seFile = _text.GetTextLine(textPos + 1);
	waitTime = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(textPos + 2).c_str());
	lookTarget = _text.GetTextLine(textPos + 3) == L"1";
	return textPos + 4;
}

std::wstring WeaponData::Serialize()
{
	std::wstring res = NextPosBase::Serialize();

	res += weaponName + L"\n";
	res += seFile + L"\n";
	res += std::to_wstring(waitTime) + L"\n";
	res += lookTarget ? L"1" : L"0";

	return res;
}

void WeaponData::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	_parts.GetWeaponFunctions()[_parts.GetWeaponFunctions().size() - 1]->SetObjectPos(_targetObject);
}

void WeaponData::SetWeaponData(PartsParameterStruct::WeaponData& _base)
{
	auto&& look = LookObj<MechaParts>();
	if (look != nullptr)
	{
		_base.partsName = look->GetThisFileName();
	}

	_base.weaponName = weaponName;
	_base.waitTime = waitTime;
}

unsigned long SwordData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = WeaponData::Deserialize(_text, _textPos);
	attackTime = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(textPos).c_str());
	return textPos + 1;
}

std::wstring SwordData::Serialize()
{
	std::wstring res = L"";

	res = WeaponData::Serialize();
	res += std::to_wstring(attackTime) + L"\n";

	return res;
}

void SwordData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto& thisParts = *LookObj<MechaParts>();

	auto function = ChPtr::Make_S<SwordFunction>();

	function->SetPartsObject(&_parts);

	function->SetBaseMecha(&_base);

	function->SetBaseData(this);

	function->Init(thisParts.GetMeshDrawer(), ChD3D11::D3D11Device());

	function->SetFrmae(_frame);

	_parts.AddWeaponFunction(function);

	NextPosBase::SetPartsParameter(_base, _parts, _frame);
}

void SwordData::SetPartsParameter(PartsParameters& _base)
{
	auto&& weap = ChPtr::Make_S<PartsParameterStruct::SwordData>();
	SetWeaponData(*weap);
	weap->attackTime = attackTime;
	weap->damageParSpeed = damageParSpeed;
	_base.weaponData.push_back(weap);
}

unsigned long GunData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = WeaponData::Deserialize(_text, _textPos);
	fireNum = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(textPos).c_str());
	bulletNum = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(textPos + 1).c_str());
	magazineNum = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(textPos + 2).c_str());
	reloadTime = ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(textPos + 3).c_str());
	range = static_cast<unsigned char>(ChStr::GetNumFromText<unsigned long,wchar_t>(_text.GetTextLine(textPos + 4).c_str()));
	frontDir.Deserialize<wchar_t>(_text.GetTextLine(textPos + 5).c_str());
	bulletFile = _text.GetTextLine(textPos + 6).c_str();

	return textPos + 7;

}

std::wstring GunData::Serialize()
{
	std::wstring res = L"";

	res = WeaponData::Serialize();
	res += std::to_wstring(fireNum) + L"\n";
	res += std::to_wstring(bulletNum) + L"\n";
	res += std::to_wstring(magazineNum) + L"\n";
	res += std::to_wstring(reloadTime) + L"\n";
	res += std::to_wstring(range) + L"\n";
	res += frontDir.Serialize<wchar_t>(L",",L";");
	res += bulletFile + L"\n";

	return res;
}

void GunData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto& thisParts = *LookObj<MechaParts>();

	auto function = ChPtr::Make_S<GunFunction>();

	function->SetPartsObject(&_parts);

	function->SetBaseMecha(&_base);

	function->SetBaseData(this);

	function->Init(thisParts.GetMeshDrawer(), ChD3D11::D3D11Device());

	function->SetFrmae(_frame);

	_parts.AddWeaponFunction(function);

	NextPosBase::SetPartsParameter(_base, _parts, _frame);

}

void GunData::SetPartsParameter(PartsParameters& _base)
{
	auto&& weap = ChPtr::Make_S<PartsParameterStruct::GunData>();
	SetWeaponData(*weap);
	weap->fireNum = fireNum;
	weap->bulletNum = bulletNum;
	weap->magazineNum = magazineNum;
	weap->reloadTime = reloadTime;
	weap->range = range;

	auto&& attackData = Attack::CreateAttackData(nullptr, ChD3D11::D3D11Device(), bulletFile);
	attackData->SetPartameter(*weap);

	_base.weaponData.push_back(weap);
}
