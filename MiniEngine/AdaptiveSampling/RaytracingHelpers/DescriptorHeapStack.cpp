﻿#include "pch.h"
#include "DescriptorHeapStack.h"

DescriptorHeapStack::DescriptorHeapStack(ID3D12Device& device, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type,
                                         UINT NodeMask):
	m_device(device)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = numDescriptors;
	desc.Type = type;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = NodeMask;
	device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDescriptorHeap));

	m_descriptorSize = device.GetDescriptorHandleIncrementSize(type);
	m_descriptorHeapCpuBase = m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
}

ID3D12DescriptorHeap& DescriptorHeapStack::GetDescriptorHeap() const
{
	return *m_pDescriptorHeap.Get();
}

void DescriptorHeapStack::AllocateDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, UINT& descriptorHeapIndex)
{
	descriptorHeapIndex = m_descriptorsAllocated;
	cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descriptorHeapCpuBase, descriptorHeapIndex, m_descriptorSize);
	m_descriptorsAllocated++;
}

UINT DescriptorHeapStack::AllocateBufferSrv(ID3D12Resource& resource)
{
	UINT descriptorHeapIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	AllocateDescriptor(cpuHandle, descriptorHeapIndex);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.NumElements = (UINT)(resource.GetDesc().Width / sizeof(UINT32));
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	m_device.CreateShaderResourceView(&resource, &srvDesc, cpuHandle);
	return descriptorHeapIndex;
}

UINT DescriptorHeapStack::AllocateBufferUav(ID3D12Resource& resource)
{
	UINT descriptorHeapIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	AllocateDescriptor(cpuHandle, descriptorHeapIndex);
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = (UINT)(resource.GetDesc().Width / sizeof(UINT32));
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;

	m_device.CreateUnorderedAccessView(&resource, nullptr, &uavDesc, cpuHandle);
	return descriptorHeapIndex;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapStack::GetGpuHandle(UINT descriptorIndex) const
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_pDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), descriptorIndex,
	                                     m_descriptorSize);
}
