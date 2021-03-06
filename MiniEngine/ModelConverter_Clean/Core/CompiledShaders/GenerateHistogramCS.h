#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// LumaBuf                           texture    uint          2d             t0      1 
// Histogram                             UAV    byte         r/w             u0      1 
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
dcl_resource_texture2d (uint,uint,uint,uint) t0
dcl_uav_raw u0
dcl_input vThreadIDInGroupFlattened
dcl_input vThreadID.xy
dcl_temps 3
dcl_tgsm_structured g0, 4, 256
dcl_thread_group 16, 16, 1
store_structured g0.x, vThreadIDInGroupFlattened.x, l(0), l(0)
sync_g_t
mov r0.zw, l(0,0,0,0)
mov r1.xw, l(0,0,0,0)
mov r1.y, l(0)
loop 
  uge r2.x, r1.y, l(384)
  breakc_nz r2.x
  iadd r0.xy, r1.xyxx, vThreadID.xyxx
  ld_indexable(texture2d)(uint,uint,uint,uint) r1.z, r0.xyzw, t0.yzxw
  atomic_iadd g0, r1.zwzz, l(1)
  iadd r1.y, r1.y, l(16)
endloop 
sync_g_t
ishl r0.x, vThreadIDInGroupFlattened.x, l(2)
ld_structured r0.y, vThreadIDInGroupFlattened.x, l(0), g0.xxxx
atomic_iadd u0, r0.x, r0.y
ret 
// Approximately 18 instruction slots used
#endif

const BYTE g_pGenerateHistogramCS[] =
{
     68,  88,  66,  67, 131, 202, 
    211, 181,  83,  35,  46, 203, 
      6, 105, 139, 165, 217, 223, 
     58, 193,   1,   0,   0,   0, 
    168,   4,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    248,   0,   0,   0,   8,   1, 
      0,   0,  24,   1,   0,   0, 
    252,   2,   0,   0, 152,   3, 
      0,   0,  82,  68,  69,  70, 
    184,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  60,   0, 
      0,   0,   0,   5,  83,  67, 
      0,   1,   0,   0, 142,   0, 
      0,   0,  82,  68,  49,  49, 
     60,   0,   0,   0,  24,   0, 
      0,   0,  32,   0,   0,   0, 
     40,   0,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0, 124,   0, 
      0,   0,   2,   0,   0,   0, 
      4,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    132,   0,   0,   0,   8,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  76, 117, 109,  97, 
     66, 117, 102,   0,  72, 105, 
    115, 116, 111, 103, 114,  97, 
    109,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  49,  48,  46,  49,   0, 
    171, 171,  73,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     79,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  83,  72, 
     69,  88, 220,   1,   0,   0, 
     80,   0,   5,   0, 119,   0, 
      0,   0, 106,   8,   0,   1, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
     68,  68,   0,   0, 157,   0, 
      0,   3,   0, 224,  17,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   2,   0,  64,   2,   0, 
     95,   0,   0,   2,  50,   0, 
      2,   0, 104,   0,   0,   2, 
      3,   0,   0,   0, 160,   0, 
      0,   5,   0, 240,  17,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   0,   1,   0,   0, 
    155,   0,   0,   4,  16,   0, 
      0,   0,  16,   0,   0,   0, 
      1,   0,   0,   0, 168,   0, 
      0,   8,  18, 240,  17,   0, 
      0,   0,   0,   0,  10,  64, 
      2,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
    190,  24,   0,   1,  54,   0, 
      0,   8, 194,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   8, 146,   0, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     34,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  48,   0, 
      0,   1,  80,   0,   0,   7, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0, 128,   1,   0,   0, 
      3,   0,   4,   3,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     30,   0,   0,   6,  50,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0,  70,   0,   2,   0, 
     45,   0,   0, 137, 194,   0, 
      0, 128,   3,  17,  17,   0, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0, 150, 124, 
     16,   0,   0,   0,   0,   0, 
    173,   0,   0,   7,   0, 240, 
     17,   0,   0,   0,   0,   0, 
    230,  10,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0,  30,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,  16,   0, 
      0,   0,  22,   0,   0,   1, 
    190,  24,   0,   1,  41,   0, 
      0,   6,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,  64, 
      2,   0,   1,  64,   0,   0, 
      2,   0,   0,   0, 167,   0, 
      0,   8,  34,   0,  16,   0, 
      0,   0,   0,   0,  10,  64, 
      2,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 240, 
     17,   0,   0,   0,   0,   0, 
    173,   0,   0,   7,   0, 224, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,  18,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
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
