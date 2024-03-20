#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"
#include"FunctionComponent/BoostComponent.h"
#include"FunctionComponent/MoveComponent.h"
#include"FunctionComponent/CameraComponent.h"
#include"../Attack/Attack.h"

#include"MechaPartsObjectFunction/WeaponFunction.h"

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY("RobotParts/" current_path)
#endif

#ifndef PARTS_DATA_CREATER
#define PARTS_DATA_CREATER(class_type) {GET_CLASS_NAME(class_type),[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<class_type>(); }}
#endif

std::map<std::string, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>MechaParts::createFunctions
{
	PARTS_DATA_CREATER(EnelgyTankData),
	PARTS_DATA_CREATER(CameraData),
	PARTS_DATA_CREATER(ScopeData),
	PARTS_DATA_CREATER(WalkData),
	PARTS_DATA_CREATER(Aerodynamics),
	PARTS_DATA_CREATER(MoveAcceleration),
	PARTS_DATA_CREATER(NextPos),
	PARTS_DATA_CREATER(RightBoostBrust),
	PARTS_DATA_CREATER(LeftBoostBrust),
	PARTS_DATA_CREATER(FrontBoostBrust),
	PARTS_DATA_CREATER(BackBoostBrust),
	PARTS_DATA_CREATER(UpBoostBrust),
	PARTS_DATA_CREATER(DownBoostBrust),
	PARTS_DATA_CREATER(SwordData),
	PARTS_DATA_CREATER(GunData),
};

ChPtr::Shared<MechaPartsObject> MechaParts::LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11* _drawer, GameFrame* _frame, const std::string& _partsFilePath)
{
	auto&& jsonObject = ChPtr::Make_S<ChCpp::JsonObject>();
	jsonObject->Set(JSON_PROPEATY_PARTS_NAME, ChCpp::JsonString::CreateObject(_partsFilePath));

	return LoadParts(_base, _device, _drawer, _frame, jsonObject);
}

ChPtr::Shared<MechaPartsObject> MechaParts::LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject> _jsonObject)
{
	auto&& partsName = _jsonObject->GetJsonString(JSON_PROPEATY_PARTS_NAME);

	auto&& loadPartss = LoadPartsList();
	auto it = loadPartss.find(*partsName);
	if (it != loadPartss.end())
	{
		auto&& partsObject = (*it).second->SetParameters(_base, _frame, _jsonObject);

		partsObject->CreateFramePosture(it->second->model.get());

		(*it).second->CreateChild(partsObject, _base, _device, _drawer, _frame, _jsonObject);

		return partsObject;
	}

	auto mechaParts = ChPtr::Make_S<MechaParts>();
	mechaParts->Load(_base, _device, *partsName);

	if (mechaParts->GetThisFileName().empty())return nullptr;

	loadPartss[*partsName] = mechaParts;
	mechaParts->SetMeshDrawer(_drawer);

	auto&& partsObject = mechaParts->SetParameters(_base, _frame, _jsonObject);

	partsObject->CreateFramePosture(mechaParts->model.get());

	mechaParts->CreateChild(partsObject,_base, _device, _drawer, _frame, _jsonObject);

	return partsObject;
}

void MechaParts::Load(BaseMecha& _base, ID3D11Device* _device, const std::string& _fileName)
{
	std::string text = "";

	{
		ChCpp::CharFile file;
		file.FileOpen(_fileName);
		text = file.FileReadText();
	}

	if (text.empty())return;

	thisFilePath = _fileName;
	
	if (thisFilePath.find("/"))
	{
		thisFileName = thisFilePath.substr(thisFilePath.find_last_of("/") + 1);
	}

	Deserialize(_base, _device, text);
}

