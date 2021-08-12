#include "pch.h"
#include "DispatchRaysInput.h"


DispatchRaysInput::DispatchRaysInput(
	ID3D12StateObject* pPSO,
	void* pHitGroupShaderTable,
	UINT hitGroupStride,
	UINT hitGroupTableSize,
	LPCWSTR rayGenExportName,
	LPCWSTR missExportName)
	: m_HitGroupStride(hitGroupStride), m_pPSO(pPSO)
{
	const UINT shaderTableSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	ID3D12StateObjectProperties* stateObjectProperties = nullptr;
	ASSERT_SUCCEEDED(pPSO->QueryInterface(IID_PPV_ARGS(&stateObjectProperties)));
	void* pRayGenShaderData = stateObjectProperties->GetShaderIdentifier(rayGenExportName);
	void* pMissShaderData = stateObjectProperties->GetShaderIdentifier(missExportName);

	// MiniEngine requires initial data to be aligned to 16 bytes
	const UINT alignment = 16;
	std::vector<BYTE> alignedShaderTableData(shaderTableSize + alignment - 1);
	BYTE* pAlignedShaderTableData = alignedShaderTableData.data() + ((UINT64)alignedShaderTableData.data() % alignment);
	memcpy(pAlignedShaderTableData, pRayGenShaderData, shaderTableSize);
	m_RayGenShaderTable.Create(L"Ray Gen Shader Table", 1, shaderTableSize, alignedShaderTableData.data());

	memcpy(pAlignedShaderTableData, pMissShaderData, shaderTableSize);
	m_MissShaderTable.Create(L"Miss Shader Table", 1, shaderTableSize, alignedShaderTableData.data());

	m_HitShaderTable.Create(L"Hit Shader Table", 1, hitGroupTableSize, pHitGroupShaderTable);

	stateObjectProperties->Release();
}

D3D12_DISPATCH_RAYS_DESC DispatchRaysInput::GetDispatchRayDesc(UINT DispatchWidth, UINT DispatchHeight)
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

void DispatchRaysInput::Cleanup()
{
	m_pPSO.Reset();
	m_RayGenShaderTable.Destroy();
	m_MissShaderTable.Destroy();
	m_HitShaderTable.Destroy();
}
