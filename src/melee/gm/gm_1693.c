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

struct lbl_8046B488_t {
    /* 0x000 */ s8 x0;
    /* 0x001 */ s8 x1;
    /* 0x002 */ s8 x2;
    /* 0x003 */ s8 x3;
    /* 0x004 */ s8 x4;
    /* 0x005 */ s8 x5;
    /* 0x006 */ u8 x6;
    /* 0x007 */ u8 x7;
    /* 0x008 */ u8 x8;
    /* 0x009 */ u8 x9;
    /* 0x00A */ u8 xA;
    /* 0x00B */ u8 xB;
    /* 0x00C */ u8 xC;
    /* 0x00D */ u8 xD;
    /* 0x00E */ u8 xE;
    /* 0x00F */ u8 xF;
    /* 0x010:0 */ u8 unk_10_b0 : 1;
    /* 0x010:1 */ u8 unk_10_b1 : 1;
    /* 0x010:2 */ u8 unk_10_b2 : 1;
    /* 0x010:3 */ u8 unk_10_b3 : 1;
    /* 0x010:4 */ u8 unk_10_b4 : 1;
    /* 0x010:5 */ u8 unk_10_b5 : 1;
    /* 0x010:6 */ u8 unk_10_b6 : 1;
    /* 0x010:7 */ u8 unk_10_b7 : 1;
    /* 0x011 */ char pad_11[0x14 - 0x11];
    /* 0x014 */ f32 x14;
    /* 0x018 */ f32 x18;
    /* 0x01C */ f32 x1C;
    /* 0x020 */ s8 x20[0xA2 - 0x20];
    /* 0x0A2 */ s8 xA2[0x124 - 0xA2];
    /* 0x124 */ s8 x124[0x1A6 - 0x124];
    /* 0x1A6 */ s8 x1A6[0x1AC - 0x1A6];
    /* 0x1AC */ u8 x1AC;
    /* 0x1AD */ u8 x1AD;
    /* 0x1AE */ s8 x1AE[0x1B2 - 0x1AE];
    /* 0x1B2 */ u8 x1B2;
    /* 0x1AE */ s8 x1B3[0x1B8 - 0x1B3];
    /* 0x1B8 */ GmRouteCallback x1B8;
    /* 0x1BC */ char pad_1BC[0x1E0 - 0x1BC];
}; /* size = 0x1E0 */
STATIC_ASSERT(sizeof(struct lbl_8046B488_t) == 0x1E0);

/* 1693A8 */ static s32 fn_801693A8(void);
/* 16A1E4 */ static s32 fn_8016A1E4(void);

/* 46B488 */ static struct lbl_8046B488_t lbl_8046B488;

static inline bool gm_80164840_inner(u8 ckind)
{
    return gm_80164840(ckind);
}

static inline bool gm_80164840_noinline(u8 ckind)
{
    return gm_80164840_inner(ckind);
}


static inline u8 gm_80169238_inner2(u8 ckind)
{
    return gm_80169238(ckind);
}

static inline u8 gm_80169238_inner(u8 ckind)
{
    return gm_80169238_inner2(ckind);
}

static inline u8 gm_80169238_noinline(u8 ckind)
{
    return gm_80169238_inner(ckind);
}

struct lbl_8046B488_t* fn_80169364(void)
{
    return &lbl_8046B488;
}

u8 gm_80169370(s32 arg0)
{
    return ((u8*) &lbl_8046B488)[arg0];
}

int gm_80169384(void)
{
    return lbl_8046B488.x7;
}

s32 gm_80169394(void)
{
    return lbl_8046B488.unk_10_b3;
}

s32 fn_801693A8(void)
{
    return lbl_8046B488.unk_10_b2;
}

static inline bool gm_801693BC_inline(u8 ckind)
{
    if (ckind - 0x1B <= 1) {
        return true;
    }
    return false;
}

/// Player is Fighting Wireframe
bool gm_801693BC(int arg0)
{
    u8 ckind;
    PAD_STACK(8);
    if (Player_GetFlagsBit1(arg0) && (int) lbl_8046B488.x8 > 1) {
        return true;
    }
    ckind = Player_GetPlayerCharacter(arg0);
    ckind -= CKIND_BOY;
    if (ckind <= CKIND_GIRL - CKIND_BOY) {
        return true;
    }
    return false;
}

