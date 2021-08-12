#pragma once

#ifndef HLSL
#include "HlslCompat.h"
#endif

struct RayTraceMeshInfo
{
	uint m_IndexOffsetBytes;
	uint m_UVAttributeOffsetBytes;
	uint m_NormalAttributeOffsetBytes;
	uint m_TangentAttributeOffsetBytes;
	uint m_BitangentAttributeOffsetBytes;
	uint m_PositionAttributeOffsetBytes;
	uint m_AttributeStrideBytes;
	uint m_MaterialInstanceId;
};
