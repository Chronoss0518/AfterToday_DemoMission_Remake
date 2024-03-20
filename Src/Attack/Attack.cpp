#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Frames/GameFrame.h"
#include"Attack.h"
#include"AttackObject.h"


#define CREATE_TYPE(_enum,_class) {ChStd::EnumCast(_enum),[]()->ChPtr::Shared<Attack::AttackBase>{return ChPtr::Make_S<_class>();}}

std::map<unsigned char, std::function<ChPtr::Shared<Attack::AttackBase>()>> CreateAttackType
{
	CREATE_TYPE(AttackType::Bullet,BulletData),
	CREATE_TYPE(AttackType::BoostBullet,BoostBulletData),
	CREATE_TYPE(AttackType::HighExplosive,HighExplosiveBulletData),
	CREATE_TYPE(AttackType::Missile,MissileData),
};

ChPtr::Shared<Attack> Attack::CreateAttackData(ChD3D11::Shader::BaseDrawMesh11* _drawer, ID3D11Device* _device, const std::string& _fileName)
{
	auto&& attackList = LoadAttackList();

	{
		auto bulletIns = attackList.find(_fileName);
		if (bulletIns != attackList.end())
		{
			return (*bulletIns).second;
		}
	}

	std::string text = "";
	ChCpp::TextObject textObject;

	{
		ChCpp::CharFile file;
		file.FileOpen(_fileName);
		text = file.FileReadText();
		textObject.SetText(text);
		file.FileClose();
	}
	
	if (textObject.LineCount() <= 2)return nullptr;

	auto attack = ChPtr::Make_S<Attack>();

	attack->SetMeshDrawer(_drawer);

	attack->Deserialize(_device,text);

	attackList[_fileName] = attack;
	
	attack->SetMeshDrawer(_drawer);

	return attack;

}

void Attack::AllRelease()
{
	auto&& attackList = LoadAttackList();
	if (attackList.empty())return;
	attackList.clear();
}

void Attack::Deserialize(ID3D11Device* _device, const std::string& _text)
{
	ChCpp::TextObject textObject;
	textObject.SetText(_text);
	
	penetration = static_cast<unsigned long>(std::atoll(textObject.GetTextLine(1).c_str()));
	hitSize = static_cast<float>(std::atof(textObject.GetTextLine(2).c_str()));

	{
		ChCpp::ModelLoader::XFile loader;
		bullet->Init(_device);
		loader.CreateModel(bullet, textObject.GetTextLine(3));
	}

	unsigned long pos = 4;

	for (unsigned long pos = 4; pos < textObject.LineCount(); pos++)
	{
		unsigned char atkType = ChStr::GetIntegialFromText<unsigned char>(textObject.GetTextLine(pos));

		if (atkType > ChStd::EnumCast(attackType))attackType = static_cast<AttackType>(atkType);

		auto&& bulletFunction = CreateAttackType[atkType]();
		pos++;
		pos = bulletFunction->Deserialize(_device, *this,textObject, pos);

		externulFunctions.push_back(bulletFunction);
	}


}

std::string Attack::Serialize()
{
	std::string res = "";

	res += std::to_string(penetration) + "\n";
	res += std::to_string(hitSize) + "\n";

	res += bullet->GetModelName() + "\n";

	for (auto&& bulletFunction : externulFunctions)
	{
		res += bulletFunction->Serialize();
	}

	return res;
}

void Attack::InitBulletObject(const ChLMat& _startMat,AttackObject& _bullet)
{
	_bullet.collider.SetScalling(hitSize);
	for (auto&& bulletFunction : externulFunctions)
	{
		bulletFunction->InitBulletObject((bullet->GetDrawLHandMatrix() * _startMat),_bullet);
	}
}

void Attack::UpdateBulletObject(AttackObject& _bullet)
{
	for (auto&& bulletFunction : externulFunctions)
	{
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
	drawer->Draw(*bullet, _mat);
}

unsigned long BulletData::Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos)
{

	unsigned long nowPos = _nowPos;
	firstSpeed = static_cast<float>(std::atof(_text.GetTextLine(nowPos).c_str()));
	_attack.AddDisplayFirstSpeed(firstSpeed);
	return nowPos + 1;
}

