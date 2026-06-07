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

struct lbl_80472E48_t {
    /* 0x00 */ u8 x0;
    /* 0x01 */ char pad_1[3];
    /* 0x04 */ s32 unk_4; /* inferred */
    /* 0x08 */ s32 unk_8; /* inferred */
    /* 0x0C */ u32 xC;
    /* 0x10 */ u8 x10;
    /* 0x11 */ char pad_11[3];
    /* 0x14 */ s32 x14[0x1B];
}; /* size = 0x80 */
STATIC_ASSERT(sizeof(struct lbl_80472E48_t) == 0x80);

static struct lbl_80472E48_t lbl_80472E48;
static int lbl_80472EC8[4];

s32 gm_80180AE4(void)
{
    return lbl_80472EC8[0] * 0xA;
}

Fighter_GObj* gm_80180AF4(void)
{
    return Player_GetEntity(1);
}

void gm_80180B18(void)
{
    int i;

    for (i = 0; i < 0x1B; i++) {
        u8 idx = gm_80164024((u8) i);
        lbl_80472E48.x14[gm_80164024((u8) i)] =
            (u32) *gmMainLib_8015D06C(idx) / 10;
    }
}

void gm_80180BA0(void)
{
    int i;

    for (i = 0; i < 0x1B; i++) {
        u8 idx = gm_80164024((u8) i);
        *gmMainLib_8015D06C(gm_80164024((u8) i)) = lbl_80472E48.x14[idx] * 0xA;
    }
}

void fn_80180C14(HSD_GObj* gobj)
{
    HSD_JObj* jobj = gobj->hsd_obj;

    if ((lbl_80472E48.x0 & 3) != 0) {
        HSD_JObjClearFlagsAll(jobj, 0x10);
        HSD_JObjAnimAll(jobj);
    }
}

/* data-recon w18: full .sbss block in target order (0x804D65C8..0x804D65DC).
 * lbl_804D65C8 and lbl_804D65D8 were undefined externs - defined in-TU
 * (single-TU refs). Statics emit at decl point => target sbss layout exact.
 * dtk size 0x8 on lbl_804D65D8 is gap-inclusive (4B align pad, idiom 101). */
static HSD_Archive* lbl_804D65C8;
static DynamicModelDesc** lbl_804D65CC;
static DynamicModelDesc** lbl_804D65D0;
static s32 lbl_804D65D4;
static s32 lbl_804D65D8;

