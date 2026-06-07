#include "ftdevice.h"

#include <placeholder.h>

#include "ft/inlines.h"

#include <dolphin/os/OSError.h>
#include <baselib/debug.h>

#pragma force_active on

/* literal */ extern char ftDevice_803C6B18[];
/* literal */ extern char ftDevice_803C6B40[];
/* literal */ extern char ftDevice_803C6B4C[];
/* literal */ extern char ftDevice_803C6B78[];
/* literal */ extern char ftCo_804D3C18[];

struct ftDeviceUnk3 ft_80459A68[1];

/* .sbss commons also emit in REVERSE declaration order ->
 * ft_804D6570 @+0x0, ftDevice_BuryThingCount @+0x4, ft_804D6578 @+0x8.
 * ft_804D6570 must be non-static (ftcoll.c imports it). */
struct ftDeviceUnk4 ft_804D6578;
int ftDevice_BuryThingCount;
int ft_804D6570;

/* TU .bss reconstruction (target layout 0xc58). Code-referenced commons
 * emit in first-reference order (ft_80459A68 only); UNREFERENCED commons
 * emit at the tail in REVERSE declaration order -> declared in reverse
 * target order so they land ftDevice_BuryThings @+0xc .. ft_8045A6A8
 * @+0xc40 (per-character Kirby-hat/costume containers; types mirror
 * ft_0877.h / ftdata.c externs). */
HSD_Joint* ft_8045A6A8[6];
HSD_Joint* ft_8045A690[6];
HSD_Joint* ft_8045A678[6];
HSD_Joint* ft_8045A660[6];
HSD_Joint* ft_8045A648[6];
HSD_Joint* ft_8045A630[6];
UnkCostumeStruct ft_8045A5B8[5];
UnkCostumeStruct ft_8045A540[5];
UnkCostumeStruct ft_8045A4E0[4];
UnkCostumeStruct ft_8045A480[4];
UnkCostumeStruct ft_8045A420[4];
UnkCostumeStruct ft_8045A3A8[5];
UnkCostumeStruct ft_8045A330[5];
UnkCostumeStruct ft_8045A2D0[4];
UnkCostumeStruct ft_8045A270[4];
UnkCostumeStruct ft_8045A1F8[5];
HSD_Joint* ft_8045A1E0[6];
UnkCostumeStruct ft_8045A168[5];
UnkCostumeStruct ft_8045A0F0[5];
UnkCostumeStruct ft_8045A090[4];
UnkCostumeStruct ft_8045A000[6];
UnkCostumeStruct ft_80459F88[5];
UnkCostumeStruct ft_80459F28[4];
UnkCostumeStruct ft_80459EC8[4];
UnkCostumeStruct ft_80459E68[4];
UnkCostumeStruct ft_80459DF0[5];
UnkCostumeStruct ft_80459D90[4];
UnkCostumeStruct ft_80459D18[5];
UnkCostumeStruct ft_80459CA0[5];
UnkCostumeStruct ft_80459C10[6];
struct ft_80459B88_t ft_80459B88;
UnkCostumeStruct ft_80459B28[4];
UnkCostumeStruct ft_80459A98[6];
struct ftDeviceUnk3 ft_80459A8C;
struct ftDeviceUnk5 ftDevice_BuryThings[2];

ColorOverlay* ftCo_800C0658(Fighter* fp)
{
    return (int) fp->x408.x28_colanim.ptr != 0 ? &fp->x408 : &fp->x488;
}

ColorOverlay* ftCo_800C0674(Fighter_GObj* gobj)
{
    return ftCo_800C0658(GET_FIGHTER(gobj));
}

/// @todo Wrong return type. Union?
enum_t ftCo_800C0694(Fighter* fp)
{
    return ftCo_800C0658(fp)->x28_colanim.i;
}

int ftCo_800C06B4(Fighter* fp)
{
    return (int) &fp->x508.x28_colanim.ptr->x7B;
}

void ftCo_800C06C0(void)
{
    ft_804D6570 = 0;
    ftDevice_BuryThingCount = 0;
    ft_804D6578.x0 = 0;
    {
        int i;
        for (i = 0; i < 4; i++) {
            ft_80459A68[i].ground = 0;
        }
    }
}

void ftCo_800C06E8(Ground_GObj* gobj, int arg1, void* func)
{
    struct ftDeviceUnk3* base = ft_80459A68;
    int i;

    for (i = 0; i < 1; i++) {
        if (base[i].ground == NULL) {
            base[i].ground = gobj;
            base[i].type = arg1;
            base[i].active_cb = func;
            ft_804D6578.x0++;
            return;
        }
    }
    OSReport("fighter chk device wind func num over!\n");
    __assert("ftdevice.c", 0x49, "0");
}

void ftCo_800C0764(Ground_GObj* arg0, u32 arg1, void* arg2)
{
    struct ftDeviceUnk3* base = ft_80459A68;
    struct ftDeviceUnk3* ptr;
    int i;

    ptr = base + 1;
    for (i = 0; i < 2; i++) {
        if (ptr->ground == NULL) {
            base[i + 1].ground = arg0;
            base[i + 1].type = arg1;
            base[i + 1].active_cb = arg2;
            ftDevice_BuryThingCount++;
            return;
        }
        ptr++;
    }
    OSReport("fighter chk device catch func num over!\n");
    __assert("ftdevice.c", 0x6FU, "0");
}

/// @todo pretty sure arg2 is a ftDevice callback, but unsure if its
/// always of type ftDevice_Callback0.
void ftCo_800C07F8(Ground_GObj* arg0, u32 arg1, void* arg2)
{
    struct ftDeviceUnk3* base = ft_80459A68;
    struct ftDeviceUnk3* ptr;
    int i;

    ptr = base + 3;
    for (i = 0; i < 1; i++) {
        if (ptr->ground == NULL) {
            base[i + 3].ground = arg0;
            base[i + 3].type = arg1;
            base[i + 3].active_cb = arg2;
            ft_804D6570++;
            return;
        }
        ptr++;
    }
    OSReport("fighter chk device coll func num over!\n");
    __assert("ftdevice.c", 0x95, "0");
}
