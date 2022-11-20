#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Bullet/Bullet.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"

void MechaPartsObject::Draw(MeshDrawer& _meshDrawer, const ChLMat& _drawMat)
{
	auto&& mesh = baseParts->GetMesh();
	lastDrawMat.Identity();

	if (positionObject != nullptr)
	{
		positionObject->UpdateAllDrawTransform();
		lastDrawMat = positionObject->GetDrawLHandMatrix();
	}

	mesh.SetOutSizdTransform(lastDrawMat);

	ChMat_11 drawMat;
	drawMat = _drawMat;

	_meshDrawer.drawer.Draw(_meshDrawer.dc, mesh, drawMat);

}
