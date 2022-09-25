#pragma once

class Bullet :public ChCpp::BaseObject
{
public://Serialize Deserialize\\

	void Deserialize(const std::string& _text);

	std::string Serialize();

private:

	//初速度//
	float firstSpeed = 0.0f;

	//貫通力//
	float penetration = 0.0f;

	//弾のモデル//
	ChPtr::Shared<ChD3D11::Mesh11>bullet = ChPtr::Make_S<ChD3D11::Mesh11>();


};