#pragma once
#include "IComponent.h"
#include "Material.h"

class Renderer : public IComponent
{
public:
	Renderer();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	void SetMaterial(Material* newMaterial) { material = newMaterial; }
	Material* GetMaterial() const { return material; }
	void SetNumFramesDirty(int numFrames) { NumFramesDirty = numFrames; }
	int GetNumFramesDirty() const { return NumFramesDirty; }
	void SetObjIndex(int index) { ObjIndex = index; }
	int GetObjIndex() const { return ObjIndex; }
	void SetMaterialIndex(int index) { MaterialIndex = index; }
	int GetMaterialIndex() const { return MaterialIndex; }
	void SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY type) { primitiveType = type; }
	D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return primitiveType; }
	void SetIndexCount(UINT count) { indexCount = count; }
	UINT GetIndexCount() const { return indexCount; }
	void SetStartIndexLocation(UINT location) { startIndexLocation = location; }
	UINT GetStartIndexLocation() const { return startIndexLocation; }
	void SetBaseVertexLocation(UINT location) { baseVertexLocation = location; }
	UINT GetBaseVertexLocation() const { return baseVertexLocation; }

private:
	Material* material;
	int NumFramesDirty = gNumFrameResources;
	int ObjIndex = -1;
	int MaterialIndex = -1;
	int PSOIndex = -1;
	D3D12_PRIMITIVE_TOPOLOGY primitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT indexCount = 0;
	UINT startIndexLocation = 0;
	UINT baseVertexLocation = 0;
};
