#pragma once

namespace Test
{

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY("RobotParts/" current_path)
#endif


	ChPtr::Shared<MechaParts>head, weap, body, rArm, lArm, foot, boost;
	ChPtr::Shared<ChCpp::FrameObject> headPos, rArmPos, lArmPos, footPos, boostPos;
	std::vector<ChPtr::Shared<ChCpp::FrameObject>>boostList;

	//BaseMecha mecha;
	ChCpp::ObjectList mechas;
	float boostPow = 1.0f;
	float boostRotation = 0.0f;
	ChVec3 testCamRotate = ChVec3(-60.0f, 0.0f, 0.0f);
	float testCamLen = 10.0f;

	ChD3D11::CB::CBLight11 light;
	MeshDrawer meshDrawer;

	inline void Init()
	{

		auto s_screen = ChWin::GetScreenSize();

		ChMat_11 proMat;
		proMat.CreateProjectionMat(ChMath::ToRadian(60.0f), s_screen.w, s_screen.h, 0.1f, 1000.0f);

		meshDrawer.drawer.Init(ChD3D11::D3D11Device());
		meshDrawer.drawer.SetProjectionMatrix(proMat);

		light.Init(ChD3D11::D3D11Device());

		auto mecha = mechas.SetObject<BaseMecha>("player");

		mecha->Create(ChVec2(s_screen.w, s_screen.h), meshDrawer);

		mecha->SetComponent<PlayerController>();

		mecha->Load(ChD3D11::D3D11Device(), "NormalRobot.amf");

		/*
		*
		mechas.Save("NormalRobot.amf");
		return 0;
		body = ChPtr::Make_S<MechaParts>();
		body->Load(mechas, ChD3D11::D3D11Device(), PARTS_DIRECTORY("Body/Normal.mpd"));

		{
			auto headPosObj = body->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>("Armature_HeadPos");

			if (!headPosObj.empty())
			{
				headPos = headPosObj[0].lock();
				headPos->UpdateAllDrawTransform();

			}
		}

		{
			auto footPosObj = body->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>("Armature_BodyPos");

			if (!footPosObj.empty())
			{
				footPos = footPosObj[0].lock();
				footPos->UpdateAllDrawTransform();

			}
		}

		{
			auto boostPosObj = body->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>("Armature_BoostPos");

			if (!boostPosObj.empty())
			{
				boostPos = boostPosObj[0].lock();
				boostPos->UpdateAllDrawTransform();

			}
		}
		{
			auto rArmPosObj = body->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>("Armature_RightArmPos");

			if (!rArmPosObj.empty())
			{
				rArmPos = rArmPosObj[0].lock();
				rArmPos->UpdateAllDrawTransform();

			}
		}
		{
			auto lArmPosObj = body->GetMesh().GetAllChildlenForName<ChCpp::FrameObject>("Armature_LeftArmPos");

			if (!lArmPosObj.empty())
			{
				lArmPos = lArmPosObj[0].lock();
				lArmPos->UpdateAllDrawTransform();

			}
		}


		ChLMat tmp = headPos->GetDrawLHandMatrix();
		head = ChPtr::Make_S<MechaParts>();
		head->Load(mechas, ChD3D11::D3D11Device(), PARTS_DIRECTORY("Head/Normal.mpd"));
		head->GetMesh().SetFrameTransform(tmp);

		tmp = footPos->GetDrawLHandMatrix();
		foot = ChPtr::Make_S<MechaParts>();
		foot->Load(mechas, ChD3D11::D3D11Device(), PARTS_DIRECTORY("Foot/Normal.mpd"));
		foot->GetMesh().SetFrameTransform(tmp);

		tmp = lArmPos->GetDrawLHandMatrix();
		lArm = ChPtr::Make_S<MechaParts>();
		lArm->Load(mechas, ChD3D11::D3D11Device(), PARTS_DIRECTORY("LArm/Normal.mpd"));
		lArm->GetMesh().SetFrameTransform(tmp);

		tmp = rArmPos->GetDrawLHandMatrix();
		rArm = ChPtr::Make_S<MechaParts>();
		rArm->Load(mechas, ChD3D11::D3D11Device(), PARTS_DIRECTORY("RArm/Normal.mpd"));
		rArm->GetMesh().SetFrameTransform(tmp);

		tmp = boostPos->GetDrawLHandMatrix();
		boost = ChPtr::Make_S<MechaParts>();
		boost->Load(mechas, ChD3D11::D3D11Device(), PARTS_DIRECTORY("Boost/Normal.mpd"));
		boost->GetMesh().SetFrameTransform(tmp);

		{

			auto&& tmpBoostList =  boost->GetMesh().GetAllChildlenConstainsName<ChCpp::FrameObject>("Boost");
			ChLMat setMat;
			setMat.SetScalling(ChVec3(0.01f));
			for (auto boostObject : tmpBoostList)
			{
				if (boostObject.lock()->GetMyName() == "Boost")continue;
				auto tmp = boostObject.lock();
				boostList.push_back(tmp);

				tmp->SetOutSizdTransform(setMat);
			}
		}

		{

			auto&& tmpBoostList = foot->GetMesh().GetAllChildlenConstainsName<ChCpp::FrameObject>("Boost");
			ChLMat setMat;
			setMat.SetScalling(ChVec3(0.01f));
			for (auto boostObject : tmpBoostList)
			{
				if (boostObject.lock()->GetMyName() == "Boost")continue;
				auto tmp = boostObject.lock();
				boostList.push_back(tmp);

				tmp->SetOutSizdTransform(setMat);
			}
		}

		weap = ChPtr::Make_S<MechaParts>();
		weap->Load(mechas, ChD3D11::D3D11Device(), PARTS_DIRECTORY("Weapon/Gun/MF-TPP1.mpd"));

		*/
	}

