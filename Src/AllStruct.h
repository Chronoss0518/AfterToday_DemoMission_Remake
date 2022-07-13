
#ifndef _AllStruct
#define _AllStruct

///////////////////////////////////////////////////////////////////////////////////////
//Struct
///////////////////////////////////////////////////////////////////////////////////////

namespace ChTitle
{


}

namespace ChGame
{
	//WeaponsData//
	struct WeapData
	{
		unsigned char Damage;
		ChVec3_9 ShotPos;
		float HitLen;
		float Speed;
		std::string SEName;
	};

	//AttackPosData//
	struct APosData
	{
		ChVec3_9 Pos;
		ChVec3_9 Dir;
		WeapData Base;

	};

}





#endif