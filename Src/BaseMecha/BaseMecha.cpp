
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Bullet/Bullet.h"
#include"MechaParts.h"
#include"MechaPartsObject.h"
#include"WeaponObject.h"
#include"BaseMecha.h"
#include"../Physics/PhysicsMachine.h"

#define SAVE_PATH(_fileName) TARGET_DIRECTORY("Save/AssemMechaFrame/" _fileName)

static const std::string partsTypeName[]
{
	"Body","Head","Foot","RightArm","LeftArm","Boost","RightWeapon","LeftWeapon"
};

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

	unsigned long pos = 0;

	for (unsigned char i = 0; i < ChStd::EnumCast(PartsPosNames::None); i++)
	{
		pos = LoadPartsList(_device, textObject, pos,static_cast<PartsPosNames>(i));
	}

	nowEnelgy = maxEnelgy;
	physics->SetMass(mass);
}

unsigned long BaseMecha::LoadPartsList(ID3D11Device* _device, const ChCpp::TextObject& _textObject, const unsigned long _pos, const BaseMecha::PartsPosNames _name)
{

	unsigned long count = std::atol(_textObject.GetTextLine(_pos + 1).c_str());

	for (unsigned long i = 0; i < count; i++)
	{
		MechaParts::LoadParts(*this, _device, _textObject.GetTextLine(i + _pos + 2), _name);
	}

	return _pos + 2 + count;
}

void BaseMecha::Save(const std::string& _fileName)
{
	std::string res = "";

	for (unsigned char i = 0; i < ChStd::EnumCast(PartsPosNames::None); i++)
	{
		res += SavePartsList(static_cast<PartsPosNames>(i));
	}

	ChCpp::File<> file;
	file.FileOpen(SAVE_PATH(+_fileName));
	file.FileWriteText(res);
	file.FileClose();

}

std::string BaseMecha::SavePartsList(const BaseMecha::PartsPosNames _name)
{
	std::string res = partsTypeName[ChStd::EnumCast(_name)] + "\n";
	res += std::to_string(mechaParts[ChStd::EnumCast(_name)].size()) + "\n";
	for (auto&& parts : mechaParts[ChStd::EnumCast(_name)])
	{
		res += parts->GetBaseObject()->GetThisFileName() + "\n";
	}
	return res;
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

	for (auto&& partsList : mechaParts)
	{
		for (auto&& parts : partsList)
		{
			parts->Draw(*drawer, drawMat);
		}
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