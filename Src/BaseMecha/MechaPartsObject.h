#pragma once

class MechaParts;

class MechaPartsObject:public ChCpp::BaseObject
{
public:

	friend MechaParts;

public:

	void SetPositoinObject(ChPtr::Shared<ChCpp::FrameObject>_positionObject) { positionObject = _positionObject; }

	MechaParts* GetBaseObject() { return baseParts; }

public:

	short GetDurableValue() { return durableValue; }

public:

	virtual void Draw(MeshDrawer& _meshDrawer,const ChLMat& _drawMat);

public:

	void Damage();

protected:

	MechaParts* baseParts;

	ChLMat lastDrawMat;

private:

	ChPtr::Shared<ChCpp::FrameObject>positionObject = nullptr;

	//�p�[�c�̉����t���O//
	ChStd::Bool releaseFlg = false;

	//�p�[�c�̑ϋv�n//
	short durableValue = 10000;

	 
};
