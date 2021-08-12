#include "pch.h"
#include "Scene.h"

#include "Model.h"

#include "RaytracingHelpers/AccelerationStructureBuilder.h"
#include "RaytracingHelpers/DescriptorHeapStack.h"
#include "Shaders/Util/RaytracingHlslCompat.h"

#include <iostream>

using namespace Graphics;
using namespace Math;

Scene::Scene()
{
	m_Camera.SetEyeAtUp(Vector3(10.0f, 10.0f, 10.0f), Vector3(kZero), Vector3(kYUnitVector));
	m_Camera.SetZRange(1.0f, 10000.0f);
	m_CameraController.reset(new GameCore::CameraController(m_Camera, Vector3(kYUnitVector)));
}

Scene::~Scene()
{
	m_Model.Clear();

	m_ScratchBuffer.Destroy();
	m_InstanceDataBuffer.Destroy();
	m_TLAS.Reset();
	for (size_t i = 0; i < m_BLASes.size(); i++)
	{
		m_BLASes[i].Reset();
	}
	m_BLASes.clear();

	m_HitShaderMeshInfo.Destroy();

	delete m_pDescriptorHeap;
}

void Scene::SetupBenchmark(Benchmark& benchmark)
{
	benchmark.RegisterCallback([&](const BenchConfig& config)
		{
			m_Camera.SetPosition(config.cameraPos);
			m_CameraController->SetCurrentHeading(config.cameraYaw);
			m_CameraController->SetCurrentPitch(config.cameraPitch);
		});

	benchmark.RegisterDebug([&]()
		{
			const Vector3 p = m_Camera.GetPosition();

			const float yaw = m_CameraController->GetCurrentHeading();
			const float pitch = m_CameraController->GetCurrentPitch();

			std::cout << "Position: " << p.GetX() << ", " << p.GetY() << ", " << p.GetZ() << std::endl;
			std::cout << "Pitch: " << pitch << std::endl;
			std::cout << "Yaw: " << yaw << std::endl;
		});
}

void Scene::Load(const std::string& fileName)
{
	ASSERT(m_Model.Load(fileName.c_str()), "Failed to load model!");
	ASSERT(m_Model.m_Header.meshCount > 0, "Model has no mesh!");

	m_pDescriptorHeap = new DescriptorHeapStack(*g_Device, 200, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 0);

	m_ModelDescriptors = std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>(m_Model.m_Header.materialCount);

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
}

void Scene::Build(ComPtr<ID3D12Device5> pRTDevice)
{
	// Call our acceleration structure builder.
	AccelerationStructureBuilder::Build(pRTDevice, &m_Model, m_pDescriptorHeap, m_ScratchBuffer, m_InstanceDataBuffer,
		m_TLAS, m_BLASes);
}

void Scene::Update(float deltaT)
{
	m_CameraController->Update(deltaT);
}

void Scene::Render(GraphicsContext& gfxContext)
{
	struct VSConstants
	{
		Matrix4 viewProjection;
		Matrix4 model;
	} vsConstants;

	vsConstants.viewProjection = m_Camera.GetViewProjMatrix();
	vsConstants.model = GetModelMatrix();

	gfxContext.SetDynamicConstantBufferView(0, sizeof(vsConstants), &vsConstants);

	// Prepare states
	// gfxContext.SetConstants(4, baseVertex, materialIdx);
	gfxContext.SetIndexBuffer(m_Model.m_IndexBuffer.IndexBufferView());
	gfxContext.SetVertexBuffer(0, m_Model.m_VertexBuffer.VertexBufferView());

	uint32_t materialIdx = 0xFFFFFFFFul;
	const uint32_t vertexStride = m_Model.m_VertexStride;

	for (uint32_t meshIdx = 0; meshIdx < m_Model.m_Header.meshCount; meshIdx++)
	{
		const Model::Mesh& mesh = m_Model.m_pMesh[meshIdx];

		const uint32_t indexCount = mesh.indexCount;
		const uint32_t startIndex = mesh.indexDataByteOffset / sizeof(uint16_t);
		const uint32_t baseVertex = mesh.vertexDataByteOffset / vertexStride;

		if (mesh.materialIndex != materialIdx)
		{
			materialIdx = mesh.materialIndex;
			gfxContext.SetDynamicDescriptors(1, 0, 6, m_Model.GetSRVs(materialIdx));
		}

		// Invoke draw call
		gfxContext.DrawIndexed(indexCount, startIndex, baseVertex);
	}
}

Matrix4 Scene::GetModelMatrix() const
{
	const XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	const XMVECTOR rot = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	const XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	return Matrix4(XMMatrixTransformation(pos, rot, scale, pos, rot, pos));
}
