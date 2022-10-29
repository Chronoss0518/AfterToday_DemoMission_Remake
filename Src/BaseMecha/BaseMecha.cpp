
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Bullet/Bullet.h"
#include"MechaParts.h"
#include"MechaPartsObject.h"
#include"BaseMecha.h"

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

	nowEnelgy = maxEnelgy;

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
	ChLMat tmp;
	tmp.SetRotationYAxis(ChMath::ToRadian(rot.y));


	for (unsigned char i = 0; i <= ChStd::EnumCast(InputName::Jump) - ChStd::EnumCast(InputName::Front); i++)
	{
		if (!inputFlgs.GetBitFlg(i + ChStd::EnumCast(InputName::Front)))continue;
		NormalMoveUpdate(i, i != ChStd::EnumCast(InputName::Jump) ? movePow : jumpPow, tmp);
	}

	if (inputFlgs.GetBitFlg(ChStd::EnumCast(InputName::LeftRotation)))
	{
		rotateVec.y -= rotatePow;
	}
	if (inputFlgs.GetBitFlg(ChStd::EnumCast(InputName::RightRotation)))
	{
		rotateVec.y += rotatePow;
	}

}

void BaseMecha::NormalMoveUpdate(unsigned char _inputName, float _movePow, const ChLMat& _nowPosMatrix)
{
	if (_inputName > ChStd::EnumCast(InputName::Jump))return;

	if (inputFlgs.GetBitFlg(ChStd::EnumCast(InputName::Avo))) inputFlgs.SetBitTrue(_inputName + ChStd::EnumCast(InputName::FrontAvo));

	else if (inputFlgs.GetBitFlg(ChStd::EnumCast(InputName::Boost))) inputFlgs.SetBitTrue(_inputName + ChStd::EnumCast(InputName::FrontBoost));

	moveVec += _nowPosMatrix.TransformCoord(GetDirectionVector(_inputName)) * _movePow;

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
		auto lookPos = tmpMat.Transform(ChVec3(0.0f, -5.0f, 5.0f));
		auto camPos = tmpMat.Transform(ChVec3(0.0f, 0.0f,-10.0f));

		camMat.CreateViewMatLookTarget(camPos, lookPos, ChVec3(0.0f, 1.0f, 0.0f));
	}

	drawer->drawer.SetViewMatrix(camMat);

}

void BaseMecha::BaseMove()
{

	pos += moveVec;
	rot -= rotateVec;

	moveVec *= 0.9f;
	rotateVec *= 0.5f;
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
