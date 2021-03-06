#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Note: shader requires additional functionality:
//       Typed UAV Load Additional Formats
//
//
// Buffer Definitions: 
//
// cbuffer CB0
// {
//
//   float2 g_RcpBufferDim;             // Offset:    0 Size:     8
//   float g_BloomStrength;             // Offset:    8 Size:     4
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// LinearSampler                     sampler      NA          NA             s0      1 
// Bloom                             texture  float3          2d             t0      1 
// SrcColor                              UAV  float3          2d             u0      1 
// OutLuma                               UAV   float          2d             u1      1 
// CB0                               cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Output
cs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[1], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_uav_typed_texture2d (float,float,float,float) u0
dcl_uav_typed_texture2d (float,float,float,float) u1
dcl_input vThreadID.xy
dcl_temps 2
dcl_thread_group 8, 8, 1
ld_uav_typed_indexable(texture2d)(float,float,float,float) r0.xyz, vThreadID.xyyy, u0.xyzw
utof r1.xy, vThreadID.xyxx
add r1.xy, r1.xyxx, l(0.500000, 0.500000, 0.000000, 0.000000)
mul r1.xy, r1.xyxx, cb0[0].xyxx
sample_l_indexable(texture2d)(float,float,float,float) r1.xyz, r1.xyxx, t0.xyzw, s0, l(0.000000)
mad r0.xyz, cb0[0].zzzz, r1.xyzx, r0.xyzx
dp3 r0.x, r0.xyzx, l(0.212671, 0.715160, 0.072169, 0.000000)
store_uav_typed u0.xyzw, vThreadID.xyyy, r0.xxxx
mad r0.x, r0.x, l(15.000000), l(1.000000)
log r0.x, r0.x
mul r0.x, r0.x, l(0.250000)
store_uav_typed u1.xyzw, vThreadID.xyyy, r0.xxxx
ret 
// Approximately 13 instruction slots used
#endif

const BYTE g_pDebugLuminanceLdr2CS[] =
{
     68,  88,  66,  67,  99, 216, 
    248, 178, 253, 158,  37,  20, 
     81, 192, 221, 225, 235,  87, 
     57,  42,   1,   0,   0,   0, 
     52,   6,   0,   0,   7,   0, 
      0,   0,  60,   0,   0,   0, 
     84,   2,   0,   0, 100,   2, 
      0,   0, 116,   2,   0,   0, 
    120,   4,   0,   0, 136,   4, 
      0,   0,  36,   5,   0,   0, 
     82,  68,  69,  70,  16,   2, 
      0,   0,   1,   0,   0,   0, 
      8,   1,   0,   0,   5,   0, 
      0,   0,  60,   0,   0,   0, 
      0,   5,  83,  67,   0,   1, 
      0,   0, 232,   1,   0,   0, 
     82,  68,  49,  49,  60,   0, 
      0,   0,  24,   0,   0,   0, 
     32,   0,   0,   0,  40,   0, 
      0,   0,  36,   0,   0,   0, 
     12,   0,   0,   0,   0,   0, 
      0,   0, 220,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 234,   0, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   9,   0,   0,   0, 
    240,   0,   0,   0,   4,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,   9,   0, 
      0,   0, 249,   0,   0,   0, 
      4,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     76, 105, 110, 101,  97, 114, 
     83,  97, 109, 112, 108, 101, 
    114,   0,  66, 108, 111, 111, 
    109,   0,  83, 114,  99,  67, 
    111, 108, 111, 114,   0,  79, 
    117, 116,  76, 117, 109,  97, 
      0,  67,  66,  48,   0, 171, 
    171, 171,   1,   1,   0,   0, 
      2,   0,   0,   0,  32,   1, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 112,   1,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,   2,   0,   0,   0, 
    136,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    172,   1,   0,   0,   8,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 196,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 103,  95, 
     82,  99, 112,  66, 117, 102, 
    102, 101, 114,  68, 105, 109, 
      0, 102, 108, 111,  97, 116, 
     50,   0, 171, 171,   1,   0, 
      3,   0,   1,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    127,   1,   0,   0, 103,  95, 
     66, 108, 111, 111, 109,  83, 
    116, 114, 101, 110, 103, 116, 
    104,   0, 102, 108, 111,  97, 
    116,   0, 171, 171,   0,   0, 
      3,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    188,   1,   0,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  49,  48,  46, 
     49,   0,  73,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     79,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  83,  72, 
     69,  88, 252,   1,   0,   0, 
     80,   0,   5,   0, 127,   0, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  90,   0, 
      0,   3,   0,  96,  16,   0, 
      0,   0,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  85,  85, 
      0,   0, 156,  24,   0,   4, 
      0, 224,  17,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
    156,  24,   0,   4,   0, 224, 
     17,   0,   1,   0,   0,   0, 
     85,  85,   0,   0,  95,   0, 
      0,   2,  50,   0,   2,   0, 
    104,   0,   0,   2,   2,   0, 
      0,   0, 155,   0,   0,   4, 
      8,   0,   0,   0,   8,   0, 
      0,   0,   1,   0,   0,   0, 
    163,   0,   0, 136, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   5,   2,   0, 
     70, 238,  17,   0,   0,   0, 
      0,   0,  86,   0,   0,   4, 
     50,   0,  16,   0,   1,   0, 
      0,   0,  70,   0,   2,   0, 
      0,   0,   0,  10,  50,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,  63,   0,   0, 
      0,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8,  50,   0,  16,   0, 
      1,   0,   0,   0,  70,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 128,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     72,   0,   0, 141, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   0,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10, 114,   0,  16,   0, 
      0,   0,   0,   0, 166, 138, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  16,   0,   0,  10, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0, 109, 198,  89,  62, 
    186,  20,  55,  63,  87, 205, 
    147,  61,   0,   0,   0,   0, 
    164,   0,   0,   6, 242, 224, 
     17,   0,   0,   0,   0,   0, 
     70,   5,   2,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,   9,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 112,  65,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     47,   0,   0,   5,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  62, 
    164,   0,   0,   6, 242, 224, 
     17,   0,   1,   0,   0,   0, 
     70,   5,   2,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     62,   0,   0,   1,  83,  70, 
     73,  48,   8,   0,   0,   0, 
      0,   8,   0,   0,   0,   0, 
      0,   0,  83,  84,  65,  84, 
    148,   0,   0,   0,  13,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   7,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  82,  84,  83,  48, 
      8,   1,   0,   0,   2,   0, 
      0,   0,   4,   0,   0,   0, 
     24,   0,   0,   0,   2,   0, 
      0,   0, 160,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     72,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     84,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    116,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
    148,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   1,   0, 
      0,   0,  92,   0,   0,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0, 124,   0, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  21,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 127, 127, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     21,   0,   0,   0,   4,   0, 
      0,   0,   4,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 127, 127,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};
