#include "particle.h"


#include <math.h>
#include <dolphin/gx.h>
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/mcc.h>
#include <dolphin/os.h>
#include <dolphin/pad.h>
#include <dolphin/vi.h>
#include <baselib/cobj.h>
#include <baselib/gobj.h>
#include <baselib/gobjgxlink.h>
#include <baselib/gobjobject.h>
#include <baselib/list.h>
#include <baselib/memory.h>
#include <baselib/mtx.h>
#include <baselib/perf.h>
#include <baselib/psappsrt.h>
#include <baselib/psstructs.h>
#include <baselib/random.h>
#include <baselib/state.h>
#include <baselib/video.h>
#include <MetroTRK/ppc_reg.h>
#include <MSL/trigf.h>

/* The original generator.c TU has no MSL sqrtf weak statics in .sdata2;
 * declare the intrinsic directly instead of pulling in math_ppc.h. */
extern f64 __frsqrte(f64);

/* This file restores an original translation unit of HSD's particle
 * library that had been merged into particle.c ("generator.c" per the
 * DOL assert strings at .data 0x8040C248 ("generator.c", "psCamera");
 * TU evidence: own .sdata2 literal pool at 0x804DE9A8 (dup @391/@535/
 * @3751/@241 copies), .bss base hsd_804D0F90, extab 0x80005588,
 * extabindex 0x8000575C. Derivation: campaign/scratch/tu-split-particle/.
 */

/* .bss (tentative definition, same emission style as before the split) */
/* 4D0F90 */ struct hsd_804D0F60_t hsd_804D0F90;

/* .sbss owned by this TU (declaration order fixes the section layout) */
/* 4D78E8 */ static u32 hsd_804D78E8;
/* 4D78EC */ static u32 hsd_804D78EC;
/* 4D78F0 */ static u32 hsd_804D78F0;
/* 4D78F4 */ static u32 hsd_804D78F4;
/* 4D78F8 */ static u32 hsd_804D78F8;
/* 4D78FC */ static HSD_Generator* hsd_804D78FC;
/* 4D7900 */ static u32 hsd_804D7900;

/* Owned by the particle.c TU; this TU reads them as externs (the DOL
 * shows per-site named materializations = consumer-TU style, idiom 46). */
extern u16 hsd_804D78DA;
extern u16 hsd_804D78E0;
extern HSD_PSTexGroup** psTexGroupArray[65];
/* NOTE: symbols.txt names are historical; binary-proven semantics
 * (psInitDataBankLoad stores, byte-matched): D0D58 "psCmdListArray"
 * holds per-bank counts, D0E5C "ptclref_804D0E5C" holds the per-bank
 * HSD_PSCmdList* tables. */
extern int psCmdListArray[65];
extern HSD_PSCmdList** ptclref_804D0E5C[65];

/* .sdata */
/* 4D6368 */ static u16 lbl_804D6368 = 0x100;

u16 hsd_8039D1EC(void)
{
    lbl_804D6368++;
    if (lbl_804D6368 < 256) {
        lbl_804D6368 = 256;
    }
    return lbl_804D6368;
}

static char lbl_804D636C[] = "jobj.h";
static char lbl_804D6374[] = "jobj";

/* Duplicates of the jobj.h inlines: this original TU emitted its own copies
 * of the assert strings (lbl_804D636C/lbl_804D6374). */
static inline bool HSD_JObjMtxIsDirty_dup(HSD_JObj* jobj)
{
    bool result;
    (jobj) ? ((void) 0) : __assert(lbl_804D636C, 564, lbl_804D6374);
    result = false;
    if (!(jobj->flags & JOBJ_USER_DEF_MTX) && (jobj->flags & JOBJ_MTX_DIRTY)) {
        result = true;
    }
    return result;
}

static inline void HSD_JObjSetupMatrix_dup(HSD_JObj* jobj)
{
    if (!jobj || !HSD_JObjMtxIsDirty_dup(jobj)) {
        return;
    }
    HSD_JObjSetupMatrixSub(jobj);
}

void hsd_8039D214(HSD_Generator* gen)
{
    HSD_JObj* jobj;

    if (!(gen->type & 0x100)) {
        return;
    }

    if ((jobj = gen->jobj) == NULL) {
        return;
    }

    HSD_JObjSetupMatrix_dup(jobj);

    if (gen->type & 0x200) {
        gen->pos.x = gen->jobj->mtx[0][3];
        gen->pos.y = gen->jobj->mtx[1][3];
        gen->pos.z = gen->jobj->mtx[2][3];
    }

    if (gen->type & 0x800) {
        if (gen->appsrt != NULL && gen->appsrt->gp == gen) {
            gen->appsrt->translate.x = gen->jobj->mtx[0][3];
            gen->appsrt->translate.y = gen->jobj->mtx[1][3];
            gen->appsrt->translate.z = gen->jobj->mtx[2][3];
        }
    }

    if (gen->type & 0x1000) {
        if (gen->appsrt != NULL && gen->appsrt->gp == gen) {
            HSD_MtxGetScale(gen->jobj->mtx, &gen->appsrt->scale);
        }
    }
}

void hsd_8039D354(u32 unused)
{
    HSD_ObjAllocInit(&hsd_804D0F90.alloc_data, 0x94, 4);
    hsd_804D78FC = NULL;
    hsd_804D78E0 = 0;
    hsd_804D78DA = 0;
    hsd_804D78F4 = 0;
    hsd_804D78F0 = 0;
    hsd_804D78E8 = 0;
    hsd_804D78EC = 0;
    hsd_804D78F8 = 0;
    hsd_804D7900 = 0;
}

HSD_Generator* hsd_8039D3AC(HSD_Generator* gen, HSD_Generator* prev)
{
    if (gen->type & 0x80) {
        hsd_8039D0A0(gen);
    }
    if (gen->numChild != 0) {
        gen->random = 0.0F;
        gen->genLife = 1;
        return gen;
    }
    if (gen->type & 0x100) {
        if (gen->jobj != NULL) {
            if (gen->type & 0x1800) {
                HSD_psAppSRT* srt = gen->appsrt;
                if (srt != NULL) {
                    if (srt->gp == gen) {
                        if (srt->usedCount != 1) {
                            gen->random = 0.0F;
                            gen->genLife = 1;
                            return gen;
                        }
                    }
                }
            }
        }
    }
    if (prev == NULL) {
        hsd_804D78FC = gen->next;
    } else {
        prev->next = gen->next;
    }
    if (gen->appsrt != NULL) {
        psRemoveGeneratorSRT(gen);
    }
    if (gen->jobj != NULL) {
        HSD_JObjUnref(gen->jobj);
        gen->jobj = NULL;
    }
    HSD_ObjFree(&hsd_804D0F90.alloc_data, gen);
    hsd_804D78E0--;
    return prev;
}

