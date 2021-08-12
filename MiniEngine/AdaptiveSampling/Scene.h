#pragma once
#include "Camera.h"
#include "CameraController.h"
#include "Model.h"

#include "RaytracingHelpers/DescriptorHeapStack.h"

#include "Benchmark.h"

struct MeshRootConstant
{
	UINT meshId;
};

class Scene
{
public:
	Scene();
	~Scene();

	void SetupBenchmark(Benchmark& benchmark);

	void Load(const std::string& fileName);
	void Build(ComPtr<ID3D12Device5> pRTDevice);

	void Update(float deltaT);
	void Render(GraphicsContext& gfxContext);

	const Camera& GetCamera() const { return m_Camera; }

private:
	Math::Matrix4 GetModelMatrix() const;

private:
	friend class Raytracer;

	Model m_Model;
	// Stuff to access in the raytracer for the shaders.
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_ModelDescriptors;
	D3D12_CPU_DESCRIPTOR_HANDLE m_ModelMeshInfo{};
	D3D12_CPU_DESCRIPTOR_HANDLE m_ModelIndices{};
	D3D12_GPU_DESCRIPTOR_HANDLE m_ModelSRVs{};
	StructuredBuffer m_HitShaderMeshInfo;

	Camera m_Camera;
	std::auto_ptr<GameCore::CameraController> m_CameraController;

	// Acceleration structure data
	DescriptorHeapStack* m_pDescriptorHeap{};
	ByteAddressBuffer m_ScratchBuffer;
	ByteAddressBuffer m_InstanceDataBuffer;
	ComPtr<ID3D12Resource> m_TLAS;
	std::vector<ComPtr<ID3D12Resource>> m_BLASes;
};
