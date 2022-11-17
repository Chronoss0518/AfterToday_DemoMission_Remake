#pragma once

class CloudList
{
public:

	void Init();

	void Update();

	void Draw(ID3D11DeviceContext* _dc,ChD3D11::Shader::BaseDrawPolygonBoard11& _drawPoBo);

private:

	void CloudBurth();

	void UpdateMoveDirection();

	ChVec3 CreateVec();

	ChVec3 moveDir;
	float moveSize;
	const unsigned char chengeIntervalCnt = 100;
	unsigned char chengeCnt = 0;

	const unsigned char startCloudCnt = 10;
	const unsigned char maxCloufdCnt = 50;

	struct CloudObject
	{
		float scl = 1.0f;
		ChVec3 pos;
	};

	std::vector<ChPtr::Shared<CloudObject>> cloudList;
	
	ChD3D11::PolygonBoard11 pobo;

	ChPtr::Shared<ChD3D11::Texture11>texture = nullptr;

};
