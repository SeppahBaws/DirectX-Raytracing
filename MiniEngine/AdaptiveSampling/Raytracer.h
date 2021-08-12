#pragma once
#include "ColorBuffer.h"
#include "RaytracingHelpers/DispatchRaysInput.h"
#include "RaytracingHelpers/RaytracingPSO.h"
#include "RaytracingHelpers/DescriptorHeapStack.h"

#include "Benchmark.h"

struct NoiseMapGenParams;
struct ShadowSettings;

class Raytracer
{
public:
	Raytracer() = default;

	void Initialize(ComPtr<ID3D12Device5> rtDevice, Benchmark& benchmark);
	void Cleanup();

	void Render(GraphicsContext& ctx);

	void ResetFrameCounter() { m_FrameCount = 0; }

private:
	void CreateOutputBuffers();
	void InitializeViews();
	void CreateRootSignatures();
	void CreatePipelines(ComPtr<ID3D12Device5> rtDevice);
	void CreateShaderResourceHeap();
	void CreateShaderBindingTable();

private:
	uint32_t m_FrameCount{};

	// Previous frame's viewprojection matrix for temporal reprojection
	Math::Matrix4 m_PrevFrameVP;
	Math::Vector3 m_PrevFramePos;

	std::unique_ptr<DescriptorHeapStack> m_pDescriptorHeap;

	// Main output that our raytracer will write to. This will be the final image.
	// Used as ping-pong buffers for temporal values.
	ColorBuffer m_ColorOutput;
	D3D12_GPU_DESCRIPTOR_HANDLE m_ColorOutputUAV;

	RootSignature m_GlobalRS;
	RootSignature m_RayGenRS;
	RootSignature m_MissRS;
	RootSignature m_HitGroupRS;

	RaytracingPSO m_DefaultRTPSO;

	// Position + depth pass
	RaytracingPSO m_PosRTPSO;
	RootSignature m_PosGlobalRS;
	RootSignature m_PosRayGenRS;
	RootSignature m_PosMissRS;
	RootSignature m_PosHitGroupRS;

	// RT Effects pass
	RaytracingPSO m_EffectsRTPSO;
	RootSignature m_EffectsGlobalRS;
	RootSignature m_EffectsRayGenRS;
	RootSignature m_EffectsMissRS;
	RootSignature m_EffectsHitGroupRS;

	// Noise map generation
	ComputePSO m_NoiseMapGenPSO;
	RootSignature m_NoiseMapGenRS;
	ComputePSO m_NoiseMapBlurPSO;
	RootSignature m_NoiseMapBlurRS;

	// Final combine pass
	ComputePSO m_CombinePSO;
	RootSignature m_CombineRS;

	ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap;

	ByteAddressBuffer m_DynamicConstantBuffer;
	ByteAddressBuffer m_ShadowSettingsBuffer;
	ByteAddressBuffer m_NoiseMapGenParamsBuffer;
	ByteAddressBuffer m_FinalCombineParamsBuffer;

	DispatchRaysInput m_DefaultDispatchInput;
	DispatchRaysInput m_PositionDispatchInput;
	DispatchRaysInput m_EffectsDispatchInput;

	// Raytracing resources

	// Store the surface normals, plus the depth
	ColorBuffer m_NormalDepthOutput;
	D3D12_GPU_DESCRIPTOR_HANDLE m_NormalDepthOutputUAV;
	// The buffer where base diffuse colors will be stored.
	ColorBuffer m_DiffuseOutput;
	D3D12_GPU_DESCRIPTOR_HANDLE m_DiffuseOutputUAV;

	// Used to store the output of the RT Effects pass
	// R: averaged raytracing output
	// G: counter for how many frames this pixel's average is
	// B: depth
	ColorBuffer m_RTEffectsOutput{ Color(1.0f, 0.0f, 0.0f) };
	D3D12_GPU_DESCRIPTOR_HANDLE m_RTEffectsOutputUAV;
	ColorBuffer m_RTEffectsPrevious; // Previous frame 
	D3D12_GPU_DESCRIPTOR_HANDLE m_RTEffectsPreviousUAV;

	// Our adaptive sampling output buffer
	ColorBuffer m_NoiseMapOutput{ Color(1.0f, 0.0f, 0.0f) };
	D3D12_GPU_DESCRIPTOR_HANDLE m_NoiseMapOutputUAV;
};
