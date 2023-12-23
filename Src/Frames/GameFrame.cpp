#include"../BaseIncluder.h"

#include"../Shader/EffectObject/EffectObjectShader.h"

#include"../AllStruct.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../Attack/AttackObject.h"
#include"../GameScript/GameScript.h"
#include"GameFrame.h"

#include"../BaseMecha/Controller/ControllerBase.h"

#include"../EffectObjects/ShotEffectList/ShotEffectList.h"
#include"../EffectObjects/SmokeEffectList/SmokeEffectList.h"

#include"../BaseMecha/CPU/CPULooker.h"

//想定するメカオブジェクトの最大数//
#define MAX_MECHA_OBJECT_COUNT 10

#define BASE_FPS 60
#define GRAVITY_POWER 9.8f
#define DEBUG_FLG false

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY("RobotParts/" current_path)
#endif

#define INIT_SMOKE_DISPERSAL_POWER 5.0f
#define INIT_SMOKE_ALPHA_POWER 0.5f

#define DISPLAY_FPS_FLG false
#define DISPLAY_NOW_BULLET_NUM_FLG 0

///////////////////////////////////////////////////////////////////////////////////////
//Gameメソッド
///////////////////////////////////////////////////////////////////////////////////////

void GameFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	std::string stageName = "stage1.chs";
	auto&& sendData = ChPtr::SharedSafeCast<StageDataStructure>(_sendData);
	if (sendData != nullptr)stageName = sendData->stageName;

	ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 1.0f, 1.0f));

	script = ChPtr::Make_S<GameScript>();
	InitScriptFunction();
	ChSystem::SysManager().SetFPS(BASE_FPS);

	PhysicsMachine::SetFPS(BASE_FPS);
	PhysicsMachine::SetGravityAcceleration(GRAVITY_POWER);
	PhysicsMachine::SetAirRegist(0.2f);

	auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();

#if DISPLAY_FPS_FLG | DISPLAY_NOW_BULLET_NUM_FLG
	box.Create("Text", ChINTPOINT(100, 0), ChINTPOINT(1000, 100), windows->GetWindObject());
	box.SetEnableFlg(false);
#endif

	meshDrawer.drawer.Init(ChD3D11::D3D11Device());
	meshDrawer.drawer.SetCullMode(D3D11_CULL_BACK);

	waterSplashEffectShader = ChPtr::Make_S<EffectObjectShader>();
	waterSplashEffectShader->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 4);

	shotEffectList = ChPtr::Make_S<ShotEffectList>();
	shotEffectList->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 10);

	smokeEffectList = ChPtr::Make_S<SmokeEffectList>();
	smokeEffectList->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 100, GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);
	smokeEffectList->SetMaxColorPower(0.5f);
	smokeEffectList->SetMinColorPower(0.3f);
	smokeEffectList->SetDownSpeedOnAlphaValue(0.01f);
	smokeEffectList->SetInitialDispersalPower(1.0f);

	enemyMarkerShader = ChPtr::Make_S<EffectObjectShader>();
	enemyMarkerShader->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT);

	light.Init(ChD3D11::D3D11Device());
	light.SetUseLightFlg(true);
	light.SetDirectionLightData(true, ChVec3(1.0f, 1.0f, 1.0f), ChVec3(0.0f, -1.0f, 0.0f), 0.3f);

	centerUITexture->CreateTexture(TEXTURE_DIRECTORY("BattleBarUI/BattleBarFrame.png"));
	receveDamageUITexture->CreateTexture(TEXTURE_DIRECTORY("BattleBarUI/BattleBar_Damage.png"));
	enelgyUITexture->CreateTexture(TEXTURE_DIRECTORY("BattleBarUI/BattleBar_Enelgy.png"));

	gageDrawer.Init(ChD3D11::D3D11Device());

	gageDrawer.SetStartDrawDir(ChVec2(0.0f, -1.0f));
	uiDrawer.Init(ChD3D11::D3D11Device());

	centerUISprite.SetInitPosition();
	centerUISprite.SetPos(0, ChVec2(-0.39f, 0.69f));
	centerUISprite.SetUVPos(0, ChVec2(0.0f, 1.0f));
	
	centerUISprite.SetPos(1, ChVec2(0.39f, 0.69f));
	centerUISprite.SetUVPos(1, ChVec2(1.0f, 1.0f));

	centerUISprite.SetPos(2, ChVec2(0.39f, -0.69f));
	centerUISprite.SetUVPos(2, ChVec2(1.0f, 0.0f));

	centerUISprite.SetPos(3, ChVec2(-0.39f, -0.69f));
	centerUISprite.SetUVPos(3, ChVec2(0.0f, 0.0f));

	{
		ChMat_11 proMat;
		proMat.CreateProjectionMat(ChMath::ToRadian(60.0f), GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, GAME_PROJECTION_NEAR, GAME_PROJECTION_FAR);

		projectionMat = proMat;

		meshDrawer.drawer.SetProjectionMatrix(proMat);
		shotEffectList->SetProjectionMatrix(proMat);
		smokeEffectList->SetProjectionMatrix(proMat);
	}

	LoadStage(stageName);

	drawMecha = mechaList.GetObjectList<BaseMecha>()[mechaView].lock();

	enemyMarkerTexture->CreateTexture(TEXTURE_DIRECTORY("Ts.png"), ChD3D11::D3D11Device());

	enemyMarker.CreateRenderTarget(
		ChD3D11::D3D11Device(),
		GAME_WINDOW_WIDTH_LONG, GAME_WINDOW_HEIGHT_LONG);

}

