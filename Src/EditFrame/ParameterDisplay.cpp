
#include"../BaseIncluder.h"

#include"ParameterList.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"PartsParameters.h"
#include"ParameterPanel.h"
#include"ParameterDisplay.h"

#define MAIN_DATA_HARDNESS u8"�d��"
#define MAIN_DATA_MASS u8"�d��"

#define ENELGY_TANK_DATA_MAX_ENELGY u8"�G�l���M�[�񕜑��x"
#define ENELGY_TANK_DATA_CHARGE_ENELGY u8"�G�l���M�[���~��"

#define WALK_DATA_MOVE_POWER u8"�ړ���"
#define WALK_DATA_ROTATE_POWER u8"��]��"
#define WALK_DATA_JUMP_POWER u8"���ė�"

#define BOOST_DATA_BOOST_USE_ENELGY u8"�u�[�X�g����G�l���M�["
#define BOOST_DATA_BOOST_POWER u8"�u�[�X�g���i��"
#define BOOST_DATA_AVOID_USE_ENELGY u8"�N�C�b�N�u�[�X�g����G�l���M�["
#define BOOST_DATA_AVOID_POWER u8"�N�C�b�N�u�[�X�g���i��"
#define BOOST_DATA_AVOID_WAIT u8"�N�C�b�N�u�[�X�g�Ԋu"

#define FRONT_BOOST_DATA(_str) u8"�O��" _str
#define BACK_BOOST_DATA(_str) u8"���" _str
#define RIGHT_BOOST_DATA(_str) u8"�E��" _str
#define LEFT_BOOST_DATA(_str) u8"����" _str
#define UP_BOOST_DATA(_str) u8"���" _str
#define DOWN_BOOST_DATA(_str) u8"����" _str


#define WEAPON_DATA_WAIT_TIME u8"�U����̑ҋ@����"

#define SWORD_DATA_DAMAGE_PAR_SPEED u8"���x�ɑ΂���_���[�W��"
#define SWORD_DATA_ATTACK_TIME u8"�U�����x"

#define GUN_DATA_FIRE_NUM u8"��x�ɔ��Ԃ���e�̗�"
#define GUN_DATA_BULLET_NUM u8"�����[�h�ŃZ�b�g�ł���ő�e��"
#define GUN_DATA_MAGAZINE_NUM u8"�����[�h�\��"
#define GUN_DATA_RELOAD_TIME u8"�����[�h����"
#define GUN_DATA_RANGE u8"�Ԃꂽ�ۂ̍ő�p�x"

#define ATTACK_DATA_PENETRATION u8"�ђʗ�"
#define ATTACK_DATA_HIT_SIZE u8"�q�b�g�͈�"

#define BULLET_DATA_FIRST_SPEED u8"�e�̏���"

#define BOOST_BULLET_DATA_START_BOOST_TIME u8"�u�[�X�g�J�n����"
#define BOOST_BULLET_DATA_USE_BOOST_TIME u8"�u�[�X�g��������"
#define BOOST_BULLET_DATA_BOOST_POWER u8"�u�[�X�g���i��"

#define HIGH_EXPLOSIVE_BULLET_DATA_BLAST_RANGE u8"�����͈�"

#define MISSILE_DATA_ROTATE_SPEED u8"�ő��]��"
#define MISSILE_DATA_LOST_RANGE u8"�������p�x"

#define PARTIAL_PANEL_COUNT 22

#define ENTIRE_PANEL_HCOUNT 3
#define ENTIRE_PANEL_VCOUNT 22

#define ENTIRE_PANEL_VERTICAL_COUNT 19
#define ENTIRE_PANEL_HORIZONTAL_COUNT 3

#define PALAMETER_COUNT 37

#define PARTIAL_DISPLAY_START_X 970.0f
#define PARTIAL_DISPLAY_START_Y 30.0f

#define UP_TEXT_COLOR ChVec4::FromColor(1.0f,0.0f,0.0f,1.0f)
#define DOWN_TEXT_COLOR ChVec4::FromColor(0.0f,0.0f,1.0f,1.0f)

