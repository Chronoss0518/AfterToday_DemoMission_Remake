#include"../BaseIncluder.h"

#include"../Shader/EffectObject/EffectObjectShader.h"
#include"../Shader/EffectSprite/EffectSpriteShader.h"

#include"../AllStruct.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/FunctionComponent/WeaponComponents.h"
#include"../Attack/AttackObject.h"
#include"../Attack/Attack.h"
#include"../GameScript/GameScript.h"
#include"../WeaponDataDrawUI/WeaponDataDrawUI.h"
#include"GameFrame.h"

#include"../BaseMecha/Controller/ControllerBase.h"

#include"../EffectObjects/ShotEffectList/ShotEffectList.h"
#include"../EffectObjects/SmokeEffectList/SmokeEffectList.h"

#include"../BaseMecha/CPU/CPULooker.h"

#include"../GameInMessageBox/GameInMessageBox.h"

//想定するメカオブジェクトの最大数//
#define MAX_MECHA_OBJECT_COUNT 10

#define GRAVITY_POWER 9.8f
#define DEBUG_FLG false

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY(L"RobotParts/" current_path)
#endif

#ifndef FIELD_DIRECTORY
#define FIELD_DIRECTORY(current_path) MESH_DIRECTORY(L"Field/" current_path)
#endif

#define INIT_SMOKE_DISPERSAL_POWER 5.0f
#define INIT_SMOKE_ALPHA_POWER 0.5f

#define DISPLAY_FPS_FLG false
#define DISPLAY_NOW_BULLET_NUM_FLG 0

//SF = Success and Failed
#define SF_MESSAGE_AFTER_FRAME static_cast<unsigned long>(BASE_FPS * 1.5f)
#define SF_MESSAGE_ADD_MESSAGE_FRAME static_cast<unsigned long>(BASE_FPS * 0.125f)

#define SUCCESS_PAUSE_COUNT static_cast<unsigned long>(BASE_FPS * 5.0f)

#define HIT_ICON_TOP 209.0f
#define HIT_ICON_LEFT 565.0f
#define HIT_ICON_BOTTOM HIT_ICON_TOP + 50.0f
#define HIT_ICON_RIGHT HIT_ICON_LEFT + 150.0f

#define CENTER_UI_TOP 110.0f
#define CENTER_UI_LEFT 390.0f
#define CENTER_UI_SIZE 500.0f
#define CENTER_UI_BOTTOM CENTER_UI_TOP + CENTER_UI_SIZE
#define CENTER_UI_RIGHT CENTER_UI_LEFT + CENTER_UI_SIZE

#define SHOT_TARGET_MARKER_SIZE 10.0f
#define SHOT_TARGET_DISTANCE 5.0f

#define ENEMY_TARGET_MARKER_SIZE 112.0f
#define ENEMY_TARGET_RANGE_COEFFICIENT 0.8f

///////////////////////////////////////////////////////////////////////////////////////
//Gameメソッド
///////////////////////////////////////////////////////////////////////////////////////

void GameFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	std::wstring stageName = L"stage1.chs";
	auto&& sendData = ChPtr::SharedSafeCast<StageDataStructure>(_sendData);
	if (sendData != nullptr)stageName = sendData->stageScriptPath;

	auto&& mouse = ChWin::Mouse();
	mouse.SetVisibleFlg(false);
	mouse.SetCenterFixedFlg(true);

	resultData = sendData->resultData;

	ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 0.0f, 1.0f));

	script = ChPtr::Make_S<GameScript>();
	InitScriptFunction();
	ChSystem::SysManager().SetFPS(BASE_FPS);

	PhysicsMachine::SetFPS(BASE_FPS);
	PhysicsMachine::SetGravityAcceleration(GRAVITY_POWER);
	PhysicsMachine::SetAirRegist(0.2f);

	auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();

#if DISPLAY_FPS_FLG | DISPLAY_NOW_BULLET_NUM_FLG
	box.Create(L"Text", ChINTPOINT(100, 0), ChINTPOINT(1000, 100), windows->GetWindObject());
	box.SetEnableFlg(false);
