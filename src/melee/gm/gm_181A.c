#include "gmregclear.h"

#include "gm_unsplit.h"
#include "platform.h"

#include "baselib/forward.h"

#include <dolphin/gx.h>
#include <sysdolphin/baselib/aobj.h>
#include <sysdolphin/baselib/controller.h>
#include <sysdolphin/baselib/dobj.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjgxlink.h>
#include <sysdolphin/baselib/gobjobject.h>
#include <sysdolphin/baselib/gobjproc.h>
#include <sysdolphin/baselib/jobj.h>
#include <sysdolphin/baselib/mobj.h>
#include <sysdolphin/baselib/random.h>
#include <sysdolphin/baselib/sislib.h>
#include <sysdolphin/baselib/tobj.h>
#include <sysdolphin/baselib/util.h>
#include <melee/cm/camera.h>
#include <melee/db/db.h>
#include <melee/ef/efasync.h>
#include <melee/ef/eflib.h>
#include <melee/ft/ft_0877.h>
#include <melee/ft/ftbosslib.h>
#include <melee/ft/ftlib.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_1A36.h>
#include <melee/gm/gm_1B03.h>
#include <melee/gm/gmadventure.h>
#include <melee/gm/gmmain_lib.h>
#include <melee/gm/gmregcommon.h>
#include <melee/gm/types.h>
#include <melee/gr/ground.h>
#include <melee/gr/grpushon.h>
#include <melee/gr/stage.h>
#include <melee/if/ifcoget.h>
#include <melee/if/ifnametag.h>
#include <melee/if/ifstatus.h>
#include <melee/if/ifstock.h>
#include <melee/if/iftime.h>
#include <melee/it/item.h>
#include <melee/lb/lb_00B0.h>
#include <melee/lb/lb_00F9.h>
#include <melee/lb/lbarchive.h>
#include <melee/lb/lbaudio_ax.h>
#include <melee/lb/lbbgflash.h>
#include <melee/lb/lbcardgame.h>
#include <melee/lb/lbcardnew.h>
#include <melee/lb/lblanguage.h>
#include <melee/lb/lbtime.h>
#include <melee/mp/mpcoll.h>
#include <melee/pl/player.h>
#include <melee/sc/types.h>
#include <melee/ty/toy.h>

typedef struct RegClearSpawnEntry {
    /* 0x00 */ s32 x0;
    /* 0x04 */ u8 x4;
    /* 0x05 */ u8 x5;
    /* 0x06 */ u8 x6;
    /* 0x07 */ u8 x7;
    /* 0x08 */ f32 x8;
    /* 0x0C */ f32 xC;
} RegClearSpawnEntry;

static struct lbl_80472ED8_t {
    /* 0x000 */ s32 x0;
    /* 0x004 */ s32 x4;
    /* 0x008 */ s32 x8;
    /* 0x00C */ PlayerInitData xC;
    /* 0x030 */ u8 pad_30[0x24];
    /* 0x054 */ RegClearSpawnEntry x54[101];
    /* 0x6A4 */ RegClearSpawnEntry* x6A4;
    /* 0x6A8 */ RegClearSpawnEntry* x6A8;
    /* 0x6AC */ RegClearSpawnEntry* x6AC;
    /* 0x6B0 */ RegClearSpawnEntry* x6B0;
    /* 0x6B4 */ RegClearSpawnEntry* x6B4;
    /* 0x6B8 */ RegClearSpawnEntry* x6B8;
} lbl_80472ED8;

static struct lbl_80473594_t {
    /* 0x00 */ u8 x0;
    /* 0x02 */ u16 x2;
    /* 0x04 */ int x4;
    /* 0x08 */ int x8;
    /* 0x0C */ int xC;
    /* 0x10 */ s8 x10;
    /* 0x11 */ u8 x11;
} lbl_80473594;

int gm_80181A14(void)
{
    return lbl_80473594.x0;
}

u32 gm_80181A24(void)
{
    return lbl_80473594.x2;
}

s32 gm_80181A34(void)
{
    return lbl_80473594.x4;
}

/* data-recon w18: defined in-TU from target bytes (was undefined extern).
 * 6 record blocks of 0xC0 bytes each: 26 words of 0x0FFFFFFF score sentinel
 * + 22 zero words. Defining the previously-extern GLOBAL keeps named relocs
 * (idiom 98); numeric brace-init consumes 0 at-ids. */