std::string BulletData::Serialize()
{
	std::string res =  std::to_string(ChStd::EnumCast(AttackType::Bullet)) + "\n";
	res += std::to_string(firstSpeed) + "\n";
	return res;
}

void BulletData::InitBulletObject(const ChLMat& _startMat, AttackObject& _bullet)
{
	ChLMat mat = _startMat;

	ChVec3 dir = mat.GetZAxisDirection();

	dir.Normalize();

	_bullet.SetPosition(_startMat.GetPosition());
	_bullet.SetRotation(mat.GetRotation());

	dir *= firstSpeed;
	dir.x = -dir.x;
	_bullet.physics->AddMovePowerVector(dir);


	for (unsigned char i = 0; i < 5; i++)
	{
		float pow = static_cast<float>((rand() % 100) * 0.01);
		pow = pow - (pow * 0.5f) + _bullet.dispersalPower;
		float move = static_cast<float>((rand() % 100) * 0.0001) + 0.01f;
		for (unsigned char j = 0; j < i; j++)
		{
			pow *= 0.5f;
			move *= 1.5f;
		}
		//frame->AddSmokeEffectObject(physics->GetPosition(), physics->GetAddMovePowerVector() * -0.01f);
		_bullet.frame->AddSmokeEffectObject(_bullet.physics->GetPosition(), _bullet.physics->GetAddMovePowerVector() * move, pow);
	}
}

void BulletData::UpdateBulletObject(AttackObject& _bullet)
{
	if (_bullet.hitFlg)
	{
		_bullet.UpdateHit();
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

unsigned long BoostBulletData::Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos)
{

	startBoostTime = std::atol(_text.GetTextLine(_nowPos).c_str());
	useBoostTime = std::atol(_text.GetTextLine(_nowPos + 1).c_str());
	boostPow = static_cast<float>(std::atof(_text.GetTextLine(_nowPos + 2).c_str()));

	_attack.AddDisplayStartBoostTime(startBoostTime);
	_attack.AddDisplayUseBoostTime(useBoostTime);
	_attack.AddDisplayBoostPow(boostPow);
	return _nowPos + 3;

}

std::string BoostBulletData::Serialize()
{
	std::string res = std::to_string(ChStd::EnumCast(AttackType::BoostBullet)) + "\n";

	res += std::to_string(startBoostTime) + "\n";
	res += std::to_string(useBoostTime) + "\n";
	res += std::to_string(boostPow) + "\n";

	return res;
}

void BoostBulletData::UpdateBulletObject(AttackObject& _bullet)
{

}

unsigned long HighExplosiveBulletData::Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos)
{

	blastRange = static_cast<float>(std::atof(_text.GetTextLine(_nowPos).c_str()));

	_attack.AddDisplayBlastRange(blastRange);
	return _nowPos + 1;
}

std::string HighExplosiveBulletData::Serialize()
{
	std::string res = std::to_string(ChStd::EnumCast(AttackType::HighExplosive)) + "\n";

	res += std::to_string(blastRange) + "\n";

	return res;
}

void HighExplosiveBulletData::UpdateBulletObject(AttackObject& _bullet)
{

}

unsigned long MissileData::Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos)
{
	rotateSpeed = static_cast<float>(std::atof(_text.GetTextLine(_nowPos).c_str()));
	lostRange = static_cast<float>(std::atof(_text.GetTextLine(_nowPos + 1).c_str()));

	_attack.AddDisplayRotateSpeed(rotateSpeed);
	_attack.AddDisplayLostRange(lostRange);

	return _nowPos + 2;
}

std::string MissileData::Serialize()
{
	std::string res = std::to_string(ChStd::EnumCast(AttackType::Missile)) + "\n";

	res += std::to_string(rotateSpeed) + "\n";
	res += std::to_string(lostRange) + "\n";

	return res;
}

void MissileData::UpdateBulletObject(AttackObject& _bullet)
{

}
