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
dcl_uav_typed_texture2d (float,float,float,float) u0
dcl_input vThreadID.x
dcl_temps 6
dcl_thread_group 64, 1, 1
iadd r0.x, -vThreadID.x, cb0[1].x
ishl r0.x, r0.x, l(2)
ld_raw_indexable(raw_buffer)(mixed,mixed,mixed,mixed) r0.x, r0.x, t1.xxxx
ushr r0.yz, r0.xxxx, l(0, 8, 20, 0)
and r1.yz, r0.yyzy, l(0, 4095, 4095, 0)
and r0.yz, r0.xxxx, l(0, 254, 1, 0)
utof r2.xy, r0.yzyy
mul r0.y, r2.x, l(0.001969)
bfi r0.x, l(1), l(1), r0.x, l(0)
iadd r1.x, r1.y, r0.x
iadd r3.xy, r1.xzxx, l(-1, 0, 0, 0)
mov r3.zw, l(0,0,0,0)
ld_indexable(texture2d)(float,float,float,float) r0.x, r3.xyzw, t0.xyzw
utof r0.zw, r1.yyyz
mov r2.z, l(0.500000)
add r0.zw, r0.zzzw, r2.yyyz
mov r1.w, l(0)
ld_indexable(texture2d)(float,float,float,float) r1.x, r1.yzww, t0.xyzw
add r1.w, r0.x, r1.x
add r0.x, r0.x, -r1.x
lt r1.x, l(0.000000), r0.x
lt r2.x, r0.x, l(0.000000)
iadd r1.x, -r1.x, r2.x
itof r1.x, r1.x
mul r0.x, |r0.x|, l(0.250000)
mov r2.x, l(0)
mov r2.y, cb0[0].y
mov r3.xy, l(22.500000,0,0,0)
mov r4.xy, l(22.500000,0,0,0)
mov r2.z, l(0)
loop 
  uge r2.w, r2.z, l(7)
  breakc_nz r2.w
  mul r3.zw, r2.xxxy, icb[r2.z + 0].xxxx
  mad r4.zw, r0.zzzw, cb0[0].xxxy, -r3.zzzw
  mad r3.zw, r0.zzzw, cb0[0].xxxy, r3.zzzw
  sample_l_indexable(texture2d)(float,float,float,float) r2.w, r4.zwzz, t0.yzwx, s0, l(0.000000)
  mad r2.w, -r1.w, l(0.500000), r2.w
  ge r4.z, |r2.w|, r0.x
  lt r4.w, icb[r2.z + 0].x, r3.x
  and r4.z, r4.w, r4.z
  lt r4.w, l(0.000000), r2.w
  lt r2.w, r2.w, l(0.000000)
  iadd r2.w, -r4.w, r2.w
  itof r2.w, r2.w
  eq r5.y, r1.x, r2.w
  mov r5.xz, icb[r2.z + 0].xxxx
  movc r3.xy, r4.zzzz, r5.xyxx, r3.xyxx
  sample_l_indexable(texture2d)(float,float,float,float) r2.w, r3.zwzz, t0.yzwx, s0, l(0.000000)
  mad r2.w, -r1.w, l(0.500000), r2.w
  ge r3.z, |r2.w|, r0.x
  lt r3.w, icb[r2.z + 0].x, r4.x
  and r3.z, r3.w, r3.z
  lt r3.w, l(0.000000), r2.w
  lt r2.w, r2.w, l(0.000000)
  iadd r2.w, -r3.w, r2.w
  itof r2.w, r2.w
  eq r5.w, r1.x, r2.w
  movc r4.xy, r3.zzzz, r5.zwzz, r4.xyxx
  iadd r2.z, r2.z, l(1)
endloop 
min r0.x, r3.x, r4.x
add r0.z, r3.x, r4.x
div r0.x, r0.x, r0.z
add r0.x, -r0.x, l(0.500000)
lt r0.z, r3.x, r4.x
movc r0.z, r0.z, r3.y, r4.y
and r0.x, r0.x, r0.z
max r0.x, r0.x, r0.y
lt r0.y, l(0.010000), r0.x
if_nz r0.y
  add r2.x, r0.x, r0.x
  mad r2.y, -r0.x, l(2.000000), l(1.000000)
  mov r2.z, l(0)
else 
  mov r2.xyz, l(0,0,0.250000,0)
endif 
store_uav_typed u0.xyzw, r1.yzzz, r2.xyzz
ret 
// Approximately 79 instruction slots used
#endif

