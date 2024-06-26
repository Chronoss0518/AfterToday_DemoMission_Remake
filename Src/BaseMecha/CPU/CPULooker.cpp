#include"../../BaseIncluder.h"

#include<math.h>

#include"../../Frames/GameFrame.h"
#include"../BaseMecha.h"
#include"../Controller/ControllerBase.h"
#include"CPULooker.h"

#define FEATURE true
#define MATH_MATRIX true

#define TEST true

unsigned long resetNum = -1;

void LookAnchor::AddLookAnchorPosition(const ChVec3& _size, const ChLMat& _drawMat)
{
	auto&& lookAnchor = ChPtr::Make_S<LookAnchorPosition>();
	lookAnchor->size = _size;
	lookAnchor->drawMatrix = _drawMat;
	positionList.push_back(lookAnchor);
}

void LookAnchor::UpdateLookAnchorPosition(unsigned long _num, const ChLMat& _drawMat)
{
	if (_num >= positionList.size())return;

	positionList[_num]->drawMatrix = _drawMat;
}

void LookAnchor::RemoveLookAnchorPosition(unsigned long _num)
{
	if (_num >= positionList.size())return;

	auto findPos = positionList.begin() + _num;
	positionList.erase(findPos);
}

std::vector<ChPtr::Shared<LookSquareValue>> LookAnchor::GetMapSquares(const ChLMat& _viewMatrix, const ChLMat& _projectionMatrix)
{
	std::vector<ChPtr::Shared<LookSquareValue>> res;

	for (unsigned long i = 0; i < positionList.size(); i++)
	{
		auto&& anchorPos = positionList[i];

		ChVec4 square = ChVec4(10e+37f, (10e+37f) * -1.0f, (10e+37f) * -1.0f, 10e+37f);

		auto mathSquare = ChPtr::Make_S<LookSquareValue>();
		mathSquare->distance = (10e+37f) * -1.0f;

		ChVec4 pos[8]
		{
			ChVec4(-anchorPos->size.x,anchorPos->size.y,-anchorPos->size.z,1.0f),
			ChVec4(anchorPos->size.x,anchorPos->size.y,-anchorPos->size.z,1.0f),
			ChVec4(anchorPos->size.x,-anchorPos->size.y,-anchorPos->size.z,1.0f),
			ChVec4(-anchorPos->size.x,-anchorPos->size.y,-anchorPos->size.z,1.0f),
			ChVec4(-anchorPos->size.x,anchorPos->size.y,anchorPos->size.z,1.0f),
			ChVec4(anchorPos->size.x,anchorPos->size.y,anchorPos->size.z,1.0f),
			ChVec4(anchorPos->size.x,-anchorPos->size.y,anchorPos->size.z,1.0f),
			ChVec4(-anchorPos->size.x,-anchorPos->size.y,anchorPos->size.z,1.0f)
		};

		for (unsigned char j = 0; j < 8; j++)
		{
			auto&& position = pos[j];
			position = anchorPos->drawMatrix.Transform(position);
			position = _viewMatrix.Transform(position);
			position = _projectionMatrix.Transform(position);
			float w = position.w;
			position /= w;


			if (square.left > position.x)square.left = position.x;
			if (square.right < position.x)square.right = position.x;
			if (square.bottom > position.y)square.bottom = position.y;
			if (square.top < position.y)square.top = position.y;
			if (mathSquare->distance < position.z)mathSquare->distance = position.z;

		}

		if (mathSquare->distance < 0.0f)continue;
		if (mathSquare->distance > 1.0f)continue;

		mathSquare->square.AddSquare(square);
		mathSquare->square.And(ChVec4(-1.0f, 1.0f, 1.0f, -1.0f));

		if (mathSquare->square.GetCount() <= 0)continue;

		res.push_back(mathSquare);
	}

	return res;
}

