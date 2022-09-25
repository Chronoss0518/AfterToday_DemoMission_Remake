#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../HitPosList/HitPosList.h"
#include"../Damage/Damage.h"
#include"BaseMecha.h"
#include"MechaParts.h"

void MechaParts::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);

	unsigned long lineCount = text.LineCount();

	{
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(model, text.GetTextLine(0));
	}

	hardness = std::atof(text.GetTextLine(1).c_str());
	mass = std::atof(text.GetTextLine(2).c_str());

	for (unsigned long i = 3; i < lineCount; i++)
	{

		i = CreateEnelgyTank(text, i);
		i = CreateCamera(text, i);
		i = CreateScope(text, i);
		i = CreateNormalWork(text, i);
		i = CreateBoost(text, i);
		i = CreateSword(text, i);
		i = CreateGun(text, i);
	}

}

unsigned long MechaParts::CreateEnelgyTank(ChCpp::TextObject& _textObject, unsigned long _linePos)
{

	std::string typeName = _textObject.GetTextLine(_linePos);
	if (typeName.find("EnelgyTank") == typeName.npos)return _linePos;
	unsigned long linePos = _linePos + 1;
	auto&& enelgyTank = SetComponent<EnelgyTank>();
	std::string text = "";
	for (unsigned long i = 0; i < 1; i++)
	{
		text += _textObject.GetTextLine(linePos + i) + "\n";
		linePos = _linePos + 1 + i;
	}
	enelgyTank->Deserialize(text);

	return linePos;
}

unsigned long MechaParts::CreateCamera(ChCpp::TextObject& _textObject, unsigned long _linePos)
{
	std::string typeName = _textObject.GetTextLine(_linePos);
	if (typeName.find("Camera") == typeName.npos)return _linePos;
	unsigned long linePos = _linePos + 1;
	auto&& enelgyTank = SetComponent<Camera>();
	std::string text = "";
	for (unsigned long i = 0; i < 3; i++)
	{
		text += _textObject.GetTextLine(linePos + i) + "\n";
		linePos = _linePos + 1 + i;
	}
	enelgyTank->Deserialize(text);

	return linePos;
}

unsigned long MechaParts::CreateScope(ChCpp::TextObject& _textObject, unsigned long _linePos)
{
	std::string typeName = _textObject.GetTextLine(_linePos);
	if (typeName.find("Scope") == typeName.npos)return _linePos;
	unsigned long linePos = _linePos + 1;
	auto&& enelgyTank = SetComponent<Scope>();
	std::string text = "";
	for (unsigned long i = 0; i < 3; i++)
	{
		text += _textObject.GetTextLine(linePos + i) + "\n";
		linePos = _linePos + 1 + i;
	}
	enelgyTank->Deserialize(text);

	return linePos;
}

unsigned long MechaParts::CreateNormalWork(ChCpp::TextObject& _textObject, unsigned long _linePos)
{
	std::string typeName = _textObject.GetTextLine(_linePos);
	if (typeName.find("NormalWork") == typeName.npos)return _linePos;
	unsigned long linePos = _linePos + 1;
	auto&& enelgyTank = SetComponent<NormalWork>();
	std::string text = "";
	for (unsigned long i = 0; i < 3; i++)
	{
		text += _textObject.GetTextLine(linePos + i) + "\n";
		linePos = _linePos + 1 + i;
	}
	enelgyTank->Deserialize(text);

	return linePos;
}

unsigned long MechaParts::CreateBoost(ChCpp::TextObject& _textObject, unsigned long _linePos)
{
	std::string typeName = _textObject.GetTextLine(_linePos);
	if (typeName.find("Boost") == typeName.npos)return _linePos;
	unsigned long linePos = _linePos + 1;
	auto&& enelgyTank = SetComponent<Boost>();
	std::string text = "";
	for (unsigned long i = 0; i < 4; i++)
	{
		text += _textObject.GetTextLine(linePos + i) + "\n";
		linePos = _linePos + 1 + i;
	}
	enelgyTank->Deserialize(text);

	return linePos;
}

unsigned long MechaParts::CreateSword(ChCpp::TextObject& _textObject, unsigned long _linePos)
{
	std::string typeName = _textObject.GetTextLine(_linePos);
	if (typeName.find("Sword") == typeName.npos)return _linePos;
	unsigned long linePos = _linePos + 1;
	auto&& enelgyTank = SetComponent<Sword>();
	std::string text = "";
	for (unsigned long i = 0; i < 2; i++)
	{
		text += _textObject.GetTextLine(linePos + i) + "\n";
		linePos = _linePos + 1 + i;
	}
	enelgyTank->Deserialize(text);

	return linePos;
}

