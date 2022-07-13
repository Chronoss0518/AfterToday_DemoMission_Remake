#pragma once

class CloudObj;

class CloudList
{
public:

	void Init();

	void Update();

	void Draw();

private:

	void CloudBurth();

	ChVec3_9 MoveDir;
	float MoveSize;
	const unsigned char ChengeIntervalCnt = 100;
	unsigned char ChengeCnt = 0;


	const unsigned char StartCloudCnt = 10;
	const unsigned char MaxCloufdCnt = 50;


	std::vector<ChPtr::Shared<CloudObj>> Cloud_List;


};


class CloudObj
{
public:

	CloudObj(){}

	~CloudObj(){}

	void Init();

	inline const ChVec3_9* GetPos() { return &Pos; }

	void Move(const ChVec3_9* _Move);

	void Draw();

private:

	float Scl = 1.0f;
	ChVec3_9 Pos;

	std::string TextureName = "Cloud";

};
