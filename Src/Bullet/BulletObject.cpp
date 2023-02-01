
#include"../BaseIncluder.h"
#include"../Frames/GameFrame.h"
#include"Bullet.h"
#include"BulletObject.h"
#include"../BaseMecha/BaseMecha.h"

void BulletObject::Init(const ChLMat& _startMat)
{
	data->InitBulletObject(_startMat ,*this);
}

unsigned long BulletObject::GetPenetration()
{ 
	return data->GetPenetration();
}

float BulletObject::GetHitSize()
{
	return data->GetHitSize();
}

void BulletObject::Update()
{
	data->UpdateBulletObject(*this);
}

void BulletObject::Move()
{
	data->MoveBulletObject(*this);
	frame->AddSmokeEffectObject(physics->GetPosition(), physics->GetAddMovePowerVector() * -1.0f);
}

void BulletObject::Draw3D()
{
	ChLMat tmp = ChLMat();
	tmp.SetScalling(ChVec3(10.0f));
	tmp.SetPosition(physics->GetPosition());
	tmp.SetRotation(physics->GetRotation());
	ChMat_11 draw;
	draw = tmp;

	collider.SetPosition(physics->GetPosition());
	collider.SetRotation(physics->GetRotation());
	data->Draw(draw);
}
