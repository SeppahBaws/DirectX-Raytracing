#ifndef PACKING_HLSLI
#define PACKING_HLSLI

// Packs a float3 into a float2 using the Octahedral mapping.
// Implementation from Unity's HDRP: https://github.com/Unity-Technologies/Graphics
inline float2 PackOctahedral(float3 n)
{
    n *= rcp(dot(abs(n), 1.0));
    const float t = saturate(-n.z);
    return n.xy + (n.xy >= 0.0 ? t : -t);
}

// Unpacks a float2 Octahedral encoded vector.
// Implementation from Unity's HDRP: https://github.com/Unity-Technologies/Graphics
inline float3 UnpackOctahedral(float2 f)
{
    float3 n = float3(f.x, f.y, 1.0 - abs(f.x) - abs(f.y));

    const float t = max(-n.z, 0.0);
    n.xy += n.xy >= 0.0 ? -t.xx : t.xx;

    return normalize(n);
}

#endif // PACKING_HLSLI