std::vector<ChPtr::Shared<LookSquareValue>> LookAnchor::GetMapSquares(const ChLMat& _vpMatrix)
{

	std::vector<ChPtr::Shared<LookSquareValue>> res;

	for (unsigned long i = 0; i < positionList.size(); i++)
	{
		auto&& anchorPos = positionList[i];
		ChLMat tmpMat = anchorPos->drawMatrix * _vpMatrix;

		ChVec4 square = ChVec4(10e+37f, (10e+37f) * -1.0f, (10e+37f) * -1.0f, 10e+37f);

		auto mathSquare = ChPtr::Make_S<LookSquareValue>();
		mathSquare->distance = (10e+37f) * -1.0f;

		ChVec4 pos[8]
		{
			ChVec4(-anchorPos->size.x,anchorPos->size.y,-anchorPos->size.z,1.0f),
			ChVec4(anchorPos->size.x,anchorPos->size.y,-anchorPos->size.z,1.0f),
			ChVec4(anchorPos->size.x,-anchorPos->size.y,-anchorPos->size.z,1.0f),
			ChVec4(-anchorPos->size.x,-anchorPos->size.y,-anchorPos->size.z,1.0f),
			ChVec4(-anchorPos->size.x,anchorPos->size.y,anchorPos->size.z,1.0f),
			ChVec4(anchorPos->size.x,anchorPos->size.y,anchorPos->size.z,1.0f),
			ChVec4(anchorPos->size.x,-anchorPos->size.y,anchorPos->size.z,1.0f),
			ChVec4(-anchorPos->size.x,-anchorPos->size.y,anchorPos->size.z,1.0f)
		};

		for (unsigned char j = 0; j < 8; j++)
		{
			auto&& position = pos[j];
			position = tmpMat.Transform(position);
			float w = position.w;
			position /= w;
			position.z = w;


			if (square.left > position.x)square.left = position.x;
			if (square.right < position.x)square.right = position.x;
			if (square.bottom > position.y)square.bottom = position.y;
			if (square.top < position.y)square.top = position.y;
			if (mathSquare->distance < position.z)mathSquare->distance = position.z;

		}

		if (mathSquare->distance < 0.0f)continue;
		if (mathSquare->distance > 1.0f)continue;

		mathSquare->square.AddSquare(square);
		mathSquare->square.And(ChVec4(-1.0f, 1.0f, 1.0f, -1.0f));

		if (mathSquare->square.GetCount() <= 0)continue;

		res.push_back(mathSquare);
	}

	return res;

}

