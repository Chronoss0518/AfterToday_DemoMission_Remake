#pragma once

static constexpr float PARAMETER_PANEL_WIDTH = (280.0f);
static constexpr float PARAMETER_PANEL_HEIGHT = (30.0f);

class ParameterTitlePanel
{
public:

	ParameterTitlePanel()
	{
		sprite.Init();
	}

public:

	static ChPtr::Shared<ParameterTitlePanel> CreatePanel(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::wstring& _title);

public:

	void CreateTitle(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::wstring& _title);

protected:

	virtual void CreateTitleImage(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::wstring& _title);

public:

	inline void SetBackGround(ChPtr::Shared<ChD3D11::Texture11> _background)
	{
		background = _background;
	}

public:

	virtual void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec2& _leftTop);

protected:

	void DrawBackGround(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec2& _leftTop);

protected:

	virtual void CreateTexture(ChD3D11::Texture11& _outTexture, TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::wstring& _text, const ChVec2& _size);

protected:

	ChPtr::Shared<ChD3D11::Texture11> title = nullptr;
	ChPtr::Shared<ChD3D11::Texture11> background = nullptr;
	ChD3D11::Sprite11 sprite;
};

enum class ParameterOpelator
{
	EQUAL,
	BIGGER,
	SMALLER
};

class ParameterValuePanel : public ParameterTitlePanel
{
public:

	static ChPtr::Shared<ParameterValuePanel> CreatePanel(ID3D11Device* _device, TextDrawerWICBitmap& _titleDrawer, const std::wstring& _title, TextDrawerWICBitmap& _valueDrawer, unsigned long* _baseValue, unsigned long* _nextValue, bool _inversOperatorFlg);

	static ChPtr::Shared<ParameterValuePanel> CreatePanel(ID3D11Device* _device, TextDrawerWICBitmap& _titleDrawer, const std::wstring& _title, TextDrawerWICBitmap& _valueDrawer, float* _baseValue , float* _nextValue, bool _inversOperatorFlg);

public:

	inline void SetInversOperatorFlg(bool _flg) { inversOperatorFlg = _flg; }

protected:

	virtual ParameterOpelator GetParameterOperator() = 0;
	
public:

	void CreateTitleImage(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::wstring& _title)override;

	void CreateTexture(ChD3D11::Texture11& _outTexture, TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::wstring& _text, const ChVec2& _size)override;

public:

	virtual void UpdateValue(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device) = 0;

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec2& _leftTop)override;

protected:

	bool inversOperatorFlg = false;
	ChPtr::Shared<ChD3D11::Texture11> value = nullptr;
};

class ParameterULValuePanel : public ParameterValuePanel
{
public:

	void CreateValue(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, unsigned long* _baseValue, unsigned long* _nextValue);

public:

	ParameterOpelator GetParameterOperator()override;

public:

	void UpdateValue(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device)override;

private:


	unsigned long beforeValue = 0;
	unsigned long* baseValue = nullptr;
	unsigned long* nextValue = nullptr;
};

class ParameterFValuePanel : public ParameterValuePanel
{
public:

	void CreateValue(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, float* _baseValue, float* _nextValue);

public:

	ParameterOpelator GetParameterOperator()override;

public:

	void UpdateValue(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device)override;

private:

	float beforeValue = 0.0f;
	float* baseValue = nullptr;
	float* nextValue = nullptr;
};
