#include "soundtest.h"

#include "placeholder.h"

#include "gm/forward.h"

#include "gm/gm_unsplit.h"
#include "gm/gmmain_lib.h"
#include "if/textlib.h"
#include "if/types.h"
#include "lb/lbarchive.h"
#include "lb/lbaudio_ax.h"
#include "lb/lbcardgame.h"
#include "lb/lbcardnew.h"
#include "lb/lblanguage.h"
#include "lb/lbsnap.h"
#include "ty/toy.h"
#include "ty/tylist.h"

#include <printf.h>
#include <dolphin/mtx.h>
#include <baselib/cobj.h>
#include <baselib/fog.h>
#include <baselib/gobj.h>
#include <baselib/gobjgxlink.h>
#include <baselib/gobjobject.h>
#include <baselib/gobjplink.h>
#include <baselib/gobjproc.h>
#include <baselib/gobjuserdata.h>
#include <baselib/lobj.h>
#include <baselib/memory.h>
#include <baselib/particle.h>
#include <baselib/sislib.h>
#include <baselib/sobjlib.h>
#include <MSL/stdio.h>
#include <MSL/string.h>

/// .sdata
/* 4D5850 */ static int un_804D5850 = 0x7F;
/* 4D5854 */ static int un_804D5854 = 0x7F;
/* 4D5858 */ static int un_804D5858 = 0x7F;
/* 4D585C */ static int un_804D585C = -1;

/// .sbss
/* 4D6DA8 */ static int* un_804D6DA8;
/* 4D6DAC */ static int un_804D6DAC;
/* 4D6DB0 */ static int un_804D6DB0;
/* 4D6DB4 */ static int un_804D6DB4;
/* 4D6DB8 */ static int un_804D6DB8;
/* 4D6DBC */ static int un_804D6DBC;
/* 4D6DC0 */ static int un_804D6DC0;

int un_802FF88C(void);
void un_802FF934(void);
int un_802FF958(void);
int un_802FF99C(int arg0);
s32 un_802FF9DC(void);
int un_802FFB58(int arg0);
int un_802FFBAC(int arg0);
bool un_802FFC30(void);
bool un_802FFC6C(bool update_scene);

/// .data
/* "Sound Test Menu" */
u8 un_803F9F28[16] = {
    0x53, 0x6F, 0x75, 0x6E, 0x64, 0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x4D,
    0x65, 0x6E, 0x75, 0x00,
};

/* "Sound Mode " */
u8 un_803F9F38[12] = {
    0x53, 0x6F, 0x75, 0x6E, 0x64, 0x20, 0x4D, 0x6F, 0x64, 0x65, 0x20, 0x00,
};

/* "Mas Volume" */
u8 un_803F9F44[11] = {
    0x4D, 0x61, 0x73, 0x20, 0x56, 0x6F, 0x6C, 0x75, 0x6D, 0x65, 0x00,
};

/* "FGM Volume" */
u8 un_803F9F50[11] = {
    0x46, 0x47, 0x4D, 0x20, 0x56, 0x6F, 0x6C, 0x75, 0x6D, 0x65, 0x00,
};

/* "BGM Volume" */
u8 un_803F9F5C[11] = {
    0x42, 0x47, 0x4D, 0x20, 0x56, 0x6F, 0x6C, 0x75, 0x6D, 0x65, 0x00,
};

/* "DSP Level" */
u8 un_803F9F68[10] = {
    0x44, 0x53, 0x50, 0x20, 0x4C, 0x65, 0x76, 0x65, 0x6C, 0x00,
};

/* "FGM Group" */
u8 un_803F9F74[10] = {
    0x46, 0x47, 0x4D, 0x20, 0x47, 0x72, 0x6F, 0x75, 0x70, 0x00,
};

/* "FGM Name" */
u8 un_803F9F80[9] = {
    0x46, 0x47, 0x4D, 0x20, 0x4E, 0x61, 0x6D, 0x65, 0x00,
};

/* "BGM Name" */
u8 un_803F9F8C[9] = {
    0x42, 0x47, 0x4D, 0x20, 0x4E, 0x61, 0x6D, 0x65, 0x00,
};

/* "-- EXIT --" */
u8 un_803F9F98[11] = {
    0x2D, 0x2D, 0x20, 0x45, 0x58, 0x49, 0x54, 0x20, 0x2D, 0x2D, 0x00,
};

/* Sound-test menu item table (entries reference the un_802FF*
 * callbacks, the label strings above and the value cells). Two
 * runtime-written cells live at +0x2C and +0x38. */
void* un_803F9FA4[97] = {
    NULL, (void*) un_802FF88C, un_803F9F28,
    NULL, NULL, NULL,
    NULL, NULL, (void*) 0x00000002,
    (void*) un_802FF934, un_803F9F38, NULL,
    &un_804D6DBC, NULL, NULL,
    NULL, (void*) 0x00000003, (void*) un_802FF958,
    un_803F9F44, NULL, &un_804D5850,
    NULL, (void*) 0x42FE0000, (void*) 0x3F800000,
    (void*) 0x00000003, (void*) un_802FF958, un_803F9F50,
    NULL, &un_804D5854, NULL,
    (void*) 0x42FE0000, (void*) 0x3F800000, (void*) 0x00000003,
    (void*) un_802FF958, un_803F9F5C, NULL,
    &un_804D5858, NULL, (void*) 0x42FE0000,
    (void*) 0x3F800000, (void*) 0x00000003, (void*) un_802FF99C,
    un_803F9F68, NULL, &un_804D6DAC,
    NULL, (void*) 0x42FE0000, (void*) 0x3F800000,
    (void*) 0x00000002, (void*) un_802FF9DC, un_803F9F74,
    NULL, &un_804D6DB0, NULL,
    (void*) 0x425C0000, (void*) 0x3F800000, (void*) 0x00000002,
    (void*) un_802FFB58, un_803F9F80, NULL,
    &un_804D6DB4, NULL, NULL,
    NULL, (void*) 0x00000002, (void*) un_802FFBAC,
    un_803F9F8C, NULL, &un_804D6DB8,
    NULL, (void*) 0x42C40000, (void*) 0x3F800000,
    (void*) 0x00000001, (void*) un_802FFC6C, un_803F9F98,
    NULL, NULL, NULL,
    NULL, NULL, (void*) 0x00000009,
    (void*) un_802FFC30, NULL, NULL,
    NULL, NULL, NULL,
    NULL, (void*) 0x536D5374, (void*) 0x2E646174,
    NULL, (void*) 0x736D536F, (void*) 0x756E6454,
    (void*) 0x6573744C, (void*) 0x6F616444, (void*) 0x61746100,
    NULL,
};

/* Runtime-written cells inside the menu table (formerly modeled as
 * separate statics; the original keeps them inside un_803F9FA4). */
#define un_803F9FD0 (*(int*) &un_803F9FA4[11])
#define un_803F9FDC (*(float*) &un_803F9FA4[14])
#define un_803FA070 (*(int*) &un_803F9FA4[51])
#define un_803FA090 (*(int*) &un_803F9FA4[59])
#define un_803FA098 (*(float*) &un_803F9FA4[61])
#define un_803FA09C (*(float*) &un_803F9FA4[62])
#define un_803FA0B0 (*(int*) &un_803F9FA4[67])

