#include "gm_1832.h"

#include "gm_1B03.static.h"

#include "gm_unsplit.h"

#include "pl/pl_040D.h"

#include <sysdolphin/baselib/aobj.h>
#include <sysdolphin/baselib/cobj.h>
#include <sysdolphin/baselib/dobj.h>
#include <sysdolphin/baselib/fog.h>
#include <sysdolphin/baselib/gobjgxlink.h>
#include <sysdolphin/baselib/gobjobject.h>
#include <sysdolphin/baselib/gobjplink.h>
#include <sysdolphin/baselib/gobjproc.h>
#include <sysdolphin/baselib/mobj.h>
#include <sysdolphin/baselib/random.h>
#include <sysdolphin/baselib/sislib.h>
#include <sysdolphin/baselib/sobjlib.h>
#include <sysdolphin/baselib/state.h>
#include <sysdolphin/baselib/tobj.h>
#include <sysdolphin/baselib/util.h>
#include <sysdolphin/baselib/wobj.h>
#include <melee/cm/camera.h>
#include <melee/ef/efasync.h>
#include <melee/ef/eflib.h>
#include <melee/ft/ft_0877.h>
#include <melee/ft/ftbosslib.h>
#include <melee/ft/ftdemo.h>
#include <melee/ft/ftlib.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_1A36.h>
#include <melee/gm/gm_1A3F.h>
#include <melee/gm/gm_1A45.h>
#include <melee/gm/gmmain_lib.h>
#include <melee/gm/types.h>
#include <melee/gr/ground.h>
#include <melee/gr/grpushon.h>
#include <melee/gr/stage.h>
#include <melee/if/ifall.h>
#include <melee/if/ifstatus.h>
#include <melee/it/it_266F.h>
#include <melee/it/item.h>
#include <melee/lb/lb_00B0.h>
#include <melee/lb/lb_00F9.h>
#include <melee/lb/lb_0192.h>
#include <melee/lb/lbarchive.h>
#include <melee/lb/lbaudio_ax.h>
#include <melee/lb/lbbgflash.h>
#include <melee/lb/lblanguage.h>
#include <melee/mn/mnname.h>
#include <melee/mn/types.h>
#include <melee/mp/mpcoll.h>
#include <melee/pl/player.h>
#include <melee/sc/types.h>
#include <Runtime/runtime.h>

#if defined(__MWERKS__) && !defined(M2CTX)
#define SECTION_SDATA2 __declspec(section ".sdata2")
#else
#define SECTION_SDATA2
#endif

extern int lbl_804D6608;

typedef struct {
    s32 v[6];
} ClassicProcArray;

extern ClassicProcArray lbl_803B7C40;
extern ClassicProcArray lbl_803B7C28;

/// .sbss zero-init globals emit in REVERSE declaration order; the swapped
/// spelling below yields target order (lbl_804D6628 at +0).
DynamicModelDesc** lbl_804D662C;
HSD_Archive* lbl_804D6628;

/// Item menu table: 31 (s16 item_kind, s16 sis_id) pairs (target bytes;
/// the strings + stage table below complete this unit's .data stream).
u8 lbl_803D9828[0x7C] = {
    0x00, 0x12, 0x00, 0x15, 0x00, 0x09, 0x00, 0x0B, 0x00, 0x08, 0x00, 0x0A,
    0x00, 0x10, 0x00, 0x12, 0x00, 0x19, 0x00, 0x23, 0x00, 0x15, 0x00, 0x19,
    0x00, 0x16, 0x00, 0x1A, 0x00, 0x17, 0x00, 0x13, 0x00, 0x0C, 0x00, 0x0E,
    0x00, 0x0B, 0x00, 0x0D, 0x00, 0x18, 0x00, 0x02, 0x00, 0x1C, 0x00, 0x26,
    0x00, 0x0E, 0x00, 0x10, 0x00, 0x0F, 0x00, 0x11, 0x00, 0x14, 0x00, 0x18,
    0x00, 0x11, 0x00, 0x14, 0x00, 0x07, 0x00, 0x09, 0x00, 0x22, 0x00, 0x1B,
    0x00, 0x06, 0x00, 0x08, 0x00, 0x13, 0x00, 0x16, 0x00, 0x1A, 0x00, 0x4E,
    0x00, 0x1B, 0x00, 0x4F, 0x00, 0x0A, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x0F,
    0x00, 0x1D, 0x00, 0x4C, 0x00, 0x20, 0x00, 0x51, 0x00, 0x1F, 0x00, 0x4D,
    0x00, 0x1E, 0x00, 0x50, 0x00, 0x05, 0x00, 0x05, 0x00, 0x21, 0x00, 0x52,
    0xFC, 0x19, 0x00, 0x17
};

extern u8 lbl_803D9DD0[];

/// Training mode speed multipliers (x2, x1.5, x1, x2/3, x1/2, x1/4).
const f32 lbl_803B7C68[6] = { 2.0F, 1.5F, 1.0F, 0.666F, 0.5F, 0.25F };

const s32 lbl_803B7C80[10] = { 30, -20, 15, -12, 10, -8, 6, -4, 2, 1 };

typedef struct TrainingCamDesc {
    s32 x00, x04, x08, x0C, x10, x14;
    void* x18;
    void* x1C;
    s32 x20, x24;
    f32 x28, x2C, x30, x34;
} TrainingCamDesc;
STATIC_ASSERT(sizeof(TrainingCamDesc) == 0x38);

const TrainingCamDesc lbl_803B7CA8 = {
    0,    1,         640,   480, 640, 480, &lbl_803D9DD0[0x24],
    &lbl_803D9DD0[0x38], 0, 0,   0.1F, 30000.0F, 60.0F,
    1.3333332538604736F, /* 0x3FAAAAAA */
};

