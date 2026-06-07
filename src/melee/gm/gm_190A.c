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

/// @file gm_190A.c -- original translation-unit boundary restored from
/// gm_18A5.c (TU split; .text 0x80190ABC..0x80196510).

// Keep in sync with the lbl_803D9D20_t definition in gm_18A5.static.h
// (gm_18A5.c defines + initializes the object; this TU reads it extern).
struct lbl_803D9D20_t {
    /*  +0 */ u8 x0[0x59];
    /* +59 */ u8 x59[0x72 - 0x59];
    /* +72 */ u8 x72[0x8C - 0x72];
};
extern struct lbl_803D9D20_t lbl_803D9D20;

// gm_804771C4 lives in gm_18A5.c's .bss block (the binary anchors it at
// lbl_80473AB8+0x370C); this TU reads it extern (consumer-style named
// refs, per the DOL shapes).
extern TmData gm_804771C4;

// This TU's only .bss object (DOL: 0x804799B8, the unit's .bss base).

struct Lbl804799B8_t {
    u8 x0;
    u8 x1;
    u8 x2;
    u8 x3;
    u8 x4;
    u8 x5;
    u8 x6;
    u8 x7;
    u8 x8;
    u8 x9;
    u8 xA;
    u8 xB;
    u16 xC_counter;
    u8 xE;
    u8 xF;
    u8 pad2[0x10];
};
static struct Lbl804799B8_t lbl_804799B8;

HSD_Archive* lbl_804D6638;
HSD_Archive* lbl_804D6640;
HSD_Archive* lbl_804D6644;
HSD_Archive* lbl_804D6648;
SceneDesc* lbl_804D664C;
SceneDesc* lbl_804D6650;
HSD_GObj* lbl_804D663C;

// This TU's .data base (DOL 0x803D9F80, 0x58 bytes, no relocs;
// initializer read back from the target object). Head 0x40 bytes are
// u16 pairs (read via the *(u16*)/(s16*) casts below); tail 0x18 bytes
// are byte-indexed (lbl_803D9F80[0x40 + i * 2]).
u8 lbl_803D9F80[0x58] = {
    0x00, 0x4A, 0x00, 0x4A, 0x00, 0x4D, 0x00, 0x4B, 0x00, 0x4B, 0x00,
    0x4D, 0x00, 0x50, 0x00, 0x4E, 0x00, 0x4F, 0x00, 0x4F, 0x00, 0x51,
    0x00, 0x00, 0x00, 0x52, 0x00, 0x52, 0x00, 0x5C, 0x00, 0x5C, 0x00,
    0x60, 0x00, 0x5D, 0x00, 0x5D, 0x00, 0x60, 0x00, 0x5F, 0x00, 0x61,
    0x00, 0x62, 0x00, 0x62, 0x00, 0x64, 0x00, 0x00, 0x00, 0x53, 0x00,
    0x6F, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x58, 0x00, 0x65, 0x00, 0x00,
    0x02, 0x03, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x02, 0x04, 0x10, 0x1F, 0x03, 0x3F, 0x03, 0x03, 0x03, 0x09, 0x00,
};

typedef void (*lbl_803D9FD8_fn)(s32*, u32, u32);

lbl_803D9FD8_fn lbl_803D9FD8[] = {
    fn_801937C4, fn_80193B58, fn_80193FCC, fn_80194658, fn_801949B4,
    fn_80194BC4, fn_80194D84, fn_80194D84, fn_80194D84, NULL,
    fn_80194F30, fn_801953C8, fn_80194F30, fn_80195AF0, fn_80195AF0,
    fn_80195CCC, fn_80194F30, fn_8019610C, fn_8019610C,
};

extern const f32 lbl_804DA6FC; // 143.0f
extern const f32 lbl_804DA700; // 183.0f
extern const f32 lbl_804DA704; // 48.6f
extern const f32 lbl_804DA708; // 514.0f
extern const f32 lbl_804DA70C; // 87.0f

void fn_80190ABC(int mode)
{
    u8* table = lbl_803D9F80;
    TmData* tm;
    s32 cur_opt;
    s32 opt;
    s32 i;

    tm = gm_8018F634();
    cur_opt = tm->cur_option;
    opt = MIN(cur_opt, 6);

    if (opt == 6 && mode != 0 && cur_opt < 9) {
        return;
    }

    switch (mode) {
    case 0: {
        s32 flag = !!gm_804771C4.match_type;
        HSD_SisLib_803A6368(tm->x4E0, *(u16*) (table + opt * 4 + flag * 2));
        break;
    }
    case 1: {
        s32 flag = !!gm_804771C4.match_type;
        HSD_SisLib_803A6368(tm->x4E8[opt],
                            *(u16*) (table + opt * 4 + flag * 2 + 0x1C));
        break;
    }
    case 2: {
        s32 display_val;
        if (opt == 2 && gm_804771C4.match_type == 0) {
            s32 val = (&tm->match_type)[opt];
            switch (val) {
            case 16:
            case 17:
                display_val = val + 0xE1;
                break;
            case 21:
            case 22:
                display_val = val + 0xDE;
                break;
            case 30:
            case 31:
                display_val = val + 0xD7;
                break;
            default:
                display_val =
                    lbl_803D9D20.x0[val] + *(u16*) (table + opt * 2 + 0x34);
                break;
            }
        } else {
            display_val =
                *(u16*) (table + opt * 2 + 0x34) + (&tm->match_type)[opt];
        }
        HSD_SisLib_803A6368(tm->x500[opt], display_val);
        break;
    }
    case 3: {
        s32 display_val =
            lbl_803D9D20.x0[tm->entrants] + 0xB0 - tm->hmn_cpu_count;
        HSD_SisLib_803A6368(tm->x4E4, display_val);
        break;
    }
    case 4: {
        HSD_SisLib_803A7664(tm->x518[0]);
        {
            f32 f_col_mul = 149.0f;
            f32 f_col_add = 537.0f;
            f32 f_row_mul = 47.0f;
            f32 f_row_add = 409.0f;
            for (i = 0; i < 0x24; i++) {
                s32 col = i % 4;
                s32 row = i / 4;
                f32 pos_x = f_col_mul * (f32) col + f_col_add;
                f32 pos_y = f_row_mul * (f32) row + f_row_add;
                fn_8018ECA8(i + lbl_804799B8.x6 * 4, 0xFF, 0, pos_x, pos_y, 0);
            }
        }
        break;
    }
    case 5: {
        HSD_SisLib_803A7664(tm->x518[1]);
        HSD_SisLib_803A7664(tm->x518[2]);
        {
            f32 f_mul = lbl_804DA704;
            f32 f_add = lbl_804DA700;
            for (i = 0; i < 12; i++) {
                s32 idx = lbl_804799B8.x3 + i;
                if (idx > lbl_804799B8.x4) {
                    continue;
                }
                {
                    f32 pos_y = f_mul * (f32) i + f_add;
                    fn_8018ECA8(tm->x37[idx].x9, tm->x37[idx].x0, 1,
                                lbl_804DA6FC, pos_y, 2);
                    idx = lbl_804799B8.x3 + i;
                    fn_8018ECA8(tm->x37[idx].x9, tm->x37[idx].x0, 1,
                                lbl_804DA6FC, pos_y, 2);
                }
            }
        }
        break;
    }
    case 6: {
        s32 idx;
        HSD_SisLib_803A7664(tm->x524[0]);
        HSD_SisLib_803A7664(tm->x524[1]);
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        fn_8018ECA8(tm->x37[idx].x9, tm->x37[idx].x0, 3, lbl_804DA708,
                    lbl_804DA70C, 4);
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        fn_8018ECA8(tm->x37[idx].x9, tm->x37[idx].x0, 3, lbl_804DA708,
                    lbl_804DA70C, 4);
        break;
    }
    }
}

// Positioned defs (idiom 87): these five named consts emit between
// fn_80190ABC's pool literals and gm_80190EA4's 0.0f in the original
// .sdata2 stream (DOL 0x804DA6FC..0x804DA70C).
const f32 lbl_804DA6FC = 143.0f;
const f32 lbl_804DA700 = 183.0f;
const f32 lbl_804DA704 = 48.6f;
const f32 lbl_804DA708 = 514.0f;
const f32 lbl_804DA70C = 87.0f;

#pragma push
#pragma dont_inline on
extern f32 lbl_804DA6D8; // 4.5f
extern f32 lbl_804DA6DC; // 130.0f
extern f32 lbl_804DA6E0; // -278.0f
extern f32 lbl_804DA6E4; // 255.0f

void gm_80190EA4(void)
{
    int i;
    TmData* tmdata = gm_8018F634();

    fn_80190174(lbl_804D664C->cameras->desc);
    fn_801902F0(0);
    fn_80193308();
    fn_8019027C(lbl_804D664C->lights);
    fn_80193230();
    tmdata->cur_option = 0;

    while (tmdata->cur_option < 7) {
        fn_80190ABC(0);
        fn_80190ABC(2);
        fn_80190ABC(3);
        fn_80190ABC(1);
        tmdata->cur_option += 1;
    }
    if (gm_804771C4.match_type == 0) {
        TmData* tmdata = gm_8018F634();
        fn_8018EC7C();
        fn_8018E618(tmdata->entrants, lbl_804DA6D8, 1);
        fn_80190480(lbl_804DA6DC);
        fn_80190520(lbl_804DA6E0, lbl_804DA6E4, 0.0f);
        if (0) {} // @-pin shim (+2): EA4 literal->extern swap, see gm18a5_w12
        if (0) {} // @-pin shim (+2)
    }
    tmdata->x20 = 0;

    for (i = 0; i < 64; i++) {
        if (gmMainLib_8015CC34()->handicap == 1) {
            tmdata->x37[i].x2 = 5;
        } else {
            tmdata->x37[i].x2 = 9;
        }
    }
}
#pragma pop

/// Initializes the time menu state when entering the time selection screen.
void gm_80190FE4(int arg0)
{
    TmData* tm = gm_8018F634();

    if (mn_8022F218() == 0) {
        fn_80190174(lbl_804D664C->cameras->desc);
        fn_801902F0(0);
        fn_80193308();
        fn_8019027C(lbl_804D664C->lights);
        fn_80193230();

        for (tm->cur_option = 0; tm->cur_option < 7; tm->cur_option++) {
            fn_80190ABC(0);
            fn_80190ABC(2);
            fn_80190ABC(3);
            fn_80190ABC(1);
        }

        tm->x20 = 0;
        tm->cur_option = 0xC;

        if (arg0 != 0x78) {
            tm->x37[lbl_804799B8.x2 + lbl_804799B8.x3].x9 = arg0;
        }

        fn_80190ABC(5);
        fn_80190ABC(6);
    }
}

void fn_801910E0(HSD_GObj* gobj)
{
    u16* counter = &lbl_804799B8.xC_counter;
    HSD_JObj* jobj = gobj->hsd_obj;

    if (*counter > 0x320U) {
        *counter = 0;
    }
    fn_8019044C(jobj, (f32) *counter);
    *counter = *counter + 1;
}

void fn_80191154(HSD_GObj* gobj)
{
    TmData* tm;
    HSD_JObj* jobj;
    u8* xE_ptr;

    tm = gm_8018F634();
    jobj = gobj->hsd_obj;

    if (tm->cur_option < 9) {
        xE_ptr = &lbl_804799B8.xE;
        if (*xE_ptr > 0x27U) {
            *xE_ptr = 0xA;
        }
    } else {
        xE_ptr = &lbl_804799B8.xE;
        if (lbl_804799B8.xE == 0x31) {
            fn_80190ABC(6);
            fn_80190ABC(5);
            tm->cur_option = 0xA;
        }
        if (*xE_ptr > 0x4EU) {
            *xE_ptr = 0x32;
        }
    }
    xE_ptr = &lbl_804799B8.xE;
    fn_8019044C(jobj, (float) lbl_804799B8.xE);
    (*xE_ptr)++;
}