void fn_80169434(GmRouteCallback fn)
{
    lbl_8046B488.x1B8 = fn;
}

bool fn_80169444(bool arg0)
{
    struct lbl_8046B488_t* gp = &lbl_8046B488;

    if (gp->x1B8 != NULL) {
        if (gp->x1B8(arg0) == 1) {
            gp->x1B8 = NULL;
            return true;
        }
    }
    return false;
}
int gm_801694A0(HSD_GObj* arg0)
{
    int i;
    int count = lbl_8046B488.x7;
    PAD_STACK(8);
    for (i = 0; i < 6; i++) {
        if (Player_GetPlayerSlotType(i) != Gm_PKind_NA &&
            Player_GetFlagsBit1(i) && Player_GetPlayerState(i) == 2)
        {
            count++;
        }
    }
    return count;
}

UNK_T gm_80169520(void)
{
    return lbl_8046B488.x20;
}

UNK_T gm_80169530(void)
{
    return lbl_8046B488.xA2;
}

UNK_T gm_80169540(void)
{
    return lbl_8046B488.x124;
}

void fn_80169550(int slot)
{
    s8 idx = lbl_8046B488.x1A6[slot];
    lbl_8046B488.x20[idx] = -1;
}

void fn_80169574(ssize_t size, s8* buf)
{
    memzero(buf, size);
    buf[size] = -2;
}

s32 fn_801695BC(u8 arg0, u8 arg1, u8 arg2, u8* arg3, u8* arg4)
{
    u8 sp1C[0x0C];
    s32 temp_r3_2;
    s32 var_ctr_2;
    s32 var_r28;
    s32 var_r28_2;
    s32 var_r7_2;
    s32 var_r8;
    s8 temp_r0;
    s8 temp_r3_3;
    s8 temp_r4;
    s8 temp_r4_3;
    s8 temp_r5;
    s8 var_r7;
    u32 var_ctr;
    u8 temp_r0_2;
    u8 temp_r3;
    u8 temp_r3_4;
    u8* temp_r4_2;
    u8* temp_r6;
    u8* var_r27;
    u8* var_r3;
    u8* var_r3_2;
    u8* var_r6;
    u8* temp_r27;

    temp_r3 = gm_80169238_noinline(arg0);
    if ((s8) arg0 != 0x21) {
        var_r7 = 0;
        if (temp_r3 > 0) {
            temp_r3_2 = (s32) temp_r3 - 8;
            if ((s32) temp_r3 > 8) {
                var_ctr = (u32) (temp_r3_2 + 7) >> 3;
                var_r6 = &sp1C[0];
                if (temp_r3_2 > 0) {
                    do {
                        var_r6[0] = (u8) var_r7;
                        var_r6[1] = (u8) (var_r7 + 1);
                        temp_r5 = var_r7 + 4;
                        var_r6[2] = (u8) (var_r7 + 2);
                        temp_r4 = var_r7 + 5;
                        temp_r3_3 = var_r7 + 6;
                        var_r6[3] = (u8) (var_r7 + 3);
                        temp_r0 = var_r7 + 7;
                        var_r7 += 8;
                        var_r6[4] = (u8) temp_r5;
                        var_r6[5] = (u8) temp_r4;
                        var_r6[6] = (u8) temp_r3_3;
                        var_r6[7] = (u8) temp_r0;
                        var_r6 += 8;
                        var_ctr -= 1;
                    } while (var_ctr != 0);
                }
            }
            var_r3_2 = &sp1C[var_r7];
            var_ctr_2 = (s32) temp_r3 - (s32) var_r7;
            if ((s32) var_r7 < (s32) temp_r3) {
                do {
                    *var_r3_2 = (u8) var_r7;
                    var_r3_2 += 1;
                    var_r7 += 1;
                    var_ctr_2 -= 1;
                } while (var_ctr_2 != 0);
            }
        }

        if (arg1 == (s8) arg0) {
            sp1C[(s8) arg2] = 0xFF;
        }

        var_r27 = &sp1C[0];
        var_r28 = 0;
    loop_12:
        if (var_r28 < (s32) temp_r3) {
            temp_r4_2 = &sp1C[HSD_Randi((s32) temp_r3)];
            temp_r3_4 = *temp_r4_2;
            var_r28 += 1;
            *temp_r4_2 = *var_r27;
            *var_r27 = temp_r3_4;
            var_r27 += 1;
            goto loop_12;
        }

        var_r3 = arg3;
        var_r8 = 0;
        var_r7_2 = 0;
    loop_19:
        temp_r6 = &arg4[var_r7_2];
        if ((s8) *temp_r6 != -2) {
            if ((s8) arg0 == (s8) *var_r3) {
                if ((s8) sp1C[var_r8 % temp_r3] == -1) {
                    var_r8 += 1;
                }
                temp_r0_2 = sp1C[var_r8 % temp_r3];
                var_r8 += 1;
                *temp_r6 = temp_r0_2;
            }
            var_r7_2 += 1;
            var_r3 += 1;
            goto loop_19;
        }

        var_r28_2 = 1;
    loop_26:
        temp_r27 = &arg4[var_r28_2];
        if ((s8) temp_r27[0] != -2) {
            var_r3 = &arg3[var_r28_2];
            if (((s8) arg0 == (s8) var_r3[0]) &&
                ((s8) arg0 == (s8) var_r3[-1]))
            {
                var_r3 = (u8*) HSD_Randi(2);
                if (var_r3 != NULL) {
                    temp_r4_3 = (s8) temp_r27[0];
                    temp_r27[0] = temp_r27[-1];
                    temp_r27[-1] = (u8) temp_r4_3;
                }
            }
            var_r28_2 += 1;
            goto loop_26;
        }
        return (s32) var_r3;
    }
    return (s32) temp_r3;
}

