#include "gm_18A5.h"

#include "ft/forward.h"

#include <sysdolphin/baselib/controller.h>
#include <sysdolphin/baselib/dobj.h>
#include <sysdolphin/baselib/fog.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjgxlink.h>
#include <sysdolphin/baselib/gobjobject.h>
#include <sysdolphin/baselib/gobjplink.h>
#include <sysdolphin/baselib/gobjproc.h>
#include <sysdolphin/baselib/jobj.h>
#include <sysdolphin/baselib/mobj.h>
#include <sysdolphin/baselib/particle.h>
#include <sysdolphin/baselib/random.h>
#include "dolphin/gx/GXStruct.h"
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_unsplit.h>
#include <melee/gm/gmmain_lib.h>
#include <melee/gm/gmtou.h>
#include <melee/gm/types.h>
#include <melee/lb/lb_00F9.h>
#include <melee/lb/lbarchive.h>
#include <melee/lb/lbaudio_ax.h>
#include <melee/lb/lbdvd.h>
#include <melee/lb/lblanguage.h>
#include <melee/mn/mnmain.h>
#include <melee/mn/mnmainrule.h>
#include <melee/mn/mnname.h>
#include <melee/mn/mnnamenew.h>
#include <melee/mn/mnstagesel.h>
#include <melee/pl/player.h>
#include <melee/sc/types.h>

/// @file gm_1965.c -- original translation-unit boundary restored from
/// gm_18A5.c (TU split; .text 0x80196510..0x8019B8C4).

// Keep in sync with the BracketEntry definition in gm_18A5.static.h.
// (This TU reads the bracket table defined by gm_18A5.c.)
typedef struct BracketEntry {
    /* 0x00 */ u8 x0;
    /* 0x01 */ u8 x1;
    /* 0x02 */ u8 x2;
    /* 0x03 */ u8 x3;
    /* 0x04 */ u8 x4;
    /* 0x05 */ u8 x5;
    /* 0x06 */ u8 x6;
    /* 0x07 */ u8 pad7[0x0C - 0x07];
    /* 0x0C */ s32 xC;
    /* 0x10 */ s32 x10;
    /* 0x14 */ s32 x14;
    /* 0x18 */ s32 x18;
    /* 0x1C */ f32 x1C;
    /* 0x20 */ GXColor x20;
    /* 0x24 */ u8 x24;
    /* 0x25 */ u8 x25;
    /* 0x26 */ u8 x26;
    /* 0x27 */ u8 x27;
    /* 0x28 */ u8 x28;
    /* 0x29 */ u8 pad29[0x2C - 0x29];
    /* 0x2C */ HSD_GObj* x2C;
    /* 0x30 */ u8 x30;
    /* 0x31 */ u8 pad31;
    /* 0x32 */ u8 x32;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ s32 x34;
    /* 0x38 */ s32 x38;
    /* 0x3C */ s32 x3C;
    /* 0x40 */ s32 x40;
    /* 0x44 */ s32 x44;
    /* 0x48 */ s32 x48;
    /* 0x4C */ u8 x4C;
    /* 0x4D */ u8 x4D;
    /* 0x4E */ u8 x4E;
    /* 0x4F */ u8 x4F;
    /* 0x50 */ u8 x50;
    /* 0x51 */ u8 x51;
    /* 0x52 */ u8 x52;
    /* 0x53 */ u8 pad53;
    /* 0x54 */ u16 x54;
    /* 0x56 */ u8 pad56[0x58 - 0x56];
    /* 0x58 */ HSD_GObj* x58;
    /* 0x5C */ u8 x5C;
    /* 0x5D */ u8 pad5D;
    /* 0x5E */ u8 x5E;
    /* 0x5F */ u8 pad5F;
    /* 0x60 */ s32 x60;
    /* 0x64 */ s32 x64;
    /* 0x68 */ s32 x68;
    /* 0x6C */ s32 x6C;
    /* 0x70 */ s32 x70;
    /* 0x74 */ s32 x74;
    /* 0x78 */ u8 x78;
    /* 0x79 */ u8 pad79;
    /* 0x7A */ u8 x7A;
    /* 0x7B */ u8 pad7B[0x7D - 0x7B];
    /* 0x7D */ u8 x7D;
    /* 0x7E */ u8 x7E;
    /* 0x7F */ u8 pad7F;
    /* 0x80 */ u16 x80;
    /* 0x82 */ u8 pad82[0x84 - 0x82];
    /* 0x84 */ HSD_GObj* x84;
    /* 0x88 */ u8 x88;
    /* 0x89 */ u8 pad89;
    /* 0x8A */ u8 x8A;
    /* 0x8B */ u8 pad8B;
    /* 0x8C */ s32 x8C;
    /* 0x90 */ s32 x90;
    /* 0x94 */ s32 x94;
    /* 0x98 */ s32 x98;
    /* 0x9C */ s32 x9C;
    /* 0xA0 */ s32 xA0;
    /* 0xA4 */ u8 xA4;
    /* 0xA5 */ u8 padA5;
    /* 0xA6 */ u8 xA6;
    /* 0xA7 */ u8 padA7[0xA9 - 0xA7];
    /* 0xA9 */ u8 xA9;
    /* 0xAA */ u8 xAA;
    /* 0xAB */ u8 padAB;
    /* 0xAC */ u16 xAC;
    /* 0xAE */ u8 padAE[0xB0 - 0xAE];
    /* 0xB0 */ HSD_GObj* xB0;
    /* 0xB4 */ u8 xB4;
    /* 0xB5 */ u8 padB5;
    /* 0xB6 */ u8 xB6;
    /* 0xB7 */ u8 padB7;
    /* 0xB8 */ s32 xB8;
    /* 0xBC */ s32 xBC;
    /* 0xC0 */ s32 xC0;
    /* 0xC4 */ s32 xC4;
    /* 0xC8 */ s32 xC8;
    /* 0xCC */ s32 xCC;
    /* 0xD0 */ u8 xD0;
    /* 0xD1 */ u8 padD1;
    /* 0xD2 */ u8 xD2;
    /* 0xD3 */ u8 padD3[0xD5 - 0xD3];
    /* 0xD5 */ u8 xD5;
    /* 0xD6 */ u8 xD6;
    /* 0xD7 */ u8 padD7;
    /* 0xD8 */ u16 xD8;
    /* 0xDA */ u8 padDA[0xDC - 0xDA];
} BracketEntry;

extern BracketEntry lbl_80473AB8[0x40];

struct lbl_803DA0D0_t {
    /* 0x00 */ u8 icon_model_map[0x18];
    /* 0x18 */ u8 pad_0x18[0x1E - 0x18];
    /* 0x1E */ u8 rank_thresholds[32][6];
    /* 0xDE */ u8 pad_0xDE[0xE0 - 0xDE];
    /* 0xE0 */ f32 bounce_y[41];
}; /* size = 0x184 */
STATIC_ASSERT(sizeof(struct lbl_803DA0D0_t) == 0x184);

/// .data reconstruction (idiom 69/98): table content recovered from the
/// target binary (lbl_803DA0D0, .data+0x0, 0x184 bytes, no relocs).
struct lbl_803DA0D0_t lbl_803DA0D0 = {
    /* icon_model_map */
    {
        0x00, 0x01, 0x02, 0x03, 0x05, 0x06, 0x0C, 0x06, 0x06, 0x07, 0x09, 0x08,
        0x06, 0x09, 0x04, 0x09, 0x0A, 0x0B, 0x0C, 0x02, 0x0C, 0x06, 0x07, 0x09
    },
    /* pad_0x18 */
    { 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00 },
    /* rank_thresholds */
    {
        { 0x02, 0x00, 0x00, 0x00, 0x00, 0x03 },
        { 0x03, 0x00, 0x00, 0x00, 0x05, 0x06 },
        { 0x04, 0x00, 0x00, 0x00, 0x06, 0x07 },
        { 0x06, 0x0A, 0x00, 0x00, 0x0C, 0x0D },
        { 0x08, 0x0C, 0x00, 0x00, 0x0E, 0x0F },
        { 0x0C, 0x12, 0x15, 0x00, 0x17, 0x18 },
        { 0x10, 0x18, 0x1C, 0x00, 0x1E, 0x1F },
        { 0x18, 0x24, 0x2A, 0x2E, 0x30, 0x31 },
        { 0x20, 0x30, 0x38, 0x3C, 0x3E, 0x3F },
        { 0x02, 0x00, 0x00, 0x00, 0x00, 0x03 },
        { 0x03, 0x00, 0x00, 0x00, 0x00, 0x04 },
        { 0x04, 0x00, 0x00, 0x00, 0x06, 0x07 },
        { 0x06, 0x00, 0x00, 0x00, 0x08, 0x09 },
        { 0x09, 0x00, 0x00, 0x00, 0x0C, 0x0D },
        { 0x02, 0x00, 0x00, 0x00, 0x00, 0x03 },
        { 0x03, 0x00, 0x00, 0x00, 0x00, 0x04 },
        { 0x04, 0x00, 0x00, 0x00, 0x00, 0x05 },
        { 0x04, 0x00, 0x00, 0x00, 0x06, 0x07 },
        { 0x05, 0x00, 0x00, 0x00, 0x07, 0x08 },
        { 0x06, 0x00, 0x00, 0x00, 0x08, 0x09 },
        { 0x07, 0x00, 0x00, 0x00, 0x09, 0x0A },
        { 0x08, 0x00, 0x00, 0x00, 0x0A, 0x0B },
        { 0x08, 0x0C, 0x00, 0x00, 0x0E, 0x0F },
        { 0x09, 0x00, 0x00, 0x00, 0x0C, 0x0D },
        { 0x0A, 0x00, 0x00, 0x00, 0x0E, 0x0F },
        { 0x0B, 0x00, 0x00, 0x00, 0x0F, 0x10 },
        { 0x0C, 0x00, 0x00, 0x00, 0x10, 0x11 },
        { 0x0D, 0x00, 0x00, 0x00, 0x11, 0x12 },
        { 0x0E, 0x00, 0x00, 0x00, 0x12, 0x13 },
        { 0x0F, 0x00, 0x00, 0x00, 0x13, 0x14 },
        { 0x10, 0x00, 0x00, 0x00, 0x14, 0x15 },
        { 0x10, 0x14, 0x00, 0x00, 0x16, 0x17 },
    },
    /* pad_0xDE */
    { 0x00, 0x00 },
    /* bounce_y */
    { 0.0F },
};

extern HSD_GObj* lbl_804D663C;

static s32 lbl_804D4190 = 0xFFFFFFFF;
static s32 lbl_804D4194 = -1;

void fn_80196510(void)
{
    int stage;

    gm_8018F634();

    while (1) {
        stage = fn_8018F4A0();
        lbl_804D4190 = stage;
        if (lbl_804D4194 != lbl_804D4190) {
            break;
        }
        if (fn_801642A0() != 0) {
            break;
        }
    }

    lbl_804D4194 = lbl_804D4190;
}

s32 fn_8019655C(void)
{
    return lbl_804D4190;
}

bool fn_80196564(TmData* arg0)
{
    if ((arg0->stage_selection_type == 0 && arg0->x32 == 0) ||
        arg0->stage_selection_type == 1)
    {
        return true;
    }
    return false;
}

bool fn_80196594(TmData* arg0)
{
    if ((arg0->stage_selection_type == 2 && arg0->x32 == 0) ||
        arg0->stage_selection_type == 3)
    {
        return true;
    }
    return false;
}

void fn_801965C4(void)
{
    TmData* temp_r3;
    int temp_r4;

    temp_r3 = gm_8018F634();
    temp_r3->x2D = 1;
    temp_r4 = temp_r3->stage_selection_type;

    if (fn_80196594(temp_r3)) {
        temp_r3->x32 = 1;
        gm_SetPendingScene(3);
        gm_801A4B60();
        return;
    }
    if (fn_80196564(temp_r3)) {
        temp_r3->x32 = 1;
        temp_r3->x28 = lbl_804D4190;
    }
    gm_SetPendingScene(4);
    gm_801A4B60();
}