const s32 lbl_803B7CE0[9] = { 0x1E,       0x32, 0x003B0000,
                              0x003C0046, 0x0001005A, 0x00640000,
                              0x006E0082, 0x96, 0x00A00000 };

const u8 lbl_803B7D04[0x14] = { 0 };

typedef struct {
    /* 0x00 */ f32 vals[3];
} ClassicSlotVals;
STATIC_ASSERT(sizeof(ClassicSlotVals) == 0xC);

typedef struct {
    /* 0x00 */ f32 x00;
    /* 0x04 */ f32 x04;
    /* 0x08 */ Vec3 x08;
    /* 0x14 */ u8 pad_14[0x8];
} ClassicCharLayout;
STATIC_ASSERT(sizeof(ClassicCharLayout) == 0x1C);

typedef struct {
    /* 0x00 */ f32 x00;
    /* 0x04 */ f32 x04;
    /* 0x08 */ f32 x08;
    /* 0x0C */ u8 pad_0C[0x8];
} ClassicTeamEntry;
STATIC_ASSERT(sizeof(ClassicTeamEntry) == 0x14);

typedef struct {
    /* 0x00 */ f32 x00[3];
    /* 0x0C */ f32 x0C[3];
    /* 0x18 */ f32 x18[3];
    /* 0x24 */ f32 x24[3];
} ClassicSplashRow;
STATIC_ASSERT(sizeof(ClassicSplashRow) == 0x30);

/// Classic mode stage data table entry (size 0x10)
/// Table has 65 entries: 13 stages x 5 difficulty levels
typedef struct ClassicStageEntry {
    /* 0x00 */ u8 stage_kind;
    /* 0x01 */ u8 pad_1;
    /* 0x02 */ u16 scale0_pct; ///< Divided by 100 to get ratio
    /* 0x04 */ u16 scale1_pct; ///< Divided by 100 to get ratio
    /* 0x06 */ u8 pad_6[0xA];
} ClassicStageEntry;
STATIC_ASSERT(sizeof(ClassicStageEntry) == 0x10);

extern ClassicStageEntry lbl_803D9910[65];


typedef struct {
    u8 b7 : 1, b6 : 1, b5 : 1, b4 : 1, b3 : 1, b2 : 1, b1 : 1, b0 : 1;
} u8_bits;


/// Stays static: the target splits this region into THREE globals
/// (lbl_80473700 0x114 / gm_80473814 0x204 / gm_80473A18 0xA0) and its
/// relocs are spelled against the split names; a global merged def would
/// name-mismatch every css access in matched fns (idiom-139 limit).
static TrainingModeState lbl_80473700;
UnkAllstarData gm_80473A18;

SECTION_SDATA2 f32 lbl_804DA608 = 10.0F;
SECTION_SDATA2 f32 lbl_804DA60C = 1.0F;
extern f32 lbl_804DA634;
extern const f32 lbl_804DA63C;

int gm_80188454(int idx)
{
    return lbl_80473700.char_data[idx];
}

int fn_8018846C(void)
{
    int result;
    int* ptr = (int*) &lbl_80473700;
    int* p;

    result = pl_8004134C(0);
    p = ptr + Player_GetPlayerCharacter(0) + 2;
    if (*p < result) {
        *p = result;
    }
    if (result != 0) {
        ptr[67] = result;
        ptr[68] = 1;
    }
    if (ptr[68] != 0) {
        return ptr[67];
    }
    return result;
}

inline int fn_801884F8_inline(void)
{
    int result;
    TrainingModeState* state = &lbl_80473700;

    result = pl_80041300(0);
    if (result != 0) {
        state->result_cache[0] = result;
        state->result_cache[1] = 1;
    }
    if (state->result_cache[1] != 0) {
        return state->result_cache[0];
    }
    return result;
}

#pragma push
#pragma dont_inline on
int fn_801884F8(void)
{
    int result;
    TrainingModeState* state = &lbl_80473700;

    result = pl_80041300(0);
    if (result != 0) {
        state->result_cache[0] = result;
        state->result_cache[1] = 1;
    }
    if (state->result_cache[1] != 0) {
        result = state->result_cache[0];
    }
    return result;
}
#pragma pop
void fn_80188550(int arg0)
{
    int current = lbl_80473700.count;

    if (arg0 != current) {
        if (arg0 > lbl_80473700.count) {
            int i = 0;
            int skip = lbl_80473700.count;
            int remaining = arg0 - current;

            for (i = 0; i < 4; i++) {
                if (i != 0) {
                    if (skip == 0) {
                        if (i != 0) {
                            lbl_80473700.players[i].slot_type = 1;
                        } else {
                            lbl_80473700.players[i].slot_type = 0;
                        }
                        gm_8016EDDC(i, &lbl_80473700.players[i]);
                        if (--remaining == 0) {
                            break;
                        }
                    } else {
                        skip--;
                    }
                }
            }
        } else {
            int to_remove = current - arg0;
            int j;
            for (j = 3; j >= 0; j--) {
                if (j != 0 && to_remove != 0) {
                    if (current != 3) {
                        fn_8016EF98(to_remove + 1);
                    } else {
                        fn_8016EF98(j);
                    }
                    if (--to_remove == 0) {
                        break;
                    }
                }
            }
        }
        lbl_80473700.count = arg0;
    }
}