/* "GmLangTypeJP" */
u8 un_803FA3F4[13] = {
    0x47, 0x6D, 0x4C, 0x61, 0x6E, 0x67, 0x54, 0x79, 0x70, 0x65, 0x4A, 0x50,
    0x00,
};

/* "GmLangTypeUS" */
u8 un_803FA404[13] = {
    0x47, 0x6D, 0x4C, 0x61, 0x6E, 0x67, 0x54, 0x79, 0x70, 0x65, 0x55, 0x53,
    0x00,
};

/* "Battle Royal" */
u8 un_803FB064[13] = {
    0x42, 0x61, 0x74, 0x74, 0x6C, 0x65, 0x20, 0x52, 0x6F, 0x79, 0x61, 0x6C,
    0x00,
};

/* "Team Battle" */
u8 un_803FB074[12] = {
    0x54, 0x65, 0x61, 0x6D, 0x20, 0x42, 0x61, 0x74, 0x74, 0x6C, 0x65, 0x00,
};


/// .sdata (reconstructed)
u8 un_804D5860[5] = { 0x4E, 0x4F, 0x4E, 0x45, 0x00 }; /* "NONE" */
u8 un_804D5868[8] = { 0x41, 0x43, 0x54, 0x49, 0x56, 0x45, 0x44, 0x00 }; /* "ACTIVED" */
u8 un_804D5870[5] = { 0x41, 0x55, 0x54, 0x4F, 0x00 }; /* "AUTO" */
u8 un_804D5878[4] = { 0x4F, 0x46, 0x46, 0x00 }; /* "OFF" */
u8 un_804D587C[3] = { 0x4F, 0x4E, 0x00 }; /* "ON" */
void* un_804D5880[2] = { un_804D5878, un_804D587C };
void* un_804D5888[2] = { un_803FA3F4, un_803FA404 };
u8 un_804D5890[7] = { 0x4D, 0x61, 0x73, 0x74, 0x65, 0x72, 0x00 }; /* "Master" */
u8 un_804D5898[8] = { 0x44, 0x65, 0x76, 0x65, 0x6C, 0x6F, 0x70, 0x00 }; /* "Develop" */
u8 un_804D58A0[8] = { 0x41, 0x64, 0x64, 0x72, 0x34, 0x20, 0x3A, 0x00 }; /* "Addr4 :" */
u8 un_804D58A8[8] = { 0x75, 0x33, 0x32, 0x20, 0x20, 0x20, 0x3A, 0x00 }; /* "u32   :" */
u8 un_804D58B0[8] = { 0x41, 0x64, 0x64, 0x72, 0x32, 0x20, 0x3A, 0x00 }; /* "Addr2 :" */
u8 un_804D58B8[8] = { 0x75, 0x31, 0x36, 0x20, 0x20, 0x20, 0x3A, 0x00 }; /* "u16   :" */
u8 un_804D58C0[8] = { 0x41, 0x64, 0x64, 0x72, 0x20, 0x20, 0x3A, 0x00 }; /* "Addr  :" */
u8 un_804D58C8[8] = { 0x75, 0x38, 0x20, 0x20, 0x20, 0x20, 0x3A, 0x00 }; /* "u8    :" */
u8 un_804D58D0[8] = { 0x4E, 0x6F, 0x74, 0x68, 0x69, 0x6E, 0x67, 0x00 }; /* "Nothing" */
u8 un_804D58D8[8] = { 0x56, 0x65, 0x72, 0x79, 0x6C, 0x6F, 0x77, 0x00 }; /* "Verylow" */
u8 un_804D58E0[4] = { 0x4C, 0x6F, 0x77, 0x00 }; /* "Low" */
u8 un_804D58E4[7] = { 0x4D, 0x69, 0x64, 0x64, 0x6C, 0x65, 0x00 }; /* "Middle" */
u8 un_804D58EC[5] = { 0x48, 0x69, 0x67, 0x68, 0x00 }; /* "High" */
u8 un_804D58F4[5] = { 0x52, 0x75, 0x6C, 0x65, 0x00 }; /* "Rule" */
void* un_804D58FC[2] = { un_803FB064, un_803FB074 };
static s32 un_804D5904[1] = { 0 }; /* keeps un_804D5908 8-aligned (dtk pad) */
s32 un_804D5908 = 1;
s32 un_804D590C = 1;
s32 un_804D5910 = 1;
u8 un_804D5914[7] = { 0x53, 0x49, 0x4D, 0x50, 0x4C, 0x45, 0x00 }; /* "SIMPLE" */
u8 un_804D591C[8] = { 0x43, 0x68, 0x61, 0x72, 0x61, 0x20, 0x3A, 0x00 }; /* "Chara :" */
u8 un_804D5924[8] = { 0x43, 0x6F, 0x6C, 0x6F, 0x72, 0x20, 0x3A, 0x00 }; /* "Color :" */
u8 un_804D592C[7] = { 0x4D, 0x6F, 0x64, 0x65, 0x20, 0x3A, 0x00 }; /* "Mode :" */
u8 un_804D5934[6] = { 0x4C, 0x75, 0x69, 0x67, 0x69, 0x00 }; /* "Luigi" */
u8 un_804D593C[5] = { 0x4D, 0x61, 0x72, 0x73, 0x00 }; /* "Mars" */
u8 un_804D5944[7] = { 0x4D, 0x65, 0x77, 0x74, 0x77, 0x6F, 0x00 }; /* "Mewtwo" */
u8 un_804D594C[6] = { 0x50, 0x75, 0x72, 0x69, 0x6E, 0x00 }; /* "Purin" */
u8 un_804D5954[6] = { 0x46, 0x61, 0x6C, 0x63, 0x6F, 0x00 }; /* "Falco" */
u8 un_804D595C[7] = { 0x43, 0x2D, 0x4C, 0x69, 0x6E, 0x6B, 0x00 }; /* "C-Link" */
u8 un_804D5964[4] = { 0x52, 0x6F, 0x69, 0x00 }; /* "Roi" */
u8 un_804D5968[6] = { 0x50, 0x69, 0x63, 0x68, 0x75, 0x00 }; /* "Pichu" */
u8 un_804D5970[6] = { 0x47, 0x61, 0x6E, 0x6F, 0x6E, 0x00 }; /* "Ganon" */