/// Randomly assigns bracket positions for tournament seeding.
void fn_80196684(s32 bracket_idx)
{
    BracketEntry* entries = (BracketEntry*) ((u8*) lbl_80473AB8 + 0);
    s32 rand_val = HSD_Randi(entries[bracket_idx].x51 +
                             entries[bracket_idx].x7D +
                             entries[bracket_idx].xA9);

    if (rand_val < (s32) entries[bracket_idx].x51) {
        entries[bracket_idx].x4C = 0;
        if (HSD_Randi(entries[bracket_idx].x7D +
                      entries[bracket_idx].xA9) <
            (s32) entries[bracket_idx].x7D)
        {
            entries[bracket_idx].x78 = 1;
            entries[bracket_idx].xA4 = 2;
            return;
        }
        entries[bracket_idx].x78 = 2;
        entries[bracket_idx].xA4 = 1;
        return;
    }
    if (rand_val <
        (s32) (entries[bracket_idx].x51 + entries[bracket_idx].x7D))
    {
        entries[bracket_idx].x78 = 0;
        if (HSD_Randi(entries[bracket_idx].x51 +
                      entries[bracket_idx].xA9) <
            (s32) entries[bracket_idx].x51)
        {
            entries[bracket_idx].x4C = 1;
            entries[bracket_idx].xA4 = 2;
            return;
        }
        entries[bracket_idx].x4C = 2;
        entries[bracket_idx].xA4 = 1;
        return;
    }
    entries[bracket_idx].xA4 = 0;
    if (HSD_Randi(entries[bracket_idx].x51 +
                  entries[bracket_idx].x7D) <
        (s32) entries[bracket_idx].x51)
    {
        entries[bracket_idx].x4C = 1;
        entries[bracket_idx].x78 = 2;
        return;
    }
    entries[bracket_idx].x4C = 2;
    entries[bracket_idx].x78 = 1;
}

void fn_801967E0(s32 arg0)
{
    s32 rand;
    BracketEntry* ent = &lbl_80473AB8[arg0];
    PAD_STACK(0x10);

    rand = HSD_Randi(ent->x51 + ent->x7D + ent->xA9 + ent->xD5);

    if (rand < (s32) ent->x51) {
        ent->x4C = 0;
        rand = HSD_Randi(ent->x7D + ent->xA9 + ent->xD5);
        if (rand < (s32) ent->x7D) {
            ent->x78 = 1;
            rand = HSD_Randi(ent->xA9 + ent->xD5);
            if (rand < (s32) ent->xA9) {
                ent->xA4 = 2;
                ent->xD0 = 3;
                return;
            }
            ent->xA4 = 3;
            ent->xD0 = 2;
            return;
        }
        if (rand < (s32) (ent->x7D + ent->xA9)) {
            ent->xA4 = 1;
            rand = HSD_Randi(ent->x7D + ent->xD5);
            if (rand < (s32) ent->x7D) {
                ent->x78 = 2;
                ent->xD0 = 3;
                return;
            }
            ent->x78 = 3;
            ent->xD0 = 2;
            return;
        }
        ent->xD0 = 1;
        rand = HSD_Randi(ent->x7D + ent->xA9);
        if (rand < (s32) ent->xA9) {
            ent->x78 = 2;
            ent->xA4 = 3;
            return;
        }
        ent->x78 = 3;
        ent->xA4 = 2;
        return;
    }

    if (rand < (s32) (ent->x51 + ent->x7D)) {
        ent->x78 = 0;
        rand = HSD_Randi(ent->x51 + ent->xA9 + ent->xD5);
        if (rand < (s32) ent->x51) {
            ent->x4C = 1;
            rand = HSD_Randi(ent->xA9 + ent->xD5);
            if (rand < (s32) ent->xA9) {
                ent->xA4 = 2;
                ent->xD0 = 3;
                return;
            }
            ent->xA4 = 3;
            ent->xD0 = 2;
            return;
        }
        if (rand < (s32) (ent->x51 + ent->xA9)) {
            ent->xA4 = 1;
            rand = HSD_Randi(ent->x51 + ent->xD5);
            if (rand < (s32) ent->x51) {
                ent->x4C = 2;
                ent->xD0 = 3;
                return;
            }
            ent->x4C = 3;
            ent->xD0 = 2;
            return;
        }
        ent->xD0 = 1;
        rand = HSD_Randi(ent->x51 + ent->xA9);
        if (rand < (s32) ent->x51) {
            ent->x4C = 2;
            ent->xA4 = 3;
            return;
        }
        ent->x4C = 3;
        ent->xA4 = 2;
        return;
    }

    if (rand < (s32) (ent->xA9 + ent->x51 + ent->x7D)) {
        ent->xA4 = 0;
        rand = HSD_Randi(ent->x51 + ent->x7D + ent->xD5);
        if (rand < (s32) ent->x51) {
            ent->x4C = 1;
            rand = HSD_Randi(ent->x7D + ent->xD5);
            if (rand < (s32) ent->x7D) {
                ent->x78 = 2;
                ent->xD0 = 3;
                return;
            }
            ent->x78 = 3;
            ent->xD0 = 2;
            return;
        }
        if (rand < (s32) (ent->x51 + ent->x7D)) {
            ent->x78 = 1;
            rand = HSD_Randi(ent->x51 + ent->xD5);
            if (rand < (s32) ent->x51) {
                ent->x4C = 2;
                ent->xD0 = 3;
                return;
            }
            ent->x4C = 3;
            ent->xD0 = 2;
            return;
        }
        ent->xD0 = 1;
        rand = HSD_Randi(ent->x51 + ent->x7D);
        if (rand < (s32) ent->x51) {
            ent->x4C = 2;
            ent->x78 = 3;
            return;
        }
        ent->x4C = 3;
        ent->x78 = 2;
        return;
    }

    ent->xD0 = 0;
    rand = HSD_Randi(ent->x51 + ent->x7D + ent->xA9);
    if (rand < (s32) ent->x51) {
        ent->x4C = 1;
        rand = HSD_Randi(ent->x7D + ent->xA9);
        if (rand < (s32) ent->x7D) {
            ent->x78 = 2;
            ent->xA4 = 3;
            return;
        }
        ent->x78 = 3;
        ent->xA4 = 2;
        return;
    }
    if (rand < (s32) (ent->x51 + ent->x7D)) {
        ent->x78 = 1;
        rand = HSD_Randi(ent->x51 + ent->xA9);
        if (rand < (s32) ent->x51) {
            ent->x4C = 2;
            ent->xA4 = 3;
            return;
        }
        ent->x4C = 3;
        ent->xA4 = 2;
        return;
    }
    ent->xA4 = 1;
    rand = HSD_Randi(ent->x51 + ent->x7D);
    if (rand < (s32) ent->x51) {
        ent->x4C = 2;
        ent->x78 = 3;
        return;
    }
    ent->x4C = 3;
    ent->x78 = 2;
    return;
}

/// With lbl_803DA0D0 defined in-TU (data reconstruction), MWCC unrolls the
/// constant-bound loop and then auto-inlines this function into
/// fn_8019B458; the target keeps the bl (idiom 73 pragma-at-callee).
#pragma dont_inline on
s32 fn_80196CF8(void)
{
    TmData* tmdata;
    s32 x24;
    s32 result;
    s32 entrants;
    s32 i;

    tmdata = gm_8018F634();
    entrants = tmdata->entrants;
    x24 = tmdata->x24;

    result = 0;
    if (x24 > (s32) lbl_803DA0D0.rank_thresholds[entrants][5]) {
        return 6;
    }

    for (i = 5; i >= 0; i--) {
        if (x24 <= (s32) lbl_803DA0D0.rank_thresholds[entrants][i]) {
            result = i;
        }
    }

    return result;
}
#pragma dont_inline off

static struct Lbl804799D8_t {
    u32 x0;       // 0x00 counter
    u32 x4;       // 0x04 frame counter
    s32 x8;       // 0x08
    s32 xC;       // 0x0C
    u16 x10;      // 0x10
    u16 x12[4];   // 0x12 per-player u16 counters
    u8 x1A;       // 0x1A
    u8 x1B;       // 0x1B
    u8 x1C;       // 0x1C
    u8 x1D[4];    // 0x1D per-player bytes
    u8 x21[4];    // 0x21 per-player anim counters
    u8 x25[4];    // 0x25 per-player bytes
    u8 x29;       // 0x29
    struct {      // 0x2A per-player anim data (stride 6)
        u8 start; // +0
        u8 cur;   // +1
        u8 end;   // +2
        u8 state; // +3
        u8 done;  // +4
        u8 loop;  // +5
    } x2A[4];
    u8 _pad0[2]; // 0x42-0x43
    u8 x44[4];   // 0x44 per-player state
    u8* x48;     // 0x48
    u8 x4C;      // 0x4C
    u8 x4D;      // 0x4D
    u8 x4E[20];  // 0x4E
    u8 _pad1[2]; // 0x62-0x63
    Vec3 x64;    // 0x64
    Vec3 x70;    // 0x70
    u8 _pad2[4]; // 0x7C-0x7F
} lbl_804799D8;

/// Cycles animation frame counter and updates JObj animation.
void fn_80196DBC(HSD_GObj* gobj)
{
    HSD_JObj* jobj;
    u16* counter = &lbl_804799D8.x10;

    jobj = gobj->hsd_obj;
    if (*counter > 0x320U) {
        *counter = 0;
    }
    fn_8019044C(jobj, (f32) *counter);
    *counter = *counter + 1;
}

/// Updates the tournament UI animation frame counter.
void fn_80196E30(HSD_GObj* gobj)
{
    u8* base_ptr;
    s32 cur_option;
    HSD_JObj* jobj;
    u8* x1A_ptr;
    u8 val;

    base_ptr = (u8*) &lbl_804799D8;
    cur_option = gm_8018F634()->cur_option;
    jobj = gobj->hsd_obj;

    if (cur_option <= 0x1A || cur_option >= 0x1F) {
        if ((u8) * (x1A_ptr = base_ptr + 0x1A) > 0x77U) {
            *x1A_ptr = 0x5A;
        }
    } else if (cur_option <= 0x1E) {
        if ((u8) * (x1A_ptr = base_ptr + 0x1A) > 0x9FU) {
            *x1A_ptr = 0x82;
        }
    }

    val = *(base_ptr += 0x1A);
    fn_8019044C(jobj, (f32) val);
    *base_ptr = *base_ptr + 1;
}