void hsd_8039D4DC(HSD_Generator* gen)
{
    HSD_Generator* cur;

    cur = hsd_804D78FC;
    hsd_804D78F8 = 0;
    while (cur != NULL) {
        if (cur == gen) {
            hsd_804D78F8 =
                (u32) hsd_8039D3AC(gen, (HSD_Generator*) hsd_804D78F8);
            if (hsd_804D78F8 != 0) {
                while (((HSD_Generator*) hsd_804D78F8)->next != NULL) {
                    hsd_804D78F8 = (u32) ((HSD_Generator*) hsd_804D78F8)->next;
                }
            } else if (hsd_804D78FC != NULL) {
                hsd_804D78F8 = (u32) hsd_804D78FC;
                while (((HSD_Generator*) hsd_804D78F8)->next != NULL) {
                    hsd_804D78F8 = (u32) ((HSD_Generator*) hsd_804D78F8)->next;
                }
            }
            return;
        }
        hsd_804D78F8 = (u32) cur;
        cur = cur->next;
    }
}

void hsd_8039D580(HSD_JObj* jobj)
{
    HSD_Generator* next;
    HSD_Generator* cur;

    if (jobj != NULL) {
        cur = hsd_804D78FC;
        while (cur != NULL) {
            next = cur->next;
            if (cur->jobj == jobj) {
                hsd_8039D4DC(cur);
            }
            cur = next;
        }
    }
}

void hsd_8039D5DC(HSD_JObj* jobj)
{
    HSD_Generator* next;
    HSD_Generator* gen;

    if (jobj == NULL) {
        return;
    }
    gen = hsd_804D78FC;
    while (gen != NULL) {
        next = gen->next;
        if (gen->jobj == jobj) {
            hsd_8039D4DC(gen);
        }
        gen = next;
    }
    if (jobj->flags & 0x1000) {
        return;
    }
    jobj = jobj == NULL ? NULL : jobj->child;
    while (jobj != NULL) {
        hsd_8039D5DC(jobj);
        jobj = jobj == NULL ? NULL : jobj->next;
    }
}

void hsd_8039D688(HSD_JObj* jobj, f32** unused1, s32 unused2)
{
    HSD_Generator* next;
    HSD_Generator* gen;

    gen = hsd_804D78FC;
    hsd_804D78F8 = 0;
    while (gen != NULL) {
        next = gen->next;
        if (gen->jobj == jobj) {
            if (gen->appsrt != NULL) {
                u16 type = gen->type;
                if (type & 0x100) {
                    gen->type = type | 0x80;
                }
            }
            hsd_804D78F8 =
                (u32) hsd_8039D3AC(gen, (HSD_Generator*) hsd_804D78F8);
        } else {
            hsd_804D78F8 = (u32) gen;
        }
        gen = next;
    }
}

void hsd_8039D71C(HSD_Generator* gen)
{
    HSD_JObj* jobj;
    Mtx mtx;
    Vec3 col;
    f32 col0_x, col0_y, col0_z;
    f32 col1_x, col1_y, col1_z;
    PAD_STACK(12);

    if (!(gen->type & 0x100)) {
        return;
    }

    if ((jobj = gen->jobj) == NULL) {
        return;
    }

    HSD_JObjSetupMatrix_dup(jobj);

    if (gen->type & 0x200) {
        gen->pos.x = gen->jobj->mtx[0][3];
        gen->pos.y = gen->jobj->mtx[1][3];
        gen->pos.z = gen->jobj->mtx[2][3];
    }

    if (gen->type & 0x800) {
        if (gen->appsrt != NULL && gen->appsrt->gp == gen) {
            gen->appsrt->translate.x = gen->jobj->mtx[0][3];
            gen->appsrt->translate.y = gen->jobj->mtx[1][3];
            gen->appsrt->translate.z = gen->jobj->mtx[2][3];
        }
    }

    if (gen->type & 0x1000) {
        if (gen->appsrt != NULL && gen->appsrt->gp == gen) {
            HSD_MtxGetScale(gen->jobj->mtx, &gen->appsrt->scale);
        }
    }

    col.x = jobj->mtx[0][0];
    col.y = jobj->mtx[1][0];
    col.z = jobj->mtx[2][0];
    PSVECNormalize(&col, &col);
    col0_x = col.x;
    col0_y = col.y;
    col0_z = col.z;

    col.x = jobj->mtx[0][1];
    col.y = jobj->mtx[1][1];
    col.z = jobj->mtx[2][1];
    PSVECNormalize(&col, &col);
    col1_x = col.x;
    col1_y = col.y;
    col1_z = col.z;

    col.x = jobj->mtx[0][2];
    col.y = jobj->mtx[1][2];
    col.z = jobj->mtx[2][2];
    PSVECNormalize(&col, &col);

    {
        f32 cx = col.x;
        f32 cy = col.y;
        f32 cz = col.z;
        f32 vx = gen->vel.x;
        f32 vy = gen->vel.y;
        f32 vz = gen->vel.z;
        gen->vel.x = col0_x * vx + col1_x * vy + cx * vz;
        gen->vel.y = col0_y * vx + col1_y * vy + cy * vz;
        gen->vel.z = col0_z * vx + col1_z * vy + cz * vz;
    }

    if ((gen->type & 0xF) == 1) {
        f32 ax = gen->aux.line.x2;
        f32 ay = gen->aux.line.y2;
        f32 az = gen->aux.line.z2;
        gen->aux.line.x2 =
            jobj->mtx[0][0] * ax + jobj->mtx[0][1] * ay + jobj->mtx[0][2] * az;
        gen->aux.line.y2 =
            jobj->mtx[1][0] * ax + jobj->mtx[1][1] * ay + jobj->mtx[1][2] * az;
        gen->aux.line.z2 =
            jobj->mtx[2][0] * ax + jobj->mtx[2][1] * ay + jobj->mtx[2][2] * az;
    }
}

