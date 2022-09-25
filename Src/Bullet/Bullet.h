#pragma once

class Bullet :public ChCpp::BaseObject
{
public://Serialize Deserialize\\

	void Deserialize(const std::string& _text);

	std::string Serialize();

private:

	//�����x//
	float firstSpeed = 0.0f;

	//�ђʗ�//
	float penetration = 0.0f;

	//�e�̃��f��//
	ChPtr::Shared<ChD3D11::Mesh11>bullet = ChPtr::Make_S<ChD3D11::Mesh11>();


};