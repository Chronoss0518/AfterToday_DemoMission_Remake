#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Bullet/Bullet.h"
#include"BaseMecha.h"
#include"MechaParts.h"

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY("RobotParts/" current_path)
#endif

std::map<std::string, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>MechaParts::createFunctions
{
	{"EnelgyTank:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<EnelgyTankData>(); }},
	{"Camera:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<CameraData>(); }},
	{"Scope:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<ScopeData>(); }},
	{"NormalWork:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<NormalWorkData>(); }},
	{"Boost:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<BoostData>(); }},
	{"Sword:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<SwordData>(); }},
	{"Gun:",[](MechaParts& _this)->ChPtr::Shared<PartsDataBase> {return _this.SetComponent<GunData>(); }}
};

void MechaParts::Load(BaseMecha& _base, ID3D11Device* _device, const std::string& _fileName)
{
	auto&& loadPartss = LoadPartsList();
	if (loadPartss.find(_fileName) != loadPartss.end())
	{
		SetParameters(_base);
		return;
	}


	std::string text = "";

	{
		ChCpp::File<> file;
		file.FileOpen(_fileName);
		text = file.FileReadText();
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
		char test[1024];
		GetCurrentDirectory(1024, test);
		OutputDebugString(test);
		std::string tmp = MESH_DIRECTORY("RobotsParts/" + textObject.GetTextLine(0));
		//tmp = "/" + tmp;
		loader.CreateModel(model, tmp);
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
	unsigned long linePos = _linePos + 1;
	std::string text = "";
	unsigned long outCount = 0;
	for (unsigned long i = 0; i < parts->GetPartsLineCount(); i++)
	{
		text += _textObject.GetTextLine(linePos + i) + "\n";
		outCount = i;
	}
	linePos = linePos + outCount + 1;
	parts->Deserialize(text);
	parts->SetPartsParameter(_base);
	return linePos;
}

void MechaParts::SetParameters(BaseMecha& _base)
{
	for (auto&& com : GetComponents<PartsDataBase>())
	{
		com->SetPartsParameter(_base);
	}
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

void EnelgyTankData::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	maxEnelgy = std::atol(text.GetTextLine(0).c_str());
	createEnelgy = std::atol(text.GetTextLine(1).c_str());
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
}

void CameraData::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	fovy = static_cast<float>(std::atof(text.GetTextLine(0).c_str()));
	cameraCount = std::atol(text.GetTextLine(1).c_str());
	cameraObject = text.GetTextLine(2);
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

}

void ScopeData::Deserialize(const std::string& _text)
{
	CameraData::Deserialize(_text);
	ChCpp::TextObject text;
	text.SetText(_text);
	minFovy = static_cast<float>(std::atof(text.GetTextLine(CameraData::GetPartsLineCount()).c_str()));
	maxFovy = static_cast<float>(std::atof(text.GetTextLine(CameraData::GetPartsLineCount() + 1).c_str()));
	fovySlideSpeed = static_cast<float>(std::atof(text.GetTextLine(CameraData::GetPartsLineCount() + 2).c_str()));
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

}

void NormalWorkData::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	movePow = static_cast<float>(std::atof(text.GetTextLine(0).c_str()));
	rotatePow = static_cast<float>(std::atof(text.GetTextLine(1).c_str()));
	jumpPow = static_cast<float>(std::atof(text.GetTextLine(2).c_str()));
}

std::string NormalWorkData::Serialize()
{
	std::string res = "";

	res += std::to_string(movePow) + "\n";
	res += std::to_string(rotatePow) + "\n";
	res += std::to_string(jumpPow) + "\n";

	return res;
}

void NormalWorkData::SetPartsParameter(BaseMecha& _base)
{

}

void BoostData::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	useEnelgy = std::atol(text.GetTextLine(0).c_str());
	boostPower = static_cast<float>(std::atof(text.GetTextLine(1).c_str()));
	avoidUseEnelgy = std::atol(text.GetTextLine(2).c_str());
	avoidPow = static_cast<float>(std::atof(text.GetTextLine(3).c_str()));
}

std::string BoostData::Serialize()
{
	std::string res = "";

	res += std::to_string(useEnelgy) + "\n";
	res += std::to_string(boostPower) + "\n";
	res += std::to_string(avoidUseEnelgy) + "\n";
	res += std::to_string(avoidPow) + "\n";

	return res;
}

void BoostData::SetPartsParameter(BaseMecha& _base)
{

}

void WeaponData::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	weatTime = std::atol(text.GetTextLine(0).c_str());
}

std::string WeaponData::Serialize()
{
	std::string res = "";

	res += std::to_string(weatTime) + "\n";

	return res;
}

void SwordData::Deserialize(const std::string& _text)
{
	WeaponData::Deserialize(_text);
	ChCpp::TextObject text;
	text.SetText(_text);
	attackTime = std::atol(text.GetTextLine(WeaponData::GetPartsLineCount()).c_str());
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

void GunData::Deserialize(const std::string& _text)
{
	WeaponData::Deserialize(_text);
	ChCpp::TextObject text;
	text.SetText(_text);
	fireNum = std::atol(text.GetTextLine(WeaponData::GetPartsLineCount()).c_str());
	bulletNum = std::atol(text.GetTextLine(WeaponData::GetPartsLineCount() + 1).c_str());
	magazineNum = std::atol(text.GetTextLine(WeaponData::GetPartsLineCount() + 2).c_str());
	reloadTime = std::atol(text.GetTextLine(WeaponData::GetPartsLineCount() + 3).c_str());
	bulletFile = text.GetTextLine(WeaponData::GetPartsLineCount() + 4).c_str();
	
	return;

	unsigned long bulletType = std::atol(text.GetTextLine(5).c_str());

	std::string bulletData = "";

	for (unsigned long i = 6; i < text.Count(); i++)
	{
		if (i > 6)bulletData += "\n";
		bulletData += text.GetTextLine(i);
	}

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
