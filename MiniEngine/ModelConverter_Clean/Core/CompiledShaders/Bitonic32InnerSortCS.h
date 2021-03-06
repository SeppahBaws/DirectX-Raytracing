#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer CB1
// {
//
//   uint CounterOffset;                // Offset:    0 Size:     4
//   uint NullItem;                     // Offset:    4 Size:     4
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// g_CounterBuffer                   texture    byte         r/o             t0      1 
// g_SortBuffer                          UAV    byte         r/w             u0      1 
// CB1                               cbuffer      NA          NA            cb1      1 
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
dcl_constantbuffer CB1[1], immediateIndexed
dcl_resource_raw t0
dcl_uav_raw u0
dcl_input vThreadIDInGroupFlattened
dcl_input vThreadGroupID.x
dcl_temps 6
dcl_tgsm_structured g0, 4, 2048
dcl_thread_group 1024, 1, 1
ld_raw_indexable(raw_buffer)(mixed,mixed,mixed,mixed) r0.x, cb1[0].x, t0.xxxx
imad r0.y, vThreadGroupID.x, l(2048), vThreadIDInGroupFlattened.x
and r0.z, r0.y, l(1023)
ult r0.w, r0.y, r0.x
ishl r1.xy, vThreadIDInGroupFlattened.xxxx, l(2, 1, 0, 0)
imad r1.x, vThreadGroupID.x, l(8192), r1.x
ld_raw_indexable(raw_buffer)(mixed,mixed,mixed,mixed) r1.z, r1.x, u0.xxxx
movc r1.z, r0.w, r1.z, cb1[0].y
store_structured g0.x, r0.z, l(0), r1.z
iadd r0.y, r0.y, l(1024)
and r1.z, r0.y, l(2047)
ult r0.x, r0.y, r0.x
ishl r0.y, r0.y, l(2)
ld_raw_indexable(raw_buffer)(mixed,mixed,mixed,mixed) r1.w, r0.y, u0.xxxx
movc r1.w, r0.x, r1.w, cb1[0].y
store_structured g0.x, r1.z, l(0), r1.w
sync_g_t
bfi r2.xy, l(10, 31, 0, 0), l(0, 1, 0, 0), vThreadIDInGroupFlattened.xxxx, l(1024, 1, 0, 0)
xor r2.zw, r2.xxxy, l(0, 0, 1024, 1)
ld_structured r1.w, r2.z, l(0), g0.xxxx
ld_structured r3.x, r2.x, l(0), g0.xxxx
xor r3.y, r1.w, cb1[0].y
xor r3.z, r3.x, cb1[0].y
ult r3.y, r3.y, r3.z
if_nz r3.y
  store_structured g0.x, r2.z, l(0), r3.x
  store_structured g0.x, r2.x, l(0), r1.w
endif 
sync_g_t
and r3.xyzw, r1.yyyy, l(1024, 1536, 1792, 1920)
bfi r3.xyzw, l(9, 8, 7, 6), l(0, 0, 0, 0), vThreadIDInGroupFlattened.xxxx, r3.xyzw
iadd r3.xyzw, r3.xyzw, l(512, 256, 128, 64)
xor r4.xyzw, r3.xyzw, l(512, 256, 128, 64)
ld_structured r1.w, r4.x, l(0), g0.xxxx
ld_structured r2.x, r3.x, l(0), g0.xxxx
xor r2.z, r1.w, cb1[0].y
xor r5.x, r2.x, cb1[0].y
ult r2.z, r2.z, r5.x
if_nz r2.z
  store_structured g0.x, r4.x, l(0), r2.x
  store_structured g0.x, r3.x, l(0), r1.w
endif 
sync_g_t
ld_structured r1.w, r4.y, l(0), g0.xxxx
ld_structured r2.x, r3.y, l(0), g0.xxxx
xor r2.z, r1.w, cb1[0].y
xor r3.x, r2.x, cb1[0].y
ult r2.z, r2.z, r3.x
if_nz r2.z
  store_structured g0.x, r4.y, l(0), r2.x
  store_structured g0.x, r3.y, l(0), r1.w
endif 
sync_g_t
ld_structured r1.w, r4.z, l(0), g0.xxxx
ld_structured r2.x, r3.z, l(0), g0.xxxx
xor r2.z, r1.w, cb1[0].y
xor r3.x, r2.x, cb1[0].y
ult r2.z, r2.z, r3.x
if_nz r2.z
  store_structured g0.x, r4.z, l(0), r2.x
  store_structured g0.x, r3.z, l(0), r1.w
