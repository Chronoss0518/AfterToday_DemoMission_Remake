#pragma once

void ReleaseMesh11(ChPtr::Shared<ChD3D11::Mesh11<wchar_t>>& _meshObject);

float HorizontalToProjection(float _x);

float VerticalToProjection(float _y);

ChVec4 RectToGameWindow(const ChVec4 _rect);

ChVec3 ColorTextToColorVector3(
	const std::wstring& _r,
	const std::wstring& _g,
	const std::wstring& _b);

bool IsMoucePosOnRect(const ChVec4& _rect);

bool IsMoucePosOnSprite(const ChD3D11::Sprite11& _sprite);

std::wstring CreateMoneyText(const std::wstring& _money);

struct RotationData
{
	float xzRad = 0.0f;
	float yRad = 0.0f;
};

RotationData GetRotationFromDir(ChVec3 _dir);