/// @todo Currently 79.85% match - needs register allocation fix
void fn_80191240(HSD_GObj* gobj)
{
    TmData* tm;
    s32 val;
    HSD_JObj* jobj;
    HSD_JObj* first_child;
    HSD_JObj* sibling;

    tm = gm_8018F634();
    val = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;
    if (tm->cur_option >= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }
    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
    if (jobj == NULL) {
        first_child = NULL;
    } else {
        first_child = jobj->child;
    }
    if (first_child == NULL) {
        sibling = NULL;
    } else {
        sibling = first_child->next;
    }
    if (val == tm->cur_option) {
        if (lbl_804799B8.x7 != 0) {
            fn_8019044C(first_child, 1.0F);
            lbl_804799B8.x7 = lbl_804799B8.x7 - 1;
        } else {
            fn_8019044C(first_child, 0.0F);
        }
        if (lbl_804799B8.x8 != 0) {
            fn_8019044C(sibling, 1.0F);
            lbl_804799B8.x8 = lbl_804799B8.x8 - 1;
            return;
        }
        fn_8019044C(sibling, 0.0F);
        return;
    }
    if (val > tm->cur_option) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }
    if (val == 5 && gm_804771C4.match_type != 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    }
    fn_8019044C(jobj, 0.0F);
}

/// Updates text visibility and menu state for tournament bracket options.
void fn_801913BC(HSD_GObj* gobj)
{
    TmData* tm;
    u32 idx;
    HSD_JObj* jobj;
    s32 cur;
    u8* counter_ptr;
    PAD_STACK(0x60);

    tm = gm_8018F634();
    idx = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (tm->cur_option < 9) {
        HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

        if ((s32) idx == 0) {
            tm->x518[0]->hidden = 1;
            tm->x518[1]->hidden = 1;
            tm->x518[2]->hidden = 1;
            tm->x524[0]->hidden = 1;
            tm->x524[1]->hidden = 1;
            tm->x4E0->hidden = 0;
        }

        if ((s32) idx <= tm->cur_option) {
            tm->x4E8[idx]->hidden = 0;
            tm->x500[idx]->hidden = 0;
            if ((s32) idx < 3 && gm_804771C4.match_type == 0) {
                tm->x4E4->hidden = 1;
            } else {
                tm->x4E4->hidden = 0;
            }
            if ((s32) idx == 5 && gm_804771C4.match_type != 0) {
                tm->x4E8[idx]->hidden = 1;
                tm->x500[idx]->hidden = 1;
                HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
            }
        } else {
            tm->x4E8[idx]->hidden = 1;
            tm->x500[idx]->hidden = 1;
        }

        if (tm->cur_option >= 3 && gm_804771C4.match_type == 0) {
            tm->x4E4->hidden = 0;
        } else {
            tm->x4E4->hidden = 1;
        }

        cur = tm->cur_option;
        if ((s32) idx == cur) {
            counter_ptr = &(&lbl_804799B8.x0)[0x10 + idx];
            if (*counter_ptr >= 0x14U) {
                *counter_ptr = 0xA;
            }
            fn_8019044C(jobj, (f32) *counter_ptr);
            *counter_ptr = *counter_ptr + 1;
            return;
        }
        if ((s32) idx > cur) {
            HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
            return;
        }
        fn_8019044C(jobj, 0.0f);
        return;
    }

    HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    tm->x4E8[idx]->hidden = 1;
    tm->x500[idx]->hidden = 1;
    if ((s32) idx == 0) {
        if (tm->cur_option > 9) {
            tm->x518[1]->hidden = 0;
            tm->x518[2]->hidden = 0;
            tm->x524[0]->hidden = 0;
            tm->x524[1]->hidden = 0;
            if (tm->cur_option != 0xF) {
                tm->x518[0]->hidden = 1;
            } else {
                tm->x518[0]->hidden = 0;
            }
        }
        tm->x4E4->hidden = 1;
        tm->x4E0->hidden = 1;
    }
}

