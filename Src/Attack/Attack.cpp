#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Frames/GameFrame.h"
#include"Attack.h"
#include"AttackObject.h"

ChPtr::Shared<Attack> Attack::CreateBullet(MeshDrawer* _drawer, ID3D11Device* _device, const std::string& _fileName)
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

	auto bullet = ChPtr::Make_S<Attack>();

	bullet->SetMeshDrawer(_drawer);

	bullet->Deserialize(_device,text);

	bulletList[_fileName] = bullet;
	
	bullet->SetMeshDrawer(_drawer);

	return bullet;

}

void Attack::AllRelease()
{
	LoadBulletList().clear();
}

void Attack::Deserialize(ID3D11Device* _device, const std::string& _text)
{
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	bulletType = std::atoi(textObject.GetTextLine(0).c_str());;
	firstSpeed = static_cast<float>(std::atof(textObject.GetTextLine(1).c_str()));
	penetration = static_cast<unsigned long>(std::atoll(textObject.GetTextLine(2).c_str()));
	hitSize = static_cast<float>(std::atof(textObject.GetTextLine(3).c_str()));


	{
		ChCpp::ModelLoader::XFile loader;
		bullet->Init(_device);
		loader.CreateModel(bullet, textObject.GetTextLine(4));
	}

	CreateBulletData();
	CrateBoostBulletData();
	CreateHighExplosiveBulletData();
	CrateMissileData();

	unsigned long pos = 5;

	for (auto&& bulletFunction : externulFunction)
	{

		if (bulletFunction == nullptr)continue;
		pos = bulletFunction->Deserialize(_device, textObject , pos);
	}

}

std::string Attack::Serialize()
{
	std::string res = std::to_string(bulletType);
	
	res += std::to_string(firstSpeed) + "\n";
	res += std::to_string(penetration) + "\n";
	res += std::to_string(hitSize) + "\n";
	res += bullet->GetModelName() + "\n";

	for (auto&& bulletFunction : externulFunction)
	{
		if (bulletFunction == nullptr)continue;
		res += bulletFunction->Serialize();
	}

	return res;
}

void Attack::CreateBulletData()
{	
	if(!(bulletType & (1 << ChStd::EnumCast(AttackType::Bullet))))return;
	externulFunction[ChStd::EnumCast(AttackType::Bullet)] = ChPtr::Make_S<BulletData>();
}

void Attack::CrateBoostBulletData()
{
	if (!(bulletType & (1 << ChStd::EnumCast(AttackType::BoostBullet))))return;
	externulFunction[ChStd::EnumCast(AttackType::BoostBullet)] = ChPtr::Make_S<BoostBulletData>();
}

void Attack::CreateHighExplosiveBulletData()
{
	if (!(bulletType & (1 << ChStd::EnumCast(AttackType::HighExplosive))))return;
	externulFunction[ChStd::EnumCast(AttackType::HighExplosive)] = ChPtr::Make_S<HighExplosiveBulletData>();
}

void Attack::CrateMissileData()
{
	if (!(bulletType & (1 << ChStd::EnumCast(AttackType::Missile))))return;
	externulFunction[ChStd::EnumCast(AttackType::Missile)] = ChPtr::Make_S<MissileData>();
}

void Attack::InitBulletObject(const ChLMat& _startMat,AttackObject& _bullet)
{
	ChLMat mat = (bullet->GetDrawLHandMatrix() * _startMat);

	ChVec3 dir = mat.GetZAxisDirection();

	dir.Normalize();

	_bullet.SetPosition(_startMat.GetPosition());
	_bullet.SetRotation(mat.GetRotation());

	dir *= firstSpeed;
	dir.x = -dir.x;
	_bullet.physics->AddMovePowerVector(dir);

	_bullet.collider.SetScalling(hitSize);
}

void Attack::UpdateBulletObject(AttackObject& _bullet)
{
	for (auto&& bulletFunction : externulFunction)
	{
		if (bulletFunction == nullptr)continue;
		bulletFunction->UpdateBulletObject(_bullet);
	}

}

void Attack::MoveBulletObject(AttackObject& _bullet)
{
	_bullet.physics->SetPosition(_bullet.physics->GetPosition() + _bullet.physics->GetAddMovePowerVector());
	_bullet.physics->SetRotation(_bullet.physics->GetRotation() + _bullet.physics->GetAddRotatePowerVector());
}

void Attack::Draw(const ChMat_11& _mat)
{
	if (bullet == nullptr)return;
	drawer->drawer.Draw(drawer->dc, *bullet, _mat);
}

unsigned long BulletData::Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos)
{
	return _nowPos;
}

std::string BulletData::Serialize()
{
	return "";
}

void BulletData::UpdateBulletObject(AttackObject& _bullet)
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

unsigned long BoostBulletData::Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos)
{

	startBoostTime = std::atol(_text.GetTextLine(_nowPos).c_str());
	boostPow = static_cast<float>(std::atof(_text.GetTextLine(_nowPos + 1).c_str()));
	return _nowPos + 2;

}

std::string BoostBulletData::Serialize()
{
	std::string res = "";

	res += std::to_string(startBoostTime) + "\n";
	res += std::to_string(boostPow) + "\n";

	return res;
}

void BoostBulletData::UpdateBulletObject(AttackObject& _bullet)
{

}

unsigned long HighExplosiveBulletData::Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos)
{

	blastRange = static_cast<float>(std::atof(_text.GetTextLine(_nowPos).c_str()));

	return _nowPos + 1;
}

std::string HighExplosiveBulletData::Serialize()
{
	std::string res = "";

	res += std::to_string(blastRange) + "\n";

	return res;
}

void HighExplosiveBulletData::UpdateBulletObject(AttackObject& _bullet)
{

}

unsigned long MissileData::Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos)
{
	rotateSpeed = static_cast<float>(std::atof(_text.GetTextLine(_nowPos).c_str()));
	lostRange = static_cast<float>(std::atof(_text.GetTextLine(_nowPos + 1).c_str()));

	return _nowPos + 2;
}

std::string MissileData::Serialize()
{
	std::string res = "";

	res += std::to_string(rotateSpeed) + "\n";
	res += std::to_string(lostRange) + "\n";

	return res;
}

void MissileData::UpdateBulletObject(AttackObject& _bullet)
{

}