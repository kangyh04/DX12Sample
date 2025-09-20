#pragma once

#include "D3DDevice.h"
#include "Texture.h"
#include "Material.h"
#include "FrameResource.h"

class D3DAppDevice : public D3DDevice
{
public:
	virtual bool Initialize(HWND mainWhd, int width, int height) override;
	virtual void RegisterMesh(class Mesh* mesh) override;

protected:
	void Build();

private:
	void BuildFrameResources();
	void BuildRootSignature();
	void BuildInputLayout();
	void BuildPSO();

private:
	vector<unique_ptr<FrameResource>> frameResources;

	ComPtr<ID3D12RootSignature> rootSignature = nullptr;

	// vector<unique_ptr<MeshGeometry>> allMeshGeometries;
	// vector<unique_ptr<Material>> allMaterials;
	unordered_map<string, ComPtr<ID3D12PipelineState>> allPSOs;
	unordered_map<string, D3D12_GRAPHICS_PIPELINE_STATE_DESC> allPSODescs;

	vector<D3D12_INPUT_ELEMENT_DESC> mStdInputLayout;

};