#pragma push
#pragma dont_inline on
void fn_801697FC(s8 character, s8 costume, s8 new_character, s8 new_costume,
                 s8* buf)
{
    u8 ncolors = gm_80169238(character);
    int i;

    if (character == 0x21) {
        return;
    }
    if (new_character == character && costume == new_costume) {
        costume = (s8) ((costume + 1) % ncolors);
    }
    for (i = 0; (s8) buf[i] != -2; i++) {
        buf[i] = costume;
    }
}
#pragma pop

void fn_8016989C(u8* arg0, u8 arg1, u8 arg2, u8* arg3, u8* arg4)
{
    s32 i;
    for (i = 0; i < 3; i++) {
        fn_801695BC(arg0[i], arg1, arg2, arg3, arg4);
    };
}

#pragma push
#pragma dont_inline on
void fn_80169900(u8 arg0, struct lbl_8046B488_t* arg1, s8* arg2, s8* arg3)
{
    s32 var_r28 = 0;

    switch ((s32) arg0) {
    case 0: {
        s8* src = arg3;
        s8* dst = arg2;
        while (*src != -2) {
            *dst = (s8) arg1->x0;
            src += 1;
            dst += 1;
        }
        break;
    }
    case 1: {
        s8* dst = arg2;
        s8* src = arg3;
        while (*src != -2) {
            if (HSD_Randi(2) != 0) {
                *dst = arg1->x0;
            } else {
                *dst = arg1->x1;
            }
            src += 1;
            dst += 1;
        }
        break;
    }
    case 2: {
        s32 var_r27 = HSD_Randi(5);
        while (arg3[var_r28] != -2) {
            if (var_r27 != 0) {
                var_r27 -= 1;
                arg2[var_r28] = arg1->x0;
            } else {
                var_r27 = 5;
                if (HSD_Randi(2) != 0) {
                    if ((s32) arg1->x1 != 0x21 && gm_80164840(arg1->x1)) {
                        arg2[var_r28] = arg1->x1;
                    } else {
                        arg2[var_r28] = arg1->x0;
                    }
                } else {
                    if ((s32) arg1->x2 != 0x21 && gm_80164840(arg1->x2)) {
                        arg2[var_r28] = arg1->x2;
                    } else {
                        arg2[var_r28] = arg1->x0;
                    }
                }
            }
            var_r28 += 1;
        }
        break;
    }
    }
}
#pragma pop