/* 3FA128 */ static struct {
    u8 _pad0[0x220];
    u16 x220;
    u8 _pad222[0x2];
    u8 x224;
    u8 x225;
    u8 x226;
    u8 x227;
} un_803FA128;
/* 3FA258 */ static struct un_803FA258_t {
    int x0;
    int x4[4];
    int x14[4];
    int x24[4];
    int x34_pad;
    int x38[4];
    int x48[4];
    int x58[4];
    int x68[4];
    f32 x78[4];
    f32 x88[4];
    f32 x98[4];
    int xA8[4];
    int xB8[4];
    int xC8;
    int xCC[4];
    int xDC[4];
    int xEC[3];
    int xF8;
    int xFC;
    int x100;
    int x104;
    int x108;
    int x10C;
    int x110;
    int x114;
    int x118;
    int x11C;
    int x120;
    int x124;
    int x128;
    int x12C;
    int x130;
    u8 _pad134[0x4];
    void* x138;
    u8 _pad13C[0x8];
    u8 x144[0x44];
    s32 x188;
} un_803FA258;
/* 3FA348 */ static u16 un_803FA348;
/* 3FA34C */ static u8 un_803FA34C;
/* 3FA32C */ static u8 un_803FA32C;
/* 3FA658 */ static u8 un_803FA658[0x290] = { 0 };
/* 3FA8E8 */ static u8 un_803FA8E8[0x15C] = { 0 };
/* 3FAA44 */ static u8 un_803FAA44[0xC0] = { 0 };
/* 3FB168 */ static u8 un_803FB168[0x4A4] = { 0 };
/* 3FB60C */ static u8 un_803FB60C[0xE0] = { 0 };
/* 3FB728 */ static u8 un_803FB728[0xC0] = { 0 };
/* 3FB870 */ static u8 un_803FB870[0xE0] = { 0 };
/* 3FB98C */ static u8 un_803FB98C[0xC0] = { 0 };
/* 3FBA9C */ static u8 un_803FBA9C[0xC0] = { 0 };
/* 3FBB98 */ static u8 un_803FBB98[0xC0] = { 0 };
/* 3FBCAC */ static u8 un_803FBCAC[0xC0] = { 0 };
/* 3FBDC0 */ static u8 un_803FBDC0[0xC0] = { 0 };
/* 3FBFFC */ static u8 un_803FBFFC[0xC0] = { 0 };
/* 3FC0FC */ static u8 un_803FC0FC[0xC0] = { 0 };
/* 3FC22C */ static u8 un_803FC22C[0x1F4] = { 0 };
/* 3FC63C */ static u8 un_803FC63C[0x80] = { 0 };
/* 3FC70C */ static u8 un_803FC70C[0x11C] = { 0 };
/* 3FC828 */ static u8 un_803FC828[0xA0] = { 0 };
/* 3FC8C8 */ static u8 un_803FC8C8[0x168] = { 0 };
/* 3FCA40 */ static u8 un_803FCA40[0x1A0] = { 0 };
/* 3FCC38 */ static u8 un_803FCC38[0xFC] = { 0 };
/* 3FCD34 */ static u8 un_803FCD34[0x118] = { 0 };
/* 3FCE4C */ static u8 un_803FCE4C[0xE0] = { 0 };
/* 3FD064 */ static u8 un_803FD064[0x1C0] = { 0 };
/* 3FD310 */ static u8 un_803FD310[0x27C] = { 0 };
/* 3FD58C */ static u8 un_803FD58C[0x2D8] = { 0 };
/* 3FD864 */ static u8 un_803FD864[0x294] = { 0 };
/* 3FDAF8 */ static u8 un_803FDAF8[0x80] = { 0 };
/* 3FDB9C */ static u8 un_803FDB9C[0x84] = { 0 };
/* 3FD224 */ extern char un_803FD224[];
/* 3FD230 */ extern char un_803FD230[];
/* 3FD23C */ extern char un_803FD23C[];
/* 3FD24C */ extern char un_803FD24C[];
/* 3FD258 */ extern char un_803FD258[];
/* 3FD264 */ extern char un_803FD264[];
/* 3FD274 */ extern void* un_803FD274[6];
/* 3FD28C */ extern char un_803FD28C[];
/* 3FD29C */ extern char un_803FD29C[];
/* 3FD2AC */ extern char un_803FD2AC[];

/// .sbss (extern)
/* 4D6DC8 */ extern s32 un_804D6DC8;
/* 4D6DCC */ extern s32 un_804D6DCC;
/* 4D6DD0 */ extern float un_804D6DD0;
/* 4D6DD8 */ extern s32 un_804D6DD8;
/* 4D6DDC */ extern s32 un_804D6DDC;
/* 4D6DE0 */ extern s32 un_804D6DE0;
/* 4D6DE4 */ extern s32 un_804D6DE4;
/* 4D6DE8 */ extern s32 un_804D6DE8;
/* 4D6DEC */ extern s32 un_804D6DEC;
/* 4D6DF0 */ extern s32 un_804D6DF0;
/* 4D6DF4 */ extern s32 un_804D6DF4;
/* 4D6DF8 */ extern s32 un_804D6DF8;
/* 4D6DFC */ extern s32 un_804D6DFC;
/* 4D6E00 */ extern s32 un_804D6E00;
/* 4D6E04 */ extern void* un_804D6E04;
/* 4D6E08 */ extern HSD_GObj* un_804D6E08;
/* 4D6E0C */ extern s32 un_804D6E0C;
/* 4D6E10 */ extern s32 un_804D6E10;
/* 4D6E14 */ extern s32 un_804D6E14;



/// .bss (extern)
/* 45A6C0 */ extern u8 gmMainLib_8045A6C0[];

/// .sdata2 (extern)
/* 4DDC38 */ extern float un_804DDC38;
/* 4DDC3C */ extern float un_804DDC3C;
/* 4DDC48 */ extern float un_804DDC48;
/* 4DDC4C */ extern float un_804DDC4C;
/* 4DDC50 */ extern float un_804DDC50;

void un_802FF7DC(void)
{
    lbArchive_LoadSymbols("SmSt.dat", &un_804D6DA8, "smSoundTestLoadData", 0);
    un_803F9FDC = un_804D6DA8[0];
    un_803F9FD0 = un_804D6DA8[1];
    un_803FA070 = un_804D6DA8[2];
    un_803FA090 = un_804D6DA8[3];
    un_803FA09C = un_804D6DA8[4];
    un_803FA0B0 = un_804D6DA8[5];
}

bool un_802FF884(char* unused)
{
    return false;
}

int un_802FF88C(void)
{
    un_804D5858 = 0x7F;
    un_804D6DB8 = 0;
    un_804D5854 = 0x7F;
    un_804D5850 = 0x7F;
    un_804D6DB4 = 0;
    un_804D6DB0 = 0;
    un_804D6DC0 = 0;
    un_804D585C = -1;
    un_804D6DBC = lbAudioAx_80024BD0();
    lbAudioAx_800236DC();
    lbAudioAx_800245D4(0x7F);
    lbAudioAx_800245F4(0x7F);
    lbAudioAx_80024614(0x7F);
    if (un_80302DF0()) {
        un_80302DF8(un_80302DF0(), fn_802FFCC8);
    }
    {
        un_80304138_objalloc_t* x = un_80302DF0();
        x->x4->scale_x = un_804DDC38;
        x->x4->scale_y = un_804DDC3C;
    }
    return 1;
}

void un_802FF934(void)
{
    lbAudioAx_80024C08(un_804D6DBC);
}