HSD_Generator* hsd_8039D9C8(void)
{
    HSD_Generator* gen;
    PAD_STACK(8);

    gen = HSD_ObjAlloc(&hsd_804D0F90.alloc_data);
    if (gen != NULL) {
        memset(gen, 0, 0x94);
    }
    if (gen == NULL) {
        return NULL;
    }

    hsd_804D78E0++;
    if (hsd_804D78E0 > hsd_804D78DA) {
        hsd_804D78DA = hsd_804D78E0;
    }

    if ((HSD_Generator*) hsd_804D78F8 == NULL ||
        ((HSD_Generator*) hsd_804D78F8)->next == NULL)
    {
        if (hsd_804D78FC == NULL) {
            gen->next = NULL;
            hsd_804D78FC = gen;
        } else {
            gen->next = hsd_804D78FC->next;
            hsd_804D78FC->next = gen;
        }
    } else {
        gen->next = ((HSD_Generator*) hsd_804D78F8)->next->next;
        ((HSD_Generator*) hsd_804D78F8)->next->next = gen;
    }

    lbl_804D6368++;
    if (lbl_804D6368 < 0x100) {
        lbl_804D6368 = 0x100;
    }
    gen->idnum = lbl_804D6368;
    gen->appsrt = NULL;
    gen->userfunc = NULL;
    return gen;
}

static char lbl_8040C248[] = "generator.c";
static char lbl_8040C254[] = "psCamera";

/* The original TU's HSD_ASSERT(677, psCamera) emitted its own copies of
 * the file/condition strings (idiom 27); psCamera was the original name
 * of hsd_804D78F0. */

