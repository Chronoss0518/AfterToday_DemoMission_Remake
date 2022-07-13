#include"../ChDXLibrary/ChGameIncludeFile.h"
#include"../AllStruct.h"
#include"../HitPosList/HitPosList.h"
#include"../Damage/Damage.h"
#include"../BaseRobot/RobParts.h"
#include"../BaseRobot/BaseRobot.h"
#include"PlayerObj.h"

void PlayerObj::Init(unsigned short _PartsNum)
{

	//ChStd::COLOR1f Col(1.0f, 1.0f, 1.0f, 1.0f);

	ChMatrix_9 TmpMat;

	BaseD.EnemyFlg = ChStd::False;

	Mouse = std::make_shared<ChMouseController>(TRUE, &ChWinAPID);

	//Mouse->ShouFlg(ChStd::True);
	Mat.Identity();

	MoveD.TmpRot.x = 0.0f;
	MoveD.TmpRot.y = 0.0f;

	BaseD.Rot.x = 0.0f;
	BaseD.Rot.y = 0.0f;

	Col = ChStd::COLOR1f(1.0f, 0.0f, 0.0f, 1.0f);
	BInit(_PartsNum);
	AddDamage = 5;
	PartsSet();
}

void PlayerObj::Input()
{

	if (HP < 1)return;



	ChWinAPID.SetKeyCode();

	if (ChWinAPID.IsPushKeyNoHold(VK_RETURN))InputFlg = !InputFlg;

	if (InputFlg)
	{
		InputKeyData();
		return;
	}

	InputCPUData();


}

