#include "pch.h"
#include "AdaptiveSampling.h"

#include "GameCore.h"
#include "GraphicsCore.h"
#include "CommandContext.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "BufferManager.h"
#include "Model.h"
#include "GameInput.h"
#include "Scene.h"

#include <CompiledShaders/DepthViewerVS.h>
#include <CompiledShaders/DepthViewerPS.h>
#include <CompiledShaders/ModelVS.h>
#include <CompiledShaders/ModelPS.h>

using namespace GameCore;
using namespace Graphics;

// MAIN FUNCTION
CREATE_APPLICATION(AdaptiveSampling)

AdaptiveSampling* AdaptiveSampling::s_Instance = nullptr;

const char* raytracingModes[] =
{
	"Off",
	"Full Raytrace"
};
enum RaytracingMode
{
	RTM_Off = 0,
	RTM_FullRaytrace,
};
EnumVar raytracingMode("Application/Raytracing Mode", RTM_Off, _countof(raytracingModes), raytracingModes);

AdaptiveSampling::AdaptiveSampling()
{
	if (s_Instance)
		throw std::exception("Application instance already exists!");
	s_Instance = this;

	// Engine settings.
	TargetResolution = k900p;
	g_DisplayWidth = 1600;
	g_DisplayHeight = 900;
}

void AdaptiveSampling::Startup(void)
{
	// Query a raytracing-compatible device from the default device.
	ASSERT_SUCCEEDED(g_Device->QueryInterface(IID_PPV_ARGS(&m_pRTDevice)), "DXR device is not supported on this system!");

	const SamplerDesc sampler;

	m_RootSig.Reset(2, 1);
	m_RootSig.InitStaticSampler(0, sampler, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSig[0].InitAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
	m_RootSig[1].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 6, D3D12_SHADER_VISIBILITY_PIXEL);
	m_RootSig.Finalize(L"RaytracingTest", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	DXGI_FORMAT colorFormat = g_SceneColorBuffer.GetFormat();
	DXGI_FORMAT depthFormat = g_SceneDepthBuffer.GetFormat();

	// Depth state
	m_DepthPSO.SetRootSignature(m_RootSig);
	m_DepthPSO.SetRasterizerState(RasterizerDefault);
	m_DepthPSO.SetBlendState(BlendNoColorWrite);
	m_DepthPSO.SetDepthStencilState(DepthStateReadWrite);
	m_DepthPSO.SetInputLayout(_countof(inputElementDescs), inputElementDescs);
	m_DepthPSO.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_DepthPSO.SetRenderTargetFormats(0, nullptr, depthFormat);
	m_DepthPSO.SetVertexShader(g_pDepthViewerVS, sizeof(g_pDepthViewerVS));
	m_DepthPSO.SetPixelShader(g_pDepthViewerPS, sizeof(g_pDepthViewerPS));
	m_DepthPSO.Finalize();

	// Model state
	m_ModelPSO = m_DepthPSO;
	m_ModelPSO.SetBlendState(BlendDisable);
	m_ModelPSO.SetDepthStencilState(DepthStateTestEqual);
	m_ModelPSO.SetRenderTargetFormats(1, &colorFormat, depthFormat);
	m_ModelPSO.SetVertexShader(g_pModelVS, sizeof(g_pModelVS));
	m_ModelPSO.SetPixelShader(g_pModelPS, sizeof(g_pModelPS));
	m_ModelPSO.Finalize();

	m_Benchmark.Load("Benchmarks/benchmark_san-miguel.json");
	m_Benchmark.RegisterCallback([&](const BenchConfig& /*config*/)
		{
			m_Raytracer.ResetFrameCounter();
		});
	m_Scene.SetupBenchmark(m_Benchmark);

	TextureManager::Initialize(L"Textures/");
	m_Scene.Load(m_Benchmark.GetSceneFile());

	m_Scene.Build(m_pRTDevice);

	m_Raytracer.Initialize(m_pRTDevice, m_Benchmark);

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
}

void AdaptiveSampling::Cleanup(void)
{
	m_Raytracer.Cleanup();

	m_pRTDevice.Reset();
}

void AdaptiveSampling::Update(float deltaT)
{
	ScopedTimer _prof(L"Update State");

	m_Scene.Update(deltaT);

	// Switching views
	if (GameInput::IsFirstPressed(GameInput::kKey_numpad1))
		m_Benchmark.SelectConfig(0);
	else if (GameInput::IsFirstPressed(GameInput::kKey_numpad2))
		m_Benchmark.SelectConfig(1);
	else if (GameInput::IsFirstPressed(GameInput::kKey_numpad3))
		m_Benchmark.SelectConfig(2);
	else if (GameInput::IsFirstPressed(GameInput::kKey_numpad4))
		m_Benchmark.SelectConfig(3);

	if (GameInput::IsFirstPressed(GameInput::kKey_9))
		m_Benchmark.NextConfig();
	if (GameInput::IsFirstPressed(GameInput::kKey_0))
		m_Benchmark.OutputDebug();

	if (GameInput::IsFirstPressed(GameInput::kKey_1))
		raytracingMode = RTM_Off;
	else if (GameInput::IsFirstPressed(GameInput::kKey_2))
	{
		raytracingMode = RTM_FullRaytrace;
		m_Raytracer.ResetFrameCounter();
	}

	const Matrix4 viewProj = m_Scene.GetCamera().GetViewProjMatrix();
	if (viewProj == m_ViewProjLastFrame)
	{
		m_Raytracer.ResetFrameCounter();
	}
	m_ViewProjLastFrame = viewProj;

	m_Benchmark.Update(deltaT);
}

void AdaptiveSampling::RenderScene(void)
{
	GraphicsContext& gfxContext = GraphicsContext::Begin(L"Scene Render");

	auto pfnSetupGraphicsState = [&](void)
	{
		gfxContext.SetRootSignature(m_RootSig);
		gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	};

	if (raytracingMode == RTM_Off)
	{
		// Z pre-pass
		{
			ScopedTimer _prof(L"Z PrePass", gfxContext);

			pfnSetupGraphicsState();

			gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
			gfxContext.ClearDepth(g_SceneDepthBuffer);

			gfxContext.SetPipelineState(m_DepthPSO);

			gfxContext.SetDepthStencilTarget(g_SceneDepthBuffer.GetDSV());
			gfxContext.SetViewportAndScissor(m_MainViewport, m_MainScissor);

			m_Scene.Render(gfxContext);
		}

		// Raster Color Pass
		{
			ScopedTimer _prof(L"Render Color", gfxContext);

			gfxContext.TransitionResource(g_SceneColorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);
			gfxContext.ClearColor(g_SceneColorBuffer);

			pfnSetupGraphicsState();

			gfxContext.SetPipelineState(m_ModelPSO);

			gfxContext.TransitionResource(g_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_READ);
			gfxContext.SetRenderTarget(g_SceneColorBuffer.GetRTV(), g_SceneDepthBuffer.GetDSV_DepthReadOnly());
			gfxContext.SetViewportAndScissor(0, 0, g_SceneColorBuffer.GetWidth(), g_SceneColorBuffer.GetHeight());

			m_Scene.Render(gfxContext);
		}
	}
	else
	{
		m_Raytracer.Render(gfxContext);
	}

	gfxContext.Finish();
}