const BYTE g_pFXAAPass2VDebug2CS[] =
{
     68,  88,  66,  67,  80, 117, 
     62,  67, 179, 131, 210,  68, 
     42, 148, 197, 196, 106,  46, 
    248, 165,   1,   0,   0,   0, 
    164,  14,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
     76,   3,   0,   0,  92,   3, 
      0,   0, 108,   3,   0,   0, 
     68,  13,   0,   0, 224,  13, 
      0,   0,  82,  68,  69,  70, 
     12,   3,   0,   0,   1,   0, 
      0,   0,   8,   1,   0,   0, 
      5,   0,   0,   0,  60,   0, 
      0,   0,   0,   5,  83,  67, 
      0,   1,   0,   0, 228,   2, 
      0,   0,  82,  68,  49,  49, 
     60,   0,   0,   0,  24,   0, 
      0,   0,  32,   0,   0,   0, 
     40,   0,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0, 220,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    234,   0,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 239,   0,   0,   0, 
      7,   0,   0,   0,   6,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 249,   0, 
      0,   0,   4,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   9,   0,   0,   0, 
      2,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  76, 105, 110, 101, 
     97, 114,  83,  97, 109, 112, 
    108, 101, 114,   0,  76, 117, 
    109,  97,   0,  87, 111, 114, 
    107,  81, 117, 101, 117, 101, 
      0,  68, 115, 116,  67, 111, 
    108, 111, 114,   0,  67,  66, 
     48,   0, 171, 171,   2,   1, 
      0,   0,   5,   0,   0,   0, 
     32,   1,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 232,   1, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,   2,   0, 
      0,   0,   0,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  36,   2,   0,   0, 
      8,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     60,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     96,   2,   0,   0,  12,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  60,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 112,   2, 
      0,   0,  16,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 136,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 172,   2,   0,   0, 
     20,   0,   0,   0,   8,   0, 
      0,   0,   0,   0,   0,   0, 
    192,   2,   0,   0,   0,   0, 
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
      0,   0, 247,   1,   0,   0, 
     67, 111, 110, 116, 114,  97, 
    115, 116,  84, 104, 114, 101, 
    115, 104, 111, 108, 100,   0, 
    102, 108, 111,  97, 116,   0, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   2,   0,   0, 
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
    127,   2,   0,   0,  83, 116, 
     97, 114, 116,  80, 105, 120, 
    101, 108,   0, 117, 105, 110, 
    116,  50,   0, 171, 171, 171, 
      1,   0,  19,   0,   1,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 183,   2,   0,   0, 
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
     83,  72,  69,  88, 208,   9, 
      0,   0,  80,   0,   5,   0, 
    116,   2,   0,   0, 106,   8, 
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
      0,   0, 156,  24,   0,   4, 
      0, 224,  17,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
     95,   0,   0,   2,  18,   0, 
      2,   0, 104,   0,   0,   2, 
      6,   0,   0,   0, 155,   0, 
      0,   4,  64,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  30,   0,   0,   8, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,   2, 128, 
     65,   0,   0,   0,  10, 128, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  41,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   2,   0, 
      0,   0, 165,   0,   0, 137, 
    194,   2,   0, 128, 131, 153, 
     25,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      6, 112,  16,   0,   1,   0, 
      0,   0,  85,   0,   0,  10, 
     98,   0,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  20,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,  10,  98,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   6,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0, 255,  15, 
      0,   0, 255,  15,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,  10,  98,   0,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0, 254,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  86,   0,   0,   5, 
     50,   0,  16,   0,   2,   0, 
      0,   0, 150,   5,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   4,   2, 
      1,  59, 140,   0,   0,  11, 
     18,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  30,   0, 
      0,   7,  18,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  30,   0,   0,  10, 
     50,   0,  16,   0,   3,   0, 
      0,   0, 134,   0,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   8, 194,   0, 
     16,   0,   3,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  45,   0,   0, 137, 
    194,   0,   0, 128,  67,  85, 
     21,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0,  86,   0,   0,   5, 
    194,   0,  16,   0,   0,   0, 
      0,   0,  86,   9,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5,  66,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  63, 
      0,   0,   0,   7, 194,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  14,  16,   0,   0,   0, 
      0,   0,  86,   9,  16,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   5, 130,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     45,   0,   0, 137, 194,   0, 
      0, 128,  67,  85,  21,   0, 
     18,   0,  16,   0,   1,   0, 
      0,   0, 150,  15,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   7, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   8,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
     49,   0,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  49,   0, 
      0,   7,  18,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  30,   0,   0,   8, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,  43,   0, 
      0,   5,  18,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   8,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16, 128, 129,   0, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  62,  54,   0,   0,   5, 
     18,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   6,  34,   0,  16,   0, 
      2,   0,   0,   0,  26, 128, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   8,  50,   0,  16,   0, 
      3,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 180,  65, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   8,  50,   0, 
     16,   0,   4,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    180,  65,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     66,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  48,   0, 
      0,   1,  80,   0,   0,   7, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   7,   0,   0,   0, 
      3,   0,   4,   3,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     56,   0,   0,   8, 194,   0, 
     16,   0,   3,   0,   0,   0, 
      6,   4,  16,   0,   2,   0, 
      0,   0,   6, 144, 144,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  11, 
    194,   0,  16,   0,   4,   0, 
      0,   0, 166,  14,  16,   0, 
      0,   0,   0,   0,   6, 132, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 166,  14, 
     16, 128,  65,   0,   0,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10, 194,   0,  16,   0, 
      3,   0,   0,   0, 166,  14, 
     16,   0,   0,   0,   0,   0, 
      6, 132,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    166,  14,  16,   0,   3,   0, 
      0,   0,  72,   0,   0, 141, 
    194,   0,   0, 128,  67,  85, 
     21,   0, 130,   0,  16,   0, 
      2,   0,   0,   0, 230,  10, 
     16,   0,   4,   0,   0,   0, 
    150, 115,  16,   0,   0,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     50,   0,   0,  10, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  63,  58,   0,  16,   0, 
      2,   0,   0,   0,  29,   0, 
      0,   8,  66,   0,  16,   0, 
      4,   0,   0,   0,  58,   0, 
     16, 128, 129,   0,   0,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     49,   0,   0,   8, 130,   0, 
     16,   0,   4,   0,   0,   0, 
     10, 144, 144,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   1,   0,   0,   7, 
     66,   0,  16,   0,   4,   0, 
      0,   0,  58,   0,  16,   0, 
      4,   0,   0,   0,  42,   0, 
     16,   0,   4,   0,   0,   0, 
     49,   0,   0,   7, 130,   0, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  49,   0, 
      0,   7, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  30,   0,   0,   8, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16, 128, 
     65,   0,   0,   0,   4,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  43,   0, 
      0,   5, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     24,   0,   0,   7,  34,   0, 
     16,   0,   5,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   6,  82,   0,  16,   0, 
      5,   0,   0,   0,   6, 144, 
    144,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  55,   0, 
      0,   9,  50,   0,  16,   0, 
      3,   0,   0,   0, 166,  10, 
     16,   0,   4,   0,   0,   0, 
     70,   0,  16,   0,   5,   0, 
      0,   0,  70,   0,  16,   0, 
      3,   0,   0,   0,  72,   0, 
      0, 141, 194,   0,   0, 128, 
     67,  85,  21,   0, 130,   0, 
     16,   0,   2,   0,   0,   0, 
    230,  10,  16,   0,   3,   0, 
      0,   0, 150, 115,  16,   0, 
      0,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  63,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     29,   0,   0,   8,  66,   0, 
     16,   0,   3,   0,   0,   0, 
     58,   0,  16, 128, 129,   0, 
      0,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  49,   0,   0,   8, 
    130,   0,  16,   0,   3,   0, 
      0,   0,  10, 144, 144,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      4,   0,   0,   0,   1,   0, 
      0,   7,  66,   0,  16,   0, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   3,   0,   0,   0, 
     42,   0,  16,   0,   3,   0, 
      0,   0,  49,   0,   0,   7, 
    130,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     49,   0,   0,   7, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  30,   0, 
      0,   8, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      3,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     43,   0,   0,   5, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,  24,   0,   0,   7, 
    130,   0,  16,   0,   5,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     55,   0,   0,   9,  50,   0, 
     16,   0,   4,   0,   0,   0, 
    166,  10,  16,   0,   3,   0, 
      0,   0, 230,  10,  16,   0, 
      5,   0,   0,   0,  70,   0, 
     16,   0,   4,   0,   0,   0, 
     30,   0,   0,   7,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,  22,   0, 
      0,   1,  51,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   4,   0,   0,   0, 
      0,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,  16,   0, 
      4,   0,   0,   0,  14,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  63,  49,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     10,   0,  16,   0,   4,   0, 
      0,   0,  55,   0,   0,   9, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,   1,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     52,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  49,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,  10, 215,  35,  60, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  31,   0,   4,   3, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   7, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  10,  34,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  64,   1,  64,   0,   0, 
      0,   0, 128,  63,  54,   0, 
      0,   5,  66,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     18,   0,   0,   1,  54,   0, 
      0,   8, 114,   0,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128,  62,   0,   0,   0,   0, 
     21,   0,   0,   1, 164,   0, 
      0,   7, 242, 224,  17,   0, 
      0,   0,   0,   0, 150,  10, 
     16,   0,   1,   0,   0,   0, 
     70,  10,  16,   0,   2,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  79,   0,   0,   0, 
      6,   0,   0,   0,   8,   0, 
      0,   0,   1,   0,   0,   0, 
     31,   0,   0,   0,   8,   0, 
      0,   0,   7,   0,   0,   0, 
      2,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     11,   0,   0,   0,   3,   0, 
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
