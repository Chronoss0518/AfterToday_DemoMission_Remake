#pragma once

class GameInMessageBox
{
public:

	void Init(ID3D11Device* _device);

	void Release();

	void Update();

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer);

	inline bool IsDrawMessage() { return afterFrameCount > 0; }

	void EndSetDrawMessage();

	void EndAfterTime();

	void SetMessage(const std::wstring& _messenger, const std::wstring& _message, unsigned long _afterFrame, unsigned long _messageAddFrame);

private:

	struct TextDrawerClass
	{
		ChD3D::DirectFontFromWICBitmap drawer;
		ChD3D::TextFormatObject format;
		ChD3D::BrushObject brush;
		ChD3D::WICBitmapObject bitmap;
	};

private:

	void CreateText(ChD3D11::Texture11& _outTexture,TextDrawerClass& _drawer,const std::wstring& _text,const ChVec4& _screenRect);

private:

	ID3D11Device* device = nullptr;

	TextDrawerClass messageDrawer;
	TextDrawerClass messengerDrawer;

	std::wstring message = L"";
	std::wstring drawMessage = L"";
	long messageAddFrame = 0;
	long messageAddCount = 0;

	long afterFrameCount = -1;

	ImageSprite messageBox;

	ImageSprite messengerText;
	ChD3D11::Sprite11 messageTextSprite;
	ChPtr::Shared<ChD3D11::Texture11> nowTextTexture;
	ChPtr::Shared<ChD3D11::Texture11> nextTextTexture;
};