
#include"../BaseIncluder.h"

#include"ParameterList.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"PartsParameters.h"
#include"ParameterPanel.h"
#include"ParameterDisplay.h"

#define MAIN_DATA_HARDNESS u8"硬さ"
#define MAIN_DATA_MASS u8"重量"

#define ENELGY_TANK_DATA_MAX_ENELGY u8"エネルギー回復速度"
#define ENELGY_TANK_DATA_CHARGE_ENELGY u8"エネルギー貯蓄量"

#define WALK_DATA_MOVE_POWER u8"移動力"
#define WALK_DATA_ROTATE_POWER u8"回転力"
#define WALK_DATA_JUMP_POWER u8"飛翔力"

#define BOOST_DATA_BOOST_USE_ENELGY u8"ブースト消費エネルギー"
#define BOOST_DATA_BOOST_POWER u8"ブースト推進力"
#define BOOST_DATA_AVOID_USE_ENELGY u8"クイックブースト消費エネルギー"
#define BOOST_DATA_AVOID_POWER u8"クイックブースト推進力"
#define BOOST_DATA_AVOID_WAIT u8"クイックブースト間隔"

#define FRONT_BOOST_DATA(_str) u8"前方" _str
#define BACK_BOOST_DATA(_str) u8"後方" _str
#define RIGHT_BOOST_DATA(_str) u8"右方" _str
#define LEFT_BOOST_DATA(_str) u8"左方" _str
#define UP_BOOST_DATA(_str) u8"上方" _str
#define DOWN_BOOST_DATA(_str) u8"下方" _str


#define WEAPON_DATA_WAIT_TIME u8"攻撃後の待機時間"

#define SWORD_DATA_DAMAGE_PAR_SPEED u8"速度に対するダメージ量"
#define SWORD_DATA_ATTACK_TIME u8"攻撃速度"

#define GUN_DATA_FIRE_NUM u8"一度に発車する弾の量"
#define GUN_DATA_BULLET_NUM u8"リロードでセットできる最大弾数"
#define GUN_DATA_MAGAZINE_NUM u8"リロード可能回数"
#define GUN_DATA_RELOAD_TIME u8"リロード時間"
#define GUN_DATA_RANGE u8"ぶれた際の最大角度"

#define ATTACK_DATA_PENETRATION u8"貫通力"
#define ATTACK_DATA_HIT_SIZE u8"ヒット範囲"

#define BULLET_DATA_FIRST_SPEED u8"弾の初速"

#define BOOST_BULLET_DATA_START_BOOST_TIME u8"ブースト開始時間"
#define BOOST_BULLET_DATA_USE_BOOST_TIME u8"ブースト持続時間"
#define BOOST_BULLET_DATA_BOOST_POWER u8"ブースト推進力"

#define HIGH_EXPLOSIVE_BULLET_DATA_BLAST_RANGE u8"爆発範囲"

#define MISSILE_DATA_ROTATE_SPEED u8"最大回転量"
#define MISSILE_DATA_LOST_RANGE u8"見失う角度"

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

