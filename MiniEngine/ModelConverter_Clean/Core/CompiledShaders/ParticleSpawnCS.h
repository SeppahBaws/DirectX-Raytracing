#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer EmissionProperties
// {
//
//   float3 LastEmitPosW;               // Offset:    0 Size:    12
//   float EmitSpeed;                   // Offset:   12 Size:     4
//   float3 EmitPosW;                   // Offset:   16 Size:    12
//   float FloorHeight;                 // Offset:   28 Size:     4 [unused]
//   float3 EmitDirW;                   // Offset:   32 Size:    12
//   float Restitution;                 // Offset:   44 Size:     4 [unused]
//   float3 EmitRightW;                 // Offset:   48 Size:    12
//   float EmitterVelocitySensitivity;  // Offset:   60 Size:     4
//   float3 EmitUpW;                    // Offset:   64 Size:    12
//   uint MaxParticles;                 // Offset:   76 Size:     4
//   float3 Gravity;                    // Offset:   80 Size:    12 [unused]
//   uint TextureID;                    // Offset:   92 Size:     4 [unused]
//   float3 EmissiveColor;              // Offset:   96 Size:    12 [unused]
//   float pad;                         // Offset:  108 Size:     4 [unused]
//   uint4 RandIndex[64];               // Offset:  112 Size:  1024
//
// }
//
// Resource bind info for g_ResetData
// {
//
//   struct ParticleSpawnData
//   {
//       
//       float AgeRate;                 // Offset:    0
//       float RotationSpeed;           // Offset:    4
//       float StartSize;               // Offset:    8
//       float EndSize;                 // Offset:   12
//       float3 Velocity;               // Offset:   16
//       float Mass;                    // Offset:   28
//       float3 SpreadOffset;           // Offset:   32
//       float Random;                  // Offset:   44
//       float4 StartColor;             // Offset:   48
//       float4 EndColor;               // Offset:   64
//
//   } $Element;                        // Offset:    0 Size:    80
//
// }
//
// Resource bind info for g_OutputBuffer
// {
//
//   struct ParticleMotion
//   {
//       
//       float3 Position;               // Offset:    0
//       float Mass;                    // Offset:   12
//       float3 Velocity;               // Offset:   16
//       float Age;                     // Offset:   28
//       float Rotation;                // Offset:   32
//       uint ResetDataIndex;           // Offset:   36
//
//   } $Element;                        // Offset:    0 Size:    40
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// g_ResetData                       texture  struct         r/o             t0      1 
// g_OutputBuffer                        UAV  struct     r/w+cnt             u2      1 
// EmissionProperties                cbuffer      NA          NA            cb2      1 
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
dcl_constantbuffer CB2[71], dynamicIndexed
dcl_resource_structured t0, 80
dcl_uav_structured_opc u2, 40
dcl_input vThreadID.x
dcl_temps 5
dcl_thread_group 64, 1, 1
imm_atomic_alloc r0.x, u2
uge r0.y, r0.x, cb2[4].w
if_nz r0.y
  ret 
endif 
mov r0.y, vThreadID.x
ld_structured_indexable(structured_buffer, stride=80)(mixed,mixed,mixed,mixed) r1.xyzw, cb2[r0.y + 7].x, l(16), t0.xyzw
ld_structured_indexable(structured_buffer, stride=80)(mixed,mixed,mixed,mixed) r2.xyzw, cb2[r0.y + 7].x, l(32), t0.xyzw
add r3.xyz, -cb2[0].xyzx, cb2[1].xyzx
mul r4.xyz, r1.yyyy, cb2[4].xyzx
mad r4.xyz, r1.xxxx, cb2[3].xyzx, r4.xyzx
mad r4.xyz, r1.zzzz, cb2[2].xyzx, r4.xyzx
mad r4.xyz, r3.xyzx, cb2[3].wwww, r4.xyzx
mad r3.xyz, -r3.xyzx, r2.wwww, cb2[1].xyzx
add r1.xyz, r2.xyzx, r3.xyzx
mad r2.xyz, cb2[2].xyzx, cb2[0].wwww, r4.xyzx
store_structured u2.xyzw, r0.x, l(0), r1.xyzw
mov r2.w, l(0)
store_structured u2.xyzw, r0.x, l(16), r2.xyzw
mov r1.x, l(0)
mov r1.y, cb2[r0.y + 7].x
store_structured u2.xy, r0.x, l(32), r1.xyxx
ret 
// Approximately 23 instruction slots used
#endif

