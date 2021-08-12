#include "pch.h"
#include "RaytracingTest.h"

#include <iostream>
#include <algorithm>


#include "BufferManager.h"
#include "SamplerManager.h"
#include "CommandContext.h"
#include "Utility.h"
#include "Utils.h"
#include "VertPosTexNorm.h"
#include "GameInput.h"

#include <CompiledShaders/DepthViewerVS.h>
#include <CompiledShaders/ModelPS.h>
#include <CompiledShaders/ModelVS.h>

#include <CompiledShaders/DiffuseRayGen.h>
#include <CompiledShaders/DiffuseHit.h>
#include <CompiledShaders/DiffuseMiss.h>

#include <CompiledShaders/AORayGen.h>
#include <CompiledShaders/AOHit.h>
#include <CompiledShaders/AOMiss.h>

#include <CompiledShaders/RayBinningCompute.h>
#include <CompiledShaders/DiffuseRayGenBinned.h>
#include <CompiledShaders/DiffuseHitBinned.h>

#include "RaytracingHlslCompat.h"
#include "RaytraceCommon.h"
#include "RayBinningCommon.h"

using namespace Graphics;
using namespace GameCore;

const char* raytracingModes[] =
{
	"Off",
	"Diffuse",
	"RayBinning"
};
enum RaytracingMode
{
	RTM_Off = 0,
	RTM_RaytraceDiffuse,
	RTM_RaytraceBinning
};
EnumVar raytracingMode("Application/RaytracingMode", RTM_Off, _countof(raytracingModes), raytracingModes);

struct MeshRootConstant
{
	UINT meshId;
};

void RaytracingTest::Startup(void)
{
#ifdef _DEBUG
	ComPtr<ID3D12DebugDevice> debugDevice;
	ASSERT_SUCCEEDED(g_Device->QueryInterface(IID_PPV_ARGS(&debugDevice)));
	debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY);
	debugDevice.Reset();
