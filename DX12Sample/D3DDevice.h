#pragma once
#include "GraphicsDevice.h"
#include "Material.h"
#include "Mesh.h"

#include "D3DUtil.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class D3DDevice : public IGraphicsDevice
{
public:
	~D3DDevice();

public:
	virtual bool Initialize(HWND mainWhd, int width, int height) override;
	virtual void OnResize(int width, int height) override;
	virtual void Update();
	virtual void Draw();

protected:
	virtual void CreateRtvAndDsvDescriptorHeaps();

protected:
	void FlushCommandQueue();

private:
	void CreateCommandObjects();
	void CreateSwapChain(HWND mainWnd, UINT width, UINT height);

	void UpdateInstanceBuffer();
	void UpdateMaterialBuffer();
	void UpdateMainPassCB();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:
	ID3D12Resource* CurrentBackBuffer() const
	{
		return swapChainBuffer[currentBackBuffer].Get();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			currentBackBuffer,
			rtvDescriptorSize);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const
	{
		return dsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

protected:
	ComPtr<ID3D12Device> d3dDevice;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<IDXGIFactory4> dxgiFactory;

	ComPtr<ID3D12Fence> fence;
	UINT64 currentFence = 0;

	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12CommandAllocator> directCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList> commandList;

	static const int SwapChainBufferCount = 2;
	int currentBackBuffer = 0;
	ComPtr<ID3D12Resource> swapChainBuffer[SwapChainBufferCount];
	ComPtr<ID3D12Resource> depthStencilBuffer;

	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	D3D12_VIEWPORT screenViewport;
	D3D12_RECT scissorRect;

	UINT rtvDescriptorSize = 0;
	UINT dsvDescriptorSize = 0;
	UINT cbvSrvUavDescriptorSize = 0;

	D3D_DRIVER_TYPE d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	bool fourxMsaaState = false;
	UINT fourxMsaaQuality = 4;

};