/// Updates the visibility and animation state of a Tournament Mode menu
/// option.
void fn_80191678(HSD_GObj* gobj)
{
    TmData* tmdata;
    s32 idx;
    HSD_JObj* jobj;
    u8* ptr;

    tmdata = gm_8018F634();
    idx = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (tmdata->cur_option >= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if (idx == tmdata->cur_option) {
        ptr = ((u8*) &lbl_804799B8) + idx;
        if (*(ptr += 0x16) > 0x64) {
            *ptr = 0;
        }
        fn_8019044C(jobj, (f32) *ptr);
        *ptr = *ptr + 1;
        return;
    }

    HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
}

/// @todo Currently 89.68% match - permuter couldn't improve
/// Updates menu option selection animation.
void fn_8019175C(HSD_GObj* gobj)
{
    HSD_JObj* jobjs[3];
    HSD_JObj* first_child;
    HSD_JObj* option_a;
    HSD_JObj* option_b;
    u8* counter;
    s32 i;
    TmData* tm;
    HSD_JObj* jobj;
    HSD_JObj* root_jobj;
    HSD_JObj* child;

    tm = gm_8018F634();
    fn_8018F62C(gobj);
    root_jobj = GET_JOBJ(gobj);
    jobj = root_jobj;

    if (tm->cur_option >= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if (root_jobj == NULL) {
        first_child = NULL;
    } else {
        first_child = root_jobj->child;
    }
    jobjs[2] = first_child;

    if (jobjs[2] == NULL) {
        child = NULL;
    } else {
        child = jobjs[2]->next;
    }

    if (child == NULL) {
        child = NULL;
    } else {
        child = child->next;
    }

    if (child == NULL) {
        option_a = NULL;
    } else {
        option_a = child->next;
    }
    jobjs[0] = option_a;

    if (option_a == NULL) {
        option_b = NULL;
    } else {
        option_b = jobjs[0]->next;
    }
    jobjs[1] = option_b;

    if (tm->cur_option >= 6) {
        counter = &lbl_804799B8.xF;
        if (*counter >= 0x14U) {
            *counter = 0;
        }
        i = 0;
        do {
            if (i != tm->cur_option - 6) {
                fn_8019044C(jobjs[i], 0.0F);
            } else {
                fn_8019044C(jobjs[i], (f32) *counter);
            }
            i += 1;
        } while (i <= 2);
        *counter = *counter + 1;
        return;
    }

    HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    goto pad_label; // @-pin shim (id ledger: base-local deletion paid back)
pad_label:;
}

/// Updates visibility and animation frame of a Training Mode HUD element.
void fn_801918F0(HSD_GObj* gobj)
{
    TmData* tm;
    HSD_JObj* jobj;
    u8* counter;

    tm = gm_8018F634();
    jobj = gobj->hsd_obj;

    if (tm->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    switch (tm->cur_option) {
    case 0xB:
        counter = &lbl_804799B8.pad2[0xC];
        if (*counter > 5) {
            *counter = 0;
        }
        fn_8019044C(jobj, (float) *counter);
        if (*counter < 5) {
            *counter = *counter + 1;
        }
        break;
    case 0xD:
    case 0xE:
        counter = &lbl_804799B8.pad2[0xC];
        if (*counter < 10) {
            *counter = 10;
        }
        fn_8019044C(jobj, (float) *counter);
        if (*counter < 15) {
            *counter = *counter + 1;
        }
        break;
    case 0xF:
        fn_8019044C(jobj, 16.0F);
        break;
    default:
        fn_8019044C(jobj, 0.0F);
        break;
    }
}

/// Updates the tournament bracket display visibility and animation.
void fn_80191A54(HSD_GObj* gobj)
{
    TmData* td;
    HSD_JObj* jobj;
    HSD_JObj* jobj_copy;
    HSD_JObj* child;
    int cur_option;

    td = gm_8018F634();
    jobj = gobj->hsd_obj;
    jobj_copy = jobj;

    if (td->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj_copy, 0x10);
        return;
    }

    HSD_JObjClearFlagsAll(jobj_copy, 0x10);

    if (jobj == NULL) {
        child = NULL;
    } else {
        child = jobj->child;
    }

    cur_option = td->cur_option;
    if (cur_option == 0xA || cur_option == 0xC || cur_option == 0x10) {
        fn_8019044C(child, (f32) lbl_804799B8.x2);
        HSD_MObjReqAnim(child->u.dobj->mobj, (f32) lbl_804799B8.xA);
    } else {
        HSD_MObjReqAnim(child->u.dobj->mobj, 6.0F);
    }
    HSD_MObjAnim(child->u.dobj->mobj);
}

/// Updates button highlight animation based on current menu option.
/// @note timers points into lbl_804799B8 (binary-proven: the DOL reads
///       lbl_804799B8+0xA; there is no separate 0x40-byte array).
void fn_80191B5C(void* gobj)
{
    u8* timers;
    TmData* tm;
    HSD_JObj* jobj;

    timers = &lbl_804799B8.x0;
    tm = gm_8018F634();
    jobj = (HSD_JObj*) ((HSD_GObj*) gobj)->hsd_obj;

    if (tm->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    } else {
        HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
        switch (tm->cur_option) {
        case 10:
            fn_8019044C(jobj, (f32) timers[0xA]);
            break;
        case 11:
            fn_8019044C(jobj, 5.0F);
            break;
        case 12:
            fn_8019044C(jobj, (f32) (s32) (timers[0xA] + 0x14));
            break;
        case 13:
        case 14:
        case 15:
            fn_8019044C(jobj, 25.0F);
            break;
        case 16:
            fn_8019044C(jobj, (f32) (s32) (timers[0xA] + 0x28));
            break;
        }
    }
}

s32 lbl_804D6658;

/// Updates JObj visibility based on current menu option selection.
void fn_80191CA4(HSD_GObj* gobj)
{
    TmData* tm;
    HSD_JObj* jobj;

    tm = gm_8018F634();
    jobj = gobj->hsd_obj;
    if (tm->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }
    if (lbl_804D6658 < 10) {
        lbl_804D6658 += 1;
    }
    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
    fn_8019044C(jobj, (f32) lbl_804D6658);
}

extern f32 lbl_804DA738;       // 12.800008f (DOL 414cccd5, NOT 12.8f)
extern const f32 lbl_804DA734; // 666.0f
extern const f32 lbl_804DA73C; // 2.6200008f (DOL 4027ae18, NOT 2.62f)
extern const f32 lbl_804DA740; // 0.1f
extern const f32 lbl_804DA744; // 201.0f

/// GObj callback for tournament bracket slot UI elements.
void fn_80191D38(HSD_GObj* gobj)
{
    TmData* tm;
    s32 idx;
    HSD_JObj* jobj;

    tm = gm_8018F634();
    idx = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (tm->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if ((s32) idx < (s32) lbl_804799B8.x3 || (s32) idx >= (s32) tm->x2E ||
        (s32) idx > (s32) (lbl_804799B8.x3 + 0xB))
    {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    fn_8018FDC4(
        jobj, lbl_804DA734,
        -((lbl_804DA73C * (f32) (idx - lbl_804799B8.x3)) - lbl_804DA738),
        lbl_804DA740);

    if ((u8) tm->x37[idx].x5 != 0) {
        fn_8019044C(jobj, lbl_804DA744);
    } else {
        fn_8019044C(jobj,
                    fn_8018F71C((s32) tm->x37[idx].x3, (s32) tm->x37[idx].x7));
    }

    if (lbl_803D9D20.x72[tm->x37[idx].x3] != 0) {
        HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
    } else {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
    }
}

/// Updates visibility and position of a tournament bracket player entry JObj.
void fn_80191E9C(HSD_GObj* gobj)
{
    TmData* tm;
    s32 idx;
    HSD_JObj* jobj;

    tm = gm_8018F634();
    idx = fn_8018F62C(gobj);
    jobj = gobj->hsd_obj;

    if (tm->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if (lbl_804799B8.x0 == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    if ((s32) idx < (s32) lbl_804799B8.x3 || (s32) idx >= (s32) tm->x2E ||
        (s32) idx > (s32) (lbl_804799B8.x3 + 0xB))
    {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    fn_8018FDC4(
        jobj, lbl_804DA734,
        -((lbl_804DA73C * (f32) (idx - lbl_804799B8.x3)) - lbl_804DA738),
        lbl_804DA740);
    fn_8019044C(jobj, (f32) tm->x37[idx].x2);
}

// Positioned defs (idiom 87): after the last use (f32 const defs that
// precede a use CONST-PROP into duplicate pool literals -- measured in
// this TU). Original slots 0x804DA73C/40 sit before fn_80191FD4's
// 200.0f/0.9f pool pair; ours land 8 bytes earlier (the 734/738 slots
// are use-locked, see below). 2.6200008f encodes DOL 4027ae18, NOT
// 2.62f.
const f32 lbl_804DA73C = 2.6200008f;
const f32 lbl_804DA740 = 0.1f;

void fn_80191FD4(HSD_GObj* gobj)
{
    TmData* tm;
    HSD_JObj* jobj;
    HSD_JObj* child;
    HSD_JObj* sibling;
    u32 idx;
    s32 slot;
    enum CSSIconHud hud;

    tm = gm_8018F634();
    jobj = gobj->hsd_obj;
    idx = fn_8018F62C(gobj);

    if ((s32) tm->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, 0x10U);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, 0x10U);

    if ((s32) idx == 0x19) {
        if (jobj == NULL) {
            child = NULL;
        } else {
            child = jobj->child;
        }
        fn_8019044C(child, 0.0f);

        if (child == NULL) {
            sibling = NULL;
        } else {
            sibling = child->next;
        }
        slot = lbl_804799B8.x2 + lbl_804799B8.x3;

        if ((u8) tm->x37[slot].x4 != 0) {
            fn_8019044C(sibling, lbl_804DA744);
            return;
        }
        goto pad_label; // @-pin shim (+1): 201.0f -> lbl_804DA744 swap
pad_label:;

        fn_8019044C(sibling, fn_8018F71C((s32) tm->x37[slot].x2,
                                         (s32) tm->x37[slot].x6));
        {
            u8 chr = tm->x37[lbl_804799B8.x2 + lbl_804799B8.x3].x2;
            if (lbl_803D9D20.x72[chr] != 0) {
                HSD_JObjClearFlagsAll(sibling, 0x10U);
                return;
            }
            if (fn_8018F3D0(fn_8018F310(fn_8018F6FC((enum CSSIconHud) chr))) ==
                1)
            {
                fn_8019044C(sibling, 200.0f);
                return;
            }
            HSD_JObjSetFlagsAll(sibling, 0x10U);
            return;
        }
    }

    if ((s32) tm->cur_option != 0xB) {
        HSD_JObjSetFlagsAll(jobj, 0x10U);
        return;
    }

    fn_8018FF9C(jobj, 0.9f, 0.9f, lbl_804DA734);
    if (jobj == NULL) {
        child = NULL;
    } else {
        child = jobj->child;
    }

    hud = fn_8018F6DC(fn_8018F3BC((s32) idx));
    slot = lbl_804799B8.x2 + lbl_804799B8.x3;
    if ((s32) tm->x37[slot].x2 == hud) {
        fn_8019044C(child, (f32) (lbl_804799B8.xA + 0xA));
    } else {
        hud = fn_8018F6DC(fn_8018F3BC((s32) idx));
        {
            s32 flag;
            if ((lbl_803D9D20.x72[hud] != 0) ||
                (fn_8018F3D0(fn_8018F310(fn_8018F6FC(hud))) != 2))
            {
                flag = 1;
            } else {
                flag = 0;
            }
            if (flag == 0) {
                HSD_JObjSetFlagsAll(child, 0x10U);
            } else {
                fn_8019044C(child, 0.0f);
            }
        }
    }

    if (child == NULL) {
        sibling = NULL;
    } else {
        sibling = child->next;
    }
    HSD_JObjClearFlagsAll(sibling, 0x10U);

    hud = fn_8018F6DC(fn_8018F3BC((s32) idx));
    if (lbl_803D9D20.x72[hud] != 0) {
        slot = lbl_804799B8.x2 + lbl_804799B8.x3;
        if ((s32) tm->x37[slot].x2 == fn_8018F6DC(fn_8018F3BC((s32) idx))) {
            fn_8019044C(sibling, (f32) ((tm->x37[slot].x6 * 0x1E) +
                                        fn_8018F6DC(fn_8018F3BC((s32) idx))));
            return;
        }
        fn_8019044C(sibling, (f32) fn_8018F6DC(fn_8018F3BC((s32) idx)));
        return;
    }

    if (fn_8018F3D0(fn_8018F310(fn_8018F3BC((s32) idx))) == 1) {
        fn_8019044C(sibling, 200.0f);
        return;
    }
    HSD_JObjSetFlagsAll(sibling, 0x10U);
}

// Positioned def (idiom 87): original slot 0x804DA744 sits right after
// fn_80191FD4's 200.0f/0.9f pool pair -- ours lands exactly there.
const f32 lbl_804DA744 = 201.0f;

extern const f32 lbl_804DA750; // -1.8f
extern const f32 lbl_804DA754; // 7.1899995f (DOL 40e6147a, NOT 7.19f)
extern const f32 lbl_804DA758; // 2.7f
extern f32 lbl_804DA75C;       // 2.3f

/// Updates tournament menu cursor JObj visibility and position.
void fn_8019237C(HSD_GObj* gobj)
{
    TmData* tm;
    HSD_JObj* jobj;

    tm = gm_8018F634();
    jobj = gobj->hsd_obj;

    if (tm->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if (tm->cur_option != 0xF) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    fn_8019044C(jobj, (f32) lbl_804799B8.xA);
    fn_8018FDC4(
        jobj, (lbl_804DA754 * (f32) (lbl_804799B8.x5 % 4)) + lbl_804DA750,
        -((lbl_804DA75C * (f32) ((s32) lbl_804799B8.x5 / 4)) - lbl_804DA758),
        lbl_804DA734);
}

// Positioned defs (idiom 87): original slots 0x804DA750/54/58 directly
// follow fn_80191FD4's pool pair; ours emit after their one user
// (fn_8019237C, const-prop law). 7.1899995f encodes DOL 40e6147a.
const f32 lbl_804DA750 = -1.8f;
const f32 lbl_804DA754 = 7.1899995f;
const f32 lbl_804DA758 = 2.7f;

/// @todo Currently 92.46% match - permuter couldn't improve
void fn_8019249C(HSD_GObj* gobj)
{
    TmData* tmdata;
    HSD_JObj* jobj;
    HSD_JObj* child;
    HSD_JObj* child2;
    int cur_option;

    tmdata = gm_8018F634();
    jobj = gobj->hsd_obj;

    if (tmdata->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
    cur_option = tmdata->cur_option;
    if (cur_option != 0xD && cur_option != 0xE) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    child = HSD_JObjGetChild(jobj);
    child = HSD_JObjGetNext(child);
    child = HSD_JObjGetNext(child);
    child = HSD_JObjGetNext(child);
    child = HSD_JObjGetNext(child);
    child = HSD_JObjGetNext(child);
    child = HSD_JObjGetNext(child);
    HSD_JObjGetNext(child);
    child2 = HSD_JObjGetNext(child);
    child2 = HSD_JObjGetNext(child2);
    child2 = HSD_JObjGetNext(child2);
    child2 = HSD_JObjGetNext(child2);
    HSD_JObjGetNext(child2);

    if (cur_option == 0xD) {
        fn_8019044C(child, 0.0F);
        fn_8019044C(child2, (f32) (lbl_804799B8.xA + 10));
    } else {
        fn_8019044C(child, (f32) (lbl_804799B8.xA + 10));
        fn_8019044C(child2, 0.0F);
    }
}

extern const f32 lbl_804DA760; // 0.3f

#pragma push
#pragma dont_inline on
void fn_80192690(HSD_GObj* gobj)
{
    TmData* tmdata = gm_8018F634();
    HSD_JObj* jobj = gobj->hsd_obj;

    if (tmdata->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }
    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);
    if (tmdata->cur_option < 0x11 || tmdata->cur_option > 0x12) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }
    fn_8018FDC4(jobj, lbl_804DA734, lbl_804DA734, lbl_804DA760);
    fn_8019044C(jobj, tmdata->cur_option - 0x11);
}
#pragma pop

// Positioned def (idiom 87): original slot 0x804DA760 sits between the
// lbl_804DA750..58 trio and fn_80192BB0's 11.5f/3.5f pool pair.
const f32 lbl_804DA760 = 0.3f;

/// @todo Currently 96.8% match - permuter couldn't improve
void fn_80192758(HSD_GObj* gobj)
{
    TmData* tmdata;
    HSD_JObj* jobj;
    u8* data;
    u8* data3;
    u8* data2;
    HSD_JObj* child;
    u8 val;

    data = (u8*) &lbl_804799B8;
    tmdata = gm_8018F634();
    jobj = gobj->hsd_obj;

    if (tmdata->cur_option <= 9) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    HSD_JObjClearFlagsAll(jobj, JOBJ_HIDDEN);

    if (data[0] == 0) {
        HSD_JObjSetFlagsAll(jobj, JOBJ_HIDDEN);
        return;
    }

    data2 = data + 2;
    data3 = data + 3;
    fn_8019044C(jobj, (float) tmdata->x37[data2[0] + data3[0]].x2);

    if (data[1] != 0) {
        HSD_JObjClearFlags((jobj == NULL) ? NULL : jobj->child, JOBJ_HIDDEN);

        child = (jobj == NULL) ? NULL : jobj->child;
        HSD_JObjClearFlags((child == NULL) ? NULL : child->next, JOBJ_HIDDEN);

        val = tmdata->x37[data2[0] + data3[0]].x2;
        if (val == 9) {
            HSD_JObjSetFlags((jobj == NULL) ? NULL : jobj->child,
                             JOBJ_HIDDEN);
            return;
        }
        if (val == 1) {
            child = (jobj == NULL) ? NULL : jobj->child;
            HSD_JObjSetFlags((child == NULL) ? NULL : child->next,
                             JOBJ_HIDDEN);
            return;
        }
    } else {
        HSD_JObjSetFlags((jobj == NULL) ? NULL : jobj->child, JOBJ_HIDDEN);

        child = (jobj == NULL) ? NULL : jobj->child;
        HSD_JObjSetFlags((child == NULL) ? NULL : child->next, JOBJ_HIDDEN);
    }
    // @-pin shim (id ledger: 5 stmt-ternaries -> arg-ternaries, minus 2
    // discarded-call expansions in fn_8019249C at +2 each)
    goto pad_label;
pad_label:;
}

extern s32 lbl_804D665C;

#pragma push
#pragma inline_depth(0)
s32 fn_80192938(void)
{
    s32 i;
    TmData* tm;
    s32 j;
    s32 start;
    s32 a, b;
    u8 tmp;
    u8 handicap;
    PAD_STACK(8);

    tm = gm_8018F634();
    lbl_804D6658 = 0;
    tm->cur_option = 9;

    if ((s32) gm_804771C4.match_type == 0) {
        tm->x2E = lbl_803D9D20.x0[tm->entrants];
        tm->x2F = (u8) tm->hmn_cpu_count;
    } else {
        u8 comb = (u8) tm->combatants;
        tm->x2E = comb;
        tm->x2F = comb;
        tm->x30 = (u8) tm->entrants;
    }

    for (i = 0; i < 0x40; i++) {
        tm->x37[i].x5 = 0;
        tm->x37[i].xD = (u8) i;

        if ((u8) gmMainLib_8015CC34()->handicap == 1) {
            if ((s32) gm_804771C4.match_type == 0) {
                gmMainLib_8015CC34()->handicap = 0;
                tm->x37[i].x2 = 9;
            } else {
                tm->x37[i].x2 = 5;
            }
        } else {
            tm->x37[i].x2 = 9;
        }

        tm->x37[i].x3 = fn_8018F410();
        tm->x37[i].x7 = HSD_Randi(
            (s32) gm_80169238(fn_8018F6FC((enum CSSIconHud) tm->x37[i].x3)));

        if (i < (s32) tm->x2E) {
            ((u8*) &tm->x37[i])[-1] = 1;
            if (i < (s32) tm->x2F) {
                tm->x37[i].x0 = 0;
                tm->x37[i].x9 = (u16) (i + 0x320);
            } else {
                tm->x37[i].x0 = 1;
                tm->x37[i].x9 = (u16) ((i + 0x384) - tm->x2F);
                if (((s32) gm_804771C4.match_type == 0) &&
                    ((s32) tm->cpu_level == 0))
                {
                    tm->x37[i].x1 = HSD_Randi(9) + 1;
                } else {
                    tm->x37[i].x1 = (u8) tm->cpu_level;
                }
            }
        } else {
            ((u8*) &tm->x37[i])[-1] = 0;
            tm->x37[i].x0 = 3;
            tm->x37[i].x9 = 0x3E7;
        }
    }

    start = 0;
    tm->x4B8[0].x2 = 0;
    tm->x4B8[1].x2 = 0;
    tm->x4B8[2].x2 = 0;
    tm->x4B8[3].x2 = 0;

    if ((s32) gm_804771C4.match_type == 0) {
        if ((s32) lbl_804D665C < 2) {
            start = 1;
        }
        for (j = 0; j < 0x3E8; j++) {
            a = start + HSD_Randi(tm->x2E - start);
            b = start + HSD_Randi(tm->x2E - start);
            tmp = tm->x37[a].xD;
            tm->x37[a].xD = tm->x37[b].xD;
            tm->x37[b].xD = tmp;
        }
    }

    lbl_804799B8.x2 = 0;
    lbl_804799B8.x3 = 0;
    lbl_804799B8.x4 = 0xB;
    lbl_804799B8.x5 = 0;
    lbl_804799B8.x6 = 0;
    handicap = gmMainLib_8015CC34()->handicap;
    lbl_804799B8.x0 = handicap;
    tm->x24 = 0;
    tm->pad_x34[0] = 0xFF;
    tm->x33 = 0xFF;
    return (s32) handicap;
}
#pragma pop

void fn_80192BB0(void)
{
    HSD_GObj* gobj;
    HSD_JObj* jobj;
    s32 i;

    fn_8019035C(1, lbl_804D664C->models[2], 0, 0x1A, 2, 1,
                (void (*)(HSD_GObj*)) fn_8019175C, 0.0f);

    for (i = 0; i < 6; i++) {
        gobj = fn_8019035C(1, lbl_804D664C->models[3], 0, 0x1A, 2, 1,
                           fn_80191240, 0.0f);
        jobj = (HSD_JObj*) gobj->hsd_obj;
        fn_8018FBD8((void*) gobj, i);
        HSD_JObjSetTranslateY(jobj, -((3.5f * (f32) i) - 11.5f));

        gobj = fn_8019035C(1, lbl_804D664C->models[0], 0, 0x1A, 2, 1,
                           fn_801913BC, 0.0f);
        jobj = (HSD_JObj*) gobj->hsd_obj;
        fn_8018FBD8((void*) gobj, i);
        HSD_JObjSetTranslateY(jobj, -((3.5f * (f32) i) - 11.5f));

        gobj = fn_8019035C(1, lbl_804D664C->models[1], 0, 0x1A, 2, 1,
                           fn_80191678, 0.0f);
        jobj = (HSD_JObj*) gobj->hsd_obj;
        fn_8018FBD8((void*) gobj, i);
        HSD_JObjSetTranslateY(jobj, -((3.5f * (f32) i) - 11.5f));
    }
}

void fn_80192E6C(void)
{
    int new_var;
    HSD_GObj* gobj;
    HSD_JObj* new_var3;
    HSD_JObj* jobj;
    s32 i;
    s32 j;
    f32 new_var2;
    PAD_STACK(16);

    fn_8019035C(1, lbl_804D6650->models[4], 0, 0x1A, 2, 1, fn_801918F0, 0.0f);
    fn_8019035C(1, lbl_804D6650->models[7], 0, 0x1A, 2, 1, fn_80191A54, 0.0f);
    fn_8019035C(1, lbl_804D6650->models[5], 0, 0x1A, 2, 1,
                (void (*)(HSD_GObj*)) fn_80191B5C, 0.0f);
    fn_8019035C(1, lbl_804D6650->models[0], 0, 0x1A, 2, 1, fn_80191CA4, 0.0f);
    fn_8019035C(1, lbl_804D6650->models[6], 0, 0x1A, 2, 1, fn_8019237C, 0.0f);
    fn_8019035C(1, lbl_804D6650->models[10], 0, 0x1A, 2, 1, fn_8019249C, 0.0f);
    HSD_JObjSetFlagsAll((HSD_JObj*) fn_8019035C(1, lbl_804D6650->models[2], 0,
                                                0x1A, 2, 1, fn_80192758, 0.0f)
                            ->hsd_obj,
                        0x10U);

    for (i = 0; i < 0x40; i++) {
        gobj = fn_8019035C(1, lbl_804D6650->models[8], 0, 0x1A, 2, 1,
                           fn_80191D38, 0.0f);
        fn_8018FDC4((HSD_JObj*) gobj->hsd_obj, -12.300001f, 12.800008f,
                    lbl_804DA734);
        fn_8018FBD8((void*) gobj, i);

        gobj = fn_8019035C(1, lbl_804D6650->models[3], 0, 0x1A, 2, 1,
                           fn_80191E9C, 0.0f);
        jobj = (HSD_JObj*) gobj->hsd_obj;
        HSD_JObjSetFlagsAll(jobj, 0x10U);
        fn_8018FDC4(jobj, -12.300001f, lbl_804DA734, lbl_804DA734);
        fn_8018FBD8((void*) gobj, i);
    }

    j = 0; // @-pin shim (-1): while-form pays the lbl_804DA734 CSE temp id
    while (j <= 0x19) {
        new_var2 = (f32) j;
        gobj = fn_8019035C(1, lbl_804D6650->models[9], 0, 0x1A, 2, 1,
                           (void (*)(HSD_GObj*)) fn_80191FD4, new_var2);
        jobj = (HSD_JObj*) gobj->hsd_obj;
        new_var3 = jobj;
        if (j != 0x19) {
            fn_8018FF9C(new_var3, 0.65f, 0.66f, lbl_804DA734);
            fn_8018FDC4(new_var3, (6.0f * (f32) (j % 5)) + -3.0f,
                        -((4.500006f * (f32) (new_var = j / 5)) - 2.3f), 0.2f);
        } else {
            fn_8018FDC4(new_var3, -1.0f, 10.099993f, 0.2f);
        }
        fn_8018FBD8((void*) gobj, j);
        j++;
    }

    fn_8019035C(1, lbl_804D6650->models[1], 0, 0x1A, 2, 1, fn_80192690, 0.0f);
}

// Positioned def (idiom 87): the original slot 0x804DA734 sits between
// fn_80191B5C's 25.0f and fn_80191D38's pool range; ours must follow
// its last user (fn_80192E6C, const-prop law).
// lbl_804DA738/lbl_804DA75C stay UNDEFINED here by design: their pool
// slots are fn_80192E6C's kept 12.800008f/2.3f literals (@-entries).
// The extern-swap of those two sites was re-probed this wave and still
// regresses E6C (97.71, loop LICM shape) -- w12 park stands.
const f32 lbl_804DA734 = 666.0f;

#pragma push
#pragma dont_inline on
void fn_80193230(void)
{
    HSD_GObj* gobj;
    HSD_Fog* fog;
    PAD_STACK(4);

    fn_8019035C(0, lbl_804D664C->models[5], 0, 0x1A, 2, 1, fn_801910E0, 0.0F);
    fn_8019035C(0, lbl_804D664C->models[4], 0, 0x1A, 2, 1, fn_80191154, 0.0F);
    fn_80192BB0();
    fn_80192E6C();
    gobj = GObj_Create(0xE, 0x1A, 0);
    fog = HSD_FogLoadDesc(lbl_804D664C->fogs[0].desc);
    HSD_GObjObject_80390A70(gobj, HSD_GObj_804D7848, fog);
    GObj_SetupGXLink(gobj, HSD_GObj_FogCallback, 0, 0);
}
#pragma pop

/// .sdata2
/* 4DA78C */ extern const s32 lbl_804DA78C;

void fn_80193308(void)
{
    HSD_Text* created_text2;
    s32* text_color_word;
    s32 color;
    TmData* tm;
    HSD_Text* text;
    GXColor* text_color;
    HSD_Text** ptr;
    f32 y;
    s32 i;
    s32 idx;
    s32* color_word;
    s32 count;
    HSD_Text* created_text;

    color = lbl_804DA78C;
    tm = gm_8018F634();

    tm->x4E0 = HSD_SisLib_803A5ACC(0, (s32) lbl_804D663C, 124.5f, 45.0f, 0.0f,
                                   391.0f, 30.0f);
    tm->x4E0->default_fitting = 1;
    HSD_SisLib_803A6368(tm->x4E0, 0x4A);

    tm->x4E4 = HSD_SisLib_803A5ACC(0, (s32) lbl_804D663C, 393.0f, 221.7f, 0.0f,
                                   360.0f, 40.0f);
    tm->x4E4->hidden = 1;

    for (i = 0; i < 6; i++) {
        tm->x4E8[i] = HSD_SisLib_803A5ACC(0, (s32) lbl_804D663C, 160.0f, 48.0f,
                                          0.0f, 360.0f, 40.0f);
        text = tm->x4E8[i];
        text->pos_x = 123.0f;
        y = (35.7f * (f32) i) + 113.0f;
        text->pos_y = y;
        text->pos_z = 0.0f;
        tm->x4E8[i]->hidden = 1;
        HSD_SisLib_803A6368(tm->x4E8[i], 0x5C);

        tm->x500[i] = HSD_SisLib_803A5ACC(0, (s32) lbl_804D663C, 160.0f, 48.0f,
                                          0.0f, 360.0f, 40.0f);
        text = tm->x500[i];
        text->pos_x = 343.0f;
        text->pos_y = y;
        text->pos_z = 0.0f;
        tm->x500[i]->hidden = 1;
        HSD_SisLib_803A6368(tm->x500[i], 0x53);
    }

    tm->x518[0] = HSD_SisLib_803A6754(0, (s32) lbl_804D663C);
    text = tm->x518[0];
    text->font_size.x = 0.5f;
    text->font_size.y = 0.5f;
    tm->x518[0]->default_kerning = 1;

    count = 0;
    idx = 1;
    if ((!tm) && (!tm)) {
    }
    color_word = &color;
    do {
        created_text = HSD_SisLib_803A6754(0, (s32) lbl_804D663C);
        ptr = &tm->x518[idx];
        *ptr = created_text;
        text = *ptr;
        text->font_size.x = 0.58f;
        text->font_size.y = 0.55f;
        (*ptr)->default_kerning = 1;
        if (count != 0) {
            *(text_color_word = (s32*) (&(*ptr)->text_color)) = *color_word;
        }
        count += 1;
        idx = 2;
    } while (count < 2);

    count = 0;
    idx = 3;
    do {
        created_text2 = HSD_SisLib_803A6754(0, (s32) lbl_804D663C);
        ptr = &tm->x518[idx];
        *ptr = created_text2;
        text = *ptr;
        text->font_size.x = 0.85f;
        text->font_size.y = 1.35f;
        (*ptr)->default_kerning = 1;
        (*ptr)->default_alignment = 1;
        if (count) {
            *((s32*) (text_color = &(*ptr)->text_color)) = *color_word;
            ;
        }
        count += 1;
        idx = 4;
    } while (count < 2);
    PAD_STACK(0x28);
}

// Positioned def (idiom 87): original slot 0x804DA78C sits just before
// fn_80193308's pool cluster; ours emits after its one user (const-prop
// law). 0x46DC46FF = the DOL word (an RGBA color, not a float).
const s32 lbl_804DA78C = 0x46DC46FF;

#pragma push
#pragma dont_inline on
void fn_801935B8(void)
{
    TmData* tm;
    HSD_GObj* gobj;
    HSD_Fog* fog;
    s32 i;

    tm = gm_8018F634();
    fn_8018FBE0(0, 0, 0, 5, 5, 0x3e7, 3);
    fn_801902F0((s32) fn_80190174(lbl_804D664C->cameras->desc));
    fn_80193308();
    fn_8019027C(lbl_804D664C->lights);
    fn_8019035C(0, lbl_804D664C->models[5], 0, 0x1A, 2, 1, fn_801910E0,
                0.0f);
    fn_8019035C(0, lbl_804D664C->models[4], 0, 0x1A, 2, 1, fn_80191154,
                0.0f);
    fn_80192BB0();
    fn_80192E6C();
    gobj = GObj_Create(0xE, 0x1A, 0);
    fog = HSD_FogLoadDesc(lbl_804D664C->fogs[0].desc);
    HSD_GObjObject_80390A70(gobj, HSD_GObj_804D7848, fog);
    GObj_SetupGXLink(gobj, HSD_GObj_FogCallback, 0, 0);

    lbl_804D6658 = 0;
    lbl_804799B8.xC_counter = 0;
    lbl_804799B8.xE = 0;
    lbl_804799B8.xF = 0;
    lbl_804799B8.pad2[12] = 0;

    for (i = 0; i < 6; i++) {
        lbl_804799B8.pad2[i] = 0xa;
        lbl_804799B8.pad2[i + 6] = 0;
        (&tm->match_type)[i] = lbl_803D9F80[0x40 + i * 2];
    }

    lbl_804799B8.x1 = 0;
    lbl_804799B8.x0 = gmMainLib_8015CC34()->handicap;

    tm->x32 = 0;
    for (i = 0; i < 25; i++) {
        lbl_803D9D20.x72[i] = gm_80164840((u8) fn_8018F6FC(i));
    }
}
#pragma pop

void fn_801937C4(s32* arg0, u32 arg1, u32 arg2)
{
    u8* table = lbl_803D9F80;
    s32 idx;
    s32* ptr;
    s32 val;
    u8* entry;

    table = (u8*) table + 0;
    if (arg1 & 0x40001) {
        lbAudioAx_80024030(2);
        lbl_804799B8.x7 = 5;
        idx = arg0[0];
        entry = table + (idx << 1) + (gm_804771C4.match_type != 0);
        ptr = arg0 + idx;
        val = *++ptr;
        if (val > (s32) entry[0x40]) {
            *ptr = val - 1;
        } else {
            *ptr = (s32) entry[0x4C];
        }
    } else if (arg1 & 0x80002) {
        lbAudioAx_80024030(2);
        lbl_804799B8.x8 = 5;
        idx = arg0[0];
        entry = table + (idx << 1) + (gm_804771C4.match_type != 0);
        ptr = arg0 + idx;
        val = *++ptr;
        if (val < (s32) entry[0x4C]) {
            *ptr = val + 1;
        } else {
            *ptr = (s32) entry[0x40];
        }
    }

    if (arg1 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
        }
        fn_80190ABC(3);
    }

    if (arg2 & 0x100) {
        s32 j;
        s32 remaining;
        u8* tp;
        s32* dp;

        lbAudioAx_80024030(1);
        idx = arg0[0];
        arg0[0] = idx + 1;
        idx = arg0[0];
        tp = table + (idx << 1);
        dp = arg0 + idx;
        remaining = 6 - idx;
        if (idx < 6) {
            for (j = 0; j < remaining; j++) {
                *++dp = tp[(gm_804771C4.match_type != 0) + 0x40];
                tp += 2;
            }
        }
        if (gm_804771C4.match_type == 0) {
            if (fn_8018F808() < 2) {
                idx = arg0[0];
                arg0[idx + 1] = 2;
            } else {
                s32 count = fn_8018F808();
                idx = arg0[0];
                arg0[idx + 1] = count;
            }
        }
    } else if (arg2 & 0x200) {
        lbAudioAx_80024030(0);
        gm_801A42F8(1);
        gm_801A4B60();
    }

    if (arg2 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
        }
        fn_80190ABC(3);
    }
}


void fn_80193B58(s32* arg0, u32 arg1, u32 arg2)
{
    u8* table = lbl_803D9F80;
    s32 idx;
    s32* ptr;
    s32 val;
    u8* entry;
    s32 clamp_val;
    int* mt = &gm_804771C4.match_type;
    TmData* tm;

    if (*mt == 0) {
        lbl_804D665C = fn_8018F808();
    }

    if (*mt == 0) {
        clamp_val = MAX(2, (s32) lbl_804D665C);
        idx = arg0[0];
        ptr = arg0 + idx;
        val = *++ptr;
        if (val > clamp_val) {
            *ptr = clamp_val;
            fn_80190ABC(3);
        }
    }

    if (arg1 & 0x40001) {
        if (*mt == 0) {
            idx = arg0[0];
            entry = table + (idx << 1) + (*mt != 0);
            ptr = arg0 + idx;
            val = *++ptr;
            if (val > (s32) entry[0x40]) {
                *ptr = val - 1;
                lbAudioAx_80024030(2);
                lbl_804799B8.x7 = 5;
            } else {
                s32 max = (s32) entry[0x4C];
                if (clamp_val < max) {
                    *ptr = clamp_val;
                } else {
                    *ptr = max;
                }
                idx = arg0[0];
                entry = table + (idx << 1) + (*mt != 0);
                val = arg0[idx + 1];
                if (val != (s32) entry[0x40]) {
                    lbAudioAx_80024030(2);
                    lbl_804799B8.x7 = 5;
                }
            }
        } else {
            lbAudioAx_80024030(2);
            lbl_804799B8.x7 = 5;
            idx = arg0[0];
            entry = table + (idx << 1) + (*mt != 0);
            ptr = arg0 + idx;
            val = *++ptr;
            if (val > (s32) entry[0x40]) {
                *ptr = val - 1;
            } else {
                *ptr = (s32) entry[0x4C];
            }
        }
    } else if (arg1 & 0x80002) {
        if (*mt == 0) {
            idx = arg0[0];
            entry = table + (idx << 1) + (*mt != 0);
            ptr = arg0 + idx;
            val = *++ptr;
            if (val < (s32) entry[0x4C]) {
                if (val + 1 <= clamp_val) {
                    *ptr = val + 1;
                    lbAudioAx_80024030(2);
                    lbl_804799B8.x8 = 5;
                    goto post;
                }
                *ptr = (s32) entry[0x40];
                idx = arg0[0];
                val = arg0[idx + 1];
                if (val != clamp_val) {
                    lbAudioAx_80024030(2);
                    lbl_804799B8.x8 = 5;
                }
            } else {
                *ptr = (s32) entry[0x40];
                idx = arg0[0];
                entry = table + (idx << 1) + (*mt != 0);
                val = arg0[idx + 1];
                if (val != (s32) entry[0x4C]) {
                    lbAudioAx_80024030(2);
                    lbl_804799B8.x8 = 5;
                }
            }
        } else {
            lbAudioAx_80024030(2);
            lbl_804799B8.x8 = 5;
            idx = arg0[0];
            entry = table + (idx << 1) + (*mt != 0);
            ptr = arg0 + idx;
            val = *++ptr;
            if (val < (s32) entry[0x4C]) {
                *ptr = val + 1;
            } else {
                *ptr = (s32) entry[0x40];
            }
        }
    }

post:
    if (*mt == 0) {
        ((TmData*) arg0)->x30 = (u8) arg0[2];
    } else {
        if (arg0[3] > arg0[2] - 1) {
            arg0[3] = arg0[2] - 1;
        }
    }

    if (arg1 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
        }
        fn_80190ABC(3);
    }

    if (arg2 & 0x100) {
        lbAudioAx_80024030(1);
        arg0[0] = arg0[0] + 1;
        if (*mt == 0) {
            u8 x30 = ((TmData*) arg0)->x30;
            if (x30 == 2) {
                arg0[3] = 0;
            } else if (x30 == 3) {
                arg0[3] = 9;
            } else {
                arg0[3] = 0xE;
            }
            tm = gm_8018F634();
            fn_8018EC7C();
            fn_8018E618(tm->entrants, lbl_804DA6D8, 1);
            fn_80190480(lbl_804DA6DC);
            fn_80190520(lbl_804DA6E0, lbl_804DA6E4, 0.0f);
        }
    } else if (arg2 & 0x200) {
        lbAudioAx_80024030(0);
        arg0[0] = arg0[0] - 1;
    }

    if (arg2 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
        }
        fn_80190ABC(3);
    }
}

void fn_80193FCC(s32* arg0, u32 arg1, u32 arg2)
{
    u8* table = lbl_803D9F80;
    s32 idx;
    s32* ptr;
    s32 val;
    u8* entry;
    s32 clamp_val;
    int* mt = &gm_804771C4.match_type;
    TmData* tm;

    if (*mt != 0) {
        lbl_804D665C = fn_8018F808();
    }

    if (*mt != 0) {
        if (fn_8018F808() < 2) {
            clamp_val = 2;
        } else {
            clamp_val = fn_8018F808();
        }
        idx = arg0[0];
        ptr = arg0 + idx;
        val = *++ptr;
        if (val > clamp_val) {
            *ptr = clamp_val;
            fn_80190ABC(3);
        }
    }

    if (arg1 & 0x40001) {
        if (*mt != 0) {
            idx = arg0[0];
            entry = table + (idx << 1) + (*mt != 0);
            ptr = arg0 + idx;
            val = *++ptr;
            if (val > (s32) entry[0x40]) {
                *ptr = val - 1;
                lbAudioAx_80024030(2);
                lbl_804799B8.x7 = 5;
            } else {
                s32 max_plus1 = (s32) entry[0x4C] + 1;
                if (clamp_val < max_plus1) {
                    *ptr = clamp_val;
                } else {
                    *ptr = max_plus1;
                }
                idx = arg0[0];
                entry = table + (idx << 1) + (*mt != 0);
                val = arg0[idx + 1];
                if (val != (s32) entry[0x40]) {
                    lbAudioAx_80024030(2);
                    lbl_804799B8.x7 = 5;
                }
            }
        } else {
            lbAudioAx_80024030(2);
            lbl_804799B8.x7 = 5;
            idx = arg0[0];
            entry = table + (idx << 1) + (*mt != 0);
            ptr = arg0 + idx;
            val = *++ptr;
            if (val > (s32) entry[0x40]) {
                *ptr = val - 1;
            } else {
                *ptr = (s32) entry[0x4C];
            }
        }
        if (*mt != 0) {
            goto post_clamp;
        }
        {
            u8 x30 = ((TmData*) arg0)->x30;
            switch (x30) {
            case 2:
                if (arg0[3] == 0x1F) {
                    arg0[3] = 8;
                }
                break;
            case 3:
                if (arg0[3] == 8) {
                    arg0[3] = 0xD;
                }
                break;
            case 4:
                if (arg0[3] == 0xD) {
                    arg0[3] = 0x1F;
                }
                break;
            }
            tm = gm_8018F634();
            fn_8018EC7C();
            fn_8018E618(tm->entrants, lbl_804DA6D8, 1);
            fn_80190480(lbl_804DA6DC);
            fn_80190520(lbl_804DA6E0, lbl_804DA6E4, 0.0f);
        }
    } else if (arg1 & 0x80002) {
        if (*mt != 0) {
            idx = arg0[0];
            entry = table + (idx << 1) + (*mt != 0);
            ptr = arg0 + idx;
            val = *++ptr;
            if (val < (s32) entry[0x4C] + 1) {
                if (val + 1 <= clamp_val && val + 1 < arg0[2]) {
                    *ptr = val + 1;
                    lbAudioAx_80024030(2);
                    lbl_804799B8.x8 = 5;
                    goto after_right;
                }
                *ptr = (s32) entry[0x40];
                idx = arg0[0];
                val = arg0[idx + 1];
                if (val != clamp_val) {
                    lbAudioAx_80024030(2);
                    lbl_804799B8.x8 = 5;
                }
            } else {
                *ptr = (s32) entry[0x40];
                idx = arg0[0];
                entry = table + (idx << 1) + (*mt != 0);
                val = arg0[idx + 1];
                if (val != (s32) entry[0x4C] + 1) {
                    lbAudioAx_80024030(2);
                    lbl_804799B8.x8 = 5;
                }
            }
        } else {
            lbAudioAx_80024030(2);
            lbl_804799B8.x8 = 5;
            idx = arg0[0];
            entry = table + (idx << 1) + (*mt != 0);
            ptr = arg0 + idx;
            val = *++ptr;
            if (val < (s32) entry[0x4C]) {
                if (*mt != 0) {
                    if (val == arg0[2] - 1) {
                        *ptr = (s32) entry[0x40];
                        goto after_right;
                    }
                    *ptr = val + 1;
                    goto after_right;
                }
                *ptr = val + 1;
            } else {
                *ptr = (s32) entry[0x40];
            }
        }
    after_right:
        if (*mt != 0) {
            goto post_clamp;
        }
        {
            u8 x30 = ((TmData*) arg0)->x30;
            switch (x30) {
            case 2:
                if (arg0[3] == 9) {
                    arg0[3] = 0;
                }
                break;
            case 3:
                if (arg0[3] == 0xE) {
                    arg0[3] = 9;
                }
                break;
            case 4:
                if (arg0[3] == 0) {
                    arg0[3] = 0xE;
                }
                break;
            }
            tm = gm_8018F634();
            fn_8018EC7C();
            fn_8018E618(tm->entrants, lbl_804DA6D8, 1);
            fn_80190480(lbl_804DA6DC);
            fn_80190520(lbl_804DA6E0, lbl_804DA6E4, 0.0f);
        }
    }

post_clamp:
    if (*mt != 0) {
        if (arg0[3] > arg0[2] - 1) {
            arg0[3] = arg0[2] - 1;
        }
        if (arg0[4] > arg0[3] - 1) {
            arg0[4] = arg0[3] - 1;
        }
        if (arg0[4] > arg0[2] - arg0[3]) {
            arg0[4] = arg0[2] - arg0[3];
        }
    }

    if (*mt != 0) {
        ((TmData*) arg0)->x30 = (u8) arg0[3];
    }

    if (arg1 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
            if (*mt == 0) {
                if (arg2 & 0x100) {
                    if ((s32) lbl_804D665C < 2) {
                        arg0[4] = 1;
                    } else {
                        arg0[4] = (s32) lbl_803D9D20.x0[arg0[3]];
                    }
                    fn_80190ABC(1);
                }
            }
        }
        fn_80190ABC(3);
    }

    if (arg2 & 0x100) {
        lbAudioAx_80024030(1);
        arg0[0] = arg0[0] + 1;
    } else if (arg2 & 0x200) {
        lbAudioAx_80024030(0);
        arg0[0] = arg0[0] - 1;
        if (*mt == 0) {
            fn_8018EC7C();
        }
    }

    if (arg2 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
            if (*mt == 0) {
                if (arg2 & 0x100) {
                    if ((s32) lbl_804D665C < 2) {
                        arg0[4] = 1;
                    } else {
                        arg0[4] = (s32) lbl_803D9D20.x0[arg0[3]];
                    }
                    fn_80190ABC(1);
                }
            }
        }
        fn_80190ABC(3);
    }
}