long fn_80169A84(u8 arg0, s8* arg1, s8* arg2)
{
    s32 i;
    s32 count;
    s8* base;
    s8* p;
    s8* q;
    s8* dst;
    s8* src;
    u8 tmp;
    s8 result;

    base = (s8*) &lbl_8046B488;
    if ((s32) arg0 == 1) {
        i = 0;
        p = base + 0x1C0;
        do {
            if (i != 4 && gm_80164840_noinline((u8) i) != 0) {
                *p = i;
            } else {
                *p = -1;
            }
            i += 1;
            p += 1;
        } while (i < 0x1A);

        i = 0;
        p = base + 0x1C0;
        do {
            q = base + HSD_Randi(0x1B);
            tmp = q[0x1C0];
            i += 1;
            q[0x1C0] = (u8) *p;
            *p = tmp;
            p += 1;
        } while (i < 0x1A);

        count = 0;
        p = base + 0x1C0;
        i = 0xD;
        do {
            if ((s8) p[0] != -1) {
                count += 1;
                if (count > 0x10) {
                    p[0] = -1;
                }
            }
            if ((s8) p[1] != -1) {
                count += 1;
                if (count > 0x10) {
                    p[1] = -1;
                }
            }
            p += 2;
            i -= 1;
        } while (i != 0);

        dst = arg1;
        src = arg2;
        i = 0;
        while ((s8) *src != -2) {
            while ((s8) base[i + 0x1C0] == -1) {
                i = (i + 1) % 27;
            }
            result = Player_800325C8((CharacterKind) (s8) base[i + 0x1C0], 0);
            *dst = result;
            i += 1;
            src += 1;
            dst += 1;
        }
        return (s32) result;
    } else if ((s8) arg0 >= 1) {
        return (s32) base;
    } else if ((s8) arg0 >= 0) {
        dst = arg1;
        src = arg2;
        while ((s8) *src != -2) {
            *dst = -1;
            src += 1;
            dst += 1;
        }
        return (s32) src;
    } else {
        return (s32) base;
    }
}

void fn_80169C54(s8 arg0, s8 arg1)
{
    s32 buf[7];
    UNUSED u8 padB[8];
    struct lbl_8046B488_t* st;
    s8* pc;
    s8* cp;
    s32* bp;
    s32 n;
    s32 count;
    s32 j;
    s32 k;
    s8 p;
    u8 ch;
    s8 c;

    n = 0;
    pc = (s8*) fn_8016AE60();
    st = (struct lbl_8046B488_t*) ((u8*) pc + 0);
    buf[0] = -1;
    buf[1] = -1;
    buf[2] = -1;
    buf[3] = -1;
    buf[4] = -1;
    buf[5] = -1;
    buf[6] = -1;
    bp = buf;
    for (j = 3; j != 0; j--) {
        if ((s32) *pc == 4) {
            if (st->xB == 0) {
                count = gm_80169238_noinline(4U);
                for (k = 0; k < count; k++) {
                    bp[k] = k;
                    n += 1;
                }
            } else {
                n = 1;
                buf[0] = st->xC;
            }
            break;
        }
        pc += 1;
    }

    if (arg0 == 4) {
        buf[n] = arg1;
        n += 1;
    }
    if (n > 0) {
        j = 0;
        cp = (s8*) st;
        do {
            ch = *cp;
            c = (s8) ch;
            if (((s8) ch != 0x21) && ((s8) ch != 4)) {
                p = Player_800325C8((CharacterKind) c, 0);
                if ((p != -1) && (p != 4)) {
                    for (k = 0; k < n; k++) {
                        Player_80031DA8((s32) p, buf[k]);
                    }
                }
                p = Player_800325C8((CharacterKind) c, 1);
                if ((p != -1) && (p != 4)) {
                    for (k = 0; k < n; k++) {
                        Player_80031DA8((s32) p, buf[k]);
                    }
                }
            }
            j += 1;
            cp += 1;
        } while (j < 3);
        if (arg0 != 4) {
            p = Player_800325C8((CharacterKind) arg0, 0);
            if ((p != -1) && (p != 4)) {
                for (k = 0; k < n; k++) {
                    Player_80031DA8((s32) p, buf[k]);
                }
            }
            p = Player_800325C8((CharacterKind) arg0, 1);
            if ((p != -1) && (p != 4)) {
                for (k = 0; k < n; k++) {
                    Player_80031DA8((s32) p, buf[k]);
                }
            }
        }
    }
}