unsigned long MechaParts::CreateGun(ChCpp::TextObject& _textObject, unsigned long _linePos)
{
	std::string typeName = _textObject.GetTextLine(_linePos);
	if (typeName.find("Gun") == typeName.npos)return _linePos;
	unsigned long linePos = _linePos + 1;
	auto&& enelgyTank = SetComponent<Gun>();
	std::string text = "";
	for (unsigned long i = 0; i < 2; i++)
	{
		text += _textObject.GetTextLine(linePos + i) + "\n";
		linePos = _linePos + 1 + i;
	}
	enelgyTank->Deserialize(text);

	return linePos;
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

	for (auto&& com : GetComponents<PartsFunctionBase>())
	{
		res += com->GetPartsTypeTag() + "{";
		std::string tmp = com->Serialize();
		res += tmp;
		res += "}";
	}

	return res;
}

void EnelgyTank::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	maxEnelgy = std::atol(text.GetTextLine(0).c_str());
	createEnelgy = std::atol(text.GetTextLine(1).c_str());
}

std::string EnelgyTank::Serialize()
{
	std::string res = "";

	res += std::to_string(maxEnelgy) + "\n";
	res += std::to_string(createEnelgy) + "\n";

	return res;
}

void Camera::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	fovy = std::atof(text.GetTextLine(0).c_str());
	cameraCount = std::atol(text.GetTextLine(2).c_str());
}

std::string Camera::Serialize()
{
	std::string res = "";

	res += std::to_string(fovy) + "\n";
	res += std::to_string(cameraCount) + "\n";

	return res;
}

void Scope::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	minFovy = std::atof(text.GetTextLine(0).c_str());
	maxFovy = std::atof(text.GetTextLine(1).c_str());
	fovySlideSpeed = std::atof(text.GetTextLine(2).c_str());
}

std::string Scope::Serialize()
{
	std::string res = "";
	
	res += std::to_string(minFovy) + "\n";
	res += std::to_string(maxFovy) + "\n";
	res += std::to_string(fovySlideSpeed) + "\n";

	return res;
}

void NormalWork::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	movePow = std::atof(text.GetTextLine(0).c_str());
	rotatePow = std::atof(text.GetTextLine(1).c_str());
	jumpPow = std::atof(text.GetTextLine(2).c_str());
}

std::string NormalWork::Serialize()
{
	std::string res = "";

	res += std::to_string(movePow) + "\n";
	res += std::to_string(rotatePow) + "\n";
	res += std::to_string(jumpPow) + "\n";

	return res;
}

void Boost::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	useEnelgy = std::atol(text.GetTextLine(0).c_str());
	boostPower = std::atof(text.GetTextLine(1).c_str());
	avoidPow = std::atof(text.GetTextLine(2).c_str());
	avoidUseEnelgy = std::atol(text.GetTextLine(3).c_str());
}

std::string Boost::Serialize()
{
	std::string res = "";

	res += std::to_string(useEnelgy) + "\n";
	res += std::to_string(boostPower) + "\n";
	res += std::to_string(avoidPow) + "\n";
	res += std::to_string(avoidUseEnelgy) + "\n";

	return res;
}

void Weapon::Attack()
{
	if (weatCount > 0 || attackFlg)return;
	attackFlg = true;
}

void Sword::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	weatTime = std::atol(text.GetTextLine(0).c_str());
	attackTime = std::atol(text.GetTextLine(1).c_str());
}

std::string Sword::Serialize()
{
	std::string res = "";

	res += std::to_string(weatTime) + "\n";
	res += std::to_string(attackTime) + "\n";

	return res;
}

void Sword::Update()
{

}

void Sword::Move()
{

}

void Gun::Deserialize(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
	weatTime = std::atol(text.GetTextLine(0).c_str());
	fireNum = std::atol(text.GetTextLine(1).c_str());
	bulletNum = std::atol(text.GetTextLine(2).c_str());
	magazineNum = std::atol(text.GetTextLine(3).c_str());
	reloadTime = std::atol(text.GetTextLine(4).c_str());

	{
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(bullet, text.GetTextLine(5));
	}

}

std::string Gun::Serialize()
{
	std::string res = "";

	res += std::to_string(weatTime) + "\n";
	res += std::to_string(fireNum) + "\n";
	res += std::to_string(bulletNum) + "\n";
	res += std::to_string(magazineNum) + "\n";
	res += std::to_string(reloadTime) + "\n";
	res += bullet->GetModelName() + "\n";

	return res;
}

void Gun::Update()
{

}

void Gun::Move()
{

}