void fn_80188644(void)
{
    int i;
    Vec3 sp10;
    int saved_count;

    PAD_STACK(8);

    saved_count = lbl_80473700.count;
    lbl_80473700.count = 1;
    Player_SetFacingDirection(0, lbl_804DA60C);
    Player_SetHUDDamage(0, 0);
    Stage_80224E64(0, &sp10);
    Player_800328D4(0, &sp10);
    ifStatus_802F6508(0);

    i = 0;
    do {
        if (i != 0 && i <= saved_count) {
            fn_8016EF98(i);
            if (i == 1) {
                if (i != 0) {
                    lbl_80473700.players[i].slot_type = 1;
                } else {
                    lbl_80473700.players[i].slot_type = 0;
                }
                gm_8016EDDC(i, &lbl_80473700.players[i]);
            }
        }
        i++;
    } while (i < 4);

    lbl_80473700.css.menu_values[0] = 0;
    lbl_80473700.css.menu_values[1] = 0;
    lbl_80473700.css.menu_values[2] = 0;
    lbl_80473700.css.menu_values[3] = 0;
    saved_count = 0;
    lbl_80473700.css.menu_values[4] = saved_count;
    lbl_80473700.css.menu_values[5] = saved_count;
    lbl_80473700.css.menu_values[6] = saved_count;
    lbl_80473700.css.menu_values[saved_count] = 2;
}

void fn_80188738(HSD_JObj* arg0)
{
    HSD_JObj* jobjs[3];
    int i;
    int val;

    if (pl_800412D0(0) > 999) {
        val = 999;
    } else {
        val = pl_800412D0(0);
    }

    jobjs[2] = arg0;
    jobjs[1] = (jobjs[2] == NULL) ? NULL : jobjs[2]->next;
    jobjs[0] = (jobjs[1] == NULL) ? NULL : jobjs[1]->next;

    if ((val / 100) != 0) {
        int q = val + 0;
        HSD_JObjReqAnimAll(jobjs[0], (f32) (q / 100));
    } else {
        HSD_JObjReqAnimAll(jobjs[0], lbl_804DA608);
    }

    if ((val / 10) != 0) {
        HSD_JObjReqAnimAll(jobjs[1], (f32) ((val % 100) / 10));
    } else {
        HSD_JObjReqAnimAll(jobjs[1], lbl_804DA608);
    }

    HSD_JObjReqAnimAll(jobjs[2], (f32) (val % 10));

    for (i = 0; i < 3; i++) {
        HSD_JObjAnimAll(jobjs[i]);
    }
}

static int fn_8018846C_noInline(void)
{
    return fn_8018846C();
}

void fn_80188910(HSD_JObj* arg0)
{
    HSD_JObj* jobjs[3];
    int i;
    int val;

    if (fn_8018846C() > 999) {
        val = 999;
    } else {
        val = fn_8018846C_noInline();
    }

    jobjs[2] = arg0;
    jobjs[1] = (jobjs[2] == NULL) ? NULL : jobjs[2]->next;
    jobjs[0] = (jobjs[1] == NULL) ? NULL : jobjs[1]->next;

    if ((val / 100) != 0) {
        int q = val + 0;
        HSD_JObjReqAnimAll(jobjs[0], (f32) (q / 100));
    } else {
        HSD_JObjReqAnimAll(jobjs[0], lbl_804DA608);
    }
    if ((val / 10) != 0) {
        HSD_JObjReqAnimAll(jobjs[1], (f32) ((val % 100) / 10));
    } else {
        HSD_JObjReqAnimAll(jobjs[1], lbl_804DA608);
    }
    HSD_JObjReqAnimAll(jobjs[2], (f32) (val % 10));

    for (i = 0; i < 3; i++) {
        HSD_JObjAnimAll(jobjs[i]);
    }
}

void fn_80188B3C(HSD_JObj* arg0)
{
    HSD_JObj* jobjs[3];
    int i;
    int val;

    if (fn_801884F8_inline() > 999) {
        val = 999;
    } else {
        val = fn_801884F8();
    }

    jobjs[2] = arg0;
    jobjs[1] = (jobjs[2] == NULL) ? NULL : jobjs[2]->next;
    jobjs[0] = (jobjs[1] == NULL) ? NULL : jobjs[1]->next;

    if ((val / 100) != 0) {
        int q = val + 0;
        HSD_JObjReqAnimAll(jobjs[0], (f32) (q / 100));
    } else {
        HSD_JObjReqAnimAll(jobjs[0], lbl_804DA608);
    }
    if ((val / 10) != 0) {
        HSD_JObjReqAnimAll(jobjs[1], (f32) ((val % 100) / 10));
    } else {
        HSD_JObjReqAnimAll(jobjs[1], lbl_804DA608);
    }
    HSD_JObjReqAnimAll(jobjs[2], (f32) (val % 10));

    for (i = 0; i < 3; i++) {
        HSD_JObjAnimAll(jobjs[i]);
    }
}

void fn_80188D3C(HSD_JObj* arg0)
{
    HSD_JObj* jobjs[3];
    int i;
    int val;
    int hundreds;

    val = gm_80473814.x144[0x19];

    jobjs[2] = (arg0 == NULL) ? NULL : arg0->child;
    jobjs[1] = (jobjs[2] == NULL) ? NULL : jobjs[2]->next;
    jobjs[0] = (jobjs[1] == NULL) ? NULL : jobjs[1]->next;

    hundreds = val / 100;
    if (hundreds != 0) {
        HSD_JObjReqAnimAll(jobjs[0], (f32) hundreds);
    } else {
        HSD_JObjReqAnimAll(jobjs[0], lbl_804DA608);
    }

    if ((val / 10) != 0) {
        HSD_JObjReqAnimAll(jobjs[1], (f32) ((val - (hundreds * 100)) / 10));
    } else {
        HSD_JObjReqAnimAll(jobjs[1], lbl_804DA608);
    }

    HSD_JObjReqAnimAll(jobjs[2], (f32) (val % 10));

    for (i = 0; i < 3; i++) {
        HSD_JObjAnimAll(jobjs[i]);
    }
}