u8 lbl_803D8D08[0x480] = {
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
    0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/* data-recon w18: dead .data string objects at target offsets 0x480..0x53A.
 * Byte-proven: code reaches them ONLY via base+offset off lbl_803D8D08
 * (target gm_80182174 uses addi rX,rBase,0x480.. and the string objects have
 * ZERO relocs) - so they emit as unreferenced initialized statics (idiom 51).
 * Decl order = .data emission order (idiom 69). */
static char lbl_803D9188[] = "GmKumite.dat";
static char lbl_803D9198[] = "gmKumiteSystemTable10man";
static char lbl_803D91B4[] = "gmKumiteSystemTable100man";
static char lbl_803D91D0[] = "gmKumiteSystemTable10min";
static char lbl_803D91EC[] = "gmKumiteSystemTable60min";
static char lbl_803D9208[] = "gmKumiteSystemTableEndless";
static char lbl_803D9224[] = "gmKumiteSystemTableMercilessly";

void gm_80181A44(int c_kind, int arg1, bool arg2)
{
    u8* base;

    base = lbl_803D8D08;

    switch (arg1) {
    case 0x21:
        (base + 0x6C)[c_kind] = arg2;
        break;
    case 0x22:
        (base + 0x12C)[c_kind] = arg2;
        break;
    case 0x23:
        (base + 0x1EC)[c_kind] = arg2;
        break;
    case 0x24:
        (base + 0x2AC)[c_kind] = arg2;
        break;
    case 0x25:
        (base + 0x36C)[c_kind] = arg2;
        break;
    case 0x26:
        (base + 0x42C)[c_kind] = arg2;
        break;
    }
}

void gm_80181AC8(int c_kind, int arg1, u16 arg2)
{
    u8* base;

    base = lbl_803D8D08;

    switch (arg1) {
    case 0x21:
        ((s16*) (base + 0x88))[c_kind] = arg2;
        break;
    case 0x22:
        ((s16*) (base + 0x148))[c_kind] = arg2;
        break;
    case 0x23:
        ((s16*) (base + 0x208))[c_kind] = arg2;
        break;
    case 0x24:
        ((s16*) (base + 0x2C8))[c_kind] = arg2;
        break;
    case 0x25:
        ((s16*) (base + 0x388))[c_kind] = arg2;
        break;
    case 0x26:
        ((s16*) (base + 0x448))[c_kind] = arg2;
        break;
    }
}

void gm_80181B64(int c_kind, int arg1, s32 arg2)
{
    u8* base;

    base = lbl_803D8D08;

    switch (arg1) {
    case 0x21:
        ((s32*) base)[c_kind] = arg2;
        break;
    case 0x22:
        ((s32*) (base + 0xC0))[c_kind] = arg2;
        break;
    case 0x23:
        ((s32*) (base + 0x180))[c_kind] = arg2;
        break;
    case 0x24:
        ((s32*) (base + 0x240))[c_kind] = arg2;
        break;
    case 0x25:
        ((s32*) (base + 0x300))[c_kind] = arg2;
        break;
    case 0x26:
        ((s32*) (base + 0x3C0))[c_kind] = arg2;
        break;
    }
}

#pragma dont_inline on
int fn_80181BFC(int* arg0)
{
    int i;
    int count = 0;

    for (i = 1; i < 6; i++) {
        if (Player_GetFalls(i) == 0 &&
            Player_GetPlayerSlotType(i) != Gm_PKind_NA)
        {
            count += 1;
        } else if (arg0 != NULL) {
            *arg0 = i;
        }
    }
    return count;
}
#pragma dont_inline reset

s32 fn_80181C80(s32 arg0)
{
    struct lbl_80472ED8_t* state = &lbl_80472ED8;
    s32 var_r29;
    s32 var_r30;
    volatile s32 sp38;
    UNUSED u8 _[4];
    PlayerInitData sp10;

    PAD_STACK(4);
    gm_801A4310();
    var_r30 = 0;
    sp10 = state->xC;

    for (var_r29 = 1; var_r29 < 6; var_r29++) {
        if (Player_GetFalls(var_r29) == 0 &&
            Player_GetPlayerSlotType(var_r29) != Gm_PKind_NA)
        {
            var_r30++;
        } else {
            sp38 = var_r29;
        }
    }

    if ((s32) state->x54[arg0].x4 > var_r30 && state->x8 > 0x5A) {
        if (Player_GetPlayerSlotType(sp38) != Gm_PKind_NA) {
            Player_SetFalls(sp38, 0);
            Player_SetSuicideCount(sp38, 0);
            fn_8016EF98(sp38);
        }
        state->x54[arg0].x0 = -2;
        sp10.team = !Player_GetTeam(0);
        sp10.c_kind = state->x54[arg0].x5;
        sp10.cpu_level = state->x54[arg0].x6;
        sp10.xE = state->x54[arg0].x7;
        sp10.x18 = state->x54[arg0].x8;
        sp10.x1C = state->x54[arg0].xC;
        gm_8016EDDC(sp38, &sp10);
        Player_SetNametagSlotID(sp38, 0x78);
        un_802FD28C(sp38);
    }
    return state->x0++;
}

void fn_80181E18(void)
{
    s32 mode;
    s32 var_r29;
    s32 next;
    s32 temp;
    s32 count;
    s32 i;

    mode = gm_801A4310();

    if (lbl_80472ED8.x8 <= 0x5A) {
        lbl_80472ED8.x8 += 1;
    }

    if (mode < 0x25) {
        if (mode < 0x23) {
            if (mode < 0x21) {
            } else {
                Player_GetFalls(0);
            }
        } else if (gm_8016AEEC() == 0 && gm_8016AEFC() == 0x3B) {
            lbl_80473594.x0 = 1;
            gm_8016B33C(7);
            gm_8016B328();
        }
    } else if (mode < 0x27) {
        if (Player_GetFalls(0) != 0) {
            gm_8016B33C(5);
            gm_8016B328();
        }
    }

    for (var_r29 = 0; var_r29 < 101; var_r29++) {
        if (lbl_80472ED8.x54[var_r29].x0 == -2) {
            continue;
        }

        if (lbl_80473594.x8 < 0x23) {
            if (lbl_80473594.x8 < 0x21) {
            } else {
                lbl_80473594.x4 = gm_8016AEDC();
            }
        }

        temp = var_r29 - fn_80181BFC(NULL);
        if (temp < 0) {
            temp = 0;
        }
        lbl_80473594.x2 = (s16) (temp + lbl_80472ED8.x4);

        switch (mode) {
        case 0x21:
            temp = var_r29 - fn_80181BFC(NULL);
            if (temp < 0) {
                temp = 0;
            }
            ifStock_802FA2D0(0xA - (temp + lbl_80472ED8.x4));
            break;
        case 0x22:
            temp = var_r29 - fn_80181BFC(NULL);
            if (temp < 0) {
                temp = 0;
            }
            ifStock_802FA2D0(0x64 - (temp + lbl_80472ED8.x4));
            break;
        default:
            temp = var_r29 - fn_80181BFC(NULL);
            if (temp < 0) {
                temp = 0;
            }
            ifStock_802FA2D0(temp + lbl_80472ED8.x4);
            break;
        }

        next = lbl_80472ED8.x54[var_r29].x0;

        if (next == -1) {
            fn_80181C80(var_r29);
            return;
        }

        if (next == 0x3E7) {
            count = 0;
            for (i = 1; i < 6; i++) {
                if (Player_GetFalls(i) == 0 &&
                    Player_GetPlayerSlotType(i) != Gm_PKind_NA)
                {
                    count += 1;
                }
            }
            if (count == 0) {
                lbl_80473594.x0 = 1;
                gm_8016B33C(7);
                gm_8016B328();
            }
        } else {
            if (next < var_r29) {
                s32 k;
                for (k = next; k < var_r29; k++) {
                    lbl_80472ED8.x54[k].x0 = -1;
                    lbl_80472ED8.x4 += 1;
                }
            }
            fn_80181C80(lbl_80472ED8.x54[var_r29].x0);
            temp = var_r29 - fn_80181BFC(NULL);
            if (temp < 0) {
                temp = 0;
            }
            ifStock_802FA2D0(temp + lbl_80472ED8.x4);
        }
        break;
    }
    PAD_STACK(16);
}

void gm_80182174(void)
{
    u8* data = lbl_803D8D08;
    s32 mode;
    s32 i;
    RegClearSpawnEntry* src;
    RegClearSpawnEntry* dst;

    mode = gm_801A4310();

    lbArchive_80016DBC((const char*) &data[0x480], &lbl_80472ED8.x6A4,
                       (const char*) &data[0x490], &lbl_80472ED8.x6A8,
                       (const char*) &data[0x4AC], &lbl_80472ED8.x6AC,
                       (const char*) &data[0x4C8], &lbl_80472ED8.x6B0,
                       (const char*) &data[0x4E4], &lbl_80472ED8.x6B4,
                       (const char*) &data[0x500], &lbl_80472ED8.x6B8,
                       (const char*) &data[0x51C], 0);

    lbl_80472ED8.x0 = 0;
    lbl_80472ED8.x4 = 0;
    lbl_80472ED8.x8 = 0;

    gm_8016795C(&lbl_80472ED8.xC);

    lbl_80472ED8.xC.c_kind = 0x1B;
    lbl_80472ED8.xC.slot_type = 1;
    lbl_80472ED8.xC.stocks = 1;
    lbl_80472ED8.xC.xD_b4 = 1;

    switch (mode) {
    case 0x21:
        src = lbl_80472ED8.x6A4;
        dst = lbl_80472ED8.x54;
        for (i = 0; i < 101; i++) {
            dst->x0 = src->x0;
            dst->x4 = src->x4;
            dst->x5 = src->x5;
            dst->x8 = src->x8;
            dst->xC = src->xC;
            dst->x6 = src->x6;
            dst->x7 = src->x7;
            if (src->x0 == 0x3E7) {
                break;
            }
            src++;
            dst++;
        }
        break;
    case 0x22:
        src = lbl_80472ED8.x6A8;
        dst = lbl_80472ED8.x54;
        for (i = 0; i < 101; i++) {
            dst->x0 = src->x0;
            dst->x4 = src->x4;
            dst->x5 = src->x5;
            dst->x8 = src->x8;
            dst->xC = src->xC;
            dst->x6 = src->x6;
            dst->x7 = src->x7;
            if (src->x0 == 0x3E7) {
                break;
            }
            src++;
            dst++;
        }
        break;
    case 0x23:
        src = lbl_80472ED8.x6AC;
        dst = lbl_80472ED8.x54;
        for (i = 0; i < 101; i++) {
            dst->x0 = src->x0;
            dst->x4 = src->x4;
            dst->x5 = src->x5;
            dst->x8 = src->x8;
            dst->xC = src->xC;
            dst->x6 = src->x6;
            dst->x7 = src->x7;
            if (src->x0 == 0x3E7) {
                break;
            }
            src++;
            dst++;
        }
        break;
    case 0x24:
        src = lbl_80472ED8.x6B0;
        dst = lbl_80472ED8.x54;
        for (i = 0; i < 101; i++) {
            dst->x0 = src->x0;
            dst->x4 = src->x4;
            dst->x5 = src->x5;
            dst->x8 = src->x8;
            dst->xC = src->xC;
            dst->x6 = src->x6;
            dst->x7 = src->x7;
            if (src->x0 == 0x3E7) {
                break;
            }
            src++;
            dst++;
        }
        break;
    case 0x25:
        src = lbl_80472ED8.x6B4;
        dst = lbl_80472ED8.x54;
        for (i = 0; i < 101; i++) {
            dst->x0 = src->x0;
            dst->x4 = src->x4;
            dst->x5 = src->x5;
            dst->x8 = src->x8;
            dst->xC = src->xC;
            dst->x6 = src->x6;
            dst->x7 = src->x7;
            if (src->x0 == 0x3E7) {
                break;
            }
            src++;
            dst++;
        }
        break;
    case 0x26:
        src = lbl_80472ED8.x6B8;
        dst = lbl_80472ED8.x54;
        for (i = 0; i < 101; i++) {
            dst->x0 = src->x0;
            dst->x4 = src->x4;
            dst->x5 = src->x5;
            dst->x8 = src->x8;
            dst->xC = src->xC;
            dst->x6 = src->x6;
            dst->x7 = src->x7;
            if (src->x0 == 0x3E7) {
                break;
            }
            src++;
            dst++;
        }
        break;
    }

    lbl_80473594.x10 = Player_GetPlayerId(0);
    lbl_80473594.x11 = Player_GetNametagSlotID(0);
    HSD_GObj_SetupProc(GObj_Create(0xFU, 0x11U, 0U),
                       (HSD_GObjEvent) fn_80181E18, 0x15U);
    gm_80168F88();
    PAD_STACK(8);
}

bool gm_80182510(void)
{
    switch (gm_801A4310()) {
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
        return true;
    }
    return false;
}

void gm_80182554(int arg0, int arg1)
{
    u8* base = (u8*) &lbl_80472ED8;
    *(int*) (base + 0x6C8) = arg0;
    *(int*) (base + 0x6C4) = arg1;
    *(u8*) (base + 0x6BC) = 0;
    *(int*) (base + 0x6C0) = 0;
    *(u16*) (base + 0x6BE) = 0;
}

typedef struct {
    /* 0x00 */ u32 scores[27];
    /* 0x6C */ u8 icons[28];
    /* 0x88 */ u16 times[28];
} RecordBlock; /* 0xC0 */

s32 gm_80182578(void)
{
    u8* base = (u8*) &lbl_80472ED8;
    int* pmode = (int*) (base + 0x6C4);
    int* pidx = (int*) (base + 0x6C8);
    RecordBlock* blocks = (RecordBlock*) lbl_803D8D08;
    int idx = *(int*) (base + 0x6C8);
    s32 mode = *(s32*) (base + 0x6C4);
    int time_val;
    u32 score_val;

    switch (mode) {
    case 33:
        time_val = blocks[0].times[idx];
        break;
    case 34:
        time_val = blocks[1].times[idx];
        break;
    case 35:
        time_val = blocks[2].times[idx];
        break;
    case 36:
        time_val = blocks[3].times[idx];
        break;
    case 37:
        time_val = blocks[4].times[idx];
        break;
    case 38:
        time_val = blocks[5].times[idx];
        break;
    default:
        time_val = 0;
        break;
    }

    switch (mode) {
    case 33:
        score_val = blocks[0].scores[idx];
        break;
    case 34:
        score_val = blocks[1].scores[idx];
        break;
    case 35:
        score_val = blocks[2].scores[idx];
        break;
    case 36:
        score_val = blocks[3].scores[idx];
        break;
    case 37:
        score_val = blocks[4].scores[idx];
        break;
    case 38:
        score_val = blocks[5].scores[idx];
        break;
    default:
        score_val = 0;
        break;
    }

    if (mode < 0x25) {
        if (mode < 0x23) {
            if (mode < 0x21) {
                return mode;
            }
            if (mode == 0x21) {
                mode = gmMainLib_8015D6BC(gm_80164024((u8) idx));
            } else {
                mode = gmMainLib_8015D710(gm_80164024((u8) idx));
            }
            if ((u8) *(u8*) (base + 0x6BC) != 0) {
                u32 score_store = (u32) *(s32*) (base + 0x6C0);
                if (score_store < score_val) {
                    int m = *pmode;
                    int i = *pidx;
                    switch (m) {
                    case 33:
                        blocks[0].icons[i] = (u8) *(u8*) (base + 0x6BC);
                        break;
                    case 34:
                        blocks[1].icons[i] = (u8) *(u8*) (base + 0x6BC);
                        break;
                    case 35:
                        blocks[2].icons[i] = (u8) *(u8*) (base + 0x6BC);
                        break;
                    case 36:
                        blocks[3].icons[i] = (u8) *(u8*) (base + 0x6BC);
                        break;
                    case 37:
                        blocks[4].icons[i] = (u8) *(u8*) (base + 0x6BC);
                        break;
                    case 38:
                        blocks[5].icons[i] = (u8) *(u8*) (base + 0x6BC);
                        break;
                    }
                    switch (m) {
                    case 33:
                        blocks[0].scores[i] = score_store;
                        break;
                    case 34:
                        blocks[1].scores[i] = score_store;
                        break;
                    case 35:
                        blocks[2].scores[i] = score_store;
                        break;
                    case 36:
                        blocks[3].scores[i] = score_store;
                        break;
                    case 37:
                        blocks[4].scores[i] = score_store;
                        break;
                    case 38:
                        blocks[5].scores[i] = score_store;
                        break;
                    }
                    {
                        u16 time_store = *(u16*) (base + 0x6BE);
                        switch (m) {
                        case 33:
                            blocks[0].times[i] = time_store;
                            break;
                        case 34:
                            blocks[1].times[i] = time_store;
                            break;
                        case 35:
                            blocks[2].times[i] = time_store;
                            break;
                        case 36:
                            blocks[3].times[i] = time_store;
                            break;
                        case 37:
                            blocks[4].times[i] = time_store;
                            break;
                        case 38:
                            blocks[5].times[i] = time_store;
                            break;
                        }
                    }
                }
                return mode;
            }
            if ((s32) *(u16*) (base + 0x6BE) > (s32) time_val && mode == 0) {
                int m = *pmode;
                int i = *pidx;
                switch (m) {
                case 33:
                    blocks[0].times[i] = (u16) *(u16*) (base + 0x6BE);
                    break;
                case 34:
                    blocks[1].times[i] = (u16) *(u16*) (base + 0x6BE);
                    break;
                case 35:
                    blocks[2].times[i] = (u16) *(u16*) (base + 0x6BE);
                    break;
                case 36:
                    blocks[3].times[i] = (u16) *(u16*) (base + 0x6BE);
                    break;
                case 37:
                    blocks[4].times[i] = (u16) *(u16*) (base + 0x6BE);
                    break;
                case 38:
                    blocks[5].times[i] = (u16) *(u16*) (base + 0x6BE);
                    break;
                }
            }
            return mode;
        }
        if ((u8) *(u8*) (base + 0x6BC) != 0) {
            u16 time_store = *(u16*) (base + 0x6BE);
            if ((s32) time_store > (s32) time_val) {
                switch (mode) {
                case 33:
                    blocks[0].icons[idx] = (u8) *(u8*) (base + 0x6BC);
                    break;
                case 34:
                    blocks[1].icons[idx] = (u8) *(u8*) (base + 0x6BC);
                    break;
                case 35:
                    blocks[2].icons[idx] = (u8) *(u8*) (base + 0x6BC);
                    break;
                case 36:
                    blocks[3].icons[idx] = (u8) *(u8*) (base + 0x6BC);
                    break;
                case 37:
                    blocks[4].icons[idx] = (u8) *(u8*) (base + 0x6BC);
                    break;
                case 38:
                    blocks[5].icons[idx] = (u8) *(u8*) (base + 0x6BC);
                    break;
                }
                switch (mode) {
                case 33:
                    blocks[0].times[idx] = time_store;
                    break;
                case 34:
                    blocks[1].times[idx] = time_store;
                    break;
                case 35:
                    blocks[2].times[idx] = time_store;
                    break;
                case 36:
                    blocks[3].times[idx] = time_store;
                    break;
                case 37:
                    blocks[4].times[idx] = time_store;
                    break;
                case 38:
                    blocks[5].times[idx] = time_store;
                    break;
                }
            }
        }
        return mode;
    } else if (mode < 0x27) {
        if ((s32) *(u16*) (base + 0x6BE) > (s32) time_val) {
            switch (mode) {
            case 33:
                blocks[0].times[idx] = (u16) *(u16*) (base + 0x6BE);
                break;
            case 34:
                blocks[1].times[idx] = (u16) *(u16*) (base + 0x6BE);
                break;
            case 35:
                blocks[2].times[idx] = (u16) *(u16*) (base + 0x6BE);
                break;
            case 36:
                blocks[3].times[idx] = (u16) *(u16*) (base + 0x6BE);
                break;
            case 37:
                blocks[4].times[idx] = (u16) *(u16*) (base + 0x6BE);
                break;
            case 38:
                blocks[5].times[idx] = (u16) *(u16*) (base + 0x6BE);
                break;
            }
        }
        return mode;
    }
    return mode;
    PAD_STACK(0x48);
}

void fn_80182B5C(void)
{
    typedef struct {
        /* 0x000 */ char pad_0[0x6BC];
        /* 0x6BC */ u8 x0;
        /* 0x6BD */ char pad_6BD;
        /* 0x6BE */ u16 x2;
        /* 0x6C0 */ int x4;
        /* 0x6C4 */ int x8;
        /* 0x6C8 */ int xC;
        /* 0x6CC */ s8 x10;
        /* 0x6CD */ u8 x11;
    } ext_t;
    RecordBlock* blocks = (RecordBlock*) lbl_803D8D08;
    ext_t* st = (ext_t*) &lbl_80472ED8;
    int mode = st->x8;
    int idx = st->xC;
    int var_r6;
    u32 var_r30;

    switch (mode) {
    case 33:
        var_r6 = blocks[0].times[idx];
        break;
    case 34:
        var_r6 = blocks[1].times[idx];
        break;
    case 35:
        var_r6 = blocks[2].times[idx];
        break;
    case 36:
        var_r6 = blocks[3].times[idx];
        break;
    case 37:
        var_r6 = blocks[4].times[idx];
        break;
    case 38:
        var_r6 = blocks[5].times[idx];
        break;
    default:
        var_r6 = 0;
        break;
    }

    switch (mode) {
    case 33:
        var_r30 = blocks[0].scores[idx];
        break;
    case 34:
        var_r30 = blocks[1].scores[idx];
        break;
    case 35:
        var_r30 = blocks[2].scores[idx];
        break;
    case 36:
        var_r30 = blocks[3].scores[idx];
        break;
    case 37:
        var_r30 = blocks[4].scores[idx];
        break;
    case 38:
        var_r30 = blocks[5].scores[idx];
        break;
    default:
        var_r30 = 0;
        break;
    }

    switch (mode) {
    case 33:
        gmMainLib_8015D6BC(gm_80164024((u8) idx));
        goto func;
    case 34:
        gmMainLib_8015D710(gm_80164024((u8) idx));
    func:
        if (st->x0 != 0) {
            if ((u32) st->x4 < var_r30) {
                gm_8016B350(0x9C40);
                gm_8016B364(0x144);
                gm_80167858((s32) st->x10, (s32) st->x11,
                            0xD, 0x5A);
                return;
            }
        } else {
            gm_8016B364(0x148);
            gm_8016B378(0x28);
            return;
        }
        break;
    case 35:
    case 36:
        if (st->x0 != 0 && (s32) st->x2 > var_r6) {
            gm_8016B350(0x9C40);
            gm_8016B364(0x144);
            gm_80167858((s32) st->x10, (s32) st->x11, 0xD,
                        0x5A);
        }
        break;
    case 37:
    case 38:
        if ((s32) st->x2 > var_r6) {
            gm_8016B350(0x9C40);
            gm_8016B364(0x144);
            gm_80167858((s32) st->x10, (s32) st->x11, 0xD,
                        0x5A);
        }
        break;
    }
    PAD_STACK(24);
}

static UnkMultimanData lbl_804D65E0;

UnkMultimanData* gm_80182DF0(int c_kind, int arg1)
{
    UnkMultimanData* result = &lbl_804D65E0;
    u8* base = lbl_803D8D08;

    switch (arg1) {
    case 33:
        result->x0_0 = (base + 0x6C)[c_kind];
        result->x2 = ((u16*) (base + 0x88))[c_kind];
        result->x4 = ((s32*) base)[c_kind];
        break;
    case 34:
        result->x0_0 = (base + 0x12C)[c_kind];
        result->x2 = ((u16*) (base + 0x148))[c_kind];
        result->x4 = ((s32*) (base + 0xC0))[c_kind];
        break;
    case 35:
        result->x0_0 = (base + 0x1EC)[c_kind];
        result->x2 = ((u16*) (base + 0x208))[c_kind];
        break;
    case 36:
        result->x0_0 = (base + 0x2AC)[c_kind];
        result->x2 = ((u16*) (base + 0x2C8))[c_kind];
        break;
    case 37:
        result->x0_0 = (base + 0x36C)[c_kind];
        result->x2 = ((u16*) (base + 0x388))[c_kind];
        break;
    case 38:
        result->x0_0 = (base + 0x42C)[c_kind];
        result->x2 = ((u16*) (base + 0x448))[c_kind];
        break;
    }
    return result;
}