void GameFrame::InitScriptFunction()
{
	script->SetFunction("Initialize", [&](const std::string& _text) {initFlg = true; });

	script->SetFunction("LoadBGM", [&](const std::string& _text) {
		AddBGM(_text);
		});

	script->SetFunction("Play", [&](const std::string& _text) {
		auto argment = ChStr::Split(_text, " ");

	if (nowPlayAudio != "")audios[nowPlayAudio]->Stop();

	nowPlayAudio = argment[0];
	audios[nowPlayAudio]->Play();
		});

	script->SetFunction("Stop", [&](const std::string& _text) {
		audios[nowPlayAudio]->Stop();
		});

	script->SetFunction("Pause", [&](const std::string& _text) {
		audios[nowPlayAudio]->Pause();
		});

	script->SetFunction("LoadMap", [&](const std::string& _text) {AddField(_text); });

	script->SetFunction("LoadMecha", [&](const std::string& _text) {AddMecha(_text); });

	script->SetFunction("LoadSkyObject", [&](const std::string& _text) {AddSkyObject(_text); });

	script->SetFunction("Loop", [&](const std::string& _text)
		{
			script->SetLoopPos(_text);
		});

	script->SetFunction("End", [&](const std::string& _text)
		{
			script->SetPosToLoopStart(_text);
		});

	script->SetFunction("SetControllerUsing", [&](const std::string& _text)
		{
			bool setFlg = false;
			std::string test = "";
			if (_text.length() == 4)
			{
				setFlg = true;
				test = "true";
			}

			if (_text.length() == 5)
			{
				setFlg = false;
				test = "false";
			}

			bool testFlg = true;
			std::string useText = _text;
			for (unsigned long i = 0; i < test.size(); i++)
			{
				useText[i] = useText[i] < 'a' ? useText[i] + ('a' - 'A') : useText[i];
				if (useText[i] == test[i])continue;
				testFlg = false;
				break;
			}

			if (!testFlg)return;

			SetAllControllerFlg(setFlg);
		});

	//target < inputNum//
	script->SetFunction("SkipIfGreater", [&](const std::string& _text)
		{
			auto args = ChStr::Split(_text, " ");

			unsigned long skip = ChStr::GetIntegialFromText<unsigned char>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetIntegialFromText<unsigned char>(args[1]);
			unsigned long targetNum = GettargetNum(args);
			if (targetNum >= base)return;

			script->SetNowScriptCount(script->GetScriptCount() + skip);
		});


	//target <= inputNum//
	script->SetFunction("SkipIfMore", [&](const std::string& _text)
		{
			auto args = ChStr::Split(_text, " ");
			unsigned long skip = ChStr::GetIntegialFromText<unsigned char>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetIntegialFromText<unsigned char>(args[1]);
			unsigned long targetNum = GettargetNum(args);

			if (targetNum > base)return;

			script->SetNowScriptCount(script->GetScriptCount() + skip);
		});

	//target == inputNum//
	script->SetFunction("SkipIfEqual", [&](const std::string& _text)
		{
			auto args = ChStr::Split(_text, " ");
			unsigned long skip = ChStr::GetIntegialFromText<unsigned char>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetIntegialFromText<unsigned char>(args[1]);
			unsigned long targetNum = GettargetNum(args);

			if (targetNum != base)return;

			script->SetNowScriptCount(script->GetScriptCount() + skip);
		});

	//target >= inputNum//
	script->SetFunction("SkipIfLess", [&](const std::string& _text)
		{
			auto args = ChStr::Split(_text, " ");
			unsigned long skip = ChStr::GetIntegialFromText<unsigned char>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetIntegialFromText<unsigned char>(args[1]);
			unsigned long targetNum = GettargetNum(args);

			if (targetNum < base)return;

			script->SetNowScriptCount(script->GetScriptCount() + skip);
		});

	//target > inputNum//
	script->SetFunction("SkipIfSmaller", [&](const std::string& _text)
		{
			auto args = ChStr::Split(_text, " ");
			unsigned long skip = ChStr::GetIntegialFromText<unsigned char>(args[0]);
			if (skip <= 0)skip = 1;
			unsigned long base = ChStr::GetIntegialFromText<unsigned char>(args[1]);
			unsigned long targetNum = GettargetNum(args);

			if (targetNum <= base)return;

			script->SetNowScriptCount(script->GetScriptCount() + skip);
		});


}

