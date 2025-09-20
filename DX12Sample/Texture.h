#pragma once

struct Texture
{
	string Name;
	wstring Filename;
	UINT Offset;

	ComPtr<ID3D12Resource> Resource = nullptr;
	ComPtr<ID3D12Resource> UploadHeap = nullptr;
};

