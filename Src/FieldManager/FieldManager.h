#pragma once

class FieldManager final
{
public:

	inline ~FieldManager() { Release(); }

public:

	void Init(const std::basic_string<wchar_t>& _fieldFile, ChD3D11::Shader::BaseDrawMesh11<wchar_t>& _meshDrawer);

	void Release();

private:

	void CreateFieldModel(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>_object, ChD3D11::Shader::BaseDrawMesh11<wchar_t>& _meshDrawer);

	ChPtr::Shared<ChCpp::ModelObject<wchar_t>>CreateModel(const std::basic_string<wchar_t>& _modelName,ChD3D11::Shader::BaseDrawMesh11<wchar_t>& _meshDrawer);

	ChVec3 CreateVectorValues(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>_object,const ChVec3& _default);

	void CreatePositions(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>_object);

	void CreatePointNames(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>_object);

public:

	std::vector<ChPtr::Shared<ChVec3>> GetPositionFromName(const std::basic_string<wchar_t>& _pointName);

public:

	inline bool IsLoadFieldModels() { return !fieldModels.empty(); }

public:

	void Draw(ChD3D11::Shader::BaseDrawMesh11<wchar_t>& _meshDrawer,ChLMat _viewMat);

private:

	float maxDrawLen = 1000.0f;

	std::vector<ChPtr::Shared<ChCpp::ModelObject<wchar_t>>>fieldModels;
	std::vector<ChPtr::Shared<ChVec3>>positions;

	std::map<std::basic_string<wchar_t>, std::vector<size_t>>pointNames;
};