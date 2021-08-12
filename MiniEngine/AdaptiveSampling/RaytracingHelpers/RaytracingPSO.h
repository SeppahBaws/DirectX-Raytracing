#pragma once

#include "PipelineState.h"

class RaytracingPSO : public PSO
{
public:
	RaytracingPSO();
	void Destroy();

	ID3D12StateObject* Get() const { return m_PSO.Get(); }

	void AddShader(const void* pShaderCode, SIZE_T shaderCodeSize, LPCWSTR exportName);
	void AddClosestHitGroup(LPCWSTR closestHitExportName, LPCWSTR hitGroupExportName, D3D12_HIT_GROUP_TYPE hitGroupType);

	void Configure(UINT payloadSize, UINT attributeSize, UINT maxRecursionDepth);

	void AddLocalRootSignature(const RootSignature& signature, LPCWSTR exportName);
	void SetGlobalRootSignature(const RootSignature& signature);

	HRESULT Finalize(ComPtr<ID3D12Device5> pRTDevice);

private:
	CD3DX12_STATE_OBJECT_DESC m_PSODesc;
	ComPtr<ID3D12StateObject> m_PSO;
};
