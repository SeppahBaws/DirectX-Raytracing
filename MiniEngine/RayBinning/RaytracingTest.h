#pragma once
#include "Camera.h"
#include "CameraController.h"
#include "ColorBuffer.h"
#include "DepthBuffer.h"
#include "GameCore.h"
#include "Model.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "RaytracingPSO.h"

using namespace Microsoft::WRL;

// TODO: look at moving this out and using the RootSignature instead
class DescriptorHeapStack
{
public:
	DescriptorHeapStack(ID3D12Device& device, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT NodeMask) :
		m_device(device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = numDescriptors;
		desc.Type = type;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = NodeMask;
		device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDescriptorHeap));

		m_descriptorSize = device.GetDescriptorHandleIncrementSize(type);
		m_descriptorHeapCpuBase = m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}

	ID3D12DescriptorHeap& GetDescriptorHeap() { return *m_pDescriptorHeap.Get(); }

	void AllocateDescriptor(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, _Out_ UINT& descriptorHeapIndex)
	{
		descriptorHeapIndex = m_descriptorsAllocated;
		cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descriptorHeapCpuBase, descriptorHeapIndex, m_descriptorSize);
		m_descriptorsAllocated++;
	}

	UINT AllocateBufferSrv(_In_ ID3D12Resource& resource)
	{
		UINT descriptorHeapIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		AllocateDescriptor(cpuHandle, descriptorHeapIndex);
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.NumElements = (UINT)(resource.GetDesc().Width / sizeof(UINT32));
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
		srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		m_device.CreateShaderResourceView(&resource, &srvDesc, cpuHandle);
		return descriptorHeapIndex;
	}

	UINT AllocateBufferUav(_In_ ID3D12Resource& resource)
	{
		UINT descriptorHeapIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		AllocateDescriptor(cpuHandle, descriptorHeapIndex);
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = (UINT)(resource.GetDesc().Width / sizeof(UINT32));
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
		uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;

		m_device.CreateUnorderedAccessView(&resource, nullptr, &uavDesc, cpuHandle);
		return descriptorHeapIndex;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(UINT descriptorIndex)
	{
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_pDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), descriptorIndex, m_descriptorSize);
	}
private:
	ID3D12Device& m_device;
	ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;
	UINT m_descriptorsAllocated = 0;
	UINT m_descriptorSize;
	D3D12_CPU_DESCRIPTOR_HANDLE m_descriptorHeapCpuBase;
};

// From D3D12RaytracingMiniEngineSample
struct RaytracingDispatchRayInputs
{
	RaytracingDispatchRayInputs() {}
	RaytracingDispatchRayInputs(
		ID3D12StateObject* pPSO,
		void* pHitGroupShaderTable,
		UINT hitGroupStride,
		UINT hitGroupTableSize,
		LPCWSTR rayGenExportName,
		LPCWSTR missExportName) : m_HitGroupStride(hitGroupStride), m_pPSO(pPSO)
	{
		const UINT shaderTableSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		ID3D12StateObjectProperties* stateObjectProperties = nullptr;
		ASSERT_SUCCEEDED(pPSO->QueryInterface(IID_PPV_ARGS(&stateObjectProperties)));
		void* pRayGenShaderData = stateObjectProperties->GetShaderIdentifier(rayGenExportName);
		void* pMissShaderData = stateObjectProperties->GetShaderIdentifier(missExportName);

		// MiniEngine requires initial data to be aligned to 16 bytes
		UINT alignment = 16;
		std::vector<BYTE> alignedShaderTableData(shaderTableSize + alignment - 1);
		BYTE* pAlignedShaderTableData = alignedShaderTableData.data() + ((UINT64)alignedShaderTableData.data() % alignment);
		memcpy(pAlignedShaderTableData, pRayGenShaderData, shaderTableSize);
		m_RayGenShaderTable.Create(L"Ray Gen Shader Table", 1, shaderTableSize, alignedShaderTableData.data());

		memcpy(pAlignedShaderTableData, pMissShaderData, shaderTableSize);
		m_MissShaderTable.Create(L"Miss Shader Table", 1, shaderTableSize, alignedShaderTableData.data());

		m_HitShaderTable.Create(L"Hit Shader Table", 1, hitGroupTableSize, pHitGroupShaderTable);

		stateObjectProperties->Release();
	}

	D3D12_DISPATCH_RAYS_DESC GetDispatchRayDesc(UINT DispatchWidth, UINT DispatchHeight)
	{
		D3D12_DISPATCH_RAYS_DESC dispatchRaysDesc = {};

		dispatchRaysDesc.RayGenerationShaderRecord.StartAddress = m_RayGenShaderTable.GetGpuVirtualAddress();
		dispatchRaysDesc.RayGenerationShaderRecord.SizeInBytes = m_RayGenShaderTable.GetBufferSize();
		dispatchRaysDesc.HitGroupTable.StartAddress = m_HitShaderTable.GetGpuVirtualAddress();
		dispatchRaysDesc.HitGroupTable.SizeInBytes = m_HitShaderTable.GetBufferSize();
		dispatchRaysDesc.HitGroupTable.StrideInBytes = m_HitGroupStride;
		dispatchRaysDesc.MissShaderTable.StartAddress = m_MissShaderTable.GetGpuVirtualAddress();
		dispatchRaysDesc.MissShaderTable.SizeInBytes = m_MissShaderTable.GetBufferSize();
		dispatchRaysDesc.MissShaderTable.StrideInBytes = dispatchRaysDesc.MissShaderTable.SizeInBytes; // Only one entry
		dispatchRaysDesc.Width = DispatchWidth;
		dispatchRaysDesc.Height = DispatchHeight;
		dispatchRaysDesc.Depth = 1;
		return dispatchRaysDesc;
	}