void fn_80188EE8(HSD_GObj* gobj)
{
    TrainingModeState* state;
    CssSubStruct* sub;
    s32* menu;
    HSD_JObj* jobj;
    HSD_Text* text;
    s32 val;

    PAD_STACK(8);

    state = &lbl_80473700;
    state = (TrainingModeState*) ((u8*) state + 0);
    sub = &state->css;

    if (gm_801A45E8(2) != 0) {
        HSD_SisLib_803A6368(sub->text, 0x1E);
        HSD_JObjSetFlagsAll(sub->jobjs[3], 0x10);
    } else {
        HSD_JObjClearFlagsAll(sub->jobjs[3], 0x10);
    }

    if ((u32) sub->anim_frames[22] < 10 ||
        ((u32) sub->anim_frames[22] >= 20 && (u32) sub->anim_frames[22] < 30))
    {
        sub->anim_frames[22]++;
        sub->anim_frames[1] = sub->anim_frames[22];
    }

    HSD_JObjReqAnimAll(sub->jobjs[22], (f32) (u32) sub->anim_frames[22]);
    HSD_JObjAnimAll(sub->jobjs[22]);
    HSD_JObjReqAnimAll(sub->jobjs[1], (f32) (u32) sub->anim_frames[1]);
    HSD_JObjAnimAll(sub->jobjs[1]);

    sub->text->pos_x =
        (12.0f * (9.798828f + HSD_JObjGetTranslationX(sub->jobjs[1]))) + 50.0f;
    sub->text->pos_y = 150.0f;

    fn_80188738(sub->jobjs[9]);
    fn_80188910(sub->jobjs[5]);
    fn_80188B3C(sub->jobjs[19]);

    menu = state->css.menu_values;
    menu = (s32*) ((u8*) menu + 0);

    jobj = sub->jobjs[12];
    HSD_JObjReqAnimAll(jobj, (f32) state->css.x03);
    HSD_JObjAnimAll(jobj);

    jobj = sub->jobjs[13];
    HSD_JObjReqAnimAll(jobj, (f32) (u32) state->css.menu_values[0]);
    HSD_JObjAnimAll(jobj);

    jobj = sub->jobjs[23];
    HSD_JObjReqAnimAll(jobj, (f32) state->css.x00);
    HSD_JObjAnimAll(jobj);

    jobj = sub->jobjs[34];
    HSD_JObjReqAnimAll(jobj, (f32) (u32) menu[0]);
    HSD_JObjAnimAll(jobj);

    val = state->css.menu_values[1];
    text = state->css.text;
    if (lbLang_IsSettingUS() != 0 && val == 0x13) {
        HSD_SisLib_803A6368(text, 0x17);
    } else {
        HSD_SisLib_803A6368(text,
                            (s32) ((s16*) &lbl_803D9828[val * 4])[1]);
    }

    jobj = sub->jobjs[32];
    HSD_JObjReqAnimAll(jobj, (f32) (u32) (state->css.menu_values[2] + 1));
    HSD_JObjAnimAll(jobj);

    jobj = sub->jobjs[31];
    HSD_JObjReqAnimAll(jobj, (f32) (u32) state->css.menu_values[3]);
    HSD_JObjAnimAll(jobj);

    fn_80188D3C(sub->jobjs[27]);

    HSD_JObjSetFlags(sub->jobjs[25], 0x10);

    val = state->css.menu_values[6];
    jobj = sub->jobjs[26];
    if (val == 2 && state->mode == 3) {
        val = 3;
    }
    HSD_JObjReqAnimAll(jobj, (f32) val);
    HSD_JObjAnimAll(jobj);
}

