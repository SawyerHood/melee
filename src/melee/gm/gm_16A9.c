#include "gm_1601.h"


#include "gm_unsplit.h"
#include "gmmain_lib.h"
#include "gmstamina.h"
#include "placeholder.h"
#include "stddef.h"

#include "baselib/gobjplink.h"
#include "baselib/jobj.h"
#include "baselib/particle.h"
#include "cm/camera.h"

#include "gm/forward.h"

#include "gm/types.h"
#include "gr/ground.h"
#include "gr/stage.h"
#include "if/ifnametag.h"
#include "if/ifstatus.h"
#include "lb/lb_00B0.h"
#include "lb/lb_00F9.h"
#include "lb/lbaudio_ax.h"
#include "lb/lblanguage.h"
#include "lb/lbtime.h"
#include "mn/mnstagesel.h"
#include "pl/player.h"
#include "pl/plbonus.h"
#include "pl/plbonuslib.h"
#include "sc/types.h"
#include "ty/toy.h"

#include <m2c_macros.h>
#include <baselib/video.h>
#include <sysdolphin/baselib/controller.h>
#include <sysdolphin/baselib/random.h>
#include <melee/gm/gm_1A45.h>
#include <melee/pl/player.h>

/* 46B668 */ static struct lbl_8046B668_t lbl_8046B668;

void gm_8016A92C(StartMeleeRules* arg0)
{
    lbl_8046B668.arr2[0] = -2;
    arg0->x58 = &lbl_8046B668;
}

bool gm_8016A944(void)
{
    if (gm_8016AE50()->x58 != NULL) {
        return true;
    }
    return false;
}

UNK_T gm_8016A97C(void)
{
    return &M2C_FIELD(&lbl_8046B668, UNK_T*, 0x1C);
}

struct lbl_8046B668_t* gm_8016A98C(void)
{
    return &lbl_8046B668;
}

int gm_8016A998(s8 arg0, s8 arg1)
{
    int i;
    struct lbl_8046B668_t* ptr = &lbl_8046B668;
    for (i = 0; i < 27; i++) {
        if (ptr->arr2[i] == -2) {
            ptr->arr2[i + 1] = -2;
            ptr->arr2[i] = arg1;
            ptr->arr1[i] = (u8) arg0;
            return i;
        }
    }
    return -1;
}

int gm_8016A9E8(u8 arg0, s8 arg1)
{
    int i;
    int found;
    struct lbl_8046B668_t* ptr = &lbl_8046B668;

    found = -1;
    for (i = 0; i < 27; i++) {
        if (ptr->arr2[i] == -2) {
            found = i;
            break;
        }
    }
    if (found != -1) {
        for (i = found; i >= 0; i--) {
            ptr->arr2[i + 1] = ptr->arr2[i];
            ptr->arr1[i + 1] = ptr->arr1[i];
        }
        ptr->arr2[0] = arg1;
        ptr->arr1[0] = (u8) arg0;
        found++;
    }
    return found;
}

bool gm_8016AC44(s8 ckind, s8 costume_id)
{
    s32 idx;
    s32 i;
    bool found;

    found = false;
    if (gm_8016AE50()->x58 != NULL) {
        found = true;
    }
    if (found == 1) {
        idx = -1;
        for (i = 0; i < 27; i++) {
            if (lbl_8046B668.arr2[i] == -2) {
                idx = i;
                break;
            }
        }
        if (idx != -1) {
            for (i = idx; i >= 0; i--) {
                if (lbl_8046B668.arr2[i] == costume_id &&
                    ckind == lbl_8046B668.arr1[i])
                {
                    return true;
                }
            }
        }
    }
    return false;
}