int un_802FF958(void)
{
    u8 tmp = un_804D5854;
    lbAudioAx_800245F4(tmp);
    tmp = un_804D5858;
    lbAudioAx_80024614(tmp);
    tmp = un_804D5850;
    lbAudioAx_800245D4(tmp);
    return 0;
}

int un_802FF99C(int arg0)
{
    u8 tmp = un_804D6DAC;
    lbAudioAx_80024634(tmp);
    un_802FFB58(arg0);
    return 0;
}

s32 un_802FF9DC(void)
{
    struct SmStData {
        u8 pad0[0x18];
        s32* x18;
    }* d = (struct SmStData*) un_804D6DA8;
    s32 i;

    un_804D6DB4 = 0;
    for (i = 0; i < un_804D6DB0; i++) {
        un_804D6DB4 += d->x18[i];
    }
    un_803FA098 = (f32) un_804D6DB4;
    un_803FA09C = (f32) (un_804D6DB4 + d->x18[un_804D6DB0]);
    return 0;
}

int un_802FFB58(int arg0)
{
    if (arg0 == 1) {
        lbAudioAx_80023694();
        lbAudioAx_80023B24(((int**) un_804D6DA8)[5][un_804D6DB4]);
    } else if (arg0 == 0) {
        lbAudioAx_80023694();
    }
    return 0;
}

int un_802FFBAC(int arg0)
{
    if (arg0 == 1) {
        if (un_804D585C == un_804D6DB8) {
            int x = (un_804D6DC0 + 1) & 1;
            un_804D6DC0 = x;
            lbAudioAx_80024E50(x);
        } else {
            lbAudioAx_80023F28(un_804D6DB8);
            un_804D585C = un_804D6DB8;
            un_804D6DC0 = 0;
        }
    } else if (arg0 == 0) {
        lbAudioAx_800236DC();
        un_804D585C = -1;
        un_804D6DC0 = 0;
    }
    return 0;
}

bool un_802FFC30(void)
{
    lbAudioAx_800236DC();
    lbAudioAx_800245D4(0x7F);
    lbAudioAx_800245F4(0x7F);
    lbAudioAx_80024614(0x7F);
    return true;
}

bool un_802FFC6C(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80023694();
        lbAudioAx_80024030(0);
        lbAudioAx_800236DC();
        lbAudioAx_800245D4(0x7F);
        lbAudioAx_800245F4(0x7F);
        lbAudioAx_80024614(0x7F);
        gm_801A42F8(GM_TITLE);
        gm_801A4B60();
    }
    return true;
}

bool fn_802FFCC8(void)
{
    return false;
}

#pragma dont_inline on
void un_802FFCD0(int count, void* data)
{
    int* arr = data;
    int i;
    for (i = 0; i < count; i++) {
        arr[i] = arr[count];
    }
}
#pragma dont_inline reset

void un_802FFD94(int arg0, void* arg1, void* arg2)
{
    struct un_80304138_objalloc_t* d;
    if (arg0 == 1) {
        d = un_80302DF0();
        lbAudioAx_80024030(1);
        un_80304210(d, arg1, 0, -60, 0);
        un_80302DF8(un_80302DF0(), arg2);
    }
}

int fn_802FFE0C(int arg0)
{
    switch (arg0) {
    case 0:
        lbAudioAx_80024030(0);
        un_80304334(un_80302DF0());
        break;
    case 6:
        lbAudioAx_80024030(1);
        gm_SetPendingScene(4);
        gm_801A4B60();
        break;
    }
    return 0;
}

bool fn_802FFE6C(bool update_scene)
{
    bool res = update_scene;
    if (update_scene == false) {
        lbAudioAx_80024030(0);
        un_80304334(un_80302DF0());
        res = false;
    }

    return res;
}

bool un_802FFEA4(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0x4);
        gm_801A4B60();
    }

    return false;
}

void un_802FFEE0(s32* arg0)
{
#define ST_MIN(a, b) ((a) < (b) ? (a) : (b))
    u16 val = ST_MIN((u16) un_803FA258.x12C, 0x3E);
#undef ST_MIN

    ((u16*) arg0)[0] = val;
    ((u16*) arg0)[1] = (u16) un_803FA258.x130;
    arg0[1] = 0x98967F;
    arg0[2] = 0;
}

void un_802FFF2C(StartMeleeData* arg0)
{
    StartMeleeRules* r = &arg0->rules;
    struct un_803FA258_t* s = &un_803FA258;
    PlayerInitData* p;
    s32 i;
    s32 timer;

    gm_80167A64(r);
    r->x2_2 = 0;
    r->is_teams = s->x4[1];
    switch (s->xC8) {
    case 0:
        r->x0_0 = 0;
        timer = s->xCC[1] + s->xCC[0] * 0x3C;
        if (timer != 0) {
            r->x10 = timer;
            r->x0_6 = 1;
        } else {
            r->x0_6 = 0;
        }
        break;
    case 1:
        r->x0_0 = 1;
        r->x0_6 = 0;
        break;
    case 2:
        r->x0_0 = 2;
        timer = s->xCC[1] + s->xCC[0] * 0x3C;
        if (timer != 0) {
            r->x10 = timer;
            r->x0_6 = 1;
        } else {
            r->x0_6 = 0;
        }
        break;
    default:
        r->x0_0 = 0;
        r->x0_6 = 0;
        break;
    }
    r->xE = s->x4[1];
    r->x20 = -1;
    r->xB = s->xDC[0] - 1;
    r->xC = -1;
    r->x30 = s->xEC[0];
    gm_80167A14(arg0->players);
    for (i = 0; i < 2; i++) {
        arg0->players[2 * i].c_kind = s->x4[2 * i];
        arg0->players[2 * i].slot_type = s->x24[2 * i];
        arg0->players[2 * i].color = s->x38[2 * i];
        arg0->players[2 * i].sub_color = s->x48[2 * i];
        arg0->players[2 * i].team = s->x58[2 * i];
        arg0->players[2 * i].xC_b0 = un_803FA32C;
        arg0->players[2 * i].x12 = s->x68[2 * i];
        arg0->players[2 * i].x18 = s->x78[2 * i];
        arg0->players[2 * i].x1C = s->x88[2 * i];
        arg0->players[2 * i].xE = s->xA8[2 * i];
        arg0->players[2 * i].cpu_level = s->xB8[2 * i];
        arg0->players[2 * i].stocks = un_803FA32C;
        arg0->players[2 * i].xC_b1 = 0;
        arg0->players[2 * i].x20 = s->x98[2 * i];
        arg0->players[2 * i + 1].c_kind = s->x4[2 * i + 1];
        arg0->players[2 * i + 1].slot_type = s->x24[2 * i + 1];
        arg0->players[2 * i + 1].color = s->x38[2 * i + 1];
        arg0->players[2 * i + 1].sub_color = s->x48[2 * i + 1];
        arg0->players[2 * i + 1].team = s->x58[2 * i + 1];
        arg0->players[2 * i + 1].xC_b0 = un_803FA32C;
        arg0->players[2 * i + 1].x12 = s->x68[2 * i + 1];
        arg0->players[2 * i + 1].x18 = s->x78[2 * i + 1];
        arg0->players[2 * i + 1].x1C = s->x88[2 * i + 1];
        arg0->players[2 * i + 1].xE = s->xA8[2 * i + 1];
        arg0->players[2 * i + 1].cpu_level = s->xB8[2 * i + 1];
        arg0->players[2 * i + 1].stocks = un_803FA32C;
        arg0->players[2 * i + 1].xC_b1 = 0;
        arg0->players[2 * i + 1].x20 = s->x98[2 * i + 1];
    }
}