#pragma dont_inline on
void fn_801891F4(void)
{
    CssSubStruct* sub = &lbl_80473700.css;
    u64 buttons;
    int i;
    int count;

    buttons = gm_801A36C0((u8) lbl_80473700.mode);

    if (gm_801A45E8(2) != 0) {
        if (sub->x01 == 0) {
            fn_801651FC(0, 0);
            count = lbl_80473700.count;
            sub->x03 = 0;
            for (i = 0; i < 4; i++) {
                if (i != 0 && count != 0) {
                    Player_SetPlayerAndEntityCpuType(i, 0);
                    count--;
                    if (count == 0) {
                        break;
                    }
                }
            }
            sub->anim_frames[22] = 0;
        }
        sub->x01 = 1;

        if (buttons & 0x1000000000ULL) {
            lbAudioAx_80024030(2);
            if ((u8) sub->x00 != 0) {
                sub->x00--;
                if (sub->x00 == 5) {
                    sub->x00 = 4;
                }
            } else {
                sub->x00 = 8;
            }
        }

        if (buttons & 0x2000000000ULL) {
            lbAudioAx_80024030(2);
            if ((u8) sub->x00 < 8) {
                sub->x00++;
                if (sub->x00 == 5) {
                    sub->x00 = 6;
                }
            } else {
                sub->x00 = 0;
            }
        }

        switch (sub->x00) {
        case 0:
            if (buttons & 0x4000000000ULL) {
                lbAudioAx_80024030(2);
                if (sub->menu_values[sub->x00] != 0) {
                    sub->menu_values[sub->x00]--;
                    return;
                }
                sub->menu_values[sub->x00] = 5;
                return;
            }
            if (buttons & 0x8000000000ULL) {
                lbAudioAx_80024030(2);
                if ((u32) sub->menu_values[sub->x00] < 5) {
                    sub->menu_values[sub->x00]++;
                    return;
                }
                sub->menu_values[sub->x00] = 0;
                return;
            }
            break;
        case 1:
            if (buttons & 0x4000000000ULL) {
                lbAudioAx_80024030(2);
                if (sub->menu_values[sub->x00] != 0) {
                    sub->menu_values[sub->x00]--;
                } else {
                    sub->menu_values[sub->x00] = 0x1D;
                }
            } else if (buttons & 0x8000000000ULL) {
                lbAudioAx_80024030(2);
                if ((u32) sub->menu_values[sub->x00] < 0x1D) {
                    sub->menu_values[sub->x00]++;
                } else {
                    sub->menu_values[sub->x00] = 0;
                }
            }
            if (buttons & 0x100ULL) {
                Vec3 pos;
                s16 item;
                HSD_JObj* jobj;
                lbAudioAx_80024030(8);
                item = *(s16*) &((s32*) lbl_803D9828)[sub->menu_values[1]];
                jobj = Player_GetEntity(0)->hsd_obj;
                /// Binary-proven: the target's 979-assert pools the
                /// "jobj.h"/"jobj" strings (no per-TU __FILE__ string).
                ((jobj) ? ((void) 0) : __assert("jobj.h", 979, "jobj"));
                pos = jobj->translate;
                pos.y += lbl_804DA608;
                it_8026D258(&pos, (ItemKind) item);
                return;
            }
            break;
        case 2:
            if (buttons & 0x4000000000ULL) {
                lbAudioAx_80024030(2);
                if (sub->menu_values[sub->x00] != 0) {
                    sub->menu_values[sub->x00]--;
                    return;
                }
                sub->menu_values[sub->x00] = 2;
                return;
            }
            if (buttons & 0x8000000000ULL) {
                lbAudioAx_80024030(2);
                if ((u32) sub->menu_values[sub->x00] < 2) {
                    sub->menu_values[sub->x00]++;
                    return;
                }
                sub->menu_values[sub->x00] = 0;
                return;
            }
            break;
        case 3:
            if (buttons & 0x4000000000ULL) {
                lbAudioAx_80024030(2);
                if (sub->menu_values[sub->x00] != 0) {
                    sub->menu_values[sub->x00]--;
                    return;
                }
                sub->menu_values[sub->x00] = 5;
                return;
            }
            if (buttons & 0x8000000000ULL) {
                lbAudioAx_80024030(2);
                if ((u32) sub->menu_values[sub->x00] < 5) {
                    sub->menu_values[sub->x00]++;
                    return;
                }
                sub->menu_values[sub->x00] = 0;
                return;
            }
            break;
        case 4:
            if (buttons & 0x4000000000ULL) {
                lbAudioAx_80024030(2);
                if (sub->menu_values[sub->x00] != 0) {
                    sub->menu_values[sub->x00]--;
                    if (sub->menu_values[sub->x00] == 0) {
                        gm_801A36E0(0, 0x19);
                        return;
                    }
                } else {
                    sub->menu_values[sub->x00] = 0x3E7;
                    return;
                }
            } else if (buttons & 0x8000000000ULL) {
                lbAudioAx_80024030(2);
                if ((u32) sub->menu_values[sub->x00] < 0x3E7) {
                    sub->menu_values[sub->x00]++;
                    if (sub->menu_values[sub->x00] == 0x3E7) {
                        gm_801A36E0(0, 0x19);
                        return;
                    }
                } else {
                    sub->menu_values[sub->x00] = 0;
                    return;
                }
            }
            break;
        case 5:
            if (buttons & 0x4000000000ULL) {
                lbAudioAx_80024030(2);
                sub->menu_values[sub->x00] = 0;
                return;
            }
            if (buttons & 0x8000000000ULL) {
                lbAudioAx_80024030(2);
                sub->menu_values[sub->x00] = 1;
                return;
            }
            break;
        case 6:
            if (buttons & 0x4000000000ULL) {
                lbAudioAx_80024030(2);
                if (sub->menu_values[sub->x00] != 0) {
                    sub->menu_values[sub->x00]--;
                } else {
                    sub->menu_values[sub->x00] = 2;
                }
                switch (sub->menu_values[sub->x00]) {
                case 0:
                    fn_801650E8();
                    return;
                case 1:
                    fn_80165190(0, 5);
                    return;
                case 2:
                    if (lbl_80473700.mode == 3) {
                        fn_801652B0(0, 0);
                        return;
                    }
                    fn_801652B0(0, 3);
                    return;
                }
            } else if (buttons & 0x8000000000ULL) {
                lbAudioAx_80024030(2);
                if ((u32) sub->menu_values[sub->x00] < 2) {
                    sub->menu_values[sub->x00]++;
                } else {
                    sub->menu_values[sub->x00] = 0;
                }
                switch (sub->menu_values[sub->x00]) {
                case 0:
                    fn_801650E8();
                    return;
                case 1:
                    fn_80165190(0, 5);
                    return;
                case 2:
                    if (lbl_80473700.mode == 3) {
                        fn_801652B0(0, 0);
                        return;
                    }
                    fn_801652B0(0, 3);
                    return;
                }
            }
            break;
        case 7:
            if (buttons & 0x100ULL) {
                lbAudioAx_80024030(1);
                fn_80188644();
                return;
            }
            break;
        case 8:
            if (buttons & 0x100ULL) {
                gm_8016B328();
                return;
            }
            break;
        }
    } else {
        if (sub->x01 == 1) {
            ClassicProcArray speeds;
            int cpu_type;
            int damage;
            PAD_STACK(40);

            speeds = *(ClassicProcArray*) lbl_803B7C68;

            lbAudioAx_80024030(0);
            sub->anim_frames[22] = 0x14;
            lb_80019880(__cvt_dbl_usll(
                (f64) (0.016666668f / ((f32*) speeds.v)[sub->menu_values[0]] *
                       (f32) (*(u32*) 0x800000F8 >> 2))));

            fn_80188550(sub->menu_values[2] + 1);

            cpu_type = sub->menu_values[3];
            count = lbl_80473700.count;
            sub->x03 = (u8) cpu_type;
            for (i = 0; i < 4; i++) {
                if (i != 0 && count != 0) {
                    Player_SetPlayerAndEntityCpuType(i, cpu_type);
                    count--;
                    if (count == 0) {
                        break;
                    }
                }
            }

            damage = sub->menu_values[4];
            count = lbl_80473700.count;
            for (i = 0; i < 4; i++) {
                if (i != 0 && count != 0) {
                    Player_SetHUDDamage(i, damage);
                    count--;
                    if (count == 0) {
                        break;
                    }
                }
            }

            for (i = 1; i < 4; i++) {
                fn_8016B388(i, sub->menu_values[4]);
            }

            switch (sub->menu_values[6]) {
            case 0:
                fn_801650E8();
                break;
            case 1:
                fn_80165190(0, 5);
                break;
            case 2:
                if (lbl_80473700.mode == 3) {
                    fn_801652B0(0, 0);
                } else {
                    fn_801652B0(0, 3);
                }
                break;
            }
        }
        sub->x01 = 0;
    }
}
#pragma dont_inline off

