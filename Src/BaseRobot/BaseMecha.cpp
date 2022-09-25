
#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../HitPosList/HitPosList.h"
#include"../Damage/Damage.h"

#include"MechaParts.h"
#include"BaseMecha.h"
#include"../PlayerObj/PlayerObj.h"
#include"../EnemyObj/EnemyObj.h"


BaseMecha::BaseMecha()
{
	mechasNo = GetList().size();
	GetList().push_back(this);
}

BaseMecha::~BaseMecha()
{
	auto&& list = GetList();
	auto&& it = std::find(list.begin(), list.end(), this);
	list.erase(it);
}

void BaseMecha::Create(ID3D11Device* _device, unsigned long _w, unsigned long _h)
{
	auto&& system = *ChSystem::SysManager().GetSystem<ChSystem::Windows>();


}

void BaseMecha::Release()
{

}

void BaseMecha::Update()
{

}

void BaseMecha::Move()
{

}

void BaseMecha::Draw2D()
{
	if (mechaNo != GetMechaCamNo())return;



}

void BaseMecha::Draw3D()
{
	partsList.ObjectDraw3D();
}

void BaseMecha::Deserialize(const std::string& _desirialize)
{
	
}

std::string BaseMecha::Serialize()
{

}
