#pragma once

class GameFrame;
class BaseMecha;

struct LookSquareValue
{
	ChCpp::MathSquare square = ChCpp::MathSquare();
	float distance = (10e+37f) * -1.0f;
	std::string objectName = "";
};

class LookAnchor : public ChCpp::BaseComponent
{
public:

	void AddLookAnchorPosition(const ChVec3& _size, const ChLMat& _drawMat);

	void RemoveLookAnchorPosition(unsigned long _num);

	void UpdateLookAnchorPosition(unsigned long _num, const ChLMat& _drawMat);

	inline unsigned long GetPositionListCount() { return positionList.size(); }

	std::vector<ChPtr::Shared<LookSquareValue>> GetMapSquares(const ChLMat& _viewMatrix, const ChLMat& _projectionMatrix);

	std::vector<ChPtr::Shared<LookSquareValue>> GetMapSquares(const ChLMat& _vpMatrix);

private:

	struct LookAnchorPosition
	{
		ChVec3 size = ChVec3();
		ChLMat drawMatrix = ChLMat();
	};

	std::vector<ChPtr::Shared<LookAnchorPosition>> positionList;

};

class MapLookAnchor:public ChCpp::BaseComponent
{
public:

	enum class AnchorPosName
	{
		LeftTop,
		RightTop,
		RightBottom,
		LeftBottom
	};

	struct MapLookAnchorPosition
	{
		ChVec3 min = ChVec3(10e+37f);
		ChVec3 max = ChVec3((10e+37f) * -1.0f);
	};

	struct MapLookAnchorObject 
	{
		std::vector<ChPtr::Shared<MapLookAnchorPosition>> anchors;
		ChLMat drawMatrix = ChLMat();
		std::string objectName = "";
	};


	inline static void SetCutCount(unsigned long _cutCount)
	{
		if (_cutCount <= 0)return;
		CutCount() = _cutCount;
	}

	void SetPositionList(ChCpp::FrameObject& _model, const ChLMat& _drawMat);

	std::vector<ChPtr::Shared<LookSquareValue>> GetMapSquares(const ChLMat& _viewMatrix, const ChLMat& _projectionMatrix);

	std::vector<ChPtr::Shared<LookSquareValue>> GetMapSquares(const ChLMat& _vpMatrix);

private:

	void CreateFramePosition(ChCpp::FrameObject& _frame, const ChLMat& _drawMat);

	std::vector<ChPtr::Shared<MapLookAnchorObject>> positionList;

	inline static unsigned long& CutCount()
	{
		static unsigned long cutCount = 0;
		return cutCount;
	}

};

class CPUObjectLooker:public ChCpp::BaseComponent
{
public:

	struct UseSquareValues
	{
		std::vector<ChPtr::Shared<LookSquareValue>> values;
		ChPtr::Weak<BaseMecha> otherMecha;
		float nearDistance = 1e+38f;
	};

	enum class MemberType : unsigned char
	{
		Member,
		Enemy,
		Other,
	};

	static const unsigned char MEMBER_TYPE_COUNT = 3;

	enum class DistanceType : unsigned char
	{
		Near,
		Far,
		None
	};

	static const unsigned char DISTANCE_TYPE_COUNT = 3;

	enum class DamageSizeType : unsigned char
	{
		Many,
		Few,
		None
	};

	static const unsigned char DAMAGE_SIZE_TYPE_COUNT = 3;

public:

	void Init()override;

	void Draw2D()override;

	void DrawEnd()override;

	void Release()override;

public:

	inline void SetGameFrame(GameFrame* _frame) { frame = _frame; }

	inline void SetViewMatrix(const ChLMat& _mat) { viewMatrix = _mat; }

	inline void SetProjectionMatrix(const ChLMat& _mat) { projectionMatrix = _mat; }

	inline void UpdateCount(const unsigned long _updateCount) { updateCount = _updateCount; }

public:
	
	ChStd::Bool IsEndUpdate() { return updateFlg == 0; }

public:

	ChPtr::Shared<CPUObjectLooker::UseSquareValues>& GetLookTypeMechas(MemberType _member, DistanceType _distance, DamageSizeType _damageSize);

private:
	void FindMecha();

	void MenyDamageTest(ChPtr::Shared<UseSquareValues>& _base, ChPtr::Shared<UseSquareValues>& _target);

	void FewDamageTest(ChPtr::Shared<UseSquareValues>& _base, ChPtr::Shared<UseSquareValues>& _target);

	GameFrame* frame = nullptr;
	ChLMat projectionMatrix;
	ChLMat viewMatrix;

	ChCpp::MultiThread thread;
	ChStd::Bool endFlg = false;
	ChStd::Bool updateFlg = false;

	unsigned long updateCount = 1;
	unsigned long nowUpdateCount = 0;

	ChPtr::Shared<UseSquareValues> lookMechaTypes[MEMBER_TYPE_COUNT][DISTANCE_TYPE_COUNT][DAMAGE_SIZE_TYPE_COUNT];
	
	std::vector<ChPtr::Shared<UseSquareValues>>lookMecas;
	std::vector<ChPtr::Shared<LookSquareValue>>lookMaps;
	ChD3D11::Texture11 mechaTexture;

	ChD3D11::Texture11 mapTexture_Cube;
	ChD3D11::Texture11 mapTexture_Cube_001;
	ChD3D11::Texture11 mapTexture_Cube_002;
	ChD3D11::Texture11 mapTexture_Plane_002;

	ChD3D11::ShaderParts::ViewPort drawPosition;

	ChD3D11::Sprite11 sprite;

	ChD3D11::Shader::BaseDrawSprite11 spriteDrawer;

	std::vector<ChPtr::Weak<BaseMecha>>lookMechaList;

};