/// SIS/archive name strings (numeric u8 spelling = zero @-ids, idiom 98).
u8 lbl_803D98C8[0x1B] = { /* "ScGamTraining_scene_models" */
    0x53, 0x63, 0x47, 0x61, 0x6D, 0x54, 0x72, 0x61, 0x69, 0x6E, 0x69, 0x6E,
    0x67, 0x5F, 0x73, 0x63, 0x65, 0x6E, 0x65, 0x5F, 0x6D, 0x6F, 0x64, 0x65,
    0x6C, 0x73, 0x00
};
u8 lbl_803D98E4[0xC] = { /* "SdTrain.usd" */
    0x53, 0x64, 0x54, 0x72, 0x61, 0x69, 0x6E, 0x2E, 0x75, 0x73, 0x64, 0x00
};
u8 lbl_803D98F0[0x11] = { /* "SIS_TrainingData" */
    0x53, 0x49, 0x53, 0x5F, 0x54, 0x72, 0x61, 0x69, 0x6E, 0x69, 0x6E, 0x67,
    0x44, 0x61, 0x74, 0x61, 0x00
};
u8 lbl_803D9904[0xC] = { /* "SdTrain.dat" */
    0x53, 0x64, 0x54, 0x72, 0x61, 0x69, 0x6E, 0x2E, 0x64, 0x61, 0x74, 0x00
};

