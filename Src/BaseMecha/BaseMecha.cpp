
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Attack/Attack.h"
#include"MechaParts.h"
#include"MechaPartsObject.h"
#include"BaseMecha.h"
#include"../Frames/GameFrame.h"
#include"../Physics/PhysicsMachine.h"
#include"FunctionComponent/WeaponComponents.h"
#include"FunctionComponent/BoostComponent.h"
#include"../Attack/AttackObject.h"
#include"Controller/ControllerBase.h"

#include"CPU/CPULooker.h"

#define SAVE_PATH(_fileName) TARGET_DIRECTORY("Save/AssemMechaFrame/" _fileName)

#define CAMERA_Y_POS 5.0f

#define CENTER_LEN 5.0f

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
	physics->SetLeftRightWallLen(3.0f);
	physics->SetFrontBackWallLen(3.0f);
}

BaseMecha::~BaseMecha()
{

}

void BaseMecha::Create(const ChVec2& _viewSize, MeshDrawer& _drawer, GameFrame* _frame)
{
	viewSize = _viewSize;
	drawer = &_drawer;
	frame = _frame;
	physics->Init();
	mechasNo = frame->GetMechaList().GetObjectCount();
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

	SetComponent<LookAnchor>();

	LoadPartsList(_device, textObject);

	nowEnelgy = maxEnelgy;
	nowDurable = durable;
	physics->SetMass(mass);
}

void BaseMecha::LoadPartsList(ID3D11Device* _device, const ChCpp::TextObject& _textObject)
{

	unsigned long count = std::atol(_textObject.GetTextLine(0).c_str());

	for (unsigned long i = 0; i < count; i++)
	{
		auto texts = ChStr::Split(_textObject.GetTextLine(i + 1), " ");

		auto parts = MechaParts::LoadParts(*this, _device, drawer, frame, texts[0]);
		parts->SetFrame(frame);
		parts->SetBaseMecha(this);


		auto anchor = GetComponent<LookAnchor>();

		parts->SetLookAnchorNo(anchor->GetPositionListCount());

		anchor->AddLookAnchorPosition(parts->GetBaseObject()->GetMesh().GetInitAllFrameBoxSize() * 2.0f, ChLMat());

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
				auto&& weap = GetComponentObject<RightWeaponComponent>();

				weap->AddWeapon(parts);
				parts->SetRWeapon(true);
				continue;
			}

			if (text.find("L*") != std::string::npos)
			{
				auto&& weap = GetComponentObject<LeftWeaponComponent>();

				weap->AddWeapon(parts);
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

		res += parts->GetBaseObject()->GetThisFileName() + positionName + weaponName + "\n";
	}
	return res;
}

void BaseMecha::Release()
{
	for (auto&& parts : mechaParts)
	{
		if (parts == nullptr)continue;
		parts->Release();
	}

	mechaParts.clear();
}

void BaseMecha::Update()
{
	for (auto&& parts : mechaParts)
	{
		parts->Update();
	}
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

}

void BaseMecha::MoveEnd()
{
	viewHorizontal = physics->GetRotation().y;

	auto viewPos = GetViewPos();
	auto viewLookPos = GetViewLookPos();

	ChMat_11 tmpMat;
	tmpMat.CreateViewMatLookTarget(viewPos, viewLookPos, ChVec3(0.0f, 1.0f, 0.0f));

	viewMat = tmpMat;

	auto&& objectLooker = GetComponent<CPUObjectLooker>();

	if (objectLooker == nullptr)return;

	objectLooker->SetViewMatrix(viewMat);
}

ChVec3 BaseMecha::GetViewPos()
{

	ChLMat camYMat, camXMat;

	camYMat.SetRotationYAxis(ChMath::ToRadian(viewHorizontal));
	camXMat.SetRotationXAxis(-ChMath::ToRadian(viewVertical));
	camYMat = camXMat * camYMat;
	camYMat.SetPosition(centerPos + ChVec3(0.0f, CAMERA_Y_POS, 0.0f));

	return camYMat.Transform(ChVec3(0.0f, 0.0f, -15.0f));
}

ChVec3 BaseMecha::GetViewLookPos()
{

	ChLMat camYMat, camXMat;

	camYMat.SetRotationYAxis(ChMath::ToRadian(viewHorizontal));
	camXMat.SetRotationXAxis(-ChMath::ToRadian(viewVertical));
	camYMat = camXMat * camYMat;
	camYMat.SetPosition(centerPos + ChVec3(0.0f, CAMERA_Y_POS - 2.0f, 0.0f));

	return camYMat.Transform(ChVec3(0.0f, 0.0f, 5.0f));
}

void BaseMecha::BaseMove()
{
	physics->Update();
	physics->SetPosition(physics->GetPosition() + physics->GetAddMovePowerVector());
	physics->SetRotation(physics->GetRotation() + physics->GetAddRotatePowerVector());

	ChVec3 pos = physics->GetPosition();
	ChVec3 normal = (pos - centerPos);
	float tmp = normal.Len() - CENTER_LEN;
	if (tmp < 0)return;
	normal.Normalize();
	normal.val.SetLen(tmp);
	centerPos += normal;
}

