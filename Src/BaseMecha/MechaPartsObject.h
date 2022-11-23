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

	//パーツの解除フラグ//
	ChStd::Bool releaseFlg = false;

	//パーツの耐久地//
	short durableValue = 10000;

	 
};