void ParameterPartialDisplay::Init(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	emptyPanel = ParameterTitlePanel::CreatePanel(_device, _textDrawer, "");
	emptyPanel->SetBackGround(titleBGTexture);

	AddParameters(_device, _textDrawer, "Main Data");
	AddParameters(_device, _textDrawer, "Main Data",_titleDrawer, MAIN_DATA_HARDNESS, _valueDrawer, &baseParameter->mainData.hardness, &nextParameter->mainData.hardness,false);
	AddParameters(_device, _textDrawer, "Main Data",_titleDrawer, MAIN_DATA_MASS, _valueDrawer, &baseParameter->mainData.mass, &nextParameter->mainData.mass, true);

	AddParameters(_device, _textDrawer, "Enelgy Tank Data");
	AddParameters(_device, _textDrawer, "Enelgy Tank Data", _titleDrawer, ENELGY_TANK_DATA_MAX_ENELGY, _valueDrawer, &baseParameter->enelgyTankData.chargeEnelgy, &nextParameter->enelgyTankData.chargeEnelgy, false);
	AddParameters(_device, _textDrawer, "Enelgy Tank Data", _titleDrawer, ENELGY_TANK_DATA_CHARGE_ENELGY, _valueDrawer, &baseParameter->enelgyTankData.maxEnelgy, &nextParameter->enelgyTankData.maxEnelgy, false);

	AddParameters(_device, _textDrawer, "Walk Data");
	AddParameters(_device, _textDrawer, "Walk Data", _titleDrawer, WALK_DATA_MOVE_POWER, _valueDrawer, &baseParameter->walkData.movePower, &nextParameter->walkData.movePower,false);
	AddParameters(_device, _textDrawer, "Walk Data", _titleDrawer, WALK_DATA_ROTATE_POWER, _valueDrawer, &baseParameter->walkData.rotatePower, &nextParameter->walkData.rotatePower, false);
	AddParameters(_device, _textDrawer, "Walk Data", _titleDrawer, WALK_DATA_JUMP_POWER, _valueDrawer, &baseParameter->walkData.jumpPower, &nextParameter->walkData.jumpPower, false);

	AddParameters(_device, _textDrawer, "Front Boost Data");
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &baseParameter->frontBoostData.boostPower, &nextParameter->frontBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &baseParameter->frontBoostData.boostUseEnelgy, &nextParameter->frontBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &baseParameter->frontBoostData.avoidPower, &nextParameter->frontBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &baseParameter->frontBoostData.avoidUseEnelgy, &nextParameter->frontBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Front Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &baseParameter->frontBoostData.avoidWait, &nextParameter->frontBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Back Boost Data");
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &baseParameter->backBoostData.boostPower, &nextParameter->backBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &baseParameter->backBoostData.boostUseEnelgy, &nextParameter->backBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &baseParameter->backBoostData.avoidPower, &nextParameter->backBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &baseParameter->backBoostData.avoidUseEnelgy, &nextParameter->backBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Back Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &baseParameter->backBoostData.avoidWait, &nextParameter->backBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Right Boost Data");
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &baseParameter->rightBoostData.boostPower, &nextParameter->rightBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &baseParameter->rightBoostData.boostUseEnelgy, &nextParameter->rightBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &baseParameter->rightBoostData.avoidPower, &nextParameter->rightBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &baseParameter->rightBoostData.avoidUseEnelgy, &nextParameter->rightBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Right Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &baseParameter->rightBoostData.avoidWait, &nextParameter->rightBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Left Boost Data");
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &baseParameter->leftBoostData.boostPower, &nextParameter->leftBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &baseParameter->leftBoostData.boostUseEnelgy, &nextParameter->leftBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &baseParameter->leftBoostData.avoidPower, &nextParameter->leftBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &baseParameter->leftBoostData.avoidUseEnelgy, &nextParameter->leftBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Left Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &baseParameter->leftBoostData.avoidWait, &nextParameter->leftBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Up Boost Data");
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &baseParameter->upBoostData.boostPower, &nextParameter->upBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &baseParameter->upBoostData.boostUseEnelgy, &nextParameter->upBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &baseParameter->upBoostData.avoidPower, &nextParameter->upBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &baseParameter->upBoostData.avoidUseEnelgy, &nextParameter->upBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Up Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &baseParameter->upBoostData.avoidWait, &nextParameter->upBoostData.avoidWait, true);

	AddParameters(_device, _textDrawer, "Down Boost Data");
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_BOOST_USE_ENELGY, _valueDrawer, &baseParameter->downBoostData.boostPower, &nextParameter->downBoostData.boostPower, false);
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_BOOST_POWER, _valueDrawer, &baseParameter->downBoostData.boostUseEnelgy, &nextParameter->downBoostData.boostUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_AVOID_USE_ENELGY, _valueDrawer, &baseParameter->downBoostData.avoidPower, &nextParameter->downBoostData.avoidPower, false);
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_AVOID_POWER, _valueDrawer, &baseParameter->downBoostData.avoidUseEnelgy, &nextParameter->downBoostData.avoidUseEnelgy, true);
	AddParameters(_device, _textDrawer, "Down Boost Data", _titleDrawer, BOOST_DATA_AVOID_WAIT, _valueDrawer, &baseParameter->downBoostData.avoidWait, &nextParameter->downBoostData.avoidWait, true);

	UpdateCountPanels(_device, _textDrawer);

	baseParameterCount = panels.size();

	AddWeaponParameters(_device, _textDrawer, _titleDrawer, _valueDrawer);

	UpdateCountPanels(_device, _textDrawer);
}

void ParameterPartialDisplay::AddPanelList()
{
	auto&& countPanel =  ChPtr::Make_S<ParameterTitlePanel>();
	countPanels.push_back(countPanel);
	panels.push_back(countPanel);
	countPanel->SetBackGround(titleBGTexture);
}

void ParameterPartialDisplay::AddWeaponParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	for (auto&& weapon : nextParameter->weaponData)
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
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, SWORD_DATA_DAMAGE_PAR_SPEED, _valueDrawer, GetULWeaponParameter(), &param->damageParSpeed, false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, SWORD_DATA_ATTACK_TIME, _valueDrawer, GetULWeaponParameter(), &param->attackTime, false);
}