bool un_803001DC(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0x1);
        gm_801A4B60();
    }

    return false;
}

int un_80300218(void)
{
    lbLang_SetSavedLanguage(un_803FA258.x0);
    return 0;
}

int un_80300248(int arg0)
{
    if (un_803FA258.x4[0] && arg0 == 1) {
        lbAudioAx_80024030(1);
        gmMainLib_8015FB68();
    }
    return 0;
}

int un_80300290(int arg0)
{
    if (arg0 == 1) {
        struct un_80304138_objalloc_t* x = un_80302DF0();
        lbAudioAx_80024030(1);
        un_80304210(x, &un_803FA658, 0, -60, 0);
        un_80302DF8(un_80302DF0(), fn_802FFE6C);
    }
    return 0;
}

bool un_803002FC(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0x2);
        gm_801A4B60();
    }

    return false;
}

s32 un_80300338(void)
{
    u8* src;

    src = gmMainLib_8045A6C0;
    src = src + un_803FA128.x220;

    un_803FA128.x224 = src[0x1868];
    un_803FA128.x225 = src[0x1869];
    un_803FA128.x226 = src[0x186A];
    un_803FA128.x227 = src[0x186B];
    return 0;
}

s32 un_80300378(void)
{
    u8* ptr;

    un_803FA128.x220 &= 0xFFFE;

    ptr = gmMainLib_8045A6C0;
    ptr = ptr + un_803FA128.x220;

    un_803FA128.x224 = ptr[0x1868];
    un_803FA128.x225 = ptr[0x1869];
    un_803FA128.x226 = ptr[0x186A];
    un_803FA128.x227 = ptr[0x186B];

    return 0;
}

s32 un_803003C4(void)
{
    u8* ptr;

    un_803FA128.x220 &= 0xFFFC;

    ptr = gmMainLib_8045A6C0;
    ptr = ptr + un_803FA128.x220;

    un_803FA128.x224 = ptr[0x1868];
    un_803FA128.x225 = ptr[0x1869];
    un_803FA128.x226 = ptr[0x186A];
    un_803FA128.x227 = ptr[0x186B];

    return 0;
}

s32 un_80300410(s32 arg0)
{
    if (arg0 == 1) {
        u8* dst;
        lbAudioAx_80024030(1);
        dst = gmMainLib_8045A6C0;
        dst = dst + un_803FA128.x220;
        dst[0x1868] = un_803FA128.x224;
        dst[0x1869] = un_803FA128.x225;
        dst[0x186A] = un_803FA128.x226;
        dst[0x186B] = un_803FA128.x227;
    }
    return 0;
}

/// these will try to inline un_802FFD94 otherwise
#pragma push
#pragma dont_inline on
int un_80300480(int arg0)
{
    un_802FFD94(arg0, &un_803FB168, fn_802FFE0C);
    return 0;
}

int un_803004B4(int arg0)
{
    un_802FFD94(arg0, &un_803FA8E8, fn_802FFE6C);
    return 0;
}
#pragma pop

#pragma dont_inline on
int un_803004E8(int arg0)
{
    un_802FFD94(arg0, &un_803FAA44, fn_802FFE6C);
    return 0;
}

int un_8030051C(int arg0)
{
    un_802FFD94(arg0, &un_803FB60C, fn_802FFE6C);
    return 0;
}

int un_80300550(int arg0)
{
    un_802FFD94(arg0, &un_803FB728, fn_802FFE6C);
    return 0;
}

int un_80300584(int arg0)
{
    un_802FFD94(arg0, &un_803FB870, fn_802FFE6C);
    return 0;
}

int un_803005B8(int arg0)
{
    un_802FFD94(arg0, &un_803FB98C, fn_802FFE6C);
    return 0;
}

int un_803005EC(int arg0)
{
    un_802FFD94(arg0, &un_803FBA9C, fn_802FFE6C);
    return 0;
}

int un_80300620(int arg0)
{
    un_802FFD94(arg0, &un_803FBB98, fn_802FFE6C);
    return 0;
}

int un_80300654(int arg0)
{
    un_802FFD94(arg0, &un_803FBCAC, fn_802FFE6C);
    return 0;
}

int un_80300688(int arg0)
{
    un_802FFD94(arg0, &un_803FBDC0, fn_802FFE6C);
    return 0;
}

int un_803006BC(int arg0)
{
    un_802FFD94(arg0, &un_803FBFFC, fn_802FFE6C);
    return 0;
}

int un_803006F0(int arg0)
{
    un_802FFD94(arg0, &un_803FC0FC, fn_802FFE6C);
    return 0;
}

int un_80300724(int arg0)
{
    un_802FFD94(arg0, &un_803FC22C, fn_802FFE6C);
    return 0;
}
#pragma dont_inline reset

void un_80300758(int arg0)
{
    if (arg0 == 1) {
        un_802FFCD0(4, (u8*) &un_803FA258 + 0x10);
    }
}

void un_80300790(int arg0)
{
    if (arg0 == 1) {
        un_802FFCD0(4, (u8*) &un_803FA258 + 0x24);
    }
}

#pragma dont_inline on
int un_803007C8(int arg0)
{
    un_802FFD94(arg0, &un_803FC63C, fn_802FFE6C);
    return 0;
}

int un_803007FC(int arg0)
{
    un_802FFD94(arg0, &un_803FCA40, fn_802FFE6C);
    return 0;
}

int un_80300830(int arg0)
{
    un_802FFD94(arg0, &un_803FCC38, fn_802FFE6C);
    return 0;
}

int un_80300864(int arg0)
{
    un_802FFD94(arg0, &un_803FCE4C, fn_802FFE6C);
    return 0;
}

int un_80300898(int arg0)
{
    un_802FFD94(arg0, &un_803FD064, fn_802FFE6C);
    return 0;
}

int un_803008CC(int arg0)
{
    un_802FFD94(arg0, &un_803FDAF8, fn_802FFE6C);
    return 0;
}

int un_80300900(int arg0)
{
    un_802FFD94(arg0, &un_803FDB9C, fn_802FFE6C);
    return 0;
}

int un_80300934(int arg0)
{
    un_802FFD94(arg0, &un_803FD310, fn_802FFE6C);
    return 0;
}
#pragma dont_inline reset

bool un_80300968(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_801A42F8(GM_HANYU_CSS);
        gm_801A4B60();
    }

    return false;
}

bool un_803009A4(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_801A42F8(GM_HANYU_SSS);
        gm_801A4B60();
    }

    return false;
}

