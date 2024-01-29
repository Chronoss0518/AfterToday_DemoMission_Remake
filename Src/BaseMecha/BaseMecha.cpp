
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Attack/Attack.h"
#include"MechaParts.h"
#include"MechaPartsObject.h"
#include"BaseMecha.h"
#include"../Frames/GameFrame.h"
#include"FunctionComponent/WeaponComponents.h"
#include"FunctionComponent/BoostComponent.h"
#include"../Attack/AttackObject.h"
#include"Controller/ControllerBase.h"

#include"CPU/CPULooker.h"

#define CAMERA_Y_POS 4.0f

#define CENTER_LEN 5.0f

#define HIT_EFFECT_DRAW_FRAME static_cast<long>(BASE_FPS * 2.0f)

static const std::string partsTypeName[]
{
	"Body","Head","Foot","RightArm","LeftArm","Boost","Weapon","Extra"
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

void BaseMecha::Create(const ChVec2& _viewSize, ChD3D11::Shader::BaseDrawMesh11& _drawer, GameFrame* _frame)
{
	viewSize = _viewSize;
	drawer = &_drawer;
	frame = _frame;
	physics->Init();
	if (ChPtr::NullCheck(_frame))return;
	mechasNo = frame->GetMechas().size();
}

void BaseMecha::Load(ID3D11Device* _device, const std::string& _fileName)
{

	std::string text = "";

	ChCpp::CharFile file;
	file.FileOpen(_fileName);
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
				ChVector3 tmpHitSize = parts->GetColliderSize();
				auto positionObject = parts->GetPositionObject();

				if (positionObject != nullptr)tmpHitSize += positionObject->positionObject->GetDrawLHandMatrix().GetPosition();


				tmpHitSize.Abs();
				if (baseHitSize < tmpHitSize.x)baseHitSize = tmpHitSize.x;
				if (baseHitSize < tmpHitSize.y)baseHitSize = tmpHitSize.y;
				if (baseHitSize < tmpHitSize.z)baseHitSize = tmpHitSize.z;


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

	testCollider.SetScalling(baseHitSize);

	return;
}

void BaseMecha::Save(const std::string& _fileName)
{
	std::string res = "";

	res += SavePartsList();

	ChCpp::CharFile file;
	file.FileOpen(_fileName);
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


	physics->Update();

	float moveSize = physics->GetAddMovePowerVector().Len();

	testCollider.SetScalling(moveSize + baseHitSize);

}

void BaseMecha::Move()
{
	BaseMove();

	inputFlgs.SetAllDownFlg();

}

void BaseMecha::MoveEnd()
{

	damageDir = ChVec3();

	viewHorizontal = physics->GetRotation().y;

	auto viewPos = GetViewPos();
	auto viewLookPos = GetViewLookPos();

	ChMat_11 tmpMat;
	tmpMat.CreateViewMatLookTarget(viewPos, viewLookPos, ChVec3(0.0f, 1.0f, 0.0f));

	viewMat = tmpMat;

	auto&& objectLooker = GetComponent<CPUObjectLooker>();

	if (objectLooker == nullptr)return;

	objectLooker->SetViewMatrix(viewMat);

	if (hitEffectDrawFrame < 0)return;
	hitEffectDrawFrame--;
}

void BaseMecha::DrawWeaponComponent(ChPtr::Shared<WeaponComponent> _component)
{
	if (_component == nullptr)return;
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
	physics->SetPosition(physics->GetPosition() + physics->GetAddMovePowerVector());
	physics->SetRotation(physics->GetRotation() + physics->GetAddRotatePowerVector());

	ChVec3 pos = physics->GetPosition();
	ChVec3 normal = (pos - centerPos);
	normal.y = 0.0f;
	centerPos.y = pos.y + CAMERA_Y_POS;

	testCollider.SetPosition(pos);

	float tmp = normal.Len() - CENTER_LEN;
	if (tmp < 0)return;
	normal.Normalize();
	normal.val.SetLen(tmp);
	centerPos += normal;
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

void BaseMecha::Draw2D()
{
	if (ChPtr::NullCheck(frame))return;

	


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

	if (mechaPartsPosList.empty())return;

	_parts.SetPositoinObject(mechaPartsPosList[_no]);
	mechaPartsPosList[_no]->nextParts = &_parts;
	_parts.GetBaseObject()->GetMesh().SetFrameTransform(ChLMat());
}

void BaseMecha::SetHitEffectDrawFrame()
{
	//if (hitEffectDrawFrame >= 0)return;
	hitEffectDrawFrame = HIT_EFFECT_DRAW_FRAME;
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

long BaseMecha::GetHitEffectDrawStartFrame()
{
	return HIT_EFFECT_DRAW_FRAME;
}

void BaseMecha::TestBulletHit(AttackObject& _obj)
{
	if (_obj.IsUseMechaTest(mechasNo))return;
	if (_obj.IsHit())return;
	if (_obj.IsUseMechaTeamTest(GetTeamNo()))
	{
		if (!frame->IsFriendryFireFlg())return;
	}
	if (IsBreak())return;

	ChVec3 dir = _obj.GetMovePower();

	float moveLen = dir.Len();

	float hitSize = _obj.GetHitSize();

	ChVec3 pos = _obj.GetPosition();

	testAttackCollider.SetScalling(hitSize + moveLen);

	testAttackCollider.SetPosition(pos);

	if (!testCollider.IsHit(&testAttackCollider))return;

	dir.Normalize();

	unsigned long cutCount = static_cast<unsigned long>(moveLen / hitSize) + 1;

	testAttackCollider.SetScalling(hitSize);

	bool isHitFlg = false;

	ChVec3 nowVector = dir;

	float nowPos = 0.0f;
	for (unsigned long i = 0; i < cutCount; i++)
	{
		nowPos = hitSize * i;
		if (nowPos <= 0.0f)nowPos = 1.0f;
		nowVector.val.SetLen(nowPos);

		testAttackCollider.SetPosition(pos + nowVector);

		for (auto&& parts : mechaParts)
		{
			float damage = parts->GetDamage(testAttackCollider, _obj);
			if (damage == 0.0f)continue;
			nowDurable -= damage;
			isHitFlg = true;
			//damageDir = testAttackCollider.GetPos() - physics->GetPosition();
			damageDir = dir * -1.0f;
			damageDir.Normalize();
			break;
		}

		if (!isHitFlg)continue;
		_obj.SetMovePower(nowVector);
		_obj.SetIsHitTrue();

		break;
	}

	if (breakFlg)return;
	if (nowDurable > 0)return;

	Break();
	
	Destroy();

	breakFlg = true;
}

void BaseMecha::Break()
{
	frame->BreakMecha(this);
}