std::vector<ChPtr::Shared<LookSquareValue>> MapLookAnchor::GetMapSquares(const ChLMat& _viewMatrix, const ChLMat& _projectionMatrix)
{
	std::vector<ChPtr::Shared<LookSquareValue>> res;

	for (auto&& anchorObj : positionList)
	{
		for (auto&& anchorPos : anchorObj->anchors)
		{
			ChVec4 square = ChVec4(10e+37f, (10e+37f) * -1.0f, (10e+37f) * -1.0f, 10e+37f);

			ChVec4 pos[8]
			{
				ChVec4(anchorPos->min.x,anchorPos->max.y,anchorPos->min.z,1.0f),
				ChVec4(anchorPos->max.x,anchorPos->max.y,anchorPos->min.z,1.0f),
				ChVec4(anchorPos->max.x,anchorPos->min.y,anchorPos->min.z,1.0f),
				ChVec4(anchorPos->min.x,anchorPos->min.y,anchorPos->min.z,1.0f),
				ChVec4(anchorPos->min.x,anchorPos->max.y,anchorPos->max.z,1.0f),
				ChVec4(anchorPos->max.x,anchorPos->max.y,anchorPos->max.z,1.0f),
				ChVec4(anchorPos->max.x,anchorPos->min.y,anchorPos->max.z,1.0f),
				ChVec4(anchorPos->min.x,anchorPos->min.y,anchorPos->max.z,1.0f)
			};

			int frameList[8][3] = { {1,3,4},{0,2,5},{1,3,6},{0,2,7},{5,7,0},{4,6,1},{5,7,2},{4,6,3} };

			auto mathSquare = ChPtr::Make_S<LookSquareValue>();
			mathSquare->distance = (10e+37f) * -1.0f;

#if FEATURE

			ChVec2 anchorPos[4];

#endif

			float testMaxZ = (10e+37f) * -1.0f;
			float testMinZ = (10e+37f);
			for (unsigned char i = 0; i < 8; i++)
			{
				ChVec4& position = pos[i];
				position = anchorObj->drawMatrix.Transform(position);
				position = _viewMatrix.Transform(position);
				if (testMaxZ < position.z)testMaxZ = position.z;
				if (testMinZ > position.z)testMinZ = position.z;
			}

			for (unsigned char i = 0; i < 8; i++)
			{

				if (testMinZ > GAME_PROJECTION_FAR)break;
				if (testMaxZ < GAME_PROJECTION_NEAR)break;

				ChVec4& position = pos[i];
				float z = position.z;
				if (z < GAME_PROJECTION_NEAR)
				{

					ChVec3 normal = ChVec3();

					for (unsigned char j = 0; j < 3; j++)
					{
						if (normal.z > pos[frameList[i][j]].z)continue;
						normal = pos[frameList[i][j]];
					}
					normal = normal - position;
					z = normal.z;
					normal /= z;
					float length = (GAME_PROJECTION_NEAR + (GAME_PROJECTION_NEAR * 0.01f)) - position.z;
					normal *= length;
					position += normal;
					position.w = 1.0f;
				}
			}

			for (unsigned char i = 0; i < 8; i++)
			{
				if (testMinZ > GAME_PROJECTION_FAR)break;
				if (testMaxZ < GAME_PROJECTION_NEAR)break;

				ChVec4& position = pos[i];
				position = _projectionMatrix.Transform(position);
				float w = position.w;
				position /= w;


#if FEATURE

				if (anchorPos[ChStd::EnumCast(AnchorPosName::LeftTop)].x > position.x &&
					anchorPos[ChStd::EnumCast(AnchorPosName::LeftTop)].y < position.y)anchorPos[ChStd::EnumCast(AnchorPosName::LeftTop)] = position;
				if (anchorPos[ChStd::EnumCast(AnchorPosName::RightTop)].x < position.x &&
					anchorPos[ChStd::EnumCast(AnchorPosName::RightTop)].y < position.y)anchorPos[ChStd::EnumCast(AnchorPosName::RightTop)] = position;
				if (anchorPos[ChStd::EnumCast(AnchorPosName::RightBottom)].x < position.x &&
					anchorPos[ChStd::EnumCast(AnchorPosName::RightBottom)].y > position.y)anchorPos[ChStd::EnumCast(AnchorPosName::RightBottom)] = position;
				if (anchorPos[ChStd::EnumCast(AnchorPosName::LeftBottom)].x > position.x &&
					anchorPos[ChStd::EnumCast(AnchorPosName::LeftBottom)].y > position.y)anchorPos[ChStd::EnumCast(AnchorPosName::LeftBottom)] = position;

#else

				if (square.left > position.x)square.left = position.x;
				if (square.right < position.x)square.right = position.x;
				if (square.bottom > position.y)square.bottom = position.y;
				if (square.top < position.y)square.top = position.y;

#endif
				if (mathSquare->distance < position.z)mathSquare->distance = position.z;
			}

			if (mathSquare->distance < 0.0f)continue;
			if (mathSquare->distance > 1.0f)continue;

#if FEATURE

			mathSquare->square.SetSquare(
				anchorPos[ChStd::EnumCast(AnchorPosName::LeftTop)],
				anchorPos[ChStd::EnumCast(AnchorPosName::RightTop)],
				anchorPos[ChStd::EnumCast(AnchorPosName::RightBottom)],
				anchorPos[ChStd::EnumCast(AnchorPosName::LeftBottom)],
				CutCount());

#else

			mathSquare->square.SetSquare(square);

#endif
			mathSquare->square.And(ChVec4(-1.0f, 1.0f, 1.0f, -1.0f));

			if (mathSquare->square.GetCount() <= 0)continue;

			mathSquare->objectName = anchorObj->objectName;
			res.push_back(mathSquare);
		}
	}

	return res;

}