void fn_80194658(s32* arg0, u32 arg1, u32 arg2)
{
    u8* table;
    s32 idx;
    s32* ptr;
    s32 val;
    s32 dir;
    int* dir_ptr;
    u8* entry;
    s32 changed = 0;

    table = lbl_803D9F80;
    idx = arg0[0];
    dir_ptr = &gm_804771C4.match_type;
    dir = *dir_ptr;

    ptr = arg0 + idx;
    val = *(++ptr);

    if (dir == 0) {
        if (lbl_804D665C < 2) {
            if (arg1 & 0x40001) {
                if (val != 0) {
                    *ptr = 0;
                } else {
                    *ptr = 1;
                }
                goto end;
            }
            if (arg1 & 0x80002) {
                changed = 1;
                if (val != 0) {
                    *ptr = 0;
                } else {
                    *ptr = 1;
                }
                goto end;
            }
            goto end;
        }
    }

    if (arg1 & 0x40001) {
        entry = table + (idx << 1) + (dir != 0);
        if (val > (s32) entry[0x40]) {
            *ptr = *ptr - 1;
        } else if (dir == 0) {
            *ptr = (s32) lbl_803D9D20.x0[arg0[3]];
        } else {
            *ptr = (s32) entry[0x4C];
        }
    } else if (arg1 & 0x80002) {
        entry = table + (idx << 1) + (dir != 0);
        changed = 1;
        if (val < (s32) entry[0x4C]) {
            *ptr = *ptr + 1;
            if (*dir_ptr != 0) {
                if (arg0[4] > arg0[3] - 1 || arg0[4] > arg0[2] - arg0[3]) {
                    arg0[arg0[0] + 1] = 1;
                }
            }
        } else if (dir == 0) {
            *ptr = (s32) entry[0x40];
        } else {
            *ptr = 1;
        }
    }

end:
    if (*dir_ptr == 0) {
        if (arg0[4] > (s32) lbl_803D9D20.x0[arg0[3]]) {
            arg0[arg0[0] + 1] = 0;
        }
    } else {
        if (arg0[4] > arg0[3] - 1) {
            arg0[4] = arg0[3] - 1;
        }
        if (arg0[4] > arg0[2] - arg0[3]) {
            arg0[4] = arg0[2] - arg0[3];
        }
    }

    if (val != arg0[4]) {
        lbAudioAx_80024030(2);
        if (changed == 0) {
            lbl_804799B8.x7 = 5;
        } else {
            lbl_804799B8.x8 = 5;
        }
    }

    if (arg1 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
        }
        fn_80190ABC(3);
        fn_80190ABC(1);
    }

    if (arg2 & 0x100) {
        lbAudioAx_80024030(1);
        arg0[0] = arg0[0] + 1;
    } else if (arg2 & 0x200) {
        lbAudioAx_80024030(0);
        arg0[0] = arg0[0] - 1;
    }

    if (arg2 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
        }
        fn_80190ABC(3);
        fn_80190ABC(1);
    }
}

