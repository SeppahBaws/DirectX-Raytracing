#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer CB0
// {
//
//   float2 RcpTextureSize;             // Offset:    0 Size:     8
//   float ContrastThreshold;           // Offset:    8 Size:     4
//   float SubpixelRemoval;             // Offset:   12 Size:     4
//   uint LastQueueIndex;               // Offset:   16 Size:     4
//   uint2 StartPixel;                  // Offset:   20 Size:     8
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// LinearSampler                     sampler      NA          NA             s0      1 
// Color                             texture  float3          2d             t0      1 
// Luma                              texture   float          2d             t1      1 
// WorkCount                             UAV    byte         r/w             u0      1 
// WorkQueue                             UAV    byte         r/w             u1      1 
// ColorQueue                            UAV  float3         buf             u2      1 
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
dcl_constantbuffer CB0[2], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_uav_raw u0
dcl_uav_raw u1
dcl_uav_typed_buffer (float,float,float,float) u2
dcl_input vThreadIDInGroup.xy
dcl_input vThreadID.xy
dcl_temps 5
dcl_tgsm_structured g0, 4, 100
dcl_thread_group 8, 8, 1
iadd r0.yz, vThreadID.xxyx, cb0[1].yyzy
umax r1.x, vThreadIDInGroup.y, vThreadIDInGroup.x
ult r1.x, r1.x, l(5)
if_nz r1.x
  iadd r1.xy, r0.yzyy, vThreadIDInGroup.xyxx
  itof r1.xy, r1.xyxx
  mul r1.xy, r1.xyxx, cb0[0].xyxx
  gather4_indexable(texture2d)(float,float,float,float) r1.xyzw, r1.xyxx, t1.xyzw, s0.x
  imad r2.x, vThreadIDInGroup.y, l(10), vThreadIDInGroup.x
  ishl r2.y, r2.x, l(1)
  store_structured g0.x, r2.y, l(0), r1.w
  imad r1.w, r2.x, l(2), l(1)
  store_structured g0.x, r1.w, l(0), r1.z
  iadd r1.z, r2.y, l(10)
  store_structured g0.x, r1.z, l(0), r1.x
  iadd r1.x, r1.z, l(1)
  store_structured g0.x, r1.x, l(0), r1.y
endif 
sync_g_t
iadd r1.x, vThreadIDInGroup.x, l(1)
imad r1.x, vThreadIDInGroup.y, l(10), r1.x
ld_structured r1.y, r1.x, l(0), g0.xxxx
iadd r2.xyzw, r1.xxxx, l(10, 9, 11, 20)
ld_structured r1.z, r2.y, l(0), g0.xxxx
ld_structured r1.w, r2.x, l(0), g0.xxxx
ld_structured r2.x, r2.z, l(0), g0.xxxx
ld_structured r2.y, r2.w, l(0), g0.xxxx
max r2.z, r1.z, r1.y
max r2.w, r1.w, r2.y
max r2.w, r2.w, r2.x
max r2.z, r2.w, r2.z
min r2.w, r1.z, r1.y
min r3.x, r1.w, r2.y
min r3.x, r2.x, r3.x
min r2.w, r2.w, r3.x
add r2.z, -r2.w, r2.z
lt r2.w, r2.z, cb0[0].z
if_nz r2.w
  ret 