void ParameterPartialDisplay::Init(PartsParameters& _baseParameterList, PartsParameters& _nextParameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	emptyPanel = ParameterTitlePanel::CreatePanel(_device, _textDrawer, "");
	emptyPanel->SetBackGround(titleBGTexture);

	AddParameters(_device, _textDrawer, "Main Data");
	AddParameters(_device, _textDrawer, "Main Data",_titleDrawer, MAIN_DATA_HARDNESS, _valueDrawer, &_baseParameterList.mainData.hardness, &_nextParameterList.mainData.hardness,false);
	AddParameters(_device, _textDrawer, "Main Data",_titleDrawer, MAIN_DATA_MASS, _valueDrawer, &_baseParameterList.mainData.mass, &_nextParameterList.mainData.mass, true);

	AddParameters(_device, _textDrawer, "Enelgy Tank Data");
	AddParameters(_device, _textDrawer, "Enelgy Tank Data", _titleDrawer, ENELGY_TANK_DATA_MAX_ENELGY, _valueDrawer, &_baseParameterList.enelgyTankData.chargeEnelgy, &_nextParameterList.enelgyTankData.chargeEnelgy, false);
	AddParameters(_device, _textDrawer, "Enelgy Tank Data", _titleDrawer, ENELGY_TANK_DATA_CHARGE_ENELGY, _valueDrawer, &_baseParameterList.enelgyTankData.maxEnelgy, &_nextParameterList.enelgyTankData.maxEnelgy, false);

	AddParameters(_device, _textDrawer, "Walk Data");
	AddParameters(_device, _textDrawer, "Walk Data", _titleDrawer, WALK_DATA_MOVE_POWER, _valueDrawer, &_baseParameterList.walkData.movePower, &_nextParameterList.walkData.movePower,false);
	AddParameters(_device, _textDrawer, "Walk Data", _titleDrawer, WALK_DATA_ROTATE_POWER, _valueDrawer, &_baseParameterList.walkData.rotatePower, &_nextParameterList.walkData.rotatePower, false);
	AddParameters(_device, _textDrawer, "Walk Data", _titleDrawer, WALK_DATA_JUMP_POWER, _valueDrawer, &_baseParameterList.walkData.jumpPower, &_nextParameterList.walkData.jumpPower, false);

	AddParameters(_device, _textDrawer, "Front Boost Data");
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.boostPower, &_nextParameterList.frontBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &_baseParameterList.frontBoostData.boostUseEnelgy, &_nextParameterList.frontBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.avoidPower, &_nextParameterList.frontBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &_baseParameterList.frontBoostData.avoidUseEnelgy, &_nextParameterList.frontBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &_baseParameterList.frontBoostData.avoidWait, &_nextParameterList.frontBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Back Boost Data");
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.boostPower, &_nextParameterList.frontBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &_baseParameterList.frontBoostData.boostUseEnelgy, &_nextParameterList.frontBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.avoidPower, &_nextParameterList.frontBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &_baseParameterList.frontBoostData.avoidUseEnelgy, &_nextParameterList.frontBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &_baseParameterList.frontBoostData.avoidWait, &_nextParameterList.frontBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Right Boost Data");
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.boostPower, &_nextParameterList.frontBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &_baseParameterList.frontBoostData.boostUseEnelgy, &_nextParameterList.frontBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.avoidPower, &_nextParameterList.frontBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &_baseParameterList.frontBoostData.avoidUseEnelgy, &_nextParameterList.frontBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &_baseParameterList.frontBoostData.avoidWait, &_nextParameterList.frontBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Left Boost Data");
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.boostPower, &_nextParameterList.frontBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &_baseParameterList.frontBoostData.boostUseEnelgy, &_nextParameterList.frontBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.avoidPower, &_nextParameterList.frontBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &_baseParameterList.frontBoostData.avoidUseEnelgy, &_nextParameterList.frontBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &_baseParameterList.frontBoostData.avoidWait, &_nextParameterList.frontBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Up Boost Data");
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.boostPower, &_nextParameterList.frontBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &_baseParameterList.frontBoostData.boostUseEnelgy, &_nextParameterList.frontBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.avoidPower, &_nextParameterList.frontBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &_baseParameterList.frontBoostData.avoidUseEnelgy, &_nextParameterList.frontBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &_baseParameterList.frontBoostData.avoidWait, &_nextParameterList.frontBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Down Boost Data");
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.boostPower, &_nextParameterList.frontBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &_baseParameterList.frontBoostData.boostUseEnelgy, &_nextParameterList.frontBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &_baseParameterList.frontBoostData.avoidPower, &_nextParameterList.frontBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &_baseParameterList.frontBoostData.avoidUseEnelgy, &_nextParameterList.frontBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &_baseParameterList.frontBoostData.avoidWait, &_nextParameterList.frontBoostData.avoidWait, true);

	baseParameterCount = panels.size();

	AddWeaponParameters(_nextParameterList, _device, _textDrawer, _titleDrawer, _valueDrawer);

	UpdateCountPanels(_device,_textDrawer);

}

