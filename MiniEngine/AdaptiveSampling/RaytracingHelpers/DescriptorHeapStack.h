#pragma once

class DescriptorHeapStack
{
public:
	DescriptorHeapStack(ID3D12Device& device, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT NodeMask);

	ID3D12DescriptorHeap& GetDescriptorHeap() const;

	void AllocateDescriptor(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, _Out_ UINT& descriptorHeapIndex);
	UINT AllocateBufferSrv(_In_ ID3D12Resource& resource);
	UINT AllocateBufferUav(_In_ ID3D12Resource& resource);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(UINT descriptorIndex) const;

private:
	ID3D12Device& m_device;
	ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;
	UINT m_descriptorsAllocated = 0;
	UINT m_descriptorSize;
	D3D12_CPU_DESCRIPTOR_HANDLE m_descriptorHeapCpuBase;
};
