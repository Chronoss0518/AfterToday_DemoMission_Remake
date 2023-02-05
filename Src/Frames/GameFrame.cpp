#include"../BaseIncluder.h"

#include"../Shader/EffectObject/EffectObjectShader.h"

#include"../AllStruct.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../Bullet/BulletObject.h"
#include"../GameScript/GameScript.h"
#include"GameFrame.h"

#include"../BaseMecha/Controller/ControllerBase.h"

#include"../CloudObj/CloudObj.h"
#include"../Physics/PhysicsMachine.h"

#include"../EffectObjects/ShotEffectList/ShotEffectList.h"
#include"../EffectObjects/SmokeEffectList/SmokeEffectList.h"

#define PLAYER_MECHA_FILE_NAME ""
#if 0
#define GAME_WINDOW_WITDH 3840
#define GAME_WINDOW_HEIGHT 2160
#endif

//想定するメカオブジェクトの最大数//
#define MAX_MECHA_OBJECT_COUNT 100
#define GAME_WINDOW_WITDH 1280
#define GAME_WINDOW_HEIGHT 720
#define BASE_FPS 60
#define GRAVITY_POWER 4.9f
#define DEBUG_FLG 1

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY("RobotParts/" current_path)
#endif

///////////////////////////////////////////////////////////////////////////////////////
//Gameメソッド
///////////////////////////////////////////////////////////////////////////////////////

void GameFrame::Init()
{
	script = ChPtr::Make_S<GameScript>();
	InitScriptFunction();
	ChSystem::SysManager().SetFPS(BASE_FPS);

	PhysicsMachine::SetFPS(BASE_FPS);
	PhysicsMachine::SetGravityAcceleration(GRAVITY_POWER);
	PhysicsMachine::SetAirRegist(0.1f);

	auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();
	box.Create("Text", ChINTPOINT(100, 0), ChINTPOINT(1000, 100), windows->GetWindObject());
	box.SetEnableFlg(false);

	windSize = ChVec2(static_cast<float>(GAME_WINDOW_WITDH), static_cast<float>(GAME_WINDOW_HEIGHT));

	meshDrawer.drawer.Init(ChD3D11::D3D11Device());
	meshDrawer.drawer.SetCullMode(D3D11_CULL_BACK);

	waterSplashEffectShader = ChPtr::Make_S<EffectObjectShader>();

	waterSplashEffectShader->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 4);

	shotEffectList = ChPtr::Make_S<ShotEffectList>();

	shotEffectList->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 6);

	smokeEffectList = ChPtr::Make_S<SmokeEffectList>();

	smokeEffectList->Init(ChD3D11::D3D11Device(), MAX_MECHA_OBJECT_COUNT * 100);

	smokeEffectList->SetMaxColorPower(0.8f);
	smokeEffectList->SetMinColorPower(0.3f);

	smokeEffectList->SetDownSpeedOnAlphaValue(0.001f);
	smokeEffectList->SetDispersalPower(0.05f);

	{
		ChMat_11 proMat;
		proMat.CreateProjectionMat(ChMath::ToRadian(60.0f), windSize.w, windSize.h, 0.1f, 100000.0f);

		meshDrawer.drawer.SetProjectionMatrix(proMat);
		shotEffectList->SetProjectionMatrix(proMat);
		smokeEffectList->SetProjectionMatrix(proMat);

		light.Init(ChD3D11::D3D11Device());
		light.SetUseLightFlg(true);
		light.SetDirectionLightData(true, ChVec3(1.0f, 1.0f, 1.0f), ChVec3(0.0f, -1.0f, 0.0f), 0.3f);
	}

	//LoadMechas();
	//LoadMaps();
	//LoadBGMs();

	LoadStage();

	light.Init(ChD3D11::D3D11Device());

	enemyMarkerTexture->CreateTexture(TEXTURE_DIRECTORY("Ts.png"), ChD3D11::D3D11Device());
	baseMarkerTexture->CreateTexture(TEXTURE_DIRECTORY("Window.png"), ChD3D11::D3D11Device());

	enemyMarker.CreateRenderTarget(
		ChD3D11::D3D11Device(),
		GAME_WINDOW_WITDH, GAME_WINDOW_HEIGHT);

	baseMarker.CreateRenderTarget(
		ChD3D11::D3D11Device(),
		GAME_WINDOW_WITDH, GAME_WINDOW_HEIGHT);


}