f32 hsd_8039DAD4(HSD_Generator* gen)
{
    Vec3 vel_copy;
    u8 _pad[4];
    Vec3 emit_pos;
    Vec3 vel_temp;
    Vec3 vel_out;
    Mtx rot_mtx;
    Mtx jobj_mtx;
    Vec3 look_dir;
    Vec3 cam_up;
    Vec3 cross1;
    Vec3 vel_norm;
    Mtx trig_mtx;
    f32 vel_mag_sq;
    f32 angle1;
    f32 sin_az;
    f32 cos_az;
    f32 sin_el;
    f32 elevation;
    f32 min_a;
    f32 angle_step;
    f32 cur_angle;
    f32 radius;
    f32 cone_angle;
    f32 angle3;
    volatile f32 y1;
    f32 abs_z;
    f32 abs_comb;
    f32 abs_r22;
    f32 abs_comb2;
    volatile f32 y2;
    f32 abs_sa;
    f32 abs_sa2;
    volatile f32 y3;
    volatile f32 y4;
    volatile f32 y5;
    volatile f32 y6;
    PAD_STACK(4);

    angle1 = 0.0F;
    angle3 = angle1;

    if (gen->count < 1.0F) {
        return gen->count;
    }

    /* Copy velocity */
    vel_copy.x = gen->vel.x;
    vel_copy.y = gen->vel.y;
    vel_copy.z = gen->vel.z;

    /* Compute velocity magnitude */
    if ((vel_mag_sq = vel_copy.x * vel_copy.x + vel_copy.y * vel_copy.y +
                      vel_copy.z * vel_copy.z) > 0.0F)
    {
        f64 guess = __frsqrte(vel_mag_sq);
        guess = 0.5 * guess * (3.0 - vel_mag_sq * (guess * guess));
        guess = 0.5 * guess * (3.0 - vel_mag_sq * (guess * guess));
        guess = 0.5 * guess * (3.0 - vel_mag_sq * (guess * guess));
        y1 = (f32) (vel_mag_sq * guess);
        vel_mag_sq = y1;
    }

    /* Initialize rotation matrix */
    PSMTXIdentity(rot_mtx);

    /* JObj-based rotation: type & 0x100 and jobj!=NULL and type & 0x400
       and !(kind & 0x3C000) */
    if ((gen->type & 0x100) && gen->jobj != NULL && (gen->type & 0x400) &&
        !(gen->kind & 0x30000))
    {
        PSMTXCopy((void*) ((u8*) gen->jobj + 0x44), jobj_mtx);

        /* Extract and normalize each column of the 3x3 rotation */
        vel_out.x = jobj_mtx[0][0];
        vel_out.y = jobj_mtx[1][0];
        vel_out.z = jobj_mtx[2][0];
        PSVECNormalize(&vel_out, &vel_out);
        rot_mtx[0][0] = vel_out.x;
        rot_mtx[1][0] = vel_out.y;
        rot_mtx[2][0] = vel_out.z;

        vel_out.x = jobj_mtx[0][1];
        vel_out.y = jobj_mtx[1][1];
        vel_out.z = jobj_mtx[2][1];
        PSVECNormalize(&vel_out, &vel_out);
        rot_mtx[0][1] = vel_out.x;
        rot_mtx[1][1] = vel_out.y;
        rot_mtx[2][1] = vel_out.z;

        vel_out.x = jobj_mtx[0][2];
        vel_out.y = jobj_mtx[1][2];
        vel_out.z = jobj_mtx[2][2];
        PSVECNormalize(&vel_out, &vel_out);
        rot_mtx[0][2] = vel_out.x;
        rot_mtx[1][2] = vel_out.y;
        rot_mtx[2][2] = vel_out.z;

        rot_mtx[2][3] = 0.0F;
        rot_mtx[1][3] = 0.0F;
        rot_mtx[0][3] = 0.0F;
    }

    /* Billboard orientation: kind & 0x10000 */
    if (gen->kind & 0x10000) {
        ((void) ((hsd_804D78F0)
                     ? ((void) 0)
                     : __assert(lbl_8040C248, 677, lbl_8040C254)));
        {
            HSD_CObj* cobj = (HSD_CObj*) hsd_804D78F0;
            void* view = *(void**) ((u8*) cobj + 0x24);
            look_dir.x = *(f32*) ((u8*) view + 0x0C) - gen->pos.x;
            view = *(void**) ((u8*) cobj + 0x24);
            look_dir.y = *(f32*) ((u8*) view + 0x10) - gen->pos.y;
            view = *(void**) ((u8*) cobj + 0x24);
            look_dir.z = *(f32*) ((u8*) view + 0x14) - gen->pos.z;
        }
        PSVECNormalize(&look_dir, &look_dir);
        HSD_CObjGetUpVector((HSD_CObj*) hsd_804D78F0, &cam_up);
        PSVECNormalize(&cam_up, &cam_up);
        PSVECCrossProduct(&cam_up, &look_dir, &cross1);
        PSVECCrossProduct(&look_dir, &cross1, &cam_up);

        rot_mtx[0][0] = cross1.x;
        rot_mtx[1][0] = cross1.y;
        rot_mtx[2][0] = cross1.z;
        rot_mtx[0][1] = cam_up.x;
        rot_mtx[1][1] = cam_up.y;
        rot_mtx[2][1] = cam_up.z;
        rot_mtx[0][2] = look_dir.x;
        rot_mtx[1][2] = look_dir.y;
        rot_mtx[2][2] = look_dir.z;
    }

    /* Velocity-based rotation */
    if ((gen->type & 0xF) != 1 && vel_mag_sq > 1.1920929e-7F) {
        vel_norm.x = gen->vel.x;
        vel_norm.y = gen->vel.y;
        vel_norm.z = gen->vel.z;
        PSVECNormalize(&vel_norm, &vel_norm);

        {
            abs_z = vel_norm.z;
            *(s32*) &abs_z &= 0x7FFFFFFF;
            if (abs_z < 1.1754944e-38F) {
                if (vel_norm.y >= 0.0F) {
                    sin_az = 1.5707964F;
                } else {
                    sin_az = -1.5707964F;
                }
            } else {
                sin_az = atan2f(vel_norm.y, vel_norm.z);
            }
        }
        cos_az = sinf(sin_az);
        sin_az = cosf(sin_az);

        {
            f32 comb = vel_norm.y * cos_az + vel_norm.z * sin_az;
            abs_comb = comb;
            *(s32*) &abs_comb &= 0x7FFFFFFF;
            if (abs_comb < 1.1754944e-38F) {
                if (vel_norm.x >= 0.0F) {
                    elevation = 1.5707964F;
                } else {
                    elevation = -1.5707964F;
                }
            } else {
                elevation = atan2f(vel_norm.x, comb);
            }
            sin_el = sinf(elevation);
            {
                f32 cos_el = cosf(elevation);
                f32 neg_cos_az = -cos_az;
                f32 neg_sin_az_s = -sin_az;

                trig_mtx[0][0] = cos_el;
                trig_mtx[0][1] = 0.0F;
                trig_mtx[0][2] = sin_el;
                trig_mtx[0][3] = 0.0F;
                trig_mtx[1][0] = neg_cos_az * sin_el;
                trig_mtx[1][1] = sin_az;
                trig_mtx[1][2] = cos_az * cos_el;
                trig_mtx[1][3] = 0.0F;
                trig_mtx[2][0] = neg_sin_az_s * sin_el;
                trig_mtx[2][1] = neg_cos_az;
                trig_mtx[2][2] = sin_az * cos_el;
                trig_mtx[2][3] = 0.0F;
            }
        }
        PSMTXConcat(rot_mtx, trig_mtx, rot_mtx);
    }

    /* Type 2: extract Euler angles from rotation matrix */
    if ((gen->type & 0xF) == 2) {
        {
            abs_r22 = rot_mtx[2][2];
            *(s32*) &abs_r22 &= 0x7FFFFFFF;
            if (abs_r22 < 1.1754944e-38F) {
                if (rot_mtx[1][2] >= 0.0F) {
                    angle1 = 1.5707964F;
                } else {
                    angle1 = -1.5707964F;
                }
            } else {
                angle1 = atan2f(rot_mtx[1][2], rot_mtx[2][2]);
            }
        }
        {
            f32 comb =
                rot_mtx[1][2] * sinf(angle1) + rot_mtx[2][2] * cosf(angle1);
            abs_comb2 = comb;
            *(s32*) &abs_comb2 &= 0x7FFFFFFF;
            if (abs_comb2 < 1.1754944e-38F) {
                if (rot_mtx[0][2] >= 0.0F) {
                    angle3 = 1.5707964F;
                } else {
                    angle3 = -1.5707964F;
                }
            } else {
                angle3 = atan2f(rot_mtx[0][2], comb);
            }
        }
    }

    /* Angle step computation (pre-loop) */
    if (gen->angle < 0.0F) {
        switch (gen->type & 0xF) {
        case 0:
        case 3:
        case 4: {
            f32 rnd;
            f32 range;
            min_a = gen->aux.disc.minAngle;
            rnd = HSD_Randf();
            range = gen->aux.disc.maxAngle - min_a;
            angle_step = range / (f32) (s32) gen->count;
            cur_angle = angle_step * rnd + min_a;
            break;
        }
        case 6:
        case 7: {
            f32 rnd;
            f32 range;
            min_a = gen->aux.cone.minAngle;
            rnd = HSD_Randf();
            range = gen->aux.cone.maxAngle - min_a;
            angle_step = range / (f32) (s32) gen->count;
            cur_angle = angle_step * rnd + min_a;
            break;
        }
        default: {
            f32 rnd = HSD_Randf();
            cur_angle = (f32) (2.0 * (M_PI * rnd));
            angle_step = (f32) (M_TAU / (s32) gen->count);
            break;
        }
        }
    }

    /* Main particle emission loop */
    /* binary-proven: the DOL's epsilon is (f64)(f32)0.001 (lbl_804DE9F8) */
    while (gen->count >= 1.0F) {
        switch (gen->type & 0xF) {
        case 0: /* point, disc, cone, sphere, etc. */
        case 3:
        case 4:
        case 6:
        case 7: {
            /* Compute radius */
            if (gen->radius < 0.0F) {
                sin_az = -gen->radius;
                radius = 1.0F;
            } else {
                radius = HSD_Randf();
                if ((gen->type & 0xF) == 3 || (gen->type & 0xF) == 4) {
                    f32 t;
                    if (radius > 0.0F) {
                        f64 guess = __frsqrte(radius);
                        guess = 0.5 * guess * (3.0 - radius * (guess * guess));
                        guess = 0.5 * guess * (3.0 - radius * (guess * guess));
                        guess = 0.5 * guess * (3.0 - radius * (guess * guess));
                        y2 = (f32) (radius * guess);
                        t = y2;
                    } else {
                        t = radius;
                    }
                    radius = t;
                }
                sin_az = radius * gen->radius;
            }

            /* Compute emission angle per shape */
            switch (gen->type & 0xF) {
            case 6:
                /* Cone/sphere shape 6 */
                if (gen->angle < 0.0F) {
                    abs_sa = sin_az;
                    cur_angle += angle_step;
                    *(s32*) &abs_sa &= 0x7FFFFFFF;
                    if (abs_sa < 1.1754944e-38F) {
                        if (gen->aux.cone.height >= 0.0F) {
                            cone_angle = -gen->angle;
                        } else {
                            cone_angle = (f32) (M_PI - gen->angle);
                        }
                    } else {
                        cone_angle =
                            (f32) (M_PI_2 -
                                   atan2f(gen->aux.cone.height, sin_az) -
                                   gen->angle);
                    }
                } else {
                    cur_angle = gen->aux.cone.minAngle;
                    {
                        f32 rnd = HSD_Randf();
                        f32 range = gen->aux.cone.maxAngle - cur_angle;
                        cur_angle = range * rnd + cur_angle;
                    }
                    abs_sa2 = sin_az;
                    *(s32*) &abs_sa2 &= 0x7FFFFFFF;
                    if (abs_sa2 < 1.1754944e-38F) {
                        if (gen->aux.cone.height >= 0.0F) {
                            cone_angle = gen->angle;
                        } else {
                            cone_angle = (f32) (M_PI + gen->angle);
                        }
                    } else {
                        cone_angle =
                            (f32) (gen->angle +
                                   (M_PI_2 -
                                    atan2f(gen->aux.cone.height, sin_az)));
                    }
                }
                break;
            case 7:
                /* Shape 7 */
                if (gen->angle < 0.0F) {
                    cone_angle = (f32) (M_PI_2 - gen->angle);
                    cur_angle += angle_step;
                } else {
                    cur_angle = gen->aux.disc.minAngle;
                    {
                        f32 rnd = HSD_Randf();
                        f32 range = gen->aux.disc.maxAngle - cur_angle;
                        cur_angle = range * rnd + cur_angle;
                        cone_angle = (f32) (M_PI_2 + gen->angle);
                    }
                }
                break;
            default:
                /* Shapes 0, 3, 4 */
                if (gen->angle < 0.0F) {
                    cone_angle = -gen->angle;
                    cur_angle += angle_step;
                    cone_angle = radius * cone_angle;
                } else {
                    cur_angle = gen->aux.disc.minAngle;
                    {
                        f32 rnd = HSD_Randf();
                        f32 range = gen->aux.disc.maxAngle - cur_angle;
                        cone_angle = radius * gen->angle;
                        cur_angle = range * rnd + cur_angle;
                    }
                }
                break;
            }

            /* Compute emission position from angle */
            emit_pos.x = sin_az * cosf(cur_angle);
            emit_pos.y = sin_az * sinf(cur_angle);

            /* Shape 6,7: z from random, scale by height */
            if ((gen->type & 0xF) == 6 || (gen->type & 0xF) == 7) {
                emit_pos.z = HSD_Randf();
                if ((gen->type & 0xF) == 6) {
                    emit_pos.x = emit_pos.x * (1.0F - emit_pos.z);
                    emit_pos.y = emit_pos.y * (1.0F - emit_pos.z);
                }
                emit_pos.z = emit_pos.z * gen->aux.cone.height;
            } else {
                emit_pos.z = 0.0F;
            }

            /* Compute velocity direction from cone angle */
            sin_az = vel_mag_sq * sinf(cone_angle);
            vel_out.x = sin_az * cosf(cur_angle);
            vel_out.y = sin_az * sinf(cur_angle);
            vel_out.z = vel_mag_sq * cosf(cone_angle);

            /* Shape 3: scale velocity by radius factor */
            if ((gen->type & 0xF) == 3) {
                vel_out.x *= radius;
                vel_out.y *= radius;
                vel_out.z *= radius;
            }

            /* Apply rotation matrix to emission position */
            PSMTXMultVec(rot_mtx, &emit_pos, &emit_pos);
            emit_pos.x += gen->pos.x;
            emit_pos.y += gen->pos.y;
            emit_pos.z += gen->pos.z;

            /* Apply rotation matrix to velocity */
            PSMTXMultVec(rot_mtx, &vel_out, &vel_out);

            /* Spawn particle */
            hsd_80398F0C(gen->linkNo, gen->bank, gen->kind, gen->texGroup,
                         (s32) gen->cmdList, gen->life, 0, (s32) gen,
                         emit_pos.x, emit_pos.y, emit_pos.z, vel_out.x,
                         vel_out.y, vel_out.z, gen->size, gen->grav,
                         gen->fric);
            break;
        }

        case 1: /* line */
        {
            f32 rnd = HSD_Randf();
            emit_pos.x = rnd * gen->aux.line.x2;
            emit_pos.y = rnd * gen->aux.line.y2;
            emit_pos.z = rnd * gen->aux.line.z2;

            PSMTXMultVec(rot_mtx, &emit_pos, &emit_pos);
            emit_pos.x += gen->pos.x;
            emit_pos.y += gen->pos.y;
            emit_pos.z += gen->pos.z;

            PSMTXMultVec(rot_mtx, &vel_copy, &vel_out);

            hsd_80398F0C(gen->linkNo, gen->bank, gen->kind, gen->texGroup,
                         (s32) gen->cmdList, gen->life, 0, (s32) gen,
                         emit_pos.x, emit_pos.y, emit_pos.z, vel_out.x,
                         vel_out.y, vel_out.z, gen->size, gen->grav,
                         gen->fric);
            break;
        }

        case 2: /* tornado */
        {
            if (gen->radius < 0.0F) {
                sin_az = 1.0F;
            } else {
                sin_az = HSD_Randf();
            }
            if (gen->angle < 0.0F) {
                cur_angle += angle_step;
            } else {
                f32 rnd = HSD_Randf();
                cur_angle = (f32) (2.0 * (M_PI * rnd));
            }
            gen->aux.line.x2 = vel_mag_sq;
            hsd_80398F0C(gen->linkNo, gen->bank, gen->kind | 4, gen->texGroup,
                         (s32) gen->cmdList, gen->life, 0, (s32) gen, 0.0F,
                         0.0F, 0.0F, cur_angle, sin_az, 0.0F, gen->size,
                         angle1, angle3);
            break;
        }

        case 5: /* rect */
        {
            emit_pos.x = HSD_Randf();
            emit_pos.y = HSD_Randf();
            emit_pos.z = HSD_Randf();

            switch (gen->aux.rect.flag) {
            case 0: /* default: no modification */
                break;
            case 1: /* +x */
                emit_pos.x = emit_pos.x > 0.5F ? 1.0F : 0.0F;
                break;
            case 2: /* +y */
                emit_pos.y = emit_pos.y > 0.5F ? 1.0F : 0.0F;
                break;
            case 3: /* xy split */
            {
                f32 rnd = HSD_Randf();
                f32 a2 = gen->aux.rect.x;
                f32 b2 = gen->aux.rect.y;
                if (rnd > a2 / (a2 + b2)) {
                    emit_pos.y = emit_pos.y > 0.5F ? 1.0F : 0.0F;
                } else {
                    emit_pos.x = emit_pos.x > 0.5F ? 1.0F : 0.0F;
                }
                break;
            }
            case 4: /* +z */
                emit_pos.z = emit_pos.z > 0.5F ? 1.0F : 0.0F;
                break;
            case 5: /* xz split */
            {
                f32 rnd = HSD_Randf();
                f32 a2 = gen->aux.rect.x;
                f32 b2 = gen->aux.rect.z;
                if (rnd > a2 / (a2 + b2)) {
                    emit_pos.z = emit_pos.z > 0.5F ? 1.0F : 0.0F;
                } else {
                    emit_pos.x = emit_pos.x > 0.5F ? 1.0F : 0.0F;
                }
                break;
            }
            case 6: /* yz split */
            {
                f32 rnd = HSD_Randf();
                f32 a2 = gen->aux.rect.y;
                f32 b2 = gen->aux.rect.z;
                if (rnd > a2 / (a2 + b2)) {
                    emit_pos.z = emit_pos.z > 0.5F ? 1.0F : 0.0F;
                } else {
                    emit_pos.y = emit_pos.y > 0.5F ? 1.0F : 0.0F;
                }
                break;
            }
            case 7: /* 3-way split */
            {
                f32 rnd = HSD_Randf();
                f32 a2 = gen->aux.rect.z;
                f32 b2 = gen->aux.rect.y;
                f32 c2 = gen->aux.rect.x;
                f32 ab = b2 * a2;
                f32 ca = c2 * b2;
                f32 total = c2 * (b2 + a2) + ab;
                f32 r0 = 1.0F / total;
                f32 t1 = r0 * ca;
                if (rnd < t1) {
                    emit_pos.z = emit_pos.z > 0.5F ? 1.0F : 0.0F;
                } else if (rnd > 1.0F - r0 * (c2 * a2)) {
                    emit_pos.y = emit_pos.y > 0.5F ? 1.0F : 0.0F;
                } else {
                    emit_pos.x = emit_pos.x > 0.5F ? 1.0F : 0.0F;
                }
                break;
            }
            default:
                break;
            }

            /* Common rect post-processing */
            emit_pos.x = emit_pos.x - 0.5F;
            emit_pos.y = emit_pos.y - 0.5F;
            emit_pos.z = emit_pos.z - 0.5F;

            /* Multiply by rect's 3x3 matrix */
            vel_temp.x = gen->aux.rect.xx * emit_pos.x +
                         gen->aux.rect.yx * emit_pos.y +
                         gen->aux.rect.zx * emit_pos.z;
            vel_temp.y = gen->aux.rect.xy * emit_pos.x +
                         gen->aux.rect.yy * emit_pos.y +
                         gen->aux.rect.zy * emit_pos.z;
            vel_temp.z = gen->aux.rect.xz * emit_pos.x +
                         gen->aux.rect.yz * emit_pos.y +
                         gen->aux.rect.zz * emit_pos.z;

            PSMTXMultVec(rot_mtx, &vel_temp, &emit_pos);
            emit_pos.x += gen->pos.x;
            emit_pos.y += gen->pos.y;
            emit_pos.z += gen->pos.z;

            /* Compute velocity from rect axes */
            {
                f32 scale;
                f32 mag = gen->aux.rect.zz * gen->aux.rect.zz +
                          (gen->aux.rect.zx * gen->aux.rect.zx +
                           gen->aux.rect.zy * gen->aux.rect.zy);
                if (mag > 0.0F) {
                    f64 guess = __frsqrte(mag);
                    guess = 0.5 * guess * (3.0 - mag * (guess * guess));
                    guess = 0.5 * guess * (3.0 - mag * (guess * guess));
                    guess = 0.5 * guess * (3.0 - mag * (guess * guess));
                    y3 = (f32) (mag * guess);
                    mag = y3;
                }
                scale = vel_mag_sq / mag;
                vel_out.x = gen->aux.rect.zx * scale;
                vel_out.y = gen->aux.rect.zy * scale;
                vel_out.z = gen->aux.rect.zz * scale;
            }

            PSMTXMultVec(rot_mtx, &vel_out, &vel_out);

            hsd_80398F0C(gen->linkNo, gen->bank, gen->kind, gen->texGroup,
                         (s32) gen->cmdList, gen->life, 0, (s32) gen,
                         emit_pos.x, emit_pos.y, emit_pos.z, vel_out.x,
                         vel_out.y, vel_out.z, gen->size, gen->grav,
                         gen->fric);
            break;
        }

        case 8: /* sphere emission */
        {
            if (0.0F == gen->aux.cone.height ||
                __fabs(gen->aux.cone.height - M_PI) < 0.001F)
            {
                /* Uniform sphere emission */
                f32 rnd = HSD_Randf();
                if (rnd > 0.0F) {
                    f64 guess = __frsqrte(rnd);
                    guess = 0.5 * guess * (3.0 - rnd * (guess * guess));
                    guess = 0.5 * guess * (3.0 - rnd * (guess * guess));
                    guess = 0.5 * guess * (3.0 - rnd * (guess * guess));
                    y4 = (f32) (rnd * guess);
                    rnd = y4;
                }
                radius = (f32) (M_PI_2 * rnd);
                if (HSD_Randf() < 0.5F) {
                    radius = (f32) (M_PI - radius);
                }
            } else {
                f32 rnd = HSD_Randf();
                if (rnd > 0.0F) {
                    f64 guess = __frsqrte(rnd);
                    guess = 0.5 * guess * (3.0 - rnd * (guess * guess));
                    guess = 0.5 * guess * (3.0 - rnd * (guess * guess));
                    guess = 0.5 * guess * (3.0 - rnd * (guess * guess));
                    y5 = (f32) (rnd * guess);
                    rnd = y5;
                }
                radius = gen->aux.cone.height * rnd;
            }
            {
                f32 rnd = HSD_Randf();
                cone_angle = (f32) (2.0 * (M_PI * rnd));
                sin_az = gen->radius;
                if (sin_az < 0.0F) {
                    sin_az = -sin_az;
                } else {
                    f32 rnd2 = HSD_Randf();
                    if (rnd2 > 0.0F) {
                        f64 guess = __frsqrte(rnd2);
                        guess = 0.5 * guess * (3.0 - rnd2 * (guess * guess));
                        guess = 0.5 * guess * (3.0 - rnd2 * (guess * guess));
                        guess = 0.5 * guess * (3.0 - rnd2 * (guess * guess));
                        y6 = (f32) (rnd2 * guess);
                        rnd2 = y6;
                    }
                    sin_az = sin_az * rnd2;
                }
            }

            vel_out.x = sinf(radius) * cosf(cone_angle);
            vel_out.y = sinf(radius) * sinf(cone_angle);
            vel_out.z = cosf(radius);

            PSMTXMultVec(rot_mtx, &vel_out, &emit_pos);

            /* Scale position by aux.rect.x */
            vel_out.x = emit_pos.x * gen->aux.rect.x;
            vel_out.y = emit_pos.y * gen->aux.rect.x;
            vel_out.z = emit_pos.z * gen->aux.rect.x;

            /* Scale velocity if needed */
            if (gen->radius >= 0.0F && gen->aux.rect.x < 0.0F) {
                f32 scale = sin_az / gen->radius;
                vel_out.x *= scale;
                vel_out.y *= scale;
                vel_out.z *= scale;
            }

            emit_pos.x = sin_az * emit_pos.x + gen->pos.x;
            emit_pos.y = sin_az * emit_pos.y + gen->pos.y;
            emit_pos.z = sin_az * emit_pos.z + gen->pos.z;

            hsd_80398F0C(gen->linkNo, gen->bank, gen->kind, gen->texGroup,
                         (s32) gen->cmdList, gen->life, 0, (s32) gen,
                         emit_pos.x, emit_pos.y, emit_pos.z, vel_out.x,
                         vel_out.y, vel_out.z, gen->size, gen->grav,
                         gen->fric);
            break;
        }

        default: /* shape > 8 */
        {
            if (hsd_804D78E8 != 0) {
                ((void (*)(HSD_Generator*, Mtx)) hsd_804D78E8)(gen, rot_mtx);
            }
            break;
        }
        }

        gen->count -= 1.0F;
    }

    return gen->count;
}