void MechaParts::Deserialize(BaseMecha& _base, ID3D11Device* _device, const std::string& _text)
{

	ChCpp::TextObject textObject;
	textObject.SetText(_text);

	unsigned long lineCount = textObject.LineCount();

	{
		ChCpp::ModelLoader::XFile loader;
		model->Init(_device);
		std::string tmp = textObject.GetTextLine(0);
		loader.CreateModel(model, tmp);
		defaultFrameMat = model->GetFrameTransformLMat();

		auto test = model->GetInitAllFrameMinPos().y;
		if (groundHeight > test)
		{
			groundHeight = test;
		}
	}

	hardness = static_cast<unsigned long>(std::atoll(textObject.GetTextLine(1).c_str()));
	mass = static_cast<float>(std::atof(textObject.GetTextLine(2).c_str()));

	for (unsigned long i = 3; i < lineCount; i++)
	{
		i = CreateDatas(_base, textObject, i);
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

unsigned long MechaParts::CreateDatas(BaseMecha& _base, ChCpp::TextObject& _textObject, unsigned long _linePos)
{

	std::string typeName = _textObject.GetTextLine(_linePos);
	if (typeName.length() <= 0)return _linePos + 1;
	typeName.pop_back();
	typeName.pop_back();
	auto&& createFunction = createFunctions.find(typeName);
	if (createFunction == createFunctions.end())return _linePos + 1;
	auto&& parts = (*createFunction).second(*this);
	unsigned long linePos = parts->Deserialize(_textObject, _linePos + 1);
	return linePos;
}

void MechaParts::CreateChild(ChPtr::Shared<MechaPartsObject> _partsObject, BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject> _jsonObject)
{
	for (auto&& posData : positions)
	{
		auto&& jsonObject = _jsonObject->GetJsonObject(posData.first);
		if (jsonObject == nullptr)continue;

		auto&& childParts = LoadParts(_base, _device, drawer, _frame, jsonObject);

		childParts->SetPositoinObject(_partsObject.get(), posData.second);
		_partsObject->AddChildObject(posData.first, childParts);

	}
	_partsObject->SetHitSize();
}

ChPtr::Shared<MechaPartsObject>  MechaParts::SetParameters(BaseMecha& _base, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject> _jsonObject)
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

std::string MechaParts::Save(const std::string& _fileName)
{
	std::string res = Serialize();

	{
		ChCpp::CharFile file;
		file.FileOpen(_fileName);
		file.FileWriteText(res);
		file.FileClose();
	}

	return res;
}

std::string MechaParts::Serialize()
{
	std::string res;

	//モデル名//
	res += model->GetModelName() + "\n";

	//パーツの硬さ//
	res += std::to_string(hardness) + "\n";

	//パーツの重さ//
	res += std::to_string(mass) + "\n";

	for (auto&& com : GetComponents<PartsDataBase>())
	{
		res += com->GetPartsTypeTag() + ":{\n";
		res += com->Serialize();
		res += "}\n";
	}

	return res;
}

void MechaParts::AddWeaponData(ChPtr::Shared<MechaPartsObject> _partsObject, BaseMecha& _base, ChPtr::Shared<ChCpp::JsonObject> _jsonObject)
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

unsigned long EnelgyTankData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	maxEnelgy = std::atol(_text.GetTextLine(_textPos).c_str());
	chargeEnelgy = std::atol(_text.GetTextLine(_textPos + 1).c_str());

	return _textPos + 2;
}

std::string EnelgyTankData::Serialize()
{
	std::string res = "";

	res += std::to_string(maxEnelgy) + "\n";
	res += std::to_string(chargeEnelgy) + "\n";

	return res;
}

void EnelgyTankData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	_base.AddMaxEnelgy(maxEnelgy);
	_base.AddChargeEnelgy(chargeEnelgy);
}

unsigned long CameraData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	fovy = static_cast<float>(std::atof(_text.GetTextLine(_textPos).c_str()));
	cameraCount = std::atol(_text.GetTextLine(_textPos + 1).c_str());
	cameraObject = _text.GetTextLine(_textPos + 2);
	return _textPos + 3;
}

std::string CameraData::Serialize()
{
	std::string res = "";

	res += std::to_string(fovy) + "\n";
	res += std::to_string(cameraCount) + "\n";
	res += cameraObject + "\n";
	return res;
}

void CameraData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	//auto camera = ChPtr::Make_S<CameraObject>();

	//_base.AddCamera(camera);
}

unsigned long ScopeData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	unsigned long textPos = CameraData::Deserialize(_text, _textPos);
	minFovy = static_cast<float>(std::atof(_text.GetTextLine(textPos).c_str()));
	maxFovy = static_cast<float>(std::atof(_text.GetTextLine(textPos + 1).c_str()));
	fovySlideSpeed = static_cast<float>(std::atof(_text.GetTextLine(textPos + 2).c_str()));
	return textPos + 3;
}

std::string ScopeData::Serialize()
{
	std::string res = "";
	res = CameraData::Serialize();
	res += std::to_string(minFovy) + "\n";
	res += std::to_string(maxFovy) + "\n";
	res += std::to_string(fovySlideSpeed) + "\n";

	return res;
}

void ScopeData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	//auto camera = ChPtr::Make_S<CameraObject>();

	//_base.AddCamera(camera);
}

unsigned long Aerodynamics::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	upPowerParSpeed = static_cast<float>(std::atof(_text.GetTextLine(_textPos).c_str()));
	return _textPos + 1;
}

std::string Aerodynamics::Serialize()
{
	std::string res = "";

	res += std::to_string(upPowerParSpeed) + "\n";

	return res;
}

void Aerodynamics::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& moveAcceleration = GetComponent<Aerodynamics>(_base);

}

unsigned long MoveAcceleration::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	acceleration = static_cast<float>(std::atof(_text.GetTextLine(_textPos).c_str()));
	deceleration = static_cast<float>(std::atof(_text.GetTextLine(_textPos + 1).c_str()));
	return _textPos + 2;
}

