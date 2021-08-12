#include "pch.h"
#include "AccelerationStructureBuilder.h"

#include "Model.h"
#include "DescriptorHeapStack.h"

using namespace Graphics;

void AccelerationStructureBuilder::Build(ComPtr<ID3D12Device5> rtDevice, const Model* pModel, DescriptorHeapStack* descriptorHeap,
	ByteAddressBuffer& scratchBuffer, ByteAddressBuffer& instanceDataBuffer, ComPtr<ID3D12Resource>& tlas,
	std::vector<ComPtr<ID3D12Resource>>& blases)
{
	UINT numMeshes = pModel->m_Header.meshCount;
	const UINT numBottomLevels = 1;

	// Get the TLAS prebuild info, so that we know how much scratch buffer size we need.
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO tlasPrebuildInfo;
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC tlasDesc = {};
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& tlasInputs = tlasDesc.Inputs;
	tlasInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	tlasInputs.NumDescs = numBottomLevels;
	tlasInputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
	tlasInputs.pGeometryDescs = nullptr;
	tlasInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	rtDevice->GetRaytracingAccelerationStructurePrebuildInfo(&tlasInputs, &tlasPrebuildInfo);

	const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> geometryDescs(numMeshes);
	UINT64 scratchBufferSizeNeeded = tlasPrebuildInfo.ScratchDataSizeInBytes;

	// Set up the descriptor for the mesh
	for (UINT i = 0; i < numMeshes; i++)
	{
		Model::Mesh& mesh = pModel->m_pMesh[i];

		D3D12_RAYTRACING_GEOMETRY_DESC& desc = geometryDescs[i];
		desc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
		desc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;

		D3D12_RAYTRACING_GEOMETRY_TRIANGLES_DESC& trianglesDesc = desc.Triangles;
		trianglesDesc.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		trianglesDesc.VertexCount = mesh.vertexCount;
		trianglesDesc.VertexBuffer.StartAddress = pModel->m_VertexBuffer.GetGpuVirtualAddress() + (mesh.vertexDataByteOffset + mesh.attrib[Model::attrib_position].offset);
		trianglesDesc.VertexBuffer.StrideInBytes = mesh.vertexStride;
		trianglesDesc.IndexBuffer = pModel->m_IndexBuffer.GetGpuVirtualAddress() + mesh.indexDataByteOffset;
		trianglesDesc.IndexCount = mesh.indexCount;
		trianglesDesc.IndexFormat = DXGI_FORMAT_R16_UINT;
		trianglesDesc.Transform3x4 = 0;
	}

	// Prepare the BLAS create structs
	std::vector<UINT64> blasSize(numBottomLevels);
	std::vector<D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC> blasDescs(numBottomLevels);
	for (UINT i = 0; i < numBottomLevels; i++)
	{
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& blasDesc = blasDescs[i];
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& blasInputs = blasDesc.Inputs;
		blasInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
		blasInputs.NumDescs = numMeshes;
		blasInputs.pGeometryDescs = &geometryDescs[i];
		blasInputs.Flags = buildFlags;
		blasInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;

		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO blasPrebuildInfo;
		rtDevice->GetRaytracingAccelerationStructurePrebuildInfo(&blasInputs, &blasPrebuildInfo);

		blasSize[i] = blasPrebuildInfo.ResultDataMaxSizeInBytes;
		scratchBufferSizeNeeded = std::max(blasPrebuildInfo.ScratchDataSizeInBytes, scratchBufferSizeNeeded);
	}

	// In order to build the Acceleration Structure, we need some extra scratch storage.
	scratchBuffer.Create(L"Acceleration Structure Scratch Buffer", static_cast<UINT>(scratchBufferSizeNeeded), 1);

	D3D12_HEAP_PROPERTIES defaultHeapDesc = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto topLevelDesc = CD3DX12_RESOURCE_DESC::Buffer(tlasPrebuildInfo.ResultDataMaxSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	g_Device->CreateCommittedResource(
		&defaultHeapDesc,
		D3D12_HEAP_FLAG_NONE,
		&topLevelDesc,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		nullptr,
		IID_PPV_ARGS(&tlas));

	tlasDesc.DestAccelerationStructureData = tlas->GetGPUVirtualAddress();
	tlasDesc.ScratchAccelerationStructureData = scratchBuffer.GetGpuVirtualAddress();

	// Create the BLASes
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC> instanceDescs(numBottomLevels);
	blases.resize(numBottomLevels);
	for (UINT i = 0; i < blasDescs.size(); i++)
	{
		auto& blas = blases[i];

		auto bottomLevelDesc = CD3DX12_RESOURCE_DESC::Buffer(blasSize[i], D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		g_Device->CreateCommittedResource(
			&defaultHeapDesc,
			D3D12_HEAP_FLAG_NONE,
			&bottomLevelDesc,
			D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
			nullptr,
			IID_PPV_ARGS(&blas));

		blasDescs[i].DestAccelerationStructureData = blas->GetGPUVirtualAddress();
		blasDescs[i].ScratchAccelerationStructureData = scratchBuffer.GetGpuVirtualAddress();

		D3D12_RAYTRACING_INSTANCE_DESC& instanceDesc = instanceDescs[i];
		UINT descriptorIndex = descriptorHeap->AllocateBufferUav(*blas.Get());
		UNREFERENCED_PARAMETER(descriptorIndex);

		// Identity matrix
		ZeroMemory(instanceDesc.Transform, sizeof(instanceDesc.Transform));
		instanceDesc.Transform[0][0] = 1.0f;
		instanceDesc.Transform[1][1] = 1.0f;
		instanceDesc.Transform[2][2] = 1.0f;

		instanceDesc.AccelerationStructure = blases[i]->GetGPUVirtualAddress();
		instanceDesc.Flags = 0;
		instanceDesc.InstanceID = 0;
		instanceDesc.InstanceMask = 1;
		instanceDesc.InstanceContributionToHitGroupIndex = i;
	}

	instanceDataBuffer.Create(L"Instance Data Buffer", numBottomLevels, sizeof(D3D12_RAYTRACING_INSTANCE_DESC), instanceDescs.data());

	tlasInputs.InstanceDescs = instanceDataBuffer.GetGpuVirtualAddress();
	tlasInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;

	// Finally, here we create the acceleration structure.
	GraphicsContext& gfxContext = GraphicsContext::Begin(L"Create Acceleration Structure");
	ID3D12GraphicsCommandList* pCommandList = gfxContext.GetCommandList();

	ComPtr<ID3D12GraphicsCommandList4> pRaytracingCommandList;
	ASSERT_SUCCEEDED(pCommandList->QueryInterface(IID_PPV_ARGS(&pRaytracingCommandList)));

	ID3D12DescriptorHeap* descriptorHeaps[] = { &descriptorHeap->GetDescriptorHeap() };
	pRaytracingCommandList->SetDescriptorHeaps(ARRAYSIZE(descriptorHeaps), descriptorHeaps);

	auto uavBarrier = CD3DX12_RESOURCE_BARRIER::UAV(nullptr);
	for (UINT i = 0; i < blasDescs.size(); i++)
	{
		pRaytracingCommandList->BuildRaytracingAccelerationStructure(&blasDescs[i], 0, nullptr);
	}
	pCommandList->ResourceBarrier(1, &uavBarrier);

	pRaytracingCommandList->BuildRaytracingAccelerationStructure(&tlasDesc, 0, nullptr);

	// TODO: i think we need a barrier here...
	gfxContext.FlushResourceBarriers();

	gfxContext.Finish();
}