/// Tournament mode JObj visibility/animation callback.
void fn_80196EEC(HSD_GObj* gobj)
{
    TmData* tmdata;
    HSD_JObj* jobj;
    u8* counter;
    int in_range;

    tmdata = gm_8018F634();
    jobj = gobj->hsd_obj;

    if (gm_8018F634()->cur_option >= 0x13 && gm_8018F634()->cur_option <= 0x1A)
    {
        in_range = 1;
    } else {
        in_range = 0;
    }

    if (in_range == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
    counter = &lbl_804799D8.x1B;

    if (*counter != 0) {
        *counter = *counter - 1;
        HSD_JObjAnimAll(jobj);
        if (tmdata->x33 == tmdata->pad_x34[0]) {
            if (*counter > 2) {
                *counter = 2;
            }
            HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        }
    } else {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        if (tmdata->cur_option == 0x14) {
            tmdata->cur_option = 0x15;
        }
    }
}

extern s32 lbl_803B7CE0[9];

extern const f32 lbl_804DA7E0; // -19.5f
extern const f32 lbl_804DA7E4; // 13.0f
extern const f32 lbl_804DA7E8; // 6.5f
extern const f32 lbl_804DA7EC; // 19.5f
extern const f32 lbl_804DA7F0; // 2.0f
extern const f32 lbl_804DA808; // 0.0f
extern const f32 lbl_804DA80C; // 1.0f

void fn_80196FFC(HSD_GObj* gobj)
{
    TmData* tm;
    struct Lbl804799D8_t* d8 = &lbl_804799D8;
    HSD_JObj* jobj;
    s32 pnum;
    s32 in_range;
    f32 x;
    u8 players;
    u8 state;
    u8 start_frame, cur_frame, end_frame, loop_flag;
    struct lbl_803B7CE0_t {
        s32 x[9];
    } table;

    tm = gm_8018F634();
    pnum = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    table = *(struct lbl_803B7CE0_t*) lbl_803B7CE0;

    if ((s32) gm_8018F634()->cur_option >= 0x1B &&
        (s32) gm_8018F634()->cur_option <= 0x1E)
    {
        in_range = 1;
    } else {
        in_range = 0;
    }

    if (in_range == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if ((s8) (u8) HSD_PadMasterStatus[(u8) pnum].err != 0 &&
        (u8) tm->x4B8[pnum].x0 != 1)
    {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        HSD_SisLib_803A7548(tm->x524[2], (s32) pnum, lbl_804DA808, lbl_804DA808);
        return;
    }

    state = d8->x2A[pnum].state;
    if (state == 4) {
        HSD_SisLib_803A7548(tm->x524[2], (s32) pnum, lbl_804DA808, lbl_804DA808);
    } else {
        HSD_SisLib_803A7548(tm->x524[2], (s32) pnum, lbl_804DA80C, lbl_804DA80C);
    }

    players = tm->x30;
    if ((s32) players == 4) {
        x = (lbl_804DA7E4 * (f32) pnum) + lbl_804DA7E0;
    } else if ((s32) players == 3) {
        x = lbl_804DA7E8 + ((lbl_804DA7E4 * (f32) pnum) - lbl_804DA7EC);
    } else {
        x = lbl_804DA7E8 + ((lbl_804DA7E4 * (lbl_804DA7F0 * (f32) pnum)) - lbl_804DA7EC);
    }

    HSD_JObjSetTranslateX(jobj, x);

    tm->x524[2]->hidden = 0;

    state = d8->x2A[pnum].state;
    d8->x2A[pnum].start = ((u16*) &table)[state * 3 + 0];
    d8->x2A[pnum].end = ((u16*) &table)[state * 3 + 1];
    d8->x2A[pnum].loop = ((u16*) &table)[state * 3 + 2];

    start_frame = d8->x2A[pnum].start;
    cur_frame = d8->x2A[pnum].cur;
    end_frame = d8->x2A[pnum].end;
    loop_flag = d8->x2A[pnum].loop;

    if (cur_frame < start_frame) {
        d8->x2A[pnum].cur = start_frame;
        cur_frame = start_frame;
    }

    if (cur_frame < end_frame) {
        d8->x2A[pnum].cur = (u8) (cur_frame + 1);
    } else {
        d8->x2A[pnum].done = 1;
        if (loop_flag != 0) {
            d8->x2A[pnum].cur = start_frame;
        } else {
            d8->x2A[pnum].cur = end_frame;
        }
    }

    state = d8->x2A[pnum].state;
    if (state == 0 && d8->x44[pnum] == 6) {
        if (d8->x2A[pnum].done != 0 && (u8) tm->x4B8[pnum].x0 != 0) {
            d8->x2A[pnum].state = 1;
            d8->x2A[pnum].done = 0;
        }
    }

    state = d8->x2A[pnum].state;
    if (state == 1) {
        if (d8->x2A[pnum].done != 0) {
            d8->x2A[pnum].state = 2;
            d8->x2A[pnum].done = 0;
        }
    }

    fn_8019044C(jobj, (f32) d8->x2A[pnum].cur);
}

extern const f32 lbl_804DA7E0; // -19.5f
extern const f32 lbl_804DA7E4; // 13.0f
extern const f32 lbl_804DA7E8; // 6.5f
extern const f32 lbl_804DA7EC; // 19.5f
extern const f32 lbl_804DA7F0; // 2.0f
extern const f32 lbl_804DA810; // 4.5f
extern const f32 lbl_804DA814; // 5.5f
extern const f32 lbl_804DA818; // 666.0f
extern const f32 lbl_804DA81C; // 0.3f
extern const f32 lbl_804DA820; // 12.6f

/// Updates visibility and position of a tournament menu JObj.

void fn_801973F8(HSD_GObj* gobj)
{
    TmData* tm;
    s32 pnum;
    HSD_JObj* jobj;
    f32 x;
    s32 cond;
    u8 player_count;

    tm = gm_8018F634();
    pnum = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (gm_8018F634()->cur_option >= 0x1B && gm_8018F634()->cur_option <= 0x1E)
    {
        cond = 1;
    } else {
        cond = 0;
    }

    if (cond == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if ((s8) (u8) HSD_PadMasterStatus[(u8) pnum].err != 0 &&
        tm->x4B8[pnum].x0 != 1)
    {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    player_count = tm->x30;
    if ((s32) player_count == 4) {
        x = (lbl_804DA7E4 * (f32) pnum) + lbl_804DA7E0;
    } else if ((s32) player_count == 3) {
        x = lbl_804DA7E8 + ((lbl_804DA7E4 * (f32) pnum) - lbl_804DA7EC);
    } else {
        x = lbl_804DA7E8 +
            ((lbl_804DA7E4 * (lbl_804DA7F0 * (f32) pnum)) - lbl_804DA7EC);
    }

    fn_8018FDC4(jobj, lbl_804DA810 + x, lbl_804DA814, lbl_804DA818);

    if (lbl_804799D8.x2A[pnum].state == 4) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    }
}

/// Updates visibility of a tournament menu JObj based on current menu state.
void fn_801975C8(HSD_GObj* gobj)
{
    TmData* tmdata;
    u32 idx;
    HSD_JObj* jobj;
    s32 in_range;

    tmdata = gm_8018F634();
    idx = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (gm_8018F634()->cur_option >= 0x1B && gm_8018F634()->cur_option <= 0x1E)
    {
        in_range = 1;
    } else {
        in_range = 0;
    }

    if (in_range == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if (HSD_PadMasterStatus[(u8) idx].err != 0 && tmdata->x4B8[idx].x0 != 1) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    if (lbl_804799D8.x2A[idx].state == 4) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    }
}

/// Updates visibility and animation of tournament menu JObj.
void fn_801976D4(HSD_GObj* gobj)
{
    HSD_JObj* jobj;
    u8* counter;
    s32 is_in_range;
    PAD_STACK(8);

    gm_8018F634();
    jobj = gobj->hsd_obj;
    if (gm_8018F634()->cur_option >= 0x1B && gm_8018F634()->cur_option <= 0x1E)
    {
        is_in_range = 1;
    } else {
        is_in_range = 0;
    }
    if (is_in_range == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    } else {
        HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
        counter = &lbl_804799D8.x1C;
        if (*counter > 0x3C) {
            *counter = 0x3C;
        }
        fn_8019044C(jobj, (f32) *counter);
        *counter = *counter + 1;
    }
}

void fn_801977AC(HSD_GObj* gobj)
{
    TmData* tm;
    s32 pnum;
    HSD_JObj* jobj;
    struct Lbl804799D8_t* d8 = &lbl_804799D8;
    s32 in_range;
    f32 x;
    u8 players;

    tm = gm_8018F634();
    pnum = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (gm_8018F634()->cur_option >= 0x1B && gm_8018F634()->cur_option <= 0x1E)
    {
        in_range = 1;
    } else {
        in_range = 0;
    }

    if (in_range == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    players = tm->x30;
    if ((s32) players == 4) {
        x = (lbl_804DA7E4 * (f32) pnum) + lbl_804DA7E0;
    } else if ((s32) players == 3) {
        x = lbl_804DA7E8 + ((lbl_804DA7E4 * (f32) pnum) - lbl_804DA7EC);
    } else {
        x = lbl_804DA7E8 +
            ((lbl_804DA7E4 * (lbl_804DA7F0 * (f32) pnum)) - lbl_804DA7EC);
    }

    fn_8018FDC4(jobj, lbl_804DA81C + x, lbl_804DA820, lbl_804DA818);

    if (d8->x2A[pnum].state == 4) {
        if (d8->x1D[pnum] < 0x28) {
            d8->x1D[pnum] = d8->x1D[pnum] + 1;
        }

        HSD_JObjSetTranslateY(jobj, lbl_803DA0D0.bounce_y[d8->x1D[pnum]]);
    } else {
        d8->x1D[pnum] = 0;
    }

    if ((s8) (u8) HSD_PadMasterStatus[(u8) pnum].err != 0 &&
        tm->x4B8[pnum].x0 != 1)
    {
        HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

        players = tm->x30;
        if ((s32) players == 4) {
            x = (lbl_804DA7E4 * (f32) pnum) + lbl_804DA7E0;
        } else if ((s32) players == 3) {
            x = lbl_804DA7E8 + ((lbl_804DA7E4 * (f32) pnum) - lbl_804DA7EC);
        } else {
            x = lbl_804DA7E8 +
                ((lbl_804DA7E4 * (lbl_804DA7F0 * (f32) pnum)) - lbl_804DA7EC);
        }

        fn_8018FDC4(jobj, lbl_804DA81C + x, lbl_804DA820, lbl_804DA818);
    }
}

void fn_80197AF0(HSD_GObj* gobj)
{
    TmData* tm;
    s32 pnum;
    HSD_JObj* jobj;
    struct Lbl804799D8_t* d8 = &lbl_804799D8;
    s32 in_range;
    f32 x;
    u8 players;
    u8 state;

    tm = gm_8018F634();
    pnum = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (((s32) gm_8018F634()->cur_option >= 0x1B) &&
        ((s32) gm_8018F634()->cur_option <= 0x1E))
    {
        in_range = 1;
    } else {
        in_range = 0;
    }

    if (in_range == 0) {
        HSD_JObjSetFlagsAll(jobj, 0x10U);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, 0x10U);

    if (((s8) HSD_PadMasterStatus[(u8) pnum].err != 0) &&
        ((u8) tm->x4B8[pnum].x0 != 1))
    {
        HSD_JObjSetFlagsAll(jobj, 0x10U);
        return;
    }

    if ((u8) tm->x4B8[pnum].x0 == 1) {
        HSD_JObjSetFlagsAll(jobj, 0x10U);
    }

    players = tm->x30;
    if ((s32) players == 4) {
        x = (lbl_804DA7E4 * (f32) pnum) + lbl_804DA7E0;
    } else if ((s32) players == 3) {
        x = lbl_804DA7E8 + ((lbl_804DA7E4 * (f32) pnum) - lbl_804DA7EC);
    } else {
        x = lbl_804DA7E8 +
            ((lbl_804DA7E4 * (lbl_804DA7F0 * (f32) pnum)) - lbl_804DA7EC);
    }

    fn_8018FDC4(jobj, x, lbl_804DA818, lbl_804DA818);

    if (d8->x44[pnum] != 6 || d8->x2A[pnum].state == 1 ||
        d8->x2A[pnum].state == 2 || d8->x2A[pnum].state == 4)
    {
        HSD_JObjSetFlagsAll(jobj, 0x10U);
    }

    if (d8->x12[pnum] < 0x258U) {
        d8->x12[pnum] = (u16) (d8->x12[pnum] + 1);
    } else {
        d8->x12[pnum] = 0U;
    }
    fn_8019044C(jobj, (f32) d8->x12[pnum]);
}

/// Updates visibility of a menu JObj based on current menu option.
void fn_80197D4C(HSD_GObj* gobj)
{
    HSD_JObj* jobj;
    u8* counter;
    int cond;
    PAD_STACK(8);

    gm_8018F634();
    jobj = gobj->hsd_obj;
    if (gm_8018F634()->cur_option >= 0x1B && gm_8018F634()->cur_option <= 0x1E)
    {
        cond = 1;
    } else {
        cond = 0;
    }
    if (cond == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }
    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
    counter = &lbl_804799D8.x29;
    fn_8019044C(jobj, (float) lbl_804799D8.x29);
    if (*counter < 10U) {
        *counter = *counter + 1;
    }
}

/// @todo Currently 69.71% match - needs register allocation fix
/// Updates player HUD element visibility and position.
void fn_80197E18(HSD_GObj* gobj)
{
    HSD_JObj* jobj;
    TmData* data;
    s32 pnum;
    s32 in_range;
    f32 x_pos;

    data = gm_8018F634();
    pnum = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (gm_8018F634()->cur_option >= 0x1B && gm_8018F634()->cur_option <= 0x1E)
    {
        in_range = 1;
    } else {
        in_range = 0;
    }

    if (in_range == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if (HSD_PadMasterStatus[(u8) pnum].err != 0 && data->x4B8[pnum].x0 != 1) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    if (data->x4B8[pnum].x0 != 1) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    }

    if ((s32) data->x30 == 4) {
        x_pos = (lbl_804DA7E4 * pnum) + (lbl_804DA7E0);
    } else if ((s32) data->x30 == 3) {
        x_pos = lbl_804DA7E8 + ((lbl_804DA7E4 * pnum) - lbl_804DA7EC);
    } else {
        x_pos = lbl_804DA7E8 +
                ((lbl_804DA7E4 * (lbl_804DA7F0 * pnum)) - lbl_804DA7EC);
    }
    fn_8018FDC4(jobj, x_pos, lbl_804DA818, lbl_804DA818);
}

/// @todo Currently 70.24% match - needs register allocation fix
/// Updates JObj visibility based on menu option and player state.
void fn_80197FD8(HSD_GObj* gobj)
{
    TmData* tm_data;
    s32 port;
    HSD_JObj* jobj;
    s32 option_in_range;
    u8 port_u8;

    tm_data = gm_8018F634();
    port = fn_8018F62C(gobj);
    jobj = (HSD_JObj*) gobj->hsd_obj;

    if (gm_8018F634()->cur_option >= 0x1B && gm_8018F634()->cur_option <= 0x1E)
    {
        option_in_range = 1;
    } else {
        option_in_range = 0;
    }

    if (option_in_range == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    port_u8 = (u8) port;
    if ((s8) HSD_PadMasterStatus[port_u8].err != 0) {
        if (tm_data->x4B8[port].x0 != 1) {
            HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
            return;
        }
    } else {
        if (lbl_804799D8.x2A[port].state == 4) {
            HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        }
    }

    {
        f32 x_position;
        u8 num_players = tm_data->x30;

        if ((s32) num_players == 4) {
            x_position = (lbl_804DA7E4 * (f32) port) + lbl_804DA7E0;
        } else if ((s32) num_players == 3) {
            x_position =
                lbl_804DA7E8 + ((lbl_804DA7E4 * (f32) port) - lbl_804DA7EC);
        } else {
            x_position =
                lbl_804DA7E8 +
                ((lbl_804DA7E4 * (lbl_804DA7F0 * (f32) port)) - lbl_804DA7EC);
        }

        fn_8018FDC4(jobj, x_position, lbl_804DA818, lbl_804DA818);
    }
}

extern const f32 lbl_804DA824; // 0.01f

/// @todo Down to linking
/// Updates player cursor animation and position.
void fn_801981A0(HSD_GObj* gobj)
{
    TmData* data;
    s32 pnum;
    HSD_JObj* jobj;
    struct Lbl804799D8_t* d8 = &lbl_804799D8;
    u8* state_ptr;
    f32 x_pos;
    s32 in_range;
    u8 counter;

    data = gm_8018F634();
    pnum = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (gm_8018F634()->cur_option >= 0x1B && gm_8018F634()->cur_option <= 0x1E)
    {
        in_range = 1;
    } else {
        in_range = 0;
    }

    if (in_range == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if (HSD_PadMasterStatus[(u8) pnum].err != 0 &&
        d8->x2A[pnum].state != 4)
    {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    if (*(&d8->x44[pnum]) == 6) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    }

    if ((s32) data->x30 == 4) {
        x_pos = (lbl_804DA7E4 * (f32) pnum) + (lbl_804DA7E0);
    } else if ((s32) data->x30 == 3) {
        x_pos = lbl_804DA7E4;
        x_pos = lbl_804DA7E8 + ((x_pos * (f32) pnum) - lbl_804DA7EC);
    } else {
        x_pos = lbl_804DA7E8 +
                ((lbl_804DA7E4 * (lbl_804DA7F0 * (f32) pnum)) - lbl_804DA7EC);
    }

    fn_8018FDC4(jobj, x_pos, lbl_804DA818, lbl_804DA824);

    counter = d8->x21[pnum];
    counter = (counter + 1) % 11;
    d8->x21[pnum] = counter;

    if (*(&d8->x44[pnum]) == 7) {
        fn_8019044C(jobj, (f32) d8->x21[pnum]);
        return;
    }

    fn_8019044C(jobj, (f32) (d8->x21[pnum] + 0x14));
}

/// Updates the visibility and position of a player's controller indicator.
void fn_801983E4(HSD_GObj* gobj)
{
    TmData* tm;
    s32 pnum;
    HSD_JObj* jobj;
    f32 x;
    s32 cond;
    u8 player_count;

    tm = gm_8018F634();
    pnum = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (gm_8018F634()->cur_option >= 0x1B && gm_8018F634()->cur_option <= 0x1E)
    {
        cond = 1;
    } else {
        cond = 0;
    }

    if (cond == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if ((s8) (u8) HSD_PadMasterStatus[(u8) pnum].err == 0 ||
        tm->x4B8[pnum].x0 == 1)
    {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    player_count = tm->x30;
    if ((s32) player_count == 4) {
        x = (lbl_804DA7E4 * (f32) pnum) + lbl_804DA7E0;
    } else if ((s32) player_count == 3) {
        x = lbl_804DA7E8 + ((lbl_804DA7E4 * (f32) pnum) - lbl_804DA7EC);
    } else {
        x = lbl_804DA7E8 +
            ((lbl_804DA7E4 * (lbl_804DA7F0 * (f32) pnum)) - lbl_804DA7EC);
    }

    fn_8018FDC4(jobj, x, lbl_804DA818, lbl_804DA824);
}

void fn_80198584(ResultsData* results)
{
    HSD_JObj* jobj;
    int cur_option;

    cur_option = gm_8018F634()->cur_option;
    jobj = results->x28;
    if (cur_option == 0x1D) {
        HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
    } else {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    }
}

extern s32 lbl_804D667C;

void fn_801985D4(HSD_GObj* gobj)
{
    HSD_JObj* jobj;
    HSD_JObj* child;
    s32 in_range;

    /* @-pin shim (+1): repays the pool id freed by the 0.0f -> lbl_804DA808
     * conversions so the landed @698/@699/@795 symbols.txt pins hold
     * (idioms 41/90: label ids bind at function entry). */
    goto body;
body:
    gm_8018F634();
    jobj = gobj->hsd_obj;

    if ((s32) gm_8018F634()->cur_option >= 0x1F &&
        (s32) gm_8018F634()->cur_option <= 0x27)
    {
        in_range = 1;
    } else {
        in_range = 0;
    }

    if (in_range == 0) {
        HSD_JObjSetTranslateZ(jobj, 10000.0f);
    } else {
        HSD_JObjSetTranslateZ(jobj, lbl_804DA808);

        if (lbl_804799D8.x0 < 0xAU) {
            lbl_804D667C = 1;
            return;
        }

        HSD_JObjAnimAll(jobj);
        child = HSD_JObjGetChild(jobj);

        if (HSD_DObjGetFlags(HSD_JObjGetDObj(child)) & 1) {
            lbl_804D667C = 1;
        } else {
            lbl_804D667C = 0;
        }

        HSD_JObjGetTranslation(child, &lbl_804799D8.x64);
        HSD_JObjGetScale(child, &lbl_804799D8.x70);
    }
}

/// Animates and positions the tournament bracket results overlay JObj.
/// Uses HSD_JObjSetTranslateZ for visibility, HSD_JObjSetTranslate/SetScale
/// for positioning from lbl_804799D8 cached values.
void fn_80198824(HSD_GObj* gobj)
{
    HSD_JObj* jobj;
    s32 in_range;

    gm_8018F634();
    jobj = gobj->hsd_obj;

    if ((s32) gm_8018F634()->cur_option >= 0x1F &&
        (s32) gm_8018F634()->cur_option <= 0x27)
    {
        in_range = 1;
    } else {
        in_range = 0;
    }

    if (in_range == 0) {
        HSD_JObjSetTranslateZ(jobj, 10000.0f);
    } else {
        HSD_JObjSetTranslateZ(jobj, lbl_804DA808);

        if (lbl_804D667C == 0) {
            HSD_JObjSetTranslateZ(jobj, 10000.0f);
        } else {
            u32* frame_ctr = &lbl_804799D8.x4;
            u32 min_frame = (u32) lbl_804799D8.x8;
            u32 max_frame = (u32) lbl_804799D8.xC;

            if (*frame_ctr < min_frame) {
                *frame_ctr = min_frame;
            }
            if (*frame_ctr < max_frame) {
                *frame_ctr = *frame_ctr + 1;
            }

            HSD_JObjReqAnimAll(jobj, (f32) *frame_ctr);
            HSD_JObjAnimAll(jobj);

            HSD_JObjSetTranslate(jobj, &lbl_804799D8.x64);
            HSD_JObjSetScale(jobj, &lbl_804799D8.x70);
        }
    }
}

/// Initializes tournament mode match data.
/// Initializes tournament mode match data structures.
/// Type casts used to match target instruction patterns (stw/sth vs stb).
#pragma push
#pragma auto_inline off
void fn_80198BA0(void)
{
    TmData* td;
    u8* ptr;
    u8* ptr7;
    u8* ptr8;
    s32 i;

    ptr = (u8*) &lbl_804799D8;
    td = gm_8018F634();

    /* Clear first 4 bytes as word (stw instruction) */
    *(u32*) &ptr[0x00] = 0;
    if (td && td) {
        /* FAKE MATCH: Empty block affects register allocation */
    }
    /* Clear 2 bytes at offset 0x10 as halfword (sth instruction) */
    *(u16*) &ptr[0x10] = 0;

    if (td->x2C != 0) {
        ptr[0x1A] = 0x50;
    } else {
        ptr[0x1A] = 0x5A;
    }

    ptr[0x1C] = 0;
    ptr7 = ptr;
    ptr8 = ptr;

    for (i = 0; i < td->x30; i++) {
        ptr[0x25] = 0;
        /* Clear 2 bytes as halfword (sth instruction) */
        *(u16*) &ptr7[0x12] = 0;
        ptr7 += 2;
        ptr[0x1D] = 0;
        ptr8[0x2A] = 0;
        ptr8[0x2B] = 0;
        ptr8[0x2C] = 0x1E;
        ptr8[0x2E] = 0;
        ptr8[0x2F] = 0;
        ptr8[0x2D] = 0;
        ptr8 += 6;
        ptr[0x44] = 6;
        ptr++;
    }
}
#pragma pop

/// @todo Currently 99.63% match - permuter couldn't improve (instruction
/// scheduling)
/// Initializes tournament mode text displays.
extern const f32 lbl_804DA82C; // 0.054945f
extern const f32 lbl_804DA830; // 0.08f
extern const f32 lbl_804DA834; // 320.0f
extern const f32 lbl_804DA838; // 250.0f
extern const f32 lbl_804DA83C; // 1.5f
extern const f32 lbl_804DA840; // 80.0f

#pragma push
#pragma auto_inline off
void fn_80198C60(void)
{
    TmData* td;
    HSD_Text* text;

    PAD_STACK(24);

    td = gm_8018F634();
    td->x524[2] = HSD_SisLib_803A6754(0, (s32) lbl_804D663C);
    text = td->x524[2];
    text->font_size.x = lbl_804DA82C;
    text->font_size.y = lbl_804DA830;
    td->x524[2]->default_alignment = 1;
    td->x524[2]->default_kerning = 1;

    td->x524[3] = HSD_SisLib_803A6754(0, (s32) lbl_804D663C);
    td->x524[3]->default_alignment = 1;
    td->x524[3]->default_kerning = 1;
    goto pad_label_a; // @-pin shims (+2): five literal->extern swaps keep the
pad_label_a:;        // "    " string pinned at @3652
    goto pad_label_b;
pad_label_b:;
    HSD_SisLib_803A6B98(td->x524[3], lbl_804DA834, lbl_804DA838, "    ");
    HSD_SisLib_803A7548(td->x524[3], 0, lbl_804DA83C, lbl_804DA83C);
}
#pragma pop

extern SceneDesc* lbl_804D666C;
extern const f32 lbl_804DA808; // 0.0f

/// Initializes the scene rendering components for the gm_18A5 game mode.
void fn_80198D18(void)
{
    TmData* td;
    HSD_Text* text;
    HSD_GObj* gobj;
    PAD_STACK(24);

    gm_8018F634();
    gobj = fn_80190174(lbl_804D666C->cameras->desc);
    fn_801901F8(lbl_804D666C->cameras->desc);
    fn_801902F0((int) gobj);
    fn_8019027C(lbl_804D666C->lights);
    fn_8019035C(0, lbl_804D666C->models[5], 0, 0x1A, 2, 1, fn_80196DBC,
                lbl_804DA808);
    fn_8019035C(0, lbl_804D666C->models[4], 0, 0x1A, 2, 1, fn_80196E30,
                lbl_804DA840);

    td = gm_8018F634();
    td->x524[2] = HSD_SisLib_803A6754(0, (s32) lbl_804D663C);
    text = td->x524[2];
    text->font_size.x = lbl_804DA82C;
    text->font_size.y = lbl_804DA830;
    td->x524[2]->default_alignment = 1;
    td->x524[2]->default_kerning = 1;

    td->x524[3] = HSD_SisLib_803A6754(0, (s32) lbl_804D663C);
    td->x524[3]->default_alignment = 1;
    td->x524[3]->default_kerning = 1;
    HSD_SisLib_803A6B98(td->x524[3], lbl_804DA834, lbl_804DA838, "    ");
    HSD_SisLib_803A7548(td->x524[3], 0, lbl_804DA83C, lbl_804DA83C);

    gobj = GObj_Create(0xE, 0x1A, 0);
    HSD_GObjObject_80390A70(gobj, HSD_GObj_804D7848,
                            (0, HSD_FogLoadDesc(lbl_804D666C->fogs[0].desc)));
    GObj_SetupGXLink(gobj, HSD_GObj_FogCallback, 0, 0);
    fn_80198BA0();
}

extern SceneDesc* lbl_804D6674;
extern const f32 lbl_804DA844;
extern const f32 lbl_804DA848;
extern const f32 lbl_804DA84C;
extern const f32 lbl_804DA850;
extern const f32 lbl_804DA854;
extern const f32 lbl_804DA858;
extern const f32 lbl_804DA85C;
extern const f32 lbl_804DA860;
extern const f32 lbl_804DA864;
extern const f32 lbl_804DA868;
extern const f32 lbl_804DA86C;
extern const f32 lbl_804DA870;
extern const f32 lbl_804DA874;

void fn_80198EBC(void)
{
    TmData* td;
    HSD_GObj* gobj;
    HSD_JObj* jobj;
    HSD_JObj* jobj2;
    HSD_JObj* c;
    struct lbl_803DA0D0_t* da0d0;
    s32 i, j;
    f32 anim_rate;
    f32 pos;
    f32 hide_z, show_z;
    f32 f_848, f_850, f_84C, f_854, f_7F0;
    f32 f_858, f_834, f_85C;
    f32 f_7E4, f_7E0, f_7E8, f_7EC;
    f32 f_864;

    da0d0 = &lbl_803DA0D0;
    td = gm_8018F634();
    gm_8018F634();

    gobj = fn_80190174(lbl_804D666C->cameras->desc);
    fn_801901F8(lbl_804D666C->cameras->desc);
    fn_801902F0((int) gobj);
    fn_8019027C(lbl_804D666C->lights);

    fn_8019035C(0, lbl_804D666C->models[5], 0, 0x1A, 2, 1, fn_80196DBC,
                lbl_804DA808);
    fn_8019035C(0, lbl_804D666C->models[4], 0, 0x1A, 2, 1, fn_80196E30,
                lbl_804DA840);
    fn_80198C60();

    gobj = GObj_Create(0xE, 0x1A, 0);
    HSD_GObjObject_80390A70(gobj, HSD_GObj_804D7848,
                            HSD_FogLoadDesc(lbl_804D666C->fogs[0].desc));
    GObj_SetupGXLink(gobj, HSD_GObj_FogCallback, 0, 0);
    fn_80198BA0();

    hide_z = 10000.0f;
    f_848 = lbl_804DA848;
    f_850 = lbl_804DA850;
    f_84C = lbl_804DA84C;
    f_854 = lbl_804DA854;
    f_7F0 = lbl_804DA7F0;
    f_858 = lbl_804DA858;
    f_834 = lbl_804DA834;
    f_85C = lbl_804DA85C;
    show_z = lbl_804DA808;
    f_7E4 = lbl_804DA7E4;
    f_7E0 = lbl_804DA7E0;
    f_7E8 = lbl_804DA7E8;
    f_7EC = lbl_804DA7EC;
    f_864 = lbl_804DA864;

    for (i = 0; i < td->x30; i++) {
        if (td->x4B8[i].x0 == 0) {
            anim_rate = (f32) i;
        } else if (td->x4B8[i].x0 == 1) {
            anim_rate = lbl_804DA844;
        } else {
            anim_rate = (f32) i;
            td->x4B8[i].x0 = 0;
            lbl_804799D8.x44[i] = 6;
            lbl_804799D8.x2A[i].state = 4;
        }

        gobj = fn_8019035C(0, lbl_804D6674->models[12], 0, 0x1A, 2, 1,
                           fn_80196FFC, anim_rate);
        jobj = GET_JOBJ(gobj);
        fn_8018FBD8(gobj, i);
        c = HSD_JObjGetChild(jobj);
        HSD_MObjRemoveAnimByFlags(c->u.dobj->mobj, 4);
        c = HSD_JObjGetChild(jobj);
        c = HSD_JObjGetNext(c);
        HSD_MObjRemoveAnimByFlags(c->u.dobj->mobj, 4);
        c = HSD_JObjGetChild(jobj);
        c = HSD_JObjGetNext(c);
        c = HSD_JObjGetNext(c);
        HSD_MObjRemoveAnimByFlags(c->u.dobj->mobj, 4);

        if (td->x30 == 4) {
            pos = f_848 + (f_850 * (f32) i + f_84C);
        } else if (td->x30 == 3) {
            pos = f_848 + (f_850 * (f32) i + f_854);
        } else {
            pos = f_848 + (f_850 * (f_7F0 * (f32) i) + f_854);
        }
        pos -= f_834;
        fn_8018ECA8(td->x4B8[i].x6, td->x4B8[i].x0, 5, f_858 * pos - f_85C,
                    lbl_804DA860, 5);

        td->x524[2]->hidden = true;

        gobj = fn_8019035C(0, lbl_804D6674->models[10], 0, 0x1A, 2, 1,
                           fn_801973F8, anim_rate);
        jobj = GET_JOBJ(gobj);
        fn_8018FBD8(gobj, i);

        c = HSD_JObjGetChild(jobj);
        jobj2 = c;

        HSD_JObjSetTranslateZ(c, hide_z);

        if (da0d0->icon_model_map[td->x4B8[i].x1] == 0) {
            HSD_JObjSetTranslateZ(c, show_z);

            for (j = 1; j <= 12; j++) {
                jobj2 = HSD_JObjGetNext(jobj2);
                HSD_JObjSetTranslateZ(jobj2, hide_z);
            }
        } else {
            for (j = 1; j <= 12; j++) {
                jobj2 = HSD_JObjGetNext(jobj2);
                HSD_JObjSetTranslateZ(jobj2, hide_z);

                if (da0d0->icon_model_map[td->x4B8[i].x1] == j) {
                    HSD_JObjSetTranslateZ(jobj2, show_z);

                    for (j++; j <= 12; j++) {
                        jobj2 = HSD_JObjGetNext(jobj2);
                        HSD_JObjSetTranslateZ(jobj2, hide_z);
                    }
                    break;
                }
            }
        }

        gobj = fn_8019035C(0, lbl_804D6674->models[4], 0, 0x1A, 2, 1,
                           fn_801975C8, anim_rate);
        jobj = GET_JOBJ(gobj);
        fn_8018FBD8(gobj, i);

        if (td->x30 == 4) {
            pos = f_7E4 * (f32) i + f_7E0;
        } else if (td->x30 == 3) {
            pos = f_7E8 + (f_7E4 * (f32) i - f_7EC);
        } else {
            pos = f_7E8 + (f_7E4 * (f_7F0 * (f32) i) - f_7EC);
        }
        fn_8018FDC4(jobj, pos - f_864, lbl_804DA868, lbl_804DA86C);

        c = HSD_JObjGetChild(jobj);
        jobj = HSD_JObjGetNext(c);
        fn_8019044C(jobj, fn_8018F71C(td->x4B8[i].x1, td->x4B8[i].x3));

        gobj = fn_8019035C(0, lbl_804D6674->models[9], 0, 0x1A, 2, 1,
                           fn_801977AC, anim_rate);
        jobj = GET_JOBJ(gobj);
        fn_8018FBD8(gobj, i);
        fn_8019044C(jobj, anim_rate);

        gobj = fn_8019035C(0, lbl_804D6674->models[8], 0, 0x1A, 2, 1,
                           fn_80197AF0, lbl_804DA808);
        fn_8018FBD8(gobj, i);

        gobj = fn_8019035C(0, lbl_804D6674->models[13], 0, 0x1A, 2, 1,
                           fn_80197E18, anim_rate);
        jobj = GET_JOBJ(gobj);
        fn_8018FBD8(gobj, i);

        if (td->x4B8[i].x0 == 1) {
            fn_8019044C(jobj, (f32) td->x4B8[i].x4);
        } else {
            HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        }

        if (td->x31 != 0) {
            gobj = fn_8019035C(0, lbl_804D6674->models[11], 0, 0x1A, 2, 1,
                               fn_80197FD8, anim_rate);
            jobj = GET_JOBJ(gobj);
            fn_8018FBD8(gobj, i);
            fn_8019044C(jobj, (f32) td->x4B8[i].x5);
        }

        gobj = fn_8019035C(0, lbl_804D6674->models[7], 0, 0x1A, 2, 1,
                           fn_801981A0, anim_rate);
        jobj = GET_JOBJ(gobj);
        fn_8018FBD8(gobj, i);
        fn_8018FDC4(jobj, lbl_804DA870, lbl_804DA818, lbl_804DA874);

        gobj = fn_8019035C(0, lbl_804D6674->models[14], 0, 0x1A, 2, 1,
                           fn_801983E4, anim_rate);
        fn_8018FBD8(gobj, i);
    }

    fn_8019035C(0, lbl_804D6674->models[5], 0, 0x1A, 2, 1, fn_801976D4,
                lbl_804DA808);

    gobj = fn_8019035C(0, lbl_804D6674->models[0], 0, 0x1A, 2, 1, NULL,
                       lbl_804DA808);
    {
        HSD_JObj* j16;
        j16 = GET_JOBJ(gobj);

        j16 = HSD_JObjGetChild(j16);
        j16 = HSD_JObjGetNext(j16);
        j16 = HSD_JObjGetNext(j16);
        j16 = HSD_JObjGetNext(j16);
        j16 = HSD_JObjGetNext(j16);
        j16 = HSD_JObjGetNext(j16);
        for (j = 0; j <= 40; j++) {
            fn_8019044C(j16, (f32) (j + 10));
            da0d0->bounce_y[j] = HSD_JObjGetTranslationY(j16);
        }
    }

    fn_8019035C(0, lbl_804D6674->models[6], 0, 0x1A, 2, 1, fn_80197D4C,
                lbl_804DA808);
    fn_8019035C(0, lbl_804D6674->models[2], 0, 0x1A, 3, 1,
                (void (*)(HSD_GObj*)) fn_80198584, lbl_804DA808);
}

extern MatchEnd gm_80477738;
extern SceneDesc* lbl_804D6670;

void fn_80199AF0(void)
{
    TmData* td1;
    TmData* td2;
    HSD_JObj* jobj;
    HSD_JObj* next;
    HSD_GObj* gobj;
    s32 mode;
    s32 slot;
    s32 bracket_idx;
    s32 result;
    s32 i;
    s32 local1;
    s32 local2;
    PAD_STACK(16);

    td1 = gm_8018F634();
    td2 = gm_8018F634();

    result = fn_8018F508(&local1);
    if (result == 1) {
        result = 1;
    } else if (td2->x33 == 5) {
        result = 2;
    } else {
        result = 0;
    }
    mode = result;

    result = fn_8018F508(&local2);
    if (result == 1) {
        slot = local2;
    } else {
        u8* p = (u8*) &gm_80477738;
        if (p[0x58] != 3 && p[0x5E] == 0) {
            slot = 0;
        } else {
            p += 0xA8;
            if (p[0x58] != 3 && p[0x5E] == 0) {
                slot = 1;
            } else {
                p += 0xA8;
                if (p[0x58] != 3 && p[0x5E] == 0) {
                    slot = 2;
                } else {
                    p += 0xA8;
                    if (p[0x58] != 3 && p[0x5E] == 0) {
                        slot = 3;
                    } else {
                        slot = -1;
                    }
                }
            }
        }
    }

    bracket_idx = fn_8018F74C();
    gm_8018F634();

    gobj = fn_80190174(lbl_804D666C->cameras->desc);
    fn_801901F8(lbl_804D666C->cameras->desc);
    fn_801902F0((int) gobj);
    fn_8019027C(lbl_804D666C->lights);
    fn_8019035C(0, lbl_804D666C->models[5], 0, 0x1A, 2, 1, fn_80196DBC, lbl_804DA808);
    fn_8019035C(0, lbl_804D666C->models[4], 0, 0x1A, 2, 1, fn_80196E30,
                lbl_804DA840);

    fn_80198C60();

    gobj = GObj_Create(0xE, 0x1A, 0);
    HSD_GObjObject_80390A70(gobj, HSD_GObj_804D7848,
                            HSD_FogLoadDesc(lbl_804D666C->fogs[0].desc));
    GObj_SetupGXLink(gobj, HSD_GObj_FogCallback, 0, 0);
    fn_80198BA0();

    if (td1->x33 == 5) {
        mode = 2;
    }

    if (mode != 2) {
        if (lbl_80473AB8[bracket_idx].x18 == 0) {
            return;
        }
    }

    fn_8019035C(0, lbl_804D6670->models[0], mode, 0x1A, 3, 1, fn_801985D4,
                lbl_804DA808);
    gobj = fn_8019035C(0, lbl_804D6670->models[2], 0, 0x1A, 3, 1, fn_80198824,
                       lbl_804DA808);

    if (gobj->hsd_obj == NULL) {
        jobj = NULL;
    } else {
        jobj = ((HSD_JObj*) gobj->hsd_obj)->child;
    }

    lbl_804799D8.x8 = mode * 0x14;
    if (mode == 2) {
        lbl_804799D8.xC = 0x96;
    } else {
        lbl_804799D8.xC = mode * 0x14 + 0x13;
    }

    HSD_JObjSetTranslateZ(jobj, 10000.0f);

    if (lbl_803DA0D0.icon_model_map[td1->x4B8[slot].x1] == 0) {
        HSD_JObjSetTranslateZ(jobj, lbl_804DA808);
        for (i = 1; i <= 12; i++) {
            if (jobj == NULL) {
                next = NULL;
            } else {
                next = jobj->next;
            }
            jobj = next;
            HSD_JObjSetTranslateZ(next, 10000.0f);
        }
    } else {
        for (i = 1; i <= 12; i++) {
            if (jobj == NULL) {
                next = NULL;
            } else {
                next = jobj->next;
            }
            jobj = next;
            HSD_JObjSetTranslateZ(next, 10000.0f);

            if ((s32) lbl_803DA0D0.icon_model_map[td1->x4B8[slot].x1] == i) {
                HSD_JObjSetTranslateZ(next, lbl_804DA808);
                for (slot = i + 1; slot <= 12; slot++) {
                    if (jobj == NULL) {
                        next = NULL;
                    } else {
                        next = jobj->next;
                    }
                    jobj = next;
                    HSD_JObjSetTranslateZ(next, 10000.0f);
                }
                break;
            }
        }
    }
}

void fn_8019A158(void)
{
    TmData* td1;
    TmData* td2;
    s32 mode;
    s32 slot;
    s32 bracket_idx;
    s32 result;
    s32 local1, local2;
    s32 i;
    s32 counter;
    u8* me;
    u8* cursor;
    PAD_STACK(16);

    td1 = gm_8018F634();
    lbl_804799D8.x48 = (u8*) &gm_80477738;
    mode = 0;
    lbl_804799D8.x0 = 0;

    td2 = gm_8018F634();

    result = fn_8018F508(&local1);
    if (result == 1) {
        mode = 1;
    } else if (td2->x33 == 5) {
        mode = 2;
    }

    me = lbl_804799D8.x48;
    result = fn_8018F508(&local2);
    if (result == 1) {
        slot = local2;
    } else if (me[0x58] != 3 && me[0x5E] == 0) {
        slot = 0;
    } else {
        cursor = me + 0xA8;
        if (me[0x100] != 3 && cursor[0x5E] == 0) {
            slot = 1;
        } else {
            me = cursor + 0xA8;
            if (cursor[0x100] != 3 && me[0x5E] == 0) {
                slot = 2;
            } else {
                cursor = me + 0xA8;
                if (me[0x100] != 3 && cursor[0x5E] == 0) {
                    slot = 3;
                } else {
                    slot = -1;
                }
            }
        }
    }

    bracket_idx = fn_8018F74C();

    for (i = 0; i < 20; i++) {
        lbl_804799D8.x4E[i] = 0;
    }

    if (mode == 1) {
        BracketEntry* bracket = &lbl_80473AB8[bracket_idx];
        u8* bp = (u8*) bracket;
        u8* matched = bp + slot * 0x2C;

        for (i = 0; i < 4; i++) {
            if (i == slot) {
                matched[0x4C] = 0;
            } else {
                bp[0x4C] = 3;
            }
            bp += 0x2C;
        }
    } else if (td1->x2D == 1) {
        BracketEntry* bracket = &lbl_80473AB8[bracket_idx];
        cursor = (u8*) bracket;

        if (bracket->x4E == 3) {
            bracket->x4C = 3;
        } else {
            me = lbl_804799D8.x48;
            {
                u8 v = me[0x5E];
                me[0x5D] = v;
                bracket->x4C = v;
            }
            me = lbl_804799D8.x48;
            if (me[0x5E] == 0) {
                slot = 0;
            }
        }

        {
            u8 check = bracket->x7A;
            cursor += 0x2C;
            if (check == 3) {
                cursor[0x4C] = 3;
            } else {
                me = lbl_804799D8.x48;
                me += 0xA8;
                {
                    u8 v = me[0x5E];
                    me[0x5D] = v;
                    cursor[0x4C] = v;
                }
                me = lbl_804799D8.x48;
                if (me[0x106] == 0) {
                    slot = 1;
                }
            }
        }

        {
            u8 check = cursor[0x7A];
            cursor += 0x2C;
            if (check == 3) {
                cursor[0x4C] = 3;
            } else {
                me = lbl_804799D8.x48;
                me += 0x150;
                {
                    u8 v = me[0x5E];
                    me[0x5D] = v;
                    cursor[0x4C] = v;
                }
                me = lbl_804799D8.x48;
                if (me[0x1AE] == 0) {
                    slot = 2;
                }
            }
        }

        {
            u8 check = cursor[0x7A];
            cursor += 0x2C;
            if (check == 3) {
                cursor[0x4C] = 3;
            } else {
                me = lbl_804799D8.x48;
                me += 0x1F8;
                {
                    u8 v = me[0x5E];
                    me[0x5D] = v;
                    cursor[0x4C] = v;
                }
                me = lbl_804799D8.x48;
                if (me[0x256] == 0) {
                    slot = 3;
                }
            }
        }
    } else {
        BracketEntry* bracket = &lbl_80473AB8[bracket_idx];
        counter = 0;

        if (bracket->x4E == 3) {
            bracket->x4C = 4;
        } else {
            bracket->x4C = 0;
            counter = 1;
        }

        cursor = (u8*) bracket + 0x2C;
        if (bracket->x7A == 3) {
            cursor[0x4C] = 4;
        } else {
            cursor[0x4C] = counter;
            counter++;
        }

        {
            u8 check = cursor[0x7A];
            cursor += 0x2C;
            if (check == 3) {
                cursor[0x4C] = 4;
            } else {
                cursor[0x4C] = counter;
                counter++;
            }
        }

        {
            u8 check = cursor[0x7A];
            cursor += 0x2C;
            if (check == 3) {
                cursor[0x4C] = 4;
            } else {
                cursor[0x4C] = counter;
                counter++;
            }
        }

        switch (counter) {
        case 3:
            fn_80196684(bracket_idx);
            break;
        case 2: {
            s32 rand_val = HSD_Randi(bracket->x51 + bracket->x7D);
            if (rand_val < bracket->x51) {
                bracket->x4C = 0;
                bracket->x78 = 1;
            } else {
                bracket->x4C = 1;
                bracket->x78 = 0;
            }
            break;
        }
        case 4:
            fn_801967E0(bracket_idx);
            break;
        }

        me = lbl_804799D8.x48;
        cursor = (u8*) &lbl_80473AB8[bracket_idx];
        {
            u8 v = cursor[0x4C];
            me[0x5D] = v;
            me = lbl_804799D8.x48;
            me[0x5E] = v;
            v = cursor[0x78];
            me[0x105] = v;
            me = lbl_804799D8.x48;
            me[0x106] = v;
            v = cursor[0xA4];
            me[0x1AD] = v;
            me = lbl_804799D8.x48;
            me[0x1AE] = v;
            v = cursor[0xD0];
            me[0x255] = v;
            me = lbl_804799D8.x48;
            me[0x256] = v;
        }

        if (bracket->x4C == 0) {
            slot = 0;
        }
        if (bracket->x78 == 0) {
            slot = 1;
        }
        cursor = (u8*) bracket + 0x2C;
        if (cursor[0x78] == 0) {
            cursor += 0x2C;
            slot = 2;
        } else {
            cursor += 0x2C;
        }
        if (cursor[0x78] == 0) {
            slot = 3;
        }
    }

    {
        BracketEntry* bracket = &lbl_80473AB8[bracket_idx];
        lbl_804799D8.x4C = slot;
        cursor = (u8*) bracket + slot * 0x2C;
        lbl_804799D8.x4D = cursor[0x4E];

        if (lbl_804799D8.x4D == 0 && bracket->x18 != 0) {
            u8 s = lbl_804799D8.x4C;
            u16 val = td1->x4B8[s].x6;
            if (val <= 0x78) {
                gm_80167858(s, (s32) val, 0x1F, 0x78);
            } else {
                gm_80167858(s, 0x78, 0x1F, 0x78);
            }
        }

        cursor = (u8*) &lbl_80473AB8[bracket_idx] + slot * 0x2C;
        {
            u8 model_idx = cursor[0x50];
            fn_8018F00C((char*) lbl_804799D8.x4E, td1->x37[model_idx].x9);
        }
    }
}

/// #fn_8019A158_end

/// @todo Currently 98.36% match - needs branch pattern fix (beq+b vs bne)
#pragma dont_inline on
void fn_8019A71C(s32* state, u32 unused1, u32 unused2)
{
    u32* counter = &lbl_804799D8.x0;

    switch (*state) {
    case 0x13:
        fn_8019B458(state);
        break;
    }

    if (*state > 0x14) {
        *counter = *counter + 1;
    }

    if (*counter >= 0x14U) {
        *state = 0x15;
    }

    if (*counter == 0x1EU) {
        *state = 0x16;
    }

    if (*counter > 0x1EU) {
        *state = 0x17;
    }

    if (*counter >= 0x3CU) {
        *state = 0x18;
    }

    if (*counter >= 0x46U) {
        *state = 0x19;
    }

    if (*counter >= 0x5AU) {
        fn_8018EC48();
        *state = 0x1A;
        fn_8018EC7C();
        if (fn_8018F508(0) == 1) {
            *state = 0x1F;
            return;
        }
        *state = 0x1B;
    }
}
#pragma dont_inline reset

extern s32 lbl_804D6678;

void gm_8019A828(void)
{
    gm_8018F634()->x32 = 0;
    lbl_804D6678 = 1;
    gm_8018F634()->cur_option = 0x1B;
}

void fn_8019A86C(s32* arg0, u32 arg1, u32 arg2)
{
    TmData* tm = (TmData*) arg0;
    s32 var_r26 = 0;
    s32 var_r28 = 0;
    struct Lbl804799D8_t* d8 = &lbl_804799D8;
    s32 i;
    PAD_STACK(0x28);

    if (tm->cur_option == 0x1B) {
        fn_8019B81C(arg0);
    }

    if (tm->cur_option == 0x1D) {
        d8->x0 += 1;
        if ((arg2 & 0x600) || (d8->x0 >= 0x12CU)) {
            lbAudioAx_80024030(0);
            fn_8018EC48();
            tm->x2D = 0;
            tm->cur_option = 0x1F;
        }
        if (arg2 & 0x1100) {
            TmData* t;
            s32 stype;
            s32 cond;

            lbAudioAx_80024030(1);
            t = gm_8018F634();
            t->x2D = 1;
            stype = t->stage_selection_type;
            if ((stype == 2 && (u8) t->x32 == 0) || stype == 3) {
                cond = 1;
            } else {
                cond = 0;
            }
            if (cond != 0) {
                t->x32 = 1;
                gm_SetPendingScene(3U);
                gm_801A4B60();
                return;
            }
            if ((stype == 0 && (u8) t->x32 == 0) || stype == 1) {
                cond = 1;
            } else {
                cond = 0;
            }
            if (cond != 0) {
                t->x32 = 1;
                t->x28 = (u32) lbl_804D4190;
            }
            gm_SetPendingScene(4U);
            gm_801A4B60();
        }
    } else {
        u8 n_players = tm->x30;
        u8 pnum = 0;

        for (i = 0; i < (s32) n_players; i++) {
            if (tm->x4B8[i].x0 == 1) {
                var_r26 += 1;
            } else {
                u8 err = (u8) HSD_PadMasterStatus[pnum].err;
                if ((s8) err != 0) {
                    var_r28 = 1;
                }
                {
                    u8 state = d8->x2A[i].state;
                    if (((state == 2 && (u8) d8->x2A[i].cur >= 0x3CU) ||
                         (state == 4 && (u8) d8->x2A[i].cur == 0x82)) &&
                        (s8) err == 0)
                    {
                        var_r26 += 1;
                    }
                }
            }
            pnum += 1;
        }

        {
            s32 cond3;
            if ((fn_8018F640(4) & 0x1160) &&
                ((u32) (fn_8018F6A8(4) & 0x1160) == 0x1160))
            {
                cond3 = 1;
            } else {
                cond3 = 0;
            }
            if (cond3 != 0 && var_r28 != 0) {
                lbAudioAx_80024030(0);
                HSD_SisLib_803A5E70();
                mn_8022F138(0x19, 0x1C);
                mn_8022F138(0x12, 0x15);
                mn_8022F268();
                gm_801A4B60();
                gm_801A42F8(1);
                return;
            }
        }

        if (var_r26 == (s32) tm->x30) {
            d8->x0 += 1;
            if (d8->x0 >= 0x1EU) {
                for (i = 0; i < (s32) tm->x30; i++) {
                    if (tm->x4B8[i].x0 == 0 && d8->x2A[i].state == 4) {
                        tm->x4B8[i].x0 = 3;
                    }
                }

                {
                    TmData* t2 = gm_8018F634();
                    BracketEntry* ent = &lbl_80473AB8[fn_8018F74C()];
                    s32 hmn_count = 0;
                    s32 active_count = 0;

                    ent->x4E = t2->x4B8[0].x0;
                    ent->x7A = t2->x4B8[1].x0;
                    ent->xA6 = t2->x4B8[2].x0;
                    ent->xD2 = t2->x4B8[3].x0;

                    {
                        u8 s0 = tm->x4B8[0].x0;
                        if (s0 == 0) {
                            hmn_count += 1;
                        }
                        if (s0 != 3) {
                            active_count += 1;
                        }
                    }
                    {
                        u8 s1 = tm->x4B8[1].x0;
                        if (s1 == 0) {
                            hmn_count += 1;
                        }
                        if (s1 != 3) {
                            active_count += 1;
                        }
                    }
                    {
                        u8 s2 = tm->x4B8[2].x0;
                        if (s2 == 0) {
                            hmn_count += 1;
                        }
                        if (s2 != 3) {
                            active_count += 1;
                        }
                    }
                    {
                        u8 s3 = tm->x4B8[3].x0;
                        if (s3 == 0) {
                            hmn_count += 1;
                        }
                        if (s3 != 3) {
                            active_count += 1;
                        }
                    }

                    if (active_count <= 1) {
                        fn_8018EC48();
                        tm->x2D = 0;
                        tm->cur_option = 0x1F;
                        return;
                    }
                    if (hmn_count == 0) {
                        tm->cur_option = 0x1D;
                        return;
                    }

                    {
                        TmData* t3 = gm_8018F634();
                        s32 stype2 = t3->stage_selection_type;
                        s32 cond2;

                        t3->x2D = 1;
                        if ((stype2 == 2 && (u8) t3->x32 == 0) || stype2 == 3)
                        {
                            cond2 = 1;
                        } else {
                            cond2 = 0;
                        }
                        if (cond2 != 0) {
                            t3->x32 = 1;
                            gm_SetPendingScene(3U);
                            gm_801A4B60();
                            return;
                        }
                        if ((stype2 == 0 && (u8) t3->x32 == 0) || stype2 == 1)
                        {
                            cond2 = 1;
                        } else {
                            cond2 = 0;
                        }
                        if (cond2 != 0) {
                            t3->x32 = 1;
                            t3->x28 = (u32) lbl_804D4190;
                        }
                        gm_SetPendingScene(4U);
                        gm_801A4B60();
                    }
                }
            }
        } else {
            d8->x0 = 0;

            for (i = 0; i < (s32) tm->x30; i++) {
                if ((s8) (u8) HSD_PadMasterStatus[(u8) i].err == 0 &&
                    tm->x4B8[i].x0 == 0)
                {
                    u32 buttons = fn_8018F640(i);

                    if (buttons & 0x1100) {
                        u8 astate = d8->x44[i];

                        lbAudioAx_80024030(1);
                        if (astate == 7) {
                            d8->x44[i] = 6;
                        } else if (astate == 8) {
                            u8 np = gm_8018F634()->x30;
                            s32 count4 = 0;
                            s32 j;

                            for (j = 0; j < (s32) np; j++) {
                                if (d8->x2A[j].state == 4) {
                                    count4 += 1;
                                }
                            }
                            if (count4 < (s32) (tm->x30 - 1)) {
                                d8->x44[i] = 6;
                                d8->x2A[i].state = 4;
                            }
                        } else {
                            u8 pstate = d8->x2A[i].state;
                            if (pstate == 4) {
                                d8->x2A[i].state = 5;
                            } else if (pstate == 0 || pstate == 3 ||
                                       pstate == 5)
                            {
                                u16 val = tm->x4B8[i].x6;
                                if (val <= 0x78U) {
                                    gm_80167858(i, (s32) val, 0xB, 0x14);
                                } else {
                                    gm_80167858(i, 0x78, 0xB, 0x14);
                                }
                                d8->x2A[i].state = 1;
                            }
                        }
                    } else if (buttons & 0x400) {
                        if (d8->x44[i] != 6) {
                            lbAudioAx_80024030(0);
                            d8->x44[i] = 6;
                        } else {
                            u8 pstate2 = d8->x2A[i].state;
                            if (pstate2 == 0 || pstate2 == 3 || pstate2 == 5) {
                                u8 np2 = gm_8018F634()->x30;
                                s32 count5 = 0;
                                s32 k;

                                for (k = 0; k < (s32) np2; k++) {
                                    if (d8->x2A[k].state == 4) {
                                        count5 += 1;
                                    }
                                }
                                if (count5 < (s32) (tm->x30 - 1)) {
                                    lbAudioAx_80024030(0);
                                    d8->x44[i] = 7;
                                }
                            } else if (pstate2 == 2) {
                                lbAudioAx_80024030(0);
                                d8->x2A[i].state = 3;
                                d8->x2A[i].done = 0;
                            }
                        }
                    } else if ((buttons & 0x10000) || (buttons & 8)) {
                        if (d8->x44[i] == 8) {
                            lbAudioAx_80024030(2);
                            d8->x44[i] = 7;
                        }
                    } else if (((buttons & 0x20000) || (buttons & 4)) &&
                               d8->x44[i] == 7)
                    {
                        lbAudioAx_80024030(2);
                        d8->x44[i] = 8;
                    }
                }
            }
        }
    }
}

extern u8 lbl_804D6680[8];
extern u8 lbl_803B7D04[20];

/// Tournament match timer display/audio state machine.
/// Handles match countdown, audio transitions, and end conditions.
void fn_8019AF50(s32* arg0, u32 arg1, u32 arg2)
{
    typedef struct {
        s32 d[5];
    } TimerFmt;
    TimerFmt sp_buf;
    TmData* tm = (TmData*) arg0;
    u32* counter = &lbl_804799D8.x0;
    s32 bracketIdx;
    u32 buttons;

    sp_buf = *(TimerFmt*) lbl_803B7D04;
    bracketIdx = fn_8018F74C();

    if (*arg0 == 0x1F) {
        fn_8019B860(tm);
    }

    if (tm->x33 == 6) {
        if (lbl_804D6680[0] == 2) {
            if (lbAudioAx_80023730() == 0) {
                lbAudioAx_80023F28(
                    lbAudioAx_8002305C(fn_8018F6FC(lbl_804D6680[1]), 0));
                lbl_804D6680[0] = 3;
            }
        } else if (lbl_804D6680[0] == 0) {
            u8* bp = (u8*) &lbl_80473AB8[bracketIdx];
            s32 j;

            for (j = 0; j < 4; j++) {
                if (bp[0x30] != 0 && bp[0x4C] == 0) {
                    u8* entry = (u8*) &lbl_80473AB8[bracketIdx];
                    lbl_804D6680[1] = entry[j * 0x2C + 0x4D];
                    break;
                }
                bp += 0x2C;
            }

            lbAudioAx_80023F28(fn_80160400(fn_8018F6FC(lbl_804D6680[1])));
            lbl_804D6680[0] = 1;
        } else if (lbl_804D6680[0] == 1) {
            u8* timer_ptr = &lbl_804D6680[2];
            if (*timer_ptr > 0x3C) {
                lbl_804D6680[0] = 2;
            }
            (*timer_ptr)++;
        }
    }

    if (lbl_804799D8.x4D != 1) {
        buttons = (u32) gm_801A36A0(lbl_804799D8.x4C);
    } else {
        buttons = (u32) gm_801A36A0(4);
    }

    if (lbl_80473AB8[bracketIdx].x18 != 0) {
        if (*counter < 0xFAU) {
            (*counter)++;
            if (*counter >= 0x64U) {
                s32 count = (u32) (*counter - 0x64) / 15;
                u8* base = (u8*) counter;
                u8* dest = (u8*) &sp_buf;
                s32 i;
                for (i = 0; i < count; i++) {
                    dest[0] = base[0x4E];
                    dest[1] = base[0x4F];
                    base += 2;
                    dest += 2;
                }
            }
            HSD_SisLib_803A70A0(tm->x524[3], 0, (char*) &sp_buf);
        } else {
            *counter += 2;
            if (lbl_804799D8.x4D != 1) {
                if (*counter > 0xFAU) {
                    *counter = 0xFA;
                }
            }
            HSD_SisLib_803A70A0(tm->x524[3], 0, (char*) counter + 0x4E);
        }
    } else {
        if (*counter < 0xFAU) {
            *counter = 0xFA;
        }
    }

    if (*arg0 == 0x27) {
        if (*counter >= 0xFAU) {
            if (tm->x33 == 6) {
                if (*counter >= 0x1C20U || (buttons & 0x1100)) {
                    gm_801A42F8(1);
                    gm_801A4B60();
                }
            } else {
                fn_8018EC48();
                *arg0 = 0x13;
            }
        }
    }
}

/// @todo Currently 89.97% match - permuter couldn't improve
/// Per-frame update for tournament mode menu.
void gm_8019B2DC_OnFrame(void)
{
    s32 sp[13];
    u32 arg1;
    TmData* data;
    u32 arg2;
    s32 cond;
    s32 val;
    s32 i;
    s32* dest;
    TmData* ptr;
    s32 stage;
    PAD_STACK(4);

    data = gm_8018F634();
    arg1 = fn_8018F674(4);
    arg2 = fn_8018F640(4);

    if (gm_8018F634()->cur_option >= 0x13 && gm_8018F634()->cur_option <= 0x1A)
    {
        cond = 1;
    } else {
        cond = 0;
    }
    if (cond != 0) {
        fn_8019A71C((s32*) data, arg1, arg2);
    } else {
        if (gm_8018F634()->cur_option >= 0x1B &&
            gm_8018F634()->cur_option <= 0x1E)
        {
            cond = 1;
        } else {
            cond = 0;
        }
        if (cond != 0) {
            fn_8019A86C((s32*) data, arg1, arg2);
        } else {
            fn_8019AF50((s32*) data, arg1, arg2);
        }
    }

    val = data->stage_selection_type;
    if ((val == 0 && data->x32 == 0) || val == 1) {
        cond = 1;
    } else {
        cond = 0;
    }
    if (cond != 0) {
        stage = lbl_804D4190;
    } else {
        stage = data->x28;
    }
    sp[0] = stage;

    ptr = data;
    dest = sp;
    i = 0;
    do {
        dest[1] = ptr->x4B8[0].x0;
        dest[5] = fn_8018F6FC(ptr->x4B8[0].x1);
        i += 1;
        dest[9] = ptr->x4B8[0].x3;
        ptr = (TmData*) ((u8*) ptr + 0xA);
        dest += 1;
    } while (i < 4);

    fn_8019BA04(sp);
}

extern SceneDesc* lbl_804D6670;

/// Transitions to results screen after a tournament match.
/// Ranks players, preloads stage/character data, and starts audio.
void fn_8019B458(s32* arg0)
{
    TmData* tm = (TmData*) arg0;
    s32 i;
    u8 rank = 0;
    s32 x24;
    u8* ptr;
    TmData* td;

    s32 costumes[4];
    s32 charIDs[4];
    PAD_STACK(0x10);

    tm->x24++;
    lbl_804799D8.x0 = 0;
    tm->pad_x34[0] = tm->x33;

    td = gm_8018F634();
    x24 = td->x24;
    ptr = lbl_803DA0D0.rank_thresholds[td->entrants];

    if (x24 > (s32) ptr[5]) {
        rank = 6;
    } else {
        if (x24 <= (s32) ptr[5]) {
            rank = 5;
        }
        if (x24 <= (s32) ptr[4]) {
            rank = 4;
        }
        if (x24 <= (s32) ptr[3]) {
            rank = 3;
        }
        if (x24 <= (s32) ptr[2]) {
            rank = 2;
        }
        if (x24 <= (s32) ptr[1]) {
            rank = 1;
        }
        if (x24 <= (s32) ptr[0]) {
            rank = 0;
        }
    }

    tm->x33 = rank;

    {
        s32 match = fn_80196CF8();
        TmData* td2 = gm_8018F634();
        fn_80198D18();

        {
            HSD_GObj* gobj = fn_8019035C(0, lbl_804D6670->models[3], match,
                                         0x1A, 3, 1, fn_80196EEC, lbl_804DA808);

            if ((s32) td2->pad_x34[0] == match) {
                HSD_JObjSetFlagsAll(gobj->hsd_obj, 0x10U);
            }
        }

        if (match < 4) {
            lbl_804799D8.x1B = 0x50;
        } else if (match == 4) {
            lbl_804799D8.x1B = 0x5F;
        } else {
            lbl_804799D8.x1B = 0x61;
        }

        fn_80198BA0();
        fn_8018E618(tm->entrants, lbl_804DA810, (s32) tm->x2C);
        fn_8018E85C(lbl_804D6670->models[4], tm->x2C);
        fn_8018FA24();

        tm->cur_option = 0x14;
        tm->x2C = 0;

        for (i = 0; i < 4; i++) {
            if (tm->x4B8[i].x0 != 3) {
                charIDs[i] = fn_8018F6FC(tm->x4B8[i].x1);
                costumes[i] = tm->x4B8[i].x3;
            }
        }

        {
            volatile InternalStageId stageID;
            s32 use_random_stage;

            if ((tm->stage_selection_type == 0 && tm->x32 == 0) ||
                tm->stage_selection_type == 1)
            {
                use_random_stage = 1;
            } else {
                use_random_stage = 0;
            }

            if (use_random_stage != 0) {
                gm_8018F634();
                do {
                    lbl_804D4190 = fn_8018F4A0();
                    if (lbl_804D4194 == lbl_804D4190) {
                        if (fn_801642A0() != 0) {
                            break;
                        }
                    } else {
                        break;
                    }
                } while (true);
                lbl_804D4194 = lbl_804D4190;
                stageID = lbl_804D4190;
            } else {
                stageID = tm->x28;
            }

            {
                PreloadCacheScene* scene = lbDvd_8001822C();
                struct GameCache* gc = &scene->game_cache;
                lbDvd_800174BC();

                if (tm->x4B8[0].x0 != 3) {
                    gc->entries[0].char_id = charIDs[0];
                    gc->entries[0].color = costumes[0];
                }

                if (tm->x4B8[1].x0 != 3) {
                    gc->entries[1].char_id = charIDs[1];
                    gc->entries[1].color = costumes[1];
                }

                if (tm->x4B8[2].x0 != 3) {
                    gc->entries[2].char_id = charIDs[2];
                    gc->entries[2].color = costumes[2];
                }

                if (tm->x4B8[3].x0 != 3) {
                    gc->entries[3].char_id = charIDs[3];
                    gc->entries[3].color = costumes[3];
                }

                {
                    s32 skip_stage_cache;
                    if ((tm->stage_selection_type == 2 && tm->x32 == 0) ||
                        tm->stage_selection_type == 3)
                    {
                        skip_stage_cache = 1;
                    } else {
                        skip_stage_cache = 0;
                    }
                    if (skip_stage_cache == 0) {
                        gc->stage_id = stageID;
                    }
                }

                lbDvd_80018254();
            }

            {
                u64 audio_mask = 0;
                for (i = 0; i < 4; i++) {
                    if (tm->x4B8[i].x0 != 3) {
                        audio_mask |= lbAudioAx_80026E84(charIDs[i]);
                    }
                }
                audio_mask |= lbAudioAx_80026EBC(stageID);
                lbAudioAx_80026F2C(0x1C);
                lbAudioAx_8002702C(0xC, audio_mask);
                lbAudioAx_80027168();
            }
        }
    }
}

/// Initializes match data and transitions to next state.
/// Type cast used to match target stw instruction pattern.
void fn_8019B81C(s32* state)
{
    fn_80198BA0();
    fn_80198EBC();
    *state = 0x1C;
    lbl_804799D8.x0 = 0;
}

extern SceneDesc* lbl_804D6670;

void fn_8019B860(TmData* tm)
{
    fn_8019A158();
    fn_80199AF0();
    fn_80198BA0();
    fn_8018F888();
    fn_8018E618(tm->entrants, lbl_804DA810, tm->x2C);
    fn_8018E85C(lbl_804D6670->models[4], tm->x2C);
    tm->cur_option = 0x20;
}

/// .sdata2 reconstruction: Fix-B positioned defs (idioms 24/87/99) for the
/// named float constants this TU references through extern decls. Defined
/// at EOF, after every use, so all reference sites keep their named relocs
/// (a visible initializer would fold them to anonymous pool literals);
/// EOF defs emit after all pool creations (idiom 99: pool-offset-free).
/// Values are bit-exact recoveries from the target binary.
const f32 lbl_804DA7E0 = -19.5F;
const f32 lbl_804DA7E4 = 13.0F;
const f32 lbl_804DA7E8 = 6.5F;
const f32 lbl_804DA7EC = 19.5F;
const f32 lbl_804DA7F0 = 2.0F;
const f32 lbl_804DA808 = 0.0F;
const f32 lbl_804DA80C = 1.0F;
const f32 lbl_804DA810 = 4.5F;
const f32 lbl_804DA814 = 5.5F;
const f32 lbl_804DA818 = 666.0F;
const f32 lbl_804DA81C = 0.3F;
const f32 lbl_804DA820 = 12.6F;
const f32 lbl_804DA824 = 0.01F;
const f32 lbl_804DA82C = 0.054945F;
const f32 lbl_804DA830 = 0.08F;
const f32 lbl_804DA834 = 320.0F;
const f32 lbl_804DA838 = 250.0F;
const f32 lbl_804DA83C = 1.5F;
const f32 lbl_804DA840 = 80.0F;
const f32 lbl_804DA844 = 4.0F;
const f32 lbl_804DA848 = 73.0F;
const f32 lbl_804DA84C = 152.0F;
const f32 lbl_804DA850 = 240.0F;
const f32 lbl_804DA854 = 270.0F;
const f32 lbl_804DA858 = 0.99F;
const f32 lbl_804DA85C = 261.0F;
const f32 lbl_804DA860 = -42.0F;
const f32 lbl_804DA864 = 2.799999F;
const f32 lbl_804DA868 = 6.8F;
const f32 lbl_804DA86C = 0.001F;
const f32 lbl_804DA870 = -2.5F;
const f32 lbl_804DA874 = 0.02F;

/// Dead .data strings recovered from the target binary (idiom 51:
/// unreferenced non-const statics with initializers survive and emit at
/// decl point; EOF position keeps them after the "translate" pool string
/// at .data+0x184).
static char lbl_803DA260[] = "ScGamTour_scene_data";
static char lbl_803DA278[] = "TmBox.dat";
static char lbl_803DA284[] = "tournament_box2_array";
static char lbl_803DA29C[] = "tournament_box3_array";
static char lbl_803DA2B4[] = "tournament_box4_array";
static char lbl_803DA2CC[] = "SIS_TournamentData";
