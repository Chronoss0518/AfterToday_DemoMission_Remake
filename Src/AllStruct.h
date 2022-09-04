
#ifndef _AllStruct
#define _AllStruct

#ifdef MESH_DIRECTORY
#define ROBOT_PARTS_DIRECTORY(current_path) MESH_DIRECTORY("RobotsParts/") current_path
#define WEAPOIN_DIRECTORY(current_path) ROBOT_PARTS_DIRECTORY("Weapon/") current_path
#endif


namespace ChBattle
{

	//WeaponsData//
	struct WeapData
	{
		unsigned char Damage;
		ChVec3 ShotPos;
		float HitLen;
		float Speed;
		std::string SEName;
	};

	//AttackPosData//
	struct APosData
	{
		ChVec3 Pos;
		ChVec3 Dir;
		WeapData Base;

	};

}


#endif