s32 un_803009E0(void)
{
    s32* new_var;
    switch (un_804D6DD8) {
    case 2:
        new_var = gmMainLib_8015D06C((u8) un_804D6DC8);
        {
            u32 val = (u32) (*new_var);
            un_804D6DD0 = (float) (val / 100U);
            break;
        }

    case 3: {
        s32 result = gmMainLib_8015D48C((u8) un_804D6DC8);
        gmMainLib_8015D4E8((u8) un_804D6DC8, result);
        un_804D6DCC = result;
        break;
    }
    }
    return 0;
}

int un_80300A88(void)
{
    switch (un_804D6DD8) {
    case 2:
        break;
    case 3:
        if (un_804D6DCC >= 1) {
            un_804D6DCC = 1;
        }
        break;
    }
    return 0;
}

bool un_80300AB8(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0x4);
        gm_801A4B60();
    }

    return false;
}

bool un_80300AF4(int arg0)
{
    if (arg0 == 1) {
        lbAudioAx_80024030(1);
        un_803FA258.x4[1] = 0x3F;
        un_803FA258.x4[2] = 0xE;
        un_803FA258.x24[1] = 3;
        un_803FA258.x24[2] = 3;
        un_803FA258.x24[3] = 3;
        gm_SetPendingScene(4);
        gm_801A4B60();
    }
    return false;
}

bool un_80300B58(int arg0)
{
    if (arg0 == 1) {
        lbAudioAx_80024030(1);
        un_803FA258.x4[1] = 0x3B;
        un_803FA258.x4[2] = 0x2;
        un_803FA258.x24[1] = 3;
        un_803FA258.x24[2] = 3;
        gm_SetPendingScene(4);
        gm_801A4B60();
    }
    return false;
}

bool un_80300BBC(bool update_scene)
{
    if (update_scene == true) {
        un_803124BC();
        un_80311680();
        gm_801A42F8(GM_TOY_GALLERY);
        gm_801A4B60();
    }

    return false;
}

bool un_80300BF8(bool update_scene)
{
    if (update_scene == true) {
        un_803124BC();
        Trophy_SetUnlockState(0xE6, true);
        gm_801A42F8(GM_MENU);
        gm_801A4B60();
    }

    return false;
}

bool un_80300C3C(bool update_scene)
{
    if (update_scene == true) {
        un_803124BC();
        Trophy_SetUnlockState(0xC9, true);
        gm_801A42F8(GM_MENU);
        gm_801A4B60();
    }

    return false;
}

bool un_80300C80(int arg0)
{
    if (arg0 == 1) {
        u32* ptr = gmMainLib_8015CCF0();
        *ptr = un_804D6DDC * 10;
        un_803124BC();
        gm_801A42F8(1);
        gm_801A4B60();
    }
    return false;
}

void fn_80300CC8(int arg0)
{
    int* r29;
    int* r30;
    int* r31;

    switch (arg0) {
    case 0:
        lbAudioAx_80024030(0);
        un_80304334(un_80302DF0());
        break;
    case 6:
        lbAudioAx_80024030(1);
        r29 = gmMainLib_GetTimeMatchTotal();
        r30 = gmMainLib_GetStockMatchTotal();
        r31 = gmMainLib_GetCoinMatchTotal();
        {
            int* ptr = gmMainLib_GetBonusMatchTotal();
            *r29 = un_804D6DE0;
            *r30 = un_804D6DE4;
            *r31 = un_804D6DE8;
            *ptr = un_804D6DEC;
        }
        gm_801A42F8(1);
        gm_801A4B60();
        break;
    }
}

#pragma dont_inline on
int un_80300D78(int arg0)
{
    if (arg0 == 1) {
        lbAudioAx_80024030(1);
        un_804D6DE0 = un_804D6DF0;
        un_804D6DE4 = un_804D6DF0;
        un_804D6DE8 = un_804D6DF0;
        un_804D6DEC = un_804D6DF0;
        un_802FFD94(arg0, &un_803FC70C, fn_80300CC8);
    }
    return 0;
}
#pragma dont_inline reset

void fn_80300DE0(int arg0)
{
    s32* ptr;

    ptr = gmMainLib_8015D06C(un_804D6DC8);
    switch (arg0) {
    case 0:
        lbAudioAx_80024030(0);
        un_80304334(un_80302DF0());
        break;
    case 6:
        lbAudioAx_80024030(1);
        *ptr = (u32) (un_804DDC48 * un_804D6DD0);
        gm_801A42F8(1);
        gm_801A4B60();
        break;
    }
}

#pragma dont_inline on
int un_80300E74(int arg0)
{
    if (arg0 == 1) {
        lbAudioAx_80024030(1);
        un_804D6DD8 = 2;
        un_802FFD94(arg0, &un_803FC8C8, fn_80300DE0);
    }
    return 0;
}
#pragma dont_inline reset

void fn_80300ED0(int arg0)
{
    switch (arg0) {
    case 0:
        lbAudioAx_80024030(0);
        un_80304334(un_80302DF0());
        break;
    case 6:
        lbAudioAx_80024030(1);
        gmMainLib_8015D4E8(un_804D6DC8, un_804D6DCC);
        gm_801A42F8(1);
        gm_801A4B60();
        break;
    }
}

#pragma dont_inline on
int un_80300F3C(int arg0)
{
    if (arg0 == 1) {
        lbAudioAx_80024030(1);
        un_804D6DD8 = 3;
        un_802FFD94(arg0, &un_803FC828, fn_80300ED0);
    }
    return 0;
}
#pragma dont_inline reset

s32 un_80300F98(s32 arg0)
{
    s32* temp_r31;

    if (arg0 == 1) {
        temp_r31 = gmMainLib_GetKOTotal();
        lbAudioAx_80024030(1);
        *temp_r31 = un_804D6DF4;
        gm_801A42F8(1);
        gm_801A4B60();
    }
    return 0;
}

bool un_80300FEC(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0x4);
        gm_801A4B60();
    }

    return false;
}

int un_80301028(int arg0)
{
    if (arg0 == 1) {
        struct gmm_x0_528_t* ptr = gmMainLib_8015CDD4();
        ptr->x5 = un_804D5908 - 1;
        lbAudioAx_80024030(1);
        gm_801A42F8(4);
        gm_801A4B60();
    }
    return 0;
}

int un_80301074(int arg0)
{
    if (arg0 == 1) {
        struct gmm_x0_528_t* ptr = gmMainLib_8015CDC8();
        ptr->x5 = un_804D590C - 1;
        lbAudioAx_80024030(1);
        gm_801A42F8(3);
        gm_801A4B60();
    }
    return 0;
}

int un_803010C0(int arg0)
{
    if (arg0 == 1) {
        struct gmm_x0_528_t* ptr = gmMainLib_8015CDE0();
        ptr->x5 = un_804D5910 - 1;
        lbAudioAx_80024030(1);
        gm_801A42F8(5);
        gm_801A4B60();
    }
    return 0;
}