endif 
iadd r3.xyzw, r1.xxxx, l(-1, 1, 19, 21)
ld_structured r1.x, r3.x, l(0), g0.xxxx
ld_structured r2.w, r3.y, l(0), g0.xxxx
ld_structured r3.x, r3.z, l(0), g0.xxxx
ld_structured r3.y, r3.w, l(0), g0.xxxx
add r3.zw, r1.yyyz, r2.yyyx
add r4.x, r1.x, r3.x
add r4.y, r2.w, r3.y
add r3.x, r3.y, r3.x
add r1.x, r1.x, r2.w
mad r2.w, -r1.z, l(2.000000), r4.x
mad r3.y, -r1.w, l(2.000000), r3.z
mad r2.w, |r3.y|, l(2.000000), |r2.w|
mad r3.y, -r2.x, l(2.000000), r4.y
add r2.w, r2.w, |r3.y|
mad r3.x, -r2.y, l(2.000000), r3.x
mad r3.y, -r1.w, l(2.000000), r3.w
mad r3.x, |r3.y|, l(2.000000), |r3.x|
mad r1.x, -r1.y, l(2.000000), r1.x
add r1.x, |r1.x|, r3.x
add r3.x, r3.w, r3.z
mad r3.x, r3.x, l(2.000000), r4.x
add r3.x, r4.y, r3.x
mad r3.x, r3.x, l(0.083333), -r1.w
div_sat r2.z, |r3.x|, r2.z
mad r3.x, r2.z, l(-2.000000), l(3.000000)
mul r2.z, r2.z, r2.z
mul r2.z, r2.z, r3.x
mul r2.z, r2.z, r2.z
dp2_sat r2.z, r2.zzzz, cb0[0].wwww
ge r1.x, r2.w, r1.x
movc r1.y, r1.x, r1.y, r1.z
movc r1.z, r1.x, r2.y, r2.x
add r1.yz, -r1.wwww, r1.yyzy
ge r1.y, |r1.z|, |r1.y|
mul r1.z, r2.z, l(254.000000)
ftou r1.z, r1.z
and r1.z, r1.z, l(254)
ishl r2.xy, r0.zyzz, l(20, 8, 0, 0)
or r1.w, r2.y, r2.x
iadd r1.z, r1.w, r1.z
bfi r1.z, l(1), l(0), r1.y, r1.z
if_nz r1.x
  imm_atomic_iadd r2.x, u0, l(0), l(1)
  ishl r1.x, r2.x, l(2)
  store_raw u1.x, r1.x, r1.z
  bfi r1.x, l(1), l(1), r1.y, l(0)
  iadd r0.w, r0.z, r1.x
  iadd r3.xy, r0.ywyy, l(0, -1, 0, 0)
  mov r3.zw, l(0,0,0,0)
  ld_indexable(texture2d)(float,float,float,float) r2.yzw, r3.xyzw, t0.wxyz
  store_uav_typed u2.xyzw, r2.xxxx, r2.yzwy
else 
  imm_atomic_iadd r2.x, u0, l(4), l(1)
  iadd r0.w, -r2.x, cb0[1].x
  ishl r1.x, r0.w, l(2)
  store_raw u1.x, r1.x, r1.z
  bfi r1.x, l(1), l(1), r1.y, l(0)
  iadd r0.x, r0.y, r1.x
  iadd r1.xy, r0.xzxx, l(-1, 0, 0, 0)
  mov r1.zw, l(0,0,0,0)
  ld_indexable(texture2d)(float,float,float,float) r0.xyz, r1.xyzw, t0.xyzw
  store_uav_typed u2.xyzw, r0.wwww, r0.xyzx
endif 
ret 
// Approximately 105 instruction slots used
#endif

