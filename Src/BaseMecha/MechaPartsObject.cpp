#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Bullet/Bullet.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"

void MechaPartsObject::Draw(MeshDrawer& _meshDrawer, const ChLMat& _drawMat)
{
	auto&& mesh = baseParts->GetMesh();
	ChLMat tmp = ChLMat();

	if (positionObject != nullptr)
	{
		positionObject->UpdateAllDrawTransform();
		tmp = positionObject->GetDrawLHandMatrix();
	}

	mesh.SetOutSizdTransform(tmp);

	ChMat_11 drawMat;
	drawMat = _drawMat;

	_meshDrawer.drawer.Draw(_meshDrawer.dc, mesh, drawMat);

	//lastDrawMat = positionObject->GetDrawLHandMatrix();
}