void BaseMecha::Draw2D()
{



}

void BaseMecha::Draw3D()
{
	ChLMat drawMat;
	drawMat.SetRotationYAxis(ChMath::ToRadian(physics->GetRotation().y));
	drawMat.SetPosition(physics->GetPosition());

	ChLMat tmp;
	tmp.SetRotation(ChVec3(ChMath::ToRadian(-90.0f), 0.0f, 0.0f));
	positions[ChStd::EnumCast(PartsPosNames::RArm)][0]->positionObject->SetOutSizdTransform(tmp);
	positions[ChStd::EnumCast(PartsPosNames::LArm)][0]->positionObject->SetOutSizdTransform(tmp);

	auto boostComponent = GetComponent<BoostComponent>();

	if (boostComponent != nullptr)boostComponent->BoostDrawBegin();

	auto anchor = GetComponent<LookAnchor>();

	for (auto&& parts : mechaParts)
	{
		parts->Draw(drawMat);
		ChLMat tmpMat;
		tmpMat = parts->GetLastDrawMat();
		anchor->UpdateLookAnchorPosition(parts->GetLookAnchorNo(), tmpMat);
	}

	if (boostComponent != nullptr)boostComponent->BoostDrawEnd();

}

void BaseMecha::Deserialize(const std::string& _fileName)
{

}

std::string BaseMecha::Serialize()
{
	return "";
}

void BaseMecha::SetTeamNo(const unsigned long _team)
{
	auto controller = GetComponent<ControllerBase>();
	if (controller == nullptr)return;
	controller->SetTeamNo(_team);
}

void BaseMecha::SetGroundHeight(const float _height)
{
	if (physics->GetGroundHeight() < _height)return;

	physics->SetGroundHeight(_height);
}

void BaseMecha::SetPartsPos(MechaPartsObject& _parts, const PartsPosNames _name, unsigned long _no)
{

	if (_name == PartsPosNames::None)return;

	auto mechaPartsPosList = GetMechaPartsPosList(_name);

	_parts.SetPositoinObject(mechaPartsPosList[_no]);
	_parts.GetBaseObject()->GetMesh().SetFrameTransform(ChLMat());
}

void BaseMecha::AddMoveVector(const ChVec3& _moveVecAdd)
{
	physics->AddMovePowerVector(_moveVecAdd);
}

void BaseMecha::AddRotateVector(const ChVec3& _rotateVecAdd)
{
	physics->AddRotatePowerVector(_rotateVecAdd);
}

void BaseMecha::SetPosition(const ChVec3& _pos)
{
	physics->SetPosition(_pos);
	centerPos = _pos;
}

void BaseMecha::SetRotation(const ChVec3& _rot)
{
	physics->SetRotation(_rot);
}

ChVec3 BaseMecha::GetPosition()
{
	return physics->GetPosition();
}

ChVec3 BaseMecha::GetRotation()
{
	return physics->GetRotation();
}

unsigned long BaseMecha::GetTeamNo()
{
	auto controller = GetComponent<ControllerBase>();
	if (controller == nullptr)return 0;
	return controller->GetTeamNo();
}

void BaseMecha::TestBulletHit(AttackObject& _obj)
{
	if (_obj.GetBaseMecha() == this)return;
	if (_obj.IsHit())return;
	if (IsBreak())return;

	damageDir = ChVec3();

	ChVec3 pos = _obj.GetPosition();

	ChVec3 dir = _obj.GetMovePower();

	float moveLen = dir.Len();

	dir.Normalize();

	float hitSize = _obj.GetHitSize();

	std::vector<float> lenList;

	float nowPos = 0;

	for (nowPos = 0; nowPos < moveLen; nowPos += hitSize)
	{
		lenList.push_back(nowPos);
	}

	nowPos = nowPos - hitSize + moveLen;

	lenList.push_back(nowPos);

	ChCpp::SphereCollider collider;

	collider.SetScalling(hitSize);

	bool isHitFlg = false;

	for (float nowPos : lenList)
	{
		ChVec3 nowVector = dir;
		nowVector.val.SetLen(nowPos);

		collider.SetPosition(pos + nowVector);

		for (auto&& parts : mechaParts)
		{
			float damage = parts->GetDamage(collider, _obj);
			if (damage == 0.0f)continue;
			durable -= damage;
			isHitFlg = true;
			damageDir = collider.GetPos() - physics->GetPosition();
			damageDir.Normalize();
			break;
		}

		if (!isHitFlg)continue;
		_obj.SetMovePower(nowVector);
		_obj.SetIsHitTrue();

		break;
	}

	if (breakFlg)return;
	if (durable > 0)return;
	
	Break();
	
	Destroy();

	breakFlg = true;
}

void BaseMecha::Break()
{
	frame->BreakMecha(this);
}