#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

#include"../AllStruct.h"
#include"../HitPosList/HitPosList.h"
#include"../Damage/Damage.h"
#include"../BaseRobot/RobParts.h"
#include"../BaseRobot/BaseRobot.h"
#include"PlayerObj.h"

void PlayerObj::Init(unsigned short _PartsNum)
{

	//ChStd::COLOR1f Col(1.0f, 1.0f, 1.0f, 1.0f);

	ChLMat tmpMat;

	BaseD.EnemyFlg = false;

	//Mouse->ShouFlg(ChStd::True);
	Mat.Identity();

	MoveD.tmpRot.x = 0.0f;
	MoveD.tmpRot.y = 0.0f;

	BaseD.Rot.x = 0.0f;
	BaseD.Rot.y = 0.0f;

	Col = ChVec4(1.0f, 0.0f, 0.0f, 1.0f);
	BInit(_PartsNum);
	AddDamage = 5;
	PartsSet();
}

void PlayerObj::Input()
{

	if (HP < 1)return;

	if (ChSystem::SysManager().IsPushKeyNoHold(VK_RETURN))InputFlg = !InputFlg;

	if (InputFlg)
	{
		InputKeyData();
		return;
	}

	InputCPUData();


}

