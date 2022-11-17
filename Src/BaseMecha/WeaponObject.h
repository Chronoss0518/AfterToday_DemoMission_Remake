#pragma once

class WeaponParts;

#include"MechaPartsObject.h"

class WeaponObject:public MechaPartsObject
{
public:
	friend WeaponParts;
	
	//virtual void Attack() = 0;

};

class SwordObject :public WeaponObject
{
public:

	//void Attack()override {};

};

class GunObject :public WeaponObject
{
public:

	void Draw(MeshDrawer& _meshDrawer, const ChLMat& _drawMat)override;

	//void Attack()override {};

private:

	ChPtr::Shared<ChCpp::FrameObject>shotPos = nullptr;

	ChLMat lastShotPos;

};

class BulletObject
{

};