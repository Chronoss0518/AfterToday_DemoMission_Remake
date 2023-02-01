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
	//InitScriptFunction();
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

	smokeEffectList->Init(ChD3D11::D3D11Device(), 10000);

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

	LoadMechas();
	LoadMaps();
	LoadBGMs();

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

	script->SetFunction("LoadBGM", [&](const std::string& _text) {
		auto argment = ChStr::Split(_text, " ");
		auto audio = ChPtr::Make_S< ChD3D::AudioObject>();

		std::string audioName = argment[0];

		ChD3D::XAudioManager().CreateSound(audio.get(), SOUND_DIRECTORY(+audioName));
		audio->SetLoopFlg(false);

		for (unsigned long i = 1; i < argment.size(); i++)
		{
			if (argment[i] == "-l" || argment[i] == "--loop")
			{
				audio->SetLoopFlg(true);
				continue;
			}
			if (argment[i] == "-ls" || argment[i] == "--loopstart")
			{
				i++;
				float start = ChStr::GetFloatingFromText<float>(argment[i]);
				audio->SetLoopStartPos(start);
				audio->SetPlayTime(start);
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

		audios[nowPlayAudio]->Stop();

		audios[argment[0]]->Play();
		nowPlayAudio = argment[0];
		});

	script->SetFunction("Stop", [&](const std::string& _text) {
		audios[nowPlayAudio]->Stop();
		});

	script->SetFunction("Pause", [&](const std::string& _text) {
		audios[nowPlayAudio]->Pause();
		});

	script->SetFunction("LoadMap", [&](const std::string& _text) {
		auto argment = ChStr::Split(_text, " ");

		auto mainMap = ChPtr::Make_S<MapObject>();
		mainMap->model->Init(ChD3D11::D3D11Device());
		{
			ChCpp::ModelLoader::XFile loader;
			loader.CreateModel(mainMap->model, MESH_DIRECTORY(+argment[0]));
		}
		map.push_back(mainMap);

		for (unsigned long i = 1; i < argment.size(); i++)
		{
			if (argment[i] == "-p" || argment[i] == "--position")
			{
				i++;
				mainMap->position.Deserialize(argment[i], 0, ",", "\0");
				continue;
			}
			if (argment[i] == "-r" || argment[i] == "--rotation")
			{
				i++;
				mainMap->rotation.Deserialize(argment[i], 0, ",", "\0");
				continue;
			}
			if (argment[i] == "-s" || argment[i] == "--scalling")
			{
				i++;
				mainMap->scalling.Deserialize(argment[i], 0, ",", "\0");
				continue;
			}
			if (argment[i] == "-h" || argment[i] == "--hit")
			{
				SetHitMap(mainMap);
				continue;
			}
		}

		});

	script->SetFunction("LoadMecha", [&](const std::string& _text)
		{

			auto argment = ChStr::Split(_text, " ");

			auto&& mecha = mechaList.SetObject<BaseMecha>(argment[0]);

			mecha->Create(windSize, meshDrawer,this);

			mecha->Load(ChD3D11::D3D11Device(), argment[1]);

			for (unsigned long i = 1; i < argment.size(); i++)
			{
				if (argment[i] == "-p" || argment[i] == "--position")
				{
					i++;
					ChVec3 position;
					position.Deserialize(argment[i], 0, ",", "\0");
					mecha->SetPosition(position);
					continue;
				}
				if (argment[i] == "-r" || argment[i] == "--rotation")
				{
					i++;
					ChVec3 rotation;
					rotation.Deserialize(argment[i], 0, ",", "\0");
					mecha->SetRotation(rotation);
					continue;
				}
				if (argment[i] == "-pc" || argment[i] == "--playercontroller")
				{
					mecha->SetComponent<PlayerController>();
					continue;
				}
				if (argment[i] == "-cc" || argment[i] == "--cpucontroller")
				{
					mecha->SetComponent<CPUController>();
					continue;
				}
				if (argment[i] == "-t" || argment[i] == "--team")
				{
					i++;
					mecha->SetTeamNo(ChStr::GetIntegialFromText<unsigned char>(argment[i]));
					continue;
				}
			}
		});

	script->SetFunction("Loop", [&](const std::string& _text)
		{
			auto args = ChStr::Split(_text," ");
			loopPos[args[0]] = script->GetScriptCount();
		});

	script->SetFunction("End", [&](const std::string& _text)
		{
			auto args = ChStr::Split(_text, " ");
			script->SetNowScriptCount(loopPos[args[0]]);
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
				}
				if (args[i] == "-m" || args[i] == "--memberAll")
				{
					targetNum = mechaPartyCounter[playerParty];
				}
				if (args[i] == "-e" || args[i] == "--enemyAll")
				{
					for (auto count : mechaPartyCounter)
					{
						if (playerParty == count.first)continue;
						targetNum += count.second;
					}
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
				}
				if (args[i] == "-e" || args[i] == "--enemyAll")
				{
					for (auto count : mechaPartyCounter)
					{
						if (playerParty == count.first)continue;
						targetNum += count.second;
					}
				}
				if (args[i] == "-m" || args[i] == "--memberAll")
				{
					targetNum = mechaPartyCounter[playerParty];
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

void GameFrame::LoadScript(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);

}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::LoadMechas()
{
	{

		auto&& mecha = ChPtr::Make_S<BaseMecha>();
		mecha->SetMyName("player");

		mecha->Create(windSize, meshDrawer, this);

		mecha->Load(ChD3D11::D3D11Device(), "AirRobot.amf");
		//mecha->Load(ChD3D11::D3D11Device(), "NormalRobot.amf");
		//mecha->Load(ChD3D11::D3D11Device(), "GuardianRobot.amf");
		mecha->SetPosition(ChVec3(0.0f, 700.0f, 0.0f));
		//mecha->Save("TestAsm.amf");

		AddMecha(mecha, 0, true);
	}

	{
#if 1
		auto&& mecha = ChPtr::Make_S<BaseMecha>();
		mecha->SetMyName("enemyTest");

		mecha->Create(windSize, meshDrawer, this);

		//mecha->Load(ChD3D11::D3D11Device(), "AirRobot.amf");
		//mecha->Load(ChD3D11::D3D11Device(), "NormalRobot.amf");
		mecha->Load(ChD3D11::D3D11Device(), "GuardianRobot.amf");
		mecha->SetPosition(ChVec3(0.0f, 700.0f, 0.0f));

		AddMecha(mecha, 1, false);
#endif
	}


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

}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Release()
{

}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Update()
{

	UpdateFunction();

#if DEBUG_FLG

	auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();
	if (windows->IsPushKey(VK_ESCAPE))
	{
		shotEffectList->Release();
		smokeEffectList->Release();
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

void GameFrame::AddMecha(ChPtr::Shared<BaseMecha> _mecha, unsigned char _mechaPartyNo, const ChStd::Bool _playerFlg)
{
	if (_playerFlg)
	{
		playerParty = _mechaPartyNo;
		_mecha->SetComponent<PlayerController>();
	}
	else
	{
		auto controller = _mecha->SetComponent<CPUController>();
		controller->SetGameFrame(this);
	}

	_mecha->SetTeamNo(_mechaPartyNo);

	auto&& counter = mechaPartyCounter.find(_mechaPartyNo);
	if (counter == mechaPartyCounter.end())
	{
		mechaPartyCounter[_mechaPartyNo] = 0;
	}

	mechaList.SetObject(_mecha);
	mechaPartyCounter[_mechaPartyNo]++;
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
	smokeEffectList->AddShotEffect(_pos,_moveVector);
}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::Render2D(void)
{

}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::CamUpdate(void)
{

}

