
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Bullet/Bullet.h"
#include"MechaParts.h"
#include"MechaPartsObject.h"
#include"WeaponObject.h"
#include"BaseMecha.h"
#include"../Physics/PhysicsMachine.h"

#define SAVE_PATH(_fileName) TARGET_DIRECTORY("Save/AssemMechaFrame/" _fileName)


BaseMecha::BaseMecha()
{
	mechasNo = GetList().size();
	GetList().push_back(this);
}

BaseMecha::~BaseMecha()
{
	auto&& list = GetList();
	auto&& it = std::find(list.begin(), list.end(), this);
	list.erase(it);
}

void BaseMecha::Create(const ChVec2& _viewSize, MeshDrawer& _drawer)
{
	viewSize = _viewSize;
	drawer = &_drawer;
	physics->Init();
}

void BaseMecha::Load(ID3D11Device* _device, const std::string& _fileName)
{

	std::string text = "";

	ChCpp::File<> file;
	file.FileOpen(SAVE_PATH(+_fileName));
	text = file.FileReadText();
	file.FileClose();

	ChCpp::TextObject textObject;
	textObject.SetText(text);

	unsigned long count = std::atol(textObject.GetTextLine(0).c_str());

	for (unsigned long i = 0; i < count; i++)
	{
		LoadParts(_device, textObject.GetTextLine(i + 1));
	}

	unsigned long rWeaponCount = std::atol(textObject.GetTextLine(count + 1).c_str());

	for (unsigned long i = 0; i < rWeaponCount; i++)
	{
		LoadParts(_device, textObject.GetTextLine(i + count + 2));
	}

	unsigned long lWeaponCount = std::atol(textObject.GetTextLine(count + rWeaponCount + 2).c_str());

	for (unsigned long i = 0; i < lWeaponCount; i++)
	{
		LoadParts(_device, textObject.GetTextLine(i + count + rWeaponCount + 3));
	}

	nowEnelgy = maxEnelgy;
	physics->SetMass(mass);
}

void BaseMecha::LoadParts(ID3D11Device* _device, const std::string& _fileName)
{
	MechaParts::LoadParts(*this, _device, _fileName);
}

void BaseMecha::Save(const std::string& _fileName)
{
	std::string res = "";
	res += std::to_string(mechaParts.size()) + "\n";

	for (auto&& parts : mechaParts)
	{
		res += parts->GetBaseObject()->GetThisFileName() + "\n";
	}
	res += "RightWeapon\n";
	res += std::to_string(rightWeapon.weapon.size()) + "\n";
	for (auto&& parts : rightWeapon.weapon)
	{
		res += parts->GetBaseObject()->GetThisFileName() + "\n";
	}
	res += "LeftWeapon\n";
	res += std::to_string(leftWeapon.weapon.size()) + "\n";
	for (auto&& parts : leftWeapon.weapon)
	{
		res += parts->GetBaseObject()->GetThisFileName() + "\n";
	}

	ChCpp::File<> file;
	file.FileOpen(SAVE_PATH(+_fileName));
	file.FileWriteText(res);
	file.FileClose();

}

void BaseMecha::Release()
{

}

void BaseMecha::UpdateEnd()
{
	unsigned long fps = ChSystem::SysManager().GetFPS();

	nowEnelgy += (chargeEnelgy * (120 / fps));

	nowEnelgy = nowEnelgy > maxEnelgy ? maxEnelgy : nowEnelgy;
}

void BaseMecha::Move()
{
	BaseMove();

	inputFlgs.SetAllDownFlg();

	//return;

	ChMat_11 camMat;
	{
		ChLMat tmpMat;

		tmpMat.SetRotationYAxis(ChMath::ToRadian(rot.y));
		tmpMat.SetPosition(pos + ChVec3(0.0f, 5.0f, 0.0f));
		auto lookPos = tmpMat.Transform(ChVec3(0.0f, -9.0f, 5.0f));
		auto camPos = tmpMat.Transform(ChVec3(0.0f, 0.0f,-15.0f));

		camMat.CreateViewMatLookTarget(camPos, lookPos, ChVec3(0.0f, 1.0f, 0.0f));
	}

	drawer->drawer.SetViewMatrix(camMat);

}

void BaseMecha::BaseMove()
{
	physics->SetPosition(pos);
	physics->SetRotation(rot);
	physics->SetAddMovePowerVector(moveVec);
	physics->SetAddRotatePowerVector(rotateVec);
	physics->Update();
	moveVec = physics->GetAddMovePowerVector();
	rotateVec = physics->GetAddRotatePowerVector();

	pos += moveVec;
	
	rot -= rotateVec;


}

void BaseMecha::Draw2D()
{
	if (mechasNo != GetMechaCamNo())return;



}

void BaseMecha::Draw3D()
{
	ChLMat drawMat;
	drawMat.SetRotationYAxis(ChMath::ToRadian(rot.y));
	drawMat.SetPosition(pos);

	for (auto&& parts : mechaParts)
	{
		parts->Draw(*drawer, drawMat);
	}

}

void BaseMecha::Deserialize(const std::string& _fileName)
{

}

std::string BaseMecha::Serialize()
{
	return "";
}

void BaseMecha::SetGroundHeight(const float _height)
{
	if (physics->GetGroundHeight() < _height)return;

	physics->SetGroundHeight(_height);
}