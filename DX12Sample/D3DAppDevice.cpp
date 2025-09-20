#include "D3DAppDevice.h"
#include "TextureManager.h"
#include "RootSignatureType.h"
#include "StaticSamplers.h"
#include "ShaderManager.h"

bool D3DAppDevice::Initialize(HWND mainWhd, int width, int height)
{
	if (!D3DDevice::Initialize(mainWhd, width, height))
	{
		return false;
	}

	ThrowIfFailed(commandList->Reset(directCmdListAlloc.Get(), nullptr));

	Build();

	ThrowIfFailed(commandList->Close());
	ID3D12CommandList* cmdsLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();

	return true;
}

void D3DAppDevice::RegisterMesh(Mesh* mesh)
{
}

void D3DAppDevice::Build()
{
	BuildFrameResources();
	BuildRootSignature();
	BuildInputLayout();
	BuildPSO();
}

void D3DAppDevice::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; ++i)
	{
		frameResources.push_back(make_unique<FrameResource>(d3dDevice.Get()));
	}
}

void D3DAppDevice::BuildRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MaxTexture, 0, 0);

	CD3DX12_ROOT_PARAMETER slotRootParameter[RootSignatureParameterAmount];
	slotRootParameter[(int)RootSignatureType::Object].InitAsShaderResourceView(0, 1);
	slotRootParameter[(int)RootSignatureType::Material].InitAsShaderResourceView(1, 1);
	slotRootParameter[(int)RootSignatureType::PassCB].InitAsConstantBufferView(0);
	slotRootParameter[(int)RootSignatureType::Texture].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);

	auto staticSamplers = StaticSampler::GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(_countof(slotRootParameter), slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(d3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(rootSignature.GetAddressOf())));
}

void D3DAppDevice::BuildInputLayout()
{
	mStdInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
}

void D3DAppDevice::BuildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC defaultPsoDesc;

	ZeroMemory(&defaultPsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	defaultPsoDesc.InputLayout = { mStdInputLayout.data(), (UINT)mStdInputLayout.size() };
	defaultPsoDesc.pRootSignature = rootSignature.Get();
	auto vs = ShaderManager::GetVertexShader("Default");
	defaultPsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(vs->GetBufferPointer()),
		vs->GetBufferSize()
	};
	auto ps = ShaderManager::GetPixelShader("Default");
	defaultPsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(ps->GetBufferPointer()),
		ps->GetBufferSize()
	};
	defaultPsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	defaultPsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	defaultPsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	defaultPsoDesc.SampleMask = UINT_MAX;
	defaultPsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	defaultPsoDesc.NumRenderTargets = 1;
	defaultPsoDesc.RTVFormats[0] = backBufferFormat;
	defaultPsoDesc.SampleDesc.Count = fourxMsaaState ? 4 : 1;
	defaultPsoDesc.SampleDesc.Quality = fourxMsaaState ? (fourxMsaaQuality - 1) : 0;
	defaultPsoDesc.DSVFormat = depthStencilFormat;

	allPSODescs["Default"] = defaultPsoDesc;

	ThrowIfFailed(d3dDevice->CreateGraphicsPipelineState(&defaultPsoDesc, IID_PPV_ARGS(&allPSOs["Default"])));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC wireframePsoDesc = defaultPsoDesc;
	wireframePsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

	allPSODescs["DefaultWireframe"] = wireframePsoDesc;

	ThrowIfFailed(d3dDevice->CreateGraphicsPipelineState(&wireframePsoDesc, IID_PPV_ARGS(&allPSOs["DefaultWireframe"])));
}