void fn_80169F50(s8 arg0, s8 arg1)
{
    struct lbl_8046B488_t* gp = &lbl_8046B488;
    s32 i;
    s8* p;
    s32 flag;
    s8 v;

    flag = 0;
    if (gp->x0 == 4) {
        flag = 1;
    } else {
        p = &gp->x1;
        if (gp->x1 == 4) {
            flag = 1;
        } else if (p[1] == 4) {
            flag = 1;
        }
    }

    if (flag == 1 && gp->xE != 0) {
        for (i = 0; gp->x20[i] != -2; i++) {
            v = gp->x20[i];
            if (v == -1) {
                continue;
            }
            if (gp->xA2[i] != 4) {
                continue;
            }
            if (gp->x124[i] == -1) {
                continue;
            }
            Player_80031DA8(gp->x124[i], v);
        }
    }

    if (arg0 == 4 && gp->xE != 0) {
        for (i = 0; gp->x20[i] != -2; i++) {
            if (gp->x20[i] == -1) {
                continue;
            }
            if (gp->x124[i] == -1) {
                continue;
            }
            Player_80031DA8(gp->x124[i], arg1);
        }
    }
}

void fn_8016A09C(void)
{
    s32 i;
    s32 var_r29;
    s32 j;
    PAD_STACK(4);
    var_r29 = 0;

    gm_8016AE44();
    lbl_8046B488.unk_10_b1 = 1;
    lbl_8046B488.unk_10_b0 = 0;

    for (i = 0; i < 6; i++) {
        if (Player_GetPlayerSlotType(i) == Gm_PKind_NA) {
            if (var_r29 >= (j = (s32) lbl_8046B488.x9)) {
                break;
            }
            Player_SetFlagsBit1(i);
            var_r29++;
        }
    }

    for (j = 0; j < 6; j++) {
        if (Player_GetFlagsBit1(j) != 0) {
            Player_SetTeam(j, 4);
        }
    }
}

/// @todo Offset mismatch indicates a TU split between #lbl_8046B378 and
///       #lbl_8046B488
void gm_8016A164(void)
{
    int i;
    lbl_8046B6A0_t* match_info = gm_8016AE44();
    PAD_STACK(4);

    match_info->is_singleplayer = false;
    lbl_8046B488.x8 = 0;
    lbl_8046B488.x7 = 0;
    lbl_8046B488.unk_10_b1 = false;
    lbl_8046B488.unk_10_b0 = false;
    fn_8016B138();

    for (i = 1; i < 6; i++) {
        Player_SetSlottype(i, Gm_PKind_NA);
        Player_UnsetFlagsBit1(i);
    }
}

s32 fn_8016A1E4(void)
{
    return lbl_8046B488.unk_10_b0;
}

bool gm_8016A1F8(void)
{
    if (lbl_8046B488.unk_10_b1) {
        return true;
    }
    return false;
}

void gm_8016A21C(StartMeleeRules* arg0)
{
    arg0->x54 = (void*) &lbl_8046B488;
}

static inline s32 fn_801695BC_noinline(u8 arg0, u8 arg1, u8 arg2, u8* arg3,
                                       u8* arg4)
{
    return fn_801695BC(arg0, arg1, arg2, arg3, arg4);
}

static inline void fn_801697FC_noinline(s8 character, s8 costume,
                                        s8 new_character, s8 new_costume,
                                        s8* buf)
{
    fn_801697FC(character, costume, new_character, new_costume, buf);
}

static inline void fn_80169900_noinline(u8 arg0, struct lbl_8046B488_t* arg1,
                                        s8* arg2, s8* arg3)
{
    fn_80169900(arg0, arg1, arg2, arg3);
}