int fn_8030110C(int arg0)
{
    switch (arg0) {
    case 0:
        lbAudioAx_80024030(0);
        un_80304334(un_80302DF0());
        break;
    case 6:
        lbAudioAx_80024030(1);
        gm_801BEFA4(un_804D6DF8);
        gm_801BEFC0(un_804D6DFC);
        {
            struct gmm_x0_528_t* ptr = gmMainLib_8015CDC8();
            ptr->c_kind = (s8) un_804D6DF8;
            ptr->color = (u8) un_804D6DFC;
        }
        switch (un_804D6E00) {
        case 0:
            gm_801A42F8(0x1A);
            gm_801BF040(0x15);
            break;
        case 1:
            gm_801A42F8(0x1A);
            gm_801BF040(0x16);
            break;
        case 2:
            gm_801A42F8(0x1A);
            gm_801BF040(0x17);
            break;
        }
        gm_801A4B60();
        break;
    }
    return 0;
}

void fn_803011EC(int arg0)
{
    switch (arg0) {
    case 0:
        lbAudioAx_80024030(0);
        un_80304334(un_80302DF0());
        break;
    case 6:
        lbAudioAx_80024030(1);
        gm_801BEFA4(un_804D6DF8);
        gm_801BEFC0(un_804D6DFC);
        {
            s8 c_kind = (s8) un_804D6DF8;
            gm_8017EB30()->x0.ckind = c_kind;
        }
        {
            u8 color = (u8) un_804D6DFC;
            gm_8017EB30()->x0.color = color;
        }
        gm_801A42F8(0x15);
        gm_801A4B60();
        break;
    }
}

#pragma dont_inline on
int un_80301280(int arg0)
{
    if (arg0 == 1) {
        lbAudioAx_80024030(1);
        un_802FFD94(arg0, &un_803FCD34, fn_803011EC);
    }
    return 0;
}

int un_803012D4(int arg0)
{
    if (arg0 == 1) {
        lbAudioAx_80024030(1);
        un_802FFD94(arg0, &un_803FCD34, fn_8030110C);
    }
    return 0;
}
#pragma dont_inline reset

bool un_80301328(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_801BF708(0x0);
        gm_801A42F8(GM_OPENING_MV);
        gm_801A4B60();
    }

    return false;
}

bool un_8030136C(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_801BF708(0x5);
        gm_801A42F8(GM_OPENING_MV);
        gm_801A4B60();
    }

    return false;
}

bool un_803013B0(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_801A42F8(GM_DEBUG_CUTSCENE);
        gm_801A4B60();
    }

    return false;
}

#pragma dont_inline on
int un_803013EC(int arg0)
{
    un_802FFD94(arg0, &un_803FD58C, un_80301B48);
    return 0;
}

int un_80301420(int arg0)
{
    un_802FFD94(arg0, &un_803FD864, un_80301C80);
    return 0;
}
#pragma dont_inline reset

bool un_80301454(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_801A42F8(GM_BOOT);
        gm_801A4B60();
    }

    return false;
}

bool un_80301490(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0xD);
        gm_801A4B60();
    }

    return false;
}

bool un_803014CC(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0x7);
        gm_801A4B60();
    }

    return false;
}

bool un_80301508(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0x3);
        gm_801A4B60();
    }

    return false;
}

bool un_80301544(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0x9);
        gm_801A4B60();
    }

    return false;
}

bool un_80301580(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0xC);
        gm_801A4B60();
    }

    return false;
}

bool un_803015BC(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_801A42F8(GM_PROGRESSIVE_SCAN);
        gm_801A4B60();
    }

    return false;
}

bool un_803015F8(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0xA);
        gm_801A4B60();
    }

    return false;
}

int un_80301634(void)
{
    HSD_GObj* gobj;
    void* r31;
    void* r3;
    PAD_STACK(8);

    OSReport("<Init>\n");
    lb_8001C550();
    lb_8001D164(0);
    r31 = HSD_MemAlloc(lbSnap_8001E210());
    r3 = HSD_MemAlloc(lbSnap_8001E204());
    lbSnap_8001E218(r3, r31);
    un_804D6E04 = HSD_MemAlloc(0x96000);
    gobj = GObj_Create(0x13, 0x14, 0);
    HSD_SObjLib_803A55DC(gobj, 0x280, 0x1E0, 0xC);
    gobj->gxlink_prios = 0x40000;
    un_804D6E08 = 0;
    return 0;
}

int un_803016D8(int arg0)
{
    if (arg0 != 1) {
        return 0;
    }
    OSReport(un_803FD224);
    {
        s32 result = lb_8001B8C8(0);
        OSReport(un_803FD230, result);
    }
    return 0;
}

int un_80301734(int arg0)
{
    if (arg0 != 1) {
        return 0;
    }
    OSReport(un_803FD23C);
    {
        s32 result = lb_8001C87C();
        OSReport(un_803FD230, result);
    }
    return 0;
}

int un_8030178C(int arg0)
{
    if (arg0 != 1) {
        return 0;
    }
    OSReport(un_803FD24C);
    {
        s32 result = lb_8001C8BC();
        OSReport(un_803FD230, result);
        if (result == 0) {
            lb_8001CBAC(0);
        }
    }
    return 0;
}

int un_80301800(int arg0)
{
    if (arg0 != 1) {
        return 0;
    }
    OSReport("<Save>\n");
    lb_8001CE00();
    return 0;
}

int un_80301840(int arg0)
{
    if (arg0 != 1) {
        return 0;
    }
    OSReport("<Load>\n");
    {
        s32 result = lb_8001CBBC();
        gmMainLib_8015FA34(result);
        if (result == 0 || result == 2) {
            lb_8001CBAC(0);
        }
        OSReport(un_803FD230, result);
    }
    return 0;
}

void* un_803FD274[6] = { NULL, (void*) 0x028001E0, (void*) 0x00000004,
                          NULL, NULL, NULL };

