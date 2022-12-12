
#include"../BaseIncluder.h"
#include"../Frames/GameFrame.h"
#include"Bullet.h"
#include"BulletObject.h"
#include"../BaseMecha/BaseMecha.h"

void BulletObject::Init()
{
	data->InitBulletObject(*this);
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
	ChMat_11 tmp;
	tmp.SetPosition(physics->GetPosition());
	tmp.SetRotation(physics->GetRotation());
	tmp.SetScaleSize(ChVec3(10.0f));
	data->Draw(tmp);
}