const BYTE g_pFXAAPass1_Luma2_CS[] =
{
     68,  88,  66,  67, 207, 152, 
    225,  20, 202, 135, 230, 225, 
    224, 245, 147, 252,  32, 225, 
    251, 196,   1,   0,   0,   0, 
     48,  18,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    156,   3,   0,   0, 172,   3, 
      0,   0, 188,   3,   0,   0, 
    208,  16,   0,   0, 108,  17, 
      0,   0,  82,  68,  69,  70, 
     92,   3,   0,   0,   1,   0, 
      0,   0,  88,   1,   0,   0, 
      7,   0,   0,   0,  60,   0, 
      0,   0,   0,   5,  83,  67, 
      0,   1,   0,   0,  52,   3, 
      0,   0,  82,  68,  49,  49, 
     60,   0,   0,   0,  24,   0, 
      0,   0,  32,   0,   0,   0, 
     40,   0,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0,  28,   1, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     42,   1,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,   9,   0, 
      0,   0,  48,   1,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  53,   1, 
      0,   0,   8,   0,   0,   0, 
      6,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     63,   1,   0,   0,   8,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  73,   1,   0,   0, 
      4,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,   0,   0, 
    255, 255, 255, 255,   2,   0, 
      0,   0,   1,   0,   0,   0, 
      9,   0,   0,   0,  84,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     76, 105, 110, 101,  97, 114, 
     83,  97, 109, 112, 108, 101, 
    114,   0,  67, 111, 108, 111, 
    114,   0,  76, 117, 109,  97, 
      0,  87, 111, 114, 107,  67, 
    111, 117, 110, 116,   0,  87, 
    111, 114, 107,  81, 117, 101, 
    117, 101,   0,  67, 111, 108, 
    111, 114,  81, 117, 101, 117, 
    101,   0,  67,  66,  48,   0, 
     84,   1,   0,   0,   5,   0, 
      0,   0, 112,   1,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   2,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
      2,   0,   0,   0,  80,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 116,   2, 
      0,   0,   8,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 140,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 176,   2,   0,   0, 
     12,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    140,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    192,   2,   0,   0,  16,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 216,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 252,   2, 
      0,   0,  20,   0,   0,   0, 
      8,   0,   0,   0,   2,   0, 
      0,   0,  16,   3,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  82,  99, 112,  84, 
    101, 120, 116, 117, 114, 101, 
     83, 105, 122, 101,   0, 102, 
    108, 111,  97, 116,  50,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  71,   2, 
      0,   0,  67, 111, 110, 116, 
    114,  97, 115, 116,  84, 104, 
    114, 101, 115, 104, 111, 108, 
    100,   0, 102, 108, 111,  97, 
    116,   0,   0,   0,   3,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 134,   2, 
      0,   0,  83, 117,  98, 112, 
    105, 120, 101, 108,  82, 101, 
    109, 111, 118,  97, 108,   0, 
     76,  97, 115, 116,  81, 117, 
    101, 117, 101,  73, 110, 100, 
    101, 120,   0, 100, 119, 111, 
    114, 100,   0, 171, 171, 171, 
      0,   0,  19,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 207,   2,   0,   0, 
     83, 116,  97, 114, 116,  80, 
    105, 120, 101, 108,   0, 117, 
    105, 110, 116,  50,   0, 171, 
    171, 171,   1,   0,  19,   0, 
      1,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   7,   3, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  49,  48,  46,  49,   0, 
     73,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  79,  83, 
     71,  78,   8,   0,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  83,  72,  69,  88, 
     12,  13,   0,   0,  80,   0, 
      5,   0,  67,   3,   0,   0, 
    106,   8,   0,   1,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   1,   0,   0,   0, 
     85,  85,   0,   0, 157,   0, 
      0,   3,   0, 224,  17,   0, 
      0,   0,   0,   0, 157,   0, 
      0,   3,   0, 224,  17,   0, 
      1,   0,   0,   0, 156,   8, 
      0,   4,   0, 224,  17,   0, 
      2,   0,   0,   0,  85,  85, 
      0,   0,  95,   0,   0,   2, 
     50,  32,   2,   0,  95,   0, 
      0,   2,  50,   0,   2,   0, 
    104,   0,   0,   2,   5,   0, 
      0,   0, 160,   0,   0,   5, 
      0, 240,  17,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
    100,   0,   0,   0, 155,   0, 
      0,   4,   8,   0,   0,   0, 
      8,   0,   0,   0,   1,   0, 
      0,   0,  30,   0,   0,   7, 
     98,   0,  16,   0,   0,   0, 
      0,   0,   6,   1,   2,   0, 
     86, 134,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     83,   0,   0,   5,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     26,  32,   2,   0,  10,  32, 
      2,   0,  79,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   5,   0,   0,   0, 
     31,   0,   4,   3,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     30,   0,   0,   6,  50,   0, 
     16,   0,   1,   0,   0,   0, 
    150,   5,  16,   0,   0,   0, 
      0,   0,  70,  32,   2,   0, 
     43,   0,   0,   5,  50,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   8, 
     50,   0,  16,   0,   1,   0, 
      0,   0,  70,   0,  16,   0, 
      1,   0,   0,   0,  70, 128, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 109,   0, 
      0, 139, 194,   0,   0, 128, 
     67,  85,  21,   0, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0,  70, 126,  16,   0, 
      1,   0,   0,   0,  10,  96, 
     16,   0,   0,   0,   0,   0, 
     35,   0,   0,   7,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     26,  32,   2,   0,   1,  64, 
      0,   0,  10,   0,   0,   0, 
     10,  32,   2,   0,  41,   0, 
      0,   7,  34,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  35,   0,   0,   9, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,  30,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,  10,   0,   0,   0, 
    168,   0,   0,   9,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     30,   0,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0, 168,   0, 
      0,   9,  18, 240,  17,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  21,   0, 
      0,   1, 190,  24,   0,   1, 
     30,   0,   0,   6,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,  32,   2,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
     35,   0,   0,   8,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     26,  32,   2,   0,   1,  64, 
      0,   0,  10,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0, 167,   0,   0,   9, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0,  30,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,  10,   0,   0,   0, 
      9,   0,   0,   0,  11,   0, 
      0,   0,  20,   0,   0,   0, 
    167,   0,   0,   9,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9,  34,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
     52,   0,   0,   7,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  52,   0, 
      0,   7, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   2,   0, 
      0,   0,  52,   0,   0,   7, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     52,   0,   0,   7,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  51,   0, 
      0,   7, 130,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  51,   0,   0,   7, 
     18,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   2,   0,   0,   0, 
     51,   0,   0,   7,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,  51,   0, 
      0,   7, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   0,   0,   0,   8, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16, 128, 
     65,   0,   0,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  49,   0, 
      0,   8, 130,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
     42, 128,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     31,   0,   4,   3,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     62,   0,   0,   1,  21,   0, 
      0,   1,  30,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0,  19,   0, 
      0,   0,  21,   0,   0,   0, 
    167,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     26,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     42,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9,  34,   0, 
     16,   0,   3,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
      0,   0,   0,   7, 194,   0, 
     16,   0,   3,   0,   0,   0, 
     86,   9,  16,   0,   1,   0, 
      0,   0,  86,   1,  16,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   7,  18,   0,  16,   0, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   0,   0,   0,   7, 
     34,   0,  16,   0,   4,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   3,   0,   0,   0, 
      0,   0,   0,   7,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   7,  18,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  10, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  64,  10,   0, 
     16,   0,   4,   0,   0,   0, 
     50,   0,   0,  10,  34,   0, 
     16,   0,   3,   0,   0,   0, 
     58,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  64,  42,   0,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  11, 130,   0,  16,   0, 
      2,   0,   0,   0,  26,   0, 
     16, 128, 129,   0,   0,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  64, 
     58,   0,  16, 128, 129,   0, 
      0,   0,   2,   0,   0,   0, 
     50,   0,   0,  10,  34,   0, 
     16,   0,   3,   0,   0,   0, 
     10,   0,  16, 128,  65,   0, 
      0,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  64,  26,   0,  16,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   8, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     26,   0,  16, 128, 129,   0, 
      0,   0,   3,   0,   0,   0, 
     50,   0,   0,  10,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16, 128,  65,   0, 
      0,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  64,  10,   0,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10,  34,   0,  16,   0, 
      3,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  64, 
     58,   0,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  11, 
     18,   0,  16,   0,   3,   0, 
      0,   0,  26,   0,  16, 128, 
    129,   0,   0,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  64,  10,   0, 
     16, 128, 129,   0,   0,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10,  18,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  64, 
     10,   0,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16, 128, 
    129,   0,   0,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   7,  18,   0,  16,   0, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   3,   0,   0,   0, 
     42,   0,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,   9, 
     18,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  64, 
     10,   0,  16,   0,   4,   0, 
      0,   0,   0,   0,   0,   7, 
     18,   0,  16,   0,   3,   0, 
      0,   0,  26,   0,  16,   0, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  10,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
    171, 170, 170,  61,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,  14,  32, 
      0,   8,  66,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16, 128, 129,   0,   0,   0, 
      3,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,   9,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0, 192,   1,  64, 
      0,   0,   0,   0,  64,  64, 
     56,   0,   0,   7,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   7,  66,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,  56,   0,   0,   7, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
     15,  32,   0,   8,  66,   0, 
     16,   0,   2,   0,   0,   0, 
    166,  10,  16,   0,   2,   0, 
      0,   0, 246, 143,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  29,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     55,   0,   0,   9,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     55,   0,   0,   9,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
      0,   0,   0,   8,  98,   0, 
     16,   0,   1,   0,   0,   0, 
    246,  15,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
     86,   6,  16,   0,   1,   0, 
      0,   0,  29,   0,   0,   9, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16, 128, 
    129,   0,   0,   0,   1,   0, 
      0,   0,  26,   0,  16, 128, 
    129,   0,   0,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 126,  67, 
     28,   0,   0,   5,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,   1,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0, 254,   0,   0,   0, 
     41,   0,   0,  10,  50,   0, 
     16,   0,   2,   0,   0,   0, 
    102,  10,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
     20,   0,   0,   0,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  60,   0, 
      0,   7, 130,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  30,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
    140,   0,   0,  11,  66,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,  31,   0,   4,   3, 
     10,   0,  16,   0,   1,   0, 
      0,   0, 180,   0,   0,   9, 
     18,   0,  16,   0,   2,   0, 
      0,   0,   0, 224,  17,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  41,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   2,   0,   0,   0, 
    166,   0,   0,   7,  18, 224, 
     17,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0, 140,   0, 
      0,  11,  18,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     30,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  30,   0, 
      0,  10,  50,   0,  16,   0, 
      3,   0,   0,   0, 214,   5, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   8, 
    194,   0,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  45,   0, 
      0, 137, 194,   0,   0, 128, 
     67,  85,  21,   0, 226,   0, 
     16,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  54, 121,  16,   0, 
      0,   0,   0,   0, 164,   0, 
      0,   7, 242, 224,  17,   0, 
      2,   0,   0,   0,   6,   0, 
     16,   0,   2,   0,   0,   0, 
    150,   7,  16,   0,   2,   0, 
      0,   0,  18,   0,   0,   1, 
    180,   0,   0,   9,  18,   0, 
     16,   0,   2,   0,   0,   0, 
      0, 224,  17,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
     30,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16, 128,  65,   0, 
      0,   0,   2,   0,   0,   0, 
     10, 128,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     41,   0,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      2,   0,   0,   0, 166,   0, 
      0,   7,  18, 224,  17,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0, 140,   0,   0,  11, 
     18,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  30,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  30,   0,   0,  10, 
     50,   0,  16,   0,   1,   0, 
      0,   0, 134,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   8, 194,   0, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  45,   0,   0, 137, 
    194,   0,   0, 128,  67,  85, 
     21,   0, 114,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0, 164,   0,   0,   7, 
    242, 224,  17,   0,   2,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0, 105,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  40,   0,   0,   0, 
     20,   0,   0,   0,   4,   0, 
      0,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      2,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,   4,   0, 
      0,   0,  82,  84,  83,  48, 
    188,   0,   0,   0,   2,   0, 
      0,   0,   3,   0,   0,   0, 
     24,   0,   0,   0,   1,   0, 
      0,   0, 136,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     60,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     72,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    104,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      7,   0,   0,   0,   1,   0, 
      0,   0,  80,   0,   0,   0, 
      1,   0,   0,   0,   5,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0, 112,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,  21,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 127, 127, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};
