#include "ftmetal.h"

#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/id.h>
#include <melee/ft/fighter.h>
#include <melee/ft/ft_081B.h>
#include <melee/ft/ftanim.h>
#include <melee/ft/ftchangeparam.h>
#include <melee/ft/ftcommon.h>
#include <melee/ft/ftmaterial.h>
#include <melee/ft/ftparts.h>
#include <melee/ft/types.h>
#include <melee/lb/lb_00B0.h>

static s8 ftCo_804D3C90 = 0;

void ft_800C8170(Fighter* fp)
{
    int i;
    HSD_JObj* jobj;
    HSD_DObj* dobj;
    HSD_MObj* mobj;
    u32 flags;

    PAD_STACK(8);

    for (i = 0; i < ftPartsTable[fp->kind]->parts_num; i++) {
        if (fp->parts[i].flags_b1) {
            FighterBone* bone = &fp->parts[i];
            jobj = bone->joint;
            dobj = HSD_JObjGetDObj(jobj);
            flags = 0;
            if (bone->flags_b7) {
                flags |= JOBJ_LIGHTING;
            }
            if (bone->flags2_b0) {
                flags |= JOBJ_TEXGEN;
            }
            if (bone->flags2_b1) {
                flags |= JOBJ_SPECULAR;
            }
            if (bone->flags2_b2) {
                flags |= JOBJ_UNK_B18;
            }
            if (bone->flags2_b3) {
                flags |= JOBJ_UNK_B20;
            }
            if (bone->flags2_b4) {
                flags |= JOBJ_UNK_B19;
            }
            HSD_JObjClearFlags(jobj, 0x50180);
            HSD_JObjSetFlags(jobj, flags);
            while (dobj != NULL) {
                if (dobj != NULL) {
                    mobj = dobj->mobj;
                } else {
                    mobj = NULL;
                }
                switch (mobj->rendermode & RENDER_BLENDING) {
                case 0:
                    HSD_DObjModifyFlags(dobj, 2, 0xE);
                    break;
                case RENDER_XLU:
                    HSD_DObjModifyFlags(dobj, 8, 0xE);
                    break;
                case RENDER_XLU | RENDER_NO_ZUPDATE:
                    HSD_DObjModifyFlags(dobj, 4, 0xE);
                    break;
                default:
                    OSReport("mobj has unexpected blending flags (0x%x).",
                             dobj->mobj->rendermode);
                    HSD_Panic("ftmetal.c", 0x73, "");
                    break;
                }
                dobj = dobj != NULL ? dobj->next : NULL;
            }
        }
    }
}

static inline void enableMetal(Fighter* fp)
{
    HSD_DObj* dobj;
    int i;
    HSD_JObj* jobj;

    for (i = 0; i < ftPartsTable[fp->kind]->parts_num; i++) {
        if (fp->parts[i].flags_b1) {
            jobj = fp->parts[i].joint;
            dobj = HSD_JObjGetDObj(jobj);
            HSD_JObjSetFlags(jobj, 0x50180);
            HSD_JObjClearFlags(jobj, 0x180000);
            while (dobj != NULL) {
                HSD_DObjModifyFlags(dobj, 2, 0xE);
                dobj = dobj != NULL ? dobj->next : NULL;
            }
        }
    }
    ftCommon_80080460(fp);
}

void ftCo_800C8348(Fighter_GObj* arg0, int timer, int health)
{
    Fighter* fp = GET_FIGHTER(arg0);

    PAD_STACK(0x10);

    fp->metal_timer = timer;
    fp->metal_health = health;

    if (fp->is_metal) {
        return;
    }
    fp->is_metal = true;

    enableMetal(fp);
}

void ftCo_800C8438(Fighter_GObj* gobj)
{
    Fighter* fp;

    PAD_STACK(0x10);

    fp = GET_FIGHTER(gobj);
    fp->is_metal = fp->is_always_metal;
    fp->metal_timer = 0;
    fp->metal_health = 0;
    if (!fp->is_metal) {
        return;
    }

    enableMetal(fp);

    ftCo_800D105C(gobj);
    ft_80081C88(gobj, fp->x34_scale.y);
}

void ftCo_800C8540(Fighter_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    fp->metal_timer = 0;
    fp->metal_health = 0;
    if (!fp->is_always_metal) {
        fp->is_metal = false;
        ftCo_800D105C(gobj);
        ft_80081C88(gobj, fp->x34_scale.y);
        ft_800C8170(fp);
        ftCommon_80080474(fp);
    }
}

static inline HSD_DObj* dobj_next(HSD_DObj* dobj)
{
    if (dobj != NULL) {
        return dobj->next;
    }
    return NULL;
}

void ft_800C85B8(Fighter_GObj* gobj)
{
    HSD_Joint* sp20;
    s32 sp1C;

    Fighter* fp;
    int i;
    s32 total_dobjs;
    HSD_JObj* jobj;
    HSD_DObj* dobj;
    HSD_Joint* joint;
    s32 group_count;
    HSD_DObj* tail;
    HSD_MObj* mobj;

    PAD_STACK(8);

    fp = GET_FIGHTER(gobj);
    joint = fp->ft_data->x5C;
    sp20 = joint;
    total_dobjs = i = sp1C = 0;
    while (sp20 != NULL) {
        if (ftParts_8007506C(fp->kind, i) != 0) {
            i++;
        } else {
            HSD_IDInsertToTable(NULL, (u32) sp20, fp->parts[i].joint);
            i++;
            ftAnim_GetNextJointInTree(&sp20, &sp1C);
        }
    }
    sp20 = joint;
    sp1C = i = 0;
    while (sp20 != NULL) {
        if (ftParts_8007506C(fp->kind, i) != 0) {
            i++;
        } else {
            group_count = 0;
            jobj = fp->parts[i].joint;
            dobj = HSD_DObjLoadDesc(sp20->u.dobjdesc);
            if (dobj != NULL) {
                tail = HSD_JObjGetDObj(jobj);
                fp->parts[i].flags2_b5 = true;
                HSD_DObjResolveRefsAll(dobj, sp20->u.dobjdesc);
                if (tail == NULL) {
                    HSD_JObjAddDObj(jobj, dobj);
                } else {
                    while (tail != NULL) {
                        HSD_DObj* nxt = (tail != NULL) ? tail->next : NULL;
                        if (nxt == NULL) {
                            break;
                        }
                        tail = (tail != NULL) ? tail->next : NULL;
                    }
                    lb_8000CE30(tail, dobj);
                }
                while (true) {
                    if (dobj == NULL) {
                        break;
                    }
                    if (total_dobjs >= 0x20) {
                        OSReport("fighter parts model dobj num over!\n");
                        __assert("ftmetal.c", 0xF8, "0");
                    }
                    fp->x203C.data[total_dobjs] = dobj;
                    mobj = dobj->mobj;
                    if (mobj != NULL) {
                        hsdChangeClass(mobj, &ftMObj);
                    }
                    dobj = (dobj != NULL) ? dobj->next : NULL;
                    total_dobjs += 1;
                    group_count += 1;
                }
                if (group_count >= 0x80) {
                    OSReport("fighter dobj num over!\n");
                    __assert("ftmetal.c", 0x106, "0");
                }
                fp->parts[i].xD = total_dobjs - 1;
                fp->parts[i].flags2_b6 = true;
            }
            i++;
            ftAnim_GetNextJointInTree(&sp20, &sp1C);
        }
    }
    fp->x203C.count = total_dobjs;
}