	inline void Update()
	{

		auto windows = ChSystem::SysManager().GetSystem<ChSystem::Windows>();
		if (windows->IsPushKey(VK_ESCAPE))
		{
			windows->Release();
			return;
		}

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

		if (windows->IsPushKeyNoHold(VK_SPACE))
		{
			boostPow = 0.5f;
		}

		/*
		if (windows->IsPushKey(VK_SPACE))
		{
			float tmp = 1.0f - boostPow;
			boostPow += tmp * tmp * 0.1f;
			boostPow = boostPow > 1.0f ? 1.0f : boostPow;
		}
		else
		{
			float tmp = boostPow;
			boostPow -= tmp * tmp * 0.1f * 2.0f;
			boostPow = boostPow < 0.0f ? 0.0f : boostPow;
		}

		boostRotation += 5;
		boostRotation = boostRotation < 360.0f ? boostRotation : boostRotation - 360.0f;

		{
			ChLMat tmp;
			tmp.SetRotationYAxis(boostRotation);
			tmp.SetScalling(ChVec3(boostPow));
			for (auto boostObject : boostList)
			{
				boostObject->SetOutSizdTransform(tmp);
			}
		}

		*/

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
				camMat.CreateViewMatLookTarget(tmpVec, ChVec3(0.0f, 1.0f, 0.0f), ChVec3(0.0f, 1.0f, 0.0f));

				meshDrawer.drawer.SetViewMatrix(camMat);

				light.SetCamPos(tmpVec);
			}