std::vector<ChPtr::Shared<LookSquareValue>> MapLookAnchor::GetMapSquares(const ChLMat& _vpMatrix)
{
	std::vector<ChPtr::Shared<LookSquareValue>> res;

	for (auto&& anchorObj : positionList)
	{
		ChLMat tmpMat = anchorObj->drawMatrix * _vpMatrix;

		for (auto&& anchorPos : anchorObj->anchors)
		{

			ChVec4 square = ChVec4(10e+37f, (10e+37f) * -1.0f, (10e+37f) * -1.0f, 10e+37f);

			ChVec4 pos[8]
			{
				ChVec4(anchorPos->min.x,anchorPos->max.y,anchorPos->max.z,1.0f),
				ChVec4(anchorPos->max.x,anchorPos->max.y,anchorPos->max.z,1.0f),
				ChVec4(anchorPos->max.x,anchorPos->min.y,anchorPos->max.z,1.0f),
				ChVec4(anchorPos->min.x,anchorPos->min.y,anchorPos->max.z,1.0f),
				ChVec4(anchorPos->min.x,anchorPos->max.y,anchorPos->min.z,1.0f),
				ChVec4(anchorPos->max.x,anchorPos->max.y,anchorPos->min.z,1.0f),
				ChVec4(anchorPos->max.x,anchorPos->min.y,anchorPos->min.z,1.0f),
				ChVec4(anchorPos->min.x,anchorPos->min.y,anchorPos->min.z,1.0f)
			};


			auto mathSquare = ChPtr::Make_S<LookSquareValue>();
			mathSquare->distance = (10e+37f);


#if FEATURE

			ChVec2 anchorPos[4];

#endif

			for (unsigned char i = 0; i < 8; i++)
			{
				ChVec4& position = pos[i];
				position = tmpMat.Transform(position);
				float w = position.w;
				position /= w;

				position.z = w;
#if FEATURE

				if (anchorPos[ChStd::EnumCast(AnchorPosName::LeftTop)].x > position.x &&
					anchorPos[ChStd::EnumCast(AnchorPosName::LeftTop)].y < position.y)anchorPos[ChStd::EnumCast(AnchorPosName::LeftTop)] = position;
				if (anchorPos[ChStd::EnumCast(AnchorPosName::RightTop)].x < position.x &&
					anchorPos[ChStd::EnumCast(AnchorPosName::RightTop)].y < position.y)anchorPos[ChStd::EnumCast(AnchorPosName::RightTop)] = position;
				if (anchorPos[ChStd::EnumCast(AnchorPosName::RightBottom)].x < position.x &&
					anchorPos[ChStd::EnumCast(AnchorPosName::RightBottom)].y > position.y)anchorPos[ChStd::EnumCast(AnchorPosName::RightBottom)] = position;
				if (anchorPos[ChStd::EnumCast(AnchorPosName::LeftBottom)].x > position.x &&
					anchorPos[ChStd::EnumCast(AnchorPosName::LeftBottom)].y > position.y)anchorPos[ChStd::EnumCast(AnchorPosName::LeftBottom)] = position;

#else

				if (square.left > position.x)square.left = position.x;
				if (square.right < position.x)square.right = position.x;
				if (square.bottom > position.y)square.bottom = position.y;
				if (square.top < position.y)square.top = position.y;

#endif
				if (mathSquare->distance > position.z)mathSquare->distance = position.z;
			}

			if (mathSquare->distance < 0.0f)continue;
			if (mathSquare->distance > 1.0f)continue;

#if FEATURE

			mathSquare->square.SetSquare(
				anchorPos[ChStd::EnumCast(AnchorPosName::LeftTop)],
				anchorPos[ChStd::EnumCast(AnchorPosName::RightTop)],
				anchorPos[ChStd::EnumCast(AnchorPosName::RightBottom)],
				anchorPos[ChStd::EnumCast(AnchorPosName::LeftBottom)],
				CutCount());

#else

			mathSquare->square.SetSquare(square);

#endif
			mathSquare->square.And(ChVec4(-1.0f, 1.0f, 1.0f, -1.0f));

			if (mathSquare->square.GetCount() <= 0)continue;


			res.push_back(mathSquare);
		}
	}


	return res;

}

void MapLookAnchor::SetPositionList(ChCpp::FrameObject& _model, const ChLMat& _drawMat)
{
	CreateFramePosition(_model, _drawMat);

	for (auto&& frame : _model.GetChildlen<ChCpp::FrameObject>())
	{
		auto&& frameObject = frame.lock();
		if (frameObject == nullptr)continue;

		SetPositionList(*frameObject, _drawMat);
	}
}