void fn_80180C60(HSD_GObj* arg0)
{
    typedef struct {
        u8 b76 : 2, b54 : 2, b32 : 2, b10 : 2;
    } x0_2bits;
    f32 Ground_801C57F0();
    HSD_GObj* gobj = arg0;
    u8* base = (u8*) &lbl_80472E48;
    HSD_JObj* jobj;
    s32 dist;
    s32 disp;
    s32 d;
    u32 b76;

    dist = (s32) (0.1f * Ground_801C57F0(0));
    jobj = gobj->hsd_obj;
    if (dist < 0) {
        dist = 0;
    }

    *(s32*) (base + 0x80) = dist;
    b76 = ((u8) base[0] >> 6) & 3;

    if (b76 != 0 && (((u8) base[0] >> 4) & 3)) {
        ifTime_HideTimers();
        if (*(s32*) (base + 0x80) == *(s32*) (base + 0x84)) {
            *(s32*) (base + 0x8C) = *(s32*) (base + 0x8C) + 1;
        } else {
            *(s32*) (base + 0x8C) = 0;
        }
        if (*(s32*) (base + 0x8C) > 0x3C) {
            ((x0_2bits*) base)->b32 = 1;
            if (dist == 0 && !(base[0] & 3)) {
                ((x0_2bits*) base)->b10 = 1;
            }
        }
    } else {
        if (b76 != 0) {
            ifTime_HideTimers();
            if (*(s32*) (base + 0x80) == *(s32*) (base + 0x84)) {
                *(s32*) (base + 0x8C) = *(s32*) (base + 0x8C) + 1;
            } else {
                *(s32*) (base + 0x8C) = 0;
            }
            if (*(s32*) (base + 0x8C) > 0x78) {
                ((x0_2bits*) base)->b32 = 1;
                if (!(base[0] & 3)) {
                    ((x0_2bits*) base)->b10 = 1;
                }
            }
        } else if (gm_8016AEEC() == 0 && gm_8016AEFC() == 0x3B) {
            ((x0_2bits*) base)->b76 = 1;
            ifTime_HideTimers();
            Player_80031790(0);
        }
        if (Ground_801C1DC0() != 0) {
            if (!(((u8) base[0] >> 6) & 3)) {
                ((x0_2bits*) base)->b76 = 1;
                ifTime_HideTimers();
                Player_80031790(0);
            }
            if (!(((u8) base[0] >> 4) & 3)) {
                ((x0_2bits*) base)->b54 = 1;
                Player_80031790(0);
            }
            *(s32*) (base + 0x8C) = 0;
        }
    }

    if (!(((u8) base[0] >> 2) & 3)) {
        HSD_JObjReqAnimAll(jobj, 0.0f);
    } else if (base[0] & 3) {
        HSD_JObjSetFlagsAll(jobj, 0x10U);
    } else if (dist > ((s32*) (base + 0x14))[gm_80164024(*(u8*) (base + 4))]) {
        if (lbl_804D65D4 == 0) {
            lbAudioAx_800237A8(0x9C40, 0x7F, 0x40);
            lbAudioAx_800237A8(0x144, 0x7F, 0x40);
            gm_80167858((s32) * (s8*) (base + 0x10),
                        (s32) Player_GetNametagSlotID(0), 0xD, 0x5A);
            lbl_804D65D4 = 1;
        }
        HSD_JObjClearFlagsAll(
            HSD_JObjGetNext(HSD_JObjGetChild(HSD_JObjGetChild(jobj))), 0x10U);
    }

    if (lbLang_IsSavedLanguageUS() != 0) {
        disp = (s32) ((f64) (f32) dist / 0.304788);
    } else {
        disp = dist;
    }
    if (disp > 0x1869F) {
        disp = 0x1869F;
    }

    /* ones digit */
    HSD_JObjReqAnimAll(HSD_JObjGetNext(HSD_JObjGetNext(
                           HSD_JObjGetChild(HSD_JObjGetChild(jobj)))),
                       (f32) (disp % 10));

    /* tens digit */
    HSD_JObjReqAnimAll(HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(
                           HSD_JObjGetChild(HSD_JObjGetChild(jobj))))),
                       (f32) ((disp / 10) % 10));

    /* hundreds digit */
    d = disp / 100;
    if (d != 0) {
        HSD_JObjReqAnimAll(
            HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(
                HSD_JObjGetNext(HSD_JObjGetChild(HSD_JObjGetChild(jobj)))))),
            (f32) (d % 10));
    } else {
        HSD_JObjReqAnimAll(
            HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(
                HSD_JObjGetNext(HSD_JObjGetChild(HSD_JObjGetChild(jobj)))))),
            10.0f);
    }

    /* thousands digit */
    d = disp / 1000;
    if (d != 0) {
        HSD_JObjReqAnimAll(
            HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(
                HSD_JObjGetNext(HSD_JObjGetChild(HSD_JObjGetChild(jobj))))))),
            (f32) (d % 10));
    } else {
        HSD_JObjReqAnimAll(
            HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(
                HSD_JObjGetNext(HSD_JObjGetChild(HSD_JObjGetChild(jobj))))))),
            10.0f);
    }

    /* ten-thousands digit */
    d = disp / 10000;
    if (d != 0) {
        HSD_JObjReqAnimAll(
            HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(
                HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(
                    HSD_JObjGetChild(HSD_JObjGetChild(jobj)))))))),
            (f32) (d % 10));
    } else {
        HSD_JObjReqAnimAll(
            HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(
                HSD_JObjGetNext(HSD_JObjGetNext(HSD_JObjGetNext(
                    HSD_JObjGetChild(HSD_JObjGetChild(jobj)))))))),
            10.0f);
    }

    HSD_JObjAnimAll(jobj);
    lbl_80472EC8[1] = lbl_80472EC8[0];
    if (lbl_80472EC8[0] > lbl_80472EC8[2] + 0xA) {
        lbl_80472EC8[2] = lbl_80472EC8[0];
        lbAudioAx_80023870(0xBB, 0x7F, 0x40, 0x8A);
    }
}