/// 65 = 13 stages x 5 difficulty levels (target bytes).
ClassicStageEntry lbl_803D9910[65] = {
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x00, 0x01, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0046, 0x008C, { 0x01, 0x01, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0050, 0x0078, { 0x03, 0x01, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x02, 0x00, 0x005A, 0x0064, { 0x05, 0x01, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x01, 0x00, 0x0064, 0x005F, { 0x07, 0x01, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x01, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0046, 0x008C, { 0x03, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0050, 0x0073, { 0x04, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x02, 0x00, 0x005A, 0x0064, { 0x06, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x01, 0x00, 0x0064, 0x005F, { 0x08, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0046, 0x008C, { 0x04, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0050, 0x006E, { 0x05, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x02, 0x00, 0x005A, 0x0064, { 0x07, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x01, 0x00, 0x0064, 0x005F, { 0x09, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x03, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0046, 0x008C, { 0x05, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0050, 0x0078, { 0x06, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x02, 0x00, 0x005A, 0x0069, { 0x08, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x01, 0x00, 0x0064, 0x005F, { 0x09, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x00, 0x02, 0x17, 0x00, 0x02, 0x17, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0043, 0x008C, { 0x02, 0x02, 0x17, 0x02, 0x02, 0x17, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x004E, 0x0078, { 0x03, 0x02, 0x17, 0x03, 0x02, 0x17, 0x00, 0x00, 0x00, 0x00 } },
    { 0x02, 0x00, 0x005A, 0x0069, { 0x05, 0x02, 0x04, 0x04, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x01, 0x00, 0x0064, 0x0064, { 0x06, 0x02, 0x04, 0x06, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x01, 0x03, 0x17, 0x01, 0x03, 0x17, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0043, 0x008C, { 0x03, 0x03, 0x04, 0x03, 0x03, 0x17, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x004E, 0x0078, { 0x04, 0x03, 0x04, 0x03, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x02, 0x00, 0x005A, 0x0069, { 0x05, 0x03, 0x04, 0x05, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x01, 0x00, 0x0064, 0x0064, { 0x07, 0x03, 0x04, 0x07, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x02, 0x01, 0x17, 0x01, 0x01, 0x17, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0046, 0x008C, { 0x03, 0x01, 0x04, 0x03, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x004E, 0x0078, { 0x04, 0x01, 0x04, 0x04, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x02, 0x00, 0x005A, 0x0069, { 0x06, 0x01, 0x04, 0x05, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x01, 0x00, 0x0066, 0x0064, { 0x08, 0x01, 0x04, 0x07, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x02, 0x02, 0x04, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0046, 0x0082, { 0x03, 0x02, 0x04, 0x03, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0050, 0x006E, { 0x05, 0x02, 0x04, 0x04, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x02, 0x00, 0x005A, 0x0069, { 0x07, 0x02, 0x04, 0x05, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x01, 0x00, 0x0069, 0x0064, { 0x09, 0x02, 0x04, 0x08, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x00, 0x03, 0x17, 0x00, 0x03, 0x17, 0x00, 0x03, 0x17, 0x00 } },
    { 0x03, 0x00, 0x0043, 0x0082, { 0x02, 0x03, 0x17, 0x02, 0x03, 0x17, 0x01, 0x03, 0x17, 0x00 } },
    { 0x03, 0x00, 0x004E, 0x006E, { 0x03, 0x03, 0x17, 0x03, 0x03, 0x17, 0x03, 0x03, 0x17, 0x00 } },
    { 0x02, 0x00, 0x0059, 0x0064, { 0x04, 0x03, 0x04, 0x04, 0x03, 0x17, 0x04, 0x03, 0x17, 0x00 } },
    { 0x01, 0x00, 0x0064, 0x0064, { 0x06, 0x03, 0x04, 0x06, 0x03, 0x04, 0x06, 0x03, 0x17, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x01, 0x01, 0x17, 0x01, 0x01, 0x17, 0x01, 0x01, 0x17, 0x00 } },
    { 0x03, 0x00, 0x0045, 0x007D, { 0x02, 0x01, 0x17, 0x02, 0x01, 0x17, 0x02, 0x01, 0x17, 0x00 } },
    { 0x03, 0x00, 0x004B, 0x006E, { 0x04, 0x01, 0x11, 0x04, 0x01, 0x17, 0x03, 0x01, 0x17, 0x00 } },
    { 0x02, 0x00, 0x0059, 0x0064, { 0x05, 0x01, 0x04, 0x04, 0x01, 0x11, 0x04, 0x01, 0x17, 0x00 } },
    { 0x01, 0x00, 0x0064, 0x0064, { 0x07, 0x01, 0x04, 0x07, 0x01, 0x04, 0x07, 0x01, 0x11, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x01, 0x02, 0x17, 0x01, 0x02, 0x17, 0x01, 0x02, 0x17, 0x00 } },
    { 0x03, 0x00, 0x0046, 0x007D, { 0x03, 0x02, 0x11, 0x03, 0x02, 0x17, 0x03, 0x02, 0x17, 0x00 } },
    { 0x03, 0x00, 0x004F, 0x0070, { 0x04, 0x02, 0x04, 0x04, 0x02, 0x11, 0x04, 0x02, 0x17, 0x00 } },
    { 0x02, 0x00, 0x005A, 0x0064, { 0x05, 0x02, 0x04, 0x05, 0x02, 0x04, 0x04, 0x02, 0x11, 0x00 } },
    { 0x01, 0x00, 0x0064, 0x0064, { 0x08, 0x02, 0x04, 0x08, 0x02, 0x04, 0x07, 0x02, 0x04, 0x00 } },
    { 0x04, 0x00, 0x0032, 0x00AA, { 0x02, 0x03, 0x17, 0x02, 0x03, 0x17, 0x01, 0x03, 0x17, 0x00 } },
    { 0x03, 0x00, 0x0046, 0x007D, { 0x04, 0x03, 0x11, 0x04, 0x03, 0x04, 0x03, 0x03, 0x17, 0x00 } },
    { 0x03, 0x00, 0x0050, 0x006E, { 0x05, 0x03, 0x04, 0x04, 0x03, 0x11, 0x04, 0x03, 0x17, 0x00 } },
    { 0x02, 0x00, 0x005C, 0x0064, { 0x06, 0x03, 0x04, 0x05, 0x03, 0x04, 0x05, 0x03, 0x11, 0x00 } },
    { 0x01, 0x00, 0x0069, 0x0060, { 0x09, 0x03, 0x04, 0x08, 0x03, 0x04, 0x08, 0x03, 0x11, 0x00 } },
    { 0x04, 0x00, 0x0014, 0x0168, { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x001E, 0x0140, { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x03, 0x00, 0x0028, 0x012C, { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x02, 0x00, 0x0030, 0x00F0, { 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x01, 0x00, 0x0037, 0x00C8, { 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
};

s32 fn_80189B88(void)
{
    HSD_GObj* gobj;
    HSD_JObj* jobj;
    TrainingModeState* state;
    CssSubStruct* sub;
    int i;
    PAD_STACK(8);

    state = &lbl_80473700;
    state = (TrainingModeState*) ((u8*) state + 0);
    sub = &state->css;
    state->css.x00 = 0;
    state->css.x01 = 0;
    HSD_GObj_SetupProc(GObj_Create(0xE, 2, 0), (HSD_GObjEvent) fn_801891F4,
                       0x15);
    gobj = GObj_Create(0xE, 0xF, 0);
    jobj = HSD_JObjLoadJoint((*lbl_804D662C)->joint);
    state->css.gobj = gobj;
    HSD_GObjObject_80390A70(gobj, HSD_GObj_804D7849, jobj);
    GObj_SetupGXLink(gobj, HSD_GObj_JObjCallback, 0xB, 0);
    HSD_GObj_SetupProc(gobj, (HSD_GObjEvent) fn_80188EE8, 0x11);
    gm_8016895C(jobj, *lbl_804D662C, 0);
    HSD_JObjReqAnimAll(jobj, *(f32*) &lbl_804DA63C);
    HSD_JObjAnimAll(jobj);
    for (i = 0; i < 0x27; i++) {
        sub->anim_frames[i] = 0;
        lb_80011E24(jobj, &sub->jobjs[i], i, -1);
    }
    sub->anim_frames[22] = 0x1E;
    sub->anim_frames[1] = 0x1E;
    sub->menu_values[6] = sub->menu_values[5] = sub->menu_values[4] =
        sub->menu_values[3] = sub->menu_values[2] = sub->menu_values[1] =
            sub->menu_values[0] = 0;
    sub->menu_values[0] = 2;
    return 0;
}

TrainingModeState* gm_80189CDC(StartMeleeData* arg0)
{
    TrainingModeState* state;
    s32 i;

    arg0->rules.x0_0 = 0;
    arg0->rules.is_teams = 1;
    arg0->rules.xB = -1;
    arg0->rules.x20 = 0xFFFFFFFFFFFFFFFFULL;
    arg0->rules.x5_0 = 1;

    state = &lbl_80473700;
    state = (TrainingModeState*) ((u8*) state + 0);

    for (i = 0; i < 4; i++) {
        state->css.saved_players[i] = arg0->players[i];
        state->players[i] = state->css.saved_players[i];
    }

    state->mode = (s32) (arg0->players[0].slot - 1);
    state->count = 1;

    for (i = 0; i < 27; i++) {
        state->char_data[i] = 0;
    }

    return state;
}

inline void resetText(HSD_Text* text)
{
    text->x34.x = lbl_804DA634;
    text->x34.y = 0.6f;
}

HSD_Text* fn_8018A000(void)
{
    TrainingModeState* state;
    u8* data;
    HSD_Text** textp;
    HSD_Text* text;

    PAD_STACK(0x10);
    data = lbl_803D9828;
    state = &lbl_80473700;
    memzero(state->result_cache, 0x10);
    lbl_804D6628 =
        lbArchive_80016DBC("GmTrain", &lbl_804D662C, &data[0xA0], 0);
    fn_80189B88();
    ifAll_802F3404();
    HSD_SisLib_803A611C(0, NULL, 9, 0x14, 0, 0xE, 0, 0x12);
    if (lbLang_IsSavedLanguageUS()) {
        HSD_SisLib_803A62A0(0, (char*) &data[0xBC], (char*) &data[0xC8]);
    } else {
        HSD_SisLib_803A62A0(0, (char*) &data[0xDC], (char*) &data[0xC8]);
    }

    *(textp = &state->css.text) = HSD_SisLib_803A5ACC(
        0, 0,
        (12.0f * (HSD_JObjGetTranslationX(state->css.jobjs[1]) + 9.798828f)) +
            50.0f,
        150.0f, 0.1f, 167.0f, 16.0f);
    text = state->css.text;
    lbLang_IsSettingUS();
    HSD_SisLib_803A6368(text, (s32) * (s16*) &data[2]);
    (*textp)->default_fitting = 1;
    resetText(*textp);
    text = *textp;
    text->default_alignment = 2;
    return text;
}

SECTION_SDATA2 f32 lbl_804DA634 = 0.7F;
/// A zero-init __declspec def bss-ifies into .sbss2; const + idiom-147
/// deref-cast read is the route that emits the initialized zero slot.
const f32 lbl_804DA63C = 0.0F;

u8 gm_8018A160(u8 difficulty, u8 stage_slot)
{
    return lbl_803D9910[stage_slot + (difficulty * 5)].stage_kind;
}

f32 gm_8018A188(u8 difficulty, u8 stage_slot)
{
    return (f32) lbl_803D9910[stage_slot + (difficulty * 5)].scale0_pct /
           100.0F;
}

f32 gm_8018A1D8(u8 difficulty, u8 stage_slot)
{
    return (f32) lbl_803D9910[stage_slot + (difficulty * 5)].scale1_pct /
           100.0F;
}

u8 gm_8018A228(u8 difficulty, u8 stage_slot, u8 idx)
{
    return lbl_803D9910[stage_slot + (difficulty * 5)].pad_6[idx * 3];
}

u8 gm_8018A25C(u8 difficulty, u8 stage_slot, u8 arg2)
{
    return lbl_803D9910[stage_slot + difficulty * 5].pad_6[arg2 * 3 + 1];
}

u8 gm_8018A290(u8 difficulty, u8 stage_slot, u8 idx)
{
    return lbl_803D9910[stage_slot + (difficulty * 5)].pad_6[2 + idx * 3];
}

f32 gm_8018A2C4(u8 difficulty, u8 stage_slot)
{
    return (f32) lbl_803D9910[stage_slot + (difficulty * 5)].scale0_pct /
           100.0F;
}

f32 gm_8018A314(u8 difficulty, u8 stage_slot)
{
    return (f32) lbl_803D9910[stage_slot + (difficulty * 5)].scale1_pct /
           100.0F;
}

void fn_8018A364(int arg0_int)
{
    MatchEnd* arg0 = (MatchEnd*) arg0_int;
    struct StartMeleeRules* rules;
    gmm_x0_528_t* main_data;
    UnkAllstarData* data;
    u32 total_time;

    rules = gm_8016AE50();
    data = &gm_80473A18;

    if (fn_8017E318() > 0) {
        ((u8_bits*) &arg0->_x448[2])->b3 = 1;
    }

    if (rules->x4_5) {
        main_data = gmMainLib_8015CDE0();
        total_time = data->x0.xC.x20 + gm_8016AEDC();
        ((u8_bits*) &arg0->_x448[0])->b5 = 1;

        if ((u8) data->x0.cpu_level == 4) {
            ((u8_bits*) &arg0->_x448[0])->b2 = 1;
        }

        if (total_time < 0x6270U) {
            ((u8_bits*) &arg0->_x448[2])->b6 = 1;
        } else if (total_time < 0x9AB0U) {
            ((u8_bits*) &arg0->_x448[2])->b7 = 1;
        }

        if ((u32) data->x0.xC.x1C == 0U && arg0->player_standings[0].x44 == 0)
        {
            ((u8_bits*) &arg0->_x448[1])->b1 = 1;
        }

        if ((u8) data->x0.xC.xE != 0) {
            ((u8_bits*) &arg0->_x448[2])->b5 = 1;
        }

        if ((u8) data->x0.xC.xF != 0) {
            ((u8_bits*) &arg0->_x448[2])->b4 = 1;
        }

        if ((u32) (data->x0.xC.x1C + arg0->player_standings[0].x44) ==
            (u32) Player_GetDamage(0))
        {
            ((u8_bits*) &arg0->_x448[0])->b1 = 1;
        }

        if ((u8) data->x0.xC.xD != 0) {
            ((u8_bits*) &arg0->_x448[1])->b0 = 1;
            return;
        }

        if (arg0->player_standings[0].stocks == main_data->stocks) {
            ((u8_bits*) &arg0->_x448[1])->b2 = 1;
        }
    }
}