void hsd_8039EE24(u32 mask)
{
    HSD_Generator* gp;
    HSD_Generator* gen;

    while (hsd_804D78F4 != 0) {
        gp = (HSD_Generator*) ((HSD_SList*) hsd_804D78F4)->data;
        hsd_8039D71C(gp);
        hsd_804D78F4 = (u32) HSD_SListRemove((HSD_SList*) hsd_804D78F4);
        if (gp->jobj != NULL) {
            HSD_JObjUnref(gp->jobj);
            gp->jobj = NULL;
        }
    }

    gen = hsd_804D78FC;
    hsd_804D78F8 = 0;

    while (gen != NULL) {
        if (mask & (1 << (gen->linkNo + 16))) {
            hsd_804D78F8 = (u32) gen;
            gen = gen->next;
            continue;
        }
        if (gen->kind & 0x800) {
            hsd_804D78F8 = (u32) gen;
            gen = gen->next;
            continue;
        }
        hsd_8039D214(gen);
        if (gen->random < 0.0F) {
            gen->count -= gen->random;
        } else {
            gen->count += gen->random * HSD_Randf();
        }
        if (gen->count >= 1.0F) {
            gen->count = hsd_8039DAD4(gen);
        }
        if (gen->genLife != 0) {
            u16 life = gen->genLife - 1;
            gen->genLife = life;
            if (life == 0) {
                hsd_804D78F8 =
                    (u32) hsd_8039D3AC(gen, (HSD_Generator*) hsd_804D78F8);
                if (hsd_804D78F8 != 0) {
                    gen = ((HSD_Generator*) hsd_804D78F8)->next;
                } else {
                    gen = hsd_804D78FC;
                }
                continue;
            }
        }
        hsd_804D78F8 = (u32) gen;
        gen = gen->next;
    }
}