void MapLookAnchor::CreateFramePosition(ChCpp::FrameObject& _frame, const ChLMat& _drawMat)
{
	ChLMat drawMatrix = _frame.GetDrawLHandMatrix() * _drawMat;
	//ChLMat drawMatrix = _drawMat;

	auto&& frameCom = _frame.GetComponent<ChCpp::FrameComponent>();

	if (frameCom == nullptr)return;

	auto&& vertexList = frameCom->vertexList;

	auto lookAnchor = ChPtr::Make_S<MapLookAnchorObject>();

	lookAnchor->drawMatrix = drawMatrix;

	for (auto&& primitive : frameCom->primitives)
	{
		if (frameCom->primitives.size() <= 2)continue;

		auto&& anchorPos = ChPtr::Make_S<MapLookAnchorPosition>();

		for (auto&& vertexData : primitive->vertexData)
		{
			auto&& pos = vertexList[vertexData->vertexNo]->pos;
			//auto&& pos = _frame.GetDrawLHandMatrix().Transform(vertexList[vertexData->vertexNo]->pos);
			//auto&& pos = drawMatrix.Transform(vertexList[vertexData->vertexNo]->pos);

			if (anchorPos->min.x >= pos.x)anchorPos->min.x = pos.x;
			if (anchorPos->min.y >= pos.y)anchorPos->min.y = pos.y;
			if (anchorPos->min.z >= pos.z)anchorPos->min.z = pos.z;

			if (anchorPos->max.x <= pos.x)anchorPos->max.x = pos.x;
			if (anchorPos->max.y <= pos.y)anchorPos->max.y = pos.y;
			if (anchorPos->max.z <= pos.z)anchorPos->max.z = pos.z;

		}

		lookAnchor->anchors.push_back(anchorPos);
	}

	lookAnchor->objectName = _frame.GetMyName();
	positionList.push_back(lookAnchor);

}

unsigned long CPUObjectLooker::GetLookTypeMechas(MemberType _member, DistanceType _distance, DamageSizeType _damageSize)
{
	return lookMechaTypes[ChStd::EnumCast(_member)][ChStd::EnumCast(_distance)][ChStd::EnumCast(_damageSize)];
}

void CPUObjectLooker::Init()
{

	endFlg = false;

	updateFlg = false;

#if USE_CPU_THREAD
	thread.Init([&]()
		{
			while (!endFlg)
			{

				if (!updateFlg)continue;
				FindMecha();

				updateFlg = false;

				//OutputDebugString(("CPUObjectLooker End Time: " + std::to_string(timeGetTime() - time) + "\n").c_str());
			}
		});
#endif

	auto device = ChD3D11::D3D11Device();

	spriteDrawer.Init(device);

	spriteDrawer.SetAlphaBlendFlg(true);

	ChVec4 tmp = ChVec4(0.0f, 1.0f, 0.0f, 0.2f);

	mechaTexture.CreateColorTexture(&tmp, 1, 1);

	tmp = ChVec4(1.0f, 0.0f, 0.0f, 0.2f);
	mapTexture_Cube.CreateColorTexture(&tmp, 1, 1);

	tmp = ChVec4(1.0f, 1.0f, 0.0f, 0.2f);
	mapTexture_Cube_001.CreateColorTexture(&tmp, 1, 1);

	tmp = ChVec4(1.0f, 0.0f, 1.0f, 0.2f);
	mapTexture_Cube_002.CreateColorTexture(&tmp, 1, 1);

	tmp = ChVec4(0.0f, 1.0f, 1.0f, 0.2f);
	mapTexture_Plane_002.CreateColorTexture(&tmp, 1, 1);

	//drawPosition.SetSize(ChVec2(0.2f, 0.2f));
	drawPosition.SetSize(ChVec2(1.0f, 1.0f));

	drawPosition.SetDrawDepth(0.0f, 1.0f);
	//drawPosition.SetTopLeftPos(ChVec2(0.6f, 0.7f));
	drawPosition.SetTopLeftPos(ChVec2(-1.0f, 1.0f));

	sprite.Init();
	sprite.SetInitPosition();
}

void CPUObjectLooker::DrawBegin()
{
	nowUpdateCount = (nowUpdateCount + 1) % updateCount;
	if(!lookMechaList.empty())lookMechaList.clear();

	if (nowUpdateCount <= 0)
	{
		auto time = timeGetTime();
		for (unsigned char memberType = 0; memberType < MEMBER_TYPE_COUNT; memberType++)
		{
			for (unsigned char distanceType = 0; distanceType < DISTANCE_TYPE_COUNT; distanceType++)
			{
				for (unsigned char damageSizeType = 0; damageSizeType < DAMAGE_SIZE_TYPE_COUNT; damageSizeType++)
				{
					lookMechaTypes[memberType][distanceType][damageSizeType] = resetNum;
				}
			}
		}
#if USE_CPU_THREAD
		updateFlg = true;
#else

		FindMecha();
#endif

		//OutputDebugString(("CPUObjectLooker End Time: " + std::to_string(timeGetTime() - time) + "\n").c_str());
	}


}

