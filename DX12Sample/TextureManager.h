#pragma once
#include "DDSTextureLoader.h"
#include "D3DUtil.h"
#include <unordered_map>
#include "Singleton.h"

#define TextureRelativePath L"../../Textures/"
#define MaxTexture 64

class TextureManager : public Singleton<TextureManager>
{
public:
	static Texture* LoadTexture(
		ID3D12Device* d3dDevice,
		ID3D12GraphicsCommandList* cmdList,
		string name)
	{
		auto& allTextures = GetInstance().allTextures;
		if (allTextures.find(name) == allTextures.end())
		{
			unique_ptr<Texture> newTex = make_unique<Texture>();
			newTex->Name = name;
			newTex->Filename = TextureRelativePath + AnsiToWString(name) + L".dds";
			newTex->Offset = allTextures.size();
			ThrowIfFailed(CreateDDSTextureFromFile12(
				d3dDevice,
				cmdList,
				newTex->Filename.c_str(),
				newTex->Resource,
				newTex->UploadHeap));
			allTextures[name] = move(newTex);
		}

		return allTextures[name].get();
	}

	static void ReleaseTexture(string name)
	{
		auto& allTextures = GetInstance().allTextures;
		allTextures[name]->Resource = nullptr;
		allTextures[name]->UploadHeap = nullptr;
		allTextures[name].reset();
		allTextures.erase(name);
	}

private:
	unordered_map<string, unique_ptr<Texture>> allTextures;
};