static char lbl_8040C2A4[] = "object.h";
static char lbl_8040C2B0[] = "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF";

/* Duplicate of object.h ref_INC: this original TU emitted its own copies of
 * the assert strings (lbl_8040C2A4/lbl_8040C2B0). */
static inline void ref_INC_dup(void* o)
{
    if (o != NULL) {
        HSD_OBJ(o)->ref_count++;
        (HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF)
            ? ((void) 0)
            : __assert(lbl_8040C2A4, 93, lbl_8040C2B0);
    }
}

HSD_Generator* hsd_8039EFAC(s32 linkNo, s32 bank, s32 gfx_id, HSD_JObj* jobj)
{
    HSD_Generator* gen;

    gen = hsd_8039F05C(linkNo, bank, gfx_id);
    if (gen == NULL) {
        return NULL;
    }
    if (gen != NULL) {
        gen->jobj = jobj;
        if (jobj != NULL) {
            ref_INC_dup(jobj);
        }
    }
    gen->type |= (gen->kind & 0x20000) ? 0x500 : 0x700;
    return gen;
}

// @TODO: Currently 80.79% match - .bss.0 section anchor hoist causes
// register-allocation cascade (extra saved reg + frame shift)
HSD_Generator* hsd_8039F05C(s32 linkNo, s32 bank, s32 idx)
{
    HSD_PSTexGroup* tg;
    HSD_Generator* gen;
    s32 ofs;
    u32 shape;
    volatile f32 y1;
    volatile f32 y2;
    f32 abs_vx;
    f32 mag;
    f32 f0, f1, f3;
    u8 _pad2[4];

    if (bank >= 0x41) {
        return NULL;
    }
    if (linkNo >= 8) {
        return NULL;
    }
    if (idx >= psCmdListArray[bank]) {
        return NULL;
    }

    ofs = idx * 4;
    if ((u32) * ((s32*) ptclref_804D0E5C[bank] + idx) == 0) {
        return NULL;
    }

    gen = hsd_8039D9C8();
    if (gen != NULL) {
        gen->type = ptclref_804D0E5C[bank][idx]->type;
        gen->bank = bank;
        gen->linkNo = linkNo;
        gen->kind = ptclref_804D0E5C[bank][idx]->kind;
        gen->texGroup = ptclref_804D0E5C[bank][idx]->texGroup;
        gen->life = ptclref_804D0E5C[bank][idx]->life;
        gen->genLife = ptclref_804D0E5C[bank][idx]->genLife;
        gen->pos.z = 0.0F;
        gen->pos.y = 0.0F;
        gen->pos.x = 0.0F;
        gen->vel.x = ptclref_804D0E5C[bank][idx]->vx;
        gen->vel.y = ptclref_804D0E5C[bank][idx]->vy;
        gen->vel.z = ptclref_804D0E5C[bank][idx]->vz;
        gen->grav = ptclref_804D0E5C[bank][idx]->grav;
        gen->fric = ptclref_804D0E5C[bank][idx]->fric;
        gen->size = ptclref_804D0E5C[bank][idx]->size;
        gen->cmdList = ptclref_804D0E5C[bank][idx]->cmdList;
        gen->radius = ptclref_804D0E5C[bank][idx]->radius;
        gen->angle = ptclref_804D0E5C[bank][idx]->angle;
        gen->random = ptclref_804D0E5C[bank][idx]->random;

        if (gen->kind & 0x100) {
            f1 = gen->random;
            if (f1 < 0.0F) {
                f3 = 1.0F;
                f3 = f3 + f1 > 1.1920929e-7F ? f3 : 0.0F;
                gen->count = f3;
            } else {
                gen->count = 0.9999999F;
            }
        } else if (gen->random < 0.0F) {
            gen->count = 0.0F;
        } else {
            gen->count = HSD_Randf();
        }

        tg = psTexGroupArray[bank][gen->texGroup];
        if (tg != NULL && tg->palflag != 0) {
            gen->kind |= 0x10;
        }

        gen->jobj = NULL;
        gen->numChild = 0;

        shape = gen->type & 0xF;
        switch (shape) {
        case 0:
        case 3:
        case 4: {
            HSD_PSCmdList* c = ptclref_804D0E5C[bank][idx];
            f32 p1 = c->param1;
            if (p1 == 0.0F && c->param2 == 0.0F) {
                gen->aux.disc.minAngle = 0.0F;
                gen->aux.disc.maxAngle = 6.2831855F;
            } else {
                gen->aux.disc.minAngle = p1;
                gen->aux.disc.maxAngle =
                    ptclref_804D0E5C[bank][idx]->param2;
            }
            break;
        }
        case 1:
            gen->aux.line.x2 = ptclref_804D0E5C[bank][idx]->param1;
            gen->aux.line.y2 = ptclref_804D0E5C[bank][idx]->param2;
            gen->aux.line.z2 = ptclref_804D0E5C[bank][idx]->param3;
            break;
        case 2:
            break;
        case 6:
        case 7: {
            HSD_PSCmdList* c = ptclref_804D0E5C[bank][idx];
            f32 p1 = c->param1;
            if (p1 == 0.0F && c->param2 == 0.0F) {
                gen->aux.cone.minAngle = 0.0F;
                gen->aux.cone.maxAngle = 6.2831855F;
            } else {
                gen->aux.cone.minAngle = p1;
                gen->aux.cone.maxAngle =
                    ptclref_804D0E5C[bank][idx]->param2;
            }
            gen->aux.cone.height = ptclref_804D0E5C[bank][idx]->param3;
            break;
        }
        case 5: {
            f0 = ptclref_804D0E5C[bank][idx]->param1;
            gen->aux.rect.x = f0;
            gen->aux.rect.xx = f0;
            f0 = ptclref_804D0E5C[bank][idx]->param2;
            gen->aux.rect.y = f0;
            gen->aux.rect.yy = f0;
            f0 = ptclref_804D0E5C[bank][idx]->param3;
            gen->aux.rect.z = f0;
            gen->aux.rect.zz = f0;
            gen->aux.rect.zy = 0.0F;
            gen->aux.rect.zx = 0.0F;
            gen->aux.rect.yz = 0.0F;
            gen->aux.rect.yx = 0.0F;
            gen->aux.rect.xz = 0.0F;
            gen->aux.rect.xy = 0.0F;
            gen->aux.rect.flag = 0;
            if (ptclref_804D0E5C[bank][idx]->param1 < 0.0F) {
                gen->aux.rect.flag |= 1;
            }
            if (ptclref_804D0E5C[bank][idx]->param2 < 0.0F) {
                gen->aux.rect.flag |= 2;
            }
            if (ptclref_804D0E5C[bank][idx]->param3 < 0.0F) {
                gen->aux.rect.flag |= 4;
            }
            break;
        }
        case 8: {
            if ((mag = gen->vel.z * gen->vel.z +
                       (gen->vel.x * gen->vel.x + gen->vel.y * gen->vel.y)) >
                0.0F)
            {
                f64 guess = __frsqrte(mag);
                guess = 0.5 * guess * (3.0 - mag * (guess * guess));
                guess = 0.5 * guess * (3.0 - mag * (guess * guess));
                guess = 0.5 * guess * (3.0 - mag * (guess * guess));
                y1 = (f32) (mag * guess);
                mag = y1;
            }
            gen->aux.sphere.speed = mag;

            if ((mag = gen->vel.x * gen->vel.x + gen->vel.z * gen->vel.z) >
                0.0F)
            {
                f64 guess = __frsqrte(mag);
                guess = 0.5 * guess * (3.0 - mag * (guess * guess));
                guess = 0.5 * guess * (3.0 - mag * (guess * guess));
                guess = 0.5 * guess * (3.0 - mag * (guess * guess));
                y2 = (f32) (mag * guess);
                mag = y2;
            }

            if (mag < 1.1754944e-38F) {
                if (gen->vel.y >= 0.0F) {
                    gen->aux.sphere.latMid = 1.5707964F;
                } else {
                    gen->aux.sphere.latMid = -1.5707964F;
                }
            } else {
                gen->aux.sphere.latMid = atan2f(gen->vel.y, mag);
            }

            abs_vx = gen->vel.x;
            *(s32*) &abs_vx &= 0x7FFFFFFF;
            if (abs_vx < 1.1754944e-38F) {
                if (gen->vel.z >= 0.0F) {
                    gen->aux.sphere.lonMid = 1.5707964F;
                } else {
                    gen->aux.sphere.lonMid = -1.5707964F;
                }
            } else {
                gen->aux.sphere.lonMid = atan2f(gen->vel.z, gen->vel.x);
            }
            gen->aux.sphere.latRange =
                ptclref_804D0E5C[bank][idx]->param1;
            f1 = gen->aux.sphere.latRange;
            if (f1 < 0.0F) {
                gen->aux.sphere.latRange = -f1;
                gen->aux.sphere.speed = -gen->aux.sphere.speed;
            }
            gen->aux.sphere.lonRange =
                ptclref_804D0E5C[bank][idx]->param2;
            break;
        }
        default:
            if ((void (*)(HSD_Generator*)) hsd_804D78EC != NULL) {
                ((void (*)(HSD_Generator*)) hsd_804D78EC)(gen);
            }
            break;
        }

        if (gen->kind & 0x20000) {
            gen->type |= 0x800;
            psAddGeneratorAppSRT_begin(gen, 0);
            if (gen->appsrt != NULL) {
                gen->appsrt->xA2 = 1;
                gen->appsrt->gp = gen;
            }
        }

        if ((void (*)(HSD_Generator*)) hsd_804D7900 != NULL) {
            ((void (*)(HSD_Generator*)) hsd_804D7900)(gen);
        }
    }
    return gen;
}

HSD_Generator* hsd_8039F6CC(s32 linkNo, s32 bank, s32 gfx_id, HSD_JObj* jobj)
{
    HSD_Generator* gen;

    gen = hsd_8039F05C(linkNo, bank, gfx_id);
    if (gen == NULL) {
        return NULL;
    }
    if (gen != NULL) {
        gen->jobj = jobj;
        if (jobj != NULL) {
            ref_INC_dup(jobj);
        }
    }
    gen->type |= (gen->kind & 0x20000) ? 0x500 : 0x700;
    hsd_804D78F4 =
        (u32) HSD_SListAllocAndAppend((HSD_SList*) hsd_804D78F4, gen);
    return gen;
}
