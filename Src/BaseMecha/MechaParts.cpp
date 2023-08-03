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

std::map<std::string, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>MechaParts::createFunctions
{
	{"EnelgyTank:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<EnelgyTankData>(); }},
	{"Camera:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<CameraData>(); }},
	{"Scope:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<ScopeData>(); }},
	{"Walk:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<WalkData>(); }},
	{"Aerodynamics:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<Aerodynamics>(); }},
	{"MoveAcceleration:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<MoveAcceleration>(); }},
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
	{"WeaponPos:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<WeaponPos>(); }},
	{"Sword:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<SwordData>(); }},
	{"Gun:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<GunData>(); }},
};

ChPtr::Shared<MechaPartsObject> MechaParts::LoadParts(BaseMecha& _base, ID3D11Device* _device, MeshDrawer* _drawer, GameFrame* _frame, const std::string& _fileName)
{

	auto&& loadPartss = LoadPartsList();
	auto it = loadPartss.find(_fileName);
	if (it != loadPartss.end())
	{
		return (*it).second->SetParameters(_base, _frame);
	}

	auto mechaParts = ChPtr::Make_S<MechaParts>();
	mechaParts->Load(_base, _device, _fileName);

	if (mechaParts->GetThisFileName().empty())return nullptr;

	loadPartss[_fileName] = mechaParts;
	mechaParts->SetMeshDrawer(_drawer);
	return mechaParts->SetParameters(_base,_frame);
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
		defaultFrameMat = model->GetFrameTransformLMat();
		if (thisFileName.find("Foot/") != thisFileName.find("Body/"))
		{
			auto test = model->GetInitAllFrameMinPos().y;
			if (groundHeight > test)
			{
				groundHeight = test;
			}
		}
	}

	hardness = static_cast<unsigned long>(std::atoll(textObject.GetTextLine(1).c_str()));
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

ChPtr::Shared<MechaPartsObject>  MechaParts::SetParameters(BaseMecha& _base, GameFrame* _frame)
{
	auto parts = SetPartsParameter(_base);
	parts->SetFrame(_frame);
	for (auto&& com : GetComponents<PartsDataBase>())
	{
		com->SetPartsParameter(_base,*parts,_frame);
	}

	return parts;
}

ChPtr::Shared<MechaPartsObject>  MechaParts::SetPartsParameter(BaseMecha& _base)
{
	auto partsObject = ChPtr::Make_S<MechaPartsObject>();;

	partsObject->baseParts = this;
	
	_base.AddMass(mass);

	_base.SetGroundHeight(groundHeight);

	return partsObject;
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
	
	//パーツの硬さ//
	res += std::to_string(hardness) + "\n";

	//パーツの重さ//
	res += std::to_string(mass) + "\n";

	for (auto&& com : GetComponents<PartsDataBase>())
	{
		res += com->GetPartsTypeTag() + "{\n";
		res += com->Serialize();
		res += "}\n";
	}

	return res;
}

void MechaParts::Draw(const ChMat_11& _mat)
{
	if (model == nullptr)return;
	drawer->drawer.Draw(drawer->dc, *model, _mat);
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

void EnelgyTankData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	_base.AddMaxEnelgy(maxEnelgy);
	_base.AddChargeEnelgy(createEnelgy);
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

void ScopeData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	//auto camera = ChPtr::Make_S<CameraObject>();

	//_base.AddCamera(camera);
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

void RightArmPos::SetObjectPos(BaseMecha& _base,  MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{

	auto obj = ChPtr::Make_S<PositionObject>();
	obj->haveObject = &_parts;
	obj->positionObject = _targetObject;
	_base.AddMechaPartsPos(obj, BaseMecha::PartsPosNames::RArm);
}

void LeftArmPos::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	auto obj = ChPtr::Make_S<PositionObject>();
	obj->haveObject = &_parts;
	obj->positionObject = _targetObject;
	_base.AddMechaPartsPos(obj, BaseMecha::PartsPosNames::LArm);
}

void FootPos::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	auto obj = ChPtr::Make_S<PositionObject>();
	obj->haveObject = &_parts;
	obj->positionObject = _targetObject;
	_base.AddMechaPartsPos(obj, BaseMecha::PartsPosNames::Foot);
}

void HeadPos::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	auto obj = ChPtr::Make_S<PositionObject>();
	obj->haveObject = &_parts;
	obj->positionObject = _targetObject;
	_base.AddMechaPartsPos(obj, BaseMecha::PartsPosNames::Head);
}

void BoostPos::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	auto obj = ChPtr::Make_S<PositionObject>();
	obj->haveObject = &_parts;
	obj->positionObject = _targetObject;
	_base.AddMechaPartsPos(obj, BaseMecha::PartsPosNames::Boost);
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
	auto com = GetComponent<BoostComponent>(_base);
	
	com->AddBoostPow(boostPower, BaseMecha::InputName::RightBoost);
	com->AddBoostUseEnelgy(useEnelgy, BaseMecha::InputName::RightBoost);
	com->AddBoostAvoidPow(avoidPow, BaseMecha::InputName::RightAvo);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy, BaseMecha::InputName::RightAvo);
	com->SetBoostAvoidWait(avoidWait, BaseMecha::InputName::RightAvo);

	com->AddBoostWhereBoostName(_boostObject, BaseMecha::InputName::RightBoost);
}

void LeftBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<BoostComponent>(_base);

	com->AddBoostPow(boostPower, BaseMecha::InputName::LeftBoost);
	com->AddBoostUseEnelgy(useEnelgy, BaseMecha::InputName::LeftBoost);
	com->AddBoostAvoidPow(avoidPow, BaseMecha::InputName::LeftAvo);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy, BaseMecha::InputName::LeftAvo);
	com->SetBoostAvoidWait(avoidWait, BaseMecha::InputName::LeftAvo);

	com->AddBoostWhereBoostName(_boostObject, BaseMecha::InputName::LeftBoost);
}

void FrontBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<BoostComponent>(_base);

	com->AddBoostPow(boostPower, BaseMecha::InputName::FrontBoost);
	com->AddBoostUseEnelgy(useEnelgy, BaseMecha::InputName::FrontBoost);
	com->AddBoostAvoidPow(avoidPow, BaseMecha::InputName::FrontAvo);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy, BaseMecha::InputName::FrontAvo);
	com->SetBoostAvoidWait(avoidWait, BaseMecha::InputName::FrontAvo);

	com->AddBoostWhereBoostName(_boostObject, BaseMecha::InputName::FrontBoost);
}

void BackBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<BoostComponent>(_base);

	com->AddBoostPow(boostPower, BaseMecha::InputName::BackBoost);
	com->AddBoostUseEnelgy(useEnelgy, BaseMecha::InputName::BackBoost);
	com->AddBoostAvoidPow(avoidPow, BaseMecha::InputName::BackAvo);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy, BaseMecha::InputName::BackAvo);
	com->SetBoostAvoidWait(avoidWait, BaseMecha::InputName::BackAvo);

	com->AddBoostWhereBoostName(_boostObject, BaseMecha::InputName::BackBoost);
}

void UpBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<BoostComponent>(_base);

	com->AddBoostPow(boostPower, BaseMecha::InputName::UpBoost);
	com->AddBoostUseEnelgy(useEnelgy, BaseMecha::InputName::UpBoost);
	com->AddBoostAvoidPow(avoidPow, BaseMecha::InputName::UpAvo);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy, BaseMecha::InputName::UpAvo);
	com->SetBoostAvoidWait(avoidWait, BaseMecha::InputName::UpAvo);

	com->AddBoostWhereBoostName(_boostObject, BaseMecha::InputName::UpBoost);
}

void DownBoostBrust::SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)
{
	auto com = GetComponent<BoostComponent>(_base);

	com->AddBoostPow(boostPower, BaseMecha::InputName::DownBoost);
	com->AddBoostUseEnelgy(useEnelgy, BaseMecha::InputName::DownBoost);
	com->AddBoostAvoidPow(avoidPow, BaseMecha::InputName::DownAvo);
	com->AddBoostAvoidUseEnelgy(avoidUseEnelgy, BaseMecha::InputName::DownAvo);
	com->SetBoostAvoidWait(avoidWait, BaseMecha::InputName::DownAvo);

	com->AddBoostWhereBoostName(_boostObject, BaseMecha::InputName::DownBoost);
}

void WeaponPos::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	auto obj = ChPtr::Make_S<PositionObject>();
	obj->haveObject = &_parts;
	obj->positionObject = _targetObject;
	_base.AddMechaPartsPos(obj, BaseMecha::PartsPosNames::Weapons);
}

unsigned long WeaponData::Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos)
{
	unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
	seFile =_text.GetTextLine(textPos);
	weatTime = std::atol(_text.GetTextLine(textPos + 1).c_str());
	return textPos + 2;
}

std::string WeaponData::Serialize()
{
	std::string res = NextPosBase::Serialize();

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

	function->Init(thisParts.GetMeshDrawer(),ChD3D11::D3D11Device());

	function->SetFrmae(_frame);

	_parts.AddWeaponFunction(function);

	NextPosBase::SetPartsParameter(_base, _parts,_frame);
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

	NextPosBase::SetPartsParameter(_base, _parts,_frame);
}