void ParameterPartialDisplay::AddPanelList()
{
	auto&& countPanel =  ChPtr::Make_S<ParameterTitlePanel>();
	countPanels.push_back(countPanel);
	panels.push_back(countPanel);
	countPanel->SetBackGround(titleBGTexture);
}

void ParameterPartialDisplay::AddWeaponParameters(PartsParameters& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	for (auto&& weapon : _parameterList.weaponData)
	{
		AddParameters(_device, _textDrawer, weapon->weaponName);
		AddParameters(_device, _textDrawer, weapon->weaponName, _titleDrawer, WEAPON_DATA_WAIT_TIME, _valueDrawer, &weapon->waitTime, &weapon->waitTime, true);

		AddSwordParameters(weapon, _device, _textDrawer, _titleDrawer, _valueDrawer);
		AddGunParameters(weapon, _device, _textDrawer, _titleDrawer, _valueDrawer);

		AddAttackParameters(weapon, _device, _textDrawer, _titleDrawer, _valueDrawer);
	}
}

void ParameterPartialDisplay::AddSwordParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto param = ChPtr::SharedSafeCast<PartsParameterStruct::SwordData>(_parameterList);
	if (param == nullptr)return;
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, SWORD_DATA_DAMAGE_PAR_SPEED, _valueDrawer, &param->damageParSpeed, GetULWeaponParameter(), false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, SWORD_DATA_ATTACK_TIME, _valueDrawer, &param->attackTime, GetULWeaponParameter(), false);
}

void ParameterPartialDisplay::AddGunParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto param = ChPtr::SharedSafeCast<PartsParameterStruct::GunData>(_parameterList);
	if (param == nullptr)return;
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_FIRE_NUM, _valueDrawer, &param->fireNum, GetULWeaponParameter(),false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_BULLET_NUM, _valueDrawer, &param->bulletNum, GetULWeaponParameter(), false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_MAGAZINE_NUM, _valueDrawer, &param->magazineNum, GetULWeaponParameter(), false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_RELOAD_TIME, _valueDrawer, &param->reloadTime, GetULWeaponParameter(), false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_RANGE, _valueDrawer, &param->range, GetULWeaponParameter(), false);
}

void ParameterPartialDisplay::AddAttackParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	for (auto&& attack : _parameterList->attackData)
	{
		AddParameters(_device, _textDrawer, attack->objectName);
		AddParameters(_device, _textDrawer, attack->objectName, _titleDrawer, ATTACK_DATA_PENETRATION, _valueDrawer, &attack->penetration, GetULWeaponParameter(),false);
		AddParameters(_device, _textDrawer, attack->objectName, _titleDrawer, ATTACK_DATA_HIT_SIZE, _valueDrawer, &attack->hitSize, GetFWeaponParameter(),false);

		for (auto&& attackObj : attack->attackDataBase)
		{
			AddBulletParameters(attackObj, attack->objectName, _device, _textDrawer, _titleDrawer, _valueDrawer);
			AddBoostBulletParameters(attackObj, attack->objectName, _device, _textDrawer, _titleDrawer, _valueDrawer);
			AddHighExplosiveBulletParameters(attackObj, attack->objectName, _device, _textDrawer, _titleDrawer, _valueDrawer);
			AddMissileParameters(attackObj, attack->objectName, _device, _textDrawer, _titleDrawer, _valueDrawer);
		}
	}
}

