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
//   float2 RcpTextureSize;             // Offset:    0 Size:     8 [unused]
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
// Color                             texture    uint          2d             t0      1 
// WorkCount                             UAV    byte         r/w             u0      1 
// WorkQueue                             UAV    byte         r/w             u1      1 
// ColorQueue                            UAV  float3         buf             u2      1 
// Luma                                  UAV   float          2d             u3      1 
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
dcl_resource_texture2d (uint,uint,uint,uint) t0
dcl_uav_raw u0
dcl_uav_raw u1
dcl_uav_typed_buffer (float,float,float,float) u2
dcl_uav_typed_texture2d (float,float,float,float) u3
dcl_input vThreadIDInGroupFlattened
dcl_input vThreadGroupID.xy
dcl_input vThreadIDInGroup.xy
dcl_input vThreadID.xy
dcl_temps 5
dcl_tgsm_structured g0, 4, 100
dcl_thread_group 8, 8, 1
ult r0.x, vThreadIDInGroupFlattened.x, l(50)
if_nz r0.x
  udiv r0.x, r1.x, vThreadIDInGroupFlattened.x, l(10)
  mov r1.xz, r1.xxxx
  mov r1.yw, r0.xxxx
  iadd r0.xyzw, r1.xyzw, cb0[1].yzyz
  ishl r1.xyzw, vThreadGroupID.xyxy, l(3, 3, 3, 3)
  iadd r0.xyzw, r0.xyzw, r1.xyzw
  iadd r0.xyzw, r0.zwxy, l(-1, 4, -1, -1)
  mov r1.xy, r0.zwzz
  mov r1.zw, l(0,0,0,0)
  ld_indexable(texture2d)(uint,uint,uint,uint) r1.z, r1.xyzw, t0.yzxw
  bfi r1.w, l(11), l(4), r1.z, l(0)
  f16tof32 r2.x, r1.w
  ushr r1.zw, r1.zzzz, l(0, 0, 7, 17)
  and r1.zw, r1.zzzw, l(0, 0, 0x00007ff0, 0x00007fe0)
  f16tof32 r2.yz, r1.zzwz
  dp3 r1.z, r2.xyzx, l(0.212671, 0.715160, 0.072169, 0.000000)
  mad r1.z, r1.z, l(15.000000), l(1.000000)
  log r1.z, r1.z
  mul r1.z, r1.z, l(0.250000)
  store_uav_typed u3.xyzw, r1.xyyy, r1.zzzz
  store_structured g0.x, vThreadIDInGroupFlattened.x, l(0), r1.z
  iadd r1.x, vThreadIDInGroupFlattened.x, l(50)
  mov r0.zw, l(0,0,0,0)
  ld_indexable(texture2d)(uint,uint,uint,uint) r0.z, r0.xyzw, t0.yzxw
  bfi r0.w, l(11), l(4), r0.z, l(0)
  f16tof32 r2.x, r0.w
  ushr r0.zw, r0.zzzz, l(0, 0, 7, 17)
  and r0.zw, r0.zzzw, l(0, 0, 0x00007ff0, 0x00007fe0)
  f16tof32 r2.yz, r0.zzwz
  dp3 r0.z, r2.xyzx, l(0.212671, 0.715160, 0.072169, 0.000000)
  mad r0.z, r0.z, l(15.000000), l(1.000000)
  log r0.z, r0.z
  mul r0.z, r0.z, l(0.250000)
  store_uav_typed u3.xyzw, r0.xyyy, r0.zzzz
  store_structured g0.x, r1.x, l(0), r0.z
