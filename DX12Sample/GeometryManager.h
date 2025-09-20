#pragma once
#include "Singleton.h"
#include "Mesh.h"

#define MeshRelativePath "Models/"

class GeometryManager : public Singleton<GeometryManager>
{
public:
	static MeshGeometry* GetGeometry(string groupName)
	{

	}

	static MeshGeometry* RegisterMesh(string name, unique_ptr<MeshGeometry> mesh)
	{
		auto& allGeometries = GetInstance().allGeometries;
		if (allGeometries.find(name) != allGeometries.end())
		{
			// TODO : throw exception
			return nullptr;
		}
		allGeometries[name] = move(mesh);
		return allGeometries[name].get();
	}

	static MeshGeometry* LoadMesh(
		string groupName,
		string name,
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList)
	{
		ifstream fin(MeshRelativePath + name + ".txt");

		if (!fin)
		{
			wstring msg = L"Models/" + AnsiToWString(name) + L".txt not found.";
			MessageBox(0, msg.c_str(), 0, 0);
			return nullptr;
		}

		UINT vcount = 0;
		UINT tcount = 0;
		string ignore;

		fin >> ignore >> vcount;
		fin >> ignore >> tcount;
		fin >> ignore >> ignore >> ignore >> ignore;

		XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
		XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

		XMVECTOR vMin = XMLoadFloat3(&vMinf3);
		XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

		vector<Vertex> vertices(vcount);

		for (UINT i = 0; i < vcount; ++i)
		{
			fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
			fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;

			XMVECTOR P = XMLoadFloat3(&vertices[i].Pos);

			XMFLOAT3 spherePos;
			XMStoreFloat3(&spherePos, XMVector3Normalize(P));

			float theta = atan2f(spherePos.z, spherePos.x);

			if (theta < 0.0f)
				theta += XM_2PI;

			float phi = acosf(spherePos.y);

			float u = theta / (2.0f * XM_PI);
			float v = phi / XM_PI;

			vertices[i].TexC = { u, v };

			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);
		}

		BoundingBox bounds;
		XMStoreFloat3(&bounds.Center, 0.5f * (vMin + vMax));
		XMStoreFloat3(&bounds.Extents, 0.5f * (vMax - vMin));

		fin >> ignore >> ignore >> ignore;

		vector<int32_t> indices(3 * tcount);
		for (UINT i = 0; i < tcount; ++i)
		{
			fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
		}

		fin.close();

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(int32_t);

		auto geo = make_unique<MeshGeometry>();
		geo->Name = name;

		ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
		CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

		ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
		CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

		geo->VertexBufferGPU = D3DUtil::CreateDefaultBuffer(device,
			cmdList, vertices.data(), vbByteSize, geo->VertexBufferUploader);

		geo->IndexBufferGPU = D3DUtil::CreateDefaultBuffer(device,
			cmdList, indices.data(), ibByteSize, geo->IndexBufferUploader);

		geo->VertexByteStride = sizeof(Vertex);
		geo->VertexBufferByteSize = vbByteSize;
		geo->IndexFormat = DXGI_FORMAT_R32_UINT;
		geo->IndexBufferByteSize = ibByteSize;

		SubmeshGeometry submesh;
		submesh.IndexCount = (UINT)indices.size();
		submesh.StartIndexLocation = 0;
		submesh.BaseVertexLocation = 0;
		submesh.Bounds = bounds;

		geo->DrawArgs[name] = submesh;

		return geo;
	}

private:
	unordered_map<string, unique_ptr<MeshGeometry>> allGeometries;
};
