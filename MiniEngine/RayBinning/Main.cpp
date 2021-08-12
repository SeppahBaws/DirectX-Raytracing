#include "pch.h"
#include "GameCore.h"
#include "CommandContext.h"

#include "RaytracingTest.h"

using namespace GameCore;
using namespace Graphics;

// CREATE_APPLICATION(RaytracingTest)

inline bool IsDirectXRaytracingSupported(IDXGIAdapter1* adapter)
{
    ComPtr<ID3D12Device> testDevice;
    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureSupportData = {};

    return SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&testDevice)))
        && SUCCEEDED(testDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureSupportData, sizeof(featureSupportData)))
        && featureSupportData.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
}

// TODO: don't use the wmain function, instead do a simple check with QueryInterface and hard-exit if raytracing isn't supported. we don't care about fallbacks.

int wmain(int /*argc*/, wchar_t** /*argv*/)
{
#if _DEBUG
    ComPtr<ID3D12Debug> debugInterface;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
    {
        debugInterface->EnableDebugLayer();
    }
#endif

    ComPtr<ID3D12Device> pDevice;
    ComPtr<IDXGIAdapter1> pAdapter;
    ComPtr<IDXGIFactory2> pFactory;
    CreateDXGIFactory2(0, IID_PPV_ARGS(&pFactory));
    bool validDeviceFound = false;
    for (uint32_t Idx = 0; !validDeviceFound && DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(Idx, &pAdapter); ++Idx)
    {
        DXGI_ADAPTER_DESC1 desc;
        pAdapter->GetDesc1(&desc);

        if (IsDirectXRaytracingSupported(pAdapter.Get()))
        {
            validDeviceFound = true;
        }
        pAdapter = nullptr;
    }

    s_EnableVSync.Decrement();
    TargetResolution = k720p;
    g_DisplayWidth = 1280;
    g_DisplayHeight = 720;
    RaytracingTest app{};
    GameCore::RunApplication(app, L"RaytracingTest");
    return 0;
}
