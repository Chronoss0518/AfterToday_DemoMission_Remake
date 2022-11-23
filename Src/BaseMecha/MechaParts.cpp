#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Bullet/Bullet.h"
#include"CaneraObject.h"
#include"MechaPartsObject.h"
#include"WeaponObject.h"
#include"MechaParts.h"
#include"FunctionComponent/BoostComponents.h"
#include"FunctionComponent/MoveComponent.h"

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY("RobotParts/" current_path)
#endif

std::map<std::string, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>MechaParts::createFunctions
{
	{"EnelgyTank:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<EnelgyTankData>(); }},
	{"Camera:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<CameraData>(); }},
	{"Scope:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<ScopeData>(); }},
	{"Walk:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<WalkData>(); }},
	{"Sword:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<SwordData>(); }},
	{"Gun:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<GunData>(); }},
	{"RightArmPos:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<RightArmPos>(); }},
	{"LeftArmPos:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<LeftArmPos>(); }},
	{"FootPos:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<FootPos>(); }},
	{"HeadPos:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<HeadPos>(); }},
	{"BoostPos:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<BoostPos>(); }},
	{"RightBoostBrust:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<RightBoostBrust>(); }},
	{"LeftBoostBrust:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<LeftBoostBrust>(); }},
	{"FrontBoostBrust:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<FrontBoostBrust>(); }},
	{"BackBoostBrust:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<BackBoostBrust>(); }},
	{"UpBoostBrust:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<UpBoostBrust>(); }},
	{"DownBoostBrust:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<DownBoostBrust>(); }},
	{"HaveRightWeaponPos:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<HaveRightWeaponPos>(); }},
	{"HaveLeftWeaponPos:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<HaveLeftWeaponPos>(); }},
	{"ShotPos:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<ShotPos>(); }},
};

void MechaParts::LoadParts(BaseMecha& _base, ID3D11Device* _device, const std::string& _fileName, BaseMecha::PartsPosNames _name)
{

	auto&& loadPartss = LoadPartsList();
	auto it = loadPartss.find(_fileName);
	if (it != loadPartss.end())
	{
		(*it).second->SetParameters(_base, _name);
		return;
	}

	auto mechaParts = ChPtr::Make_S<MechaParts>();
	mechaParts->Load(_base, _device, _fileName);

	if (mechaParts->GetThisFileName().empty())return;

	loadPartss[_fileName] = mechaParts;

	mechaParts->SetParameters(_base, _name);

}


void MechaParts::Load(BaseMecha& _base, ID3D11Device* _device, const std::string& _fileName)
{
	std::string text = "";

	{
		ChCpp::File<> file;
		file.FileOpen(_fileName);
		text = file.FileReadText();
	}

	if (text.empty())return;

	thisFileName = _fileName;

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
		if (thisFileName.find("Foot/") != thisFileName.find("Body/"))
		{
			for (auto&& child : model->GetAllChildlen<ChCpp::FrameObject>())
			{
				auto childObj = child.lock();
				if (childObj == nullptr)continue;
				auto frameCom = childObj->GetComponent<ChCpp::FrameComponent>();
				if (frameCom == nullptr)continue;

				childObj->UpdateAllDrawTransform();
				float test = childObj->GetDrawLHandMatrix().Transform(frameCom->minPos).y;
				if (groundHeight < test)continue;
				groundHeight = test;
			}
		}
	}

	hardness = static_cast<float>(std::atof(textObject.GetTextLine(1).c_str()));
	mass = static_cast<float>(std::atof(textObject.GetTextLine(2).c_str()));

	for (unsigned long i = 3; i < lineCount; i++)
	{
		i = CreateDatas(_base,textObject, i);
	}

}


unsigned long MechaParts::CreateDatas(BaseMecha& _base, ChCpp::TextObject& _textObject, unsigned long _linePos)
{

	std::string typeName = _textObject.GetTextLine(_linePos);
	if (typeName.empty())return _linePos + 1;
	typeName.pop_back();
	auto createFunction = createFunctions.find(typeName);
	if (createFunction == createFunctions.end())return _linePos + 1;
	auto parts = (*createFunction).second(*this);
	unsigned long linePos = parts->Deserialize(_textObject,_linePos + 1);
	return linePos;
}

void MechaParts::SetParameters(BaseMecha& _base, BaseMecha::PartsPosNames _name)
{
	SetPartsParameter(_base, _name);

	for (auto&& com : GetComponents<PartsDataBase>())
	{
		com->SetPartsParameter(_base);
	}

}

void MechaParts::SetPartsParameter(BaseMecha& _base, BaseMecha::PartsPosNames _name)
{
	if (_name == BaseMecha::PartsPosNames::None)return;

	ChPtr::Shared<MechaPartsObject> partsObject = nullptr;
	
	if (_name != BaseMecha::PartsPosNames::RWeapons && _name != BaseMecha::PartsPosNames::LWeapons)
	{
		partsObject = ChPtr::Make_S<MechaPartsObject>();
	}
	else
	{
		partsObject = ChPtr::Make_S<WeaponObject>();
	}

	partsObject->baseParts = this;

	partsObject->Init();

	if (SetPosition(_base, partsObject, _name))
	{
		this->GetMesh().SetFrameTransform(ChLMat());
	}
	
	_base.AddMass(mass);

	_base.SetGroundHeight(groundHeight);

}

ChStd::Bool MechaParts::SetPosition(BaseMecha& _base, ChPtr::Shared<MechaPartsObject> _obj, BaseMecha::PartsPosNames _name)
{

	if (_name == BaseMecha::PartsPosNames::None)return false;

	_base.AddMechaParts(_obj, _name);

	auto mechaPartsPosList = _base.GetMechaPartsPosList(_name);
	auto mechaPartsList = _base.GetMechaPartsList(_name);

	if (mechaPartsList.size() > mechaPartsPosList.size())return false;

	_obj->SetPositoinObject(mechaPartsPosList[mechaPartsList.size() - 1]);

	if (_name == BaseMecha::PartsPosNames::RWeapons)
	{
		auto weapon = ChPtr::SharedSafeCast<WeaponObject>(_obj);
		_base.AddRightWeappon(weapon);
	}
	
	if (_name == BaseMecha::PartsPosNames::LWeapons)
	{
		auto weapon = ChPtr::SharedSafeCast<WeaponObject>(_obj);
		_base.AddLeftWeappon(weapon);
	}

	return true;
}

std::string MechaParts::Save(const std::string& _fileName)
{
	std::string res = Serialize();

	{
		ChCpp::File<> file;
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
	
	//モデルの硬さ//
	res += std::to_string(hardness) + "\n";

	//モデルの重さ//
	res += std::to_string(mass) + "\n";

	for (auto&& com : GetComponents<PartsDataBase>())
	{
		res += com->GetPartsTypeTag() + "{\n";
		res += com->Serialize();
		res += "}\n";
	}

	return res;
}

unsigned long EnelgyTankData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	maxEnelgy = std::atol(_text.GetTextLine(_textPos).c_str());
	createEnelgy = std::atol(_text.GetTextLine(_textPos + 1).c_str());

	return _textPos + 2;
}

std::string EnelgyTankData::Serialize()
{
	std::string res = "";

	res += std::to_string(maxEnelgy) + "\n";
	res += std::to_string(createEnelgy) + "\n";

	return res;
}

void EnelgyTankData::SetPartsParameter(BaseMecha& _base)
{
	_base.SetMaxEnelgy(maxEnelgy);
	_base.SetChargeEnelgy(createEnelgy);
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

void CameraData::SetPartsParameter(BaseMecha& _base)
{
	auto camera = ChPtr::Make_S<CameraObject>();

	_base.AddCamera(camera);
}

unsigned long ScopeData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	unsigned long textPos = CameraData::Deserialize(_text,_textPos);
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

void ScopeData::SetPartsParameter(BaseMecha& _base)
{
	auto camera = ChPtr::Make_S<CameraObject>();

	_base.AddCamera(camera);
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

void WalkData::SetPartsParameter(BaseMecha& _base)
{
	auto&& walk = GetComponent<MoveComponent>(_base);

	walk->SetMovePow(movePow);
	walk->SetRotatePow(rotatePow);
	walk->SetJumpPow(jumpPow);
}

unsigned long WeaponData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	weatTime = std::atol(_text.GetTextLine(_textPos).c_str());
	return _textPos + 1;
}

std::string WeaponData::Serialize()
{
	std::string res = "";

	res += std::to_string(weatTime) + "\n";

	return res;
}

unsigned long SwordData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	unsigned long textPos = WeaponData::Deserialize(_text,_textPos);
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

void SwordData::SetPartsParameter(BaseMecha& _base)
{

}

unsigned long GunData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	unsigned long textPos = WeaponData::Deserialize(_text, _textPos);
	fireNum = std::atol(_text.GetTextLine(textPos).c_str());
	bulletNum = std::atol(_text.GetTextLine(textPos + 1).c_str());
	magazineNum = std::atol(_text.GetTextLine(textPos + 2).c_str());
	reloadTime = std::atol(_text.GetTextLine(textPos + 3).c_str());
	bulletFile = _text.GetTextLine(textPos + 4).c_str();
	
	return textPos + 5;

}

std::string GunData::Serialize()
{
	std::string res = "";

	res = WeaponData::Serialize();
	res += std::to_string(fireNum) + "\n";
	res += std::to_string(bulletNum) + "\n";
	res += std::to_string(magazineNum) + "\n";
	res += std::to_string(reloadTime) + "\n";
	res += bulletFile + "\n";

	return res;
}

void GunData::SetPartsParameter(BaseMecha& _base)
{

}

void NextPosBase::SetPartsParameter(BaseMecha& _base)
{
	auto target = LookObj<MechaParts>();

	if (ChPtr::NullCheck(target))return;

	auto&& posObjects = target->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>(nextPosName);

	if (posObjects.empty())return;

	ChPtr::Shared<ChCpp::FrameObject> posObject = posObjects[0].lock();

	SetObjectPos(_base, posObject);
}

void RightArmPos::SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	_base.AddMechaPartsPos(_targetObject, BaseMecha::PartsPosNames::RArm);
}

void LeftArmPos::SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	_base.AddMechaPartsPos(_targetObject, BaseMecha::PartsPosNames::LArm);
}

void FootPos::SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	_base.AddMechaPartsPos(_targetObject, BaseMecha::PartsPosNames::Foot);
}

void HeadPos::SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	_base.AddMechaPartsPos(_targetObject, BaseMecha::PartsPosNames::Head);
}

void BoostPos::SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	_base.AddMechaPartsPos(_targetObject, BaseMecha::PartsPosNames::Boost);
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

void BoostBrust::SetPartsParameter(BaseMecha& _base)
{
	auto&& base = *LookObj<MechaParts>();
	if (ChPtr::NullCheck(&base))return;

	auto&& mesh = base.GetMesh();

	auto&& boostList = mesh.GetAllChildlenForName<ChCpp::FrameObject>(objectName);

	if (boostList.empty())return;

	auto obj = boostList[0].lock();

	SetPartsObject(_base, obj);
}

void RightBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<RightBoostComponent>(_base);
	
	com->AddBoostPow(boostPower);
	com->AddBoostUseEnelgy(useEnelgy);
	com->AddBoostAvoidPow(avoidPow);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy);
	com->SetBoostAvoidWait(avoidWait);

	com->AddBoost(_boostObject);
}

void LeftBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<LeftBoostComponent>(_base);

	com->AddBoostPow(boostPower);
	com->AddBoostUseEnelgy(useEnelgy);
	com->AddBoostAvoidPow(avoidPow);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy);
	com->SetBoostAvoidWait(avoidWait);

	com->AddBoost(_boostObject);
}

void FrontBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<FrontBoostComponent>(_base);

	com->AddBoostPow(boostPower);
	com->AddBoostUseEnelgy(useEnelgy);
	com->AddBoostAvoidPow(avoidPow);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy);
	com->SetBoostAvoidWait(avoidWait);

	com->AddBoost(_boostObject);
}

void BackBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<BackBoostComponent>(_base);

	com->AddBoostPow(boostPower);
	com->AddBoostUseEnelgy(useEnelgy);
	com->AddBoostAvoidPow(avoidPow);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy);
	com->SetBoostAvoidWait(avoidWait);

	com->AddBoost(_boostObject);
}

void UpBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<UpBoostComponent>(_base);

	com->AddBoostPow(boostPower);
	com->AddBoostUseEnelgy(useEnelgy);
	com->AddBoostAvoidPow(avoidPow);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy);
	com->SetBoostAvoidWait(avoidWait);

	com->AddBoost(_boostObject);
}

void DownBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<DownBoostComponent>(_base);

	com->AddBoostPow(boostPower);
	com->AddBoostUseEnelgy(useEnelgy);
	com->AddBoostAvoidPow(avoidPow);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy);
	com->SetBoostAvoidWait(avoidWait);

	com->AddBoost(_boostObject);
}

void HaveRightWeaponPos::SetPartsParameter(BaseMecha& _base)
{

	auto target = LookObj<MechaParts>();

	if (ChPtr::NullCheck(target))return;

	auto&& posObjects = target->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>(nextPosName);

	if (posObjects.empty())return;

	ChPtr::Shared<ChCpp::FrameObject> posObject = posObjects[0].lock();

	if (posObject == nullptr)return;

	_base.AddMechaPartsPos(posObject, BaseMecha::PartsPosNames::RWeapons);

}

void HaveLeftWeaponPos::SetPartsParameter(BaseMecha& _base)
{

	auto target = LookObj<MechaParts>();

	if (ChPtr::NullCheck(target))return;

	auto&& posObjects = target->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>(nextPosName);

	if (posObjects.empty())return;

	ChPtr::Shared<ChCpp::FrameObject> posObject = posObjects[0].lock();

	if (posObject == nullptr)return;

	_base.AddMechaPartsPos(posObject, BaseMecha::PartsPosNames::LWeapons);
}

void ShotPos::SetPartsParameter(BaseMecha& _base)
{

	auto target = LookObj<MechaParts>();

	if (ChPtr::NullCheck(target))return;

	auto&& posObjects = target->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>(nextPosName);

	if (posObjects.empty())return;

	ChPtr::Shared<ChCpp::FrameObject> posObject = posObjects[0].lock();

	if (posObject == nullptr)return;

	//_base.AddMechaPartsPos(posObject, BaseMecha::PartsPosNames::LWeapons);
}

