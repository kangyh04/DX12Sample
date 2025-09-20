#pragma once

#include "D3DUtil.h"
#include "ShaderType.h"
#include "Singleton.h"

class ShaderManager : public Singleton<ShaderManager>
{
public:
	static ComPtr<ID3DBlob> GetVertexShader(string name)
	{
		return GetShader(name, "VS", "vs_5_1");
	}
	static ComPtr<ID3DBlob> GetPixelShader(string name)
	{
		return GetShader(name, "PS", "ps_5_1");
	}
	static ComPtr<ID3DBlob> GetGeometryShader(string name)
	{
		return GetShader(name, "GS", "gs_5_1");
	}
	static ComPtr<ID3DBlob> GetComputeShader(string name)
	{
		return GetShader(name, "CS", "cs_5_1");
	}
	static ComPtr<ID3DBlob> GetDomainShader(string name)
	{
		return GetShader(name, "DS", "ds_5_1");
	}
	static ComPtr<ID3DBlob> GetHullShader(string name)
	{
		return GetShader(name, "HS", "hs_5_1");
	}
private:
	static ComPtr<ID3DBlob> GetShader(string name, string shaderType, string shaderVersion)
	{
		auto& allShaders = GetInstance().allShaders;
		string shaderName = name + shaderType;
		if (allShaders.find(name) == allShaders.end())
		{
			ComPtr<ID3DBlob> shader = nullptr;
			wstring shaderFilePath = L"Shader\\" + AnsiToWString(name) + L".hlsl";
			shader = D3DUtil::CompileShader(shaderFilePath, nullptr, shaderType, shaderVersion);
			allShaders[shaderName] = shader;
		}
		return allShaders[shaderName];
	}

	static void ReleaseShader(string name)
	{
		auto& allShaders = GetInstance().allShaders;
		allShaders[name] = nullptr;
		allShaders.erase(name);
	}

private:
	unordered_map<string, ComPtr<ID3DBlob>> allShaders;
};