			light.SetDirectionLightData(true, ChVec3(1.0f, 1.0f, 1.0f), ChVec3(0.0f, -1.0f, 0.0f), 0.3f);

		}

		mechas.ObjectUpdate();

		mechas.ObjectUpdateEnd();

		mechas.ObjectMove();

		ChD3D11::Shader11().DrawStart();

		ChD3D11::Shader11().DrawStart3D();
		light.SetDrawData(ChD3D11::D3D11DC());
		ChLMat mathMat;
		ChMat_11 tmpMat;
		meshDrawer.dc = ChD3D11::D3D11DC();
		meshDrawer.drawer.DrawStart(ChD3D11::D3D11DC());



		//mathMat.SetPosition(0.0f, -3.0f, 0.0f);
		//tmpMat = bodyPos->GetDrawLHandMatrix() * mathMat;
		mechas.ObjectDraw3D();
		//meshDrawer.Draw(ChD3D11::D3D11DC(), body->GetMesh(), tmpMat);
		//meshDrawer.Draw(ChD3D11::D3D11DC(), head->GetMesh(), tmpMat);
		//meshDrawer.Draw(ChD3D11::D3D11DC(), boost->GetMesh(), tmpMat);
		//meshDrawer.Draw(ChD3D11::D3D11DC(), foot->GetMesh(), tmpMat);
		//meshDrawer.Draw(ChD3D11::D3D11DC(), rArm->GetMesh(), tmpMat);
		//meshDrawer.Draw(ChD3D11::D3D11DC(), lArm->GetMesh(), tmpMat);

		meshDrawer.drawer.DrawEnd();
		ChD3D11::Shader11().DrawEnd();

	}


	inline void CreateSampleFile()
	{

#if 1
		//Sample
		head = ChPtr::Make_S<MechaParts>();

		head->SetHardness(2.0f);
		head->SetMass(2.0f);

		{

			auto camera = head->SetComponent<CameraData>();
			auto scope = head->SetComponent<ScopeData>();

			camera->SetCameraCount(1);
			camera->SetFovy(30.0f);

			scope->SetMaxFovy(45.0f);
			scope->SetMinFovy(10.0f);

			scope->SetFovySlideSpeed(5.0f);

		}

		head->Save(PARTS_DIRECTORY("Head/Guardian.mpd"));


		body = ChPtr::Make_S<MechaParts>();
		body->SetHardness(2.0f);
		body->SetMass(2.0f);

		{

			auto enelgyTank = body->SetComponent<EnelgyTankData>();

			enelgyTank->SetMaxEnelgy(500);
			enelgyTank->SetCreateEnelgy(20);

			auto&& headPos = body->SetComponent<HeadPos>();
			headPos->SetParamName("Armature_HeadPos");
			auto&& rArmPos = body->SetComponent<RightArmPos>();
			rArmPos->SetParamName("Armature_RightArmPos");
			auto&& lArmPos = body->SetComponent<LeftArmPos>();
			lArmPos->SetParamName("Armature_LeftArmPos");
			auto&& footPos = body->SetComponent<FootPos>();
			footPos->SetParamName("Armature_BodyPos");
			auto&& boostPos = body->SetComponent<BoostPos>();
			boostPos->SetParamName("Armature_BoostPos");
		}

		body->Save(PARTS_DIRECTORY("Body/Guardian.mpd"));

		foot = ChPtr::Make_S<MechaParts>();
		foot->SetHardness(2.0f);
		foot->SetMass(2.0f);
		{
			auto walk = foot->SetComponent<WalkData>();

			walk->SetMovePow(10.0f);
			walk->SetJumpPow(10.0f);
			walk->SetRotatePow(6.0f);

		}

		foot->Save(PARTS_DIRECTORY("Foot/Guardian.mpd"));


		boost = ChPtr::Make_S<MechaParts>();
		boost->SetHardness(2.0f);
		boost->SetMass(2.0f);

		{
			auto&& front = boost->SetComponent<FrontBoostBrust>();
			front->SetParamName("FrontBoost");
			front->SetBoostPower(30.0f);
			front->SetUseEnelgy(10);
			front->SetAvoidPower(100.0f);
			front->SetAvoidUseEnelgy(50);
			auto&& up = boost->SetComponent<UpBoostBrust>();
			up->SetParamName("JumpBoost");
			up->SetBoostPower(30.0f);
			up->SetUseEnelgy(10);
			up->SetAvoidPower(100.0f);
			up->SetAvoidUseEnelgy(50);
			auto&& left = boost->SetComponent<LeftBoostBrust>();
			left->SetParamName("LeftBoost");
			left->SetBoostPower(30.0f);
			left->SetUseEnelgy(10);
			left->SetAvoidPower(100.0f);
			left->SetAvoidUseEnelgy(50);
			auto&& right = boost->SetComponent<RightBoostBrust>();
			right->SetParamName("RightBoost");
			right->SetBoostPower(30.0f);
			right->SetUseEnelgy(10);
			right->SetAvoidPower(100.0f);
			right->SetAvoidUseEnelgy(50);

		}

		boost->Save(PARTS_DIRECTORY("Boost/Guardian.mpd"));

		rArm = ChPtr::Make_S<MechaParts>();
		rArm->SetHardness(2.0f);
		rArm->SetMass(2.0f);

		{

			auto rWeapons = rArm->SetComponent<HaveRightWeaponPos>();

			rWeapons->SetParamName("RightArm_WeaponPos");

		}
		rArm->Save(PARTS_DIRECTORY("RArm/Guardian.mpd"));



		lArm = ChPtr::Make_S<MechaParts>();
		lArm->SetHardness(2.0f);
		lArm->SetMass(2.0f);
		{

			auto lWeapons = lArm->SetComponent<HaveLeftWeaponPos>();

			lWeapons->SetParamName("LeftArm_WeaponPos");

		}
		lArm->Save(PARTS_DIRECTORY("LArm/Guardian.mpd"));


#endif

#if 0
		weap = ChPtr::Make_S<MechaParts>();
		weap->SetHardness(0.5f);
		weap->SetMass(0.5f);
		{
			auto gun = weap->SetComponent<GunData>();

			gun->SetBulletNum(30);
			gun->SetFireNum(1);
			gun->SetMagazineNum(3);
			gun->SetReloadTime(30);
			gun->SetWaitTime(3);

			weap->Save(PARTS_DIRECTORY("Weapon/Gun/MF-TPP1.mpd"));
		}
#endif 
	}
}
