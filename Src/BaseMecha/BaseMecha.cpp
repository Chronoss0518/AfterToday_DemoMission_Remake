
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Bullet/Bullet.h"
#include"MechaParts.h"
#include"MechaPartsObject.h"
#include"BaseMecha.h"
#include"../Physics/PhysicsMachine.h"

#define SAVE_PATH(_fileName) TARGET_DIRECTORY("Save/AssemMechaFrame/" _fileName)

static const std::string partsTypeName[]
{
	"Body","Head","Foot","RightArm","LeftArm","Boost","Holder","Weapon"
};

static const std::string weaponTypeName[]
{
	"R*","L*"
};

BaseMecha::BaseMecha()
{
	mechasNo = GetList().size();
	GetList().push_back(this);
	physics->SetLeftRightWallLen(3.0f);
	physics->SetFrontBackWallLen(3.0f);
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

	LoadPartsList(_device, textObject);

	nowEnelgy = maxEnelgy;
	physics->SetMass(mass);
}

void BaseMecha::LoadPartsList(ID3D11Device* _device, const ChCpp::TextObject& _textObject)
{

	unsigned long count = std::atol(_textObject.GetTextLine(0).c_str());

	for (unsigned long i = 0; i < count; i++)
	{
		auto texts = ChStr::Split(_textObject.GetTextLine(i + 1)," ");

		auto parts = MechaParts::LoadParts(*this, _device, texts[0]);

		AddMechaParts(parts);

		if (texts.size() <= 1)continue;

		for (auto&& text : texts)
		{

			bool findFlg = false;

			for (unsigned char j = 0; j < ChStd::EnumCast(PartsPosNames::None); j++)
			{
				if (text.find(partsTypeName[j]) == std::string::npos)continue;

				unsigned long no = std::atol(text.substr(partsTypeName[j].size()).c_str());
				SetPartsPos(*parts, static_cast<PartsPosNames>(j), no);
				parts->SetPartsPosData(j, no);
				findFlg = true;
				break;
			}

			if (findFlg)continue;

			if (text.find("R*") != std::string::npos)
			{
				
				AddRightWeappon(parts);
				parts->SetRWeapon(true);
				continue;
			}

			if (text.find("L*") != std::string::npos)
			{
				AddRightWeappon(parts);
				parts->SetLWeapon(true);
				continue;
			}

		}

	}

	return;
}

void BaseMecha::Save(const std::string& _fileName)
{
	std::string res = "";

	res += SavePartsList();

	ChCpp::File<> file;
	file.FileOpen(SAVE_PATH(+_fileName));
	file.FileWriteText(res);
	file.FileClose();

}

std::string BaseMecha::SavePartsList()
{
	std::string res = std::to_string(mechaParts.size()) + "\n";
	for (auto&& parts : mechaParts)
	{
		std::string positionName = "";

		if (parts->GetPartsPosName() < ChStd::EnumCast(BaseMecha::PartsPosNames::None))
		{
			positionName = " " + partsTypeName[parts->GetPartsPosName()];
			positionName += std::to_string(parts->GetPartsPosNo());
		}

		std::string weaponName = "";

		if (parts->GetRWeapon())
		{
			weaponName = " " + weaponTypeName[0];
		}

		if (parts->GetLWeapon())
		{
			weaponName = " " + weaponTypeName[1];
		}

		res += parts->GetBaseObject()->GetThisFileName() +  positionName + weaponName + "\n";
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

	ChLMat tmp;
	tmp.SetRotation(ChVec3(ChMath::ToRadian(-90.0f), 0.0f, 0.0f));
	positions[ChStd::EnumCast(PartsPosNames::RArm)][0]->positionObject->SetOutSizdTransform(tmp);

	DrawBeginFunction();

	for (auto&& parts : mechaParts)
	{
		parts->Draw(*drawer, drawMat);
	}

	DrawEndFunction();

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

void BaseMecha::SetPartsPos(MechaPartsObject& _parts, const PartsPosNames _name,unsigned long _no)
{

	if (_name  == PartsPosNames::None)return;

	auto mechaPartsPosList = GetMechaPartsPosList(_name);

	_parts.SetPositoinObject(mechaPartsPosList[_no]);
	_parts.GetBaseObject()->GetMesh().SetFrameTransform(ChLMat());
}