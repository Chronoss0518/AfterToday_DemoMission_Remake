#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Frames/GameFrame.h"
#include"Attack.h"
#include"AttackObject.h"
#include"../EditFrame/PartsParameters.h"


#define CREATE_TYPE(_enum,_class) {ChStd::EnumCast(_enum),[]()->ChPtr::Shared<Attack::AttackBase>{return ChPtr::Make_S<_class>();}}

#define ATTACK_FILE_SWORD L""
#define ATTACK_FILE_EXPLOSIVE L""


std::map<unsigned char, std::function<ChPtr::Shared<Attack::AttackBase>()>> CreateAttackType
{
	CREATE_TYPE(AttackType::Bullet,BulletData),
	CREATE_TYPE(AttackType::BoostBullet,BoostBulletData),
	CREATE_TYPE(AttackType::HighExplosive,HighExplosiveBulletData),
	CREATE_TYPE(AttackType::Missile,MissileData),
};

ChPtr::Shared<Attack> Attack::CreateAttackDataFromSword(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device, const std::wstring& _fileName)
{
	auto&& attackList = LoadAttackList();

	{
		auto bulletIns = attackList.find(_fileName);
		if (bulletIns != attackList.end())
		{
			return (*bulletIns).second;
		}
	}

	auto&& attack = CreateAttackData(_drawer, _device, ATTACK_FILE_SWORD);

	if (attack == nullptr)return nullptr;

	attack->useFileName = ATTACK_FILE_SWORD;

	attackList[_fileName] = attack;

	return attack;

}

ChPtr::Shared<Attack> Attack::CreateAttackDataFromExplosive(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device, const std::wstring& _fileName)
{
	auto&& attackList = LoadAttackList();

	{
		auto bulletIns = attackList.find(_fileName);
		if (bulletIns != attackList.end())
		{
			return (*bulletIns).second;
		}
	}

	auto&& attack = CreateAttackData(_drawer, _device, ATTACK_FILE_EXPLOSIVE);

	if (attack == nullptr)return nullptr;

	attack->useFileName = ATTACK_FILE_EXPLOSIVE;

	attackList[_fileName] = attack;

	return attack;

}

ChPtr::Shared<Attack> Attack::CreateAttackDataFromBullet(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device, const std::wstring& _fileName)
{
	auto&& attackList = LoadAttackList();

	{
		auto bulletIns = attackList.find(_fileName);
		if (bulletIns != attackList.end())
		{
			return (*bulletIns).second;
		}
	}

	auto&& attack = CreateAttackData(_drawer,_device, _fileName);

	if (attack == nullptr)return nullptr;

	attack->useFileName = _fileName;

	attackList[_fileName] = attack;

	return attack;

}

ChPtr::Shared<Attack> Attack::CreateAttackData(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device, const std::wstring& _fileName)
{
	auto&& attackList = LoadAttackList();

	std::wstring text = L"";
	ChCpp::TextObject<wchar_t> textObject;

	{
		ChCpp::WCharFile file;
		file.SetLocaleName("Japanese");
		file.FileOpen(_fileName);
		text = file.FileReadText();
		textObject.SetText(text.c_str());
		file.FileClose();
	}

	if (textObject.LineCount() <= 2)return nullptr;

	auto attack = ChPtr::Make_S<Attack>();

	attack->useFileName = _fileName;
	attack->objectName = _fileName;

	if (_fileName.find(L"/"))
	{
		attack->objectName = _fileName.substr(_fileName.find_last_of(L"/") + 1);
	}

	if (attack->objectName.find(L"."))
	{
		attack->objectName = attack->objectName.substr(0, attack->objectName.find(L"."));
	}

	attack->SetMeshDrawer(_drawer);

	attack->Deserialize(_device, text);

	return attack;

}

void Attack::AllRelease()
{
	auto&& attackList = LoadAttackList();
	if (attackList.empty())return;
	attackList.clear();
}

void Attack::Deserialize(ID3D11Device* _device, const std::wstring& _text)
{
	ChCpp::TextObject<wchar_t> textObject;
	textObject.SetText(_text.c_str());
	
	penetration = ChStr::GetNumFromText<unsigned long,wchar_t>(textObject.GetTextLine(1).c_str());
	hitSize = ChStr::GetNumFromText<float,wchar_t>(textObject.GetTextLine(2).c_str());

	{
		ChCpp::ModelLoader::XFile<wchar_t> loader;
		bullet->Init(_device);
		loader.CreateModel(bullet, textObject.GetTextLine(3));

		if (bullet->GetMyName() == L"Root")
		{
			defaultMat = bullet->GetDrawLHandMatrix();
		}

	}

	unsigned long pos = 4;

	for (unsigned long pos = 4; pos < textObject.LineCount(); pos++)
	{
		unsigned char atkType = ChStr::GetNumFromText<unsigned char,wchar_t>(textObject.GetTextLine(pos));

		if (atkType > ChStd::EnumCast(attackType))attackType = static_cast<AttackType>(atkType);

		auto&& bulletFunction = CreateAttackType[atkType]();
		pos++;
		pos = bulletFunction->Deserialize(_device, *this,textObject, pos);

		externulFunctions.push_back(bulletFunction);
	}


}