const BYTE g_pParticleSpawnCS[] =
{
     68,  88,  66,  67,  45, 150, 
    254,  23, 200, 164, 217,  50, 
    189, 193, 228,  49,  43,  41, 
     40,  55,   1,   0,   0,   0, 
    252,  12,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    204,   7,   0,   0, 220,   7, 
      0,   0, 236,   7,   0,   0, 
      4,  11,   0,   0, 160,  11, 
      0,   0,  82,  68,  69,  70, 
    140,   7,   0,   0,   3,   0, 
      0,   0, 204,   0,   0,   0, 
      3,   0,   0,   0,  60,   0, 
      0,   0,   0,   5,  83,  67, 
      0,   1,   0,   0, 100,   7, 
      0,   0,  82,  68,  49,  49, 
     60,   0,   0,   0,  24,   0, 
      0,   0,  32,   0,   0,   0, 
     40,   0,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0, 156,   0, 
      0,   0,   5,   0,   0,   0, 
      6,   0,   0,   0,   1,   0, 
      0,   0,  80,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    168,   0,   0,   0,  11,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,  40,   0, 
      0,   0,   2,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 183,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 103,  95, 
     82, 101, 115, 101, 116,  68, 
     97, 116,  97,   0, 103,  95, 
     79, 117, 116, 112, 117, 116, 
     66, 117, 102, 102, 101, 114, 
      0,  69, 109, 105, 115, 115, 
    105, 111, 110,  80, 114, 111, 
    112, 101, 114, 116, 105, 101, 
    115,   0, 171, 171, 183,   0, 
      0,   0,  15,   0,   0,   0, 
     20,   1,   0,   0, 112,   4, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 156,   0, 
      0,   0,   1,   0,   0,   0, 
    192,   4,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0, 168,   0, 
      0,   0,   1,   0,   0,   0, 
    120,   6,   0,   0,  40,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0, 108,   3, 
      0,   0,   0,   0,   0,   0, 
     12,   0,   0,   0,   2,   0, 
      0,   0, 128,   3,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 164,   3,   0,   0, 
     12,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    180,   3,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    216,   3,   0,   0,  16,   0, 
      0,   0,  12,   0,   0,   0, 
      2,   0,   0,   0, 128,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 225,   3, 
      0,   0,  28,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 180,   3,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 237,   3,   0,   0, 
     32,   0,   0,   0,  12,   0, 
      0,   0,   2,   0,   0,   0, 
    128,   3,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    246,   3,   0,   0,  44,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 180,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   2,   4, 
      0,   0,  48,   0,   0,   0, 
     12,   0,   0,   0,   2,   0, 
      0,   0, 128,   3,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  13,   4,   0,   0, 
     60,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    180,   3,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     40,   4,   0,   0,  64,   0, 
      0,   0,  12,   0,   0,   0, 
      2,   0,   0,   0, 128,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  48,   4, 
      0,   0,  76,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  68,   4,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 104,   4,   0,   0, 
     80,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    128,   3,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    112,   4,   0,   0,  92,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  68,   4, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 122,   4, 
      0,   0,  96,   0,   0,   0, 
     12,   0,   0,   0,   0,   0, 
      0,   0, 128,   3,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 136,   4,   0,   0, 
    108,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    180,   3,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    140,   4,   0,   0, 112,   0, 
      0,   0,   0,   4,   0,   0, 
      2,   0,   0,   0, 156,   4, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  76,  97, 
    115, 116,  69, 109, 105, 116, 
     80, 111, 115,  87,   0, 102, 
    108, 111,  97, 116,  51,   0, 
      1,   0,   3,   0,   1,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 121,   3,   0,   0, 
     69, 109, 105, 116,  83, 112, 
    101, 101, 100,   0, 102, 108, 
    111,  97, 116,   0,   0,   0, 
      3,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    174,   3,   0,   0,  69, 109, 
    105, 116,  80, 111, 115,  87, 
      0,  70, 108, 111, 111, 114, 
     72, 101, 105, 103, 104, 116, 
      0,  69, 109, 105, 116,  68, 
    105, 114,  87,   0,  82, 101, 
    115, 116, 105, 116, 117, 116, 
    105, 111, 110,   0,  69, 109, 
    105, 116,  82, 105, 103, 104, 
    116,  87,   0,  69, 109, 105, 
    116, 116, 101, 114,  86, 101, 
    108, 111,  99, 105, 116, 121, 
     83, 101, 110, 115, 105, 116, 
    105, 118, 105, 116, 121,   0, 
     69, 109, 105, 116,  85, 112, 
     87,   0,  77,  97, 120,  80, 
     97, 114, 116, 105,  99, 108, 
    101, 115,   0, 100, 119, 111, 
    114, 100,   0, 171,   0,   0, 
     19,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     61,   4,   0,   0,  71, 114, 
     97, 118, 105, 116, 121,   0, 
     84, 101, 120, 116, 117, 114, 
    101,  73,  68,   0,  69, 109, 
    105, 115, 115, 105, 118, 101, 
     67, 111, 108, 111, 114,   0, 
    112,  97, 100,   0,  82,  97, 
    110, 100,  73, 110, 100, 101, 
    120,   0, 117, 105, 110, 116, 
     52,   0,   1,   0,  19,   0, 
      1,   0,   4,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 150,   4, 
      0,   0, 232,   4,   0,   0, 
      0,   0,   0,   0,  80,   0, 
      0,   0,   2,   0,   0,   0, 
     84,   6,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     36,  69, 108, 101, 109, 101, 
    110, 116,   0,  80,  97, 114, 
    116, 105,  99, 108, 101,  83, 
    112,  97, 119, 110,  68,  97, 
    116,  97,   0,  65, 103, 101, 
     82,  97, 116, 101,   0, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 174,   3,   0,   0, 
     82, 111, 116,  97, 116, 105, 
    111, 110,  83, 112, 101, 101, 
    100,   0,  83, 116,  97, 114, 
    116,  83, 105, 122, 101,   0, 
     69, 110, 100,  83, 105, 122, 
    101,   0,  86, 101, 108, 111, 
     99, 105, 116, 121,   0, 171, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 121,   3, 
      0,   0,  77,  97, 115, 115, 
      0,  83, 112, 114, 101,  97, 
    100,  79, 102, 102, 115, 101, 
    116,   0,  82,  97, 110, 100, 
    111, 109,   0,  83, 116,  97, 
    114, 116,  67, 111, 108, 111, 
    114,   0, 102, 108, 111,  97, 
    116,  52,   0, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    164,   5,   0,   0,  69, 110, 
    100,  67, 111, 108, 111, 114, 
      0, 171, 171, 171,   3,   5, 
      0,   0,  12,   5,   0,   0, 
      0,   0,   0,   0,  48,   5, 
      0,   0,  12,   5,   0,   0, 
      4,   0,   0,   0,  62,   5, 
      0,   0,  12,   5,   0,   0, 
      8,   0,   0,   0,  72,   5, 
      0,   0,  12,   5,   0,   0, 
     12,   0,   0,   0,  80,   5, 
      0,   0,  92,   5,   0,   0, 
     16,   0,   0,   0, 128,   5, 
      0,   0,  12,   5,   0,   0, 
     28,   0,   0,   0, 133,   5, 
      0,   0,  92,   5,   0,   0, 
     32,   0,   0,   0, 146,   5, 
      0,   0,  12,   5,   0,   0, 
     44,   0,   0,   0, 153,   5, 
      0,   0, 172,   5,   0,   0, 
     48,   0,   0,   0, 208,   5, 
      0,   0, 172,   5,   0,   0, 
     64,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,  20,   0, 
      0,   0,  10,   0, 220,   5, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    241,   4,   0,   0, 232,   4, 
      0,   0,   0,   0,   0,   0, 
     40,   0,   0,   0,   2,   0, 
      0,   0,  64,   7,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  80,  97, 114, 116, 
    105,  99, 108, 101,  77, 111, 
    116, 105, 111, 110,   0,  80, 
    111, 115, 105, 116, 105, 111, 
    110,   0,  65, 103, 101,   0, 
     82, 111, 116,  97, 116, 105, 
    111, 110,   0,  82, 101, 115, 
    101, 116,  68,  97, 116,  97, 
     73, 110, 100, 101, 120,   0, 
      0,   0,  19,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  61,   4,   0,   0, 
    175,   6,   0,   0,  92,   5, 
      0,   0,   0,   0,   0,   0, 
    128,   5,   0,   0,  12,   5, 
      0,   0,  12,   0,   0,   0, 
     80,   5,   0,   0,  92,   5, 
      0,   0,  16,   0,   0,   0, 
    184,   6,   0,   0,  12,   5, 
      0,   0,  28,   0,   0,   0, 
    188,   6,   0,   0,  12,   5, 
      0,   0,  32,   0,   0,   0, 
    197,   6,   0,   0, 212,   6, 
      0,   0,  36,   0,   0,   0, 
      5,   0,   0,   0,   1,   0, 
     10,   0,   0,   0,   6,   0, 
    248,   6,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 160,   6,   0,   0, 
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
     83,  72,  69,  88,  16,   3, 
      0,   0,  80,   0,   5,   0, 
    196,   0,   0,   0, 106,   8, 
      0,   1,  89,   8,   0,   4, 
     70, 142,  32,   0,   2,   0, 
      0,   0,  71,   0,   0,   0, 
    162,   0,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
     80,   0,   0,   0, 158,   0, 
    128,   4,   0, 224,  17,   0, 
      2,   0,   0,   0,  40,   0, 
      0,   0,  95,   0,   0,   2, 
     18,   0,   2,   0, 104,   0, 
      0,   2,   5,   0,   0,   0, 
    155,   0,   0,   4,  64,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 178,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,   0, 224, 
     17,   0,   2,   0,   0,   0, 
     80,   0,   0,   8,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  58, 128,  32,   0, 
      2,   0,   0,   0,   4,   0, 
      0,   0,  31,   0,   4,   3, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  62,   0,   0,   1, 
     21,   0,   0,   1,  54,   0, 
      0,   4,  34,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
      2,   0, 167,   0,   0, 142, 
      2, 131,   2, 128, 131, 153, 
     25,   0, 242,   0,  16,   0, 
      1,   0,   0,   0,  10, 128, 
     32,   6,   2,   0,   0,   0, 
      7,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  16,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0, 167,   0, 
      0, 142,   2, 131,   2, 128, 
    131, 153,  25,   0, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     10, 128,  32,   6,   2,   0, 
      0,   0,   7,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     32,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,  10, 114,   0, 
     16,   0,   3,   0,   0,   0, 
     70, 130,  32, 128,  65,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   2,   0,   0,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      4,   0,   0,   0,  86,   5, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   0,   2,   0, 
      0,   0,   4,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   4,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   0, 
      2,   0,   0,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      4,   0,   0,   0,  50,   0, 
      0,  10, 114,   0,  16,   0, 
      4,   0,   0,   0, 166,  10, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   0,   2,   0, 
      0,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   4,   0, 
      0,   0,  50,   0,   0,  10, 
    114,   0,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0, 246, 143, 
     32,   0,   2,   0,   0,   0, 
      3,   0,   0,   0,  70,   2, 
     16,   0,   4,   0,   0,   0, 
     50,   0,   0,  11, 114,   0, 
     16,   0,   3,   0,   0,   0, 
     70,   2,  16, 128,  65,   0, 
      0,   0,   3,   0,   0,   0, 
    246,  15,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   7, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  11, 114,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   0,   2,   0, 
      0,   0,   2,   0,   0,   0, 
    246, 143,  32,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   4,   0, 
      0,   0, 168,   0,   0,   9, 
    242, 224,  17,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    130,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0, 168,   0, 
      0,   9, 242, 224,  17,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  16,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   5,  18,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   8,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     10, 128,  32,   6,   2,   0, 
      0,   0,   7,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0, 168,   0,   0,   9, 
     50, 224,  17,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,  32,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  23,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
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