endif 
sync_g_t
ld_structured r1.w, r4.w, l(0), g0.xxxx
ld_structured r2.x, r3.w, l(0), g0.xxxx
xor r2.z, r1.w, cb1[0].y
xor r3.x, r2.x, cb1[0].y
ult r2.z, r2.z, r3.x
if_nz r2.z
  store_structured g0.x, r4.w, l(0), r2.x
  store_structured g0.x, r3.w, l(0), r1.w
endif 
sync_g_t
and r3.xyzw, r1.yyyy, l(1984, 2016, 2032, 2040)
bfi r3.xyzw, l(5, 4, 3, 2), l(0, 0, 0, 0), vThreadIDInGroupFlattened.xxxx, r3.xyzw
iadd r3.xyzw, r3.xyzw, l(32, 16, 8, 4)
xor r4.xyzw, r3.xyzw, l(32, 16, 8, 4)
ld_structured r1.w, r4.x, l(0), g0.xxxx
ld_structured r2.x, r3.x, l(0), g0.xxxx
xor r2.z, r1.w, cb1[0].y
xor r5.x, r2.x, cb1[0].y
ult r2.z, r2.z, r5.x
if_nz r2.z
  store_structured g0.x, r4.x, l(0), r2.x
  store_structured g0.x, r3.x, l(0), r1.w
endif 
sync_g_t
ld_structured r1.w, r4.y, l(0), g0.xxxx
ld_structured r2.x, r3.y, l(0), g0.xxxx
xor r2.z, r1.w, cb1[0].y
xor r3.x, r2.x, cb1[0].y
ult r2.z, r2.z, r3.x
if_nz r2.z
  store_structured g0.x, r4.y, l(0), r2.x
  store_structured g0.x, r3.y, l(0), r1.w
endif 
sync_g_t
ld_structured r1.w, r4.z, l(0), g0.xxxx
ld_structured r2.x, r3.z, l(0), g0.xxxx
xor r2.z, r1.w, cb1[0].y
xor r3.x, r2.x, cb1[0].y
ult r2.z, r2.z, r3.x
if_nz r2.z
  store_structured g0.x, r4.z, l(0), r2.x
  store_structured g0.x, r3.z, l(0), r1.w
endif 
sync_g_t
ld_structured r1.w, r4.w, l(0), g0.xxxx
ld_structured r2.x, r3.w, l(0), g0.xxxx
xor r2.z, r1.w, cb1[0].y
xor r3.x, r2.x, cb1[0].y
ult r2.z, r2.z, r3.x
if_nz r2.z
  store_structured g0.x, r4.w, l(0), r2.x
  store_structured g0.x, r3.w, l(0), r1.w
endif 
sync_g_t
and r1.y, r1.y, l(2044)
bfi r1.y, l(1), l(0), vThreadIDInGroupFlattened.x, r1.y
iadd r1.y, r1.y, l(2)
xor r1.w, r1.y, l(2)
ld_structured r2.x, r1.w, l(0), g0.xxxx
ld_structured r2.z, r1.y, l(0), g0.xxxx
xor r3.xy, r2.xzxx, cb1[0].yyyy
ult r3.x, r3.x, r3.y
if_nz r3.x
  store_structured g0.x, r1.w, l(0), r2.z
  store_structured g0.x, r1.y, l(0), r2.x
endif 
sync_g_t
ld_structured r1.y, r2.w, l(0), g0.xxxx
ld_structured r1.w, r2.y, l(0), g0.xxxx
xor r2.xz, r1.yywy, cb1[0].yyyy
ult r2.x, r2.x, r2.z
if_nz r2.x
  store_structured g0.x, r2.w, l(0), r1.w
  store_structured g0.x, r2.y, l(0), r1.y
endif 
sync_g_t
if_nz r0.w
  ld_structured r0.z, r0.z, l(0), g0.xxxx
  store_raw u0.x, r1.x, r0.z
endif 
if_nz r0.x
  ld_structured r0.x, r1.z, l(0), g0.xxxx
  store_raw u0.x, r0.y, r0.x
endif 
ret 
// Approximately 148 instruction slots used
#endif

