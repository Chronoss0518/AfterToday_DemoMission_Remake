
#include"../BaseIncluder.h"
#include"../Frames/GameFrame.h"
#include"Bullet.h"
#include"BulletObject.h"
#include"../BaseMecha/BaseMecha.h"

void BulletObject::Init(const ChLMat& _startMat)
{
	data->InitBulletObject(_startMat ,*this);
}

void BulletObject::Update()
{
	data->UpdateBulletObject(*this);
}

void BulletObject::Move()
{
	data->MoveBulletObject(*this);
}

void BulletObject::Draw3D()
{
	ChLMat tmp = ChLMat();
	tmp.SetScalling(ChVec3(10.0f));
	tmp.SetPosition(physics->GetPosition());
	tmp.SetRotation(physics->GetRotation());
	ChMat_11 draw;
	draw = tmp;
	data->Draw(draw);
}
