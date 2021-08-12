#pragma once

struct DispatchRaysInput
{
	DispatchRaysInput() = default;
	DispatchRaysInput(
		ID3D12StateObject* pPSO,
		void* pHitGroupShaderTable,
		UINT hitGroupStride,
		UINT hitGroupTableSize,
		LPCWSTR rayGenExportName,
		LPCWSTR missExportName);

	D3D12_DISPATCH_RAYS_DESC GetDispatchRayDesc(UINT DispatchWidth, UINT DispatchHeight);

	void Cleanup();

	UINT m_HitGroupStride;
	ComPtr<ID3D12StateObject> m_pPSO;
	ByteAddressBuffer m_RayGenShaderTable;
	ByteAddressBuffer m_MissShaderTable;
	ByteAddressBuffer m_HitShaderTable;
};