s32 gm_8016A22C(s8 k0, s8 k1, s8 k2, u8 a3, u8 a4, int a5, int mode, int a7,
                u8 color, u8 p87, u8 p8b, int x6, int x7, int x9, int xA,
                int flag2, int flag1, f32 f1, f32 f2)
{
    u8* kinds;
    int i;

    memzero(&lbl_8046B488, 0x1C0);

    lbl_8046B488.x0 = k0;
    lbl_8046B488.x1 = k1;
    lbl_8046B488.x2 = k2;

    lbl_8046B488.x3 = a3;
    lbl_8046B488.x4 = a4;
    lbl_8046B488.x5 = (u8) a5;

    lbl_8046B488.x6 = (u8) x6;
    lbl_8046B488.x7 = (u8) x7;
    lbl_8046B488.x8 = (u8) x7;

    if (x9 > 5) {
        x9 = 5;
    }
    lbl_8046B488.x9 = (u8) x9;

    lbl_8046B488.xA = (u8) xA;

    lbl_8046B488.x14 = f1;
    lbl_8046B488.x18 = f2;

    lbl_8046B488.unk_10_b2 = flag2;
    lbl_8046B488.unk_10_b1 = flag1;

    lbl_8046B488.x1C = 1.0f;

    lbl_8046B488.xB = (u8) mode;
    lbl_8046B488.xD = (u8) a7;
    lbl_8046B488.xE = color;

    lbl_8046B488.xC = a3;

    lbl_8046B488.x1AC = p87;
    lbl_8046B488.x1AD = 0x21;
    lbl_8046B488.x1B2 = p8b;

    memzero(lbl_8046B488.x20, lbl_8046B488.x7);
    lbl_8046B488.x20[lbl_8046B488.x7] = (s8) -2;

    fn_80169900_noinline(lbl_8046B488.xD, &lbl_8046B488, lbl_8046B488.xA2,
                lbl_8046B488.x20);

    kinds = (u8*) &lbl_8046B488.x0;

    switch (lbl_8046B488.xB) {
    case 0:
        for (i = 0; i < 3; i++) {
            fn_801695BC_noinline(kinds[i], p87, p8b, (u8*) lbl_8046B488.xA2,
                        (u8*) lbl_8046B488.x20);
        }
        break;

    case 1:
        for (i = 0; i < 3; i++) {
            fn_801697FC_noinline(
                kinds[i], lbl_8046B488.xC, p87, p8b,
                lbl_8046B488
                    .x20); // This was being inlined, hence the no-inline
        }
        break;
    }

    return fn_80169A84(lbl_8046B488.xE, lbl_8046B488.x124, lbl_8046B488.x20);
}

void gm_8016A404(s32 arg0)
{
    M2C_FIELD(&lbl_8046B488, s32*, 0x1BC) = arg0;
}

void gm_8016A414(f32 arg8)
{
    M2C_FIELD(&lbl_8046B488, f32*, 0x1C) = arg8;
}

void gm_8016A424(s8 arg0)
{
    lbl_8046B488.xF = arg0;
}

void gm_8016A434(void)
{
    lbl_8046B488.unk_10_b3 = 1;
}

void fn_8016A450(void)
{
    lbl_8046B488.unk_10_b4 = 1;
}

void fn_8016A46C(void)
{
    lbl_8046B488.unk_10_b6 = 1;
}

void fn_8016A488(s32 arg0)
{
    if (gm_8016AE44()->hud_enabled == true) {
        Player_80031848(arg0);
    }
}