std::string MoveAcceleration::Serialize()
{
	std::string res = "";

	res += std::to_string(acceleration) + "\n";
	res += std::to_string(deceleration) + "\n";

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

unsigned long WalkData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	movePow = static_cast<float>(std::atof(_text.GetTextLine(_textPos).c_str()));
	rotatePow = static_cast<float>(std::atof(_text.GetTextLine(_textPos + 1).c_str()));
	jumpPow = static_cast<float>(std::atof(_text.GetTextLine(_textPos + 2).c_str()));
	return _textPos + 3;
}

std::string WalkData::Serialize()
{
	std::string res = "";

	res += std::to_string(movePow) + "\n";
	res += std::to_string(rotatePow) + "\n";
	res += std::to_string(jumpPow) + "\n";

	return res;
}

void WalkData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& walk = GetComponent<BaseMechaMoveComponent>(_base);

	walk->SetMovePow(movePow);
	walk->SetRotatePow(rotatePow);
	walk->SetJumpPow(jumpPow);
}

void NextPosBase::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto target = LookObj<MechaParts>();

	if (ChPtr::NullCheck(target))return;

	auto&& posObjects = target->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>(nextPosName);

	if (posObjects.empty())return;

	ChPtr::Shared<ChCpp::FrameObject> posObject = posObjects[0].lock();

	SetObjectPos(_base, _parts, posObject);
}

void NextPos::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	auto&& mechaParts = LookObj<MechaParts>();

	mechaParts->AddPosition(nextPosName, _targetObject);

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

unsigned long BoostBrust::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	objectName = _text.GetTextLine(_textPos);
	useEnelgy = std::atol(_text.GetTextLine(_textPos + 1).c_str());
	boostPower = static_cast<float>(std::atof(_text.GetTextLine(_textPos + 2).c_str()));
	avoidUseEnelgy = std::atol(_text.GetTextLine(_textPos + 3).c_str());
	avoidPow = static_cast<float>(std::atof(_text.GetTextLine(_textPos + 4).c_str()));
	avoidWait = std::atol(_text.GetTextLine(_textPos + 5).c_str());
	return _textPos + 6;
}

std::string BoostBrust::Serialize()
{

	std::string res = "";

	res = objectName;
	res += std::to_string(useEnelgy) + "\n";
	res += std::to_string(boostPower) + "\n";
	res += std::to_string(avoidUseEnelgy) + "\n";
	res += std::to_string(avoidPow) + "\n";

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

ChPtr::Shared<ChCpp::FrameObject> BoostBrust::GetFrame(BaseMecha& _base)
{
	auto&& base = *LookObj<MechaParts>();
	if (ChPtr::NullCheck(&base))return nullptr;

	auto&& mesh = base.GetMesh();

	auto&& boostList = mesh.GetAllChildlenForName<ChCpp::FrameObject>(objectName);

	if (boostList.empty())return nullptr;

	return boostList[0].lock();


}

unsigned long WeaponData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
	weaponName = _text.GetTextLine(textPos);
	seFile = _text.GetTextLine(textPos + 1);
	weatTime = std::atol(_text.GetTextLine(textPos + 2).c_str());
	return textPos + 3;
}

std::string WeaponData::Serialize()
{
	std::string res = NextPosBase::Serialize();

	res += weaponName + "\n";
	res += seFile + "\n";
	res += std::to_string(weatTime) + "\n";

	return res;
}

void WeaponData::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	_parts.GetWeaponFunctions()[_parts.GetWeaponFunctions().size() - 1]->SetObjectPos(_targetObject);
}

unsigned long SwordData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	unsigned long textPos = WeaponData::Deserialize(_text, _textPos);
	attackTime = std::atol(_text.GetTextLine(textPos).c_str());
	return textPos + 1;
}

std::string SwordData::Serialize()
{
	std::string res = "";

	res = WeaponData::Serialize();
	res += std::to_string(attackTime) + "\n";

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

unsigned long GunData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	unsigned long textPos = WeaponData::Deserialize(_text, _textPos);
	fireNum = std::atol(_text.GetTextLine(textPos).c_str());
	bulletNum = std::atol(_text.GetTextLine(textPos + 1).c_str());
	magazineNum = std::atol(_text.GetTextLine(textPos + 2).c_str());
	reloadTime = std::atol(_text.GetTextLine(textPos + 3).c_str());
	range = static_cast<unsigned char>(std::atol(_text.GetTextLine(textPos + 4).c_str()));
	bulletFile = _text.GetTextLine(textPos + 5).c_str();

	return textPos + 6;

}

std::string GunData::Serialize()
{
	std::string res = "";

	res = WeaponData::Serialize();
	res += std::to_string(fireNum) + "\n";
	res += std::to_string(bulletNum) + "\n";
	res += std::to_string(magazineNum) + "\n";
	res += std::to_string(reloadTime) + "\n";
	res += std::to_string(range) + "\n";
	res += bulletFile + "\n";

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

unsigned long PostureBase::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	return _textPos;
}

std::string PostureBase::Serialize()
{
	return "";
}

void PostureBase::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{

}