void GameFrame::SetHitMap(ChPtr::Shared<MapObject> _map)
{

	ChVec3 fieldSize;

	for (auto&& child : _map->model->GetAllChildlen<ChCpp::FrameObject>())
	{
		auto childObj = child.lock();
		if (childObj == nullptr)continue;
		auto frameCom = childObj->GetComponent<ChCpp::FrameComponent>();
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


void GameFrame::LoadStage(std::string& _stageScriptName)
{

	auto playerData = ChPtr::SharedSafeCast<PlayerData>(BaseFrame::GetData());

	std::string stageScript = "";

	{
		ChCpp::File<char> file;
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

void GameFrame::LoadScript(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);
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
}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Update()
{

	UpdateFunction();

	script->UpdateScript();

	mechas = mechaList.GetObjectList<BaseMecha>();

	auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();
	if (windows->IsPushKey(VK_ESCAPE))
	{
		windows->Release();
		return;
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

#if DISPLAY_FPS_FLG

	box.SetText("FPS:" +
		std::to_string(ChSystem::SysManager().GetNowFPSPoint()) +
		"\r\n" +
		"Bullet Num:" +
		std::to_string(bulletList.GetObjectCount()) +
		"\r\n" +
		"CPULookRobot:" +
		//(lookTarget >= mechas.size() ? "None" : mechas[lookTarget].expired() ? "None" : mechas[lookTarget].lock()->GetMyName()) +
		//"\r\n" +
		"RobotCount:" + 
		std::to_string(mechaList.GetObjectCount())
	);

#endif
	DrawFunction();

}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::UpdateFunction()
{
	mechaList.ObjectUpdateBegin();

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

	{
		drawMecha = mechaList.GetObjectList<BaseMecha>()[mechaView].lock();

		auto viewMat = drawMecha->GetViewMat();

		meshDrawer.drawer.SetViewMatrix(viewMat);

		shotEffectList->SetViewMatrix(viewMat);

		smokeEffectList->SetViewMatrix(viewMat);

		viewMat.Inverse();

		light.SetCamPos(viewMat.GetPosition());
		ChD3D::XAudioManager().InitMatrix(ChLMat());
		//ChD3D::XAudioManager().InitMatrix(viewMat);

		ChVec3 dir = ChVec3(0.25f, -0.5f, 0.25f);
		dir.Normalize();
		light.SetLightDir(dir);

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

	ChD3D11::Shader11().DrawStart();

	Render3D();

	Render2D();

	ChD3D11::Shader11().DrawEnd();

	mechaList.ObjectDrawEnd();


}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Render3D(void)
{
	meshDrawer.dc = ChD3D11::D3D11DC();

	meshDrawer.drawer.DrawStart(meshDrawer.dc);

	light.SetPSDrawData(meshDrawer.dc);

	for (auto weakMapModel : mapList.GetObjectList<MapObject>())
	{
		auto mapModel = weakMapModel.lock();
		meshDrawer.drawer.Draw(*mapModel->model, (ChMat_11)mapModel->mat);
	}

	mechaList.ObjectDraw3D();

	bulletList.ObjectDraw3D();

	meshDrawer.drawer.DrawEnd();

	shotEffectList->Draw(meshDrawer.dc);
	smokeEffectList->Draw(meshDrawer.dc);

}

void GameFrame::Render2D(void)
{
	float damageParcec = drawMecha->GetDamage() / drawMecha->GetMaxDamageGage();

	float enelgyParcec = static_cast<float>(drawMecha->GetNowEnelgy()) / drawMecha->GetMaxEnelgy();

	uiDrawer.DrawStart(ChD3D11::D3D11DC());

	uiDrawer.Draw(*centerUITexture, centerUISprite);

	uiDrawer.DrawEnd();

	gageDrawer.SetDrawValue(enelgyParcec * 0.5f);

	gageDrawer.DrawStart(ChD3D11::D3D11DC());

	gageDrawer.Draw(*enelgyUITexture, centerUISprite);

	gageDrawer.DrawEnd();

	gageDrawer.SetDrawValue(damageParcec * -0.5f);

	gageDrawer.DrawStart(ChD3D11::D3D11DC());

	gageDrawer.Draw(*receveDamageUITexture, centerUISprite);

	gageDrawer.DrawEnd();
}

void GameFrame::AddMecha(const std::string& _text)
{
	auto argment = ChStr::Split(_text, " ");

	auto&& mecha = mechaList.SetObject<BaseMecha>("");

	mecha->Create(ChVec2(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), meshDrawer, this);

	mecha->Load(ChD3D11::D3D11Device(), argment[0]);

	unsigned char teamNo = 0;

	bool playerFlg = false;
	bool cpuFlg = false;

	std::string cpuLoadData = "";

	ChVec3 position;
	ChVec3 rotation;

	for (unsigned long i = 1; i < argment.size(); i++)
	{
		if (argment[i] == "-u" || argment[i] == "--username")
		{
			i++;
			mecha->SetMyName(argment[i]);
			continue;
		}
		if (argment[i] == "-p" || argment[i] == "--position")
		{
			i++;
			position.Deserialize(argment[i], 0, ",", ";");
			continue;
		}
		if (argment[i] == "-r" || argment[i] == "--rotation")
		{
			i++;
			rotation.Deserialize(argment[i], 0, ",", ";");
			continue;
		}
		if (argment[i] == "-rp" || argment[i] == "--randomposition")
		{
			ChVec3 min = 0.0f;
			ChVec3 max = 1.0f;
			for (unsigned long j = 0; j < 2; j++)
			{
				if (argment[i + 1][0] == 'm' && argment[i + 1][1] == 'i' && argment[i + 1][2] == 'n')
				{
					i++;
					min.Deserialize(argment[i], 3, ",", ";");
				}

				if (argment[i + 1][0] == 'm' && argment[i + 1][1] == 'a' && argment[i + 1][2] == 'x')
				{
					i++;
					max.Deserialize(argment[i], 3, ",", ";");
				}
			}

			ChVec3 random;
			random.x = GameScript::GetRand(min.x, max.x);
			random.y = GameScript::GetRand(min.y, max.y);
			random.z = GameScript::GetRand(min.z, max.z);
			position += random;
			continue;
		}
		if (argment[i] == "-rr" || argment[i] == "--randomrotation")
		{

			ChVec3 min = 0.0f;
			ChVec3 max = 1.0f;
			for (unsigned long j = 0; j < 2; j++)
			{
				if (argment[i + 1][0] == 'm' && argment[i + 1][1] == 'i' && argment[i + 1][2] == 'n')
				{
					i++;
					min.Deserialize(argment[i], 3, ",", ";");
				}

				if (argment[i + 1][0] == 'm' && argment[i + 1][1] == 'a' && argment[i + 1][2] == 'x')
				{
					i++;
					max.Deserialize(argment[i], 3, ",", ";");
				}
			}

			ChVec3 random;
			random.x = GameScript::GetRand(min.x, max.x);
			random.y = GameScript::GetRand(min.y, max.y);
			random.z = GameScript::GetRand(min.z, max.z);
			rotation += random;
			continue;

		}
		if (argment[i] == "-pc" || argment[i] == "--playercontroller")
		{
			if (!cpuFlg)
			{
				playerFlg = true;
			}
			continue;
		}
		if (argment[i] == "-cc" || argment[i] == "--cpucontroller")
		{
			if (!playerFlg)
			{
				cpuFlg = true;
				i++;
				cpuLoadData = argment[i];
			}
			continue;
		}
		if (argment[i] == "-t" || argment[i] == "--team")
		{
			i++;
			teamNo = ChStr::GetIntegialFromText<unsigned char>(argment[i]);
			continue;
		}
	}

	mecha->SetPosition(position);
	mecha->SetRotation(rotation);

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
	mechaPartyCounter[teamNo] += 1;
}

void GameFrame::AddField(const std::string& _text)
{
	auto argment = ChStr::Split(_text, " ");

	auto mainMap = ChPtr::Make_S<MapObject>();
	mainMap->model->Init(ChD3D11::D3D11Device());
	unsigned long pos = argment[0].find_last_of(".");

	if (argment[0].substr(pos) == ".x") {
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(mainMap->model, MESH_DIRECTORY(+argment[0]));
	}
	else if (argment[0].substr(pos) == ".obj") {
		ChCpp::ModelLoader::ObjFile loader;
		loader.CreateModel(mainMap->model, MESH_DIRECTORY(+argment[0]));
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
		if (argment[i] == "-p" || argment[i] == "--position")
		{
			i++;
			position.Deserialize(argment[i], 0, ",", ";");
			continue;
		}
		if (argment[i] == "-r" || argment[i] == "--rotation")
		{
			i++;
			rotation.Deserialize(argment[i], 0, ",", ";");
			continue;
		}
		if (argment[i] == "-s" || argment[i] == "--scalling")
		{
			i++;
			scalling.Deserialize(argment[i], 0, ",", ";");
			continue;
		}
		if (argment[i] == "-h" || argment[i] == "--hit")
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

void GameFrame::AddSkyObject(const std::string& _text)
{
	auto argment = ChStr::Split(_text, " ");

	skySphere = ChPtr::Make_S<ChD3D11::Mesh11>();
	skySphere->Init(ChD3D11::D3D11Device());
	unsigned long pos = argment[0].find_last_of(".");
	if (argment[0].substr(pos) == ".x") {
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(skySphere, MESH_DIRECTORY(+argment[0]));
	}
	else if (argment[0].substr(pos) == ".obj") {
		ChCpp::ModelLoader::ObjFile loader;
		loader.CreateModel(skySphere, MESH_DIRECTORY(+argment[0]));
	}
	else
	{
		return;
	}

	for (unsigned long i = 1; i < argment.size(); i++)
	{
		if (argment[i] == "-s" || argment[i] == "--scalling")
		{
			i++;
			ChVec3 tmp;
			tmp.Deserialize(argment[i], 0, ",", ";");
			ChLMat mat;
			mat.SetScalling(tmp);
			skySphere->SetOutSizdTransform(mat);
			continue;
		}
	}
}

void GameFrame::AddBGM(const std::string& _text)
{
	auto argment = ChStr::Split(_text, " ");
	auto audio = ChPtr::Make_S< ChD3D::AudioObject>();

	std::string audioName = argment[0];

	ChD3D::XAudioManager().LoadSound(*audio, SOUND_DIRECTORY(+audioName));
	audio->SetLoopFlg(false);

	for (unsigned long i = 1; i < argment.size(); i++)
	{
		if (argment[i] == "-l" || argment[i] == "--loop")
		{
			audio->SetLoopFlg(true);
			continue;
		}

		if (argment[i] == "-sp" || argment[i] == "--startpos")
		{
			i++;
			unsigned long start = ChStr::GetIntegialFromText<unsigned long>(argment[i]);
			audio->SetPlayTime(start);
			continue;
		}

		if (argment[i] == "-ls" || argment[i] == "--loopstart")
		{
			i++;
			unsigned long start = ChStr::GetIntegialFromText<unsigned long>(argment[i]);
			audio->SetLoopStartPos(start);
			continue;
		}

		if (argment[i] == "-le" || argment[i] == "--loopend")
		{
			i++;
			unsigned long end = ChStr::GetIntegialFromText<unsigned long>(argment[i]);
			audio->SetLoopEndPos(end);
			continue;
		}

		if (argment[i] == "-v" || argment[i] == "--volume")
		{
			i++;
			float vol = ChStr::GetFloatingFromText<float>(argment[i]);
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

unsigned long GameFrame::GettargetNum(std::vector<std::string>& _args)
{

	unsigned long targetNum = 0;

	for (unsigned long i = 2; i < _args.size(); i++)
	{
		if (_args[i] == "-e" || _args[i] == "--enemyAll")
		{
			for (auto count : mechaPartyCounter)
			{
				if (playerParty == count.first)continue;
				targetNum += count.second;
			}
			continue;
		}
		if (_args[i] == "-p" || _args[i] == "--party" || _args[i] == "-t" || _args[i] == "--teamNo")
		{
			i++;
			unsigned char no = ChStr::GetIntegialFromText<unsigned char>(_args[i]);

			auto&& it = mechaPartyCounter.find(no);

			if (it == mechaPartyCounter.end())
			{
				continue;
			}

			targetNum = it->second;
			continue;
		}
		if (_args[i] == "-m" || _args[i] == "--memberAll")
		{
			targetNum = mechaPartyCounter[playerParty];
			continue;
		}
	}

	return targetNum;

}

void GameFrame::SetAllControllerFlg(bool _flg)
{
	for (auto&& mecha : mechaList.GetObjectList())
	{
		if (mecha.expired())continue;
		auto&& mechaObject = mecha.lock();
		if (mechaObject == nullptr)continue;
		auto&& controller = mechaObject->GetComponent<ControllerBase>();
		if (controller == nullptr)continue;
		controller->SetUsing(_flg);
	}
}