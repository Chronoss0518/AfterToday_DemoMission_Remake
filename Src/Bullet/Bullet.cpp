#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Frames/GameFrame.h"
#include"Bullet.h"
#include"BulletObject.h"

ChPtr::Shared<BulletData>(*BulletData::CreateBulletFunction[4])()
= {
	[]()->ChPtr::Shared<BulletData> {return ChPtr::Make_S<BulletData>(); },
	[]()->ChPtr::Shared<BulletData> {return ChPtr::Make_S<BoostBulletData>(); },
	[]()->ChPtr::Shared<BulletData> {return ChPtr::Make_S<HighExplosiveBulletData>(); },
	[]()->ChPtr::Shared<BulletData> {return ChPtr::Make_S<MissileData>(); }
};

ChPtr::Shared<BulletData> BulletData::CreateBullet(MeshDrawer* _drawer, ID3D11Device* _device, const std::string& _fileName)
{
	auto&& bulletList = LoadBulletList();

	{
		auto bulletIns = bulletList.find(_fileName);
		if (bulletIns != bulletList.end())
		{
			return (*bulletIns).second;
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
	
	if (textObject.LineCount() <= 2)return nullptr;

	unsigned char bulletType = std::atoi(textObject.GetTextLine(0).c_str());

	auto bullet = CreateBulletFunction[bulletType]();

	bullet->SetMeshDrawer(_drawer);
	
	{
		unsigned long pos = text.find_first_of("\n") + 1;
		text = text.substr(pos);
	}

	bullet->Deserialize(_device,text);

	bulletList[_fileName] = bullet;
	
	bullet->SetMeshDrawer(_drawer);

	return bullet;

}

void BulletData::AllRelease()
{
	LoadBulletList().clear();
}

void BulletData::Deserialize(ID3D11Device* _device, const std::string& _text)
{
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	firstSpeed = static_cast<float>(std::atof(textObject.GetTextLine(0).c_str()));
	penetration = static_cast<unsigned long>(std::atoll(textObject.GetTextLine(1).c_str()));
	hitSize = static_cast<float>(std::atof(textObject.GetTextLine(2).c_str()));


	{
		ChCpp::ModelLoader::XFile loader;
		bullet->Init(_device);
		loader.CreateModel(bullet, textObject.GetTextLine(3));

	}

}

std::string BulletData::Serialize()
{
	std::string res = GetBulletType();
	
	res += std::to_string(firstSpeed) + "\n";
	res += std::to_string(penetration) + "\n";
	res += std::to_string(hitSize) + "\n";
	res += bullet->GetModelName() + "\n";

	return res;
}

void BulletData::InitBulletObject(const ChLMat& _startMat,BulletObject& _bullet)
{
	ChLMat mat = (bullet->GetDrawLHandMatrix() * _startMat);

	ChVec3 dir = mat.GetZAxisDirection();

	_bullet.SetPosition(_startMat.GetPosition());
	_bullet.SetRotation(mat.GetRotation());

	dir *= firstSpeed;
	dir.x = -dir.x;
	_bullet.physics->AddMovePowerVector(dir);

	_bullet.collider.SetScalling(hitSize);
}

void BulletData::UpdateBulletObject(BulletObject& _bullet)
{
	if (_bullet.hitFlg)
	{
		_bullet.Destroy();
		return;
	}

	_bullet.physics->Update();

	if (_bullet.physics->IsOutSide())
	{
		_bullet.Destroy();
		return;
	}

	if (_bullet.physics->IsGround() || _bullet.physics->IsWall())
	{
		_bullet.Destroy();
		return;
	}

}

void BulletData::MoveBulletObject(BulletObject& _bullet)
{
	_bullet.physics->SetPosition(_bullet.physics->GetPosition() + _bullet.physics->GetAddMovePowerVector());
	_bullet.physics->SetRotation(_bullet.physics->GetRotation() + _bullet.physics->GetAddRotatePowerVector());
}

void BulletData::Draw(const ChMat_11& _mat)
{
	if (bullet == nullptr)return;
	drawer->drawer.Draw(drawer->dc, *bullet, _mat);
}

void BoostBulletData::Deserialize(ID3D11Device* _device, const std::string& _text)
{
	BulletData::Deserialize(_device,_text);
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	startBoostTime = std::atol(textObject.GetTextLine(4).c_str());
	boostPow = static_cast<float>(std::atof(textObject.GetTextLine(5).c_str()));

}

std::string BoostBulletData::Serialize()
{
	std::string res = BulletData::Serialize();

	res += std::to_string(startBoostTime) + "\n";
	res += std::to_string(boostPow) + "\n";

	return res;
}

void BoostBulletData::UpdateBulletObject(BulletObject& _bullet)
{

}

void HighExplosiveBulletData::Deserialize(ID3D11Device* _device, const std::string& _text)
{
	BoostBulletData::Deserialize(_device,_text);
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	blastRange = static_cast<float>(std::atof(textObject.GetTextLine(6).c_str()));

}

std::string HighExplosiveBulletData::Serialize()
{
	std::string res = BoostBulletData::Serialize();

	res += std::to_string(blastRange) + "\n";

	return res;
}

void HighExplosiveBulletData::UpdateBulletObject(BulletObject& _bullet)
{

}

void MissileData::Deserialize(ID3D11Device* _device, const std::string& _text)
{
	HighExplosiveBulletData::Deserialize(_device,_text);
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	rotateSpeed = static_cast<float>(std::atof(textObject.GetTextLine(7).c_str()));
	lostRange = static_cast<float>(std::atof(textObject.GetTextLine(8).c_str()));

}

std::string MissileData::Serialize()
{
	std::string res = HighExplosiveBulletData::Serialize();

	res += std::to_string(rotateSpeed) + "\n";
	res += std::to_string(lostRange) + "\n";

	return res;
}

void MissileData::UpdateBulletObject(BulletObject& _bullet)
{

}
