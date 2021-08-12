#include "pch.h"
#include "Raytracer.h"

#include "BufferManager.h"

#include "RaytracingHelpers/DescriptorHeapStack.h"

#include <CompiledShaders/DefaultRayGen.h>
#include <CompiledShaders/DefaultClosestHit.h>
#include <CompiledShaders/DefaultMiss.h>

#include <CompiledShaders/PosDepthRayGen.h>
#include <CompiledShaders/PosDepthClosestHit.h>
#include <CompiledShaders/PosDepthMiss.h>

#include <CompiledShaders/RTEffectsRayGen.h>
#include <CompiledShaders/RTEffectsClosestHit.h>
#include <CompiledShaders/RTEffectsMiss.h>

#include <CompiledShaders/NoiseMapGen.h>
#include <CompiledShaders/NoiseMapBlur.h>

#include <CompiledShaders/FinalCombine.h>

#include "AdaptiveSampling.h"
#include "Shaders/Util/ShaderStructs.h"
#include "Shaders/Raytracing/NoiseMapGen/NoiseMapCommon.h"
#include "Shaders/Raytracing/FinalCombine/FinalCombineCommon.h"

using namespace Graphics;

BoolVar debugNoiseMap("Application/Debug/Show Noise Map", false);
BoolVar debugRTEffects("Application/Debug/Show RT Effects", false);

NumVar shadowSphereOffset("Application/Shadows/Sphere Offset", 10.0f, 0.0f, 10.0f, 0.1f);
NumVar shadowSphereSize("Application/Shadows/Sphere Size", 0.3f, 0.0f, 10.0f, 0.1f);
NumVar shadowDirX("Application/Shadows/Direction X", 2.0f, -100.0f, 100.0f, 0.2f);
NumVar shadowDirY("Application/Shadows/Direction Y", 3.0f, -100.0f, 100.0f, 0.2f);
NumVar shadowDirZ("Application/Shadows/Direction Z", 1.0f, -100.0f, 100.0f, 0.2f);

IntVar noiseAvgSize("Application/NoiseMap Gen/Average Size", 3, 1);
NumVar noiseRTolerance("Application/NoiseMap Gen/R tolerance", 0.5f, 0.0f, 1.0f, 0.05f);
NumVar noiseGTolerance("Application/NoiseMap Gen/G tolerance", 0.2f, 0.0f, 1.0f, 0.05f);
NumVar noiseEdgeTolerance("Application/NoiseMap Gen/Edge tolerance", 0.2f, 0.0f, 10.0f, 0.05f);
NumVar noiseDepthTolerance("Application/NoiseMap Gen/Depth tolerance", 0.3f, 0.0f, FLT_MAX, 0.05f);
NumVar noiseNormalTolrance("Application/NoiseMap Gen/Normal tolerance", 0.95f, -1.0f, 1.0f, 0.05f);

BoolVar doSecondaryRTPasses("Application/Adaptive Sampling/Do secondary RT passes", true);
BoolVar useNoisemap("Application/Adaptive Sampling/Use Noisemap", true);
IntVar minimumPasses("Application/Adaptive Sampling/Minimum Passes", 4, 1, 2048);
IntVar maximumPasses("Application/Adaptive Sampling/Maximum Passes", 16, 1, 2048);
NumVar varianceToleration("Application/Adaptive Sampling/Accepted Variance", 0.001f, 0.001f, 1.0f, 0.001f);

void Raytracer::Initialize(ComPtr<ID3D12Device5> rtDevice, Benchmark& benchmark)
{
	benchmark.RegisterCallback([](const BenchConfig& config)
		{
			shadowSphereOffset = config.shadowSphereOffset;
			shadowSphereSize = config.shadowSphereSize;
			shadowDirX = config.shadowDir.GetX();
			shadowDirY = config.shadowDir.GetY();
			shadowDirZ = config.shadowDir.GetZ();
		});

	m_PrevFrameVP = AdaptiveSampling::Get()->GetScene().GetCamera().GetViewProjMatrix();
	m_PrevFramePos = AdaptiveSampling::Get()->GetScene().GetCamera().GetPosition();

	m_pDescriptorHeap = std::make_unique<DescriptorHeapStack>(
		*g_Device, 1000, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 0);

	CreateOutputBuffers();
	InitializeViews();
	CreateRootSignatures();
	CreatePipelines(rtDevice);
	CreateShaderResourceHeap();
	CreateShaderBindingTable();
}

void Raytracer::Cleanup()
{
	m_pDescriptorHeap.reset();

	m_ColorOutput.Destroy();

	m_DescriptorHeap.Reset();
	m_DynamicConstantBuffer.Destroy();
	m_ShadowSettingsBuffer.Destroy();
	m_NoiseMapGenParamsBuffer.Destroy();
	m_FinalCombineParamsBuffer.Destroy();

	m_DefaultRTPSO.Destroy();

	m_GlobalRS.DestroyAll();
	m_RayGenRS.DestroyAll();
	m_MissRS.DestroyAll();
	m_HitGroupRS.DestroyAll();

	m_DefaultDispatchInput.Cleanup();
	m_PositionDispatchInput.Cleanup();
	m_EffectsDispatchInput.Cleanup();

	m_PosRTPSO.Destroy();
	m_PosGlobalRS.DestroyAll();
	m_PosRayGenRS.DestroyAll();
	m_PosMissRS.DestroyAll();
	m_PosHitGroupRS.DestroyAll();

	m_EffectsRTPSO.Destroy();
	m_EffectsGlobalRS.DestroyAll();
	m_EffectsRayGenRS.DestroyAll();
	m_EffectsMissRS.DestroyAll();
	m_EffectsHitGroupRS.DestroyAll();

	m_NoiseMapGenPSO.DestroyAll();
	m_NoiseMapGenRS.DestroyAll();
	m_NoiseMapBlurPSO.DestroyAll();
	m_NoiseMapBlurRS.DestroyAll();

	m_CombinePSO.DestroyAll();
	m_CombineRS.DestroyAll();

	m_NormalDepthOutput.Destroy();
	m_DiffuseOutput.Destroy();
	m_RTEffectsOutput.Destroy();
	m_RTEffectsPrevious.Destroy();
	m_NoiseMapOutput.Destroy();
}

