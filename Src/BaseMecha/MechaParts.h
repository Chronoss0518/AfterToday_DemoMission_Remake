#pragma once

#include<wchar.h>
#include <stdio.h>
#include <string.h>

#include"WeaponHandType.h"

#ifndef GET_CLASS_NAME
#define GET_CLASS_NAME(cls) L## #cls
#endif

#include"BaseMecha.h"
 
#ifndef JSON_PROPEATY_PARTS_NAME
#define JSON_PROPEATY_PARTS_NAME L"Parts"
#endif

#ifndef JSON_PROPEATY_RIGHT_WEAPON
#define JSON_PROPEATY_RIGHT_WEAPON L"RightWeapon"
#endif

#ifndef JSON_PROPEATY_LEFT_WEAPON
#define JSON_PROPEATY_LEFT_WEAPON L"LeftWeapon"
#endif

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY(L"RobotParts/" current_path)
#endif

class MechaPartsObject;
class CameraComponent;
class PartsDataBase;
class WeaponDataBase;

struct PartsParameters;

namespace PartsParameterStruct
{
	struct BoostData;
	struct WeaponData;
}

class MechaParts : public ChCpp::BaseObject<wchar_t>
{
public:

	enum class PartsConnectionType
	{
		None
	};

public:

	struct MartsPositionData
	{
		ChPtr::Shared<ChCpp::FrameObject<wchar_t>>positionObject = nullptr;
		ChLMat connectionRotate = ChLMat();
		PartsConnectionType connectType = PartsConnectionType::None;
		float maxWeight = 0.0f;
	};

public:

	friend PartsDataBase;

public:

	static std::map<std::wstring,ChPtr::Shared<MechaParts>>& LoadPartsList()
	{
		static std::map<std::wstring,ChPtr::Shared<MechaParts>>ins;
		return ins;
	}

	inline static void ClearPartsList()
	{
		auto&& partsList = LoadPartsList();
		if (partsList.empty())return;
		partsList.clear();
	}

public:

	void RemoveParameter(BaseMecha& _base);

private:

	unsigned long CreateDatas(BaseMecha& _base, ChCpp::TextObject<wchar_t>& _textObject, unsigned long _linePos);

	void CreateChild(ChPtr::Shared<MechaPartsObject> _partsObject,BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject);

public://Serialize Deserialize//

	static ChPtr::Shared<MechaPartsObject> LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, GameFrame* _frame, const std::wstring& _partsFilePath);

	static ChPtr::Shared<MechaPartsObject> LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject,const std::wstring& _positionObjectType = L"", ChPtr::Shared<MechaPartsObject> _parent = nullptr);

	void Load(BaseMecha& _base, ID3D11Device* _device, const std::wstring& _fileName);

	void Deserialize(BaseMecha& _base,ID3D11Device* _device,const std::wstring& _text);

	std::wstring Save(const std::wstring& _fileName);

	std::wstring Serialize();

private:

	void LoadModel(ID3D11Device* _device, const std::wstring& _fileName);

public://Set Functions//

	ChPtr::Shared<MechaPartsObject> SetParameters(BaseMecha& _base, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject);

	virtual ChPtr::Shared<MechaPartsObject>  SetPartsParameter(BaseMecha& _base);

	void SetParameters(MechaPartsObject& _parts);

	void SetHardness(const unsigned long _hardness) { hardness = _hardness; }

	void SetMass(const float _mass) { mass = _mass; }

	inline void SetMeshDrawer(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer) { drawer = _drawer; }

public://Get Function//

	inline unsigned long GetHardness()const { return hardness; }

	inline float GetMass()const { return mass; }

	inline float GetGroundHeight() { return groundHeight; }

	inline ChD3D11::Mesh11<wchar_t>& GetMesh() { return *model; }

	inline ChLMat GetDefaultFrameMat() { return defaultFrameMat; }

	inline std::wstring GetThisFileName() { return thisFileName; }

	inline std::wstring GetThisFilePath() { return thisFilePath; }

	inline ChD3D11::Shader::BaseDrawMesh11<wchar_t>* GetMeshDrawer() { return drawer; }

	inline std::map<std::wstring, ChPtr::Shared<MartsPositionData>>& GetPositionList() { return positions; }

public:

	inline void AddPosition(
		const std::wstring& _parameter,
		ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _frame,
		const ChLMat& _connectRotate,
		const PartsConnectionType& _type = PartsConnectionType::None,
		const float _maxWeight = 0.0f)
	{
		auto&& data = ChPtr::Make_S<MartsPositionData>();
		data->positionObject = _frame;
		data->connectionRotate = _connectRotate;
		data->connectType = _type;
		data->maxWeight = _maxWeight;
		positions[_parameter] = data;
	}

private:

	void SetWeaponFunction(ChPtr::Shared<MechaPartsObject> _partsObject, BaseMecha& _base, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject, const std::wstring& _jsonPropertyName, WeaponHandType _type);

	void AddWeaponData(ChPtr::Shared<MechaPartsObject> _partsObject,BaseMecha& _base, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject);

public:

	void Draw(const ChMat_11& _mat);

private:

	//パーツの硬さ//
	unsigned long hardness = 1;

	//パーツの重さ//
	float mass = 1.0f;

	std::wstring thisFileName = L"";
	std::wstring thisFilePath = L"";

	float groundHeight = 0.0f;

	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> model = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
	ChLMat defaultFrameMat;

	ChD3D11::Shader::BaseDrawMesh11<wchar_t>* drawer = nullptr;

	static std::map<std::wstring, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>createFunctions;

	std::map<std::wstring, ChPtr::Shared<MartsPositionData>> positions;


};

class PartsDataBase :public ChCpp::BaseComponent
{
public:

	virtual void RemoveParameter(BaseMecha& _base){}

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text,const unsigned long _textPos = 0) = 0;

	virtual std::wstring Serialize() = 0;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame) = 0;

	virtual void SetPartsParameter(PartsParameters& _base, MechaPartsObject& _parts) = 0;

public://Get Functions//

	virtual std::wstring GetPartsTypeTag() = 0;

	template<class T>
	ChPtr::Shared<T> GetComponent(BaseMecha& _base)
	{
		return _base.GetComponentObject<T>();
	}

	ChD3D11::Mesh11<wchar_t>& GetModel(MechaPartsObject& _base);

public:

	//virtual void ReleaseParts(BaseMecha& _base) = 0;

};
