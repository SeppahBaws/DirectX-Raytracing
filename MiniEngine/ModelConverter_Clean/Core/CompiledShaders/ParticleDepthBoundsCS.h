#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer CBChangesPerView
// {
//
//   float4x4 gInvView;                 // Offset:    0 Size:    64 [unused]
//   float4x4 gViewProj;                // Offset:   64 Size:    64 [unused]
//   float gVertCotangent;              // Offset:  128 Size:     4 [unused]
//   float gAspectRatio;                // Offset:  132 Size:     4 [unused]
//   float gRcpFarZ;                    // Offset:  136 Size:     4 [unused]
//   float gInvertZ;                    // Offset:  140 Size:     4 [unused]
//   float2 gBufferDim;                 // Offset:  144 Size:     8 [unused]
//   float2 gRcpBufferDim;              // Offset:  152 Size:     8
//   uint gBinsPerRow;                  // Offset:  160 Size:     4 [unused]
//   uint gTileRowPitch;                // Offset:  164 Size:     4 [unused]
//   uint gTilesPerRow;                 // Offset:  168 Size:     4 [unused]
//   uint gTilesPerCol;                 // Offset:  172 Size:     4 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// gSampPointClamp                   sampler      NA          NA             s2      1 
// g_Input                           texture   float          2d             t0      1 
// g_Output8                             UAV    uint          2d             u0      1 
// g_Output16                            UAV    uint          2d             u1      1 
// g_Output32                            UAV    uint          2d             u2      1 
// CBChangesPerView                  cbuffer      NA          NA            cb1      1 
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
dcl_constantbuffer CB1[10], immediateIndexed
dcl_sampler s2, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_uav_typed_texture2d (uint,uint,uint,uint) u0
dcl_uav_typed_texture2d (uint,uint,uint,uint) u1
dcl_uav_typed_texture2d (uint,uint,uint,uint) u2
dcl_input vThreadIDInGroupFlattened
dcl_input vThreadGroupID.xy
dcl_input vThreadID.xy
dcl_temps 6
dcl_tgsm_structured g0, 4, 128
dcl_thread_group 8, 8, 1
imad r0.xy, vThreadID.xyxx, l(4, 4, 0, 0), l(1, 1, 0, 0)
utof r0.xy, r0.xyxx
mul r0.xy, r0.xyxx, cb1[9].zwzz
mad r1.xyzw, cb1[9].zwzw, l(2.000000, 0.000000, 0.000000, 2.000000), r0.xyxy
mad r0.zw, cb1[9].zzzw, l(0.000000, 0.000000, 2.000000, 2.000000), r0.xxxy
gather4_indexable(texture2d)(float,float,float,float) r2.xyzw, r0.xyxx, t0.xyzw, s2.x
gather4_indexable(texture2d)(float,float,float,float) r3.xyzw, r1.xyxx, t0.xyzw, s2.x
gather4_indexable(texture2d)(float,float,float,float) r1.xyzw, r1.zwzz, t0.xyzw, s2.x
gather4_indexable(texture2d)(float,float,float,float) r0.xyzw, r0.zwzz, t0.xyzw, s2.x
max r4.xyzw, r2.xyzw, r3.xyzw
max r5.xyzw, r0.xyzw, r1.xyzw
max r4.xyzw, r4.xyzw, r5.xyzw
min r2.xyzw, r2.xyzw, r3.xyzw
min r0.xyzw, r0.xyzw, r1.xyzw
min r0.xyzw, r0.xyzw, r2.xyzw
max r1.xy, r4.ywyy, r4.xzxx
max r1.x, r1.y, r1.x
min r0.xy, r0.ywyy, r0.xzxx
min r0.x, r0.y, r0.x
store_structured g0.x, vThreadIDInGroupFlattened.x, l(0), r1.x
bfi r0.yz, l(0, 6, 31, 0), l(0, 0, 1, 0), vThreadIDInGroupFlattened.xxxx, l(0, 64, 1, 0)
not r0.x, r0.x
store_structured g0.x, r0.y, l(0), r0.x
sync_g_t
ishl r0.x, vThreadIDInGroupFlattened.x, l(1)
ld_structured r0.z, r0.z, l(0), g0.xxxx
iadd r1.xyzw, r0.xxxx, l(8, 9, 2, 16)
ld_structured r0.w, r1.x, l(0), g0.xxxx
ld_structured r1.x, r1.y, l(0), g0.xxxx
sync_g_t
umax r0.w, r0.w, r1.x
umax r0.z, r0.w, r0.z
mov r0.y, l(0)
atomic_umax g0, r0.xyxx, r0.z
sync_g_t
and r0.zw, r0.xxxx, l(0, 0, 72, 90)
if_z r0.z
  ubfe r2.xyzw, l(31, 28, 28, 28), l(0, 3, 3, 3), vThreadIDInGroupFlattened.xxxx
  bfi r2.xyzw, l(30, 30, 30, 30), l(2, 2, 2, 2), vThreadGroupID.xyyy, r2.xyzw
  iadd r0.z, r0.x, l(64)
  ld_structured r0.z, r0.z, l(0), g0.xxxx
  not r0.z, r0.z
  ld_structured r1.x, r0.x, l(0), g0.xxxx
  f32tof16 r1.x, r1.x
  add_sat r0.z, r0.z, l(-0.001000)
  f32tof16 r0.z, r0.z
  imad r0.z, r1.x, l(0x00010000), r0.z
  store_uav_typed u0.xyzw, r2.xyzw, r0.zzzz
