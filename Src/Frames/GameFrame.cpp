#include"../BaseIncluder.h"


#include"../AllStruct.h"
#include"../BaseMecha/BaseMecha.h"
#include"../GameScript/GameScript.h"
#include"GameFrame.h"

#include"../BaseMecha/Controller/ControllerBase.h"

#include"../CloudObj/CloudObj.h"

#define PLAYER_MECHA_FILE_NAME ""

#define GAME_WINDOW_WITDH 3840
#define GAME_WINDOW_HEIGHT 2160
#define FPS 60
#define DEBUG_FLG 1


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

	ChSystem::SysManager().SetFPS(FPS);

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
	
	ChCpp::ChMultiThread thread;
	thread.Init([&]() { LoadMechas(); }) ;
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

	thread.Join();


}

void GameFrame::InitScriptFunction()
{
	script->SetFunction("Load", [&](const std::string _text)
		{
			unsigned long nowPos = 0;
			unsigned long nextPos = 0;
			std::vector<std::string>argment;

			while (1)
			{
				nextPos = _text.find(" ", nowPos + 1);
				argment.push_back(_text.substr(nowPos, nextPos - nowPos));
				if (nextPos >= _text.size())break;
				nowPos = _text.find(" ", nextPos + 1);
			}

			


		});
}

void GameFrame::LoadScript(const std::string& _text)
{
	ChCpp::TextObject text;
	text.SetText(_text);

}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::LoadMechas()
{
	auto&& mecha = mechaList.SetObject<BaseMecha>("player");

	mecha->Create(windSize, meshDrawer);

	mecha->SetComponent<PlayerController>();

	mecha->Load(ChD3D11::D3D11Device(), "NormalRobot.amf");
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
	auto mainMap = ChPtr::Make_S<ChD3D11::Mesh11>();
	mainMap->Init(ChD3D11::D3D11Device());
	{
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(mainMap, MESH_DIRECTORY("TestField.x"));
	}
	map.push_back(mainMap);

	auto subMap = ChPtr::Make_S<ChD3D11::Mesh11>();
	subMap->Init(ChD3D11::D3D11Device());
	{
		ChCpp::ModelLoader::XFile loader;
		loader.CreateModel(subMap, MESH_DIRECTORY("TestField2.x"));
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

	return;

	{
		auto&& rob = BaseMecha::GetCameraFromMecha();

		ChLMat tmpMat;
		tmpMat.SetRotationYAxis(rob.GetRotation().y);
		tmpMat.SetRotationXAxis(rob.GetRotation().x);

		ChVec3 dir = tmpMat.TransformCoord(ChVec3(0.0f, 0.0f, 1.0f));

		ChVec3 up = tmpMat.TransformCoord(ChVec3(0.0f, 1.0f, 0.0f));

		/*
		viewMat.CreateViewMat(rob.GetPosition(), dir, up);

		proMat.CreateProjectionMat(
			60.0f,//rob.GetFovy(),
			GAME_WINDOW_WITDH,
			GAME_WINDOW_HEIGHT,
			0.1f,
			10000.0f);
		*/
	}

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
}

///////////////////////////////////////////////////////////////////////////////////

void GameFrame::Render3D(void)
{
	meshDrawer.dc = ChD3D11::D3D11DC();

	meshDrawer.drawer.DrawStart(ChD3D11::D3D11DC());

	mechaList.ObjectDraw3D();


	ChMat_11 mapSizeMatrix;
	mapSizeMatrix.SetPosition(ChVec3(0.0f, -10.0f, 0.0f));
	mapSizeMatrix.SetScaleSize(ChVec3(100.0f));
	for (auto mapModel : map)
	{
		meshDrawer.drawer.Draw(meshDrawer.dc,*mapModel, mapSizeMatrix);
	}

	meshDrawer.drawer.DrawEnd();
}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::Render2D(void)
{

}

//////////////////////////////////////////////////////////////////////////////////

void GameFrame::CamUpdate(void)
{

}