void fn_8016A4C8(void)
{
    Vec3 spDC;
    f32 var_f1;
    s32 var_r27;
    s32 i;
    s32 j;
    s32 var_r4;
    s32 var_r23;
    s32 r;
    s8 chr;
    u8 cos;
    s32 var_r0;
    void (*fp)(s32, u8);

    var_r27 = 0;
    if (lbl_8046B488.unk_10_b1) {
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    if (var_r0 == 0) {
        return;
    }
    for (i = 0; i < 6; i++) {
        if (Player_GetFlagsBit1(i) != 0 && Player_GetPlayerState(i) == 0 &&
            (s32) lbl_8046B488.x7 > 0) {
            if (lbl_8046B488.x7 != 0) {
                lbl_8046B488.x7 -= 1;
            }
            fn_8016B738(1);
            Player_80036D24(i);
            lbl_8046B488.x1A6[i] = lbl_8046B488.x7;
            Player_SetFlagsBit1(i);
            Player_SetTeam(i, 4);
            Ground_801C2D24(i + (lbl_8046B488.xA - 1), &spDC);
            spDC.y = Stage_GetCamBoundsTopOffset();
            Player_80032768(i, &spDC);
            Player_SetSlottype(i, Gm_PKind_Cpu);
            Player_SetPlayerCharacter(
                i, (CharacterKind) (s8) (u8) lbl_8046B488.xA2[lbl_8046B488.x7]);
            Player_SetStocks(i, 1);
            cos = lbl_8046B488.x20[lbl_8046B488.x7];
            Player_SetCostumeId(i, (s8) cos);
            chr = Player_GetPlayerCharacter(i);
            var_r4 = 0;
            for (j = 0; j < 6; j++) {
                if (Player_GetPlayerSlotType(j) != Gm_PKind_NA &&
                    Player_GetFlagsBit1(j) == 0 &&
                    chr == Player_GetPlayerCharacter(j) &&
                    (s8) cos == Player_GetCostumeId(j)) {
                    var_r4 = 1;
                    break;
                }
            }
            Player_SetControllerIndex(i, var_r4);
            Player_SetMoreFlagsBit6(i, lbl_8046B488.xF);
            Player_SetMoreFlagsBit1(i, 0U);
            if (spDC.x >= 0.0f) {
                var_f1 = -1.0f;
            } else {
                var_f1 = 1.0f;
            }
            Player_SetFacingDirection(i, var_f1);
            Player_SetHUDDamage(i, 0);
            Player_SetPlayerId(i, i);
            Player_SetFlagsBit0(i, 0);
            Player_SetNametagSlotID(i, 0x78);
            Player_SetPlayerAndEntityCpuLevel(i, (s32) lbl_8046B488.x6);
            var_r23 = 0x17;
            if ((s32) lbl_8046B488.x7 != 1) {
                r = HSD_Randi(4);
                if (r != 3) {
                    if (r < 3) {
                        if (r < 0) {
                            var_r23 = 0x17;
                        }
                    } else {
                        var_r23 = 0x17;
                    }
                } else {
                    var_r23 = 0x18;
                }
            }
            Player_SetPlayerAndEntityCpuType(i, var_r23);
            if (lbl_8046B488.unk_10_b4) {
                Player_SetFlagsBit5(i, 1U);
                Player_SetPlayerAndEntityCpuType(i, 0x1B);
            }
            if (lbl_8046B488.unk_10_b6) {
                Player_SetFlagsAEBit0(i, 1U);
            } else {
                Player_SetFlagsAEBit0(i, 0U);
            }
            Player_SetFlagsBit6(i, lbl_8046B488.unk_10_b5);
            Player_SetModelScale(i, lbl_8046B488.x1C);
            Player_SetAttackRatio(i, lbl_8046B488.x14);
            Player_SetDefenseRatio(i, lbl_8046B488.x18);
            if ((s32) lbl_8046B488.x8 > 1) {
                Player_SetMoreFlagsBit5(i, 1U);
            } else {
                Player_SetMoreFlagsBit5(i, 0U);
            }
            if (Player_GetPlayerCharacter(i) == CKIND_KIRBY &&
                lbl_8046B488.xE != 0) {
                Player_SetUnk4D(i, (s8) (u8) lbl_8046B488.x124[lbl_8046B488.x7]);
                Player_SetFlagsAEBit1(i, 1U);
            }
            fp = *(void (**)(s32, u8)) ((u8*) &lbl_8046B488 + 0x1BC);
            if (fp != NULL) {
                fp(i, lbl_8046B488.x7);
            }
            Player_SetStructFunc(i, fn_8016A488);
            Player_80031AD0(i);
            ifStatus_802F6508(i);
            un_802FD28C(i);
        }
    }
    if ((s32) lbl_8046B488.x7 == 0) {
        for (j = 0; j < 6; j++) {
            if (Player_GetPlayerSlotType(j) != Gm_PKind_NA &&
                Player_GetFlagsBit1(j) != 0 && Player_GetStocks(j) != 0) {
                var_r27 = 1;
                break;
            }
        }
        if (var_r27 == 0) {
            lbl_8046B488.unk_10_b0 = 1;
            lbl_8046B488.unk_10_b1 = 0;
            if (lbl_8046B488.x1B8 != NULL && lbl_8046B488.x1B8(1) == 1) {
                lbl_8046B488.x1B8 = NULL;
            }
        }
    }
}