/// @todo Currently 87.58% match - permuter couldn't improve
void fn_801949B4(s32* arg0, u32 arg1, u32 arg2)
{
    u8* table = lbl_803D9F80;
    s32 idx;
    s32* ptr;
    s32 val;
    u8* entry;

    table = (u8*) table + 0;
    if (arg1 & 0x40001) {
        lbAudioAx_80024030(2);
        lbl_804799B8.x7 = 5;

        idx = *arg0;
        ptr = arg0 + idx;
        entry = table + (idx << 1) + (gm_804771C4.match_type != 0);
        val = *++ptr;

        if (val > (s32) entry[0x40]) {
            *ptr = val - 1;
        } else {
            *ptr = (s32) entry[0x4C];
        }
    } else if (arg1 & 0x80002) {
        lbAudioAx_80024030(2);
        lbl_804799B8.x8 = 5;

        idx = *arg0;
        ptr = arg0 + idx;
        entry = table + (idx << 1) + (gm_804771C4.match_type != 0);
        val = *++ptr;

        if (val < (s32) entry[0x4C]) {
            *ptr = val + 1;
        } else {
            *ptr = (s32) entry[0x40];
        }
    }

    if (arg1 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
            fn_80190ABC(1);
        }
        fn_80190ABC(3);
    }

    if (arg2 & 0x100) {
        lbAudioAx_80024030(1);
        if (gm_804771C4.match_type == 0) {
            *arg0 = *arg0 + 1;
        } else {
            *arg0 = 6;
        }
    } else if (arg2 & 0x200) {
        lbAudioAx_80024030(0);
        *arg0 = *arg0 - 1;
    }

    if (arg2 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
            fn_80190ABC(2);
            fn_80190ABC(1);
        }
        fn_80190ABC(3);
    }
}

