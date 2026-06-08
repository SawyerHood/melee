#ifndef __GALE01_028B9C
#define __GALE01_028B9C

#include "camera.h" // IWYU pragma: export

#include "baselib/forward.h"

#include "baselib/wobj.h"

#include <math.h>
#include <sysdolphin/baselib/cobj.h>
#include <melee/cm/types.h>

/// .bss
/* 452C68 */ Camera cm_80452C68;
/* 453004 */ CameraDebugMode cm_80453004;

/// .data
/* 3BCB18 */ CameraModeCallbacks cm_803BCB18 = { Camera_8002B3D4,
                                                        Camera_8002CDDC,
                                                        Camera_8002D318,
                                                        Camera_8002D85C,
                                                        Camera_8002DDC4,
                                                        Camera_8002C908,
                                                        Camera_8002E490,
                                                        0,
                                                        0 };
/* 3BCB3C */ HSD_WObjDesc cm_803BCB3C = {
    NULL, { 0.0f, 40.241425f, 300.241f }, 0
};
/* 3BCB50 */ HSD_WObjDesc cm_803BCB50 = { NULL,
                                                 { 0.0f, 10.0f, 0.0f },
                                                 0 };
/* 3BCB64 */ HSD_CameraDescPerspective cm_803BCB64 = {
    0,
    0,
    1,
    { 0, 0x280, 0, 0x1E0 },
    { 0, 0x280, 0, 0x1E0 },
    &cm_803BCB3C,
    &cm_803BCB50,
    0.0f,
    NULL,
    0.1f,
    16384.0f,
    30.0f,
    1.2173333f
};

/* 3BCB9C */ f32 cm_803BCB9C[5] = { 0.0f, 1.5f, 1.32f, 1.16f, 1.0f };
/* 3BCBB0 */ char cm_803BCBB0[0x20] = "couldn't get CmSubject struct.\n";
/* 3BCBD0 */ char cm_803BCBD0[0x7C] = "camera.c\0\0\0\0"
                                      "fov_u<MTXDegToRad(90.0F)\0\0\0\0"
                                      "fov_d<MTXDegToRad(90.0F)\0\0\0\0"
                                      "fov_r<MTXDegToRad(90.0F)\0\0\0\0"
                                      "fov_l<MTXDegToRad(90.0F)";
/// /* 3BCC4C */ static void* jumptable_803BCC4C[8] = {
///     (void*)0x8002a4f8,
///     (void*)0x8002a554,
///     (void*)0x8002a6c0,
///     (void*)0x8002a6c0,
///     (void*)0x8002a52c,
///     (void*)0x8002a6c0,
///     (void*)0x8002a700,
///     (void*)0x8002a728,
/// };

// /* 3BCCA0 */ static CameraUnkGlobals cm_803BCCA0 = {
//     83.0f,  1000.0f, -30.0f,  5.0f,  -7.0f,  17.5f,  -17.5f, 0.0f,  0.0682f,
//     60.0f,  120.0f,  0.05f,   0.1f,  120.0f, 900.0f, 0.15f,  38.0f, 0.1f,
//     0.1f,   0.001f,  0.1f,    1.0f,  1.0f,   0.6f,   0.6f,   0.05f, 0.1f,
//     29.0f,  0.1f,    0.1f,    0.1f,  0.1f,   0.5f,   0.5f,   0.4f,  -11.0f,
//     400.0f, 0.2f,    4.0f,    0.05f, 1.0f,   -7.0f,  7.0f,   0.5f,  0.5f,
//     0.004f, 0.2f,    0.025f,  0.2f,  0.003f, 0.2f,   0.025f, 0.2f,  0.02f,
//     1.0f,   0.14f,   1200.0f, -0.2f, 1.2f,   0.0f,
// };

/// .rodata
/* 3B73B8 */ Vec3 const cm_803B73B8 = { 0.0f, 0.0f, -1.0f };
/* 3B73C4 */ Vec3 const cm_803B73C4 = { 0.0f };
/* 3B73D0 */ Vec3 const cm_803B73D0 = { 0.0f, 1.0f, 0.0f };
/* 3B73DC */ Vec3 const cm_803B73DC = { 0.0f, 1.0f, 0.0f };