const BYTE g_pBitonic32InnerSortCS[] =
{
     68,  88,  66,  67,  74,  62, 
      2, 133, 233, 226,  94, 206, 
     82, 120, 179,   5, 243, 156, 
     40,  55,   1,   0,   0,   0, 
    136,  19,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    212,   1,   0,   0, 228,   1, 
      0,   0, 244,   1,   0,   0, 
     68,  18,   0,   0, 224,  18, 
      0,   0,  82,  68,  69,  70, 
    148,   1,   0,   0,   1,   0, 
      0,   0, 192,   0,   0,   0, 
      3,   0,   0,   0,  60,   0, 
      0,   0,   0,   5,  83,  67, 
      0,   1,   0,   0, 105,   1, 
      0,   0,  82,  68,  49,  49, 
     60,   0,   0,   0,  24,   0, 
      0,   0,  32,   0,   0,   0, 
     40,   0,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0, 156,   0, 
      0,   0,   7,   0,   0,   0, 
      6,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    172,   0,   0,   0,   8,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 185,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 103,  95, 
     67, 111, 117, 110, 116, 101, 
    114,  66, 117, 102, 102, 101, 
    114,   0, 103,  95,  83, 111, 
    114, 116,  66, 117, 102, 102, 
    101, 114,   0,  67,  66,  49, 
      0, 171, 171, 171, 185,   0, 
      0,   0,   2,   0,   0,   0, 
    216,   0,   0,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  40,   1, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  60,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  96,   1,   0,   0, 
      4,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     60,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     67, 111, 117, 110, 116, 101, 
    114,  79, 102, 102, 115, 101, 
    116,   0, 100, 119, 111, 114, 
    100,   0,   0,   0,  19,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   1, 
      0,   0,  78, 117, 108, 108, 
     73, 116, 101, 109,   0,  77, 
    105,  99, 114, 111, 115, 111, 
    102, 116,  32,  40,  82,  41, 
     32,  72,  76,  83,  76,  32, 
     83, 104,  97, 100, 101, 114, 
     32,  67, 111, 109, 112, 105, 
    108, 101, 114,  32,  49,  48, 
     46,  49,   0, 171, 171, 171, 
     73,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  79,  83, 
     71,  78,   8,   0,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  83,  72,  69,  88, 
     72,  16,   0,   0,  80,   0, 
      5,   0,  18,   4,   0,   0, 
    106,   8,   0,   1,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 161,   0,   0,   3, 
      0, 112,  16,   0,   0,   0, 
      0,   0, 157,   0,   0,   3, 
      0, 224,  17,   0,   0,   0, 
      0,   0,  95,   0,   0,   2, 
      0,  64,   2,   0,  95,   0, 
      0,   2,  18,  16,   2,   0, 
    104,   0,   0,   2,   6,   0, 
      0,   0, 160,   0,   0,   5, 
      0, 240,  17,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   8,   0,   0, 155,   0, 
      0,   4,   0,   4,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 165,   0,   0, 138, 
    194,   2,   0, 128, 131, 153, 
     25,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,  10, 128, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   6, 112, 
     16,   0,   0,   0,   0,   0, 
     35,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     10,  16,   2,   0,   1,  64, 
      0,   0,   0,   8,   0,   0, 
     10,  64,   2,   0,   1,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0, 255,   3, 
      0,   0,  79,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     41,   0,   0,   9,  50,   0, 
     16,   0,   1,   0,   0,   0, 
      6,  64,   2,   0,   2,  64, 
      0,   0,   2,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     35,   0,   0,   8,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,  16,   2,   0,   1,  64, 
      0,   0,   0,  32,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0, 165,   0,   0, 137, 
    194,   2,   0, 128, 131, 153, 
     25,   0,  66,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
      6, 224,  17,   0,   0,   0, 
      0,   0,  55,   0,   0,  10, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     26, 128,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
    168,   0,   0,   9,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     30,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   4,   0,   0,   1,   0, 
      0,   7,  66,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0, 255,   7, 
      0,   0,  79,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     41,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      2,   0,   0,   0, 165,   0, 
      0, 137, 194,   2,   0, 128, 
    131, 153,  25,   0, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   6, 224,  17,   0, 
      0,   0,   0,   0,  55,   0, 
      0,  10, 130,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  26, 128,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0, 190,  24,   0,   1, 
    140,   0,   0,  19,  50,   0, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0,  10,   0, 
      0,   0,  31,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   6,  64, 
      2,   0,   2,  64,   0,   0, 
      0,   4,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  87,   0, 
      0,  10, 194,   0,  16,   0, 
      2,   0,   0,   0,   6,   4, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   4,   0,   0,   1,   0, 
      0,   0, 167,   0,   0,   9, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
     18,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0,  87,   0,   0,   8, 
     34,   0,  16,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  26, 128, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  87,   0, 
      0,   8,  66,   0,  16,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     26, 128,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     79,   0,   0,   7,  34,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16,   0,   3,   0, 
      0,   0,  42,   0,  16,   0, 
      3,   0,   0,   0,  31,   0, 
      4,   3,  26,   0,  16,   0, 
      3,   0,   0,   0, 168,   0, 
      0,   9,  18, 240,  17,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0, 168,   0, 
      0,   9,  18, 240,  17,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  21,   0, 
      0,   1, 190,  24,   0,   1, 
      1,   0,   0,  10, 242,   0, 
     16,   0,   3,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   4,   0,   0,   0,   6, 
      0,   0,   0,   7,   0,   0, 
    128,   7,   0,   0, 140,   0, 
      0,  16, 242,   0,  16,   0, 
      3,   0,   0,   0,   2,  64, 
      0,   0,   9,   0,   0,   0, 
      8,   0,   0,   0,   7,   0, 
      0,   0,   6,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   6,  64,   2,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  30,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,   2,  64, 
      0,   0,   0,   2,   0,   0, 
      0,   1,   0,   0, 128,   0, 
      0,   0,  64,   0,   0,   0, 
     87,   0,   0,  10, 242,   0, 
     16,   0,   4,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   2,   0,   0,   0,   1, 
      0,   0, 128,   0,   0,   0, 
     64,   0,   0,   0, 167,   0, 
      0,   9, 130,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0, 167,   0, 
      0,   9,  18,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0,  87,   0, 
      0,   8,  66,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     26, 128,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     87,   0,   0,   8,  18,   0, 
     16,   0,   5,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  26, 128,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  79,   0,   0,   7, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   5,   0,   0,   0, 
     31,   0,   4,   3,  42,   0, 
     16,   0,   2,   0,   0,   0, 
    168,   0,   0,   9,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
    168,   0,   0,   9,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     21,   0,   0,   1, 190,  24, 
      0,   1, 167,   0,   0,   9, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0,  87,   0,   0,   8, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  26, 128, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  87,   0, 
      0,   8,  18,   0,  16,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     26, 128,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     79,   0,   0,   7,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,  31,   0, 
      4,   3,  42,   0,  16,   0, 
      2,   0,   0,   0, 168,   0, 
      0,   9,  18, 240,  17,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0, 168,   0, 
      0,   9,  18, 240,  17,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  21,   0, 
      0,   1, 190,  24,   0,   1, 
    167,   0,   0,   9, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
     87,   0,   0,   8,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  26, 128,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  87,   0,   0,   8, 
     18,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,  26, 128, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  79,   0, 
      0,   7,  66,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,  31,   0,   4,   3, 
     42,   0,  16,   0,   2,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  21,   0,   0,   1, 
    190,  24,   0,   1, 167,   0, 
      0,   9, 130,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0, 167,   0, 
      0,   9,  18,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0,  87,   0, 
      0,   8,  66,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     26, 128,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     87,   0,   0,   8,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  26, 128,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  79,   0,   0,   7, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     31,   0,   4,   3,  42,   0, 
     16,   0,   2,   0,   0,   0, 
    168,   0,   0,   9,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
    168,   0,   0,   9,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     21,   0,   0,   1, 190,  24, 
      0,   1,   1,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0, 192,   7,   0,   0, 
    224,   7,   0,   0, 240,   7, 
      0,   0, 248,   7,   0,   0, 
    140,   0,   0,  16, 242,   0, 
     16,   0,   3,   0,   0,   0, 
      2,  64,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   6,  64, 
      2,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  30,   0, 
      0,  10, 242,   0,  16,   0, 
      3,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
      2,  64,   0,   0,  32,   0, 
      0,   0,  16,   0,   0,   0, 
      8,   0,   0,   0,   4,   0, 
      0,   0,  87,   0,   0,  10, 
    242,   0,  16,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,   2,  64, 
      0,   0,  32,   0,   0,   0, 
     16,   0,   0,   0,   8,   0, 
      0,   0,   4,   0,   0,   0, 
    167,   0,   0,   9, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
     87,   0,   0,   8,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  26, 128,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  87,   0,   0,   8, 
     18,   0,  16,   0,   5,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,  26, 128, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  79,   0, 
      0,   7,  66,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   5,   0, 
      0,   0,  31,   0,   4,   3, 
     42,   0,  16,   0,   2,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  21,   0,   0,   1, 
    190,  24,   0,   1, 167,   0, 
      0,   9, 130,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0, 167,   0, 
      0,   9,  18,   0,  16,   0, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0,  87,   0, 
      0,   8,  66,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     26, 128,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     87,   0,   0,   8,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  26, 128,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  79,   0,   0,   7, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     31,   0,   4,   3,  42,   0, 
     16,   0,   2,   0,   0,   0, 
    168,   0,   0,   9,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
    168,   0,   0,   9,  18, 240, 
     17,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     21,   0,   0,   1, 190,  24, 
      0,   1, 167,   0,   0,   9, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0,  87,   0,   0,   8, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  26, 128, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  87,   0, 
      0,   8,  18,   0,  16,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     26, 128,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     79,   0,   0,   7,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,  31,   0, 
      4,   3,  42,   0,  16,   0, 
      2,   0,   0,   0, 168,   0, 
      0,   9,  18, 240,  17,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0, 168,   0, 
      0,   9,  18, 240,  17,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  21,   0, 
      0,   1, 190,  24,   0,   1, 
    167,   0,   0,   9, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0,   9,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
     87,   0,   0,   8,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  26, 128,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  87,   0,   0,   8, 
     18,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,  26, 128, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  79,   0, 
      0,   7,  66,   0,  16,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,  31,   0,   4,   3, 
     42,   0,  16,   0,   2,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      4,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      3,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  21,   0,   0,   1, 
    190,  24,   0,   1,   1,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0, 252,   7, 
      0,   0, 140,   0,   0,  10, 
     34,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     10,  64,   2,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     30,   0,   0,   7,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      2,   0,   0,   0,  87,   0, 
      0,   7, 130,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   2,   0, 
      0,   0, 167,   0,   0,   9, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0,  87,   0,   0,   8, 
     50,   0,  16,   0,   3,   0, 
      0,   0, 134,   0,  16,   0, 
      2,   0,   0,   0,  86, 133, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  79,   0, 
      0,   7,  18,   0,  16,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16,   0,   3,   0, 
      0,   0,  31,   0,   4,   3, 
     10,   0,  16,   0,   3,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0, 168,   0,   0,   9, 
     18, 240,  17,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  21,   0,   0,   1, 
    190,  24,   0,   1, 167,   0, 
      0,   9,  34,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0, 167,   0, 
      0,   9, 130,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 240,  17,   0, 
      0,   0,   0,   0,  87,   0, 
      0,   8,  82,   0,  16,   0, 
      2,   0,   0,   0,  86,   7, 
     16,   0,   1,   0,   0,   0, 
     86, 133,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     79,   0,   0,   7,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  31,   0, 
      4,   3,  10,   0,  16,   0, 
      2,   0,   0,   0, 168,   0, 
      0,   9,  18, 240,  17,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0, 168,   0, 
      0,   9,  18, 240,  17,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   2,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  21,   0, 
      0,   1, 190,  24,   0,   1, 
     31,   0,   4,   3,  58,   0, 
     16,   0,   0,   0,   0,   0, 
    167,   0,   0,   9,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    166,   0,   0,   7,  18, 224, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  21,   0, 
      0,   1,  31,   0,   4,   3, 
     10,   0,  16,   0,   0,   0, 
      0,   0, 167,   0,   0,   9, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 240,  17,   0,   0,   0, 
      0,   0, 166,   0,   0,   7, 
     18, 224,  17,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     21,   0,   0,   1,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0, 148,   0, 
      0,   0,   6,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  42,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  82,  84,  83,  48, 
    160,   0,   0,   0,   2,   0, 
      0,   0,   4,   0,   0,   0, 
     24,   0,   0,   0,   0,   0, 
      0,   0, 160,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     72,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     84,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    116,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
    148,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   0,  92,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0, 124,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   1,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0
};
