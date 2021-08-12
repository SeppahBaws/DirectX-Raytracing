//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//

#pragma once

#define NOMINMAX

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable:4328) // nonstandard extension used : class rvalue used as lvalue
#pragma warning(disable:4324) // structure was padded due to __declspec(align())

#include <winapifamily.h>        // for WINAPI_FAMILY
#include <wrl.h>

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
	#define NOMINMAX
#endif
#include <windows.h>

#if _XBOX_ONE
#ifndef _DEBUG
	#define D3DCOMPILE_NO_DEBUG 1
#endif
	#include <d3d12_x.h>
	#include <d3dx12_x.h>
	#include <xdk.h>
	#include <pix.h>

	#pragma comment(lib, "d3d12_x.lib")
	#pragma comment(lib, "xg_x.lib")
	#pragma comment(lib, "pixevt.lib")

	#define MY_IID_PPV_ARGS(ppType)  __uuidof(**(ppType)), (void**)(ppType)
#else
	#include <d3d12.h>

	#pragma comment(lib, "d3d12.lib")
	#pragma comment(lib, "dxgi.lib")

	#define D3D12_GPU_VIRTUAL_ADDRESS_NULL	  ((D3D12_GPU_VIRTUAL_ADDRESS)0)
	#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN   ((D3D12_GPU_VIRTUAL_ADDRESS)-1)
	#define MY_IID_PPV_ARGS IID_PPV_ARGS

#if _MSC_VER >= 1800
	#include <d3d11_2.h>
	#include <pix3.h>
#endif

	#include "d3dx12.h"

#endif

#include <cstdint>
#include <cstdio>
#include <cstdarg>
#include <vector>
#include <memory>
#include <string>
#include <exception>

#include <wrl.h>
#include <ppltasks.h>

#include "Utility.h"
#include "VectorMath.h"
#include "EngineTuning.h"
#include "EngineProfiling.h"

// useful for operator""s
using namespace std::string_literals;

// ComPtr<T>
using namespace Microsoft::WRL;

inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
	HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
	[[nodiscard]] HRESULT Error() const { return m_hr; }
private:
	const HRESULT m_hr;
};

#define SAFE_RELEASE(p) if (p) (p)->Release()

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw HrException(hr);
	}
}

#define ALIGN(alignment, num) ((((num) + alignment - 1) / alignment) * alignment)

inline bool operator==(const Math::Vector3& a, const Math::Vector3& b)
{
	return
		static_cast<float>(a.GetX()) == static_cast<float>(b.GetX()) &&
		static_cast<float>(a.GetY()) == static_cast<float>(b.GetY()) &&
		static_cast<float>(a.GetZ()) == static_cast<float>(b.GetZ());
}

inline bool operator==(const Math::Vector4& a, const Math::Vector4& b)
{
	return
		static_cast<float>(a.GetX()) == static_cast<float>(b.GetX()) &&
		static_cast<float>(a.GetY()) == static_cast<float>(b.GetY()) &&
		static_cast<float>(a.GetZ()) == static_cast<float>(b.GetZ()) &&
		static_cast<float>(a.GetW()) == static_cast<float>(b.GetW());
}

inline bool operator==(const Math::Matrix4& a, const Math::Matrix4& b)
{
	return
		Dot(a.GetX(), b.GetX()) == 1.0f &&
		Dot(a.GetY(), b.GetY()) == 1.0f &&
		Dot(a.GetZ(), b.GetZ()) == 1.0f &&
		Dot(a.GetW(), b.GetW()) == 1.0f;
}

inline bool operator==(const Math::Matrix3& a, const Math::Matrix3& b)
{
	return
		Dot(a.GetX(), b.GetX()) == 1.0f &&
		Dot(a.GetY(), b.GetY()) == 1.0f &&
		Dot(a.GetZ(), b.GetZ()) == 1.0f;
}

inline bool operator!=(const Math::Matrix4& a, const Math::Matrix4& b)
{
	return !(a == b);
}

inline bool operator!=(const Math::Matrix3& a, const Math::Matrix3& b)
{
	return !(a == b);
}