std::wstring Attack::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(penetration) + L"\n";
	res += std::to_wstring(hitSize) + L"\n";

	res += bullet->GetModelName() + L"\n";

	for (auto&& bulletFunction : externulFunctions)
	{
		res += bulletFunction->Serialize();
	}

	return res;
}

void Attack::SetPartameter(PartsParameterStruct::WeaponData& _partsParameter)
{
	auto&& attack = ChPtr::Make_S<PartsParameterStruct::AttackData>();

	attack->objectName = objectName;
	attack->penetration = penetration;

	attack->hitSize = hitSize;

	for (auto&& functions : externulFunctions)
	{
		functions->SetPartameter(*attack);
	}

	_partsParameter.attackData.push_back(attack);
}

void Attack::InitBulletObject(const ChLMat& _startMat,AttackObject& _bullet)
{
	_bullet.collider.SetScalling(hitSize);
	
	for (auto&& bulletFunction : externulFunctions)
	{
		bulletFunction->InitBulletObject((_startMat),_bullet);
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

unsigned long BulletData::Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos)
{

	unsigned long nowPos = _nowPos;
	firstSpeed = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(nowPos).c_str());
	return nowPos + 1;
}

std::wstring BulletData::Serialize()
{
	std::wstring res =  std::to_wstring(ChStd::EnumCast(AttackType::Bullet)) + L"\n";
	res += std::to_wstring(firstSpeed) + L"\n";
	return res;
}

void BulletData::SetPartameter(PartsParameterStruct::AttackData& _parameter)
{
	auto&& data = ChPtr::Make_S<PartsParameterStruct::BulletData>();

	data->firstSpeed = firstSpeed;
	_parameter.attackDataBase.push_back(data);
}

void BulletData::InitBulletObject(const ChLMat& _startMat, AttackObject& _bullet)
{
	ChLMat mat = _startMat;

	ChVec3 dir = mat.GetYAxisDirection();

	dir.Normalize();

	ChVec3 tmpRotate = _startMat.GetEulerRotationYXZ();
	tmpRotate.x = ChMath::ToDegree(tmpRotate.x);
	tmpRotate.y = ChMath::ToDegree(tmpRotate.y);
	tmpRotate.z = ChMath::ToDegree(tmpRotate.z);

	_bullet.SetPosition(_startMat.GetPosition());
	_bullet.SetRotation(tmpRotate);

	dir *= firstSpeed;
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

unsigned long BoostBulletData::Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos)
{

	startBoostTime = ChStr::GetNumFromText<unsigned long, wchar_t>(_text.GetTextLine(_nowPos).c_str());
	useBoostTime = ChStr::GetNumFromText<unsigned long, wchar_t>(_text.GetTextLine(_nowPos + 1).c_str());
	boostPow = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_nowPos + 2).c_str());

	return _nowPos + 3;

}

std::wstring BoostBulletData::Serialize()
{
	std::wstring res = std::to_wstring(ChStd::EnumCast(AttackType::BoostBullet)) + L"\n";

	res += std::to_wstring(startBoostTime) + L"\n";
	res += std::to_wstring(useBoostTime) + L"\n";
	res += std::to_wstring(boostPow) + L"\n";

	return res;
}

void BoostBulletData::SetPartameter(PartsParameterStruct::AttackData& _parameter)
{
	auto&& data = ChPtr::Make_S<PartsParameterStruct::BoostBulletData>();

	data->boostPow = boostPow;
	data->startBoostTime = startBoostTime;
	data->useBoostTime = useBoostTime;
	_parameter.attackDataBase.push_back(data);
}

void BoostBulletData::UpdateBulletObject(AttackObject& _bullet)
{

}

unsigned long HighExplosiveBulletData::Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos)
{

	blastRange = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_nowPos).c_str());

	return _nowPos + 1;
}

std::wstring HighExplosiveBulletData::Serialize()
{
	std::wstring res = std::to_wstring(ChStd::EnumCast(AttackType::HighExplosive)) + L"\n";

	res += std::to_wstring(blastRange) + L"\n";

	return res;
}

void HighExplosiveBulletData::SetPartameter(PartsParameterStruct::AttackData& _parameter)
{
	auto&& data = ChPtr::Make_S<PartsParameterStruct::HighExplosiveBulletData>();

	data->blastRange = blastRange;
	_parameter.attackDataBase.push_back(data);
}

void HighExplosiveBulletData::UpdateBulletObject(AttackObject& _bullet)
{

}

unsigned long MissileData::Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos)
{
	rotateSpeed = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_nowPos).c_str());
	lostRange = ChStr::GetNumFromText<float,wchar_t>(_text.GetTextLine(_nowPos + 1).c_str());

	return _nowPos + 2;
}

std::wstring MissileData::Serialize()
{
	std::wstring res = std::to_wstring(ChStd::EnumCast(AttackType::Missile)) + L"\n";

	res += std::to_wstring(rotateSpeed) + L"\n";
	res += std::to_wstring(lostRange) + L"\n";

	return res;
}

void MissileData::SetPartameter(PartsParameterStruct::AttackData& _parameter)
{
	auto&& data = ChPtr::Make_S<PartsParameterStruct::MissileData>();

	data->lostRange = lostRange;
	data->rotateSpeed = rotateSpeed;
	_parameter.attackDataBase.push_back(data);
}

void MissileData::UpdateBulletObject(AttackObject& _bullet)
{

}