void* un_804D5990[2] = { un_803FD274, NULL };
u8 un_804D5998[5] = { 0x49, 0x6E, 0x69, 0x74, 0x00 }; /* "Init" */
u8 un_804D59A0[7] = { 0x46, 0x6F, 0x72, 0x6D, 0x61, 0x74, 0x00 }; /* "Format" */
u8 un_804D59A8[7] = { 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x00 }; /* "Create" */
u8 un_804D59B0[5] = { 0x53, 0x61, 0x76, 0x65, 0x00 }; /* "Save" */
u8 un_804D59B8[5] = { 0x4C, 0x6F, 0x61, 0x64, 0x00 }; /* "Load" */
u8 un_804D59C0[7] = { 0x44, 0x65, 0x6C, 0x65, 0x74, 0x65, 0x00 }; /* "Delete" */
u8 un_804D59C8[7] = { 0x4E, 0x6F, 0x72, 0x6D, 0x61, 0x6C, 0x00 }; /* "Normal" */
u8 un_804D59D0[5] = { 0x54, 0x65, 0x61, 0x6D, 0x00 }; /* "Team" */
u8 un_804D59D8[6] = { 0x47, 0x69, 0x61, 0x6E, 0x74, 0x00 }; /* "Giant" */
u8 un_804D59E0[6] = { 0x42, 0x6F, 0x6E, 0x75, 0x73, 0x00 }; /* "Bonus" */
u8 un_804D59E8[6] = { 0x43, 0x6F, 0x72, 0x70, 0x73, 0x00 }; /* "Corps" */
u8 un_804D59F0[7] = { 0x54, 0x61, 0x72, 0x67, 0x65, 0x74, 0x00 }; /* "Target" */
u8 un_804D59F8[7] = { 0x54, 0x45, 0x53, 0x54, 0x20, 0x3E, 0x00 }; /* "TEST >" */
u8 un_804D5A00[2] = { 0x30, 0x00 }; /* "0" */
u8 un_804D5A08[4] = { 0x40, 0x50, 0x80, 0x80 };
u8 un_804D5A0C[4] = { 0xE2, 0xE2, 0xE2, 0xFF };
u8 un_804D5A10[4] = { 0xFF, 0x80, 0x20, 0xFF };
u8 un_804D5A14[4] = { 0xA0, 0xA0, 0xFF, 0xFF };
u8 un_804D5A18[3] = { 0x25, 0x64, 0x00 }; /* "%d" */
u8 un_804D5A1C[5] = { 0x25, 0x30, 0x32, 0x78, 0x00 }; /* "%02x" */
u8 un_804D5A24[5] = { 0x25, 0x30, 0x34, 0x78, 0x00 }; /* "%04x" */
u8 un_804D5A2C[5] = { 0x25, 0x30, 0x38, 0x78, 0x00 }; /* "%08x" */
u8 un_804D5A34[6] = { 0x25, 0x33, 0x2E, 0x32, 0x66, 0x00 }; /* "%3.2f" */

int un_803018BC(int arg0)
{
    if (arg0 != 1) {
        return 0;
    }
    OSReport(un_803FD258);
    {
        s32 result = lb_8001CC4C();
        OSReport(un_803FD230, result);
    }
    lb_8001CBAC(4);
    return 0;
}

int un_8030191C(int arg0)
{
    if (arg0 != 1) {
        return 0;
    }
    OSReport(un_803FD264);
    lbSnap_8001D40C(0);
    return 0;
}

int un_80301964(int arg0)
{
    if (arg0 != 1) {
        return 0;
    }

    OSReport(un_803FD28C);

    if (lbSnap_8001E058(0, un_804D6E0C) == 0xB) {
        s32 result;
        do {
            result = lb_8001B6F8();
        } while (result == 0xB);

        if (result == 0 && lbSnap_8001DE8C(un_804D6E04) != 0) {
            HSD_GObj* gobj;
            HSD_SObj* sobj;

            if (un_804D6E08 != NULL) {
                HSD_GObjPLink_80390228(un_804D6E08);
            }

            gobj = GObj_Create(0xE, 0xF, 0);
            GObj_SetupGXLink(gobj, HSD_SObjLib_803A49E0, 0x12, 0);
            un_803FD274[0] = un_804D6E04;
            sobj =
                HSD_SObjLib_803A477C(gobj, (int) un_804D5990, 0, 0, 0x80, 0);
            sobj->x10 = un_804DDC4C;
            sobj->x14 = un_804DDC50;
            sobj->x40 |= 2;
            un_804D6E08 = gobj;
        }
    }

    return 0;
}

int un_80301A64(int arg0)
{
    s32 result;

    if (arg0 != 1) {
        return 0;
    }

    OSReport(un_803FD29C);
    result = lbSnap_8001D5FC(0, un_804D6E10);
    if (result == 0xB) {
        do {
            result = lb_8001B6F8();
        } while (result == 0xB);

        if (result == 0) {
            lbSnap_8001D40C(0);
        }
    }
    return 0;
}

int un_80301AD4(int arg0)
{
    s32 result;

    if (arg0 != 1) {
        return 0;
    }

    OSReport(un_803FD2AC);
    result = lbSnap_8001D7B0(0, un_804D6E14, un_804D6E10);
    if (result == 0xB) {
        do {
            result = lb_8001B6F8();
        } while (result == 0xB);

        if (result == 0) {
            lbSnap_8001D40C(0);
        }
    }
    return 0;
}

int un_80301B48(int arg0)
{
    switch (arg0) {
    case 0:
        lbAudioAx_80024030(0);
        un_80304334(un_80302DF0());
        break;
    case 6:
        lbAudioAx_80024030(1);
        gm_SetPendingScene(6);
        gm_801A4B60();
        break;
    }
    return 0;
}

void un_80301BA8(void* arg0)
{
    u8* out = arg0;
    *(s32*) (out + 0x0) = un_803FA258.x104;
    *(s32*) (out + 0x4) = un_803FA258.x108;
    out[0xA] = (u8) un_803FA258.x100;
    out[0xB] = (u8) un_803FA258.x10C;
    out[0xC] = (u8) un_803FA258.x11C;
    out[0x8] = 0;
    out[0x9] = 0x78;
    out[0xD] = (u8) un_803FA258.x110;
    out[0x19] = (u8) un_803FA258.xF8;
    out[0xE] = (u8) un_803FA258.x114;
    out[0x1A] = (u8) un_803FA258.xF8;
    out[0xF] = (u8) un_803FA258.x118;
    out[0x1B] = (u8) un_803FA258.xF8;
    out[0x10] = (u8) un_803FA258.x120;
    out[0x1C] = (u8) un_803FA258.xFC;
    out[0x11] = (u8) un_803FA258.x124;
    out[0x1D] = (u8) un_803FA258.xFC;
    out[0x12] = (u8) un_803FA258.x128;
    out[0x1E] = (u8) un_803FA258.xFC;
    out[0x13] = 0;
    out[0x14] = 0;
    out[0x15] = 0;
    out[0x16] = 0;
    out[0x17] = 0;
    out[0x18] = 0;
}

void un_80301C64(un_80301C64_t* arg0)
{
    arg0->x0 = un_803FA258.x138;
    arg0->x4 = 0;
}

int un_80301C80(int arg0)
{
    switch (arg0) {
    case 0:
        lbAudioAx_80024030(0);
        un_80304334(un_80302DF0());
        break;
    case 6:
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0xB);
        gm_801A4B60();
        break;
    }
    return 0;
}

void un_80301CE0(int arg0)
{
    switch (arg0) {
    case 6:
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0xB);
        gm_801A4B60();
        break;
    case 1:
        un_802FFCD0(4, un_803FA258.x144);
        break;
    }
}

bool un_80301D40(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_801A42F8(GM_FIXED_CAMERA_VS);
        gm_801A4B60();
    }

    return false;
}

int un_80301D7C(int arg0)
{
    if (arg0 == 1) {
        lbAudioAx_80024030(1);
        gm_801BEB74(un_803FA258.x188 - 1);
        gm_801A42F8(0x2B);
        gm_801A4B60();
    }
    return 0;
}

bool un_80301DCC(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0x4);
        gm_801A4B60();
    }

    return false;
}

bool un_80301E08(bool update_scene)
{
    if (update_scene == true) {
        lbAudioAx_80024030(1);
        gm_SetPendingScene(0xE);
        gm_801A4B60();
    }

    return false;
}