void ParameterPartialDisplay::AddGunParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto param = ChPtr::SharedSafeCast<PartsParameterStruct::GunData>(_parameterList);
	if (param == nullptr)return;
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_FIRE_NUM, _valueDrawer, GetULWeaponParameter() ,&param->fireNum,false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_BULLET_NUM, _valueDrawer, GetULWeaponParameter() ,&param->bulletNum, false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_MAGAZINE_NUM, _valueDrawer, GetULWeaponParameter(), &param->magazineNum, false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_RELOAD_TIME, _valueDrawer, GetULWeaponParameter(), &param->reloadTime, false);
	AddParameters(_device, _textDrawer, param->weaponName, _titleDrawer, GUN_DATA_RANGE, _valueDrawer, GetULWeaponParameter(), &param->range, false);
}

void ParameterPartialDisplay::AddAttackParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	for (auto&& attack : _parameterList->attackData)
	{
		AddParameters(_device, _textDrawer, attack->objectName);
		AddParameters(_device, _textDrawer, attack->objectName, _titleDrawer, ATTACK_DATA_PENETRATION, _valueDrawer, GetULWeaponParameter(), &attack->penetration, false);
		AddParameters(_device, _textDrawer, attack->objectName, _titleDrawer, ATTACK_DATA_HIT_SIZE, _valueDrawer, GetFWeaponParameter(), &attack->hitSize,false);

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
	AddParameters(_device, _textDrawer, _title, _titleDrawer, BULLET_DATA_FIRST_SPEED, _valueDrawer, GetFWeaponParameter(), &obj->firstSpeed, false);
}

void ParameterPartialDisplay::AddBoostBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::string& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto&& obj = ChPtr::SharedSafeCast<PartsParameterStruct::BoostBulletData>(_parameterList);
	if (obj == nullptr)return;
	AddParameters(_device, _textDrawer, _title, _titleDrawer, BOOST_BULLET_DATA_START_BOOST_TIME, _valueDrawer, GetULWeaponParameter(), &obj->startBoostTime,  false);
	AddParameters(_device, _textDrawer, _title, _titleDrawer, BOOST_BULLET_DATA_USE_BOOST_TIME, _valueDrawer, GetULWeaponParameter(), &obj->useBoostTime, false);
	AddParameters(_device, _textDrawer, _title, _titleDrawer, BOOST_BULLET_DATA_BOOST_POWER, _valueDrawer, GetFWeaponParameter(), &obj->boostPow, false);
}

void ParameterPartialDisplay::AddHighExplosiveBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::string& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto&& obj = ChPtr::SharedSafeCast<PartsParameterStruct::HighExplosiveBulletData>(_parameterList);
	if (obj == nullptr)return;
	AddParameters(_device, _textDrawer, _title, _titleDrawer, HIGH_EXPLOSIVE_BULLET_DATA_BLAST_RANGE, _valueDrawer, GetFWeaponParameter(), &obj->blastRange, false);
}

void ParameterPartialDisplay::AddMissileParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::string& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	auto&& obj = ChPtr::SharedSafeCast<PartsParameterStruct::MissileData>(_parameterList);
	if (obj == nullptr)return;
	AddParameters(_device, _textDrawer, _title, _titleDrawer, MISSILE_DATA_ROTATE_SPEED, _valueDrawer, GetFWeaponParameter(), &obj->rotateSpeed, false);
	AddParameters(_device, _textDrawer, _title, _titleDrawer, MISSILE_DATA_LOST_RANGE, _valueDrawer, GetFWeaponParameter(), &obj->lostRange, false);
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

void ParameterPartialDisplay::Update(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{
	for (unsigned long i = baseParameterCount - 1; i < panels.size(); i++)
	{
		panels.pop_back();
	}

	for (auto&& panel : panels)
	{
		auto&& valuePanels = ChPtr::SharedSafeCast<ParameterValuePanel>(panel);
		if (valuePanels == nullptr)continue;
		valuePanels->UpdateValue(_valueDrawer, _device);
	}

	AddWeaponParameters(_device,_textDrawer,_titleDrawer,_valueDrawer);

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

	if (countPanelsBeforNum == countPanels.size())return;

	for (unsigned long i = 0; i < countPanels.size(); i++)
	{
		auto&& countPanel = countPanels[i];
		std::string drawBase = "";
		drawBase += std::to_string(i + 1);
		drawBase += "/";
		drawBase += std::to_string(countPanels.size());
		countPanel.lock()->CreateTitle(_textDrawer, _device, drawBase);
	}
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

void ParameterEntireDisplay::Init(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
{

}

void ParameterEntireDisplay::Update(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
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
