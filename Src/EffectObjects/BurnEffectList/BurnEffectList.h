#pragma once


class BurnEffectList
{
private:

	struct BurnEffectObject
	{
		ChVec3 position;
		ChVec2 animationDirection;
		float animationTime;
	};


public:

	void Init(ID3D11Device* _device, const unsigned long _maxCount);

	void Release();

public:

	bool IsUpdateFlg();

public:

	void AddBurnEffect(const ChVec3& _pos,const ChVec2& _animationDir);

public:

	void Update();

public:

	void Draw(ChD3D11::Shader::BaseDrawMesh11& _drawer);

private:

	ChCpp::ObjectList burnObject;
	unsigned long nowCount = 0;
};
