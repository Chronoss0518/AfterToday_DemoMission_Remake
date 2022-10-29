#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"Bullet.h"
#include"../BaseMecha/WeaponObject.h"


ChPtr::Shared<BulletData>(*BulletData::CreateBulletFunction[4])()
= {
	[]()->ChPtr::Shared<BulletData> {return ChPtr::Make_S<BulletData>(); },
	[]()->ChPtr::Shared<BulletData> {return ChPtr::Make_S<BoostBulletData>(); },
	[]()->ChPtr::Shared<BulletData> {return ChPtr::Make_S<HighExplosiveBulletData>(); },
	[]()->ChPtr::Shared<BulletData> {return ChPtr::Make_S<MissileData>(); }
};

void BulletData::CreateBullet(const std::string& _fileName)
{
	auto&& bulletList = LoadBulletList();

	{
		auto bulletIns = bulletList.find(_fileName);
		if (bulletIns != bulletList.end())
		{
			return;
		}
	}

	std::string text = "";
	ChCpp::TextObject textObject;

	{
		ChCpp::File<> file;
		file.FileOpen(_fileName);
		text = file.FileReadText();
		textObject.SetText(text);
		file.FileClose();
	}
	
	if (textObject.LineCount() <= 2)return;

	unsigned char bulletType = std::atoi(textObject.GetTextLine(0).c_str());

	auto bullet = CreateBulletFunction[bulletType]();

	{
		unsigned long pos = text.find_first_of("\n") + 1;
		text = text.substr(pos);
	}

	bullet->Deserialize(text);

	bulletList[_fileName] = bullet;

	return;

}

void BulletData::Deserialize(const std::string& _text)
{
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	firstSpeed = static_cast<float>(std::atof(textObject.GetTextLine(0).c_str()));
	penetration = static_cast<float>(std::atof(textObject.GetTextLine(1).c_str()));

	{
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(bullet, textObject.GetTextLine(2));
	}

}

std::string BulletData::Serialize()
{
	std::string res = GetBulletType();
	
	res += std::to_string(firstSpeed) + "\n";
	res += std::to_string(penetration) + "\n";
	res += bullet->GetModelName() + "\n";

	return res;
}

void BoostBulletData::Deserialize(const std::string& _text)
{
	BulletData::Deserialize(_text);
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	startBoostTime = std::atol(textObject.GetTextLine(3).c_str());
	penetration = static_cast<float>(std::atof(textObject.GetTextLine(4).c_str()));

}

std::string BoostBulletData::Serialize()
{
	std::string res = BulletData::Serialize();

	res += std::to_string(startBoostTime) + "\n";
	res += std::to_string(boostPow) + "\n";

	return res;
}

void HighExplosiveBulletData::Deserialize(const std::string& _text)
{
	BoostBulletData::Deserialize(_text);
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	blastRange = static_cast<float>(std::atof(textObject.GetTextLine(5).c_str()));

}

std::string HighExplosiveBulletData::Serialize()
{
	std::string res = BoostBulletData::Serialize();

	res += std::to_string(blastRange) + "\n";

	return res;
}

void MissileData::Deserialize(const std::string& _text)
{
	HighExplosiveBulletData::Deserialize(_text);
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	rotateSpeed = static_cast<float>(std::atof(textObject.GetTextLine(6).c_str()));
	lostRange = static_cast<float>(std::atof(textObject.GetTextLine(7).c_str()));

}

std::string MissileData::Serialize()
{
	std::string res = HighExplosiveBulletData::Serialize();

	res += std::to_string(rotateSpeed) + "\n";
	res += std::to_string(lostRange) + "\n";

	return res;
}
