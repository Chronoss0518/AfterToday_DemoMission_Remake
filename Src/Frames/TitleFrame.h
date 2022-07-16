#pragma once

class TitleFrame :public ChCpp::BaseFrame
{
public:

	void Init()override;

	void Release()override;

	void Frame()override;

private:

	void SetScript();

	void Draw();

	void Update();

	enum class Mesh :ChStd::DataNo
	{
		MSD
		, Desk
		, Room
	};

	enum class Tex :ChStd::DataNo
	{
		Title1
		, Title2
		, TitleName
		, Push_Space
	};

	unsigned char BackCol;
	const unsigned char MoveCol = 5;
	ChStd::Bool UpDownFlg = false;

	unsigned char DeviceTexChenge = 0;
	unsigned char ChengeCount = 20;

	ChCpp::ChScCon Script;

	ChAnimationObject9 Animations;

	ChTex::TextureList9 TexList;

	ChMesh::ChMeshList9 MeshList;

	ChTex::ChRTList9 RTList;

	ChPtr::Shared<ChCamera9> Cam;

	ChPtr::Shared<ChLight9> Light;

	ChPtr::Shared<ChMouCon>Mouse;
	ChVec3_9 MousePos;

};