void CPUObjectLooker::Draw2D()
{

	auto dc = ChD3D11::D3D11DC();

	unsigned int viewPortNum = 0;
	D3D11_VIEWPORT viewPort;
	dc->RSGetViewports(&viewPortNum, &viewPort);
	//drawPosition.SetDrawData(dc);

	ID3D11RenderTargetView* rt = nullptr;
	ID3D11DepthStencilView* dp = nullptr;

	dc->OMGetRenderTargets(1, &rt, &dp);

	dc->OMSetRenderTargets(1, &rt, nullptr);


	spriteDrawer.DrawStart(dc);
	for (auto&& map : lookMaps)
	{
		for (auto&& square : map->square.GetSquare())
		{

			sprite.SetPos(ChD3D11::SpritePositionName::LeftTop, ChVec2(square->left, square->top));
			sprite.SetPos(ChD3D11::SpritePositionName::RightTop, ChVec2(square->right, square->top));
			sprite.SetPos(ChD3D11::SpritePositionName::RightBottom, ChVec2(square->right, square->bottom));
			sprite.SetPos(ChD3D11::SpritePositionName::LeftBottom, ChVec2(square->left, square->bottom));

			ChD3D11::TextureBase11* base = &mapTexture_Cube;

			if (map->objectName == "Cube")base = &mapTexture_Cube;
			if (map->objectName == "Cube_001")base = &mapTexture_Cube_001;
			if (map->objectName == "Cube_002")base = &mapTexture_Cube_002;
			if (map->objectName == "Plane_002")base = &mapTexture_Plane_002;

			spriteDrawer.Draw(*base, sprite);

		}
	}

	for (auto&& mecha : lookMecas)
	{
		for (auto&& value : mecha->values)
		{
			for (auto&& square : value->square.GetSquare())
			{
				sprite.SetPos(ChD3D11::SpritePositionName::LeftTop, ChVec2(square->left, square->top));
				sprite.SetPos(ChD3D11::SpritePositionName::RightTop, ChVec2(square->right, square->top));
				sprite.SetPos(ChD3D11::SpritePositionName::RightBottom, ChVec2(square->right, square->bottom));
				sprite.SetPos(ChD3D11::SpritePositionName::LeftBottom, ChVec2(square->left, square->bottom));

				spriteDrawer.Draw(mechaTexture, sprite);

			}
		}
	}

	spriteDrawer.DrawEnd();

	dc->RSSetViewports(viewPortNum, &viewPort);


	dc->OMSetRenderTargets(1, &rt, dp);

	lookMecas.clear();

}

void CPUObjectLooker::DrawEnd()
{
	while (!IsEndUpdate())continue;
}

void CPUObjectLooker::Release()
{
	endFlg = true;
	thread.Release();
}

#if TEST