#endif

	// Check Raytracing support
	ASSERT_SUCCEEDED(g_Device->QueryInterface(IID_PPV_ARGS(&m_pRTDevice)), "DXR device is not supported on this system!");

	m_pRaytracingDescriptorHeap = std::make_unique<DescriptorHeapStack>(
		*g_Device, 200, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 0);

	SamplerDesc sampler;

	// Create our root signature
	m_RootSig.Reset(2, 1);
	m_RootSig.InitStaticSampler(0, sampler, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSig[0].InitAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	m_RootSig[1].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 6, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSig.Finalize(L"RaytracingTest", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	DXGI_FORMAT colorFormat = g_SceneColorBuffer.GetFormat();
	DXGI_FORMAT depthFormat = g_SceneDepthBuffer.GetFormat();

	// Depth state
	m_DepthPso.SetRootSignature(m_RootSig);
	m_DepthPso.SetRasterizerState(RasterizerDefault);
	m_DepthPso.SetBlendState(BlendNoColorWrite);
	m_DepthPso.SetDepthStencilState(DepthStateReadWrite);
	m_DepthPso.SetInputLayout(static_cast<UINT>(VertPosTexNorm::VertexDesc().size()), VertPosTexNorm::VertexDesc().data());
	m_DepthPso.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_DepthPso.SetRenderTargetFormats(0, nullptr, depthFormat);
	m_DepthPso.SetVertexShader(g_pDepthViewerVS, sizeof(g_pDepthViewerVS));
	m_DepthPso.Finalize();

	// Model state
	m_ModelPso = m_DepthPso;
	m_ModelPso.SetBlendState(BlendDisable);
	m_ModelPso.SetDepthStencilState(DepthStateTestEqual);
	m_ModelPso.SetRenderTargetFormats(1, &colorFormat, depthFormat);
	m_ModelPso.SetVertexShader(g_pModelVS, sizeof(g_pModelVS));
	m_ModelPso.SetPixelShader(g_pModelPS, sizeof(g_pModelPS));
	m_ModelPso.Finalize();

	TextureManager::Initialize(L"Textures/");
	// ASSERT(m_Model.Load("Models/sponza.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/cathedral.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/tokyo.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/san-miguel-low-poly-scaled.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/salle_de_bain.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/rungholt.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/gallery.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/bistro-exterior.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/bistro-interior.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/tap.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/TheMill.h3d"), "Failed to load model");
	// ASSERT(m_Model.Load("Models/TheSeaHouse.h3d"), "Failed to load model");
	ASSERT(m_Model.Load("Models/house_scene.h3d"), "Failed to load model");
	ASSERT(m_Model.m_Header.meshCount > 0, "Model contains no meshes");

	// This vector stores the descriptors for the materials of the model.
	// We need to initialize the vector here as we are going to directly access the items in this vector with indexes later.
	m_ModelDescriptors = std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>(m_Model.m_Header.materialCount);

	// Mesh Info
	std::vector<RayTraceMeshInfo> meshInfoData(m_Model.m_Header.meshCount);
	for (UINT i = 0; i < m_Model.m_Header.meshCount; ++i)
	{
		meshInfoData[i].m_IndexOffsetBytes = m_Model.m_pMesh[i].indexDataByteOffset;
		meshInfoData[i].m_UVAttributeOffsetBytes = m_Model.m_pMesh[i].vertexDataByteOffset + m_Model.m_pMesh[i].attrib[Model::attrib_texcoord0].offset;
		meshInfoData[i].m_NormalAttributeOffsetBytes = m_Model.m_pMesh[i].vertexDataByteOffset + m_Model.m_pMesh[i].attrib[Model::attrib_normal].offset;
		meshInfoData[i].m_TangentAttributeOffsetBytes = m_Model.m_pMesh[i].vertexDataByteOffset + m_Model.m_pMesh[i].attrib[Model::attrib_tangent].offset;
		meshInfoData[i].m_BitangentAttributeOffsetBytes = m_Model.m_pMesh[i].vertexDataByteOffset + m_Model.m_pMesh[i].attrib[Model::attrib_bitangent].offset;
		meshInfoData[i].m_PositionAttributeOffsetBytes = m_Model.m_pMesh[i].vertexDataByteOffset + m_Model.m_pMesh[i].attrib[Model::attrib_position].offset;
		meshInfoData[i].m_AttributeStrideBytes = m_Model.m_pMesh[i].vertexStride;
		meshInfoData[i].m_MaterialInstanceId = m_Model.m_pMesh[i].materialIndex;
	}

	m_HitShaderMeshInfo.Create(L"RayTraceMeshInfo", static_cast<UINT>(meshInfoData.size()),
		sizeof(meshInfoData[0]), meshInfoData.data());
	m_ModelIndices = m_Model.m_IndexBuffer.GetSRV();
	m_ModelMeshInfo = m_HitShaderMeshInfo.GetSRV();


	float modelRadius = Length(m_Model.m_Header.boundingBox.max - m_Model.m_Header.boundingBox.min) * .5f;
	const Vector3 eye = (m_Model.m_Header.boundingBox.min + m_Model.m_Header.boundingBox.max) * .5f + Vector3(modelRadius * .5f, 0.0f, 0.0f);
	m_Camera.SetEyeAtUp(eye, Vector3(kZero), Vector3(kYUnitVector));
	m_Camera.SetZRange(1.0f, 10000.0f);
	m_CameraController.reset(new CameraController(m_Camera, Vector3(kYUnitVector)));

	// Raytracing related initialization
	CreateOutputBuffers();
	InitializeViews();
	CreateRootSignatures();
	CreatePipelines();
	CreateAccelerationStructures();
	CreateShaderResourceHeap();
	CreateShaderBindingTables();

	m_MainViewport.TopLeftX = 0.0f;
	m_MainViewport.TopLeftY = 0.0f;
	m_MainViewport.Width = static_cast<FLOAT>(g_SceneColorBuffer.GetWidth());
	m_MainViewport.Height = static_cast<FLOAT>(g_SceneColorBuffer.GetHeight());
	m_MainViewport.MinDepth = 0.0f;
	m_MainViewport.MaxDepth = 1.0f;

	m_MainScissor.left = 0;
	m_MainScissor.top = 0;
	m_MainScissor.right = static_cast<LONG>(g_SceneColorBuffer.GetWidth());
	m_MainScissor.bottom = static_cast<LONG>(g_SceneColorBuffer.GetHeight());

	// Query Heap to measure performance
	D3D12_QUERY_HEAP_DESC desc{};
	desc.Type = D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
	desc.Count = 1;
	desc.NodeMask = 0;
	ThrowIfFailed(g_Device->CreateQueryHeap(&desc, IID_PPV_ARGS(&m_QueryHeap)));
}

void RaytracingTest::Cleanup(void)
{
	m_QueryHeap.Reset();

	m_pRTDevice.Reset();

	m_Model.Clear();
	m_HitShaderMeshInfo.Destroy();

	m_ScratchBuffer.Destroy();
	m_InstanceDataBuffer.Destroy();

	m_TLAS.Reset();
	for (int i = 0; i < m_BLASes.size(); i++)
	{
		m_BLASes[i].Reset();
	}
	m_BLASes.clear();

	m_RaytracingColorOutput.Destroy();
	m_AODirectionBuffer.Destroy();
	m_RayBinResult.Destroy();
	m_RayBinSizeResult.Destroy();
	m_RayBinningCB.Destroy();
	m_SrvUavHeap.Reset();
	m_pRaytracingDescriptorHeap.reset();

	m_DiffuseRaytracingPso.Destroy();
	m_AORaytracingPso.Destroy();

	m_DiffuseRaytracingInput.Cleanup();
	m_AORaytracingInput.Cleanup();

	m_DynamicConstantBuffer.Destroy();
}

void RaytracingTest::Update(float deltaT)
{
	ScopedTimer _prof(L"Update State");

	// Update something
	m_CameraController->Update(deltaT);
	m_ViewProjMatrix = m_Camera.GetViewProjMatrix();

	if (GameInput::IsFirstPressed(GameInput::kKey_1))
	{
		raytracingMode = RTM_Off;
		m_ModeCounter = 0;
	}
	else if (GameInput::IsFirstPressed(GameInput::kKey_2))
	{
		raytracingMode = RTM_RaytraceDiffuse;
		m_ModeCounter = 0;
	}
	else if (GameInput::IsFirstPressed(GameInput::kKey_3))
	{
		raytracingMode = RTM_RaytraceBinning;
		m_ModeCounter = 0;
	}


	if (GameInput::IsFirstPressed(GameInput::kKey_p))
	{
		std::cout << "Started frame profiling, please hold on..." << std::endl;
		m_IsProfiling = true;
		m_FramesCounted = 0;
		m_ModeCounter++;
	}

	if (m_IsProfiling)
	{
		m_FrameTimes[m_FramesCounted] = NestedTimingTree::GetTotalGpuTime();
		m_FramesCounted++;

		if (m_FramesCounted == PROFILE_MAX_FRAMES)
		{
			m_IsProfiling = false;

			// spit out total average.
			float avg = 0;
			for (float time : m_FrameTimes)
			{
				avg += time;
			}
			avg /= PROFILE_MAX_FRAMES;
			std::cout << raytracingModes[raytracingMode] << " " << m_ModeCounter << " - Average over 1000 frames:" << std::endl;
			std::cout << "    " << avg << " ms" << std::endl;
			std::cout << "==========================" << std::endl;
		}
	}
}

void RaytracingTest::RenderScene(void)
{
	const bool skipRasterize = raytracingMode != RTM_Off;
	const bool doRayBinning = raytracingMode == RTM_RaytraceBinning;

	GraphicsContext& gfxContext = GraphicsContext::Begin(L"Scene Render");

	auto pfnSetupGrahicsState = [&](void)
	{
		gfxContext.SetRootSignature(m_RootSig);
		gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfxContext.SetIndexBuffer(m_Model.m_IndexBuffer.IndexBufferView());
		gfxContext.SetVertexBuffer(0, m_Model.m_VertexBuffer.VertexBufferView());
	};

	auto copyOutputToBackBuffer = [&]()
	{
		// Transition our raytracing output and the back buffer to the propery copy states
		gfxContext.TransitionResource(m_RaytracingColorOutput, D3D12_RESOURCE_STATE_COPY_SOURCE);
		gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_COPY_DEST);

		// Wait for the transitions to end
		gfxContext.FlushResourceBarriers();

		// Issue copy command
		gfxContext.CopyBuffer(g_SceneColorBuffer, m_RaytracingColorOutput);

		// Transition the back buffer back to the rendertarget state
		gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	};

	// Z PrePass
	{
		ScopedTimer _prof(L"Z PrePass", gfxContext);
		pfnSetupGrahicsState();

		gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
		gfxContext.ClearDepth(g_SceneDepthBuffer);

		gfxContext.SetPipelineState(m_DepthPso);

		gfxContext.SetDepthStencilTarget(g_SceneDepthBuffer.GetDSV());
		gfxContext.SetViewportAndScissor(m_MainViewport, m_MainScissor);

		RenderModel(gfxContext);
	}

	// Basic rasterized color rendering
	if (!skipRasterize)
	{
		ScopedTimer _prof(L"Render Color", gfxContext);

		gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
		gfxContext.ClearColor(g_SceneColorBuffer);

		pfnSetupGrahicsState();

		gfxContext.SetPipelineState(m_ModelPso);

		gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_READ);
		gfxContext.SetRenderTarget(g_SceneColorBuffer.GetRTV(), g_SceneDepthBuffer.GetDSV_DepthReadOnly());
		gfxContext.SetViewportAndScissor(m_MainViewport, m_MainScissor);

		RenderModel(gfxContext);
	}
	// Raytracing
	else if (doRayBinning)
	{
		// Ambient Occlusion pass
		{
			ScopedTimer _prof(L"RTAO", gfxContext);
			ID3D12GraphicsCommandList* pCommandList = gfxContext.GetCommandList();
			ID3D12GraphicsCommandList4* pRtCommandList = static_cast<ID3D12GraphicsCommandList4*>(gfxContext.GetCommandList());

			ID3D12DescriptorHeap* pDescriptorHeaps[] = { &m_pRaytracingDescriptorHeap->GetDescriptorHeap() };
			pRtCommandList->SetDescriptorHeaps(ARRAYSIZE(pDescriptorHeaps), pDescriptorHeaps);
			gfxContext.SetRenderTarget(m_AODirectionBuffer.GetRTV());

			// Input data into the DynamicCB
			DynamicCB input;
			ZeroMemory(&input, sizeof(DynamicCB));
			const Matrix4& m0 = m_Camera.GetViewMatrix();
			const Matrix4& m1 = m_Camera.GetProjMatrix();
			Matrix4 m2 = Invert(m_Camera.GetViewMatrix());
			Matrix4 m3 = Invert(m_Camera.GetProjMatrix());
			memcpy(&input.view, &m0, sizeof(input.view));
			memcpy(&input.projection, &m1, sizeof(input.projection));
			memcpy(&input.viewI, &m2, sizeof(input.viewI));
			memcpy(&input.projectionI, &m3, sizeof(input.projectionI));
			input.frameCount = static_cast<uint>(Graphics::GetFrameCount());
			gfxContext.WriteBuffer(m_DynamicConstantBuffer, 0, &input, sizeof(input));

			gfxContext.TransitionResource(m_DynamicConstantBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			gfxContext.TransitionResource(m_AODirectionBuffer, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			gfxContext.FlushResourceBarriers();

			// Bind the raytracing parameters
			pCommandList->SetComputeRootSignature(m_GlobalRaytracingRootSignature.GetSignature());
			pCommandList->SetComputeRootDescriptorTable(0, m_AODirectionBufferUAV);
			pCommandList->SetComputeRootShaderResourceView(1, m_TLAS->GetGPUVirtualAddress());
			pCommandList->SetComputeRootConstantBufferView(2, m_DynamicConstantBuffer.GetGpuVirtualAddress());
			pCommandList->SetComputeRootDescriptorTable(3, m_ModelSRVs);

			// Dispatch the rays to start raytracing
			D3D12_DISPATCH_RAYS_DESC dispatchDesc = m_AORaytracingInput.GetDispatchRayDesc(g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight());
			pRtCommandList->SetPipelineState1(m_AORaytracingPso.Get());
			pRtCommandList->DispatchRays(&dispatchDesc);

			gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
		}

		// Ray binning pass
		{
			ComputeContext& cmpContext = gfxContext.GetComputeContext();
			ScopedTimer _prof(L"Ray Binning", cmpContext);

			cmpContext.TransitionResource(m_AODirectionBuffer, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			cmpContext.FlushResourceBarriers();

			cmpContext.SetRootSignature(m_RayBinningSignature);
			cmpContext.SetPipelineState(m_RayBinningPSO);

			RayBinningInfo input;
			ZeroMemory(&input, sizeof(RayBinningInfo));
			input.rayBinTileCountX = static_cast<uint>(std::ceil(g_SceneColorBuffer.GetWidth() / 16.0f));
			input.screenSize = float2{ static_cast<float>(g_SceneColorBuffer.GetWidth()), static_cast<float>(g_SceneColorBuffer.GetHeight()) };
			cmpContext.WriteBuffer(m_RayBinningCB, 0, &input, sizeof(input));

			cmpContext.TransitionResource(m_RayBinningCB, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			cmpContext.FlushResourceBarriers();

			cmpContext.SetDescriptorTable(0, m_RayBinningUAVs);
			cmpContext.SetConstantBuffer(1, m_RayBinningCB.GetGpuVirtualAddress());

			cmpContext.Dispatch(16, 16, 1);
		}

		// Main Binned Diffuse raytrace pass
		{
			ScopedTimer _prof(L"Binned Raytrace Color", gfxContext);
			ID3D12GraphicsCommandList* pCommandList = gfxContext.GetCommandList();
			ID3D12GraphicsCommandList4* pRtCommandList = static_cast<ID3D12GraphicsCommandList4*>(gfxContext.GetCommandList());

			// Transition our raytracing output so that we can write to it during the raytracing.
			gfxContext.TransitionResource(m_RaytracingColorOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

			ID3D12DescriptorHeap* pDescriptorHeaps[] = { &m_pRaytracingDescriptorHeap->GetDescriptorHeap() };
			pRtCommandList->SetDescriptorHeaps(ARRAYSIZE(pDescriptorHeaps), pDescriptorHeaps);
			gfxContext.SetRenderTarget(m_RaytracingColorOutput.GetRTV());

			// Input data into the DynamicCB
			DynamicCB input;
			ZeroMemory(&input, sizeof(DynamicCB));
			const Matrix4& m0 = m_Camera.GetViewMatrix();
			const Matrix4& m1 = m_Camera.GetProjMatrix();
			Matrix4 m2 = Invert(m_Camera.GetViewMatrix());
			Matrix4 m3 = Invert(m_Camera.GetProjMatrix());
			memcpy(&input.view, &m0, sizeof(input.view));
			memcpy(&input.projection, &m1, sizeof(input.projection));
			memcpy(&input.viewI, &m2, sizeof(input.viewI));
			memcpy(&input.projectionI, &m3, sizeof(input.projectionI));
			input.frameCount = static_cast<uint>(Graphics::GetFrameCount());
			gfxContext.WriteBuffer(m_DynamicConstantBuffer, 0, &input, sizeof(input));

			// Input data into the RayBinningInfo
			RayBinningInfo rayBinInput;
			ZeroMemory(&rayBinInput, sizeof(RayBinningInfo));
			rayBinInput.rayBinTileCountX = static_cast<uint>(std::ceil(g_SceneColorBuffer.GetWidth() / 16.0f));
			rayBinInput.screenSize = float2{ static_cast<float>(g_SceneColorBuffer.GetWidth()), static_cast<float>(g_SceneColorBuffer.GetHeight()) };
			gfxContext.WriteBuffer(m_RayBinningCB, 0, &rayBinInput, sizeof(rayBinInput));

			// Transition resources + and wait for finish
			gfxContext.TransitionResource(m_DynamicConstantBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			gfxContext.TransitionResource(m_RayBinningCB, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			gfxContext.FlushResourceBarriers();

			// Bind the raytracing parameters
			pCommandList->SetComputeRootSignature(m_GlobalBinnedRaytracingRS.GetSignature());
			pCommandList->SetComputeRootDescriptorTable(0, m_RaytracingOutputUAV);
			pCommandList->SetComputeRootShaderResourceView(1, m_TLAS->GetGPUVirtualAddress());
			pCommandList->SetComputeRootConstantBufferView(2, m_DynamicConstantBuffer.GetGpuVirtualAddress());
			pCommandList->SetComputeRootDescriptorTable(3, m_ModelSRVs);
			pCommandList->SetComputeRootDescriptorTable(4, m_AODirectionBufferSRV);
			pCommandList->SetComputeRootDescriptorTable(5, m_RayBinningUAVs);
			pCommandList->SetComputeRootConstantBufferView(6, m_RayBinningCB.GetGpuVirtualAddress());

			// BufferSizeX:
			const int32_t binningTileSize = 16;
			int32_t numTilesRayBinX = (g_SceneColorBuffer.GetWidth() + (binningTileSize - 1)) / binningTileSize;
			int32_t numTilesRayBinY = (g_SceneColorBuffer.GetHeight() + (binningTileSize - 1)) / binningTileSize;
			int32_t bufferSizeX = numTilesRayBinX * binningTileSize;
			int32_t bufferSizeY = numTilesRayBinY * binningTileSize;

			// dispatch rays over 1D-array, according to Unity HDRP this is better.
			uint32_t dispatchSize = bufferSizeX * bufferSizeY;

			// Dispatch the rays to start raytracing
			D3D12_DISPATCH_RAYS_DESC dispatchDesc = m_BinnedDiffuseRaytracingInput.GetDispatchRayDesc(dispatchSize, 1);
			pRtCommandList->SetPipelineState1(m_BinnedDiffusePSO.Get());
			pRtCommandList->DispatchRays(&dispatchDesc);


			copyOutputToBackBuffer();
		}
	}
	else
	{
		// Main Diffuse raytrace pass
		{
			ScopedTimer _prof(L"Raytrace Color", gfxContext);
			ID3D12GraphicsCommandList* pCommandList = gfxContext.GetCommandList();
			ID3D12GraphicsCommandList4* pRtCommandList = static_cast<ID3D12GraphicsCommandList4*>(gfxContext.GetCommandList());

			// Transition our raytracing output so that we can write to it during the raytracing.
			gfxContext.TransitionResource(m_RaytracingColorOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			gfxContext.FlushResourceBarriers();

			ID3D12DescriptorHeap* pDescriptorHeaps[] = { &m_pRaytracingDescriptorHeap->GetDescriptorHeap() };
			pRtCommandList->SetDescriptorHeaps(ARRAYSIZE(pDescriptorHeaps), pDescriptorHeaps);
			gfxContext.SetRenderTarget(m_RaytracingColorOutput.GetRTV());

			// Input data into the DynamicCB
			DynamicCB input;
			ZeroMemory(&input, sizeof(DynamicCB));
			const Matrix4& m0 = m_Camera.GetViewMatrix();
			const Matrix4& m1 = m_Camera.GetProjMatrix();
			Matrix4 m2 = Invert(m_Camera.GetViewMatrix());
			Matrix4 m3 = Invert(m_Camera.GetProjMatrix());
			memcpy(&input.view, &m0, sizeof(input.view));
			memcpy(&input.projection, &m1, sizeof(input.projection));
			memcpy(&input.viewI, &m2, sizeof(input.viewI));
			memcpy(&input.projectionI, &m3, sizeof(input.projectionI));
			input.frameCount = static_cast<uint>(Graphics::GetFrameCount());
			gfxContext.WriteBuffer(m_DynamicConstantBuffer, 0, &input, sizeof(input));

			gfxContext.TransitionResource(m_DynamicConstantBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			gfxContext.FlushResourceBarriers();

			// Bind the raytracing parameters
			pCommandList->SetComputeRootSignature(m_GlobalRaytracingRootSignature.GetSignature());
			pCommandList->SetComputeRootDescriptorTable(0, m_RaytracingOutputUAV);
			pCommandList->SetComputeRootShaderResourceView(1, m_TLAS->GetGPUVirtualAddress());
			pCommandList->SetComputeRootConstantBufferView(2, m_DynamicConstantBuffer.GetGpuVirtualAddress());
			pCommandList->SetComputeRootDescriptorTable(3, m_ModelSRVs);

			// Dispatch the rays to start raytracing
			D3D12_DISPATCH_RAYS_DESC dispatchDesc = m_DiffuseRaytracingInput.GetDispatchRayDesc(g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight());
			pRtCommandList->SetPipelineState1(m_DiffuseRaytracingPso.Get());
			pRtCommandList->DispatchRays(&dispatchDesc);


			// Copy raytrace output to the back-buffer
			copyOutputToBackBuffer();
		}
	}

	gfxContext.Finish();
}

void RaytracingTest::InitializeViews()
{
	// UAV for the raytracing output buffer
	{
		D3D12_CPU_DESCRIPTOR_HANDLE uavHandle;
		UINT uavDescriptorIndex;
		m_pRaytracingDescriptorHeap->AllocateDescriptor(uavHandle, uavDescriptorIndex);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_RaytracingColorOutput.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_RaytracingOutputUAV = m_pRaytracingDescriptorHeap->GetGpuHandle(uavDescriptorIndex);
	}

	// UAV and SRV for the AO raytracing output buffer
	{
		// UAV
		D3D12_CPU_DESCRIPTOR_HANDLE uavHandle;
		UINT idx;
		m_pRaytracingDescriptorHeap->AllocateDescriptor(uavHandle, idx);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_AODirectionBuffer.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_AODirectionBufferUAV = m_pRaytracingDescriptorHeap->GetGpuHandle(idx);

		// SRV
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
		m_pRaytracingDescriptorHeap->AllocateDescriptor(srvHandle, idx);
		g_Device->CopyDescriptorsSimple(1, srvHandle, m_AODirectionBuffer.GetSRV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_AODirectionBufferSRV = m_pRaytracingDescriptorHeap->GetGpuHandle(idx);
	}

	// UAV for the Ray binning output buffer
	{
		D3D12_CPU_DESCRIPTOR_HANDLE uavHandle;
		UINT uavDescIdxFirst;
		UINT unused;

		m_pRaytracingDescriptorHeap->AllocateDescriptor(uavHandle, uavDescIdxFirst);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_AODirectionBuffer.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// binning output buffer
		m_pRaytracingDescriptorHeap->AllocateDescriptor(uavHandle, unused);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_RayBinResult.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		// binning output sizes buffer
		m_pRaytracingDescriptorHeap->AllocateDescriptor(uavHandle, unused);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_RayBinSizeResult.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		
		m_RayBinningUAVs = m_pRaytracingDescriptorHeap->GetGpuHandle(uavDescIdxFirst);
	}

	// SRVs for the model textures
	{
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
		UINT srvDescriptorIndex;

		// Mesh info
		m_pRaytracingDescriptorHeap->AllocateDescriptor(srvHandle, srvDescriptorIndex);
		g_Device->CopyDescriptorsSimple(1, srvHandle, m_ModelMeshInfo, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_ModelSRVs = m_pRaytracingDescriptorHeap->GetGpuHandle(srvDescriptorIndex);

		UINT unused;
		m_pRaytracingDescriptorHeap->AllocateDescriptor(srvHandle, unused);
		g_Device->CopyDescriptorsSimple(1, srvHandle, m_ModelIndices, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		m_pRaytracingDescriptorHeap->AllocateBufferSrv(*const_cast<ID3D12Resource*>(m_Model.m_VertexBuffer.GetResource()));

		for (UINT i = 0; i < m_Model.m_Header.materialCount; i++)
		{
			UINT slot;
			m_pRaytracingDescriptorHeap->AllocateDescriptor(srvHandle, slot);
			g_Device->CopyDescriptorsSimple(1, srvHandle, *m_Model.GetSRVs(i), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			m_pRaytracingDescriptorHeap->AllocateDescriptor(srvHandle, unused);
			g_Device->CopyDescriptorsSimple(1, srvHandle, m_Model.GetSRVs(i)[3], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			m_ModelDescriptors[i] = m_pRaytracingDescriptorHeap->GetGpuHandle(slot);
		}
	}
}

void RaytracingTest::CreateRootSignatures()
{
	// Global root signature
	{
		m_GlobalRaytracingRootSignature.Reset(4, 0);
		m_GlobalRaytracingRootSignature[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1); // Render Target
		m_GlobalRaytracingRootSignature[1].InitAsBufferSRV(0); // Acceleration Structure
		m_GlobalRaytracingRootSignature[2].InitAsConstantBuffer(0); // DynamicCB
		m_GlobalRaytracingRootSignature[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // Mesh info
		m_GlobalRaytracingRootSignature.Finalize(L"Global RT Root Signature");
	}

	// Binning global root signature
	{
		m_GlobalBinnedRaytracingRS.Reset(7, 0);
		m_GlobalBinnedRaytracingRS[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1); // Render Target
		m_GlobalBinnedRaytracingRS[1].InitAsBufferSRV(0); // Acceleration Structure
		m_GlobalBinnedRaytracingRS[2].InitAsConstantBuffer(0); // DynamicCB
		m_GlobalBinnedRaytracingRS[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // Mesh info
		m_GlobalBinnedRaytracingRS[4].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 6, 1); // AO Directions
		m_GlobalBinnedRaytracingRS[5].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 10, 2); // Ray binning results
		m_GlobalBinnedRaytracingRS[6].InitAsConstantBuffer(1); // BinnedRayGenInput
		m_GlobalBinnedRaytracingRS.Finalize(L"Global Binned RT Root Signature");
	}

#pragma region Diffuse Root Signatures
	// Diffuse Ray Gen shader local root signature
	{
		m_DiffuseRayGenLocalSignature.Reset(0, 0);
		m_DiffuseRayGenLocalSignature.Finalize(L"RT diffuse Ray Gen Local Root Signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}

	// Hit shader local root signature
	{
		SamplerDesc sampler;
		m_DiffuseHitGroupLocalSignature.Reset(2, 1);
		m_DiffuseHitGroupLocalSignature.InitStaticSampler(0, sampler);
		m_DiffuseHitGroupLocalSignature[0].InitAsDescriptorTable(1);
		m_DiffuseHitGroupLocalSignature[0].SetTableRange(0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 2, 0); // Current mesh texture
		const UINT sizeOfRootConstantInDWords = (sizeof(MeshRootConstant) - 1) / sizeof(DWORD) + 1;
		m_DiffuseHitGroupLocalSignature[1].InitAsConstants(1, sizeOfRootConstantInDWords); // Current mesh material id
		m_DiffuseHitGroupLocalSignature.Finalize(L"RT diffuse Hit Local Root Signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}
#pragma endregion

#pragma region Binned Diffuse Root Signatures
	// Binned Diffuse ray Gen shader local root signature
	{
		m_BinnedDiffuseRayGenLocalSignature.Reset(0, 0);
		m_BinnedDiffuseRayGenLocalSignature.Finalize(L"Binned RT diffuse Ray Gen Local Root Signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}

	// Binned Diffuse hit local root signature
	{
		SamplerDesc sampler;
		m_BinnedDiffuseHitGroupLocalSignature.Reset(2, 1);
		m_BinnedDiffuseHitGroupLocalSignature.InitStaticSampler(0, sampler);
		m_BinnedDiffuseHitGroupLocalSignature[0].InitAsDescriptorTable(1);
		m_BinnedDiffuseHitGroupLocalSignature[0].SetTableRange(0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 2, 0); // Current mesh texture
		const UINT sizeOfRootConstantInDWords = (sizeof(MeshRootConstant) - 1) / sizeof(DWORD) + 1;
		m_BinnedDiffuseHitGroupLocalSignature[1].InitAsConstants(2, sizeOfRootConstantInDWords); // Current mesh material id
		m_BinnedDiffuseHitGroupLocalSignature.Finalize(L"RT diffuse Hit Local Root Signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}

	// Miss shader local root signature
	{
		m_DiffuseMissLocalSignature.Reset(0, 0);
		m_DiffuseMissLocalSignature.Finalize(L"RT diffuse Miss Local Root Signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}
#pragma endregion

#pragma region AO Root Signatures
	// Ray Gen
	{
		m_AORayGenLocalSignature.Reset(0, 0);
		m_AORayGenLocalSignature.Finalize(L"RT AO Ray Gen Local Root Signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}

	// Hit
	{
		m_AOHitGroupLocalSignature.Reset(1, 0);
		const UINT rootConstantSize = (sizeof(MeshRootConstant) - 1) / sizeof(DWORD) + 1;
		m_AOHitGroupLocalSignature[0].InitAsConstants(1, rootConstantSize);
		m_AOHitGroupLocalSignature.Finalize(L"RT AO Hit Local Root Signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}

	// Miss
	{
		m_AOMissLocalSignature.Reset(0, 0);
		m_AOMissLocalSignature.Finalize(L"RT AO Miss Local Root Signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}
#pragma endregion

#pragma region Ray Binning Compute Signature
	{
		m_RayBinningSignature.Reset(2, 0);
		m_RayBinningSignature[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 3);
		m_RayBinningSignature[1].InitAsConstantBuffer(0);
		m_RayBinningSignature.Finalize(L"Ray Binning Root Signature");
	}
#pragma endregion 
}

static const LPCWSTR DIFFUSE_RAY_GEN_EXPORT_NAME = L"DiffuseRayGen";
static const LPCWSTR DIFFUSE_CLOSEST_HIT_EXPORT_NAME = L"DiffuseClosestHit";
static const LPCWSTR DIFFUSE_MISS_EXPORT_NAME = L"DiffuseMiss";
static const LPCWSTR DIFFUSE_HIT_GROUP_NAME = L"DiffuseHitGroup";

static const LPCWSTR BINNED_DIFFUSE_RAY_GEN_EXPORT_NAME = L"DiffuseRayGenBinned";
static const LPCWSTR BINNED_CLOSEST_HIT_EXPORT_NAME = L"DiffuseClosestHitBinned";
static const LPCWSTR BINNED_DIFFUSE_HIT_GROUP_NAME = L"BinnedDiffuseHitGroup";

static const LPCWSTR AO_RAY_GEN_EXPORT_NAME = L"AORayGen";
static const LPCWSTR AO_CLOSEST_HIT_EXPORT_NAME = L"AOClosestHit";
static const LPCWSTR AO_MISS_EXPORT_NAME = L"AOMiss";
static const LPCWSTR AO_HIT_GROUP_NAME = L"AOHitGroup";

void RaytracingTest::CreatePipelines()
{
	CreateDiffuseRaytracingPSO();
	CreateAORaytracingPSO();
	CreateBinningPSO();
	CreateBinnedDiffusePSO();
}

void RaytracingTest::CreateDiffuseRaytracingPSO()
{
	// RayGen Shader
	m_DiffuseRaytracingPso.AddShader(g_pDiffuseRayGen, ARRAYSIZE(g_pDiffuseRayGen), DIFFUSE_RAY_GEN_EXPORT_NAME);

	// Hit shader
	m_DiffuseRaytracingPso.AddShader(g_pDiffuseHit, ARRAYSIZE(g_pDiffuseHit), DIFFUSE_CLOSEST_HIT_EXPORT_NAME);

	// Miss shader
	m_DiffuseRaytracingPso.AddShader(g_pDiffuseMiss, ARRAYSIZE(g_pDiffuseHit), DIFFUSE_MISS_EXPORT_NAME);

	// Triangle hit group
	m_DiffuseRaytracingPso.AddClosestHitGroup(DIFFUSE_CLOSEST_HIT_EXPORT_NAME, DIFFUSE_HIT_GROUP_NAME, D3D12_HIT_GROUP_TYPE_TRIANGLES);

	// Shader + pipeline config
	const UINT payloadSize = 2 * sizeof(float); // skipShading + distance (skipShading is a bool, but in HLSL a bool seems to be 4 bytes)
	const UINT attributeSize = 2 * sizeof(float); // float2 barycentrics
	const UINT maxRecursionDepth = 2;
	m_DiffuseRaytracingPso.Configure(payloadSize, attributeSize, maxRecursionDepth);


	// Ray Gen local root signature + association
	m_DiffuseRaytracingPso.AddLocalRootSignature(m_DiffuseRayGenLocalSignature, DIFFUSE_RAY_GEN_EXPORT_NAME);

	// Miss shader local root signature + association
	m_DiffuseRaytracingPso.AddLocalRootSignature(m_DiffuseMissLocalSignature, DIFFUSE_MISS_EXPORT_NAME);

	// Hit group local root signature + association
	m_DiffuseRaytracingPso.AddLocalRootSignature(m_DiffuseHitGroupLocalSignature, DIFFUSE_HIT_GROUP_NAME);

	// Global root signature
	m_DiffuseRaytracingPso.SetGlobalRootSignature(m_GlobalRaytracingRootSignature);

	// Create the diffuse raytracing state object
	HRESULT hr = m_DiffuseRaytracingPso.Finalize(m_pRTDevice);
	if (FAILED(hr))
	{
		throw std::logic_error("Failed to create Diffuse raytracing PSO");
	}
}

void RaytracingTest::CreateAORaytracingPSO()
{
	// RayGen Shader
	m_AORaytracingPso.AddShader(g_pAORayGen, ARRAYSIZE(g_pAORayGen), AO_RAY_GEN_EXPORT_NAME);

	// Hit shader
	m_AORaytracingPso.AddShader(g_pAOHit, ARRAYSIZE(g_pAOHit), AO_CLOSEST_HIT_EXPORT_NAME);

	// Miss shader
	m_AORaytracingPso.AddShader(g_pAOMiss, ARRAYSIZE(g_pAOMiss), AO_MISS_EXPORT_NAME);

	// Triangle hit group
	m_AORaytracingPso.AddClosestHitGroup(AO_CLOSEST_HIT_EXPORT_NAME, AO_HIT_GROUP_NAME, D3D12_HIT_GROUP_TYPE_TRIANGLES);

	// Shader + pipeline config
	const UINT payloadSize = 2 * sizeof(float); // skipShading + distance (skipShading is a bool, but in HLSL a bool seems to be 4 bytes)
	const UINT attributeSize = 2 * sizeof(float); // float2 barycentrics
	const UINT maxRecursionDepth = 2;
	m_AORaytracingPso.Configure(payloadSize, attributeSize, maxRecursionDepth);


	// Ray Gen local root signature + association
	m_AORaytracingPso.AddLocalRootSignature(m_AORayGenLocalSignature, AO_RAY_GEN_EXPORT_NAME);

	// Miss shader local root signature
	m_AORaytracingPso.AddLocalRootSignature(m_AOMissLocalSignature, AO_MISS_EXPORT_NAME);

	// Hit group local root signature
	m_AORaytracingPso.AddLocalRootSignature(m_AOHitGroupLocalSignature, AO_HIT_GROUP_NAME);

	// Global root signature
	m_AORaytracingPso.SetGlobalRootSignature(m_GlobalRaytracingRootSignature);


	// Create the AO raytracing state object
	HRESULT hr = m_AORaytracingPso.Finalize(m_pRTDevice);
	if (FAILED(hr))
	{
		throw std::logic_error("Failed to create AO raytracing PSO");
	}
}

void RaytracingTest::CreateBinningPSO()
{
	m_RayBinningPSO.SetRootSignature(m_RayBinningSignature);
	m_RayBinningPSO.SetComputeShader(g_pRayBinningCompute, sizeof(g_pRayBinningCompute));
	m_RayBinningPSO.Finalize();
}

void RaytracingTest::CreateBinnedDiffusePSO()
{
	m_BinnedDiffusePSO.AddShader(g_pDiffuseRayGenBinned, ARRAYSIZE(g_pDiffuseRayGenBinned), BINNED_DIFFUSE_RAY_GEN_EXPORT_NAME);
	m_BinnedDiffusePSO.AddShader(g_pDiffuseHitBinned, ARRAYSIZE(g_pDiffuseHitBinned), BINNED_CLOSEST_HIT_EXPORT_NAME);
	m_BinnedDiffusePSO.AddShader(g_pDiffuseMiss, ARRAYSIZE(g_pDiffuseMiss), DIFFUSE_MISS_EXPORT_NAME);
	m_BinnedDiffusePSO.AddClosestHitGroup(BINNED_CLOSEST_HIT_EXPORT_NAME, BINNED_DIFFUSE_HIT_GROUP_NAME, D3D12_HIT_GROUP_TYPE_TRIANGLES);
	
	const UINT payloadSize = 2 * sizeof(float);
	const UINT attributeSize = 2 * sizeof(float);
	const UINT maxRecursionDepth = 2;
	m_BinnedDiffusePSO.Configure(payloadSize, attributeSize, maxRecursionDepth);
	
	m_BinnedDiffusePSO.AddLocalRootSignature(m_BinnedDiffuseRayGenLocalSignature, BINNED_DIFFUSE_RAY_GEN_EXPORT_NAME);
	m_BinnedDiffusePSO.AddLocalRootSignature(m_DiffuseMissLocalSignature, DIFFUSE_MISS_EXPORT_NAME);
	m_BinnedDiffusePSO.AddLocalRootSignature(m_BinnedDiffuseHitGroupLocalSignature, BINNED_DIFFUSE_HIT_GROUP_NAME);
	
	m_BinnedDiffusePSO.SetGlobalRootSignature(m_GlobalBinnedRaytracingRS);
	
	HRESULT hr = m_BinnedDiffusePSO.Finalize(m_pRTDevice);
	if (FAILED(hr))
	{
		throw std::logic_error("Failed to create Binned diffuse raytracing PSO");
	}
}

void RaytracingTest::CreateAccelerationStructures()
{
	UINT numMeshes = m_Model.m_Header.meshCount;
	const UINT numBottomLevels = 1;

	// Get the TLAS prebuild info, so that we know how much scratch buffer size we need
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO tlasPrebuildInfo;
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC tlasDesc = {};
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& tlasInputs = tlasDesc.Inputs;
	tlasInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	tlasInputs.NumDescs = numBottomLevels;
	tlasInputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
	tlasInputs.pGeometryDescs = nullptr;
	tlasInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	m_pRTDevice->GetRaytracingAccelerationStructurePrebuildInfo(&tlasInputs, &tlasPrebuildInfo);

	const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS buildFlags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> geometryDescs(numMeshes);
	UINT64 scratchBufferSizeNeeded = tlasPrebuildInfo.ScratchDataSizeInBytes;

	// set up the descriptor for the mesh.
	for (UINT i = 0; i < numMeshes; i++)
	{
		auto& mesh = m_Model.m_pMesh[i];

		D3D12_RAYTRACING_GEOMETRY_DESC& desc = geometryDescs[i];
		desc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
		desc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;

		D3D12_RAYTRACING_GEOMETRY_TRIANGLES_DESC& trianglesDesc = desc.Triangles;
		trianglesDesc.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		trianglesDesc.VertexCount = mesh.vertexCount;
		trianglesDesc.VertexBuffer.StartAddress = m_Model.m_VertexBuffer.GetGpuVirtualAddress() + (mesh.vertexDataByteOffset + mesh.attrib[Model::attrib_position].offset);
		trianglesDesc.VertexBuffer.StrideInBytes = mesh.vertexStride;
		trianglesDesc.IndexBuffer = m_Model.m_IndexBuffer.GetGpuVirtualAddress() + mesh.indexDataByteOffset;
		trianglesDesc.IndexCount = mesh.indexCount;
		trianglesDesc.IndexFormat = DXGI_FORMAT_R16_UINT;
		trianglesDesc.Transform3x4 = 0;
	}

	// Prepare the BLAS create structs.
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
		m_pRTDevice->GetRaytracingAccelerationStructurePrebuildInfo(&blasInputs, &blasPrebuildInfo);

		blasSize[i] = blasPrebuildInfo.ResultDataMaxSizeInBytes;
		scratchBufferSizeNeeded = std::max(blasPrebuildInfo.ScratchDataSizeInBytes, scratchBufferSizeNeeded);
	}

	// In order to build the Acceleration Structure, we need some extra scratch storage.
	m_ScratchBuffer.Create(L"Acceleration Structure Scratch Buffer", (UINT)scratchBufferSizeNeeded, 1);

	D3D12_HEAP_PROPERTIES defaultHeapDesc = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto topLevelDesc = CD3DX12_RESOURCE_DESC::Buffer(tlasPrebuildInfo.ResultDataMaxSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	g_Device->CreateCommittedResource(
		&defaultHeapDesc,
		D3D12_HEAP_FLAG_NONE,
		&topLevelDesc,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		nullptr,
		IID_PPV_ARGS(&m_TLAS));

	tlasDesc.DestAccelerationStructureData = m_TLAS->GetGPUVirtualAddress();
	tlasDesc.ScratchAccelerationStructureData = m_ScratchBuffer.GetGpuVirtualAddress();

	// Create the BLASes
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC> instanceDescs(numBottomLevels);
	m_BLASes.resize(numBottomLevels);
	for (UINT i = 0; i < blasDescs.size(); i++)
	{
		auto& blas = m_BLASes[i];

		auto bottomLevelDesc = CD3DX12_RESOURCE_DESC::Buffer(blasSize[i], D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		g_Device->CreateCommittedResource(
			&defaultHeapDesc,
			D3D12_HEAP_FLAG_NONE,
			&bottomLevelDesc,
			D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
			nullptr,
			IID_PPV_ARGS(&blas));

		blasDescs[i].DestAccelerationStructureData = blas->GetGPUVirtualAddress();
		blasDescs[i].ScratchAccelerationStructureData = m_ScratchBuffer.GetGpuVirtualAddress();

		D3D12_RAYTRACING_INSTANCE_DESC& instanceDesc = instanceDescs[i];
		// TODO: g_pRaytracingDescriptorHeap is the DescriptorHeapStack, can we just use the auto heap by the RootSignature?
		UINT descriptorIndex = m_pRaytracingDescriptorHeap->AllocateBufferUav(*blas.Get());
		UNREFERENCED_PARAMETER(descriptorIndex);

		// Identity matrix
		ZeroMemory(instanceDesc.Transform, sizeof(instanceDesc.Transform));
		instanceDesc.Transform[0][0] = 1.0f;
		instanceDesc.Transform[1][1] = 1.0f;
		instanceDesc.Transform[2][2] = 1.0f;

		instanceDesc.AccelerationStructure = m_BLASes[i]->GetGPUVirtualAddress();
		instanceDesc.Flags = 0;
		instanceDesc.InstanceID = 0;
		instanceDesc.InstanceMask = 1;
		instanceDesc.InstanceContributionToHitGroupIndex = i;
	}

	m_InstanceDataBuffer.Create(L"Instance Data Buffer", numBottomLevels, sizeof(D3D12_RAYTRACING_INSTANCE_DESC), instanceDescs.data());

	tlasInputs.InstanceDescs = m_InstanceDataBuffer.GetGpuVirtualAddress();
	tlasInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;

	// Finally, here we create the acceleration structure.
	GraphicsContext& gfxContext = GraphicsContext::Begin(L"Create Acceleration Structure");
	ID3D12GraphicsCommandList* pCommandList = gfxContext.GetCommandList();

	ComPtr<ID3D12GraphicsCommandList4> pRaytracingCommandList;
	ASSERT_SUCCEEDED(pCommandList->QueryInterface(IID_PPV_ARGS(&pRaytracingCommandList)));

	ID3D12DescriptorHeap* descriptorHeaps[] = { &m_pRaytracingDescriptorHeap->GetDescriptorHeap() };
	pRaytracingCommandList->SetDescriptorHeaps(ARRAYSIZE(descriptorHeaps), descriptorHeaps);

	auto uavBarrier = CD3DX12_RESOURCE_BARRIER::UAV(nullptr);
	for (UINT i = 0; i < blasDescs.size(); i++)
	{
		pRaytracingCommandList->BuildRaytracingAccelerationStructure(&blasDescs[i], 0, nullptr);
	}
	pCommandList->ResourceBarrier(1, &uavBarrier);

	pRaytracingCommandList->BuildRaytracingAccelerationStructure(&tlasDesc, 0, nullptr);

	gfxContext.Finish();
}

void RaytracingTest::CreateOutputBuffers()
{
	// Don't use CreateFromSwapChain here, because that will link the output with the scene color buffer, aka stuff goes wrong.
	m_RaytracingColorOutput.Create(L"Raytracing Output Color Buffer", g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight(), 1, g_SceneColorBuffer.GetFormat());

	// we need DXGI_FORMAT_R32G32B32A32_FLOAT because we're storing float3 + depth -> we need the precision.
	m_AODirectionBuffer.Create(L"AO Direction + Depth Buffer", g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight(), 1, DXGI_FORMAT_R32G32B32A32_FLOAT);

	const int binningTileSize = 16;
	const int numTilesX = (g_SceneColorBuffer.GetWidth() + (binningTileSize - 1)) / binningTileSize;
	const int numTilesY = (g_SceneColorBuffer.GetHeight() + (binningTileSize - 1)) / binningTileSize;
	const int bufferSizeX = numTilesX * binningTileSize;
	const int bufferSizeY = numTilesY * binningTileSize;
	m_RayBinResult.Create(L"Ray Bin Result Buffer", bufferSizeX * bufferSizeY, sizeof(uint));
	m_RayBinSizeResult.Create(L"Ray Bin Size Result Buffer", bufferSizeX * bufferSizeY, sizeof(uint));
}

void RaytracingTest::CreateShaderResourceHeap()
{
	// Create a SRV/UAV/CBV descriptor heap. entries:
	// - UAV for raytracing color output
	// - UAV for AO direction output
	// - UAV for Ray Bin Result buffer
	// - UAV for Ray Bin Size Result buffer
	// - CBV for RayBinningInfo (information required for the ray binning)
	// - SRV for TLAS
	// - CBV for DynamicConstantBuffer (camera and viewport data for ray generation)
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 7;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ASSERT_SUCCEEDED(g_Device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_SrvUavHeap)));

	// Get the handle so we can write to it on the CPU side
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = m_SrvUavHeap->GetCPUDescriptorHandleForHeapStart();

	// Create the raytracing color output UAV
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	g_Device->CreateUnorderedAccessView(m_RaytracingColorOutput.GetResource(), nullptr, &uavDesc, srvHandle);

	// Create the AO direction output UAV
	srvHandle = m_SrvUavHeap->GetCPUDescriptorHandleForHeapStart();
	uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	g_Device->CreateUnorderedAccessView(m_AODirectionBuffer.GetResource(), nullptr, &uavDesc, srvHandle);
	

	// Create Ray Bin Result buffer UAV
	srvHandle = m_SrvUavHeap->GetCPUDescriptorHandleForHeapStart();
	uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 1;
	uavDesc.Buffer.StructureByteStride = sizeof(uint);
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	g_Device->CreateUnorderedAccessView(m_RayBinResult.GetResource(), nullptr, &uavDesc, srvHandle);
	
	// Create Ray Bin Size Result buffer UAV
	srvHandle = m_SrvUavHeap->GetCPUDescriptorHandleForHeapStart();
	uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 1;
	uavDesc.Buffer.StructureByteStride = sizeof(uint);
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	g_Device->CreateUnorderedAccessView(m_RayBinSizeResult.GetResource(), nullptr, &uavDesc, srvHandle);

	m_RayBinningCB.Create(L"Ray Binning Constant Buffer", 1, sizeof(RayBinningInfo));

	// Add TLAS as SRV
	srvHandle.ptr += g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = m_InstanceDataBuffer->GetGPUVirtualAddress();
	g_Device->CreateShaderResourceView(nullptr, &srvDesc, srvHandle);

	m_DynamicConstantBuffer.Create(L"Dynamic Constant Buffer", 1, sizeof(DynamicCB));
}

void RaytracingTest::CreateShaderBindingTables()
{
	// Diffuse shader table
	{
		const UINT shaderIdentifierSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		const UINT offsetToDescriptorHandle = ALIGN(sizeof(D3D12_GPU_DESCRIPTOR_HANDLE), shaderIdentifierSize);
		const UINT offsetToMaterialConstants = ALIGN(sizeof(UINT32), offsetToDescriptorHandle + sizeof(D3D12_GPU_DESCRIPTOR_HANDLE));
		const UINT shaderRecordSizeInBytes = ALIGN(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, offsetToMaterialConstants + sizeof(UINT));

		std::vector<byte> pHitShaderTable(shaderRecordSizeInBytes * m_Model.m_Header.meshCount);

		auto GetShaderTable = [=](const Model& model, ID3D12StateObject* pPSO, byte* pShaderTable)
		{
			ID3D12StateObjectProperties* stateObjectProperties = nullptr;
			ThrowIfFailed(pPSO->QueryInterface(IID_PPV_ARGS(&stateObjectProperties)));
			void* pHitGroupIdentifierData = stateObjectProperties->GetShaderIdentifier(DIFFUSE_HIT_GROUP_NAME);

			for (UINT i = 0; i < model.m_Header.meshCount; i++)
			{
				// First shader record: this identifies the hit group for which this table entry is.
				byte* pShaderRecord = i * shaderRecordSizeInBytes + pShaderTable;
				memcpy(pShaderRecord, pHitGroupIdentifierData, shaderIdentifierSize);

				// Second shader record: binds the shader descriptors.
				UINT materialIndex = model.m_pMesh[i].materialIndex;
				memcpy(pShaderRecord + offsetToDescriptorHandle, &m_ModelDescriptors[materialIndex].ptr, sizeof(m_ModelDescriptors[materialIndex].ptr));

				// Third shader record: mesh id, so that we can index into our vertex buffer correctly.
				MeshRootConstant meshConst;
				meshConst.meshId = i;
				memcpy(pShaderRecord + offsetToMaterialConstants, &meshConst, sizeof(meshConst));
			}
		};

		GetShaderTable(m_Model, m_DiffuseRaytracingPso.Get(), pHitShaderTable.data());
		m_DiffuseRaytracingInput = RaytracingDispatchRayInputs(m_DiffuseRaytracingPso.Get(), pHitShaderTable.data(), shaderRecordSizeInBytes, (UINT)pHitShaderTable.size(), DIFFUSE_RAY_GEN_EXPORT_NAME, DIFFUSE_MISS_EXPORT_NAME);
	}

	// AO shader table
	{
		const UINT shaderIdentifierSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		const UINT offsetToMaterialConstants = ALIGN(sizeof(UINT32), shaderIdentifierSize);
		const UINT shaderRecordSizeInBytes = ALIGN(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, offsetToMaterialConstants + sizeof(UINT));

		std::vector<byte> pHitShaderTable(shaderRecordSizeInBytes * m_Model.m_Header.meshCount);

		auto GetShaderTable = [=](const Model& model, ID3D12StateObject* pPSO, byte* pShaderTable)
		{
			ID3D12StateObjectProperties* stateObjectProperties = nullptr;
			ThrowIfFailed(pPSO->QueryInterface(IID_PPV_ARGS(&stateObjectProperties)));
			void* pHitGroupIdentifierData = stateObjectProperties->GetShaderIdentifier(AO_HIT_GROUP_NAME);

			for (UINT i = 0; i < model.m_Header.meshCount; i++)
			{
				// First shader record: this identifies the hit group for which this table entry is.
				byte* pShaderRecord = i * shaderRecordSizeInBytes + pShaderTable;
				memcpy(pShaderRecord, pHitGroupIdentifierData, shaderIdentifierSize);

				// Second shader record: mesh id, so that we can index into our vertex buffer correctly.
				MeshRootConstant meshConst;
				meshConst.meshId = i;
				memcpy(pShaderRecord + offsetToMaterialConstants, &meshConst, sizeof(meshConst));
			}
		};

		GetShaderTable(m_Model, m_AORaytracingPso.Get(), pHitShaderTable.data());
		m_AORaytracingInput = RaytracingDispatchRayInputs(m_AORaytracingPso.Get(), pHitShaderTable.data(),
			shaderRecordSizeInBytes, (UINT)pHitShaderTable.size(), AO_RAY_GEN_EXPORT_NAME, AO_MISS_EXPORT_NAME);
	}

	// Binned diffuse shader table
	{
		const UINT shaderIdentifierSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		const UINT offsetToDescriptorHandle = ALIGN(sizeof(D3D12_GPU_DESCRIPTOR_HANDLE), shaderIdentifierSize);
		const UINT offsetToMaterialConstants = ALIGN(sizeof(UINT32), offsetToDescriptorHandle + sizeof(D3D12_GPU_DESCRIPTOR_HANDLE));
		const UINT shaderRecordSizeInBytes = ALIGN(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, offsetToMaterialConstants + sizeof(UINT32));

		std::vector<byte> pHitShaderTable(shaderRecordSizeInBytes * m_Model.m_Header.meshCount);

		auto GetShaderTable = [=](const Model& model, ID3D12StateObject* pPSO, byte* pShaderTable)
		{
			ID3D12StateObjectProperties* stateObjectProperties = nullptr;
			ThrowIfFailed(pPSO->QueryInterface(IID_PPV_ARGS(&stateObjectProperties)));
			void* pHitGroupIdentifierData = stateObjectProperties->GetShaderIdentifier(BINNED_DIFFUSE_HIT_GROUP_NAME);

			for (UINT i = 0; i < model.m_Header.meshCount; i++)
			{
				// First shader record: this identifies the hit group for which this table entry is.
				byte* pShaderRecord = i * shaderRecordSizeInBytes + pShaderTable;
				memcpy(pShaderRecord, pHitGroupIdentifierData, shaderIdentifierSize);

				// Second shader record: binds the shader descriptors.
				UINT materialIndex = model.m_pMesh[i].materialIndex;
				memcpy(pShaderRecord + offsetToDescriptorHandle, &m_ModelDescriptors[materialIndex].ptr, sizeof(m_ModelDescriptors[materialIndex].ptr));

				// Third shader record: mesh id, so that we can index into our vertex buffer correctly.
				MeshRootConstant meshConst;
				meshConst.meshId = i;
				memcpy(pShaderRecord + offsetToMaterialConstants, &meshConst, sizeof(meshConst));
			}
		};

		GetShaderTable(m_Model, m_BinnedDiffusePSO.Get(), pHitShaderTable.data());
		m_BinnedDiffuseRaytracingInput = RaytracingDispatchRayInputs(m_BinnedDiffusePSO.Get(), pHitShaderTable.data(), shaderRecordSizeInBytes, (UINT)pHitShaderTable.size(), BINNED_DIFFUSE_RAY_GEN_EXPORT_NAME, DIFFUSE_MISS_EXPORT_NAME);
	}
}

D3D12_STATE_SUBOBJECT RaytracingTest::CreateDxilLibrary(LPCWSTR entrypoint, const void* pShaderBytecode,
	SIZE_T bytecodeLength, D3D12_DXIL_LIBRARY_DESC& dxilLibDesc, D3D12_EXPORT_DESC& exportDesc)
{
	exportDesc = { entrypoint, nullptr, D3D12_EXPORT_FLAG_NONE };
	D3D12_STATE_SUBOBJECT dxilLibSubObject = {};
	dxilLibDesc.DXILLibrary.pShaderBytecode = pShaderBytecode;
	dxilLibDesc.DXILLibrary.BytecodeLength = bytecodeLength;
	dxilLibDesc.NumExports = 1;
	dxilLibDesc.pExports = &exportDesc;
	dxilLibSubObject.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
	dxilLibSubObject.pDesc = &dxilLibDesc;
	return dxilLibSubObject;
}

void RaytracingTest::RenderModel(GraphicsContext& gfxContext)
{
	struct VSConstants
	{
		Matrix4 vpMatrix;
		Matrix4 worldMatrix;
	} vsConstants;

	vsConstants.vpMatrix = m_Camera.GetViewProjMatrix();
	const XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	const XMVECTOR rot = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	const XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	vsConstants.worldMatrix = Matrix4(XMMatrixTransformation(pos, rot, scale, pos, rot, pos));

	gfxContext.SetDynamicConstantBufferView(0, sizeof(vsConstants), &vsConstants);

	uint32_t materialIdx = 0xFFFFFFFFul;

	uint32_t VertexStride = m_Model.m_VertexStride;

	for (uint32_t meshIndex = 0; meshIndex < m_Model.m_Header.meshCount; meshIndex++)
	{
		const Model::Mesh& mesh = m_Model.m_pMesh[meshIndex];

		uint32_t indexCount = mesh.indexCount;
		uint32_t startIndex = mesh.indexDataByteOffset / sizeof(uint16_t);
		uint32_t baseVertex = mesh.vertexDataByteOffset / VertexStride;

		if (mesh.materialIndex != materialIdx)
		{
			materialIdx = mesh.materialIndex;
			gfxContext.SetDynamicDescriptors(1, 0, 6, m_Model.GetSRVs(materialIdx));
		}

		gfxContext.DrawIndexed(indexCount, startIndex, baseVertex);
	}
}