endif 
ld_structured r0.z, r1.z, l(0), g0.xxxx
ld_structured r1.x, r1.w, l(0), g0.xxxx
iadd r2.xyzw, r0.xxxx, l(18, 4, 32, 36)
ld_structured r1.y, r2.x, l(0), g0.xxxx
sync_g_t
umax r1.x, r1.y, r1.x
umax r0.z, r0.z, r1.x
atomic_umax g0, r0.xyxx, r0.z
sync_g_t
if_z r0.w
  ubfe r1.xyzw, l(30, 27, 27, 27), l(1, 4, 4, 4), vThreadIDInGroupFlattened.xxxx
  bfi r1.xyzw, l(31, 31, 31, 31), l(1, 1, 1, 1), vThreadGroupID.xyyy, r1.xyzw
  iadd r0.z, r0.x, l(64)
  ld_structured r0.z, r0.z, l(0), g0.xxxx
  not r0.z, r0.z
  ld_structured r0.w, r0.x, l(0), g0.xxxx
  f32tof16 r0.w, r0.w
  add_sat r0.z, r0.z, l(-0.001000)
  f32tof16 r0.z, r0.z
  imad r0.z, r0.w, l(0x00010000), r0.z
  store_uav_typed u1.xyzw, r1.xyzw, r0.zzzz
endif 
ld_structured r0.z, r2.y, l(0), g0.xxxx
ld_structured r0.w, r2.z, l(0), g0.xxxx
ld_structured r1.x, r2.w, l(0), g0.xxxx
sync_g_t
umax r0.w, r0.w, r1.x
umax r0.z, r0.w, r0.z
atomic_umax g0, r0.xyxx, r0.z
sync_g_t
if_z vThreadIDInGroupFlattened.x
  ld_structured r0.y, l(64), l(0), g0.xxxx
  not r0.y, r0.y
  ld_structured r0.x, r0.x, l(0), g0.xxxx
  f32tof16 r0.x, r0.x
  add_sat r0.y, r0.y, l(-0.001000)
  f32tof16 r0.y, r0.y
  imad r0.x, r0.x, l(0x00010000), r0.y
  store_uav_typed u2.xyzw, vThreadGroupID.xyyy, r0.xxxx
endif 
ret 
// Approximately 90 instruction slots used
#endif

