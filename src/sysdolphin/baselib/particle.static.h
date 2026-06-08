#ifndef __GALE01_391580
#define __GALE01_391580

#include "particle.h" // IWYU pragma: export
#include "platform.h"

#include "baselib/psstructs.h"

// types

struct ParticleScreenState {
    /* 0x00 */ u8 x0_b7 : 1;
    /* 0x00 */ u8 x0_b6 : 1;
    /* 0x00 */ u8 x0_b5 : 1;
    /* 0x00 */ u8 x0_rest : 5;
    /* 0x01 */ u8 _pad0[0x3];
    /* 0x04 */ s32 x4;
    /* 0x08 */ s32 x8;
    /* 0x0C */ s32 x0C;
    /* 0x10 */ s32 x10;
    /* 0x14 */ s32 x14;
    /* 0x18 */ s32 x18;
    /* 0x1C */ s32 x1C;
    /* 0x20 */ s32 x20;
    /* 0x24 */ s32 x24;
    /* 0x28 */ u32 x28;
    /* 0x2C */ s32 x2C;
    /* 0x30 */ void* x30;
    /* 0x34 */ s32 x34;
    /* 0x38 */ s32 x38;
    /* 0x3C */ s32 x3C;
    /* 0x40 */ s32 x40;
    /* 0x44 */ s32 x44;
    /* 0x48 */ s32 x48;
    /* 0x4C */ void* x4C;
    /* 0x50 */ void* x50;
    /* 0x54 */ u8 _pad4[0x68];
    /* 0xBC */ u32 xBC;
    /* 0xC0 */ u32 xC0;
    /* 0xC4 */ s32 xC4;
    /* 0xC8 */ s32 xC8;
    /* 0xCC */ s32 xCC;
    /* 0xD0 */ void* xD0;
    /* 0xD4 */ OSContext* xD4;
};

struct ParticleConsoleState {
    /* 00 */ u8 x0_b0 : 1;
    /* 00 */ u8 x0_b1 : 1;
    /* 04 */ u8* out_buf;
    /* 08 */ u32 buf_size;
    /* 0C */ int xC;
    /* 10 */ u8 x10;
    /* 11 */ u8 x11;
    /* 12 */ u8 x12;
    /* 13 */ u8 x13;
    /* 14 */ int x14;
    /* 18 */ int x18;
    /* 1C */ int x1C;
    /* 20 */ int x20;
    /* 20 */ UNK_T x24;
};
STATIC_ASSERT(sizeof(struct ParticleConsoleState) == 0x28);

// .sbss

/* 4D7850 */ static HSD_SList* hsd_804D7850;

// .bss

/* hsd_804D0F60 moved to hsd_3983.c, hsd_804D0F90 to hsd_39D1.c
 * (their original TUs) */

// .data

/* 4D7858 */ static f32 hsd_804D7858;
/* 4D785C */ static f32 hsd_804D785C;
/* 4D7860 */ static f32 hsd_804D7860;
/* 4D7864 */ static f32 hsd_804D7864;
/* 4D7868 */ static f32 hsd_804D7868;
/* 4D786C */ static f32 hsd_804D786C;
/* 4D7870 */ static s32 hsd_804D7870;
/* 4D7874 */ static s32 hsd_804D7874;
/* 4D7878 */ static s32 hsd_804D7878;
/* 4D787C */ static f32 hsd_804D787C;
/* 4D7880 */ static f32 hsd_804D7880;
/* 4D7884 */ static f32 hsd_804D7884;
/* 4D7888 */ static s32 hsd_804D7888;
/* 4D7890 */ static s32 hsd_804D7890;
/* 4D7894 */ static int hsd_804D7894;
/* 4D7898 */ static s32 hsd_804D7898;
/* 4D789C */ static s32 hsd_804D789C;
/* 4D78B0 */ static s32 hsd_804D78B0;
/* 4D78B4 */ static s32 hsd_804D78B4;
/* 4D78B8 */ static s32 hsd_804D78B8;
/* 4D78BC */ static s32 hsd_804D78BC;
/* 4D78C0 */ static int hsd_804D78C0;
/* 4D78C8 */ static int hsd_804D78C8;
/* 4D78CC */ static u32 hsd_804D78CC;
/* 4D78D0..4D78E2 moved to hsd_3983.c, 4D78E8..4D7900 to hsd_39D1.c
 * (their original TUs) */

#endif