void ParameterPartialDisplay::AddBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::string& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto&& obj = ChPtr::SharedSafeCast<PartsParameterStruct::BulletData>(_parameterList);
	if (obj == nullptr)return;
	AddParameters(_device, _textDrawer, _title, _titleDrawer, BULLET_DATA_FIRST_SPEED, _valueDrawer, &obj->firstSpeed, GetFWeaponParameter(),false);
}

void ParameterPartialDisplay::AddBoostBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::string& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto&& obj = ChPtr::SharedSafeCast<PartsParameterStruct::BoostBulletData>(_parameterList);
	if (obj == nullptr)return;
	AddParameters(_device, _textDrawer, _title, _titleDrawer, BOOST_BULLET_DATA_START_BOOST_TIME, _valueDrawer, &obj->startBoostTime, GetULWeaponParameter(), false);
	AddParameters(_device, _textDrawer, _title, _titleDrawer, BOOST_BULLET_DATA_USE_BOOST_TIME, _valueDrawer, &obj->useBoostTime, GetULWeaponParameter(),false);
	AddParameters(_device, _textDrawer, _title, _titleDrawer, BOOST_BULLET_DATA_BOOST_POWER, _valueDrawer, &obj->boostPow, GetFWeaponParameter(),false);
}

void ParameterPartialDisplay::AddHighExplosiveBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::string& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto&& obj = ChPtr::SharedSafeCast<PartsParameterStruct::HighExplosiveBulletData>(_parameterList);
	AddParameters(_device, _textDrawer, _title, _titleDrawer, HIGH_EXPLOSIVE_BULLET_DATA_BLAST_RANGE, _valueDrawer, &obj->blastRange, GetFWeaponParameter(),false);
	if (obj == nullptr)return;
}

void ParameterPartialDisplay::AddMissileParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::string& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto&& obj = ChPtr::SharedSafeCast<PartsParameterStruct::MissileData>(_parameterList);
	if (obj == nullptr)return;
	AddParameters(_device, _textDrawer, _title, _titleDrawer, MISSILE_DATA_ROTATE_SPEED, _valueDrawer, &obj->rotateSpeed, GetFWeaponParameter(),false);
	AddParameters(_device, _textDrawer, _title, _titleDrawer, MISSILE_DATA_LOST_RANGE, _valueDrawer, &obj->lostRange, GetFWeaponParameter(),false);
}

void ParameterPartialDisplay::AddParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::string& _title)
{

	if (PARTIAL_PANEL_COUNT - (panels.size() % PARTIAL_PANEL_COUNT) <= 1)
	{
		AddPanelList();
	}

	if (PARTIAL_PANEL_COUNT - (panels.size() % PARTIAL_PANEL_COUNT) <= 2)
	{
		panels.push_back(emptyPanel);

		AddPanelList();
	}
	auto&& panel = ParameterTitlePanel::CreatePanel(_device, _textDrawer, _title);
	panel->SetBackGround(titleBGTexture);
	panels.push_back(panel);

}

void ParameterPartialDisplay::AddParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::string& _title, TextDrawerWICBitmap& _titleDrawer, const std::string& _itemName, TextDrawerWICBitmap& _valueDrawer, unsigned long* _baseValue, unsigned long* _nextValue, bool _inversOperatorFlg)
{

	auto&& panel = ParameterValuePanel::CreatePanel(_device, _titleDrawer, _itemName, _valueDrawer, _baseValue, _nextValue, _inversOperatorFlg);
	panel->SetBackGround(valueBGTexture);
	panels.push_back(panel);

	if (PARTIAL_PANEL_COUNT - (panels.size() % PARTIAL_PANEL_COUNT) <= 1)
	{
		AddPanelList();

		auto&& panel = ParameterTitlePanel::CreatePanel(_device, _textDrawer, _title);
		panel->SetBackGround(titleBGTexture);
		panels.push_back(panel);
	}
}

void ParameterPartialDisplay::AddParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::string& _title, TextDrawerWICBitmap& _titleDrawer, const std::string& _itemName, TextDrawerWICBitmap& _valueDrawer, float* _baseValue, float* _nextValue, bool _inversOperatorFlg)
{

	auto&& panel = ParameterValuePanel::CreatePanel(_device, _titleDrawer, _itemName, _valueDrawer, _baseValue, _nextValue, _inversOperatorFlg);
	panel->SetBackGround(valueBGTexture);
	panels.push_back(panel);

	if (PARTIAL_PANEL_COUNT - (panels.size() % PARTIAL_PANEL_COUNT) <= 1)
	{
		AddPanelList();

		auto&& panel = ParameterTitlePanel::CreatePanel(_device, _textDrawer, _title);
		panel->SetBackGround(titleBGTexture);
		panels.push_back(panel);
	}
}