/// .sbss
/// non-static tentative defs emit in REVERSE lexical order; reverse-target
/// declaration order yields .sbss [6458, 645C, 6460, 6464, 6468]
/* 4D6468 */ CmSubject* cm_804D6468; // requires a size of 8?
/* 4D6464 */ HSD_CObj* cm_804D6464;
/* 4D6460 */ CmSubject* cm_804D6460;
/* 4D645C */ CmSubject* cm_804D645C;
/* 4D6458 */ CmSubject* cm_804D6458;

/// .sdata
/* 4D3938 */ __declspec(section ".sdata") char cm_804D3938[0x8] = "0";

/// sdata2
/* 4D7E00 */ extern f32 const cm_804D7E00;
/* 4D7E04 */ extern f32 const cm_804D7E04;
/* 4D7E08 */ extern f32 const cm_804D7E08;     // near clip
/* 4D7E10 */ extern f32 const cm_804D7E0C; // far clip
/* 4D7E10 */ extern f32 const cm_804D7E10;
/* 4D7E14 */ extern f32 const cm_804D7E14;
/* 4D7E2C */ extern f32 const cm_804D7E2C;
/* 4D7E30 */ extern f32 const cm_804D7E30;
/* 4D7E34 */ extern f32 const cm_804D7E34;
/* 4D7E38 */ extern f32 const cm_804D7E38;
/* 4D7E3C */ extern f32 const cm_804D7E3C;
/* 4D7E40 */ extern f32 const cm_804D7E40;
/* 4D7E44 */ extern f32 const cm_804D7E44;
/* 4D7E48 */ extern f32 const cm_804D7E48;
/* 4D7E4C */ extern f32 const cm_804D7E4C;
/* 4D7E50 */ extern f32 const cm_804D7E50;
/* 4D7E54 */ extern f32 const cm_804D7E54;
/* 4D7E58 */ extern f32 const cm_804D7E58;
/* 4D7E5C */ extern f32 const cm_804D7E5C;
/* 4D7E60 */ extern f32 const cm_804D7E60; // deg2rad
/* 4D7E64 */ extern f32 const cm_804D7E64;   // rad2deg
/* 4D7E68 */ extern f32 const cm_804D7E68;
/* 4D7E6C */ extern f32 const cm_804D7E6C;
/* 4D7E70 */ extern f32 const cm_804D7E70;
/* 4D7E74 */ extern f32 const cm_804D7E74;
/* 4D7E78 */ extern f64 const cm_804D7E78;
/* 4D7E80 */ extern f64 const cm_804D7E80;
/* 4D7E88 */ extern f32 const cm_804D7E88;
/* 4D7E8C */ extern f32 const cm_804D7E8C;
/* 4D7E90 */ extern f32 const cm_804D7E90;
/* 4D7E94 */ extern f32 const cm_804D7E94;
/* 4D7E98 */ extern f32 const cm_804D7E98;
/* 4D7EA0 */ extern f64 const cm_804D7EA0;
/* 4D7EA8 */ extern f32 const cm_804D7EA8;
/* 4D7EAC */ extern f32 const cm_804D7EAC;
/* 4D7EB0 */ extern f32 const cm_804D7EB0;
/* 4D7EB8 */ extern f64 const cm_804D7EB8;
/* 4D7EC0 */ extern f32 const cm_804D7EC0;
/* 4D7EC8 */ extern f64 const cm_804D7EC8;
/* 4D7ED0 */ extern f64 const cm_804D7ED0;
/* 4D7ED8 */ extern f64 const cm_804D7ED8;
/* 4D7EE0 */ extern f64 const cm_804D7EE0;
/* 4D7EE8 */ extern f64 const cm_804D7EE8;
/* 4D7EF0 */ extern f64 const cm_804D7EF0;
/* 4D7EF8 */ extern f32 const cm_804D7EF8;
/* 4D7EFC */ extern f32 const cm_804D7EFC;
/* 4D7F00 */ extern f32 const cm_804D7F00;
/* 4D7F04 */ extern f32 const cm_804D7F04;
/* 4D7F08 */ extern f32 const cm_804D7F08;
#endif
