
#include"../BaseIncluder.h"

#include"CloudObj.h"

#define BASE_HIGH 300.0f
#define MAX_LEN 1000.0f

void CloudList::Init()
{
	UpdateMoveDirection();

	texture = ChPtr::Make_S<ChD3D11::Texture11>();

	texture->CreateTexture(TEXTURE_DIRECTORY("Cloud.png"));

	pobo.Init(ChD3D11::D3D11Device());

	//ChTexMa.SetTexture("Cloud.png", "Cloud", "Base", 128, 128, 128, 128);

	for (unsigned char i = 0; i < startCloudCnt; i++)
	{
		CloudBurth();
	}
}

void CloudList::Update()
{
	ChVec3 tmpVec, tmpPos;
	tmpVec = moveDir * moveSize;

	for (unsigned char i =0 ;i < cloudList.size();i++)
	{

		cloudList[i]->pos += (tmpVec);
		tmpPos = cloudList[i]->pos;

		if (ChVec3::GetLen(tmpPos,ChVec3(0.0f, BASE_HIGH, 0.0f)) < MAX_LEN)continue;
		cloudList.erase(cloudList.begin() + i);
		i--;

	}

	if ((std::rand() % 5) <= 1)CloudBurth();

	if (cloudList.size() >= maxCloufdCnt)cloudList.pop_back();

	++chengeCnt %= chengeIntervalCnt;

	if (chengeCnt != 0)return;
	if ((std::rand() % 100) < 90)return;

	UpdateMoveDirection();

}

void CloudList::Draw(ID3D11DeviceContext* _dc,ChD3D11::Shader::BaseDrawPolygonBoard11& _drawPoBo)
{

	ChLMat tmpMat = _drawPoBo.GetViewMatrix();

	for (auto obj : cloudList)
	{
		tmpMat.SetScalling(obj->scl);
		tmpMat.SetPosition(obj->pos);

		_drawPoBo.Draw(_dc, *texture, pobo, (DirectX::XMFLOAT4X4)tmpMat);
	}
}

void CloudList::CloudBurth()
{
	auto tmpObj = ChPtr::Make_S<CloudObject>();
	tmpObj->scl = (float)((std::rand() % 3) + 1.0f);

	
	tmpObj->pos = CreateVec();
	tmpObj->pos *= MAX_LEN * ((std::rand() % 100) * 0.01f);
	tmpObj->pos.y = BASE_HIGH;

	cloudList.push_back(tmpObj);
}

void CloudList::UpdateMoveDirection()
{

	moveDir = CreateVec();

	moveDir.Normalize();

	moveSize = ((std::rand() % 9) + 1.0f) * 0.05f;

}

ChVec3 CloudList::CreateVec()
{
	return ChVec3(
		((std::rand() % 3) * 2.0f - 2.0f)
		, 0.0f
		, ((std::rand() % 3) * 2.0f - 2.0f));;
}