endif 
sync_g_t
iadd r0.x, vThreadIDInGroup.x, l(1)
imad r0.x, vThreadIDInGroup.y, l(10), r0.x
ld_structured r0.y, r0.x, l(0), g0.xxxx
iadd r1.xyzw, r0.xxxx, l(10, 9, 11, 20)
ld_structured r0.z, r1.y, l(0), g0.xxxx
ld_structured r0.w, r1.x, l(0), g0.xxxx
ld_structured r1.x, r1.z, l(0), g0.xxxx
ld_structured r1.y, r1.w, l(0), g0.xxxx
max r1.z, r0.z, r0.y
max r1.w, r0.w, r1.y
max r1.w, r1.w, r1.x
max r1.z, r1.w, r1.z
min r1.w, r0.z, r0.y
min r2.x, r0.w, r1.y
min r2.x, r1.x, r2.x
min r1.w, r1.w, r2.x
add r1.z, -r1.w, r1.z
lt r1.w, r1.z, cb0[0].z
if_nz r1.w
  ret 
endif 
iadd r2.yz, vThreadID.xxyx, cb0[1].yyzy
iadd r3.xyzw, r0.xxxx, l(-1, 1, 19, 21)
ld_structured r0.x, r3.x, l(0), g0.xxxx
ld_structured r1.w, r3.y, l(0), g0.xxxx
ld_structured r3.x, r3.z, l(0), g0.xxxx
ld_structured r3.y, r3.w, l(0), g0.xxxx
add r3.zw, r0.yyyz, r1.yyyx
add r4.x, r0.x, r3.x
add r4.y, r1.w, r3.y
add r3.x, r3.y, r3.x
add r0.x, r0.x, r1.w
mad r1.w, -r0.z, l(2.000000), r4.x
mad r3.y, -r0.w, l(2.000000), r3.z
mad r1.w, |r3.y|, l(2.000000), |r1.w|
mad r3.y, -r1.x, l(2.000000), r4.y
add r1.w, r1.w, |r3.y|
mad r3.x, -r1.y, l(2.000000), r3.x
mad r3.y, -r0.w, l(2.000000), r3.w
mad r3.x, |r3.y|, l(2.000000), |r3.x|
mad r0.x, -r0.y, l(2.000000), r0.x
add r0.x, |r0.x|, r3.x
add r3.x, r3.w, r3.z
mad r3.x, r3.x, l(2.000000), r4.x
add r3.x, r4.y, r3.x
mad r3.x, r3.x, l(0.083333), -r0.w
div_sat r1.z, |r3.x|, r1.z
mad r3.x, r1.z, l(-2.000000), l(3.000000)
mul r1.z, r1.z, r1.z
mul r1.z, r1.z, r3.x
mul r1.z, r1.z, r1.z
dp2_sat r1.z, r1.zzzz, cb0[0].wwww
ge r0.x, r1.w, r0.x
movc r0.y, r0.x, r0.y, r0.z
movc r0.z, r0.x, r1.y, r1.x
add r0.yz, -r0.wwww, r0.yyzy
ge r0.y, |r0.z|, |r0.y|
mul r0.z, r1.z, l(254.000000)
ftou r0.z, r0.z
and r0.z, r0.z, l(254)
ishl r1.xy, r2.zyzz, l(20, 8, 0, 0)
or r0.w, r1.y, r1.x
iadd r0.z, r0.w, r0.z
bfi r0.z, l(1), l(0), r0.y, r0.z
if_nz r0.x
  imm_atomic_iadd r1.x, u0, l(0), l(1)
  ishl r0.x, r1.x, l(2)
  store_raw u1.x, r0.x, r0.z
  bfi r0.x, l(1), l(1), r0.y, l(0)
  iadd r2.w, r2.z, r0.x
  iadd r3.xy, r2.ywyy, l(0, -1, 0, 0)
  mov r3.zw, l(0,0,0,0)
  ld_indexable(texture2d)(uint,uint,uint,uint) r0.x, r3.xyzw, t0.xyzw
  bfi r0.w, l(11), l(4), r0.x, l(0)
  f16tof32 r3.xw, r0.wwww
  ushr r0.xw, r0.xxxx, l(7, 0, 0, 17)
  and r0.xw, r0.xxxw, l(0x00007ff0, 0, 0, 0x00007fe0)
  f16tof32 r3.yz, r0.xxwx
  store_uav_typed u2.xyzw, r1.xxxx, r3.xyzw