void fn_80181598(void)
{
    typedef struct {
        u8 b76 : 2, b54 : 2, b32 : 2, b10 : 2;
    } x0_2bits;
    u8* base = (u8*) &lbl_80472E48;
    u32 mode;

    PAD_STACK(0x20);

    if (gm_801A4624() != 0) {
        return;
    }

    mode = base[0] & 3;

    if (mode != 0) {
        if (mode == 1) {
            lbAudioAx_800237A8(0xC0, 0x7F, 0x40);
            lbAudioAx_800237A8(0x148, 0x7F, 0x40);
            mode = 2;
            ((x0_2bits*) base)->b10 = mode;
        }
        lbl_804D65D8 += 1;
        if (lbl_804D65D8 >= 0xF0 ||
            (lbl_804D65D8 > 0x3C &&
             (HSD_PadCopyStatus[base[0x10]].trigger & 0x100)))
        {
            gm_8016B328();
            return;
        }
    }

    if (((base[0] >> 2) & 3) != 0 &&
        ((mode = base[0] & 3, mode == 0) || mode == 3))
    {
        *(u32*) (base + 0xC) += 1;
        if (*(u32*) (base + 0xC) > 0x3C &&
            (*(u32*) (base + 0xC) >= 0xF0 ||
             (HSD_PadCopyStatus[base[0x10]].trigger & 0x100)))
        {
            s32* punk = (s32*) (base + 4);
            s32 cur;
            s32* tbl;
            if ((cur = *(s32*) (base + 0x80)) >
                (tbl = (s32*) (base + 0x14))[gm_80164024((u8) *punk)])
            {
                tbl[gm_80164024((u8) *punk)] = cur;
            }
            gm_8016B328();
        }
    }
}

void fn_80181708(void)
{
    HSD_GObj* new_var;
    typedef struct {
        u8 b76 : 2, b54 : 2, b32 : 2, b10 : 2;
    } x0_2bits;
    HSD_JObj* jobj;
    HSD_GObj* gobj;

    u8* base = (u8*) &lbl_80472E48;
    *(s32*) (base + 0x80) = 0;
    *(s32*) (base + 0x84) = 0;
    *(s32*) (base + 0x88) = 0;
    *(s32*) (base + 0x8C) = 0;
    ((x0_2bits*) base)->b76 = 0;
    ((x0_2bits*) base)->b54 = 0;
    ((x0_2bits*) base)->b32 = 0;
    ((x0_2bits*) base)->b10 = 0;
    *(u32*) (base + 0xC) = 0;
    *(u8*) (base + 0x10) = (s8) Player_GetPlayerId(0);
    lbl_804D65D4 = 0;
    lbl_804D65D8 = 0;

    HSD_GObj_SetupProc(GObj_Create(0xEU, 0x11U, 0U),
                       (void (*)(HSD_GObj*)) fn_80181598, 0x15U);

    gobj = GObj_Create(0xEU, 0xFU, 0U);
    jobj = HSD_JObjLoadJoint((*lbl_804D65CC)->joint);
    HSD_GObjObject_80390A70(gobj, HSD_GObj_804D7849, jobj);
    GObj_SetupGXLink(gobj, HSD_GObj_JObjCallback, 0xBU, 0U);
    HSD_GObj_SetupProc(gobj, fn_80180C14, 0x15U);
    gm_8016895C(jobj, *lbl_804D65CC, 0);
    HSD_JObjReqAnimAll(jobj, 0.0f);
    HSD_JObjAnimAll(jobj);
    HSD_JObjSetFlagsAll(jobj, 0x10U);

    gobj = (new_var = GObj_Create(0xEU, 0xFU, 0U));
    jobj = HSD_JObjLoadJoint((*lbl_804D65D0)->joint);
    HSD_GObjObject_80390A70(new_var, HSD_GObj_804D7849, jobj);
    GObj_SetupGXLink(new_var, HSD_GObj_JObjCallback, 0xBU, 0U);
    HSD_GObj_SetupProc(new_var, fn_80180C60, 0x15U);
    gm_8016895C(jobj, *lbl_804D65D0, 0);
    HSD_JObjReqAnimAll(jobj, 10.0f);
    HSD_JObjAnimAll(jobj);
    HSD_JObjClearFlagsAll(jobj, 0x10U);

    HSD_JObjSetFlagsAll(
        HSD_JObjGetNext(HSD_JObjGetChild(HSD_JObjGetChild(jobj))), 0x10U);
    HSD_JObjClearFlagsAll(HSD_JObjGetNext(HSD_JObjGetNext(
                              HSD_JObjGetChild(HSD_JObjGetChild(jobj)))),
                          0x10U);

    gm_80168F88();
}

void gm_80181998(void)
{
    lbl_804D65C8 = lbArchive_80016DBC("IfHrNoCn", &lbl_804D65CC,
                                      "ScInfCnt_scene_models", 0);
    lbl_804D65C8 = lbArchive_80016DBC("IfHrReco", &lbl_804D65D0,
                                      "ScInfCnt_scene_models", 0);
    fn_80181708();
}

void gm_80181A00(s32 arg0, s32 arg1)
{
    lbl_80472E48.unk_4 = arg0;
    lbl_80472E48.unk_8 = arg1;
}
