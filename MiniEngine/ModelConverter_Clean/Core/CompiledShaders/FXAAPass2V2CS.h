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
//   float2 RcpTextureSize;             // Offset:    0 Size:     8
//   float ContrastThreshold;           // Offset:    8 Size:     4 [unused]
//   float SubpixelRemoval;             // Offset:   12 Size:     4 [unused]
//   uint LastQueueIndex;               // Offset:   16 Size:     4
//   uint2 StartPixel;                  // Offset:   20 Size:     8 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// LinearSampler                     sampler      NA          NA             s0      1 
// Luma                              texture   float          2d             t0      1 
// WorkQueue                         texture    byte         r/o             t1      1 
// ColorQueue                        texture  float3         buf             t2      1 
// DstColor                              UAV  float3          2d             u0      1 
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
dcl_immediateConstantBuffer { { 1.000000, 0, 0, 0},
                              { 2.500000, 0, 0, 0},
                              { 4.500000, 0, 0, 0},
                              { 6.500000, 0, 0, 0},
                              { 8.500000, 0, 0, 0},
                              { 10.500000, 0, 0, 0},
                              { 14.500000, 0, 0, 0},
                              { 22.500000, 0, 0, 0} }
dcl_constantbuffer CB0[2], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_raw t1
dcl_resource_buffer (float,float,float,float) t2
dcl_uav_typed_texture2d (float,float,float,float) u0
dcl_input vThreadID.x
dcl_temps 7
dcl_thread_group 64, 1, 1
iadd r0.x, -vThreadID.x, cb0[1].x
ishl r0.y, r0.x, l(2)
ld_raw_indexable(raw_buffer)(mixed,mixed,mixed,mixed) r0.y, r0.y, t1.xxxx
ushr r0.zw, r0.yyyy, l(0, 0, 8, 20)
and r1.yz, r0.zzwz, l(0, 4095, 4095, 0)
and r0.zw, r0.yyyy, l(0, 0, 254, 1)
utof r2.xy, r0.zwzz
mul r0.z, r2.x, l(0.001969)
bfi r0.y, l(1), l(1), r0.y, l(0)
iadd r1.x, r1.y, r0.y
iadd r3.xy, r1.xzxx, l(-1, 0, 0, 0)
mov r3.zw, l(0,0,0,0)
ld_indexable(texture2d)(float,float,float,float) r0.y, r3.xyzw, t0.yxzw
utof r2.xw, r1.yyyz
mov r2.z, l(0.500000)
add r2.xy, r2.yzyy, r2.xwxx
mov r1.w, l(0)
ld_indexable(texture2d)(float,float,float,float) r0.w, r1.yzww, t0.yzwx
add r1.x, r0.w, r0.y
add r0.y, -r0.w, r0.y
lt r0.w, l(0.000000), r0.y
lt r1.w, r0.y, l(0.000000)
iadd r0.w, -r0.w, r1.w
itof r0.w, r0.w
mul r0.y, |r0.y|, l(0.250000)
mov r3.x, l(0)
mov r3.y, cb0[0].y
mov r4.xy, l(22.500000,0,0,0)
mov r5.xy, l(22.500000,0,0,0)
mov r1.w, l(0)
loop 
  uge r2.z, r1.w, l(7)
  breakc_nz r2.z
  mul r2.zw, r3.xxxy, icb[r1.w + 0].xxxx
  mad r3.zw, r2.xxxy, cb0[0].xxxy, -r2.zzzw
  mad r2.zw, r2.xxxy, cb0[0].xxxy, r2.zzzw
  sample_l_indexable(texture2d)(float,float,float,float) r3.z, r3.zwzz, t0.yzxw, s0, l(0.000000)
  mad r3.z, -r1.x, l(0.500000), r3.z
  ge r3.w, |r3.z|, r0.y
  lt r4.z, icb[r1.w + 0].x, r4.x
  and r3.w, r3.w, r4.z
  lt r4.z, l(0.000000), r3.z
  lt r3.z, r3.z, l(0.000000)
  iadd r3.z, -r4.z, r3.z
  itof r3.z, r3.z
  eq r6.y, r0.w, r3.z
  mov r6.xz, icb[r1.w + 0].xxxx
  movc r4.xy, r3.wwww, r6.xyxx, r4.xyxx
  sample_l_indexable(texture2d)(float,float,float,float) r2.z, r2.zwzz, t0.yzxw, s0, l(0.000000)
  mad r2.z, -r1.x, l(0.500000), r2.z
  ge r2.w, |r2.z|, r0.y
  lt r3.z, icb[r1.w + 0].x, r5.x
  and r2.w, r2.w, r3.z
  lt r3.z, l(0.000000), r2.z
  lt r2.z, r2.z, l(0.000000)
  iadd r2.z, -r3.z, r2.z
  itof r2.z, r2.z
  eq r6.w, r0.w, r2.z
  movc r5.xy, r2.wwww, r6.zwzz, r5.xyxx
  iadd r1.w, r1.w, l(1)
