#pragma once
#include "Raytracer.h"
#include "Scene.h"
#include "Benchmark.h"

class AdaptiveSampling : public GameCore::IGameApp
{
public:
	AdaptiveSampling();

	virtual ~AdaptiveSampling() = default;

	virtual void Startup(void) override;
	virtual void Cleanup(void) override;

	virtual void Update(float deltaT) override;
	virtual void RenderScene(void) override;

	static AdaptiveSampling* Get() { return s_Instance; }

	Scene& GetScene() { return m_Scene; }

private:
	static AdaptiveSampling* s_Instance;

	Raytracer m_Raytracer;

	Benchmark m_Benchmark;
	Scene m_Scene;

	Matrix4 m_ViewProjLastFrame;

	RootSignature m_RootSig;
	GraphicsPSO m_DepthPSO;
	GraphicsPSO m_ModelPSO;

	D3D12_VIEWPORT m_MainViewport;
	D3D12_RECT m_MainScissor;

	ComPtr<ID3D12Device5> m_pRTDevice;
};