void ParameterPartialDisplay::Update(PartsParameters& _parameters, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	for (unsigned long i = 0; i < baseParameterCount; i++)
	{
		panels.pop_back();
	}

	for (auto&& panel : panels)
	{
		auto&& valuePanels = ChPtr::SharedSafeCast<ParameterValuePanel>(panel);
		if (valuePanels == nullptr)continue;
		valuePanels->UpdateValue(_textDrawer, _device);
	}

	AddWeaponParameters(_parameters,_device,_textDrawer,_titleDrawer,_valueDrawer);

	UpdateCountPanels(_device, _textDrawer);

}

void ParameterPartialDisplay::UpdateCountPanels(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer)
{
	unsigned long pushPanelCount = panels.size() % PARTIAL_PANEL_COUNT;
	if (pushPanelCount != 0)
	{
		for (unsigned long i = pushPanelCount; i < PARTIAL_PANEL_COUNT - 1; i++)
		{
			panels.push_back(emptyPanel);

		}

		AddPanelList();
	}

	for (auto&& it = countPanels.begin();it != countPanels.end();it)
	{
		if (it->expired())
		{
			it = countPanels.erase(it);
			continue;
		}

		it++;
	}

	for (unsigned long i = countPanelsBeforNum; i < countPanels.size(); i++)
	{
		auto&& countPanel = countPanels[i];
		std::string drawBase = "";
		drawBase += std::to_string(i + 1);
		drawBase += "/";
		drawBase += std::to_string(countPanels.size());
		countPanel.lock()->CreateTitle(_textDrawer, _device, drawBase);
	}
	countPanelsBeforNum = countPanels.size();
}

ChVec2 ParameterPartialDisplay::GetDrawStartPosition()
{
	return ChVec2(PARTIAL_DISPLAY_START_X, PARTIAL_DISPLAY_START_Y);
}

ChVec2 ParameterPartialDisplay::GetDrawPanelSize()
{
	return ChVec2::FromSize(PARAMETER_PANEL_WIDTH, PARAMETER_PANEL_HEIGHT * PARTIAL_PANEL_COUNT);
}

void ParameterPartialDisplay::Up()
{
	drawPosition = (drawPosition + 1) % panels.size();
}

void ParameterPartialDisplay::Down()
{
	drawPosition = (drawPosition + panels.size() - 1) % panels.size();
}

void ParameterPartialDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)
{

	unsigned long drawStarPoint = (PARTIAL_PANEL_COUNT * drawPosition) % panels.size();

	ChVec2 startPosition = GetDrawStartPosition();
	
	for (unsigned long i = 0; i < PARTIAL_PANEL_COUNT; i++)
	{
		panels[drawStarPoint + i]->Draw(_drawer,startPosition);

		startPosition.y += PARAMETER_PANEL_HEIGHT;
	}
}

void ParameterEntireDisplay::Init(PartsParameters& _baseParameterList, PartsParameters& _nextParameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{

}

void ParameterEntireDisplay::Update(PartsParameters& _parameters, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{

}

ChVec2 ParameterEntireDisplay::GetDrawStartPosition()
{
	return ChVec2(PARTIAL_DISPLAY_START_X - (PARAMETER_PANEL_WIDTH * (ENTIRE_PANEL_HCOUNT - 1)), PARTIAL_DISPLAY_START_Y);
}

ChVec2 ParameterEntireDisplay::GetDrawPanelSize()
{
	return ChVec2::FromSize(PARAMETER_PANEL_WIDTH * ENTIRE_PANEL_HCOUNT, PARAMETER_PANEL_HEIGHT * ENTIRE_PANEL_VCOUNT);
}

void ParameterEntireDisplay::Up()
{

}

void ParameterEntireDisplay::Down()
{

}

void ParameterEntireDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)
{

}