#pragma once


class BaseMecha;
class MechaParts;
class MechaPartsObject;

struct PartsParameters;

class ParameterTitlePanel;

namespace PartsParameterStruct
{
	struct WeaponData;
	struct AttackBase;
}

#include"../Frames/MenuBase.h"

class ParameterDisplay
{
public:

	inline void Init(ChPtr::Shared<ChD3D11::Texture11>& _tBG, ChPtr::Shared<ChD3D11::Texture11>& _vBG , ChPtr::Shared<PartsParameters> _baseParameterList, ChPtr::Shared<PartsParameters> _nextParameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer)
	{
		titleBGTexture = _tBG;
		valueBGTexture = _vBG;

		baseParameter = _baseParameterList;
		nextParameter = _nextParameterList;
		Init(_device, _textDrawer, _titleDrawer, _valueDrawer);
	}

protected:

	virtual void Init(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

protected:

	void AddPanelList();

	void AddMoveParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddWeaponParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer, bool _cutFlg = false, bool _drawPartsName = false);

	void AddSwordParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddGunParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddAttackParameters(ChPtr::Shared<PartsParameterStruct::WeaponData>& _parameterList, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::wstring& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddBoostBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::wstring& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddHighExplosiveBulletParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::wstring& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddMissileParameters(ChPtr::Shared<PartsParameterStruct::AttackBase>& _parameterList, const std::wstring& _title, ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void AddParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::wstring& _title);

	void AddParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::wstring& _title, TextDrawerWICBitmap& _titleDrawer, const std::wstring& _itemName, TextDrawerWICBitmap& _valueDrawer, unsigned long* _baseValue, unsigned long* _nextValue, bool _inversOperatorFlg);

	void AddParameters(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::wstring& _title, TextDrawerWICBitmap& _titleDrawer, const std::wstring& _itemName, TextDrawerWICBitmap& _valueDrawer, float* _baseValue, float* _nextValue, bool _inversOperatorFlg);


public:

	void Update(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, TextDrawerWICBitmap& _titleDrawer, TextDrawerWICBitmap& _valueDrawer);

	void UpdateCountPanels(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer);

public:

	virtual ChVec2 GetDrawStartPosition() = 0;

	virtual ChVec2 GetDrawPanelSize() = 0;

public:

	virtual void Up() = 0;

	virtual void Down() = 0;

public:

	virtual void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer) = 0;

protected:

	size_t drawPosition = 0;

	unsigned long* GetULWeaponParameter()
	{
		static unsigned long ins = 0;
		ins = 0;
		return &ins;
	}

	float* GetFWeaponParameter()
	{
		static float ins = 0.0f;
		ins = 0.0f;
		return &ins;
	}

	ChPtr::Shared<ChD3D11::Texture11>titleBGTexture = nullptr;
	ChPtr::Shared<ChD3D11::Texture11>valueBGTexture = nullptr;

	ChPtr::Shared<PartsParameters>baseParameter = nullptr;
	ChPtr::Shared<PartsParameters>nextParameter = nullptr;

protected:

	std::vector<ChPtr::Weak<ParameterTitlePanel>>countPanels;
	size_t countPanelCreateCount = 1;
	size_t virtualCountPanelCount = 0;

	size_t countPanelsBeforNum = 0;
	std::vector<ChPtr::Shared<ParameterTitlePanel>>panels;
	size_t baseParameterCount = 0;
	ChPtr::Shared<ParameterTitlePanel> emptyPanel = nullptr;
};

class ParameterList
{
private:

	static constexpr unsigned char DISPLAY_TYPE_COUNT = 2;

	enum class DisplayType
	{
		Partial,//ˆê•”•\Ž¦
		Entire//‘S‘Ì•\Ž¦
	};

public:

	void Init(ID3D11Device* _device,ChPtr::Shared<BaseMecha> _baseMecha);

public:

	void SetBaseParts(ID3D11Device* _device, ChPtr::Shared<MechaPartsObject> _partsObject);

	void SetNextParts(ID3D11Device* _device, ChPtr::Shared<MechaPartsObject> _partsObject);

public:

	void AddParameterData(PartsParameters& _parameter,ChPtr::Shared<MechaPartsObject> _partsObject);

	void SubParameterData(PartsParameters& _parameter, ChPtr::Shared<MechaPartsObject> _partsObject);

public:

	inline ChVec2 GetDrawStartPosition()
	{
		if (displays[ChStd::EnumCast(displayType)] == nullptr)return ChVec2();
		return displays[ChStd::EnumCast(displayType)]->GetDrawStartPosition();
	}

	inline ChVec2 GetDrawPanelSize()
	{
		if (displays[ChStd::EnumCast(displayType)] == nullptr)return ChVec2();
		return displays[ChStd::EnumCast(displayType)]->GetDrawPanelSize();
	}


public:

	bool Update(MenuBase::ActionType _type);

private:

	void UpdateDisplayParameter(ID3D11Device* _device);

private:

	void CreateTextDrawer(TextDrawerWICBitmap& _textDrawer, unsigned long _w, unsigned long _h, float _fontSize);

public:

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer);

private:
	
	TextDrawerWICBitmap textDrawer;
	TextDrawerWICBitmap titleTextDrawer;
	TextDrawerWICBitmap valueTextDrawer;

	DisplayType displayType = DisplayType::Partial;

	ChPtr::Shared<MechaPartsObject>baseParameter = nullptr;
	ChPtr::Shared<MechaPartsObject>nextParameter = nullptr;

	ChPtr::Shared<PartsParameters>basePartsParameter = nullptr;
	ChPtr::Shared<PartsParameters>nextPartsParameter = nullptr;
	ChPtr::Shared<PartsParameters>baseAllParameter = nullptr;
	ChPtr::Shared<PartsParameters>nextAllParameter = nullptr;

	ChPtr::Shared<ChD3D11::Texture11> titleBGTexture = nullptr;
	ChPtr::Shared<ChD3D11::Texture11> valueBGTexture = nullptr;

	ChPtr::Shared<ParameterDisplay>displays[2];
};