void CPUObjectLooker::FindMecha()
{

	auto mecha = LookObj<BaseMecha>();

	if (ChPtr::NullCheck(mecha))
	{
		Destroy();
		updateFlg = false;
		return;
	}

	if (mecha->IsBreak())
	{
		updateFlg = false;
		return;
	}

	ChVec3 pos = mecha->GetPosition();

	auto&& hitTestMap = frame->GetHitMapList();

	auto&& baseMechaList = frame->GetMechas();

	float nearLength[2] = { 10e+37f,10e+37f };
	float farLength[2] = { 0.0f ,0.0f };

	ray.SetPosition(LookObj<BaseMecha>()->GetPosition());

	ChLMat vpMat = viewMatrix * projectionMatrix;

	for (unsigned long i = 0; i < baseMechaList.size(); i++)
	{
		auto&& tmpMecha = baseMechaList[i];
		if (tmpMecha.expired())continue;
		auto&& otherMecha = tmpMecha.lock();
		if (otherMecha->GetMechaNo() == LookObj<BaseMecha>()->GetMechaNo())continue;
		unsigned long memberType = ChStd::EnumCast(MemberType::Enemy);
		auto controller = otherMecha->GetComponent<ControllerBase>();
		if (controller == nullptr)continue;
		if (controller->GetTeamNo() == mecha->GetTeamNo()) memberType = ChStd::EnumCast(MemberType::Member);

		ChVec3 targetPos = otherMecha->GetPosition();

		{
			ChVec4 tmp = vpMat.Transform(targetPos);

			tmp /= tmp.w;

			if (tmp.z < 0.0f || tmp.z > 1.0f)continue;

			tmp *= tmp;

			if (tmp.x > 1.0f || tmp.y > 1.0f)continue;
		}

		ChVec3 direction = targetPos - pos;

		float tmpLength = direction.Len();

		direction.Normalize();

		ray.SetRayDir(direction);

		bool lookFlg = true;

		for (auto&& map : hitTestMap)
		{
			auto collider = map->GetComponent<MapCollider>();

			if (collider == nullptr)continue;

			if (!collider->GetCollider().IsHit(&ray))continue;

			if (ray.GetHitVectol().Len() > tmpLength)continue;

			lookFlg = false;
			break;
		}

		if (!lookFlg)continue;

		if (memberType == ChStd::EnumCast(MemberType::Enemy))
		{
			lookMechaList.push_back(i);
		}

		MenyDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::None)][ChStd::EnumCast(DamageSizeType::Many)], i, baseMechaList);
		FewDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::None)][ChStd::EnumCast(DamageSizeType::Few)], i, baseMechaList);

		if (nearLength[memberType] > tmpLength)
		{
			nearLength[memberType] = tmpLength;

			lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Near)][ChStd::EnumCast(DamageSizeType::None)] = i;
			MenyDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Near)][ChStd::EnumCast(DamageSizeType::Many)], i, baseMechaList);
			FewDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Near)][ChStd::EnumCast(DamageSizeType::Few)], i, baseMechaList);

		}

		if (farLength[memberType] < tmpLength)
		{
			farLength[memberType] = tmpLength;

			lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Far)][ChStd::EnumCast(DamageSizeType::None)] = i;
			MenyDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Far)][ChStd::EnumCast(DamageSizeType::Many)], i, baseMechaList);
			FewDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Far)][ChStd::EnumCast(DamageSizeType::Few)], i, baseMechaList);
		}

	}

}

#else