const BYTE g_pParticleDepthBoundsCS[] =
{
     68,  88,  66,  67, 191,  55, 
    137, 151, 130, 187, 166,  75, 
    199, 104, 187,  48, 152, 234, 
    136,  63,   1,   0,   0,   0, 
    232,  17,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    232,   4,   0,   0, 248,   4, 
      0,   0,   8,   5,   0,   0, 
    240,  15,   0,   0, 140,  16, 
      0,   0,  82,  68,  69,  70, 
    168,   4,   0,   0,   1,   0, 
      0,   0,  72,   1,   0,   0, 
      6,   0,   0,   0,  60,   0, 
      0,   0,   0,   5,  83,  67, 
      0,   1,   0,   0, 128,   4, 
      0,   0,  82,  68,  49,  49, 
     60,   0,   0,   0,  24,   0, 
      0,   0,  32,   0,   0,   0, 
     40,   0,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0, 252,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     12,   1,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  20,   1,   0,   0, 
      4,   0,   0,   0,   4,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  30,   1, 
      0,   0,   4,   0,   0,   0, 
      4,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     41,   1,   0,   0,   4,   0, 
      0,   0,   4,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   2,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  52,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 103,  83, 
     97, 109, 112,  80, 111, 105, 
    110, 116,  67, 108,  97, 109, 
    112,   0, 103,  95,  73, 110, 
    112, 117, 116,   0, 103,  95, 
     79, 117, 116, 112, 117, 116, 
     56,   0, 103,  95,  79, 117, 
    116, 112, 117, 116,  49,  54, 
      0, 103,  95,  79, 117, 116, 
    112, 117, 116,  51,  50,   0, 
     67,  66,  67, 104,  97, 110, 
    103, 101, 115,  80, 101, 114, 
     86, 105, 101, 119,   0, 171, 
    171, 171,  52,   1,   0,   0, 
     12,   0,   0,   0,  96,   1, 
      0,   0, 176,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  64,   3,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
     84,   3,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    120,   3,   0,   0,  64,   0, 
      0,   0,  64,   0,   0,   0, 
      0,   0,   0,   0,  84,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 130,   3, 
      0,   0, 128,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 152,   3,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 188,   3,   0,   0, 
    132,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    152,   3,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    201,   3,   0,   0, 136,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 152,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 210,   3, 
      0,   0, 140,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 152,   3,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 219,   3,   0,   0, 
    144,   0,   0,   0,   8,   0, 
      0,   0,   0,   0,   0,   0, 
    240,   3,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     20,   4,   0,   0, 152,   0, 
      0,   0,   8,   0,   0,   0, 
      2,   0,   0,   0, 240,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  34,   4, 
      0,   0, 160,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  52,   4,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  88,   4,   0,   0, 
    164,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     52,   4,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    102,   4,   0,   0, 168,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  52,   4, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 115,   4, 
      0,   0, 172,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  52,   4,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 103,  73, 110, 118, 
     86, 105, 101, 119,   0, 102, 
    108, 111,  97, 116,  52, 120, 
     52,   0, 171, 171,   3,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     73,   3,   0,   0, 103,  86, 
    105, 101, 119,  80, 114, 111, 
    106,   0, 103,  86, 101, 114, 
    116,  67, 111, 116,  97, 110, 
    103, 101, 110, 116,   0, 102, 
    108, 111,  97, 116,   0, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 145,   3,   0,   0, 
    103,  65, 115, 112, 101,  99, 
    116,  82,  97, 116, 105, 111, 
      0, 103,  82,  99, 112,  70, 
     97, 114,  90,   0, 103,  73, 
    110, 118, 101, 114, 116,  90, 
      0, 103,  66, 117, 102, 102, 
    101, 114,  68, 105, 109,   0, 
    102, 108, 111,  97, 116,  50, 
      0, 171, 171, 171,   1,   0, 
      3,   0,   1,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    230,   3,   0,   0, 103,  82, 
     99, 112,  66, 117, 102, 102, 
    101, 114,  68, 105, 109,   0, 
    103,  66, 105, 110, 115,  80, 
    101, 114,  82, 111, 119,   0, 
    100, 119, 111, 114, 100,   0, 
      0,   0,  19,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  46,   4,   0,   0, 
    103,  84, 105, 108, 101,  82, 
    111, 119,  80, 105, 116,  99, 
    104,   0, 103,  84, 105, 108, 
    101, 115,  80, 101, 114,  82, 
    111, 119,   0, 103,  84, 105, 
    108, 101, 115,  80, 101, 114, 
     67, 111, 108,   0,  77, 105, 
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
     69,  88, 224,  10,   0,   0, 
     80,   0,   5,   0, 184,   2, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   1,   0,   0,   0, 
     10,   0,   0,   0,  90,   0, 
      0,   3,   0,  96,  16,   0, 
      2,   0,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  85,  85, 
      0,   0, 156,  24,   0,   4, 
      0, 224,  17,   0,   0,   0, 
      0,   0,  68,  68,   0,   0, 
    156,  24,   0,   4,   0, 224, 
     17,   0,   1,   0,   0,   0, 
     68,  68,   0,   0, 156,  24, 
      0,   4,   0, 224,  17,   0, 
      2,   0,   0,   0,  68,  68, 
      0,   0,  95,   0,   0,   2, 
      0,  64,   2,   0,  95,   0, 
      0,   2,  50,  16,   2,   0, 
     95,   0,   0,   2,  50,   0, 
      2,   0, 104,   0,   0,   2, 
      6,   0,   0,   0, 160,   0, 
      0,   5,   0, 240,  17,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0, 128,   0,   0,   0, 
    155,   0,   0,   4,   8,   0, 
      0,   0,   8,   0,   0,   0, 
      1,   0,   0,   0,  35,   0, 
      0,  14,  50,   0,  16,   0, 
      0,   0,   0,   0,  70,   0, 
      2,   0,   2,  64,   0,   0, 
      4,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     86,   0,   0,   5,  50,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
     50,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0, 230, 138, 
     32,   0,   1,   0,   0,   0, 
      9,   0,   0,   0,  50,   0, 
      0,  13, 242,   0,  16,   0, 
      1,   0,   0,   0, 230, 142, 
     32,   0,   1,   0,   0,   0, 
      9,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,  64, 
     70,   4,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  13, 
    194,   0,  16,   0,   0,   0, 
      0,   0, 166, 142,  32,   0, 
      1,   0,   0,   0,   9,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,  64, 
      0,   0,   0,  64,   6,   4, 
     16,   0,   0,   0,   0,   0, 
    109,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
     10,  96,  16,   0,   2,   0, 
      0,   0, 109,   0,   0, 139, 
    194,   0,   0, 128,  67,  85, 
     21,   0, 242,   0,  16,   0, 
      3,   0,   0,   0,  70,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0,  10,  96,  16,   0, 
      2,   0,   0,   0, 109,   0, 
      0, 139, 194,   0,   0, 128, 
     67,  85,  21,   0, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    230,  10,  16,   0,   1,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,  10,  96, 
     16,   0,   2,   0,   0,   0, 
    109,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    242,   0,  16,   0,   0,   0, 
      0,   0, 230,  10,  16,   0, 
      0,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
     10,  96,  16,   0,   2,   0, 
      0,   0,  52,   0,   0,   7, 
    242,   0,  16,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     52,   0,   0,   7, 242,   0, 
     16,   0,   5,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  52,   0, 
      0,   7, 242,   0,  16,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     70,  14,  16,   0,   5,   0, 
      0,   0,  51,   0,   0,   7, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     51,   0,   0,   7, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  51,   0, 
      0,   7, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  52,   0,   0,   7, 
     50,   0,  16,   0,   1,   0, 
      0,   0, 214,   5,  16,   0, 
      4,   0,   0,   0, 134,   0, 
     16,   0,   4,   0,   0,   0, 
     52,   0,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  51,   0, 
      0,   7,  50,   0,  16,   0, 
      0,   0,   0,   0, 214,   5, 
     16,   0,   0,   0,   0,   0, 
    134,   0,  16,   0,   0,   0, 
      0,   0,  51,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
    168,   0,   0,   8,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     10,  64,   2,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0, 140,   0,   0,  19, 
     98,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,  31,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      6,  64,   2,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     59,   0,   0,   5,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0, 190,  24,   0,   1, 
     41,   0,   0,   6,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,  64,   2,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
    167,   0,   0,   9,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
     30,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      8,   0,   0,   0,   9,   0, 
      0,   0,   2,   0,   0,   0, 
     16,   0,   0,   0, 167,   0, 
      0,   9, 130,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0, 167,   0, 
      0,   9,  18,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0, 190,  24, 
      0,   1,  83,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     83,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5,  34,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
    176,   0,   0,   7,   0, 240, 
     17,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0, 190,  24, 
      0,   1,   1,   0,   0,  10, 
    194,   0,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  72,   0, 
      0,   0,  90,   0,   0,   0, 
     31,   0,   0,   3,  42,   0, 
     16,   0,   0,   0,   0,   0, 
    138,   0,   0,  14, 242,   0, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0,  31,   0, 
      0,   0,  28,   0,   0,   0, 
     28,   0,   0,   0,  28,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,   6,  64, 
      2,   0, 140,   0,   0,  16, 
    242,   0,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
     30,   0,   0,   0,  30,   0, 
      0,   0,  30,   0,   0,   0, 
     30,   0,   0,   0,   2,  64, 
      0,   0,   2,   0,   0,   0, 
      2,   0,   0,   0,   2,   0, 
      0,   0,   2,   0,   0,   0, 
     70,  21,   2,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     30,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     64,   0,   0,   0, 167,   0, 
      0,   9,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0,  59,   0, 
      0,   5,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
    167,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    130,   0,   0,   5,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,   0,  32,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0, 111,  18, 131, 186, 
    130,   0,   0,   5,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  35,   0,   0,   9, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   1,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0, 164,   0,   0,   7, 
    242, 224,  17,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1, 167,   0, 
      0,   9,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0, 167,   0, 
      0,   9,  18,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0,  30,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,  18,   0, 
      0,   0,   4,   0,   0,   0, 
     32,   0,   0,   0,  36,   0, 
      0,   0, 167,   0,   0,   9, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 190,  24,   0,   1, 
     83,   0,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  83,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0, 176,   0,   0,   7, 
      0, 240,  17,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
    190,  24,   0,   1,  31,   0, 
      0,   3,  58,   0,  16,   0, 
      0,   0,   0,   0, 138,   0, 
      0,  14, 242,   0,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,  30,   0,   0,   0, 
     27,   0,   0,   0,  27,   0, 
      0,   0,  27,   0,   0,   0, 
      2,  64,   0,   0,   1,   0, 
      0,   0,   4,   0,   0,   0, 
      4,   0,   0,   0,   4,   0, 
      0,   0,   6,  64,   2,   0, 
    140,   0,   0,  16, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,  31,   0, 
      0,   0,  31,   0,   0,   0, 
     31,   0,   0,   0,  31,   0, 
      0,   0,   2,  64,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  70,  21, 
      2,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  30,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  64,   0, 
      0,   0, 167,   0,   0,   9, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0,  59,   0,   0,   5, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0, 167,   0, 
      0,   9, 130,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0, 130,   0, 
      0,   5, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
      0,  32,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
    111,  18, 131, 186, 130,   0, 
      0,   5,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     35,   0,   0,   9,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   1,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
    164,   0,   0,   7, 242, 224, 
     17,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      0,   0,   0,   0,  21,   0, 
      0,   1, 167,   0,   0,   9, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 190,  24,   0,   1, 
     83,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  83,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0, 176,   0,   0,   7, 
      0, 240,  17,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
    190,  24,   0,   1,  31,   0, 
      0,   2,  10,  64,   2,   0, 
    167,   0,   0,   9,  34,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  64,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
     59,   0,   0,   5,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 130,   0,   0,   5, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   0,  32, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0, 111,  18, 
    131, 186, 130,   0,   0,   5, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  35,   0, 
      0,   9,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      1,   0,  26,   0,  16,   0, 
      0,   0,   0,   0, 164,   0, 
      0,   6, 242, 224,  17,   0, 
      2,   0,   0,   0,  70,  21, 
      2,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  21,   0, 
      0,   1,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  90,   0,   0,   0, 
      6,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     16,   0,   0,   0,   9,   0, 
      0,   0,  11,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      7,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     82,  84,  83,  48,  84,   1, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,  24,   0, 
      0,   0,   3,   0,   0,   0, 
    184,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  84,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,  96,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0, 108,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 120,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 152,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    128,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   1,   0, 
      0,   0, 160,   0,   0,   0, 
      0,   0,   0,   0,  10,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
     20,   0,   0,   0,   4,   0, 
      0,   0,   4,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 127, 127,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      4,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    127, 127,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 127, 127, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};