void Raytracer::Render(GraphicsContext& ctx)
{
	auto copyOutputToBackBuffer = [&](ColorBuffer& colorBuffer)
	{
		// Transition resources
		ctx.TransitionResource(colorBuffer, D3D12_RESOURCE_STATE_COPY_SOURCE);
		ctx.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_COPY_DEST);
		ctx.FlushResourceBarriers();

		// Copy command
		ctx.CopyBuffer(g_SceneColorBuffer, colorBuffer);

		ctx.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
	};

	ShadowSettings shadowSettings = {
			shadowSphereOffset,
			shadowSphereSize,
			shadowDirX,
			shadowDirY,
			shadowDirZ
	};
	
	NoiseMapGenParams noiseMapGenParams = {
		noiseAvgSize,
		noiseRTolerance,
		noiseGTolerance,
		noiseEdgeTolerance,
		noiseDepthTolerance,
		noiseNormalTolrance,
		float2{
			static_cast<float>(g_SceneColorBuffer.GetWidth()),
			static_cast<float>(g_SceneColorBuffer.GetHeight())
		}
	};

	ComPtr<ID3D12GraphicsCommandList4> pRTCommandList;
	ASSERT_SUCCEEDED(ctx.GetCommandList()->QueryInterface(IID_PPV_ARGS(&pRTCommandList)));


	// // Clear our output buffers.
	{
		ctx.TransitionResource(m_ColorOutput, D3D12_RESOURCE_STATE_RENDER_TARGET);
		ctx.TransitionResource(m_NoiseMapOutput, D3D12_RESOURCE_STATE_RENDER_TARGET);
		ctx.TransitionResource(m_RTEffectsOutput, D3D12_RESOURCE_STATE_RENDER_TARGET);
		ctx.FlushResourceBarriers();

		ctx.ClearColor(m_RTEffectsOutput);
		ctx.ClearColor(m_NoiseMapOutput);
		ctx.ClearColor(m_ColorOutput);
	}

	// Position + direction data pass
	{
		ScopedTimer _prof(L"Position + direction raytrace Pass", ctx);

		ID3D12DescriptorHeap* pDescHeaps[] = { &m_pDescriptorHeap->GetDescriptorHeap() };
		pRTCommandList->SetDescriptorHeaps(ARRAYSIZE(pDescHeaps), pDescHeaps);
		ctx.SetRenderTarget(m_NormalDepthOutput.GetRTV());

		Camera cam = AdaptiveSampling::Get()->GetScene().m_Camera;
		DynamicCB input;
		ZeroMemory(&input, sizeof(DynamicCB));
		Matrix4 viewI = Invert(cam.GetViewMatrix());
		Matrix4 projI = Invert(cam.GetProjMatrix());
		Matrix4 prevViewProj = m_PrevFrameVP;
		memcpy(&input.viewI, &viewI, sizeof(input.viewI));
		memcpy(&input.projectionI, &projI, sizeof(input.projectionI));
		memcpy(&input.previousViewProj, &prevViewProj, sizeof(input.previousViewProj));
		input.previousCamPos = float4{ m_PrevFramePos.GetX(), m_PrevFramePos.GetY(), m_PrevFramePos.GetZ() };
		input.currentCamPos = float4{ cam.GetPosition().GetX(), cam.GetPosition().GetY(), cam.GetPosition().GetZ() };
		input.frameCount = static_cast<uint>(Graphics::GetFrameCount());
		input.screenSize = float2{
			static_cast<float>(g_SceneColorBuffer.GetWidth()),
			static_cast<float>(g_SceneColorBuffer.GetHeight())
		};
		ctx.WriteBuffer(m_DynamicConstantBuffer, 0, &input, sizeof(input));

		ctx.WriteBuffer(m_ShadowSettingsBuffer, 0, &shadowSettings, sizeof(shadowSettings));

		ctx.TransitionResource(m_DynamicConstantBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		ctx.TransitionResource(m_ShadowSettingsBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		ctx.FlushResourceBarriers();

		// Bind raytracing params
		pRTCommandList->SetComputeRootSignature(m_PosGlobalRS.GetSignature());
		pRTCommandList->SetComputeRootDescriptorTable(0, m_NormalDepthOutputUAV);
		pRTCommandList->SetComputeRootDescriptorTable(1, m_DiffuseOutputUAV);
		pRTCommandList->SetComputeRootShaderResourceView(2, AdaptiveSampling::Get()->GetScene().m_TLAS->GetGPUVirtualAddress());
		pRTCommandList->SetComputeRootConstantBufferView(3, m_DynamicConstantBuffer.GetGpuVirtualAddress());
		pRTCommandList->SetComputeRootConstantBufferView(4, m_ShadowSettingsBuffer.GetGpuVirtualAddress());
		pRTCommandList->SetComputeRootDescriptorTable(5, AdaptiveSampling::Get()->GetScene().m_ModelSRVs);

		// Dispatch the rays
		D3D12_DISPATCH_RAYS_DESC dispatchDesc = m_PositionDispatchInput.GetDispatchRayDesc(g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight());
		pRTCommandList->SetPipelineState1(m_PosRTPSO.Get());
		pRTCommandList->DispatchRays(&dispatchDesc);

		copyOutputToBackBuffer(m_DiffuseOutput);
	}

	// Primary RT effects pass
	{
		ScopedTimer _prof(L"RT Effects pass", ctx);

		ID3D12DescriptorHeap* pDescHeaps[] = { &m_pDescriptorHeap->GetDescriptorHeap() };
		pRTCommandList->SetDescriptorHeaps(ARRAYSIZE(pDescHeaps), pDescHeaps);
		ctx.SetRenderTarget(m_RTEffectsOutput.GetRTV());

		Camera cam = AdaptiveSampling::Get()->GetScene().m_Camera;
		DynamicCB input;
		ZeroMemory(&input, sizeof(DynamicCB));
		Matrix4 viewI = Invert(cam.GetViewMatrix());
		Matrix4 projI = Invert(cam.GetProjMatrix());
		Matrix4 prevViewProj = m_PrevFrameVP;
		memcpy(&input.viewI, &viewI, sizeof(input.viewI));
		memcpy(&input.projectionI, &projI, sizeof(input.projectionI));
		memcpy(&input.previousViewProj, &prevViewProj, sizeof(input.previousViewProj));
		input.previousCamPos = float4{ m_PrevFramePos.GetX(), m_PrevFramePos.GetY(), m_PrevFramePos.GetZ() };
		input.currentCamPos = float4{ cam.GetPosition().GetX(), cam.GetPosition().GetY(), cam.GetPosition().GetZ() };
		input.frameCount = m_FrameCount;
		input.isFirstPass = true;
		input.screenSize = float2{
			static_cast<float>(g_SceneColorBuffer.GetWidth()),
			static_cast<float>(g_SceneColorBuffer.GetHeight())
		};
		ctx.WriteBuffer(m_DynamicConstantBuffer, 0, &input, sizeof(input));

		ctx.WriteBuffer(m_ShadowSettingsBuffer, 0, &shadowSettings, sizeof(shadowSettings));

		ctx.TransitionResource(m_DynamicConstantBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		ctx.TransitionResource(m_ShadowSettingsBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		ctx.TransitionResource(m_NoiseMapOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		ctx.FlushResourceBarriers();

		// Bind raytracing params
		pRTCommandList->SetComputeRootSignature(m_EffectsGlobalRS.GetSignature());
		pRTCommandList->SetComputeRootDescriptorTable(0, m_RTEffectsOutputUAV);
		pRTCommandList->SetComputeRootDescriptorTable(1, m_RTEffectsPreviousUAV);
		pRTCommandList->SetComputeRootDescriptorTable(2, m_NormalDepthOutputUAV);
		pRTCommandList->SetComputeRootDescriptorTable(3, m_NoiseMapOutputUAV);
		pRTCommandList->SetComputeRootShaderResourceView(4, AdaptiveSampling::Get()->GetScene().m_TLAS->GetGPUVirtualAddress());
		pRTCommandList->SetComputeRootConstantBufferView(5, m_DynamicConstantBuffer.GetGpuVirtualAddress());
		pRTCommandList->SetComputeRootConstantBufferView(6, m_ShadowSettingsBuffer.GetGpuVirtualAddress());
		pRTCommandList->SetComputeRootDescriptorTable(7, AdaptiveSampling::Get()->GetScene().m_ModelSRVs);

		// Dispatch the rays
		D3D12_DISPATCH_RAYS_DESC dispatchDesc = m_EffectsDispatchInput.GetDispatchRayDesc(g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight());
		pRTCommandList->SetPipelineState1(m_EffectsRTPSO.Get());
		pRTCommandList->DispatchRays(&dispatchDesc);
	}

	if (useNoisemap)
	{
		// Noise map generation pass
		{
			ComputeContext& compute = ctx.GetComputeContext();
			ScopedTimer _prof(L"Noise map generate pass", compute);

			ctx.WriteBuffer(m_NoiseMapGenParamsBuffer, 0, &noiseMapGenParams, sizeof(noiseMapGenParams));
			ctx.TransitionResource(m_NoiseMapGenParamsBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

			compute.TransitionResource(m_ColorOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			compute.TransitionResource(m_RTEffectsOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			compute.FlushResourceBarriers();

			compute.SetRootSignature(m_NoiseMapGenRS);
			compute.SetPipelineState(m_NoiseMapGenPSO);

			compute.SetDescriptorTable(0, m_NoiseMapOutputUAV);
			compute.SetDescriptorTable(1, m_RTEffectsOutputUAV);
			compute.SetDescriptorTable(2, m_NormalDepthOutputUAV);
			pRTCommandList->SetComputeRootConstantBufferView(3, m_NoiseMapGenParamsBuffer.GetGpuVirtualAddress());

			const int dispatchX = static_cast<int>(std::ceil(g_SceneColorBuffer.GetWidth() / static_cast<float>(GEN_DISPATCH_SIZE)));
			const int dispatchY = static_cast<int>(std::ceil(g_SceneColorBuffer.GetHeight() / static_cast<float>(GEN_DISPATCH_SIZE)));
			compute.Dispatch(dispatchX, dispatchY, 1);
		}

		// Noise Map Blur
		{
			ComputeContext& compute = ctx.GetComputeContext();
			ScopedTimer _prof(L"Noise map blur pass", compute);

			ctx.WriteBuffer(m_NoiseMapGenParamsBuffer, 0, &noiseMapGenParams, sizeof(noiseMapGenParams));
			ctx.TransitionResource(m_NoiseMapGenParamsBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

			compute.TransitionResource(m_ColorOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			compute.FlushResourceBarriers();

			compute.SetRootSignature(m_NoiseMapBlurRS);
			compute.SetPipelineState(m_NoiseMapBlurPSO);

			compute.SetDescriptorTable(0, m_NoiseMapOutputUAV);
			pRTCommandList->SetComputeRootConstantBufferView(1, m_NoiseMapGenParamsBuffer.GetGpuVirtualAddress());

			const int dispatchX = static_cast<int>(std::ceil(g_SceneColorBuffer.GetWidth() / static_cast<float>(BLUR_DISPATCH_SIZE)));
			const int dispatchY = static_cast<int>(std::ceil(g_SceneColorBuffer.GetHeight() / static_cast<float>(BLUR_DISPATCH_SIZE)));
			compute.Dispatch(dispatchX, dispatchY, 1);
		}
	}

	// Secondary RT effect passes
	if (doSecondaryRTPasses)
	{
		for (int i = 1; i <= maximumPasses; i++)
		{
			ScopedTimer _prof(L"Secondary RT Effects pass", ctx);

			ID3D12DescriptorHeap* pDescHeaps[] = { &m_pDescriptorHeap->GetDescriptorHeap() };
			pRTCommandList->SetDescriptorHeaps(ARRAYSIZE(pDescHeaps), pDescHeaps);
			ctx.SetRenderTarget(m_RTEffectsOutput.GetRTV());

			Camera cam = AdaptiveSampling::Get()->GetScene().m_Camera;
			DynamicCB input;
			ZeroMemory(&input, sizeof(DynamicCB));
			Matrix4 viewI = Invert(cam.GetViewMatrix());
			Matrix4 projI = Invert(cam.GetProjMatrix());
			Matrix4 prevViewProj = m_PrevFrameVP;
			memcpy(&input.viewI, &viewI, sizeof(input.viewI));
			memcpy(&input.projectionI, &projI, sizeof(input.projectionI));
			memcpy(&input.previousViewProj, &prevViewProj, sizeof(input.previousViewProj));
			input.previousCamPos = float4{ m_PrevFramePos.GetX(), m_PrevFramePos.GetY(), m_PrevFramePos.GetZ() };
			input.currentCamPos = float4{ cam.GetPosition().GetX(), cam.GetPosition().GetY(), cam.GetPosition().GetZ() };

			// we need to increase the frame count to re-seed our pseudo-random number generator,
			// otherwise it will just give the exact same "random" values.
			input.frameCount = m_FrameCount + i;
			input.isFirstPass = false;
			input.minimumPassCount = minimumPasses;
			input.maximumPassCount = maximumPasses;
			input.varianceToleration = varianceToleration;
			input.currentPassIdx = i;
			input.screenSize = float2{
				static_cast<float>(g_SceneColorBuffer.GetWidth()),
				static_cast<float>(g_SceneColorBuffer.GetHeight())
			};
			ctx.WriteBuffer(m_DynamicConstantBuffer, 0, &input, sizeof(input));

			ctx.WriteBuffer(m_ShadowSettingsBuffer, 0, &shadowSettings, sizeof(shadowSettings));

			ctx.TransitionResource(m_DynamicConstantBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			ctx.TransitionResource(m_ShadowSettingsBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			ctx.FlushResourceBarriers();

			// Bind raytracing params
			pRTCommandList->SetComputeRootSignature(m_EffectsGlobalRS.GetSignature());
			pRTCommandList->SetComputeRootDescriptorTable(0, m_RTEffectsOutputUAV);
			pRTCommandList->SetComputeRootDescriptorTable(1, m_RTEffectsPreviousUAV);
			pRTCommandList->SetComputeRootDescriptorTable(2, m_NormalDepthOutputUAV);
			pRTCommandList->SetComputeRootDescriptorTable(3, m_NoiseMapOutputUAV);
			pRTCommandList->SetComputeRootShaderResourceView(4, AdaptiveSampling::Get()->GetScene().m_TLAS->GetGPUVirtualAddress());
			pRTCommandList->SetComputeRootConstantBufferView(5, m_DynamicConstantBuffer.GetGpuVirtualAddress());
			pRTCommandList->SetComputeRootConstantBufferView(6, m_ShadowSettingsBuffer.GetGpuVirtualAddress());
			pRTCommandList->SetComputeRootDescriptorTable(7, AdaptiveSampling::Get()->GetScene().m_ModelSRVs);

			// Dispatch the rays
			D3D12_DISPATCH_RAYS_DESC dispatchDesc = m_EffectsDispatchInput.GetDispatchRayDesc(g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight());
			pRTCommandList->SetPipelineState1(m_EffectsRTPSO.Get());
			pRTCommandList->DispatchRays(&dispatchDesc);
		}
	}

	// Store this frame's RT Effects buffer for next frame.
	{
		ctx.TransitionResource(m_RTEffectsOutput, D3D12_RESOURCE_STATE_COPY_SOURCE);
		ctx.TransitionResource(m_RTEffectsPrevious, D3D12_RESOURCE_STATE_COPY_DEST);
		ctx.FlushResourceBarriers();

		ctx.CopyBuffer(m_RTEffectsPrevious, m_RTEffectsOutput);
	}

	// Final compute combination pass
	{
		ComputeContext& compute = ctx.GetComputeContext();
		ScopedTimer _prof(L"Final combine pass", compute);

		compute.TransitionResource(m_ColorOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		compute.TransitionResource(m_DiffuseOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		compute.TransitionResource(m_RTEffectsOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		compute.TransitionResource(m_NoiseMapOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		FinalCombineParams params;
		ZeroMemory(&params, sizeof(FinalCombineParams));
		params.debugNoiseMap = debugNoiseMap;
		params.debugRTEffects = debugRTEffects;
		compute.WriteBuffer(m_FinalCombineParamsBuffer, 0, &params, sizeof(params));
		compute.TransitionResource(m_FinalCombineParamsBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		compute.FlushResourceBarriers();

		compute.SetRootSignature(m_CombineRS);
		compute.SetPipelineState(m_CombinePSO);

		compute.SetDescriptorTable(0, m_ColorOutputUAV);
		compute.SetDescriptorTable(1, m_DiffuseOutputUAV);
		compute.SetDescriptorTable(2, m_RTEffectsOutputUAV);
		compute.SetDescriptorTable(3, m_NoiseMapOutputUAV);
		pRTCommandList->SetComputeRootConstantBufferView(4, m_FinalCombineParamsBuffer.GetGpuVirtualAddress());

		const int dispatchX = static_cast<int>(std::ceil(g_SceneColorBuffer.GetWidth() / static_cast<float>(COMBINE_DISPATCH_SIZE)));
		const int dispatchY = static_cast<int>(std::ceil(g_SceneColorBuffer.GetHeight() / static_cast<float>(COMBINE_DISPATCH_SIZE)));
		compute.Dispatch(dispatchX, dispatchY, 1);
	}

	copyOutputToBackBuffer(m_ColorOutput);

	m_FrameCount++;
	m_PrevFrameVP = AdaptiveSampling::Get()->GetScene().GetCamera().GetViewProjMatrix();
	m_PrevFramePos = AdaptiveSampling::Get()->GetScene().GetCamera().GetPosition();
}

void Raytracer::CreateOutputBuffers()
{
	m_ColorOutput.Create(L"Raytracing output color buffer", g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight(), 1, g_SceneColorBuffer.GetFormat());

	m_NormalDepthOutput.Create(L"Normals + Depth output buffer", g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight(), 1, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_DiffuseOutput.Create(L"Diffuse colors output buffer", g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight(), 1, g_SceneColorBuffer.GetFormat());

	m_RTEffectsOutput.Create(L"RT Effects output buffer", g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight(), 1, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_RTEffectsPrevious.Create(L"RT Effects previous frame", g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight(), 1, DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_NoiseMapOutput.Create(L"Noise Map", g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight(), 3, DXGI_FORMAT_R8_UNORM);
}

void Raytracer::InitializeViews()
{
	Scene& scene = AdaptiveSampling::Get()->GetScene();

	// UAVs
	{
		D3D12_CPU_DESCRIPTOR_HANDLE uavHandle;
		UINT uavDescriptorIndex;

		// Main raytracing color output ping-pong buffers
		m_pDescriptorHeap->AllocateDescriptor(uavHandle, uavDescriptorIndex);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_ColorOutput.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_ColorOutputUAV = m_pDescriptorHeap->GetGpuHandle(uavDescriptorIndex);


		// Normals output buffer
		m_pDescriptorHeap->AllocateDescriptor(uavHandle, uavDescriptorIndex);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_NormalDepthOutput.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_NormalDepthOutputUAV = m_pDescriptorHeap->GetGpuHandle(uavDescriptorIndex);

		// Diffuse colors output buffer
		m_pDescriptorHeap->AllocateDescriptor(uavHandle, uavDescriptorIndex);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_DiffuseOutput.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_DiffuseOutputUAV = m_pDescriptorHeap->GetGpuHandle(uavDescriptorIndex);

		// RT Effects output buffer
		m_pDescriptorHeap->AllocateDescriptor(uavHandle, uavDescriptorIndex);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_RTEffectsOutput.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_RTEffectsOutputUAV = m_pDescriptorHeap->GetGpuHandle(uavDescriptorIndex);

		// Previous frame's RT Effects output
		m_pDescriptorHeap->AllocateDescriptor(uavHandle, uavDescriptorIndex);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_RTEffectsPrevious.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_RTEffectsPreviousUAV = m_pDescriptorHeap->GetGpuHandle(uavDescriptorIndex);

		// Noise Map
		m_pDescriptorHeap->AllocateDescriptor(uavHandle, uavDescriptorIndex);
		g_Device->CopyDescriptorsSimple(1, uavHandle, m_NoiseMapOutput.GetUAV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_NoiseMapOutputUAV = m_pDescriptorHeap->GetGpuHandle(uavDescriptorIndex);
	}

	// SRVs for the model textures
	{
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
		UINT srvDescriptorIndex;

		// Mesh info
		m_pDescriptorHeap->AllocateDescriptor(srvHandle, srvDescriptorIndex);
		g_Device->CopyDescriptorsSimple(1, srvHandle, scene.m_ModelMeshInfo, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		scene.m_ModelSRVs = m_pDescriptorHeap->GetGpuHandle(srvDescriptorIndex);

		UINT unused;
		m_pDescriptorHeap->AllocateDescriptor(srvHandle, unused);
		g_Device->CopyDescriptorsSimple(1, srvHandle, scene.m_ModelIndices, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		m_pDescriptorHeap->AllocateBufferSrv(*const_cast<ID3D12Resource*>(scene.m_Model.m_VertexBuffer.GetResource()));

		for (UINT i = 0; i < scene.m_Model.m_Header.materialCount; i++)
		{
			UINT slot;
			m_pDescriptorHeap->AllocateDescriptor(srvHandle, slot);
			g_Device->CopyDescriptorsSimple(1, srvHandle, *scene.m_Model.GetSRVs(i), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			m_pDescriptorHeap->AllocateDescriptor(srvHandle, unused);
			g_Device->CopyDescriptorsSimple(1, srvHandle, scene.m_Model.GetSRVs(i)[3], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			scene.m_ModelDescriptors[i] = m_pDescriptorHeap->GetGpuHandle(slot);
		}
	}
}

void Raytracer::CreateRootSignatures()
{
	// Default RT pass
	{
		// Global raytracing root signature
		m_GlobalRS.Reset(5, 0);
		m_GlobalRS[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1); // Render Target
		m_GlobalRS[1].InitAsBufferSRV(0); // Acceleration Structure
		m_GlobalRS[2].InitAsConstantBuffer(0); // DynamicCB
		m_GlobalRS[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // Mesh info
		m_GlobalRS[4].InitAsConstantBuffer(2); // ShadowSettings
		m_GlobalRS.Finalize(L"Global RT root signature");

		// Ray generation local root signature
		m_RayGenRS.Reset(0, 0);
		m_RayGenRS.Finalize(L"Default RT ray gen local root signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);

		// Hit local root signature
		const SamplerDesc hitSampler;
		m_HitGroupRS.Reset(2, 1);
		m_HitGroupRS.InitStaticSampler(0, hitSampler);
		m_HitGroupRS[0].InitAsDescriptorTable(1);
		m_HitGroupRS[0].SetTableRange(0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 2, 0); // Current mesh textures
		const UINT sizeOfRootConstantInDWords = (sizeof(MeshRootConstant) - 1) / sizeof(DWORD) + 1;
		m_HitGroupRS[1].InitAsConstants(1, sizeOfRootConstantInDWords); // Current mesh material id
		m_HitGroupRS.Finalize(L"Default RT hit local root signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);

		// Miss local root signature
		m_MissRS.Reset(0, 0);
		m_MissRS.Finalize(L"Default RT miss local root signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}

	// Position + Depth pass
	{
		m_PosGlobalRS.Reset(6, 0);
		m_PosGlobalRS[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1); // Normals + Depth output
		m_PosGlobalRS[1].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1); // Diffuse output
		m_PosGlobalRS[2].InitAsBufferSRV(0); // Acceleration Structure
		m_PosGlobalRS[3].InitAsConstantBuffer(0); // DynamicCB
		m_PosGlobalRS[4].InitAsConstantBuffer(1); // ShadowSettings
		m_PosGlobalRS[5].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // Mesh Info
		m_PosGlobalRS.Finalize(L"Position + Depth global root signature");

		m_PosRayGenRS.Reset(0, 0);
		m_PosRayGenRS.Finalize(L"Position + Depth ray gen root signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);

		m_PosMissRS.Reset(0, 0);
		m_PosMissRS.Finalize(L"Position + Depth miss root signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);

		const SamplerDesc hitSamplerDesc;
		m_PosHitGroupRS.Reset(2, 1);
		m_PosHitGroupRS.InitStaticSampler(0, hitSamplerDesc);
		m_PosHitGroupRS[0].InitAsDescriptorTable(1);
		m_PosHitGroupRS[0].SetTableRange(0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 2, 0); // Current mesh textures
		const UINT sizeOfRootConstantInDWords = (sizeof(MeshRootConstant) - 1) / sizeof(DWORD) + 1;
		m_PosHitGroupRS[1].InitAsConstants(2, sizeOfRootConstantInDWords); // Current mesh material id
		m_PosHitGroupRS.Finalize(L"Position + Depth hit root signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}

	// RT Effects pass
	{
		m_EffectsGlobalRS.Reset(8, 0);
		m_EffectsGlobalRS[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1); // Effects output
		m_EffectsGlobalRS[1].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1); // Previous Frame
		m_EffectsGlobalRS[2].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 2, 1); // Normals + Depth as input
		m_EffectsGlobalRS[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 3, 1); // Noise map output
		m_EffectsGlobalRS[4].InitAsBufferSRV(0); // Acceleration Structure
		m_EffectsGlobalRS[5].InitAsConstantBuffer(0); // DynamicCB
		m_EffectsGlobalRS[6].InitAsConstantBuffer(1); // ShadowSettings
		m_EffectsGlobalRS[7].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // Mesh Info
		m_EffectsGlobalRS.Finalize(L"RT Effects global root signature");

		m_EffectsRayGenRS.Reset(0, 0);
		m_EffectsRayGenRS.Finalize(L"RT Effects ray gen root signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);

		m_EffectsMissRS.Reset(0, 0);
		m_EffectsMissRS.Finalize(L"RT Effects miss root signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);

		m_EffectsHitGroupRS.Reset(0, 0);
		m_EffectsHitGroupRS.Finalize(L"RT Effects hit root signature", D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE);
	}

	// Noise map
	{
		// Generation
		m_NoiseMapGenRS.Reset(4, 0);
		m_NoiseMapGenRS[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1); // Output buffer
		m_NoiseMapGenRS[1].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1); // RT effects buffer
		m_NoiseMapGenRS[2].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 2, 1); // Normals + Depth buffer
		m_NoiseMapGenRS[3].InitAsConstantBuffer(0);
		m_NoiseMapGenRS.Finalize(L"Noise map generation root signature");

		// Blur
		m_NoiseMapBlurRS.Reset(2, 0);
		m_NoiseMapBlurRS[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1); // Output buffer
		m_NoiseMapBlurRS[1].InitAsConstantBuffer(0);
		m_NoiseMapBlurRS.Finalize(L"Noise map blur root signature");
	}

	// Final combine
	{
		m_CombineRS.Reset(5, 0);
		m_CombineRS[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1); // Output buffer
		m_CombineRS[1].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1); // Diffuse g-buffer input
		m_CombineRS[2].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 2, 1); // RT Effects g-buffer input
		m_CombineRS[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 3, 1); // Noise Map input
		m_CombineRS[4].InitAsConstantBuffer(0); // Params input
		m_CombineRS.Finalize(L"Final combine root signature");
	}
}

static const LPCWSTR DEFAULT_RAY_GEN_EXPORT_NAME = L"DefaultRayGen";
static const LPCWSTR DEFAULT_CLOSEST_HIT_EXPORT_NAME = L"DefaultClosestHit";
static const LPCWSTR DEFAULT_MISS_EXPORT_NAME = L"DefaultMiss";
static const LPCWSTR DEFAULT_HIT_GROUP_NAME = L"DefaultHitGroup";

static const LPCWSTR POS_RAY_GEN_EXPORT_NAME = L"PosDepthRayGen";
static const LPCWSTR POS_CLOSEST_HIT_EXPORT_NAME = L"PosDepthClosestHit";
static const LPCWSTR POS_MISS_EXPORT_NAME = L"PosDepthMiss";
static const LPCWSTR POS_HIT_GROUP_NAME = L"PosDepthHitGroup";

static const LPCWSTR EFFECTS_RAY_GEN_EXPORT_NAME = L"RTEffectsRayGen";
static const LPCWSTR EFFECTS_CLOSEST_HIT_EXPORT_NAME = L"RTEffectsClosestHit";
static const LPCWSTR EFFECTS_MISS_EXPORT_NAME = L"RTEffectsMiss";
static const LPCWSTR EFFECTS_HIT_GROUP_NAME = L"RTEffectsHitGroup";

void Raytracer::CreatePipelines(ComPtr<ID3D12Device5> rtDevice)
{
	// Bind shaders
	m_DefaultRTPSO.AddShader(g_pDefaultRayGen, ARRAYSIZE(g_pDefaultRayGen), DEFAULT_RAY_GEN_EXPORT_NAME);
	m_DefaultRTPSO.AddShader(g_pDefaultClosestHit, ARRAYSIZE(g_pDefaultClosestHit), DEFAULT_CLOSEST_HIT_EXPORT_NAME);
	m_DefaultRTPSO.AddShader(g_pDefaultMiss, ARRAYSIZE(g_pDefaultMiss), DEFAULT_MISS_EXPORT_NAME);

	// Hit group
	m_DefaultRTPSO.AddClosestHitGroup(DEFAULT_CLOSEST_HIT_EXPORT_NAME, DEFAULT_HIT_GROUP_NAME, D3D12_HIT_GROUP_TYPE_TRIANGLES);

	// Shader + pipeline config
	const UINT payloadSize = 2 * sizeof(float); // bool skipShading + float distance (bool is 4 bytes for some reason)
	const UINT attributeSize = 2 * sizeof(float); // float2 barycentrics
	const UINT maxRecursionDepth = 2;
	m_DefaultRTPSO.Configure(payloadSize, attributeSize, maxRecursionDepth);

	// Add root signatures and associations
	m_DefaultRTPSO.AddLocalRootSignature(m_RayGenRS, DEFAULT_RAY_GEN_EXPORT_NAME);
	m_DefaultRTPSO.AddLocalRootSignature(m_MissRS, DEFAULT_MISS_EXPORT_NAME);
	m_DefaultRTPSO.AddLocalRootSignature(m_HitGroupRS, DEFAULT_HIT_GROUP_NAME);

	// Set local root signature
	m_DefaultRTPSO.SetGlobalRootSignature(m_GlobalRS);

	// Create the state object
	HRESULT hr = m_DefaultRTPSO.Finalize(rtDevice);
	if (FAILED(hr))
	{
		throw std::logic_error("Failed to create default raytracing PSO");
	}


	// Position RTPSO
	m_PosRTPSO.AddShader(g_pPosDepthRayGen, ARRAYSIZE(g_pPosDepthRayGen), POS_RAY_GEN_EXPORT_NAME);
	m_PosRTPSO.AddShader(g_pPosDepthClosestHit, ARRAYSIZE(g_pPosDepthClosestHit), POS_CLOSEST_HIT_EXPORT_NAME);
	m_PosRTPSO.AddShader(g_pPosDepthMiss, ARRAYSIZE(g_pPosDepthMiss), POS_MISS_EXPORT_NAME);
	m_PosRTPSO.AddClosestHitGroup(POS_CLOSEST_HIT_EXPORT_NAME, POS_HIT_GROUP_NAME, D3D12_HIT_GROUP_TYPE_TRIANGLES);
	m_PosRTPSO.Configure(payloadSize, attributeSize, maxRecursionDepth);
	m_PosRTPSO.AddLocalRootSignature(m_PosRayGenRS, POS_RAY_GEN_EXPORT_NAME);
	m_PosRTPSO.AddLocalRootSignature(m_PosMissRS, POS_MISS_EXPORT_NAME);
	m_PosRTPSO.AddLocalRootSignature(m_PosHitGroupRS, POS_HIT_GROUP_NAME);
	m_PosRTPSO.SetGlobalRootSignature(m_PosGlobalRS);
	hr = m_PosRTPSO.Finalize(rtDevice);
	if (FAILED(hr))
	{
		throw std::logic_error("Failed to create position raytracing PSO");
	}

	// Effects RTPSO
	m_EffectsRTPSO.AddShader(g_pRTEffectsRayGen, ARRAYSIZE(g_pRTEffectsRayGen), EFFECTS_RAY_GEN_EXPORT_NAME);
	m_EffectsRTPSO.AddShader(g_pRTEffectsClosestHit, ARRAYSIZE(g_pRTEffectsClosestHit), EFFECTS_CLOSEST_HIT_EXPORT_NAME);
	m_EffectsRTPSO.AddShader(g_pRTEffectsMiss, ARRAYSIZE(g_pRTEffectsMiss), EFFECTS_MISS_EXPORT_NAME);
	m_EffectsRTPSO.AddClosestHitGroup(EFFECTS_CLOSEST_HIT_EXPORT_NAME, EFFECTS_HIT_GROUP_NAME, D3D12_HIT_GROUP_TYPE_TRIANGLES);
	m_EffectsRTPSO.Configure(payloadSize, attributeSize, maxRecursionDepth);
	m_EffectsRTPSO.AddLocalRootSignature(m_EffectsRayGenRS, EFFECTS_RAY_GEN_EXPORT_NAME);
	m_EffectsRTPSO.AddLocalRootSignature(m_EffectsMissRS, EFFECTS_MISS_EXPORT_NAME);
	m_EffectsRTPSO.AddLocalRootSignature(m_EffectsHitGroupRS, EFFECTS_HIT_GROUP_NAME);
	m_EffectsRTPSO.SetGlobalRootSignature(m_EffectsGlobalRS);
	hr = m_EffectsRTPSO.Finalize(rtDevice);
	if (FAILED(hr))
	{
		throw std::logic_error("Failed to create effects raytracing PSO");
	}

	// Noise map generation PSO
	m_NoiseMapGenPSO.SetRootSignature(m_NoiseMapGenRS);
	m_NoiseMapGenPSO.SetComputeShader(g_pNoiseMapGen, sizeof(g_pNoiseMapGen));
	m_NoiseMapGenPSO.Finalize();

	// Noise map blur PSO
	m_NoiseMapBlurPSO.SetRootSignature(m_NoiseMapBlurRS);
	m_NoiseMapBlurPSO.SetComputeShader(g_pNoiseMapBlur, sizeof(g_pNoiseMapBlur));
	m_NoiseMapBlurPSO.Finalize();

	// Final combine PSO
	m_CombinePSO.SetRootSignature(m_CombineRS);
	m_CombinePSO.SetComputeShader(g_pFinalCombine, sizeof(g_pFinalCombine));
	m_CombinePSO.Finalize();
}

void Raytracer::CreateShaderResourceHeap()
{
	// Create SRV/UAV/CBV descriptor heap. entries:
	// - UAV for raytracing color outputs (2)
	// - UAV for normals + depth output
	// - UAV for diffuse output
	// - SRV for TLAS
	// - CBV for DynamicConstantBuffer (camera and viewport data for ray generation)
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 10;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	g_Device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_DescriptorHeap));

	// Get the handle so we can write to it on the CPU side
	D3D12_CPU_DESCRIPTOR_HANDLE descHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	// Color output UAV (ping-pong buffers)
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	g_Device->CreateUnorderedAccessView(m_ColorOutput.GetResource(), nullptr, &uavDesc, descHandle);

	// Normals + depth output UAV
	descHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	g_Device->CreateUnorderedAccessView(m_NormalDepthOutput.GetResource(), nullptr, &uavDesc, descHandle);

	// Diffuse output
	descHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	g_Device->CreateUnorderedAccessView(m_DiffuseOutput.GetResource(), nullptr, &uavDesc, descHandle);

	// Add TLAS as SRV
	descHandle.ptr += g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	Scene& scene = AdaptiveSampling::Get()->GetScene();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = scene.m_InstanceDataBuffer->GetGPUVirtualAddress();
	g_Device->CreateShaderResourceView(nullptr, &srvDesc, descHandle);

	m_DynamicConstantBuffer.Create(L"Dynamic Constant Buffer", 1, sizeof(DynamicCB));
	m_ShadowSettingsBuffer.Create(L"Shadow Settings Buffer", 1, sizeof(ShadowSettings));
	m_NoiseMapGenParamsBuffer.Create(L"NoiseMapGen Parameters Buffer", 1, sizeof(NoiseMapGenParams));
	m_FinalCombineParamsBuffer.Create(L"Final Combine Parameters Buffer", 1, sizeof(FinalCombineParams));
}

void Raytracer::CreateShaderBindingTable()
{
	// Default shader table
	{
		Scene& scene = AdaptiveSampling::Get()->GetScene();
		const Model& model = scene.m_Model;
		
		const UINT shaderIdentifierSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		const UINT offsetToDescriptorHandle = ALIGN(sizeof(D3D12_GPU_DESCRIPTOR_HANDLE), shaderIdentifierSize);
		const UINT offsetToMaterialConstants = ALIGN(sizeof(UINT32), offsetToDescriptorHandle + sizeof(D3D12_GPU_DESCRIPTOR_HANDLE));
		const UINT shaderRecordSizeInBytes = ALIGN(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, offsetToMaterialConstants + sizeof(UINT));

		std::vector<byte> pHitShaderTable(shaderRecordSizeInBytes * model.m_Header.meshCount);

		{
			ID3D12StateObjectProperties* stateObjectProperties = nullptr;
			ThrowIfFailed(m_DefaultRTPSO.Get()->QueryInterface(IID_PPV_ARGS(&stateObjectProperties)));
			void* pHitGroupIdentifierData = stateObjectProperties->GetShaderIdentifier(DEFAULT_HIT_GROUP_NAME);

			for (UINT i = 0; i < model.m_Header.meshCount; i++)
			{
				// First shader record: this identifies the hit group for which this table entry is.
				byte* pShaderRecord = i * shaderRecordSizeInBytes + pHitShaderTable.data();
				memcpy(pShaderRecord, pHitGroupIdentifierData, shaderIdentifierSize);

				// Second shader record: binds shader descriptors
				UINT materialIndex = model.m_pMesh[i].materialIndex;
				memcpy(pShaderRecord + offsetToDescriptorHandle, &scene.m_ModelDescriptors[materialIndex].ptr, sizeof(scene.m_ModelDescriptors[materialIndex].ptr));

				// Third shader record: mesh id
				MeshRootConstant meshConst;
				meshConst.meshId = i;
				memcpy(pShaderRecord + offsetToMaterialConstants, &meshConst, sizeof(meshConst));
			}
		}

		m_DefaultDispatchInput = DispatchRaysInput(m_DefaultRTPSO.Get(), pHitShaderTable.data(), shaderRecordSizeInBytes,
			static_cast<UINT>(pHitShaderTable.size()), DEFAULT_RAY_GEN_EXPORT_NAME, DEFAULT_MISS_EXPORT_NAME);
	}

	// Position + depth shader table
	{
		Scene& scene = AdaptiveSampling::Get()->GetScene();
		const Model& model = scene.m_Model;

		const UINT shaderIdentifierSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		const UINT offsetToDescriptorHandle = ALIGN(sizeof(D3D12_GPU_DESCRIPTOR_HANDLE), shaderIdentifierSize);
		const UINT offsetToMaterialConstants = ALIGN(sizeof(UINT32), offsetToDescriptorHandle + sizeof(D3D12_GPU_DESCRIPTOR_HANDLE));
		const UINT shaderRecordSizeInBytes = ALIGN(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, offsetToMaterialConstants + sizeof(UINT));

		std::vector<byte> pHitShaderTable(shaderRecordSizeInBytes * model.m_Header.meshCount);

		{
			ID3D12StateObjectProperties* stateObjectProperties = nullptr;
			ThrowIfFailed(m_PosRTPSO.Get()->QueryInterface(IID_PPV_ARGS(&stateObjectProperties)));
			void* pHitGroupIdentifierData = stateObjectProperties->GetShaderIdentifier(POS_HIT_GROUP_NAME);

			for (UINT i = 0; i < model.m_Header.meshCount; i++)
			{
				// First shader record: this identifies the hit group for which this table entry is.
				byte* pShaderRecord = i * shaderRecordSizeInBytes + pHitShaderTable.data();
				memcpy(pShaderRecord, pHitGroupIdentifierData, shaderIdentifierSize);

				// Second shader record: binds shader descriptors
				UINT materialIndex = model.m_pMesh[i].materialIndex;
				memcpy(pShaderRecord + offsetToDescriptorHandle, &scene.m_ModelDescriptors[materialIndex].ptr, sizeof(scene.m_ModelDescriptors[materialIndex].ptr));

				// Third shader record: mesh id
				MeshRootConstant meshConst;
				meshConst.meshId = i;
				memcpy(pShaderRecord + offsetToMaterialConstants, &meshConst, sizeof(meshConst));
			}
		}

		m_PositionDispatchInput = DispatchRaysInput(m_PosRTPSO.Get(), pHitShaderTable.data(), shaderRecordSizeInBytes,
			static_cast<UINT>(pHitShaderTable.size()), POS_RAY_GEN_EXPORT_NAME, POS_MISS_EXPORT_NAME);
	}

	// Effects shader table
	{
		Scene& scene = AdaptiveSampling::Get()->GetScene();
		const Model& model = scene.m_Model;

		const UINT shaderIdentifierSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		const UINT offsetToDescriptorHandle = ALIGN(sizeof(D3D12_GPU_DESCRIPTOR_HANDLE), shaderIdentifierSize);
		const UINT offsetToMaterialConstants = ALIGN(sizeof(UINT32), offsetToDescriptorHandle + sizeof(D3D12_GPU_DESCRIPTOR_HANDLE));
		const UINT shaderRecordSizeInBytes = ALIGN(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, offsetToMaterialConstants + sizeof(UINT));

		std::vector<byte> pHitShaderTable(shaderRecordSizeInBytes * model.m_Header.meshCount);

		{
			ID3D12StateObjectProperties* stateObjectProperties = nullptr;
			ThrowIfFailed(m_EffectsRTPSO.Get()->QueryInterface(IID_PPV_ARGS(&stateObjectProperties)));
			void* pHitGroupIdentifierData = stateObjectProperties->GetShaderIdentifier(EFFECTS_HIT_GROUP_NAME);

			for (UINT i = 0; i < model.m_Header.meshCount; i++)
			{
				// First shader record: this identifies the hit group for which this table entry is.
				byte* pShaderRecord = i * shaderRecordSizeInBytes + pHitShaderTable.data();
				memcpy(pShaderRecord, pHitGroupIdentifierData, shaderIdentifierSize);

				// Second shader record: binds shader descriptors
				UINT materialIndex = model.m_pMesh[i].materialIndex;
				memcpy(pShaderRecord + offsetToDescriptorHandle, &scene.m_ModelDescriptors[materialIndex].ptr, sizeof(scene.m_ModelDescriptors[materialIndex].ptr));

				// Third shader record: mesh id
				MeshRootConstant meshConst;
				meshConst.meshId = i;
				memcpy(pShaderRecord + offsetToMaterialConstants, &meshConst, sizeof(meshConst));
			}
		}

		m_EffectsDispatchInput = DispatchRaysInput(m_EffectsRTPSO.Get(), pHitShaderTable.data(), shaderRecordSizeInBytes,
			static_cast<UINT>(pHitShaderTable.size()), EFFECTS_RAY_GEN_EXPORT_NAME, EFFECTS_MISS_EXPORT_NAME);
	}
}