void CPUObjectLooker::FindMecha()
{
	auto mecha = LookObj<BaseMecha>();

	if (ChPtr::NullCheck(mecha))
	{
		Destroy();
		updateFlg = false;
		return;
	}

	if (mecha->IsBreak())
	{
		updateFlg = false;
		return;
	}


#if MATH_MATRIX
	ChLMat vpMatrix = viewMatrix * projectionMatrix;
#endif 

#if !MATH_MATRIX
	auto&& mapSquares = frame->GetLookSquareValuesFromMap(viewMatrix, projectionMatrix);
#else
	auto&& mapSquares = frame->GetLookSquareValuesFromMap(vpMatrix);
#endif

	lookMaps = mapSquares;

	std::vector<ChPtr::Shared<UseSquareValues>> testMechaSquares;

	auto&& baseMechaList = frame->GetMechas();

	for (unsigned long i = 0; i < baseMechaList.size(); i++)
	{

		auto&& otherMecha = baseMechaList[i];

		auto otherMechaObject = otherMecha.lock();
		if (otherMechaObject == nullptr)continue;
		if (otherMechaObject.get() == mecha)continue;
		if (otherMechaObject->IsBreak())continue;


		auto&& lookAnchor = otherMechaObject->GetComponent<LookAnchor>();

		if (lookAnchor == nullptr)continue;

#if !MATH_MATRIX
		auto&& squareValue = lookAnchor->GetMapSquares(viewMatrix, projectionMatrix);
#else
		auto&& squareValue = lookAnchor->GetMapSquares(vpMatrix);
#endif

		if (squareValue.size() <= 0)continue;

		auto useSquareValues = ChPtr::Make_S<UseSquareValues>();

		useSquareValues->otherMecha = otherMecha;
		useSquareValues->values = squareValue;

		for (auto& value : squareValue)
		{
			if (useSquareValues->nearDistance < value->distance)continue;
			useSquareValues->nearDistance = value->distance;
		}

		if (testMechaSquares.empty())
		{
			testMechaSquares.push_back(useSquareValues);
			continue;
		}

		for (unsigned long i = 0; i < testMechaSquares.size(); i++)
		{
			if (useSquareValues->nearDistance > testMechaSquares[i]->nearDistance)continue;

			testMechaSquares.insert(testMechaSquares.begin() + i, useSquareValues);
			break;
		}

	}


	float nearLength = 10e+37f;
	float farLength = 0.0f;

	for (unsigned long i = 0; i < testMechaSquares.size(); i++)
	{
		auto mechaSquare = testMechaSquares[i];
		unsigned long memberType = ChStd::EnumCast(MemberType::Enemy);
		auto otherMechaObject = mechaSquare->otherMecha.lock();
		auto controller = otherMechaObject->GetComponent<ControllerBase>();
		if (controller == nullptr)continue;
		if (controller->GetTeamNo() == mecha->GetTeamNo()) memberType = ChStd::EnumCast(MemberType::Member);

		bool lookFlg = true;

		bool isTestFlg = false;

		{
			unsigned long squareZeroCount = 0;

			for (auto&& mechaS : mechaSquare->values)
			{
				if (mechaS->square.GetCount() <= 0)
				{
					squareZeroCount++;
					continue;
				}

				for (auto&& mapS : mapSquares)
				{
					if (mechaS->distance < mapS->distance)continue;
					isTestFlg = true;
					mechaS->square.Sub(mapS->square);
					if (mechaS->square.GetCount() > 0)continue;
					squareZeroCount++;
					break;
				}

			}

			if (squareZeroCount < mechaSquare->values.size())lookFlg = false;

		}


		if (!isTestFlg)lookFlg = true;

		if (!lookFlg)continue;


#if false

		lookFlg = true;

		if (testMechaSquares.size() <= 1)
		{
			for (auto&& mechaS : mechaSquare->values)
			{
				lookFlg = false;
				if (mechaS->square.GetCount() <= 0)continue;
				lookFlg = true;
				break;
			}
		}
		else
		{
			for (unsigned long j = 0; j < testMechaSquares.size(); j++)
			{
				if (i == j)continue;
				for (auto&& mechaS : mechaSquare->values)
				{
					if (mechaS->square.GetCount() <= 0)continue;
					if (testMechaSquares[j]->nearDistance > testMechaSquares[i]->nearDistance)continue;
					for (auto&& otherMechaS : testMechaSquares[j]->values)
					{
						mechaS->square.Sub(otherMechaS->square);

						if (mechaS->square.GetCount() > 0)continue;
						lookFlg = false;
					}
				}
			}
		}

		if (!lookFlg)continue;

#endif

		ChVec3 targetPos = otherMechaObject->GetPosition();

		MenyDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::None)][ChStd::EnumCast(DamageSizeType::Many)], i, baseMechaList);
		FewDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::None)][ChStd::EnumCast(DamageSizeType::Few)], i, baseMechaList);

		float tmpLength = ChVec3::GetLen(targetPos, mecha->GetPosition());

		if (nearLength > tmpLength)
		{
			nearLength = tmpLength;

			lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Near)][ChStd::EnumCast(DamageSizeType::None)] = i;
			MenyDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Near)][ChStd::EnumCast(DamageSizeType::Many)], i, baseMechaList);
			FewDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Near)][ChStd::EnumCast(DamageSizeType::Few)], i, baseMechaList);

		}

		if (farLength < tmpLength)
		{
			farLength = tmpLength;

			lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Far)][ChStd::EnumCast(DamageSizeType::None)] = i;
			MenyDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Far)][ChStd::EnumCast(DamageSizeType::Many)], i, baseMechaList);
			FewDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Far)][ChStd::EnumCast(DamageSizeType::Few)], i, baseMechaList);

		}

		lookMecas.push_back(testMechaSquares[i]);

	}
}

#endif

void CPUObjectLooker::MenyDamageTest(unsigned long& _base, unsigned long _target, std::vector<ChPtr::Weak<BaseMecha>>& _mechas)
{
	if (_target >= _mechas.size())return;

	auto target = _mechas[_target].lock();
	if (target == nullptr)return;

	if (_base >= _mechas.size())
	{
		_base = _target;
		return;
	}

	auto base = _mechas[_base].lock();
	if (base == nullptr)
	{
		_base = _target;
		return;
	}

	if (base->GetDamage() >= target->GetDamage())return;
	_base = _target;
}

void CPUObjectLooker::FewDamageTest(unsigned long& _base, unsigned long _target, std::vector<ChPtr::Weak<BaseMecha>>& _mechas)
{
	if (_target >= _mechas.size())return;

	auto target = _mechas[_target].lock();
	if (target == nullptr)return;

	if (_base >= _mechas.size())
	{
		_base = _target;
		return;
	}

	auto base = _mechas[_base].lock();
	if (base == nullptr)
	{
		_base = _target;
		return;
	}

	if (base->GetDamage() <= target->GetDamage())return;
	_base = _target;
}