/// @todo Currently 84.75% match - needs register allocation fix
void fn_80194BC4(s32* arg0, u32 arg1, u32 arg2)
{
    s16* data_ptr;
    s32 option;
    s32 value;
    s32* arr_ptr;
    s16* opt_ptr;
    u8* lookup_ptr;

    data_ptr = (s16*) lbl_803D9F80;

    if (gm_804771C4.match_type != 0) {
        return;
    }

    if (arg1 & 0x40001) {
        lbAudioAx_80024030(2);
        lbl_804799B8.x7 = 5;
        option = *arg0;
        arr_ptr = (s32*) ((u8*) arg0 + (option << 2));
        opt_ptr = (s16*) ((u8*) data_ptr + (option << 1));
        value = *(++arr_ptr);
        lookup_ptr = (u8*) opt_ptr + (gm_804771C4.match_type != 0);
        if (value > lookup_ptr[0x40]) {
            *arr_ptr = value - 1;
        } else {
            *arr_ptr = lookup_ptr[0x4C];
        }
    } else if (arg1 & 0x80002) {
        lbAudioAx_80024030(2);
        lbl_804799B8.x8 = 5;
        option = *arg0;
        arr_ptr = (s32*) ((u8*) arg0 + (option << 2));
        opt_ptr = (s16*) ((u8*) data_ptr + (option << 1));
        value = *(++arr_ptr);
        lookup_ptr = (u8*) opt_ptr + (gm_804771C4.match_type != 0);
        if (value < lookup_ptr[0x4C]) {
            *arr_ptr = value + 1;
        } else {
            *arr_ptr = lookup_ptr[0x40];
        }
    }

    if (arg1 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
        }
        fn_80190ABC(3);
    }

    if (arg2 & 0x100) {
        lbAudioAx_80024030(1);
        *arg0 = *arg0 + 1;
    } else if (arg2 & 0x200) {
        lbAudioAx_80024030(0);
        *arg0 = *arg0 - 1;
    }

    if (arg2 != 0) {
        if ((arg1 | arg2) & 0x300) {
            fn_80190ABC(0);
        }
        fn_80190ABC(3);
    }
}