void GameFrame::InitScriptFunction()
{
	script->SetFunction("Initialize", [&](const std::string& _text) {initFlg = true; });

	script->SetFunction("LoadBGM", [&](const std::string& _text) {
		auto argment = ChStr::Split(_text, " ");
		auto audio = ChPtr::Make_S< ChD3D::AudioObject>();

		std::string audioName = argment[0];

		ChD3D::XAudioManager().CreateSound(audio.get(), SOUND_DIRECTORY(+ audioName));
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
				float end = ChStr::GetFloatingFromText<float>(argment[i]);
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
	});

	script->SetFunction("Play", [&](const std::string& _text) {
		auto argment = ChStr::Split(_text, " ");

		if(nowPlayAudio != "")audios[nowPlayAudio]->Stop();

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


	script->SetFunction("SkipIfMany", [&](const std::string& _text)
		{
			auto args = ChStr::Split(_text, " ");

			unsigned long skip = ChStr::GetIntegialFromText<unsigned char>(args[0]);
			unsigned long base = ChStr::GetIntegialFromText<unsigned char>(args[1]);
			unsigned long targetNum = 0;

			for (unsigned long i = 2; i < args.size(); i++)
			{
				if (args[i] == "-p" || args[i] == "--party")
				{
					i++;
					targetNum += mechaPartyCounter[ChStr::GetIntegialFromText<unsigned char>(args[i])];
					continue;
				}
				if (args[i] == "-e" || args[i] == "--enemyAll")
				{
					for (auto count : mechaPartyCounter)
					{
						if (playerParty == count.first)continue;
						targetNum += count.second;
					}
					continue;
				}
				if (args[i] == "-m" || args[i] == "--memberAll")
				{
					targetNum = mechaPartyCounter[playerParty];
					continue;
				}
			}
			if (targetNum <= base)return;

			script->SetNowScriptCount(script->GetScriptCount() + skip);
		});

	script->SetFunction("SkipIfFew", [&](const std::string& _text)
		{
			auto args = ChStr::Split(_text, " ");
			unsigned long skip = ChStr::GetIntegialFromText<unsigned char>(args[0]);
			unsigned long base = ChStr::GetIntegialFromText<unsigned char>(args[1]);
			unsigned long targetNum = 0;

			for (unsigned long i = 2; i < args.size(); i++)
			{
				if (args[i] == "-p" || args[i] == "--party")
				{
					i++;
					targetNum += mechaPartyCounter[ChStr::GetIntegialFromText<unsigned char>(args[i])];
					continue;
				}
				if (args[i] == "-e" || args[i] == "--enemyAll")
				{
					for (auto count : mechaPartyCounter)
					{
						if (playerParty == count.first)continue;
						targetNum += count.second;
					}
					continue;
				}
				if (args[i] == "-m" || args[i] == "--memberAll")
				{
					targetNum = mechaPartyCounter[playerParty];
					continue;
				}
			}
			if (targetNum >= base)return;

			script->SetNowScriptCount(script->GetScriptCount() + skip);
		});
}

void GameFrame::SetHitMap(ChPtr::Shared<MapObject> _map)
{

	ChMat_11 mapSizeMatrix;
	mapSizeMatrix.SetPosition(_map->position);
	mapSizeMatrix.SetRotation(_map->rotation);
	mapSizeMatrix.SetScaleSize(_map->scalling);
	ChVec3 fieldSize;

	for (auto&& child : _map->model->GetAllChildlen<ChCpp::FrameObject>())
	{
		auto childObj = child.lock();
		if (childObj == nullptr)continue;
		auto frameCom = childObj->GetComponent<ChCpp::FrameComponent>();
		if (frameCom == nullptr)continue;
		childObj->UpdateAllDrawTransform();
		ChLMat tmpMat = childObj->GetDrawLHandMatrix() * mapSizeMatrix;
		ChVec3 tmp = tmpMat.TransformCoord(frameCom->boxSize);
		fieldSize = fieldSize.x > tmp.x ? fieldSize.x : tmp.x;
		fieldSize = fieldSize.y > tmp.y ? fieldSize.y : tmp.y;
		fieldSize = fieldSize.z > tmp.z ? fieldSize.z : tmp.z;
	}

	PhysicsMachine::AddField(_map->model, mapSizeMatrix);
	PhysicsMachine::SetFieldSize(fieldSize * 0.9f);
}


void GameFrame::LoadStage()
{

	auto playerData = BaseFrame::GetData<PlayerData>();

	std::string stageScript = "";
	
	{
		ChCpp::File<char> file;
		file.FileOpen(STAGE_DIRECTORY( + playerData->stageName));

		stageScript = file.FileReadText();

		file.FileClose();
	}

	script->CreateAllScript(stageScript);

	while (!initFlg)
	{
		script->UpdateScript();
	}
}

void GameFrame::LoadScript(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);

}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::LoadMechas()
{
	//AddMecha("GuardianRobot.amf --u player -pc --position 0.0f,700.0f,0.0f; -t 0");
	AddMecha("AirRobot.amf --u player -pc --position 0.0f,700.0f,0.0f; -t 0");
	//AddMecha("NormalRobot.amf --u player -pc --position 0.0f,700.0f,0.0f; -t 0");

	AddMecha("GuardianRobot.amf --u enemyTest -cc 1 --position 0.0f,700.0f,0.0f; -t 1");

}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::LoadBGMs()
{
	
	//ChD3D::XAudioManager().CreateSound(&testBattle, SOUND_DIRECTORY("TestBattle.wav"));
	//ChD3D::XAudioManager().CreateSound(&mainBattle, SOUND_DIRECTORY("MainBattle1.wav"));

	//testBattle.Play();

}

void GameFrame::LoadMaps()
{

	AddField("TestField.x -s 100.0f,100.0f,100.0f; -h");
	AddField("TestField2.x -s 100.0f,100.0f,100.0f;");

#if 0 

	auto mainMap = ChPtr::Make_S<MapObject>();
	mainMap->model->Init(ChD3D11::D3D11Device());
	{
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(mainMap->model, MESH_DIRECTORY("TestField.x"));
		mainMap->scalling = 100.0f;
	}
	map.push_back(mainMap); 
	SetHitMap(mainMap);


	auto subMap = ChPtr::Make_S<MapObject>();
	subMap->model->Init(ChD3D11::D3D11Device());
	{
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(subMap->model, MESH_DIRECTORY("TestField2.x"));
	}
	map.push_back(subMap);
#endif
}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Release()
{
	shotEffectList->Release();
	smokeEffectList->Release();
	mechaList.ClearObject();
	bulletList.ClearObject();
}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Update()
{

	script->UpdateScript();

	UpdateFunction();

#if DEBUG_FLG

	auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();
	if (windows->IsPushKey(VK_ESCAPE))
	{
		windows->Release();
		return;
	}

#endif

	DrawFunction();
	//box.SetText("Bullet Num:" + std::to_string(bulletList.GetObjectCount()));
	box.SetText("FPS:" + std::to_string(ChSystem::SysManager().GetNowFPSPoint()));
}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::UpdateFunction()
{

	shotEffectList->SetUpdateFlg(false);
	smokeEffectList->SetUpdateFlg(false);

	mechaList.ObjectUpdateBegin();

	mechaList.ObjectUpdate();

	bulletList.ObjectUpdate();

	mechaList.ObjectUpdateEnd();

	mechaList.ObjectMove();

	mechaList.ObjectMoveEnd();

	for (auto&& mecha : mechaList.GetObjectList<BaseMecha>())
	{
		auto mObj = mecha.lock();
		for (auto&& bullet : bulletList.GetObjectList<BulletObject>())
		{
			auto bObj = bullet.lock();
			mObj->TestBulletHit(*bObj);
		}
	}

	bulletList.ObjectMove();

	{
		auto targetMecha = mechaList.GetObjectList<BaseMecha>()[mechaView].lock();

		auto viewPos = targetMecha->GetViewPos();
		auto viewLookPos = targetMecha->GetViewLookPos();

		ChMat_11 tmpMat;
		tmpMat.CreateViewMatLookTarget(viewPos, viewLookPos, ChVec3(0.0f, 1.0f, 0.0f));

		meshDrawer.drawer.SetViewMatrix(tmpMat);

		shotEffectList->SetViewMatrix(tmpMat);

		smokeEffectList->SetViewMatrix(tmpMat);
		//AddSmokeEffectObject(targetMecha->GetPosition(), 0.0f);


		light.SetCamPos(viewPos);
		
		ChVec3 dir = ChVec3(0.25f, -0.5f, 0.25f);
		dir.Normalize();
		light.SetLightDir(dir);

	}

	while (!shotEffectList->IsUpdateFlg()) {}
	while (!smokeEffectList->IsUpdateFlg()) {}

}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::DrawFunction()
{
	ChD3D11::Shader11().DrawStart();

	ChD3D11::Shader11().DrawStart3D();

	Render3D();

	ChD3D11::Shader11().DrawStart2D();

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

	for (auto mapModel : map)
	{
		ChMat_11 mapSizeMatrix;
		mapSizeMatrix.SetPosition(mapModel->position);
		mapSizeMatrix.SetRotation(mapModel->rotation);
		mapSizeMatrix.SetScaleSize(mapModel->scalling);
		meshDrawer.drawer.Draw(meshDrawer.dc,*mapModel->model, mapSizeMatrix);
	}

	mechaList.ObjectDraw3D();

	bulletList.ObjectDraw3D();

	meshDrawer.drawer.DrawEnd();

	shotEffectList->Draw(meshDrawer.dc);
	smokeEffectList->Draw(meshDrawer.dc);

}

void GameFrame::AddMecha(const std::string& _text)
{

	auto argment = ChStr::Split(_text, " ");

	auto&& mecha = mechaList.SetObject<BaseMecha>("");

	mecha->Create(windSize, meshDrawer, this);

	mecha->Load(ChD3D11::D3D11Device(), argment[0]);

	unsigned char teamNo = 0;

	ChStd::Bool playerFlg = false;
	ChStd::Bool cpuFlg = false;

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
			ChVec3 position;
			position.Deserialize(argment[i], 0, ",", ";");
			mecha->SetPosition(position);
			continue;
		}
		if (argment[i] == "-r" || argment[i] == "--rotation")
		{
			i++;
			ChVec3 rotation;
			rotation.Deserialize(argment[i], 0, ",", ";");
			mecha->SetRotation(rotation);
			continue;
		}
		if (argment[i] == "-pc" || argment[i] == "--playercontroller")
		{
			if (!cpuFlg)
			{
				playerFlg = true;
				mecha->SetComponent<PlayerController>();
			}
			continue;
		}
		if (argment[i] == "-cc" || argment[i] == "--cpucontroller")
		{
			if (!playerFlg)
			{
				cpuFlg = true;
				i++;
				auto cpuController = mecha->SetComponent<CPUController>();
				cpuController->SetCPULevel(ChStr::GetIntegialFromText<unsigned char>(argment[i]));
				cpuController->SetGameFrame(this);
			}
			continue;
		}
		if (argment[i] == "-t" || argment[i] == "--team")
		{
			i++;
			teamNo = ChStr::GetIntegialFromText<unsigned char>(argment[i]);
			mecha->SetTeamNo(teamNo);
			continue;
		}
	}

	auto&& counter = mechaPartyCounter.find(teamNo);
	if (counter == mechaPartyCounter.end())
	{
		mechaPartyCounter[teamNo] = 0;
	}

	if (playerFlg) 
	{
		playerParty = teamNo;
	}

	mechaList.SetObject(mecha);
	mechaPartyCounter[teamNo]++;
}

