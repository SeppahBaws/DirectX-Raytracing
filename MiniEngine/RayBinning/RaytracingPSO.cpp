#include "pch.h"
#include "RaytracingPSO.h"

#include "RootSignature.h"

RaytracingPSO::RaytracingPSO()
{
    m_PSODesc = { D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE };
}

void RaytracingPSO::Destroy()
{
    m_PSO->Release();
}

void RaytracingPSO::AddShader(const void* pShaderCode, SIZE_T shaderCodeSize, LPCWSTR exportName)
{
    auto shaderLib = m_PSODesc.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
    D3D12_SHADER_BYTECODE bytecode = CD3DX12_SHADER_BYTECODE(pShaderCode, shaderCodeSize);
    shaderLib->SetDXILLibrary(&bytecode);
    shaderLib->DefineExport(exportName);
}

void RaytracingPSO::AddClosestHitGroup(LPCWSTR closestHitExportName, LPCWSTR hitGroupExportName, D3D12_HIT_GROUP_TYPE hitGroupType)
{
    auto hitGroup = m_PSODesc.CreateSubobject<CD3DX12_HIT_GROUP_SUBOBJECT>();
    hitGroup->SetClosestHitShaderImport(closestHitExportName);
    hitGroup->SetHitGroupExport(hitGroupExportName);
    hitGroup->SetHitGroupType(hitGroupType);
}

void RaytracingPSO::Configure(UINT payloadSize, UINT attributeSize, UINT maxRecursionDepth)
{
    auto shaderConfig = m_PSODesc.CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
    shaderConfig->Config(payloadSize, attributeSize);

    auto pipelineConfig = m_PSODesc.CreateSubobject<CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT>();
    pipelineConfig->Config(maxRecursionDepth);
}

void RaytracingPSO::AddLocalRootSignature(const RootSignature& signature, LPCWSTR exportName)
{
    auto localRootSig = m_PSODesc.CreateSubobject<CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT>();
    localRootSig->SetRootSignature(signature.GetSignature());

    auto association = m_PSODesc.CreateSubobject<CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT>();
    association->SetSubobjectToAssociate(*localRootSig);
    association->AddExport(exportName);
}

void RaytracingPSO::SetGlobalRootSignature(const RootSignature& signature)
{
    auto globalRootSig = m_PSODesc.CreateSubobject<CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT>();
    globalRootSig->SetRootSignature(signature.GetSignature());
}

HRESULT RaytracingPSO::Finalize(ComPtr<ID3D12Device5> pRTDevice)
{
    return pRTDevice->CreateStateObject(m_PSODesc, MY_IID_PPV_ARGS(&m_PSO));
}