/// @todo Currently 97.2% match - permuter couldn't improve
/// Handles button input for tournament mode menu navigation.
void fn_80194D84(s32* state, u32 buttons, u32 trigger)
{
    s32 cur_state;
    TmData* tm;

    if (buttons & 0x40001) {
        if (*state > 6) {
            lbAudioAx_80024030(2);
            *state -= 1;
        }
    } else if ((buttons & 0x80002) && (*state < 8)) {
        lbAudioAx_80024030(2);
        *state += 1;
    }

    if ((buttons != 0) && ((buttons | trigger) & 0x300)) {
        fn_80190ABC(0);
    }

    if (trigger & 0x1100) {
        lbAudioAx_80024030(1);
        fn_8018EC7C();
        cur_state = *state;
        if (cur_state == 6) {
            fn_80192938();
        } else if (cur_state == 7) {
            tm = gm_8018F634();
            tm->x20 = 1;
            HSD_SisLib_803A5E70();
            mn_8022F138(0x19, 0x1C);
            mn_8022F138(0x12, 0x15);
            if (gm_804771C4.match_type == 0) {
                mn_802317E4(lbl_804D6648, 0);
            } else {
                mn_802317E4(lbl_804D6648, 1);
            }
            return;
        } else {
            *state = 0;
        }
    } else if (trigger & 0x200) {
        lbAudioAx_80024030(0);
        if (gm_804771C4.match_type == 0) {
            *state = 5;
        } else {
            *state = 4;
        }
    }

    if ((trigger != 0) && ((buttons | trigger) & 0x300)) {
        fn_80190ABC(0);
    }
}

s32 lbl_804D6654;

/// Handles tournament settings menu input (entrant configuration).
void fn_80194F30(s32* state_ptr, u32 buttons, u32 trigger)
{
    TmData* tm = (TmData*) state_ptr;
    s32 idx;

    if (trigger & 0x1000) {
        lbAudioAx_80024030(1);
        lbl_804D6654 = tm->cur_option;
        tm->cur_option = 0x11;
        return;
    }

    if (buttons & 0x40001) {
        switch (tm->cur_option) {
        case 12:
            lbAudioAx_80024030(2);
            tm->cur_option = 0xA;
            break;
        case 16:
            lbAudioAx_80024030(2);
            tm->cur_option = 0xC;
            lbl_804799B8.x1 = 0;
            break;
        }
    } else if (buttons & 0x80002) {
        switch (tm->cur_option) {
        case 11:
            break;
        case 10:
            lbAudioAx_80024030(2);
            tm->cur_option = 0xC;
            break;
        case 12:
            if (lbl_804799B8.x0 != 0) {
                lbAudioAx_80024030(2);
                tm->cur_option = 0x10;
            }
            break;
        }
    } else if (buttons & 0x10008) {
        if (tm->cur_option != 0x10 || lbl_804799B8.x1 == 0) {
            if (lbl_804799B8.x2 != 0) {
                lbAudioAx_80024030(2);
                lbl_804799B8.x2 -= 1;
                fn_80190ABC(5);
            } else if (lbl_804799B8.x3 != 0) {
                lbAudioAx_80024030(2);
                lbl_804799B8.x3 -= 1;
                lbl_804799B8.x4 -= 1;
                fn_80190ABC(5);
                fn_80190ABC(6);
            }
        } else {
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            if (tm->x37[idx].x2 > 1) {
                lbAudioAx_80024030(2);
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x2 -= 1;
            }
        }
    } else if (buttons & 0x20004) {
        if (tm->cur_option != 0x10 || lbl_804799B8.x1 == 0) {
            if (lbl_804799B8.x2 < 0xB) {
                if (lbl_804799B8.x2 + lbl_804799B8.x3 < tm->x2E - 1) {
                    lbAudioAx_80024030(2);
                    lbl_804799B8.x2 += 1;
                    fn_80190ABC(5);
                }
            } else if (lbl_804799B8.x2 + lbl_804799B8.x3 < tm->x2E - 1) {
                lbAudioAx_80024030(2);
                lbl_804799B8.x3 += 1;
                lbl_804799B8.x4 += 1;
                fn_80190ABC(5);
                fn_80190ABC(6);
            }
        } else {
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            if (tm->x37[idx].x2 < 9) {
                lbAudioAx_80024030(2);
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x2 += 1;
            }
        }
    }

    if (trigger & 0x100) {
        switch (tm->cur_option) {
        case 12:
        case 10:
            lbAudioAx_80024030(1);
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            tm->x37[idx].x4 = tm->x37[idx].x3;
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            tm->x37[idx].x8 = tm->x37[idx].x7;
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            tm->x37[idx].xB = tm->x37[idx].x9;
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            tm->x37[idx].x6 = tm->x37[idx].x5;
            tm->cur_option += 1;
            return;
        case 16:
            if (lbl_804799B8.x0 != 1) {
                if (lbl_804799B8.x1 != 1) {
                    lbAudioAx_80024030(1);
                }
                lbl_804799B8.x1 = 1;
                return;
            }
            break;
        }
    } else if (trigger & 0x200) {
        lbAudioAx_80024030(0);
        if (tm->cur_option != 0x10 || lbl_804799B8.x1 == 0) {
            tm->cur_option = 6;
            lbl_804799B8.xE = 0;
            if (gm_804771C4.match_type == 0) {
                TmData* tmdata = gm_8018F634();
                fn_8018EC7C();
                fn_8018E618(tmdata->entrants, lbl_804DA6D8, 1);
                fn_80190480(lbl_804DA6DC);
                fn_80190520(lbl_804DA6E0, lbl_804DA6E4, 0.0f);
            }
        } else {
            lbl_804799B8.x1 = 0;
        }
    }
}

/// Handles character selection input for tournament bracket.
void fn_801953C8(s32* state_ptr, u32 buttons, u32 trigger)
{
    TmData* tm = (TmData*) state_ptr;
    s32 idx;
    s32 cur_pos;
    s32 candidate;
    s32 group;
    s32 step;
    s32 adj;
    s32 occupied;
    s32 i;

    idx = lbl_804799B8.x2 + lbl_804799B8.x3;
    cur_pos = fn_8018F310(fn_8018F6FC(tm->x37[idx].x3));

    if (trigger & 0x1000) {
        lbAudioAx_80024030(0);
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        tm->x37[idx].x3 = tm->x37[idx].x4;
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        tm->x37[idx].x7 = tm->x37[idx].x8;
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        tm->x37[idx].x5 = tm->x37[idx].x6;
        lbl_804D6654 = *state_ptr;
        *state_ptr = 0x11;
        return;
    }

    if (buttons & 0x40001) {
        group = (cur_pos / 5) * 5;
        for (i = 1; i < 5; i++) {
            candidate = cur_pos - i;
            if (candidate < group) {
                candidate += 5;
            }
            adj = fn_8018F6DC(fn_8018F3BC(candidate));
            if (lbl_803D9D20.x72[adj] == 0) {
                if (fn_8018F3D0(fn_8018F310(fn_8018F6FC(adj))) != 2) {
                    occupied = 1;
                } else {
                    occupied = 0;
                }
            } else {
                occupied = 1;
            }
            if (occupied) {
                lbAudioAx_80024030(2);
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x7 = 0;
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x5 = 0;
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x3 = fn_8018F6DC(fn_8018F3BC(candidate));
                break;
            }
        }
    } else if (buttons & 0x80002) {
        group = ((cur_pos / 5) + 1) * 5;
        for (i = 1; i < 5; i++) {
            candidate = cur_pos + i;
            if (candidate >= group) {
                candidate -= 5;
            }
            adj = fn_8018F6DC(fn_8018F3BC(candidate));
            if (lbl_803D9D20.x72[adj] == 0) {
                if (fn_8018F3D0(fn_8018F310(fn_8018F6FC(adj))) != 2) {
                    occupied = 1;
                } else {
                    occupied = 0;
                }
            } else {
                occupied = 1;
            }
            if (occupied) {
                lbAudioAx_80024030(2);
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x7 = 0;
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x5 = 0;
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x3 = fn_8018F6DC(fn_8018F3BC(candidate));
                break;
            }
        }
    } else if (buttons & 0x10008) {
        step = 5;
        for (i = 1; i < 5; i++) {
            candidate = cur_pos - step;
            if (candidate < 0) {
                candidate += 25;
            }
            adj = fn_8018F6DC(fn_8018F3BC(candidate));
            if (lbl_803D9D20.x72[adj] == 0) {
                if (fn_8018F3D0(fn_8018F310(fn_8018F6FC(adj))) != 2) {
                    occupied = 1;
                } else {
                    occupied = 0;
                }
            } else {
                occupied = 1;
            }
            if (occupied) {
                lbAudioAx_80024030(2);
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x7 = 0;
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x5 = 0;
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x3 = fn_8018F6DC(fn_8018F3BC(candidate));
                break;
            }
            step += 5;
        }
    } else if (buttons & 0x20004) {
        step = cur_pos + 5;
        for (i = 1; i < 5; i++) {
            candidate = step;
            if (candidate >= 25) {
                candidate -= 25;
            }
            adj = fn_8018F6DC(fn_8018F3BC(candidate));
            if (lbl_803D9D20.x72[adj] == 0) {
                if (fn_8018F3D0(fn_8018F310(fn_8018F6FC(adj))) != 2) {
                    occupied = 1;
                } else {
                    occupied = 0;
                }
            } else {
                occupied = 1;
            }
            if (occupied) {
                lbAudioAx_80024030(2);
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x7 = 0;
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x5 = 0;
                idx = lbl_804799B8.x2 + lbl_804799B8.x3;
                tm->x37[idx].x3 = fn_8018F6DC(fn_8018F3BC(candidate));
                break;
            }
            step += 5;
        }
    }

    if (trigger & 0x100) {
        if ((fn_8018F6A8(4) & 0x40) && (fn_8018F6A8(4) & 0x20) &&
            gm_804771C4.match_type != 0)
        {
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            if (lbl_803D9D20.x72[tm->x37[idx].x3] == 0) {
                lbAudioAx_80024030(3);
                return;
            }
            lbAudioAx_80024030(1);
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            tm->x37[idx].x5 = 1;
            *state_ptr -= 1;
            return;
        }
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        if (lbl_803D9D20.x72[tm->x37[idx].x3] == 0) {
            lbAudioAx_80024030(3);
            return;
        }
        lbAudioAx_80024030(1);
        *state_ptr -= 1;
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        if (tm->x37[idx].x5 != 0) {
            tm->x37[idx].x5 = 0;
        }
    } else if (trigger & 0x200) {
        lbAudioAx_80024030(0);
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        tm->x37[idx].x3 = tm->x37[idx].x4;
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        tm->x37[idx].x7 = tm->x37[idx].x8;
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        tm->x37[idx].x5 = tm->x37[idx].x6;
        *state_ptr -= 1;
    } else if (trigger & 0x400) {
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        adj = (u8) gm_80169238((u8) fn_8018F6FC(tm->x37[idx].x3));
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        if ((s32) tm->x37[idx].x7 < adj - 1) {
            lbAudioAx_80024030(2);
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            tm->x37[idx].x7++;
        }
    } else if (trigger & 0x800) {
        idx = lbl_804799B8.x2 + lbl_804799B8.x3;
        if (tm->x37[idx].x7 != 0) {
            lbAudioAx_80024030(2);
            idx = lbl_804799B8.x2 + lbl_804799B8.x3;
            tm->x37[idx].x7--;
        }
    }
}