	void Cleanup()
	{
		m_pPSO.Reset();
		m_RayGenShaderTable.Destroy();
		m_MissShaderTable.Destroy();
		m_HitShaderTable.Destroy();
	}


	UINT m_HitGroupStride;
	ComPtr<ID3D12StateObject> m_pPSO;
	ByteAddressBuffer m_RayGenShaderTable;
	ByteAddressBuffer m_MissShaderTable;
	ByteAddressBuffer m_HitShaderTable;
};

class RaytracingTest : public GameCore::IGameApp
{
public:
	virtual void Startup(void) override;
	virtual void Cleanup(void) override;

	virtual void Update(float deltaT) override;
	virtual void RenderScene(void) override;

private:
	void InitializeViews();
	void CreateRootSignatures();
	void CreatePipelines();
	void CreateDiffuseRaytracingPSO();
	void CreateAORaytracingPSO();
	void CreateBinningPSO();
	void CreateBinnedDiffusePSO();
	void CreateAccelerationStructures();
	void CreateOutputBuffers();
	void CreateShaderResourceHeap();
	void CreateShaderBindingTables();

	D3D12_STATE_SUBOBJECT CreateDxilLibrary(LPCWSTR entrypoint, const void* pShaderBytecode, SIZE_T bytecodeLength, D3D12_DXIL_LIBRARY_DESC& dxilLibDesc, D3D12_EXPORT_DESC& exportDesc);

	void RenderModel(GraphicsContext& gfxContext);

private:

	// Performance testings.
	bool m_IsProfiling = false;
	uint32_t m_FramesCounted = 0;
	static constexpr uint32_t PROFILE_MAX_FRAMES = 1000;
	float m_FrameTimes[PROFILE_MAX_FRAMES]{};
	uint32_t m_ModeCounter = 0;


	// Graphics states.
	RootSignature m_RootSig;
	GraphicsPSO m_DepthPso;
	GraphicsPSO m_ModelPso;

	D3D12_CPU_DESCRIPTOR_HANDLE m_SRV;

	Model m_Model;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_ModelDescriptors;
	D3D12_CPU_DESCRIPTOR_HANDLE m_ModelMeshInfo;
	D3D12_CPU_DESCRIPTOR_HANDLE m_ModelIndices;
	StructuredBuffer m_HitShaderMeshInfo;

	Camera m_Camera;
	std::auto_ptr<GameCore::CameraController> m_CameraController;
	Matrix4 m_ViewProjMatrix;
	D3D12_VIEWPORT m_MainViewport{};
	D3D12_RECT m_MainScissor{};

	// DXR-capable device
	ComPtr<ID3D12Device5> m_pRTDevice;

	// DXR resources
	RootSignature m_GlobalRaytracingRootSignature;
	RootSignature m_GlobalBinnedRaytracingRS;

	RootSignature m_DiffuseRayGenLocalSignature;
	RootSignature m_BinnedDiffuseRayGenLocalSignature;
	RootSignature m_DiffuseHitGroupLocalSignature;
	RootSignature m_BinnedDiffuseHitGroupLocalSignature;
	RootSignature m_DiffuseMissLocalSignature;

	RootSignature m_AORayGenLocalSignature;
	RootSignature m_AOHitGroupLocalSignature;
	RootSignature m_AOMissLocalSignature;

	RootSignature m_RayBinningSignature;

	// Acceleration Structure related stuff
	ByteAddressBuffer m_ScratchBuffer;
	ByteAddressBuffer m_InstanceDataBuffer;
	ComPtr<ID3D12Resource> m_TLAS;
	std::vector<ComPtr<ID3D12Resource>> m_BLASes;

	// Descriptor heap
	std::unique_ptr<DescriptorHeapStack> m_pRaytracingDescriptorHeap;

	RaytracingPSO m_DiffuseRaytracingPso;
	RaytracingPSO m_AORaytracingPso;
	ComputePSO m_RayBinningPSO;
	RaytracingPSO m_BinnedDiffusePSO;

	RaytracingDispatchRayInputs m_DiffuseRaytracingInput;
	RaytracingDispatchRayInputs m_AORaytracingInput;
	RaytracingDispatchRayInputs m_BinnedDiffuseRaytracingInput;

	// Raytracing output buffer
	ColorBuffer m_RaytracingColorOutput;
	D3D12_GPU_DESCRIPTOR_HANDLE m_RaytracingOutputUAV;

	ColorBuffer m_AODirectionBuffer;
	D3D12_GPU_DESCRIPTOR_HANDLE m_AODirectionBufferUAV;
	D3D12_GPU_DESCRIPTOR_HANDLE m_AODirectionBufferSRV;

	StructuredBuffer m_RayBinResult;
	StructuredBuffer m_RayBinSizeResult;

	D3D12_GPU_DESCRIPTOR_HANDLE m_RayBinningUAVs;
	ByteAddressBuffer m_RayBinningCB;

	D3D12_GPU_DESCRIPTOR_HANDLE m_ModelSRVs;

	// Shader Resource Heap
	ComPtr<ID3D12DescriptorHeap> m_SrvUavHeap;

	ByteAddressBuffer m_DynamicConstantBuffer;

	ComPtr<ID3D12QueryHeap> m_QueryHeap;
};
