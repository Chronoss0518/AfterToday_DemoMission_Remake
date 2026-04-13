#include"BaseIncluder.h"

#include<float.h>

void ReleaseMesh11(ChPtr::Shared<ChD3D11::Mesh11<wchar_t>>& _meshObject)
{
	if (_meshObject == nullptr)return;
	_meshObject->Release();
	_meshObject = nullptr;
}

float HorizontalToProjection(float _x)
{
	return SCREEN_TO_PROJECTION_X(_x);
}

float VerticalToProjection(float _y)
{
	return SCREEN_TO_PROJECTION_Y(_y);
}

ChVec4 RectToGameWindow(const ChVec4 _rect)
{
	ChVec4 res = _rect;

	res.left = HorizontalToProjection(res.left);
	res.right = HorizontalToProjection(res.right);

	res.top = VerticalToProjection(res.top);
	res.bottom = VerticalToProjection(res.bottom);

	return res;
}

ChVec3 ColorTextToColorVector3(
	const std::wstring& _r,
	const std::wstring& _g,
	const std::wstring& _b)
{
	return ChVec3::FromColor(
		TEXT_TO_COLOR_NUMBER(_r),
		TEXT_TO_COLOR_NUMBER(_g),
		TEXT_TO_COLOR_NUMBER(_b)
	);
}

bool IsMoucePosOnRect(const ChVec4& _rect)
{
	ChVec2 mousePos = ChWin::Mouse().GetNowProPosToChVec2();

	bool res = false;

	if (_rect.IsOnPoint(mousePos))res = true;

	return res;
}

bool IsMoucePosOnSprite(const ChD3D11::Sprite11& _sprite)
{
	ChVec2 lt = _sprite.GetPos(ChD3D11::SpritePositionName::LeftTop);
	ChVec2 rb = _sprite.GetPos(ChD3D11::SpritePositionName::RightBottom);

	ChVec4 tmpRect;
	tmpRect.left = lt.x;
	tmpRect.top = lt.y;
	tmpRect.right = rb.x;
	tmpRect.bottom = rb.y;

	return IsMoucePosOnRect(tmpRect);
}

std::wstring CreateMoneyText(const std::wstring& _money)
{
	std::wstring result = L"";

	for (unsigned long i = 0; i < _money.length(); i++)
	{
		result += _money[i];
		if (i + 1 >= _money.length())break;
		if ((_money.length() - i - 1) % 3 != 0)continue;
		result += L",";
	}

	return result;
}

RotationData GetRotationFromDir(const ChVec3& _dir)
{
	RotationData res;

	auto xzDir = _dir;
	xzDir.y = 0.0f;
	if (!xzDir.Normalize())xzDir = ChVec3(0.0f, 0.0f, 1.0f);

	auto xzCross = ChVec3::GetCross(xzDir, ChVec3(0.0f, 0.0f, 1.0f));

	res.xzRad = ChVec3::GetRadian(xzDir, ChVec3(0.0f, 0.0f, 1.0f));

	if (xzCross.y < 0.0f)res.xzRad = -res.xzRad;

	if (std::isnan(res.xzRad))res.xzRad = 0.0f;

	res.yRad = ChVec3::GetRadian(_dir, xzDir);

#if true

	if (_dir.y > 0.0f)res.yRad = -res.yRad;

#endif

	if (std::isnan(res.yRad))res.yRad = 0.0f;

	return res;
}