else 
  imm_atomic_iadd r1.x, u0, l(4), l(1)
  iadd r0.x, -r1.x, cb0[1].x
  ishl r0.w, r0.x, l(2)
  store_raw u1.x, r0.w, r0.z
  bfi r0.y, l(1), l(1), r0.y, l(0)
  iadd r2.x, r2.y, r0.y
  iadd r1.xy, r2.xzxx, l(-1, 0, 0, 0)
  mov r1.zw, l(0,0,0,0)
  ld_indexable(texture2d)(uint,uint,uint,uint) r0.y, r1.xyzw, t0.yxzw
  bfi r0.z, l(11), l(4), r0.y, l(0)
  f16tof32 r1.xw, r0.zzzz
  ushr r0.yz, r0.yyyy, l(0, 7, 17, 0)
  and r0.yz, r0.yyzy, l(0, 0x00007ff0, 0x00007fe0, 0)
  f16tof32 r1.yz, r0.yyzy
  store_uav_typed u2.xyzw, r0.xxxx, r1.xyzw
endif 
ret 
// Approximately 136 instruction slots used
#endif

const BYTE g_pFXAAPass1_RGB_CS[] =
{
     68,  88,  66,  67,  67, 154, 
    104,  87, 167, 125, 226, 188, 
    112,  64, 191, 117,  31, 151, 
    115, 254,   1,   0,   0,   0, 
    220,  21,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    112,   3,   0,   0, 128,   3, 
      0,   0, 144,   3,   0,   0, 
    124,  20,   0,   0,  24,  21, 
      0,   0,  82,  68,  69,  70, 
     48,   3,   0,   0,   1,   0, 
      0,   0,  44,   1,   0,   0, 
      6,   0,   0,   0,  60,   0, 
      0,   0,   0,   5,  83,  67, 
      0,   1,   0,   0,   8,   3, 
      0,   0,  82,  68,  49,  49, 
     60,   0,   0,   0,  24,   0, 
      0,   0,  32,   0,   0,   0, 
     40,   0,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0, 252,   0, 
      0,   0,   2,   0,   0,   0, 
      4,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      2,   1,   0,   0,   8,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  12,   1,   0,   0, 
      8,   0,   0,   0,   6,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  22,   1, 
      0,   0,   4,   0,   0,   0, 
      5,   0,   0,   0,   1,   0, 
      0,   0, 255, 255, 255, 255, 
      2,   0,   0,   0,   1,   0, 
      0,   0,   9,   0,   0,   0, 
     33,   1,   0,   0,   4,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   3,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  38,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  67, 111, 
    108, 111, 114,   0,  87, 111, 
    114, 107,  67, 111, 117, 110, 
    116,   0,  87, 111, 114, 107, 
     81, 117, 101, 117, 101,   0, 
     67, 111, 108, 111, 114,  81, 
    117, 101, 117, 101,   0,  76, 
    117, 109,  97,   0,  67,  66, 
     48,   0, 171, 171,  38,   1, 
      0,   0,   5,   0,   0,   0, 
     68,   1,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  12,   2, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   0,  36,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  72,   2,   0,   0, 
      8,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     96,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    132,   2,   0,   0,  12,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,  96,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 148,   2, 
      0,   0,  16,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 172,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 208,   2,   0,   0, 
     20,   0,   0,   0,   8,   0, 
      0,   0,   2,   0,   0,   0, 
    228,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     82,  99, 112,  84, 101, 120, 
    116, 117, 114, 101,  83, 105, 
    122, 101,   0, 102, 108, 111, 
     97, 116,  50,   0, 171, 171, 
      1,   0,   3,   0,   1,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  27,   2,   0,   0, 
     67, 111, 110, 116, 114,  97, 
    115, 116,  84, 104, 114, 101, 
    115, 104, 111, 108, 100,   0, 
    102, 108, 111,  97, 116,   0, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  90,   2,   0,   0, 
     83, 117,  98, 112, 105, 120, 
    101, 108,  82, 101, 109, 111, 
    118,  97, 108,   0,  76,  97, 
    115, 116,  81, 117, 101, 117, 
    101,  73, 110, 100, 101, 120, 
      0, 100, 119, 111, 114, 100, 
      0, 171, 171, 171,   0,   0, 
     19,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    163,   2,   0,   0,  83, 116, 
     97, 114, 116,  80, 105, 120, 
    101, 108,   0, 117, 105, 110, 
    116,  50,   0, 171, 171, 171, 
      1,   0,  19,   0,   1,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 219,   2,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  49, 
     48,  46,  49,   0,  73,  83, 
     71,  78,   8,   0,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  79,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     83,  72,  69,  88, 228,  16, 
      0,   0,  80,   0,   5,   0, 
     57,   4,   0,   0, 106,   8, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
     68,  68,   0,   0, 157,   0, 
      0,   3,   0, 224,  17,   0, 
      0,   0,   0,   0, 157,   0, 
      0,   3,   0, 224,  17,   0, 
      1,   0,   0,   0, 156,   8, 
      0,   4,   0, 224,  17,   0, 
      2,   0,   0,   0,  85,  85, 
      0,   0, 156,  24,   0,   4, 
      0, 224,  17,   0,   3,   0, 
      0,   0,  85,  85,   0,   0, 
     95,   0,   0,   2,   0,  64, 
      2,   0,  95,   0,   0,   2, 
     50,  16,   2,   0,  95,   0, 
      0,   2,  50,  32,   2,   0, 
     95,   0,   0,   2,  50,   0, 
      2,   0, 104,   0,   0,   2, 
      5,   0,   0,   0, 160,   0, 
      0,   5,   0, 240,  17,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0, 100,   0,   0,   0, 
    155,   0,   0,   4,   8,   0, 
      0,   0,   8,   0,   0,   0, 
      1,   0,   0,   0,  79,   0, 
      0,   6,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,  64, 
      2,   0,   1,  64,   0,   0, 
     50,   0,   0,   0,  31,   0, 
      4,   3,  10,   0,  16,   0, 
      0,   0,   0,   0,  78,   0, 
      0,   8,  18,   0,  16,   0, 
      0,   0,   0,   0,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,  64,   2,   0,   1,  64, 
      0,   0,  10,   0,   0,   0, 
     54,   0,   0,   5,  82,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    162,   0,  16,   0,   1,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  30,   0, 
      0,   8, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
    150, 137,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     41,   0,   0,   9, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  20,   2,   0,   2,  64, 
      0,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     30,   0,   0,   7, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  30,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0, 230,   4, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0, 255, 255, 
    255, 255,   4,   0,   0,   0, 
    255, 255, 255, 255, 255, 255, 
    255, 255,  54,   0,   0,   5, 
     50,   0,  16,   0,   1,   0, 
      0,   0, 230,  10,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   8, 194,   0,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     45,   0,   0, 137, 194,   0, 
      0, 128,   3,  17,  17,   0, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0, 150, 124, 
     16,   0,   0,   0,   0,   0, 
    140,   0,   0,  11, 130,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,  11,   0, 
      0,   0,   1,  64,   0,   0, 
      4,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0, 131,   0,   0,   5, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  85,   0, 
      0,  10, 194,   0,  16,   0, 
      1,   0,   0,   0, 166,  10, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      7,   0,   0,   0,  17,   0, 
      0,   0,   1,   0,   0,  10, 
    194,   0,  16,   0,   1,   0, 
      0,   0, 166,  14,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 240, 127, 
      0,   0, 224, 127,   0,   0, 
    131,   0,   0,   5,  98,   0, 
     16,   0,   2,   0,   0,   0, 
    166,  11,  16,   0,   1,   0, 
      0,   0,  16,   0,   0,  10, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0, 109, 198,  89,  62, 
    186,  20,  55,  63,  87, 205, 
    147,  61,   0,   0,   0,   0, 
     50,   0,   0,   9,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 112,  65,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     47,   0,   0,   5,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  62, 
    164,   0,   0,   7, 242, 224, 
     17,   0,   3,   0,   0,   0, 
     70,   5,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0, 168,   0, 
      0,   8,  18, 240,  17,   0, 
      0,   0,   0,   0,  10,  64, 
      2,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     30,   0,   0,   6,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,  64,   2,   0,   1,  64, 
      0,   0,  50,   0,   0,   0, 
     54,   0,   0,   8, 194,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  45,   0,   0, 137, 
    194,   0,   0, 128,   3,  17, 
     17,   0,  66,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
    150, 124,  16,   0,   0,   0, 
      0,   0, 140,   0,   0,  11, 
    130,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     11,   0,   0,   0,   1,  64, 
      0,   0,   4,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0, 131,   0, 
      0,   5,  18,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     85,   0,   0,  10, 194,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     17,   0,   0,   0,   1,   0, 
      0,  10, 194,   0,  16,   0, 
      0,   0,   0,   0, 166,  14, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    240, 127,   0,   0, 224, 127, 
      0,   0, 131,   0,   0,   5, 
     98,   0,  16,   0,   2,   0, 
      0,   0, 166,  11,  16,   0, 
      0,   0,   0,   0,  16,   0, 
      0,  10,  66,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0, 109, 198, 
     89,  62, 186,  20,  55,  63, 
     87, 205, 147,  61,   0,   0, 
      0,   0,  50,   0,   0,   9, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 112,  65, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  47,   0,   0,   5, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  62, 164,   0,   0,   7, 
    242, 224,  17,   0,   3,   0, 
      0,   0,  70,   5,  16,   0, 
      0,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
    168,   0,   0,   9,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1, 190,  24, 
      0,   1,  30,   0,   0,   6, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,  32,   2,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  35,   0,   0,   8, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  26,  32,   2,   0, 
      1,  64,   0,   0,  10,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0, 167,   0, 
      0,   9,  34,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0,  30,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,  10,   0, 
      0,   0,   9,   0,   0,   0, 
     11,   0,   0,   0,  20,   0, 
      0,   0, 167,   0,   0,   9, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0,  52,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     52,   0,   0,   7, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  52,   0, 
      0,   7, 130,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  52,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     51,   0,   0,   7, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  51,   0, 
      0,   7,  18,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  51,   0,   0,   7, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     51,   0,   0,   7, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   8,  66,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     49,   0,   0,   8, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,  42, 128,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  31,   0,   4,   3, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  62,   0,   0,   1, 
     21,   0,   0,   1,  30,   0, 
      0,   7,  98,   0,  16,   0, 
      2,   0,   0,   0,   6,   1, 
      2,   0,  86, 134,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  30,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0,  19,   0, 
      0,   0,  21,   0,   0,   0, 
    167,   0,   0,   9,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9, 130,   0, 
     16,   0,   1,   0,   0,   0, 
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
     86,   9,  16,   0,   0,   0, 
      0,   0,  86,   1,  16,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   7,  18,   0,  16,   0, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   0,   0,   0,   7, 
     34,   0,  16,   0,   4,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   3,   0,   0,   0, 
      0,   0,   0,   7,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  64,  10,   0, 
     16,   0,   4,   0,   0,   0, 
     50,   0,   0,  10,  34,   0, 
     16,   0,   3,   0,   0,   0, 
     58,   0,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  64,  42,   0,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  11, 130,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16, 128, 129,   0,   0,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  64, 
     58,   0,  16, 128, 129,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10,  34,   0, 
     16,   0,   3,   0,   0,   0, 
     10,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  64,  26,   0,  16,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   8, 130,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16, 128, 129,   0, 
      0,   0,   3,   0,   0,   0, 
     50,   0,   0,  10,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  64,  10,   0,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10,  34,   0,  16,   0, 
      3,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   1,  64, 
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
      0,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  64, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16, 128, 
    129,   0,   0,   0,   0,   0, 
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
      0,   0,   0,   0,  14,  32, 
      0,   8,  66,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16, 128, 129,   0,   0,   0, 
      3,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,   9,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0, 192,   1,  64, 
      0,   0,   0,   0,  64,  64, 
     56,   0,   0,   7,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   7,  66,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,  56,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     15,  32,   0,   8,  66,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0, 246, 143,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  29,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     55,   0,   0,   9,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     55,   0,   0,   9,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
      0,   0,   0,   8,  98,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  15,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
     86,   6,  16,   0,   0,   0, 
      0,   0,  29,   0,   0,   9, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16, 128, 
    129,   0,   0,   0,   0,   0, 
      0,   0,  26,   0,  16, 128, 
    129,   0,   0,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 126,  67, 
     28,   0,   0,   5,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0, 254,   0,   0,   0, 
     41,   0,   0,  10,  50,   0, 
     16,   0,   1,   0,   0,   0, 
    102,  10,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
     20,   0,   0,   0,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  60,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  30,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
    140,   0,   0,  11,  66,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  31,   0,   4,   3, 
     10,   0,  16,   0,   0,   0, 
      0,   0, 180,   0,   0,   9, 
     18,   0,  16,   0,   1,   0, 
      0,   0,   0, 224,  17,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  41,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   2,   0,   0,   0, 
    166,   0,   0,   7,  18, 224, 
     17,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0, 140,   0, 
      0,  11,  18,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     30,   0,   0,   7, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  30,   0, 
      0,  10,  50,   0,  16,   0, 
      3,   0,   0,   0, 214,   5, 
     16,   0,   2,   0,   0,   0, 
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
      3,  17,  17,   0,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0, 140,   0, 
      0,  11, 130,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,  11,   0,   0,   0, 
      1,  64,   0,   0,   4,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
    131,   0,   0,   5, 146,   0, 
     16,   0,   3,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  85,   0,   0,  10, 
    146,   0,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   7,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  17,   0,   0,   0, 
      1,   0,   0,  10, 146,   0, 
     16,   0,   0,   0,   0,   0, 
      6,  12,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
    240, 127,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    224, 127,   0,   0, 131,   0, 
      0,   5,  98,   0,  16,   0, 
      3,   0,   0,   0,   6,   3, 
     16,   0,   0,   0,   0,   0, 
    164,   0,   0,   7, 242, 224, 
     17,   0,   2,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  18,   0, 
      0,   1, 180,   0,   0,   9, 
     18,   0,  16,   0,   1,   0, 
      0,   0,   0, 224,  17,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  30,   0,   0,   9, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  10, 128,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  41,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   2,   0,   0,   0, 
    166,   0,   0,   7,  18, 224, 
     17,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0, 140,   0, 
      0,  11,  34,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     30,   0,   0,   7,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     26,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  30,   0, 
      0,  10,  50,   0,  16,   0, 
      1,   0,   0,   0, 134,   0, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   8, 
    194,   0,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  45,   0, 
      0, 137, 194,   0,   0, 128, 
      3,  17,  17,   0,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  22, 126,  16,   0, 
      0,   0,   0,   0, 140,   0, 
      0,  11,  66,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,  11,   0,   0,   0, 
      1,  64,   0,   0,   4,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
    131,   0,   0,   5, 146,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  85,   0,   0,  10, 
     98,   0,  16,   0,   0,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      7,   0,   0,   0,  17,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,  10,  98,   0, 
     16,   0,   0,   0,   0,   0, 
     86,   6,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0, 240, 127, 
      0,   0, 224, 127,   0,   0, 
      0,   0,   0,   0, 131,   0, 
      0,   5,  98,   0,  16,   0, 
      1,   0,   0,   0,  86,   6, 
     16,   0,   0,   0,   0,   0, 
    164,   0,   0,   7, 242, 224, 
     17,   0,   2,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  21,   0, 
      0,   1,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0, 136,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     47,   0,   0,   0,  19,   0, 
      0,   0,  11,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      7,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   2,   0, 
      0,   0,   6,   0,   0,   0, 
     82,  84,  83,  48, 188,   0, 
      0,   0,   2,   0,   0,   0, 
      3,   0,   0,   0,  24,   0, 
      0,   0,   1,   0,   0,   0, 
    136,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  60,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  72,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 104,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   7,   0, 
      0,   0,   1,   0,   0,   0, 
     80,   0,   0,   0,   1,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   1,   0, 
      0,   0, 112,   0,   0,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
     21,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 127, 127,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};