void GameFrame::AddField(const std::string& _text)
{
	auto argment = ChStr::Split(_text, " ");

	auto mainMap = ChPtr::Make_S<MapObject>();
	mainMap->model->Init(ChD3D11::D3D11Device());
	unsigned long pos = argment[0].find_last_of(".");
	if (argment[0].substr(pos) == ".x") {
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(mainMap->model, MESH_DIRECTORY(+ argment[0]));
	}
	else if (argment[0].substr(pos) == ".obj") {
		ChCpp::ModelLoader::ObjFile loader;
		loader.CreateModel(mainMap->model, MESH_DIRECTORY(+ argment[0]));
	}
	else
	{
		return;
	}
	map.push_back(mainMap);

	for (unsigned long i = 1; i < argment.size(); i++)
	{
		if (argment[i] == "-p" || argment[i] == "--position")
		{
			i++;
			mainMap->position.Deserialize(argment[i], 0, ",", ";");
			continue;
		}
		if (argment[i] == "-r" || argment[i] == "--rotation")
		{
			i++;
			mainMap->rotation.Deserialize(argment[i], 0, ",", ";");
			continue;
		}
		if (argment[i] == "-s" || argment[i] == "--scalling")
		{
			i++;
			mainMap->scalling.Deserialize(argment[i], 0, ",", ";");
			continue;
		}
		if (argment[i] == "-h" || argment[i] == "--hit")
		{
			SetHitMap(mainMap);
			continue;
		}
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

}

void GameFrame::AddSE(const std::string& _text)
{

}


void GameFrame::AddBullet(ChPtr::Shared<BulletObject> _bullet)
{
	bulletList.SetObject(_bullet);
}

void GameFrame::AddShotEffectObject(const ChVec3& _pos)
{
	shotEffectList->AddShotEffect(_pos);
}

void GameFrame::BreakMecha(BaseMecha* _mecha)
{
	mechaPartyCounter[_mecha->GetTeamNo()] -= 1;
}

void GameFrame::AddSmokeEffectObject(const ChVec3& _pos, const ChVec3& _moveVector)
{
	smokeEffectList->AddSmokeEffect(_pos,_moveVector);
}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::Render2D(void)
{

}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::CamUpdate(void)
{

}

