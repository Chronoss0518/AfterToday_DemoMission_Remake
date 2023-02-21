#pragma once

struct LookSquareValue
{
	ChCpp::MathSquare square;
	float distance = -1.0f;
};

class LookAnchor : public ChCpp::BaseComponent
{
public:

	inline void SetSize(const ChVec3& _size) { size = _size; }

	inline void SetDrawMatrix(const ChLMat& _drawMat) { drawMatrix = _drawMat; }

	std::vector<ChPtr::Shared<LookSquareValue>> GetMapSquares(const ChLMat& _viewMatrix, const ChLMat& _projectionMatrix);

private:

	ChVec3 size;

	ChLMat drawMatrix;

};

class MapLookAnchor:public ChCpp::BaseComponent
{
public:

	struct MapLookAnchorPosition
	{
		ChVec3 pos[4];
	};

	inline static void SetCutCount(unsigned long _cutCount)
	{
		CutCount() = _cutCount;
	}

	void SetPositionList(ChCpp::ModelObject& _model, const ChLMat& _drawMat);

	std::vector<ChPtr::Shared<LookSquareValue>> GetMapSquares(const ChLMat& _viewMatrix, const ChLMat& _projectionMatrix);

private:

	std::vector<ChPtr::Shared<MapLookAnchorPosition>> positoinList;

	inline static unsigned long& CutCount()
	{
		static unsigned long cutCount = 0;
		return cutCount;
	}

};

class CPUObjectLooker
{
public:

	void Init();

	void Release();

	void Update();

private:



};