#endif

	meshDrawer.Init(ChD3D11::D3D11Device());
	meshDrawer.SetCullMode(D3D11_CULL_BACK);
	meshDrawer.SetAlphaBlendFlg(true);

	lightBloomeDrawer.Init(ChD3D11::D3D11Device());
	lightBloomeDrawer.SetBlurPower(5);
	lightBloomeDrawer.SetBoostPower(5.0f);
	lightBloomeDrawer.SetGameWindowSize(ChVec2(GAME_WINDOW_WIDTH * 0.5f, GAME_WINDOW_HEIGHT * 0.5f));
	lightBloomeDrawer.SetLiteBlurFlg(false);

	shotTargetDrawer.Init(ChD3D11::D3D11Device());
	shotTargetDrawer.SetCullMode(D3D11_CULL_BACK);
	shotTargetDrawer.SetAlphaBlendFlg(true);
	
	shotTargetBorad.Init(ChD3D11::D3D11Device());
	shotTargetBorad.SetInitSquare();

	ChVec4 rect = ChVec4::FromRect((GAME_SPRITE_WIDTH - SHOT_TARGET_MARKER_SIZE) * 0.5f, (GAME_SPRITE_HEIGHT - SHOT_TARGET_MARKER_SIZE) * 0.5f, (GAME_SPRITE_WIDTH + SHOT_TARGET_MARKER_SIZE) * 0.5f, (GAME_SPRITE_HEIGHT + SHOT_TARGET_MARKER_SIZE) * 0.5f);
	rect = RectToGameWindow(rect);

	for (unsigned char i = 0; i < 4; i++)
	{
		ChVec3 pos = shotTargetBorad.GetPos(i);
		pos.z = SHOT_TARGET_DISTANCE;
		shotTargetBorad.SetPos(i, pos);
	}

	shotTargetMarkerTex.CreateTexture(TEXTURE_DIRECTORY(L"ATKCurrsol.png"), ChD3D11::D3D11Device());


	waterSplashEffectShader = ChPtr::Make_S<EffectObjectShader>();
	waterSplashEffectShader->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 4);

	fireShader = ChPtr::Make_S<EffectObjectShader>();
	fireShader->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 4);
	fireShader->SetEffectTexture(TEXTURE_DIRECTORY(L""), 1, 1);

	shotEffectList = ChPtr::Make_S<ShotEffectList>();
	shotEffectList->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 10);

	smokeEffectList = ChPtr::Make_S<SmokeEffectList>();
	smokeEffectList->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 100, GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	smokeEffectList->SetMaxColorPower(0.5f);
	smokeEffectList->SetMinColorPower(0.3f);
	smokeEffectList->SetDownSpeedOnAlphaValue(0.01f);
	smokeEffectList->SetInitialDispersalPower(1.0f);

	enemyMarkerShader = ChPtr::Make_S<EffectSpriteShader>();
	enemyMarkerShader->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT);
	enemyMarkerShader->SetEffectTexture(TEXTURE_DIRECTORY(L"Ts.png"),1,1);
	enemyMarkerShader->SetObjectSize(ChVec2(ENEMY_TARGET_MARKER_SIZE / GAME_WINDOW_WIDTH, ENEMY_TARGET_MARKER_SIZE / GAME_WINDOW_HEIGHT));
	for (unsigned long i = 0; i < MAX_MECHA_OBJECT_COUNT; i++)
	{
		enemyMarkerShader->SetEffectColor(ChVec4(1.0f), i);
	}

	light.Init(ChD3D11::D3D11Device());
	light.SetUseLightFlg(true);
	light.SetDirectionLightData(true, ChVec3(1.0f), ChVec3(0.0f, -1.0f, 0.0f), 0.3f);

	centerUITexture.CreateTexture(TEXTURE_DIRECTORY(L"BattleBarUI/BattleBarFrame.png"));
	receveDamageUITexture.CreateTexture(TEXTURE_DIRECTORY(L"BattleBarUI/BattleBar_Damage.png"));
	enelgyUITexture.CreateTexture(TEXTURE_DIRECTORY(L"BattleBarUI/BattleBar_Enelgy.png"));

	gageDrawer.Init(ChD3D11::D3D11Device());

	gageDrawer.SetStartDrawDir(ChVec2(0.0f, -1.0f));
	uiDrawer.Init(ChD3D11::D3D11Device());
	uiDrawer.SetAlphaBlendFlg(true);

	centerUISprite.SetInitPosition();
	centerUISprite.SetPosRect(RectToGameWindow(ChVec4::FromRect(CENTER_UI_LEFT, CENTER_UI_TOP, CENTER_UI_RIGHT, CENTER_UI_BOTTOM)));
	centerUISprite.SetUVPos(0, ChVec2(0.0f, 1.0f));
	centerUISprite.SetUVPos(1, ChVec2(1.0f, 1.0f));
	centerUISprite.SetUVPos(2, ChVec2(1.0f, 0.0f));
	centerUISprite.SetUVPos(3, ChVec2(0.0f, 0.0f));

	testTextureSprite.Init();
	testTextureSprite.SetPosRect(ChVec4::FromRect(0.1f,-0.1f,0.9f,-0.9f));

	hitIcon.sprite.Init();
	hitIcon.sprite.SetPosRect(RectToGameWindow(ChVec4::FromRect(HIT_ICON_LEFT, HIT_ICON_TOP, HIT_ICON_RIGHT, HIT_ICON_BOTTOM)));
	hitIcon.image.CreateTexture(TEXTURE_DIRECTORY(L"HitIcon.png"), ChD3D11::D3D11Device());

	{
		ChMat_11 proMat;
		proMat.CreateProjectionMat(ChMath::ToRadian(60.0f), GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, GAME_PROJECTION_NEAR, GAME_PROJECTION_FAR);

		projectionMat = proMat;

		meshDrawer.SetProjectionMatrix(proMat);
		shotEffectList->SetProjectionMatrix(proMat);
		smokeEffectList->SetProjectionMatrix(proMat);
		shotTargetDrawer.SetProjectionMatrix(proMat);
	}

	LoadStage(stageName);


	messageBox = ChPtr::Make_S<GameInMessageBox>();
	messageBox->Init(ChD3D11::D3D11Device());

	weaponDataDrawer = ChPtr::Make_S<WeaponDataDrawUI>();
	weaponDataDrawer->Init(ChD3D11::D3D11Device());
	
	rt2D.CreateRenderTarget(ChD3D11::D3D11Device(),GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	rt3D.CreateRenderTarget(ChD3D11::D3D11Device(), GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	rtHighLightMap.CreateRenderTarget(ChD3D11::D3D11Device(), GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	dsTex.CreateDepthBuffer(ChD3D11::D3D11Device(), GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	fadeOutTexture.CreateColorTexture(ChD3D11::D3D11Device(), ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f), 1, 1);

	uiSprite.Init(); 
	uiSprite.SetInitPosition();

}

void GameFrame::InitScriptFunction()
{
	script->SetFunction(L"Initialize", [&](const std::wstring& _text) {initFlg = true; });

	script->SetFunction(L"LoadBGM", [&](const std::wstring& _text) {
		AddBGM(_text);
		});

	script->SetFunction(L"Play", [&](const std::wstring& _text) {
		auto argment = ChStr::Split<wchar_t>(_text, L" ");

		if (nowPlayAudio != L"")audios[nowPlayAudio]->Stop();

		nowPlayAudio = argment[0];
		audios[nowPlayAudio]->Play();
		});

	script->SetFunction(L"Stop", [&](const std::wstring& _text) {
		audios[nowPlayAudio]->Stop();
		});

	script->SetFunction(L"Pause", [&](const std::wstring& _text) {
		audios[nowPlayAudio]->Pause();
		});

	script->SetFunction(L"LoadMap", [&](const std::wstring& _text) {AddField(_text); });

	script->SetFunction(L"LoadMecha", [&](const std::wstring& _text) {AddMecha(_text); });

	script->SetFunction(L"LoadSkyObject", [&](const std::wstring& _text) {AddSkyObject(_text); });

	script->SetFunction(L"Loop", [&](const std::wstring& _text)
		{
			script->SetLoopPos(_text);
		});

	script->SetFunction(L"End", [&](const std::wstring& _text)
		{
			script->SetPosToLoopStart(_text);
		});

	script->SetFunction(L"SetControllerUsing", [&](const std::wstring& _text)
		{
			bool setFlg = false;
			std::wstring test = L"";
			if (_text.length() == 4)
			{
				setFlg = true;
				test = L"true";
			}

			if (_text.length() == 5)
			{
				setFlg = false;
				test = L"false";
			}

			bool testFlg = true;
			std::wstring useText = _text;
			for (unsigned long i = 0; i < test.size(); i++)
			{
				useText[i] = useText[i] < 'a' ? useText[i] + ('a' - 'A') : useText[i];
				if (useText[i] == test[i])continue;
				testFlg = false;
				break;
			}

			if (!testFlg)return;

			allControllFlg = setFlg;
		});

	script->SetFunction(L"Message", [&](const std::wstring& _text) {

		auto&& texts = ChStr::Split<wchar_t>(_text, L" ");

		std::wstring messenger = L"COM",message = texts[0];
		long afterFrame = 20 * BASE_FPS, messageAddFrame = 0;

		for (unsigned long i = 1; i < texts.size(); i++)
		{
			
			if (texts[i] == L"--messenger")
			{
				if (texts.size() <= i + 1)continue;
				i++;
				messenger = texts[i];
			}

			if (texts[i] == L"--addFrame")
			{
				if (texts.size() <= i + 1)continue;
				i++;
				messageAddFrame = static_cast<long>(ChStr::GetNumFromText<float,wchar_t>(texts[i]) * BASE_FPS);
			}

			if (texts[i] == L"--afterFrame")
			{
				if (texts.size() <= i + 1)continue;
				i++;
				afterFrame = static_cast<long>(ChStr::GetNumFromText<float,wchar_t>(texts[i]) * BASE_FPS);
			}

			if (texts[i] == L"--stop")
			{
				scriptPauseOnMessageFlg = true;
			}

			if (texts[i] == L"--endDrawKeyFlg")
			{
				endDrawKeyFlg = true;
			}
		}

		messageBox->SetMessage(messenger,message,afterFrame,messageAddFrame);
		});

	script->SetFunction(L"MissionStart", [&](const std::wstring& _text) {
		missionStartAnimationFlg = true;
		});

	script->SetFunction(L"Animation", [&](const std::wstring& _text) {
		SetAnimation(_text);
		animationFlg = true;
		});

	script->SetFunction(L"Success", [&](const std::wstring& _text) {
		messageBox->SetMessage(L"COM", L"作戦終了。\n戦闘状態を解除します。", SF_MESSAGE_AFTER_FRAME, SF_MESSAGE_ADD_MESSAGE_FRAME);
		successFlg = true;
		successPauseCount = SUCCESS_PAUSE_COUNT;
		});

	script->SetFunction(L"Failed", [&](const std::wstring& _text) {
		messageBox->SetMessage(L"COM", L"作戦失敗。\n搭乗者の安否を確認します。", SF_MESSAGE_AFTER_FRAME, SF_MESSAGE_ADD_MESSAGE_FRAME);
		failedFlg = true;
		});

	//target < inputNum//
	script->SetFunction(L"SkipIfGreater", [&](const std::wstring& _text)
		{
			auto args = ChStr::Split<wchar_t>(_text, L" ");

			unsigned long skip = ChStr::GetNumFromText<unsigned char,wchar_t>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetNumFromText<unsigned char,wchar_t>(args[1]);
			unsigned long targetNum = GettargetNum(args);
			if (targetNum >= base)return;

			script->SetSkipCount(skip);
		});


	//target <= inputNum//
	script->SetFunction(L"SkipIfMore", [&](const std::wstring& _text)
		{
			auto args = ChStr::Split<wchar_t>(_text, L" ");
			unsigned long skip = ChStr::GetNumFromText<unsigned char,wchar_t>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetNumFromText<unsigned char,wchar_t>(args[1]);
			unsigned long targetNum = GettargetNum(args);

			if (targetNum > base)return;

			script->SetSkipCount(skip);
		});

	//target == inputNum//
	script->SetFunction(L"SkipIfEqual", [&](const std::wstring& _text)
		{
			auto args = ChStr::Split<wchar_t>(_text, L" ");
			unsigned long skip = ChStr::GetNumFromText<unsigned char,wchar_t>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetNumFromText<unsigned char,wchar_t>(args[1]);
			unsigned long targetNum = GettargetNum(args);

			if (targetNum != base)return;

			script->SetSkipCount(skip);
		});

	//target >= inputNum//
	script->SetFunction(L"SkipIfLess", [&](const std::wstring& _text)
		{
			auto args = ChStr::Split<wchar_t>(_text, L" ");
			unsigned long skip = ChStr::GetNumFromText<unsigned char,wchar_t>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetNumFromText<unsigned char,wchar_t>(args[1]);
			unsigned long targetNum = GettargetNum(args);

			if (targetNum < base)return;

			script->SetSkipCount(skip);
		});

	//target > inputNum//
	script->SetFunction(L"SkipIfSmaller", [&](const std::wstring& _text)
		{
			auto args = ChStr::Split<wchar_t>(_text, L" ");
			unsigned long skip = ChStr::GetNumFromText<unsigned char,wchar_t>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetNumFromText<unsigned char,wchar_t>(args[1]);
			unsigned long targetNum = GettargetNum(args);

			if (targetNum <= base)return;

			script->SetSkipCount(skip);
		});


}

void GameFrame::SetHitMap(ChPtr::Shared<MapObject> _map)
{

	ChVec3 fieldSize;

	for (auto&& child : _map->model->GetAllChildlen<ChCpp::FrameObject<wchar_t>>())
	{
		auto childObj = child.lock();
		if (childObj == nullptr)continue;
		auto frameCom = childObj->GetComponent<ChCpp::FrameComponent<wchar_t>>();
		if (frameCom == nullptr)continue;
		childObj->UpdateAllDrawTransform();
		ChLMat tmpMat = childObj->GetDrawLHandMatrix() * _map->mat;
		ChVec3 tmp = tmpMat.TransformCoord(frameCom->boxSize);
		fieldSize = fieldSize.x > tmp.x ? fieldSize.x : tmp.x;
		fieldSize = fieldSize.y > tmp.y ? fieldSize.y : tmp.y;
		fieldSize = fieldSize.z > tmp.z ? fieldSize.z : tmp.z;
	}

	auto cpuLookAnchor = _map->SetComponent<MapLookAnchor>();
	cpuLookAnchor->SetPositionList(*_map->model, _map->mat);
	
	auto mapCollider = _map->SetComponent<MapCollider>();
	mapCollider->GetCollider().SetLeftHandFlg();
	mapCollider->SetMatrix(_map->mat);
	mapCollider->SetPolygon(*_map->model);

	hitMapList.push_back(_map);
	PhysicsMachine::AddField(_map->model, _map->mat);
	PhysicsMachine::SetFieldSize(fieldSize * 0.9f);
}


void GameFrame::LoadStage(std::wstring& _stageScriptName)
{

	auto playerData = ChPtr::SharedSafeCast<PlayerData>(BaseFrame::GetData());

	std::wstring stageScript = L"";

	{
		ChCpp::File<wchar_t> file;
		file.FileOpen(STAGE_DIRECTORY(+_stageScriptName));

		stageScript = file.FileReadText();

		file.FileClose();
	}

	if (stageScript.empty())
	{
		auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();
		windows->Release();
		return;
	}

	script->CreateAllScript(stageScript);

	ChD3D::XAudioManager().LoadStart();

	while (!initFlg)
	{
		script->UpdateScript();
	}

	ChD3D::XAudioManager().LoadEnd();
}

void GameFrame::LoadScript(const std::wstring& _text)
{
	ChCpp::TextObject<wchar_t> text;
	text.SetText(_text.c_str());
}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Release()
{
	gameEndFlg = true;
	shotEffectList->Release();
	smokeEffectList->Release();
	mechaList.ClearObject();
	bulletList.ClearObject();
	mapList.ClearObject();
	mechaList.ClearObject();
	hitMapList.clear();
	audios.clear();
	PhysicsMachine::ClearFieldList();
	MechaParts::ClearPartsList();
	Attack::AllRelease();

	auto&& mouse = ChWin::Mouse();
	mouse.SetVisibleFlg(true);
	mouse.SetCenterFixedFlg(false);

}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Update()
{

	UpdateFunction();


	if (scriptPauseOnMessageFlg)
	{
		scriptPauseOnMessageFlg = messageBox->IsDrawMessage();
		scriptPauseFlg = scriptPauseOnMessageFlg;
	}
	else
	{
		endDrawKeyFlg = false;
	}


	if(!scriptPauseFlg)
		script->UpdateScript();

	if (endDrawKeyFlg)
	{
		auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();
		if (windows->IsPushKey(VK_LBUTTON))
		{
			messageBox->EndSetDrawMessage();
		}
	}



#if DEBUG_FLG

	auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();
	if (windows->IsPushKey(VK_ESCAPE))
	{
		windows->Release();
		return;
	}

	if (windows->IsPushKey(VK_RETURN))
	{
		int i = 0;
		i = 1;
	}

#endif

	//auto&& looker = enemy->GetComponent<CPUObjectLooker>();

	//auto&& lookTarget =
		//looker->GetLookTypeMechas(CPUObjectLooker::MemberType::Enemy, CPUObjectLooker::DistanceType::Near, CPUObjectLooker::DamageSizeType::None);

	DrawFunctionBegin();

#if DISPLAY_FPS_FLG

	auto pos = drawMecha != nullptr ? drawMecha->GetPosition() : ChVec3();

	box.SetText(
		//"FPS:" +
		//std::to_string(ChSystem::SysManager().GetNowFPSPoint()) +
		//"\r\n" +
		//"Bullet Num:" +
		//std::to_string(bulletList.GetObjectCount()) +
		//"\r\n" +
		"Position:x [" + std::to_string(pos.x) + "] y [" + std::to_string(pos.y) + "] z [" + std::to_string(pos.z) + "] \r\n"
		//"RobotCount:" + 
		//std::to_string(mechaList.GetObjectCount())
	);

#endif

	DrawFunction();

}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::UpdateFunction()
{
	if (allControllFlg)
	{
		mechaList.ObjectUpdateBegin();
	}

	mechaList.ObjectUpdate();

	bulletList.ObjectUpdate();

	mechaList.ObjectUpdateEnd();

	mechaList.ObjectMove();

	mechaList.ObjectMoveEnd();

	for (auto&& mecha : mechaList.GetObjectList<BaseMecha>())
	{
		auto mObj = mecha.lock();
		for (auto&& bullet : bulletList.GetObjectList<AttackObject>())
		{
			auto bObj = bullet.lock();
			if (bObj->IsHit())continue;
			mObj->TestBulletHit(*bObj);
		}
	}


	bulletList.ObjectMove();

#if USE_THREAD
	shotEffectList->SetUpdateFlg(false);
	smokeEffectList->SetUpdateFlg(false);
#endif

	Success();
	Failed();

	messageBox->Update();
	weaponDataDrawer->Update();

	for (unsigned long i = 0; i < ENEMY_TARGET_MARKER_SIZE; i++)
	{
		enemyMarkerShader->SetEffectDisplayFlg(false,i);
	}
}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::DrawFunction()
{

#if USE_THREAD
	if (!shotEffectList->IsUpdateFlg()) {}
	if (!smokeEffectList->IsUpdateFlg()) {}
#else
	shotEffectList->Update();
	smokeEffectList->Update();
#endif

	mechaList.ObjectDrawBegin();

	rt3D.SetBackColor(ChD3D11::D3D11DC(), ChVec4::FromColor(0.0f, 0.0f, 1.0f, 1.0f));
	rtHighLightMap.SetBackColor(ChD3D11::D3D11DC(), ChVec4(0.0f, 0.0f, 0.0f, 1.0f));
	rt2D.SetBackColor(ChD3D11::D3D11DC(), ChVec4(0.0f));
	dsTex.ClearDepthBuffer(ChD3D11::D3D11DC());

	uiDrawer.SetAlphaBlendFlg(true);
	ChD3D11::Shader11().DrawStart();

	Render3D();
	ID3D11RenderTargetView* renderTargetView = rt2D.GetRTView();
	ChD3D11::D3D11DC()->OMSetRenderTargets(1, &renderTargetView,nullptr);

	enemyMarkerShader->DrawStart(ChD3D11::D3D11DC());

	enemyMarkerShader->Draw(ChD3D11::D3D11DC());

	enemyMarkerShader->DrawEnd();

	Render2D();

	renderTargetView = rt3D.GetRTView();
	ChD3D11::D3D11DC()->OMSetRenderTargets(1, &renderTargetView, nullptr);

	uiDrawer.DrawStart(ChD3D11::D3D11DC());

	uiDrawer.Draw(rt2D, uiSprite);

	if (successFlg)
	{
		ChVec4 fadeOutColor = ChVec4(1.0f);
		fadeOutColor.a =(SUCCESS_PAUSE_COUNT - successPauseCount) / static_cast<float>(SUCCESS_PAUSE_COUNT);
		audios[nowPlayAudio]->SetVolume(1.0f - fadeOutColor.a);
		uiDrawer.Draw(fadeOutTexture, uiSprite, fadeOutColor);
	}

	//uiDrawer.Draw(rtHighLightMap, testTextureSprite);

	uiDrawer.DrawEnd();

	ChD3D11::Shader11().DrawEnd(rt3D);

	mechaList.ObjectDrawEnd();

	drawMecha = nullptr;

}

void GameFrame::DrawFunctionBegin()
{
	if (mechas.size() <= mechaView)return;
	auto&& tmpMecha = mechas[mechaView];
	if (tmpMecha.expired())return;
	drawMecha = tmpMecha.lock();

	auto viewMat = drawMecha->GetViewMat();

	meshDrawer.SetViewMatrix(viewMat);
	
	shotTargetDrawer.SetViewMatrix(viewMat);

	shotTargetdrawBaseMatrix.SetRotationYAxis(ChMath::ToRadian(drawMecha->GetRotation().y));
	shotTargetdrawBaseMatrix.SetPosition(drawMecha->GetPosition());
	shotTargetdrawBaseMatrix.SetScalling(SHOT_TARGET_MARKER_SIZE);

	shotEffectList->SetViewMatrix(viewMat);

	smokeEffectList->SetViewMatrix(viewMat);

	auto&& objectLooker = drawMecha->GetComponent<CPUObjectLooker>();
	if (objectLooker != nullptr)
	{

		ChLMat vpMat = viewMat * projectionMat;

		auto&& list = objectLooker->GetLookMechaList();


		for (unsigned long i = 0; i < list.size(); i++)
		{
			auto&& mecha = mechas[list[i]];
			if (mecha.expired())continue;
			auto&& mechaPointer = mecha.lock();

			ChVec4 position = vpMat.Transform(mechaPointer->GetPosition());
			position /= position.w;

			ChVec2 tmp = position;
			tmp.Abs();
			if (tmp.y > CENTER_UI_SIZE * ENEMY_TARGET_RANGE_COEFFICIENT / GAME_WINDOW_HEIGHT || tmp.x > (CENTER_UI_SIZE * ENEMY_TARGET_RANGE_COEFFICIENT / GAME_WINDOW_WIDTH))continue;

			enemyMarkerShader->SetEffectPosition(position, i);
			enemyMarkerShader->SetEffectDisplayFlg(true, i);

		}

	}

	viewMat.Inverse();

	light.SetCamPos(viewMat.GetPosition());
	//ChD3D::XAudioManager().InitMatrix(ChLMat());
	ChD3D::XAudioManager().InitMatrix(viewMat);

	ChVec3 dir = ChVec3(0.0f, -1.0f, 0.0f);
	dir.Normalize();
	light.SetLightDir(dir);
	
	auto&& rightWeaponComponent = drawMecha->GetComponent<RightWeaponComponent>();
	if (rightWeaponComponent != nullptr)
	{
		weaponDataDrawer->SetPartsName(rightWeaponComponent->GetPartsName(), WeaponDataDrawUI::DRAW_TYPE::Right);
		weaponDataDrawer->SetWeaponName(rightWeaponComponent->GetWeaponName(), WeaponDataDrawUI::DRAW_TYPE::Right);
		weaponDataDrawer->SetNowBulletNum(rightWeaponComponent->GetNowBulletNum(), WeaponDataDrawUI::DRAW_TYPE::Right);
		weaponDataDrawer->SetReloadCount(rightWeaponComponent->GetReloadCount(), WeaponDataDrawUI::DRAW_TYPE::Right);
	}

	auto&& leftWeaponComponent = drawMecha->GetComponent<LeftWeaponComponent>();
	if (leftWeaponComponent != nullptr)
	{
		weaponDataDrawer->SetPartsName(leftWeaponComponent->GetPartsName(), WeaponDataDrawUI::DRAW_TYPE::Left);
		weaponDataDrawer->SetWeaponName(leftWeaponComponent->GetWeaponName(), WeaponDataDrawUI::DRAW_TYPE::Left);
		weaponDataDrawer->SetNowBulletNum(leftWeaponComponent->GetNowBulletNum(), WeaponDataDrawUI::DRAW_TYPE::Left);
		weaponDataDrawer->SetReloadCount(leftWeaponComponent->GetReloadCount(), WeaponDataDrawUI::DRAW_TYPE::Left);
	}

}

void GameFrame::Render3D(void)
{
	light.SetUseLightFlg(true);
	light.SetPSDrawData(ChD3D11::D3D11DC());

	ID3D11RenderTargetView* meshRT[] = { rt3D.GetRTView() , rtHighLightMap.GetRTView() };
	ChD3D11::D3D11DC()->OMSetRenderTargets(2, meshRT, dsTex.GetDSView());

	meshDrawer.DrawStart(ChD3D11::D3D11DC());

	for (auto weakMapModel : mapList.GetObjectList<MapObject>())
	{
		auto mapModel = weakMapModel.lock();
		meshDrawer.Draw(*mapModel->model, (ChMat_11)mapModel->mat);
	}

	mechaList.ObjectDraw3D();

	bulletList.ObjectDraw3D();

	meshDrawer.DrawEnd();

	shotEffectList->Draw(ChD3D11::D3D11DC());

	smokeEffectList->Draw(ChD3D11::D3D11DC());

	ChD3D11::D3D11DC()->OMSetRenderTargets(1, meshRT, nullptr);

	lightBloomeDrawer.DrawStart(ChD3D11::D3D11DC());
	lightBloomeDrawer.Draw(rtHighLightMap, uiSprite);
	lightBloomeDrawer.DrawEnd();

	light.SetUseLightFlg(false);

	shotTargetDrawer.DrawStart(ChD3D11::D3D11DC());
	
	shotTargetDrawer.Draw(shotTargetMarkerTex, shotTargetBorad, (ChMat_11)shotTargetdrawBaseMatrix);

	shotTargetDrawer.DrawEnd();

}

void GameFrame::Render2D(void)
{

	if (drawMecha != nullptr)
	{
		float damageParcec = static_cast<float>(drawMecha->GetDamage()) / drawMecha->GetMaxDamageGage();

		float enelgyParcec = static_cast<float>(drawMecha->GetNowEnelgy()) / drawMecha->GetMaxEnelgy();

		if (!successFlg && !failedFlg)
		{
			gageDrawer.SetDrawValue(enelgyParcec * 0.5f);

			gageDrawer.DrawStart(ChD3D11::D3D11DC());

			gageDrawer.Draw(enelgyUITexture, centerUISprite);

			gageDrawer.DrawEnd();

			gageDrawer.SetDrawValue(damageParcec * -0.5f);

			gageDrawer.DrawStart(ChD3D11::D3D11DC());

			gageDrawer.Draw(receveDamageUITexture, centerUISprite);

			gageDrawer.DrawEnd();

		}

	}

	uiDrawer.DrawStart(ChD3D11::D3D11DC());

	messageBox->Draw(uiDrawer);

	if (!successFlg && !failedFlg)
	{
		uiDrawer.Draw(centerUITexture, centerUISprite);
		weaponDataDrawer->Draw(uiDrawer);

	}

	if (drawMecha != nullptr)
	{
		long hitEffectDrawFrame = drawMecha->GetHitEffectDrawFrame();
		if (hitEffectDrawFrame >= 0)
		{
			ChVec4 drawColor = ChVec4(1.0f);
			drawColor.a = static_cast<float>(hitEffectDrawFrame) / (drawMecha->GetHitEffectDrawStartFrame());
			uiDrawer.Draw(hitIcon.image, hitIcon.sprite, drawColor);
		}
	}

	uiDrawer.DrawEnd();

}

void GameFrame::AddMecha(const std::wstring& _text)
{
	auto argment = ChStr::Split<wchar_t>(_text, L" ");

	auto&& mecha = mechaList.SetObject<BaseMecha>();

	mecha->Create(ChVec2(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), meshDrawer, this);

	unsigned char teamNo = 0;

	bool playerFlg = false;
	bool cpuFlg = false;

	std::wstring cpuLoadData = L"";
	std::wstring loadFile = PLAYER_USE_MECHA_PATH;
	bool loadCPUFlg = false;

	ChVec3 position;
	ChVec3 rotation;

	for (unsigned long i = 0; i < argment.size(); i++)
	{
		if (argment[i] == L"-l" || argment[i] == L"-load")
		{
			i++;
			loadFile = CPU_MECHA_PATH(+argment[i]);
			loadCPUFlg = true;
		}
		if (argment[i] == L"-u" || argment[i] == L"--username")
		{
			i++;
			mecha->SetMyName(argment[i]);
			continue;
		}
		if (argment[i] == L"-p" || argment[i] == L"--position")
		{
			i++;
			position.Deserialize<wchar_t>(argment[i], 0, L",", L";");
			continue;
		}
		if (argment[i] == L"-r" || argment[i] == L"--rotation")
		{
			i++;
			rotation.Deserialize<wchar_t>(argment[i], 0, L",", L";");
			continue;
		}
		if (argment[i] == L"-rp" || argment[i] == L"--randomposition")
		{
			ChVec3 min = 0.0f;
			ChVec3 max = 1.0f;
			for (unsigned long j = 0; j < 2; j++)
			{
				if (argment[i + 1][0] == 'm' && argment[i + 1][1] == 'i' && argment[i + 1][2] == 'n')
				{
					i++;
					min.Deserialize<wchar_t>(argment[i], 3, L",", L";");
				}

				if (argment[i + 1][0] == 'm' && argment[i + 1][1] == 'a' && argment[i + 1][2] == 'x')
				{
					i++;
					max.Deserialize<wchar_t>(argment[i], 3, L",", L";");
				}
			}

			ChVec3 random;
			random.x = GameScript::GetRand(min.x, max.x);
			random.y = GameScript::GetRand(min.y, max.y);
			random.z = GameScript::GetRand(min.z, max.z);
			position += random;
			continue;
		}
		if (argment[i] == L"-rr" || argment[i] == L"--randomrotation")
		{

			ChVec3 min = 0.0f;
			ChVec3 max = 1.0f;
			for (unsigned long j = 0; j < 2; j++)
			{
				if (argment[i + 1][0] == 'm' && argment[i + 1][1] == 'i' && argment[i + 1][2] == 'n')
				{
					i++;
					min.Deserialize<wchar_t>(argment[i], 3, L",", L";");
				}

				if (argment[i + 1][0] == 'm' && argment[i + 1][1] == 'a' && argment[i + 1][2] == 'x')
				{
					i++;
					max.Deserialize<wchar_t>(argment[i], 3, L",", L";");
				}
			}

			ChVec3 random;
			random.x = GameScript::GetRand(min.x, max.x);
			random.y = GameScript::GetRand(min.y, max.y);
			random.z = GameScript::GetRand(min.z, max.z);
			rotation += random;
			continue;

		}
		if (argment[i] == L"-pc" || argment[i] == L"--playercontroller")
		{
			if (!cpuFlg)
			{
				playerFlg = true;
			}
			continue;
		}
		if (argment[i] == L"-cc" || argment[i] == L"--cpucontroller")
		{
			if (!playerFlg)
			{
				cpuFlg = true;
				i++;
				cpuLoadData = argment[i];

			}
			continue;
		}
		if (argment[i] == L"-t" || argment[i] == L"--team")
		{
			i++;
			teamNo = ChStr::GetNumFromText<unsigned char,wchar_t>(argment[i]);
			continue;
		}
	}




	mecha->SetPosition(position);
	mecha->SetRotation(rotation);

	mecha->Load(ChD3D11::D3D11Device(), loadFile);

	if (playerFlg)
	{	
		mecha->SetComponent<PlayerController>();
		auto cpuObjectLooker = mecha->SetComponent<CPUObjectLooker>();
		cpuObjectLooker->SetGameFrame(this);
		cpuObjectLooker->SetProjectionMatrix(projectionMat);

		playerParty = teamNo;
	}

	if (cpuFlg)
	{
		auto cpuController = mecha->SetComponent<CPUController>();
		cpuController->LoadCPUData(cpuLoadData);
		cpuController->SetGameFrame(this);
		auto cpuObjectLooker = mecha->SetComponent<CPUObjectLooker>();
		cpuObjectLooker->SetGameFrame(this);
		cpuObjectLooker->SetProjectionMatrix(projectionMat);
	}

	mecha->SetTeamNo(teamNo);

	auto&& counter = mechaPartyCounter.find(teamNo);
	if (counter == mechaPartyCounter.end())
	{
		mechaPartyCounter[teamNo] = 0;
	}
	
	mechaList.SetObject(mecha);
	mechas.push_back(mecha);
	mechaPartyCounter[teamNo] += 1;
}

void GameFrame::AddField(const std::wstring& _text)
{
	auto argment = ChStr::Split<wchar_t>(_text, L" ");

	auto mainMap = ChPtr::Make_S<MapObject>();
	mainMap->model->Init(ChD3D11::D3D11Device());
	unsigned long pos = argment[0].find_last_of(L".");

	if (argment[0].substr(pos) == L".x") {
		ChCpp::ModelLoader::XFile<wchar_t> loader;
		loader.CreateModel(mainMap->model, FIELD_DIRECTORY(+argment[0]));
	}
	else if (argment[0].substr(pos) == L".obj") {
		ChCpp::ModelLoader::ObjFile<wchar_t> loader;
		loader.CreateModel(mainMap->model, FIELD_DIRECTORY(+argment[0]));
	}
	else
	{
		return;
	}

	mapList.SetObject(mainMap);

	bool hitMapFlg = false;

	ChVec3 position, rotation, scalling;

	for (unsigned long i = 1; i < argment.size(); i++)
	{
		if (argment[i] == L"-p" || argment[i] == L"--position")
		{
			i++;
			position.Deserialize<wchar_t>(argment[i], 0, L",", L";");
			continue;
		}
		if (argment[i] == L"-r" || argment[i] == L"--rotation")
		{
			i++;
			rotation.Deserialize<wchar_t>(argment[i], 0, L",", L";");
			continue;
		}
		if (argment[i] == L"-s" || argment[i] == L"--scalling")
		{
			i++;
			scalling.Deserialize<wchar_t>(argment[i], 0, L",", L";");
			continue;
		}
		if (argment[i] == L"-h" || argment[i] == L"--hit")
		{
			hitMapFlg = true;
			continue;
		}
	}

	ChMat_11 mapSizeMatrix;
	mapSizeMatrix.SetPosition(position);
	mapSizeMatrix.SetRotation(rotation);
	mapSizeMatrix.SetScaleSize(scalling);
	mainMap->mat = mapSizeMatrix;

	if (hitMapFlg)
	{
		SetHitMap(mainMap);
	}

}

void GameFrame::AddSkyObject(const std::wstring& _text)
{
	auto argment = ChStr::Split<wchar_t>(_text, L" ");

	skySphere = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
	skySphere->Init(ChD3D11::D3D11Device());
	unsigned long pos = argment[0].find_last_of(L".");
	if (argment[0].substr(pos) == L".x") {
		ChCpp::ModelLoader::XFile<wchar_t> loader;
		loader.CreateModel(skySphere, FIELD_DIRECTORY(+argment[0]));
	}
	else if (argment[0].substr(pos) == L".obj") {
		ChCpp::ModelLoader::ObjFile<wchar_t> loader;
		loader.CreateModel(skySphere, FIELD_DIRECTORY(+argment[0]));
	}
	else
	{
		return;
	}

	for (unsigned long i = 1; i < argment.size(); i++)
	{
		if (argment[i] == L"-s" || argment[i] == L"--scalling")
		{
			i++;
			ChVec3 tmp;
			tmp.Deserialize<wchar_t>(argment[i], 0, L",", L";");
			ChLMat mat;
			mat.SetScalling(tmp);
			skySphere->SetOutSizdTransform(mat);
			continue;
		}
	}
}

void GameFrame::AddBGM(const std::wstring& _text)
{
	auto argment = ChStr::Split<wchar_t>(_text, L" ");
	auto audio = ChPtr::Make_S< ChD3D::AudioObject>();

	std::wstring audioName = argment[0];

	ChD3D::XAudioManager().LoadSound(*audio, SOUND_DIRECTORY(+audioName));
	audio->SetLoopFlg(false);

	for (unsigned long i = 1; i < argment.size(); i++)
	{
		if (argment[i] == L"-l" || argment[i] == L"--loop")
		{
			audio->SetLoopFlg(true);
			continue;
		}

		if (argment[i] == L"-sp" || argment[i] == L"--startpos")
		{
			i++;
			unsigned long start = ChStr::GetNumFromText<unsigned long,wchar_t>(argment[i]);
			audio->SetPlayTime(start);
			continue;
		}

		if (argment[i] == L"-ls" || argment[i] == L"--loopstart")
		{
			i++;
			unsigned long start = ChStr::GetNumFromText<unsigned long,wchar_t>(argment[i]);
			audio->SetLoopStartPos(start);
			continue;
		}

		if (argment[i] == L"-le" || argment[i] == L"--loopend")
		{
			i++;
			unsigned long end = ChStr::GetNumFromText<unsigned long,wchar_t>(argment[i]);
			audio->SetLoopEndPos(end);
			continue;
		}

		if (argment[i] == L"-v" || argment[i] == L"--volume")
		{
			i++;
			float vol = ChStr::GetNumFromText<float,wchar_t>(argment[i]);
			audio->SetVolume(vol);
			continue;
		}

	}
	audios[audioName] = audio;
}

void GameFrame::AddBullet(ChPtr::Shared<AttackObject> _bullet)
{
	bulletList.SetObject(_bullet);
}

void GameFrame::AddShotEffectObject(const ChVec3& _pos)
{
	shotEffectList->AddShotEffect(_pos);
}

void GameFrame::AddSmokeEffectObject(const ChVec3& _pos, const ChVec3& _moveVector)
{
	smokeEffectList->AddSmokeEffect(_pos, _moveVector, INIT_SMOKE_DISPERSAL_POWER, INIT_SMOKE_ALPHA_POWER);
}

void GameFrame::AddSmokeEffectObject(const ChVec3& _pos, const ChVec3& _moveVector, const float _initDispersalpower)
{
	smokeEffectList->AddSmokeEffect(_pos, _moveVector, _initDispersalpower, INIT_SMOKE_ALPHA_POWER);
}

void GameFrame::AddSmokeEffectObject(const ChVec3& _pos, const ChVec3& _moveVector, const float _initDispersalpower, const float _initAlphaPow)
{
	smokeEffectList->AddSmokeEffect(_pos, _moveVector, _initDispersalpower, _initAlphaPow);
}

std::vector<ChPtr::Shared<LookSquareValue>> GameFrame::GetLookSquareValuesFromMap(const ChLMat& _viewMatrix, const ChLMat& _projectionMatrix)
{
	std::vector<ChPtr::Shared<LookSquareValue>> res;

	for (auto&& mapWeak : mapList.GetObjectList())
	{
		auto&& map = mapWeak.lock();

		if (map == nullptr)continue;

		auto mapLookAnchor = map->GetComponent<MapLookAnchor>();

		if (mapLookAnchor == nullptr)continue;

		for (auto&& square : mapLookAnchor->GetMapSquares(_viewMatrix, _projectionMatrix))
		{
			res.push_back(square);
		}
	}

	return res;
}

std::vector<ChPtr::Shared<LookSquareValue>> GameFrame::GetLookSquareValuesFromMap(const ChLMat& _vpMatrix)
{
	std::vector<ChPtr::Shared<LookSquareValue>> res;

	for (auto&& mapWeak : mapList.GetObjectList())
	{
		auto&& map = mapWeak.lock();

		if (map == nullptr)continue;

		auto mapLookAnchor = map->GetComponent<MapLookAnchor>();

		if (mapLookAnchor == nullptr)continue;

		for (auto&& square : mapLookAnchor->GetMapSquares(_vpMatrix))
		{
			res.push_back(square);
		}
	}

	return res;
}

void GameFrame::BreakMecha(BaseMecha* _mecha)
{
	mechaPartyCounter[_mecha->GetTeamNo()] -= 1;
}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::CamUpdate(void)
{

}

unsigned long GameFrame::GettargetNum(std::vector<std::wstring>& _args)
{

	unsigned long targetNum = 0;

	for (unsigned long i = 2; i < _args.size(); i++)
	{
		if (_args[i] == L"-e" || _args[i] == L"--enemyAll")
		{
			for (auto count : mechaPartyCounter)
			{
				if (playerParty == count.first)continue;
				targetNum += count.second;
			}
			continue;
		}
		if (_args[i] == L"-p" || _args[i] == L"--party" || _args[i] == L"-t" || _args[i] == L"--teamNo")
		{
			i++;
			unsigned char no = ChStr::GetNumFromText<unsigned char,wchar_t>(_args[i]);

			auto&& it = mechaPartyCounter.find(no);

			if (it == mechaPartyCounter.end())
			{
				continue;
			}

			targetNum = it->second;
			continue;
		}
		if (_args[i] == L"-m" || _args[i] == L"--memberAll")
		{
			targetNum = mechaPartyCounter[playerParty];
			continue;
		}
	}

	return targetNum;

}

void GameFrame::MissionStartAnimation()
{
	if (!missionStartAnimationFlg)return;
	scriptPauseFlg = true;



	allControllFlg = true;
}

void GameFrame::SetAnimation(const std::wstring& _animationFilePath)
{

}

void GameFrame::Aniamtion()
{
	if (!animationFlg)return;
	allControllFlg = false;
	scriptPauseFlg = true;




	allControllFlg = true;
}

void GameFrame::Success()
{
	if (!successFlg)return;
	scriptPauseFlg = true;
	endDrawKeyFlg = false;

	if (messageBox->IsDrawMessage())return;

	successPauseCount--;
	if (successPauseCount >= 0)return;

	SendData(resultData);
	ChangeFrame(ChStd::EnumCast(FrameNo::Result));
	//allControllFlg = false;
}

void GameFrame::Failed()
{
	if (!failedFlg)return;
	scriptPauseFlg = true;
	endDrawKeyFlg = false;

	if (messageBox->IsDrawMessage())return;

	resultData->successFee = 0;
	SendData(resultData);
	ChangeFrame(ChStd::EnumCast(FrameNo::Result));
	scriptPauseFlg = false;
	allControllFlg = false;
}