endloop 
min r0.y, r4.x, r5.x
add r0.w, r4.x, r5.x
div r0.y, r0.y, r0.w
add r0.y, -r0.y, l(0.500000)
lt r0.w, r4.x, r5.x
movc r0.w, r0.w, r4.y, r5.y
and r0.y, r0.y, r0.w
max r0.y, r0.y, r0.z
lt r0.z, l(0.010000), r0.y
if_nz r0.z
  ld_uav_typed_indexable(texture2d)(float,float,float,float) r2.xyz, r1.yzzz, u0.xyzw
  ld_indexable(buffer)(float,float,float,float) r0.xzw, r0.xxxx, t2.xwyz
  add r3.xyzw, -r2.xyzx, r0.xzwx
  mad r0.xyzw, r0.yyyy, r3.xyzw, r2.xyzx
  store_uav_typed u0.xyzw, r1.yzzz, r0.xyzw
endif 
ret 
// Approximately 78 instruction slots used
#endif

const BYTE g_pFXAAPass2V2CS[] =
{
     68,  88,  66,  67, 215,  49, 
    234, 116, 182,  48, 120, 131, 
     12,  69,  91, 183,  41, 175, 
    144, 151,   1,   0,   0,   0, 
      4,  15,   0,   0,   7,   0, 
      0,   0,  60,   0,   0,   0, 
    124,   3,   0,   0, 140,   3, 
      0,   0, 156,   3,   0,   0, 
    148,  13,   0,   0, 164,  13, 
      0,   0,  64,  14,   0,   0, 
     82,  68,  69,  70,  56,   3, 
      0,   0,   1,   0,   0,   0, 
     52,   1,   0,   0,   6,   0, 
      0,   0,  60,   0,   0,   0, 
      0,   5,  83,  67,   0,   1, 
      0,   0,  16,   3,   0,   0, 
     82,  68,  49,  49,  60,   0, 
      0,   0,  24,   0,   0,   0, 
     32,   0,   0,   0,  40,   0, 
      0,   0,  36,   0,   0,   0, 
     12,   0,   0,   0,   0,   0, 
      0,   0, 252,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  10,   1, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     15,   1,   0,   0,   7,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  25,   1,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,   0,   0, 
    255, 255, 255, 255,   2,   0, 
      0,   0,   1,   0,   0,   0, 
      9,   0,   0,   0,  36,   1, 
      0,   0,   4,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   9,   0,   0,   0, 
     45,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  76, 105, 110, 101, 
     97, 114,  83,  97, 109, 112, 
    108, 101, 114,   0,  76, 117, 
    109,  97,   0,  87, 111, 114, 
    107,  81, 117, 101, 117, 101, 
      0,  67, 111, 108, 111, 114, 
     81, 117, 101, 117, 101,   0, 
     68, 115, 116,  67, 111, 108, 
    111, 114,   0,  67,  66,  48, 
      0, 171, 171, 171,  45,   1, 
      0,   0,   5,   0,   0,   0, 
     76,   1,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  20,   2, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,   2,   0, 
      0,   0,  44,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  80,   2,   0,   0, 
      8,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    104,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    140,   2,   0,   0,  12,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 104,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 156,   2, 
      0,   0,  16,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 180,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 216,   2,   0,   0, 
     20,   0,   0,   0,   8,   0, 
      0,   0,   0,   0,   0,   0, 
    236,   2,   0,   0,   0,   0, 
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
      0,   0,  35,   2,   0,   0, 
     67, 111, 110, 116, 114,  97, 
    115, 116,  84, 104, 114, 101, 
    115, 104, 111, 108, 100,   0, 
    102, 108, 111,  97, 116,   0, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  98,   2,   0,   0, 
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
    171,   2,   0,   0,  83, 116, 
     97, 114, 116,  80, 105, 120, 
    101, 108,   0, 117, 105, 110, 
    116,  50,   0, 171, 171, 171, 
      1,   0,  19,   0,   1,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 227,   2,   0,   0, 
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
     83,  72,  69,  88, 240,   9, 
      0,   0,  80,   0,   5,   0, 
    124,   2,   0,   0, 106,   8, 
      0,   1,  53,  24,   0,   0, 
     34,   0,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  32,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 144,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    208,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   8,  65, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  40,  65,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    104,  65,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 180,  65, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  90,   0, 
      0,   3,   0,  96,  16,   0, 
      0,   0,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  85,  85, 
      0,   0, 161,   0,   0,   3, 
      0, 112,  16,   0,   1,   0, 
      0,   0,  88,   8,   0,   4, 
      0, 112,  16,   0,   2,   0, 
      0,   0,  85,  85,   0,   0, 
    156,  24,   0,   4,   0, 224, 
     17,   0,   0,   0,   0,   0, 
     85,  85,   0,   0,  95,   0, 
      0,   2,  18,   0,   2,   0, 
    104,   0,   0,   2,   7,   0, 
      0,   0, 155,   0,   0,   4, 
     64,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     30,   0,   0,   8,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,   2, 128,  65,   0, 
      0,   0,  10, 128,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  41,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   2,   0,   0,   0, 
    165,   0,   0, 137, 194,   2, 
      0, 128, 131, 153,  25,   0, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   6, 112, 
     16,   0,   1,   0,   0,   0, 
     85,   0,   0,  10, 194,   0, 
     16,   0,   0,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     20,   0,   0,   0,   1,   0, 
      0,  10,  98,   0,  16,   0, 
      1,   0,   0,   0, 166,  11, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0, 255,  15,   0,   0, 
    255,  15,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,  10, 
    194,   0,  16,   0,   0,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 254,   0, 
      0,   0,   1,   0,   0,   0, 
     86,   0,   0,   5,  50,   0, 
     16,   0,   2,   0,   0,   0, 
    230,  10,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   4,   2,   1,  59, 
    140,   0,   0,  11,  34,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     30,   0,   0,  10,  50,   0, 
     16,   0,   3,   0,   0,   0, 
    134,   0,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   8, 194,   0,  16,   0, 
      3,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     45,   0,   0, 137, 194,   0, 
      0, 128,  67,  85,  21,   0, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  22, 126, 
     16,   0,   0,   0,   0,   0, 
     86,   0,   0,   5, 146,   0, 
     16,   0,   2,   0,   0,   0, 
     86,   9,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
     66,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  63,   0,   0, 
      0,   7,  50,   0,  16,   0, 
      2,   0,   0,   0, 150,   5, 
     16,   0,   2,   0,   0,   0, 
    198,   0,  16,   0,   2,   0, 
      0,   0,  54,   0,   0,   5, 
    130,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  45,   0, 
      0, 137, 194,   0,   0, 128, 
     67,  85,  21,   0, 130,   0, 
     16,   0,   0,   0,   0,   0, 
    150,  15,  16,   0,   1,   0, 
      0,   0, 150, 115,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   7,  18,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   8, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  49,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  49,   0,   0,   7, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     30,   0,   0,   8, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  43,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16, 128, 129,   0,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  62, 
     54,   0,   0,   5,  18,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   6, 
     34,   0,  16,   0,   3,   0, 
      0,   0,  26, 128,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   8, 
     50,   0,  16,   0,   4,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 180,  65,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   8,  50,   0,  16,   0, 
      5,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 180,  65, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 130,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  48,   0,   0,   1, 
     80,   0,   0,   7,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      7,   0,   0,   0,   3,   0, 
      4,   3,  42,   0,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   8, 194,   0,  16,   0, 
      2,   0,   0,   0,   6,   4, 
     16,   0,   3,   0,   0,   0, 
      6, 144, 144,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  11, 194,   0, 
     16,   0,   3,   0,   0,   0, 
      6,   4,  16,   0,   2,   0, 
      0,   0,   6, 132,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 166,  14,  16, 128, 
     65,   0,   0,   0,   2,   0, 
      0,   0,  50,   0,   0,  10, 
    194,   0,  16,   0,   2,   0, 
      0,   0,   6,   4,  16,   0, 
      2,   0,   0,   0,   6, 132, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 166,  14, 
     16,   0,   2,   0,   0,   0, 
     72,   0,   0, 141, 194,   0, 
      0, 128,  67,  85,  21,   0, 
     66,   0,  16,   0,   3,   0, 
      0,   0, 230,  10,  16,   0, 
      3,   0,   0,   0, 150, 124, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10,  66,   0,  16,   0, 
      3,   0,   0,   0,  10,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  63, 
     42,   0,  16,   0,   3,   0, 
      0,   0,  29,   0,   0,   8, 
    130,   0,  16,   0,   3,   0, 
      0,   0,  42,   0,  16, 128, 
    129,   0,   0,   0,   3,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  49,   0, 
      0,   8,  66,   0,  16,   0, 
      4,   0,   0,   0,  10, 144, 
    144,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   4,   0,   0,   0, 
      1,   0,   0,   7, 130,   0, 
     16,   0,   3,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,  42,   0,  16,   0, 
      4,   0,   0,   0,  49,   0, 
      0,   7,  66,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   3,   0, 
      0,   0,  49,   0,   0,   7, 
     66,   0,  16,   0,   3,   0, 
      0,   0,  42,   0,  16,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     30,   0,   0,   8,  66,   0, 
     16,   0,   3,   0,   0,   0, 
     42,   0,  16, 128,  65,   0, 
      0,   0,   4,   0,   0,   0, 
     42,   0,  16,   0,   3,   0, 
      0,   0,  43,   0,   0,   5, 
     66,   0,  16,   0,   3,   0, 
      0,   0,  42,   0,  16,   0, 
      3,   0,   0,   0,  24,   0, 
      0,   7,  34,   0,  16,   0, 
      6,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   3,   0, 
      0,   0,  54,   0,   0,   6, 
     82,   0,  16,   0,   6,   0, 
      0,   0,   6, 144, 144,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  55,   0,   0,   9, 
     50,   0,  16,   0,   4,   0, 
      0,   0, 246,  15,  16,   0, 
      3,   0,   0,   0,  70,   0, 
     16,   0,   6,   0,   0,   0, 
     70,   0,  16,   0,   4,   0, 
      0,   0,  72,   0,   0, 141, 
    194,   0,   0, 128,  67,  85, 
     21,   0,  66,   0,  16,   0, 
      2,   0,   0,   0, 230,  10, 
     16,   0,   2,   0,   0,   0, 
    150, 124,  16,   0,   0,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     50,   0,   0,  10,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  63,  42,   0,  16,   0, 
      2,   0,   0,   0,  29,   0, 
      0,   8, 130,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16, 128, 129,   0,   0,   0, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     49,   0,   0,   8,  66,   0, 
     16,   0,   3,   0,   0,   0, 
     10, 144, 144,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   5,   0, 
      0,   0,   1,   0,   0,   7, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   3,   0,   0,   0, 
     49,   0,   0,   7,  66,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  49,   0, 
      0,   7,  66,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  30,   0,   0,   8, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16, 128, 
     65,   0,   0,   0,   3,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  43,   0, 
      0,   5,  66,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
     24,   0,   0,   7, 130,   0, 
     16,   0,   6,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  55,   0, 
      0,   9,  50,   0,  16,   0, 
      5,   0,   0,   0, 246,  15, 
     16,   0,   2,   0,   0,   0, 
    230,  10,  16,   0,   6,   0, 
      0,   0,  70,   0,  16,   0, 
      5,   0,   0,   0,  30,   0, 
      0,   7, 130,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  22,   0,   0,   1, 
     51,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   4,   0, 
      0,   0,  10,   0,  16,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   4,   0,   0,   0, 
     10,   0,  16,   0,   5,   0, 
      0,   0,  14,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   8,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  63,  49,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   5,   0,   0,   0, 
     55,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      4,   0,   0,   0,  26,   0, 
     16,   0,   5,   0,   0,   0, 
      1,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  52,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  49,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     10, 215,  35,  60,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     31,   0,   4,   3,  42,   0, 
     16,   0,   0,   0,   0,   0, 
    163,   0,   0, 137, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    114,   0,  16,   0,   2,   0, 
      0,   0, 150,  10,  16,   0, 
      1,   0,   0,   0,  70, 238, 
     17,   0,   0,   0,   0,   0, 
     45,   0,   0, 137,  66,   0, 
      0, 128,  67,  85,  21,   0, 
    210,   0,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0, 198, 121, 
     16,   0,   2,   0,   0,   0, 
      0,   0,   0,   8, 242,   0, 
     16,   0,   3,   0,   0,   0, 
     70,   2,  16, 128,  65,   0, 
      0,   0,   2,   0,   0,   0, 
    134,   3,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0, 164,   0,   0,   7, 
    242, 224,  17,   0,   0,   0, 
      0,   0, 150,  10,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1,  62,   0, 
      0,   1,  83,  70,  73,  48, 
      8,   0,   0,   0,   0,   8, 
      0,   0,   0,   0,   0,   0, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  78,   0,   0,   0, 
      7,   0,   0,   0,   8,   0, 
      0,   0,   1,   0,   0,   0, 
     31,   0,   0,   0,   8,   0, 
      0,   0,   7,   0,   0,   0, 
      1,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      9,   0,   0,   0,   3,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
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
