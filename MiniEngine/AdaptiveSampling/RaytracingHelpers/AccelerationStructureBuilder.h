#pragma once

#include "DescriptorHeapStack.h"
#include "Model.h"

class AccelerationStructureBuilder
{
public:
	AccelerationStructureBuilder() = default;
	~AccelerationStructureBuilder() = default;

	static void Build(ComPtr<ID3D12Device5> rtDevice, const Model* pModel, DescriptorHeapStack* descriptorHeap,
		ByteAddressBuffer& scratchBuffer, ByteAddressBuffer& instanceDataBuffer, ComPtr<ID3D12Resource>& tlas,
		std::vector<ComPtr<ID3D12Resource>>& blases);
};
