#include"../BaseIncluder.h"


#include"../AllStruct.h"
#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../Bullet/BulletObject.h"
#include"../GameScript/GameScript.h"
#include"GameFrame.h"

#include"../BaseMecha/Controller/ControllerBase.h"

#include"../CloudObj/CloudObj.h"
#include"../Physics/PhysicsMachine.h"

#define PLAYER_MECHA_FILE_NAME ""

#define GAME_WINDOW_WITDH 3840
#define GAME_WINDOW_HEIGHT 2160
#define BASE_FPS 60
#define DEBUG_FLG 1

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY("RobotParts/" current_path)
#endif


///////////////////////////////////////////////////////////////////////////////////////
//GameÉÅÉ\ÉbÉh
///////////////////////////////////////////////////////////////////////////////////////

void GameFrame::TestUpdate()
{

	static ChVec3 testCamRotate = ChVec3(-60.0f, 0.0f, 0.0f);
	static float testCamLen = 10.0f;

	auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();

	if (windows->IsPushKey(VK_LEFT))
	{
		testCamRotate.y -= 5.0f;
	}

	if (windows->IsPushKey(VK_RIGHT))
	{
		testCamRotate.y += 5.0f;
	}

	if (windows->IsPushKey(VK_UP))
	{
		testCamLen -= 0.5f;
	}

	if (windows->IsPushKey(VK_DOWN))
	{
		testCamLen += 0.5f;
	}

	{


		ChLMat tmpMat;

		tmpMat.SetRotationXAxis(ChMath::ToRadian(testCamRotate.x));
		tmpMat.SetRotationYAxis(ChMath::ToRadian(testCamRotate.y));

		ChVec3 tmpVec = ChVec3(0.0f, 0.0f, -testCamLen);

		tmpVec = tmpMat.Transform(tmpVec);

		{

			ChMat_11 camMat;

			camMat.SetRotation(testCamRotate);

			//camMat.CreateViewMatLookTarget(tmpVec, headPos->GetDrawLHandMatrix().GetPosition(), ChVec3(0.0f, 1.0f, 0.0f));
			camMat.CreateViewMatLookTarget(tmpVec, 0.0f, ChVec3(0.0f, 1.0f, 0.0f));

			meshDrawer.drawer.SetViewMatrix(camMat);

			light.SetCamPos(tmpVec);
		}

		light.SetDirectionLightData(true, ChVec3(1.0f, 1.0f, 1.0f), ChVec3(0.0f, -1.0f, 0.0f), 0.3f);

	}


}

void GameFrame::Init()
{
	script = ChPtr::Make_S<GameScript>();
	//InitScriptFunction();

	ChSystem::SysManager().SetFPS(BASE_FPS);

	PhysicsMachine::SetFPS(BASE_FPS);
	PhysicsMachine::SetGravityAcceleration(9.8f);
	PhysicsMachine::SetAirRegist(0.1f);


	

	windSize = ChVec2(static_cast<float>(GAME_WINDOW_WITDH), static_cast<float>(GAME_WINDOW_HEIGHT));

#if DEBUG_FLG


	//auto s_screen = ChWin::GetScreenSize();
	//windSize = ChVec2(static_cast<float>(s_screen.w), static_cast<float>(s_screen.h));

	{

		ChMat_11 proMat;
		proMat.CreateProjectionMat(ChMath::ToRadian(60.0f), windSize.w, windSize.h, 0.1f, 100000.0f);

		meshDrawer.drawer.Init(ChD3D11::D3D11Device());
		meshDrawer.drawer.SetProjectionMatrix(proMat);

		light.Init(ChD3D11::D3D11Device());

	}

#endif

	//windSize = ChVec2(static_cast<float>(GAME_WINDOW_WITDH), static_cast<float>(GAME_WINDOW_HEIGHT));
	
	//ChCpp::ChMultiThread thread;
	//thread.Init([&]() {  }) ;
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

	//thread.Join();


}

void GameFrame::InitScriptFunction()
{

	script->SetFunction("LoadBGM", [&](const std::string& _text) {
		auto argment = ChStr::Split(_text, " ");
		auto audio = ChPtr::Make_S< ChD3D::AudioObject>();

		std::string audioName = argment[0];

		ChD3D::XAudioManager().CreateSound(audio.get(), SOUND_DIRECTORY() + audioName);
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
			loader.CreateModel(mainMap->model, MESH_DIRECTORY() + argment[0]);
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
					mecha->SetTeam(ChStr::GetIntegialFromText<unsigned char>(argment[i]));
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

		auto&& mecha = mechaList.SetObject<BaseMecha>("enemyTest");

		mecha->Create(windSize, meshDrawer, this);

		//mecha->SetComponent<PlayerController>();

		//mecha->Load(ChD3D11::D3D11Device(), "AirRobot.amf");
		mecha->Load(ChD3D11::D3D11Device(), "NormalRobot.amf");
		//mecha->Load(ChD3D11::D3D11Device(), "AirRobot.amf");
		mecha->SetPosition(ChVec3(0.0f, 700.0f, 0.0f));
	}

	{

		auto&& mecha = mechaList.SetObject<BaseMecha>("player");

		mecha->Create(windSize, meshDrawer, this);

		mecha->SetComponent<PlayerController>();

		//mecha->Load(ChD3D11::D3D11Device(), "AirRobot.amf");
		mecha->Load(ChD3D11::D3D11Device(), "NormalRobot.amf");
		//mecha->Load(ChD3D11::D3D11Device(), "AirRobot.amf");
		mecha->SetPosition(ChVec3(0.0f, 700.0f, 0.0f));
		//mecha->Save("TestAsm.amf");

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
		windows->Release();
		return;
	}

	//TestUpdate();

	

	{
		auto tmpMat = meshDrawer.drawer.GetViewMatrix();
		tmpMat.Inverse();

		light.SetCamPos(tmpMat.GetPosition());

		light.SetDirectionLightData(true, ChVec3(1.0f, 1.0f, 1.0f), ChVec3(0.0f, -1.0f, 0.0f), 0.3f);


	}


#endif

	DrawFunction();
}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::UpdateFunction()
{

	mechaList.ObjectUpdateBegin();

	mechaList.ObjectUpdate();

	mechaList.ObjectUpdateEnd();

	mechaList.ObjectMove();

	mechaList.ObjectMoveEnd();

}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::DrawFunction()
{

	ChD3D11::Shader11().DrawStart();

	ChD3D11::Shader11().DrawStart3D();

	light.SetDrawData(ChD3D11::D3D11DC());

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

	meshDrawer.drawer.DrawStart(ChD3D11::D3D11DC());

	mechaList.ObjectDraw3D();


	for (auto mapModel : map)
	{
		ChMat_11 mapSizeMatrix;
		mapSizeMatrix.SetPosition(mapModel->position);
		mapSizeMatrix.SetRotation(mapModel->rotation);
		mapSizeMatrix.SetScaleSize(mapModel->scalling);
		meshDrawer.drawer.Draw(meshDrawer.dc,*mapModel->model, mapSizeMatrix);
	}

	meshDrawer.drawer.DrawEnd();
}

void GameFrame::AddMecha(ChPtr::Shared<BaseMecha> _mecha)
{
	mechaList.SetObject(_mecha);
}

void GameFrame::AddBullet(ChPtr::Shared<BulletObject> _bullet)
{
	bulletList.SetObject(_bullet);
}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::Render2D(void)
{

}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::CamUpdate(void)
{

}