/// @todo Currently 93.5% match - permuter couldn't improve
/// Handles name entry/selection input for tournament mode.
void fn_80195AF0(s32* state_ptr, u32 buttons, u32 trigger)
{
    u8* ptr;
    u8* page_ptr;
    u8* row_ptr;
    u8* col_ptr;
    s32 cur_state;

    ptr = (u8*) &lbl_804799B8;

    if (trigger & 0x1000) {
        lbl_804D6654 = *state_ptr;
        *state_ptr = 0x11;
        return;
    }

    if (buttons & 0x40001) {
        if (*state_ptr == 0xE) {
            lbAudioAx_80024030(2);
            *state_ptr = 0xD;
        }
    } else if ((buttons & 0x80002) && (*state_ptr == 0xD)) {
        lbAudioAx_80024030(2);
        *state_ptr = 0xE;
    }

    if (trigger & 0x100) {
        u16* arr;

        lbAudioAx_80024030(1);
        col_ptr = &lbl_804799B8.x2;
        row_ptr = &lbl_804799B8.x3;
        arr = (u16*) ((u8*) state_ptr +
                      (lbl_804799B8.x2 + lbl_804799B8.x3) * 0x12);
        arr[0x21] = arr[0x20];
        cur_state = *state_ptr;
        switch (cur_state) {
        case 0xD:
            if (IsNameListFull() == 0) {
                TmData* tm = gm_8018F634();
                tm->x20 = 1;
                HSD_SisLib_803A5E70();
                mn_8022F138(0x19, 0x1C);
                mn_8022F138(0x12, 0x15);
                mnNameNew_8023EA08((UNK_T) lbl_804D6648);
            }
            return;
        case 0xE:
            page_ptr = &lbl_804799B8.x6;
            if (GetNameText(lbl_804799B8.x5 + (lbl_804799B8.x6 * 4)) != NULL) {
                s32 idx = (*col_ptr + *row_ptr) * 0x12;
                s16 val = lbl_804799B8.x5 + (*page_ptr * 4);
                ((u16*) ((u8*) state_ptr + idx))[0x20] = val;
            }
            *state_ptr = 0xF;
            fn_80190ABC(4);
            return;
        }
        return;
    }

    if (trigger & 0x200) {
        u16* arr;

        lbAudioAx_80024030(0);
        arr = (u16*) ((u8*) state_ptr +
                      (lbl_804799B8.x2 + lbl_804799B8.x3) * 0x12);
        arr[0x20] = arr[0x21];
        fn_80190ABC(5);
        fn_80190ABC(6);
        *state_ptr = 0xC;
    }
}

void fn_80195CCC(s32* arg, u32 buttons, u32 trigger)
{
    u8* ptr = (u8*) &lbl_804799B8;
    u16* arr;
    s32 selected;
    s32 slot;
    TmData* tm;
    u8 count;
    s32 i;
    s32 unique;

    PAD_STACK(16);

    if (buttons & 0x40001) {
        lbAudioAx_80024030(2);
        if ((ptr[5] % 4) != 0) {
            ptr[5] = (u8) (ptr[5] - 1);
            arr = (u16*) ((u8*) arg + (ptr[2] + ptr[3]) * 0x12);
            arr[0x20] = (s16) (ptr[5] + (ptr[6] * 4));
            fn_80190ABC(5);
            fn_80190ABC(4);
        }
    } else if (buttons & 0x80002) {
        lbAudioAx_80024030(2);
        if (((s32) (ptr[5] % 4) < 3) &&
            ((s32) (ptr[5] + ((ptr[6] * 4) + 1)) < GetNameCount()))
        {
            ptr[5] = (u8) (ptr[5] + 1);
            arr = (u16*) ((u8*) arg + (ptr[2] + ptr[3]) * 0x12);
            arr[0x20] = (s16) (ptr[5] + (ptr[6] * 4));
            fn_80190ABC(5);
            fn_80190ABC(4);
        }
    } else if (buttons & 0x10008) {
        lbAudioAx_80024030(2);
        if (((s32) ptr[5] / 4) != 0) {
            ptr[5] = (u8) (ptr[5] - 4);
            arr = (u16*) ((u8*) arg + (ptr[2] + ptr[3]) * 0x12);
            arr[0x20] = (s16) (ptr[5] + (ptr[6] * 4));
            fn_80190ABC(5);
            fn_80190ABC(4);
        } else if ((u8) ptr[6] != 0) {
            ptr[6] = (u8) (ptr[6] - 1);
            arr = (u16*) ((u8*) arg + (ptr[2] + ptr[3]) * 0x12);
            arr[0x20] = (s16) (ptr[5] + (ptr[6] * 4));
            fn_80190ABC(5);
            fn_80190ABC(4);
        }
    } else if (buttons & 0x20004) {
        lbAudioAx_80024030(2);
        if ((s32) ((s32) ptr[5] / 4) < 8) {
            if ((s32) (ptr[5] + ((ptr[6] * 4) + 4)) < GetNameCount()) {
                ptr[5] = (u8) (ptr[5] + 4);
                arr = (u16*) ((u8*) arg + (ptr[2] + ptr[3]) * 0x12);
                arr[0x20] = (s16) (ptr[5] + (ptr[6] * 4));
            }
            fn_80190ABC(5);
            fn_80190ABC(4);
        } else if ((s32) (ptr[5] + ((ptr[6] * 4) + 4)) < GetNameCount()) {
            ptr[6] = (u8) (ptr[6] + 1);
            arr = (u16*) ((u8*) arg + (ptr[2] + ptr[3]) * 0x12);
            arr[0x20] = (s16) (ptr[5] + (ptr[6] * 4));
            fn_80190ABC(5);
            fn_80190ABC(4);
        }
    }

    if (trigger & 0x100) {
        if (GetNameText(ptr[5] + (ptr[6] * 4)) != NULL) {
            selected = ptr[5] + (ptr[6] * 4);
            slot = ptr[2] + ptr[3];
            tm = gm_8018F634();
            count = tm->x2E;
            unique = 1;
            if ((s32) count > 0) {
                for (i = 0; i < (s32) count; i++) {
                    if ((i != slot) && (selected == (s32) tm->x37[i].x9)) {
                        unique = 0;
                        break;
                    }
                }
            }
            if (unique != 0) {
                lbAudioAx_80024030(1);
                arr = (u16*) ((u8*) arg +
                              (lbl_804799B8.x2 + lbl_804799B8.x3) * 0x12);
                arr[0x20] = (s16) (lbl_804799B8.x5 + (lbl_804799B8.x6 * 4));
                fn_80190ABC(5);
                fn_80190ABC(6);
                *arg = 0xC;
                return;
            }
        }
        lbAudioAx_80024030(3);
        return;
    }

    if (trigger & 0x200) {
        u16* arr2;

        lbAudioAx_80024030(0);
        arr2 = (u16*) ((u8*) arg + (lbl_804799B8.x2 + lbl_804799B8.x3) * 0x12);
        arr2[0x20] = arr2[0x21];
        fn_80190ABC(5);
        fn_80190ABC(6);
        *arg = 0xE;
    }
}

/// @todo Currently 96.88% match - needs minor register allocation fix
/// Handles confirm/cancel input for tournament start.
/// Type casts used to match target sth instruction patterns.
void fn_8019610C(s32* state, u32 buttons, u32 trigger)
{
    TmData* td;
    s32 i;
    u8* src_ptr;
    u8* dst_ptr;
    s32 base_val;

    i = 1;

    if (buttons & 0x10008) {
        if (*state == 0x12) {
            lbAudioAx_80024030(2);
            *state = 0x11;
        }
    } else if ((buttons & 0x20004) != 0) {
        if (*state == 0x11) {
            lbAudioAx_80024030(2);
            *state = 0x12;
        }
    }

    if ((trigger & 0x1100) != 0) {
        if (*state == 0x11) {
            lbAudioAx_80024030(i);
            if (gm_804771C4.match_type == 0) {
                gm_SetPendingScene(1);
            } else {
                td = gm_8018F634();
                i = 0;
                src_ptr = (u8*) td;
                dst_ptr = (u8*) td;
                base_val = i;
                while (i < td->x2E) {
                    src_ptr[0x45] = src_ptr[0x44];
                    src_ptr[0x46] = base_val;
                    if (i < td->x30) {
                        *(u16*) &dst_ptr[0x4BE] = *(u16*) &src_ptr[0x40];
                        dst_ptr[0x4BD] = src_ptr[0x39];
                        dst_ptr[0x4B9] = src_ptr[0x3A];
                        dst_ptr[0x4B8] = src_ptr[0x37];
                        dst_ptr[0x4BB] = src_ptr[0x3E];
                        dst_ptr[0x4BA] = src_ptr[0x3C];
                    }
                    src_ptr += 0x12;
                    dst_ptr += 0xA;
                    i++;
                }
                gm_SetPendingScene(2);
            }
            gm_801A4B60();
        } else {
            lbAudioAx_80024030(0);
            *state = lbl_804D6654;
        }
    } else if ((trigger & 0x200) != 0) {
        lbAudioAx_80024030(0);
        *state = lbl_804D6654;
    }
}

/// Tournament Mode per-frame update handler.
void gm_8019628C_OnFrame(void)
{
    TmData* tm;
    u32 r30;
    u32 r29;
    s32 cur_option;

    tm = gm_8018F634();
    r30 = fn_8018F674(4);
    r29 = fn_8018F640(4);

    if (mn_8022F218() != 0) {
        lbAudioAx_80024030(0);
        HSD_SisLib_803A5E70();
        mn_8022F138(1, 8);
        mn_8022F138(0x19, 0x1C);
        mn_8022F138(0x12, 0x15);
        mn_8022F268();
        gm_801A4B60();
        gm_801A42F8(1);
        return;
    }

    if (tm->x20 != 0) {
        return;
    }

    lbl_804799B8.xA = (u8) ((lbl_804799B8.xA + 1) % 11);

    cur_option = tm->cur_option;
    if (cur_option >= 9 && cur_option <= 9) {
        return;
    }
    if (cur_option >= 0x13) {
        return;
    }
    lbl_803D9FD8[cur_option]((s32*) tm, r30, r29);
}

extern struct {
    UNK_T x0;
    UNK_T x4;
    UNK_T x8;
} lbl_804771B8;

void gm_801963B4_OnEnter(void* arg0)
{
    const char* filename;
    lbAudioAx_80026F2C(0x12);
    lbAudioAx_8002702C(2, 8);
    lbAudioAx_80027168();
    lbAudioAx_80027648();
    lbl_804D6640 = lbArchive_80016DBC("GmTou1p", &lbl_804D664C,
                                      "ScGamTour_scene_data", 0);
    lbl_804D6644 = lbArchive_80016DBC("GmTou2p", &lbl_804D6650,
                                      "ScGamTour_scene_data", 0);
    lbl_804D6648 = lbArchive_LoadArchive("MnExtAll");
    filename = "TmBox.dat";
    lbl_804D6638 = lbArchive_80016DBC(
        filename, &lbl_804771B8.x0, "tournament_box2_array", &lbl_804771B8.x4,
        "tournament_box3_array", &lbl_804771B8.x8, "tournament_box4_array", 0);
    HSD_SisLib_803A62A0(0, fn_8018F5F0(), "SIS_TournamentData");
    fn_801935B8();
    lbAudioAx_800237A8(0x7539, 0x7F, 0x40);
    lbAudioAx_80023F28(gmMainLib_8015ECB0());
}

/// Scene OnLeave callback for gm_18A5. Frees archive resources.
void gm_801964A4_OnLeave(UNK_T arg)
{
    (void) arg;
    lbArchive_80016EFC(lbl_804D6640);
    lbArchive_80016EFC(lbl_804D6644);
    lbArchive_80016EFC(lbl_804D6648);
    lbArchive_80016EFC(lbl_804D6638);
    gm_8018F634()->cur_option = 0x13;
    gm_8018F634()->x2C = 1;
    gm_8018F634()->x31 = lbl_804799B8.x0;
}
