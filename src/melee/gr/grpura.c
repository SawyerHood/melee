#include "gr/grpura.h"

#include <platform.h>

#include "baselib/debug.h"
#include "baselib/gobjgxlink.h"
#include "baselib/gobjproc.h"
#include "baselib/random.h"
#include "cm/camera.h"
#include "cm/types.h"
#include "gr/grdisplay.h"
#include "gr/ground.h"
#include "gr/grzakogenerator.h"
#include "gr/inlines.h"
#include "gr/stage.h"
#include "gr/types.h"
#include "lb/lb_00B0.h"
#include "lb/lb_00F9.h"
#include "mp/mplib.h"

#include <dolphin/mtx.h>
#include <baselib/gobj.h>
#include <baselib/jobj.h>
#include <baselib/tobj.h>
#include <sysdolphin/baselib/dobj.h>

/* 213030 */ static void grPura_80213030(Ground_GObj* arg0);

StageCallbacks grPu_803E6800[] = {
    { grPura_80211EF0, grPura_80211F1C, grPura_80211F24, grPura_80211F28, 0 },
    { grPura_80212024, grPura_802120D8, grPura_802120E0, grPura_8021228C, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80211F68, grPura_80211FD0, grPura_80211FD8, grPura_80212020,
      0x40000000 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80212290, grPura_80212314, grPura_8021231C, grPura_802125EC, 0 },
    { grPura_80211F2C, grPura_80211F58, grPura_80211F60, grPura_80211F64,
      0x80000000 }
};

char grPu_803E6A30[] = "/GrPu.dat";

typedef struct grPu_StageData {
    StageData stage_data;
    char report_format[0x24];
    char filename[0xC];
} grPu_StageData;

grPu_StageData grPu_803E6A3C = {
    {
        0x11,
        grPu_803E6800,
        grPu_803E6A30,
        grPura_80211D00,
        grPura_80211CFC,
        grPura_80211DD8,
        grPura_80211DDC,
        grPura_80211E00,
        grPura_802130C0,
        grPura_802130C8,
        1,
        0,
        0,
    },
    "%s:%d: couldn t get gobj(id=%d)\n",
    { 'g', 'r', 'p', 'u', 'r', 'a', '.', 'c' },
};

GXColor grPu_803E6AA0[91] = {
    { 0x00, 0x00, 0x00, 0xFF }, { 0x00, 0x00, 0x50, 0xFF }, { 0x1E, 0x46, 0x00, 0xFF }, { 0x87, 0x87, 0x00, 0xFF },
    { 0x00, 0x00, 0x00, 0x00 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x02 }, { 0x00, 0x00, 0x00, 0x01 },
    { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x03 }, { 0x00, 0x00, 0x00, 0x02 }, { 0x3F, 0x80, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x05 }, { 0x00, 0x00, 0x00, 0x03 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x06 },
    { 0x00, 0x00, 0x00, 0x04 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x07 }, { 0x00, 0x00, 0x00, 0x05 },
    { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x08 }, { 0x00, 0x00, 0x00, 0x06 }, { 0x3F, 0x80, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x09 }, { 0x00, 0x00, 0x00, 0x07 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x0A },
    { 0x00, 0x00, 0x00, 0x08 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x0B }, { 0x00, 0x00, 0x00, 0x09 },
    { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x0C }, { 0x00, 0x00, 0x00, 0x0A }, { 0x3F, 0x80, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x0D }, { 0x00, 0x00, 0x00, 0x0B }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x0D },
    { 0x00, 0x00, 0x00, 0x0C }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x0D }, { 0x00, 0x00, 0x00, 0x0D },
    { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x0E }, { 0x00, 0x00, 0x00, 0x0E }, { 0x3F, 0x80, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x0F }, { 0x00, 0x00, 0x00, 0x0F }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x10 },
    { 0x00, 0x00, 0x00, 0x10 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x11 }, { 0x00, 0x00, 0x00, 0x11 },
    { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x12 }, { 0x00, 0x00, 0x00, 0x12 }, { 0x3F, 0x80, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x13 }, { 0x00, 0x00, 0x00, 0x13 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x14 },
    { 0x00, 0x00, 0x00, 0x14 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x15 }, { 0x00, 0x00, 0x00, 0x15 },
    { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x16 }, { 0x00, 0x00, 0x00, 0x16 }, { 0x3F, 0x80, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x17 }, { 0x00, 0x00, 0x00, 0x17 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x18 },
    { 0x00, 0x00, 0x00, 0x18 }, { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x19 }, { 0x00, 0x00, 0x00, 0x19 },
    { 0x3F, 0x80, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x1A }, { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x3F, 0x80, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x47, 0x72, 0x64, 0x49 }, { 0x7A, 0x75, 0x6D, 0x69 }, { 0x53, 0x74, 0x61, 0x72 },
    { 0x5F, 0x54, 0x6F, 0x70 }, { 0x4E, 0x5F, 0x6A, 0x6F }, { 0x69, 0x6E, 0x74, 0x00 }
};

/* datarecon-w27: 0x40c..0x61c; 42 live entries + 2 zero; trailing 4B pad from
   grPu_803E6E20 u64 8-alignment */
GrPuVtxMapEntry grPu_803E6C0C[44] = {
    { 20, 2, 21, 0, NULL },
    { 21, 2, 22, 0, NULL },
    { 22, 2, 23, 0, NULL },
    { 23, 2, 24, 0, NULL },
    { 24, 2, 25, 0, NULL },
    { 25, 2, 26, 0, NULL },
    { 26, 2, 27, 0, NULL },
    { 39, 2, 28, 0, NULL },
    { 284, 12, 21, 0, NULL },
    { 285, 12, 23, 0, NULL },
    { 267, 12, 24, 0, NULL },
    { 261, 12, 25, 0, NULL },
    { 262, 12, 26, 0, NULL },
    { 275, 12, 27, 0, NULL },
    { 276, 12, 28, 0, NULL },
    { 277, 12, 29, 0, NULL },
    { 269, 12, 30, 0, NULL },
    { 270, 12, 31, 0, NULL },
    { 272, 12, 32, 0, NULL },
    { 286, 12, 33, 0, NULL },
    { 180, 8, 23, 0, NULL },
    { 181, 8, 51, 0, NULL },
    { 182, 8, 27, 0, NULL },
    { 183, 8, 30, 0, NULL },
    { 184, 8, 31, 0, NULL },
    { 513, 25, 10, 0, NULL },
    { 514, 25, 13, 0, NULL },
    { 520, 25, 16, 0, NULL },
    { 521, 25, 19, 0, NULL },
    { 522, 25, 22, 0, NULL },
    { 496, 25, 25, 0, NULL },
    { 497, 25, 28, 0, NULL },
    { 500, 25, 31, 0, NULL },
    { 523, 25, 32, 0, NULL },
    { 524, 25, 29, 0, NULL },
    { 527, 25, 26, 0, NULL },
    { 528, 25, 23, 0, NULL },
    { 525, 25, 20, 0, NULL },
    { 526, 25, 17, 0, NULL },
    { 494, 25, 14, 0, NULL },
    { 495, 25, 11, 0, NULL },
    { -1, 0, 0, 0, NULL },
    { 0, 0, 0, 0, NULL },
    { 0, 0, 0, 0, NULL },
};

/* datarecon-w27: 0x620..0xe20, 32x32 16bpp texture;
   u64 element type pins 8-alignment (w16 sislib law) */
u64 grPu_803E6E20[0x100] = {
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534,
    0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534,
    0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534,
    0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534,
    0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0xA534A534A534A534, 0xA534A534A534A534, 0x8410841084108410, 0x8410841084108410,
    0xA534A534A534A534, 0xA534A534A534A534, 0x8410841084108410, 0x8410841084108410,
    0xA534A534A534A534, 0xA534A534A534A534, 0x8410841084108410, 0x8410841084108410,
    0xA534A534A534A534, 0xA534A534A534A534, 0x84108410A534A534, 0x84108410A534A534,
    0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0x8410841084108410, 0x8410841084108410, 0x8410841084108410, 0x8410841084108410,
    0x8410841084108410, 0x8410841084108410, 0x8410841084108410, 0x8410841084108410,
    0x8410841084108410, 0x8410841084108410, 0x8410841084108410, 0x8410841084108410,
    0x84108410A534A534, 0x84108410A534A534, 0x84108410A534A534, 0x84108410A534A534,
    0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0x738E738E738E738E, 0x738E738E738E738E, 0x738E738E738E738E, 0x738E738E738E738E,
    0x738E738E738E738E, 0x738E738E738E738E, 0x738E738E738E738E, 0x738E738E738E738E,
    0x8410841084108410, 0x8410841084108410, 0x8410841084108410, 0x8410841084108410,
    0x84108410A534A534, 0x84108410A534A534, 0x84108410A534A534, 0x84108410A534A534,
    0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
    0x738E738E738E738E, 0x738E738E738E738E, 0x738E738E738E738E, 0x738E738E738E738E,
    0x738E738E738E738E, 0x738E738E738E738E, 0x738E738E738E738E, 0x738E738E738E738E,
    0x8410841084108410, 0x8410841084108410, 0x8410841084108410, 0x8410841084108410,
    0x84108410A534A534, 0x84108410A534A534, 0x84108410A534A534, 0x84108410A534A534,
    0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534, 0xA534A534A534A534,
    0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79, 0xCE79CE79CE79CE79,
    0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF, 0xCE79CE79FFFFFFFF,
    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF
};

struct HSD_ImageDesc grPu_803E7620 = { grPu_803E6E20, 32, 32, 4, 0, 0, 0 };

void grPura_80211CFC(bool num) {}

/* datarecon-w27: grPu_804DBA70/74/78/7C (0.0F/2.0F/30.0F/-30.0F) deliberately NOT
   defined: the four anonymous pool literals created by grPura_802125F0 and
   grPura_80212CD4 occupy the exact target .sdata2 slots 0x18..0x28 byte-identically;
   a visible named def only ever ADDs 16B (idiom-147 dup law) and by-name respelling
   regresses both fns (probed w27, inline-arg const-vs-literal codegen). Cost: 4 soft
   EXPORT-MISSING[0imp] rows. */

/* 4D6AA0 */ HSD_GObj* grPu_804D6AA0; /* datarecon-w27: symbols.txt scope:global, idiom 139 */

void grPura_80211D00(void)
{
    UNUSED u32 unused1;
    UNUSED u32 unused2;
    UNUSED u32 unused3;
    Vec3 cam_offset;

    grPu_804D6AA0 = Ground_801C49F8();
    stage_info.unk8C.b4 = 0;
    stage_info.unk8C.b5 = 1;
    grPura_80211E08(0);
    grPura_80211E08(1);
    grPura_80211E08(4);
    Ground_801C39C0();
    Ground_801C3BB4();
    Stage_UnkSetVec3TCam_Offset(&cam_offset);
    Ground_801C3880(0.8f * (Stage_GetCamBoundsTopOffset() - cam_offset.y));
    Ground_801C3890(0.8f * (Stage_GetCamBoundsBottomOffset() - cam_offset.y));
    Ground_801C38A0(0.8f * (Stage_GetCamBoundsLeftOffset() - cam_offset.x));
    Ground_801C38AC(0.8f * (Stage_GetCamBoundsRightOffset() - cam_offset.x));
}

void grPura_80211DD8(void) {}

void grPura_80211DDC(void)
{
    grZakoGenerator_801CAE04(NULL);
}

bool grPura_80211E00(void)
{
    return false;
}

HSD_GObj* grPura_80211E08_noinline(int gobj_id);
HSD_GObj* grPura_80211E08_noinline(int gobj_id)
{
    return grPura_80211E08(gobj_id);
};

HSD_GObj* grPura_80211E08_noinline2(int gobj_id);
HSD_GObj* grPura_80211E08_noinline2(int gobj_id)
{
    return grPura_80211E08_noinline(gobj_id);
};

HSD_GObj* grPura_80211E08(int gobj_id)
{
    HSD_GObj* gobj;
    StageCallbacks* callbacks = &grPu_803E6800[gobj_id];

    gobj = Ground_GetStageGObj(gobj_id);

    if (gobj != NULL) {
        Ground_SetupStageCallbacks(gobj, callbacks);
    } else {
        OSReport((char*) grPu_803E6800 + 0x270,
                 (char*) grPu_803E6800 + 0x294, 0x108, gobj_id);
    }

    return gobj;
}

void grPura_80211EF0(Ground_GObj* arg0)
{
    Ground* gp = arg0->user_data;
    grAnime_801C8138(arg0, gp->map_id, 0);
}

bool grPura_80211F1C(Ground_GObj* arg0)
{
    return false;
}

void grPura_80211F24(Ground_GObj* arg0) {}

void grPura_80211F28(Ground_GObj* arg0) {}

void grPura_80211F2C(Ground_GObj* arg0)
{
    Ground* gp = arg0->user_data;
    grAnime_801C8138(arg0, gp->map_id, 0);
}

bool grPura_80211F58(Ground_GObj* arg0)
{
    return false;
}

void grPura_80211F60(Ground_GObj* arg0) {}

void grPura_80211F64(Ground_GObj* arg0) {}

void grPura_80211F68(Ground_GObj* arg0)
{
    Ground_JObjInline1(arg0);
    grPura_80212CD4(arg0);
    grPura_802125F0(arg0);
    grPura_80212FC0(arg0);
}

bool grPura_80211FD0(Ground_GObj* arg0)
{
    return false;
}

void grPura_80211FD8(Ground_GObj* arg0)
{
    grPura_80212EF4(arg0);
    Ground_801C2FE0(arg0);
    grPura_80213030(arg0);
    mpLib_80055E24(0x18);
    lb_800115F4();
}

void grPura_80212020(Ground_GObj* arg0) {}

void grPura_80212024(Ground_GObj* arg0)
{
    unsigned int uVar1;
    Ground* gp = GET_GROUND(arg0);
    PAD_STACK(16);
    grAnime_801C8138(arg0, gp->map_id, 0);
    gp->x11_flags.b012 = 2;
    gp->gv.pura.xC4 = HSD_Randi(4);
    do {
        uVar1 = HSD_Randi(4);
    } while (gp->gv.pura.xC4 == (gp->gv.pura.xC6 = uVar1));
    Ground_801C205C(&grPu_803E6AA0[gp->gv.pura.xC4]);
    Camera_SetBackgroundColor(grPu_803E6AA0[gp->gv.pura.xC4].r,
                              grPu_803E6AA0[gp->gv.pura.xC4].g,
                              grPu_803E6AA0[gp->gv.pura.xC4].b);
    gp->gv.pura.xC8 = 0;
}

bool grPura_802120D8(Ground_GObj* arg0)
{
    return false;
}

void grPura_802120E0(Ground_GObj* arg0)
{
    GXColor spilC;
    GXColor sp18;
    Ground* gp = arg0->user_data;
    f32 t;
    s16 cur;
    unsigned int uVar1;
    UNUSED u8 _pad4[4];
    PAD_STACK(8);

    if ((s16) gp->gv.pura.xC8 < 0xE10) {
        spilC = grPu_803E6AA0[gp->gv.pura.xC6];
        sp18 = grPu_803E6AA0[gp->gv.pura.xC4];
        cur = gp->gv.pura.xC8;
        gp->gv.pura.xC8 = cur + 1;
        t = (f32) cur / 3600.0f;
        sp18.r = (s8) (t * (f32) ((u8) spilC.r - (u8) sp18.r) + (f32) (u8) sp18.r);
        sp18.g = (s8) (t * (f32) ((u8) spilC.g - (u8) sp18.g) + (f32) (u8) sp18.g);
        sp18.b = (s8) (t * (f32) ((u8) spilC.b - (u8) sp18.b) + (f32) (u8) sp18.b);
        Ground_801C205C(&sp18);
        Camera_SetBackgroundColor(sp18.r, sp18.g, sp18.b);
        return;
    }
    gp->gv.pura.xC4 = gp->gv.pura.xC6;
    do {
        uVar1 = HSD_Randi(4);
    } while ((s16) gp->gv.pura.xC4 == (gp->gv.pura.xC6 = uVar1));
    gp->gv.pura.xC8 = 0;
}

void grPura_8021228C(Ground_GObj* arg0) {}

void grPura_80212290(Ground_GObj* arg0)
{
    Ground* gp = GET_GROUND(arg0);
    HSD_JObj* jobj = arg0->hsd_obj;
    PAD_STACK(8);
    arg0->render_cb = (GObj_RenderFunc) fn_802130D0;
    HSD_MObjSetToonTextureImage(&grPu_803E7620);
    lb_80011C18(jobj, 0x1000);
    grPura_80213250(jobj);
    HSD_MObjSetToonTextureImage(NULL);
    grAnime_801C8138(arg0, gp->map_id, 0);
}

bool grPura_80212314(Ground_GObj* arg0)
{
    return false;
}

void grPura_8021231C(Ground_GObj* arg0)
{
    u32 pad2;
    Vec3 vec;
    Vec3 vec2;
    Ground* gp = GET_GROUND(arg0);
    HSD_JObj* jobj = GET_JOBJ(arg0);
    Quaternion quat;

    HSD_JObjGetTranslation2(gp->gv.pura2.xC8, &vec);
    HSD_JObjSetTranslate(jobj, &vec);
    HSD_JObjGetRotation(gp->gv.pura2.xC8, &quat);
    HSD_JObjSetRotation(jobj, &quat);
    HSD_JObjGetScale(gp->gv.pura2.xC8, &vec2);
    HSD_JObjSetScale(jobj, &vec2);

    if ((HSD_JObjGetFlags(gp->gv.pura2.xC8) & 0x10) &&
        ((HSD_JObjGetFlags(jobj) & 0x10) == NULL))
    {
        HSD_JObjSetFlagsAll(jobj, 0x10);
    } else if (((HSD_JObjGetFlags(gp->gv.pura2.xC8) & 0x10) == NULL) &&
               (HSD_JObjGetFlags(jobj) & 0x10))
    {
        HSD_JObjClearFlagsAll(jobj, 0x10);
    }
}

void grPura_802125EC(Ground_GObj* arg0) {}

void grPura_802125F0(HSD_GObj* arg0)
{
    struct Pura_UnkModelDesc {
        s16 x0;
        s16 x2;
        f32 x4;
        s32 x8;
    }* desc = (void*) ((char*) grPu_803E6800 + 0x2B0);
    f32 scale;
    s32 i;
    s32 joint;
    HSD_GObj* gobj;
    Ground* gp;
    HSD_JObj* jobj;
    HSD_JObj* child;

    for (i = 0; i < 27; i++, desc++) {
        if (desc->x8 != -1) {
            gobj = grPura_80211E08(desc->x8);
            HSD_ASSERT(0x291, gobj);
            gp = GET_GROUND(gobj);
            HSD_ASSERT(0x292, gp);

            gp->gv.pura2.xC4 = desc->x0;
            joint = Ground_801C33C0(4, gp->gv.pura2.xC4);
            gp->gv.pura2.xC8 = Ground_801C3FA4(arg0, gp->gv.pura2.xC4);

            jobj = gobj->hsd_obj;
            HSD_JObjSetTranslateX(
                jobj, HSD_JObjGetTranslationX(gp->gv.pura2.xC8));
            HSD_JObjSetTranslateY(
                jobj, HSD_JObjGetTranslationY(gp->gv.pura2.xC8));
            HSD_JObjSetTranslateZ(
                jobj, HSD_JObjGetTranslationZ(gp->gv.pura2.xC8));

            if (HSD_JObjGetFlags(gp->gv.pura2.xC8) & 0x10) {
                HSD_JObjSetFlagsAll(jobj, 0x10);
            }

            jobj = gobj->hsd_obj;
            if (jobj == NULL) {
                child = NULL;
            } else {
                child = jobj->child;
            }
            HSD_JObjSetTranslateX(child, 0.0f);
            HSD_JObjSetTranslateY(child, 0.0f);
            HSD_JObjSetTranslateZ(child, 0.0f);

            jobj = gp->gv.pura2.xC8;
            scale = HSD_JObjGetScaleX(jobj);
            if (scale < 2.0f) {
                scale *= desc->x4;
            }
            HSD_JObjSetScaleX(jobj, scale);
            HSD_JObjSetScaleY(jobj, scale);
            HSD_JObjSetScaleZ(jobj, scale);

            jobj = gobj->hsd_obj;
            HSD_JObjSetScaleX(jobj, scale);
            HSD_JObjSetScaleY(jobj, scale);
            HSD_JObjSetScaleZ(jobj, scale);
        }
    }
}

void grPura_80212CD4(HSD_GObj* arg0)
{
    Vec3 sp10;
    s32 i;
    Ground* gp = arg0->user_data;
    HSD_JObj* jobj = arg0->hsd_obj;
    HSD_JObj* node;
    CmSubject* s;

    for (i = 0; i < 25; i++) {
        gp->gv.pura3.xC4[i] = NULL;
        gp->gv.pura3.x128[i] = NULL;
    }

    if (jobj == NULL) {
        node = NULL;
    } else {
        node = jobj->child;
    }
    if (node != NULL) {
        node = node->child;
        for (i = 0; i < 25; i++, node = node->next) {
            if (node == NULL) {
                return;
            }
            s = Camera_80029020();
            gp->gv.pura3.x128[i] = s;
            if (s != NULL) {
                gp->gv.pura3.xC4[i] = node;
                lb_8000B1CC(gp->gv.pura3.xC4[i], NULL, &sp10);
                s->x10 = sp10;
                if (HSD_JObjGetFlags(node) & 0x10) {
                    s->x8 = 1;
                } else {
                    s->x8 = 0;
                }
                s->x48.x = 30.0f;
                s->x48.y = -30.0f;
                s->x40.x = -30.0f;
                s->x40.y = 30.0f;
                s->x2C.x = s->x40.x;
                s->x2C.y = s->x40.y;
                s->x34.x = s->x48.x;
                s->x34.y = s->x48.y;
                s->x34.z = s->x48.z;
            }
        }
    }
}

void grPura_80212EF4(HSD_GObj* arg0)
{
    s32 i;
    Ground* gp = arg0->user_data;
    Ground* gp2 = gp;
    Vec3 spC;

    for (i = 0; i < 25; i++) {
        if (gp->gv.pura3.xC4[i] != NULL && gp->gv.pura3.x128[i] != NULL) {
            lb_8000B1CC(gp2->gv.pura3.xC4[i], NULL, &spC);
            gp->gv.pura3.x128[i]->x10 = spC;
            if (HSD_JObjGetFlags(gp2->gv.pura3.xC4[i]) & 0x10) {
                gp->gv.pura3.x128[i]->x8 = 1;
            } else {
                gp->gv.pura3.x128[i]->x8 = 0;
            }
        }
    }
}

void grPura_80212FC0(HSD_GObj* arg0)
{
    u32 var_r30;
    for (var_r30 = 0; var_r30 < 0x2A; var_r30++) {
        if (grPu_803E6C0C[var_r30].x00 != -1) {
            HSD_GObj* temp_r3 = Ground_801C2BA4(grPu_803E6C0C[var_r30].x02);
            if (temp_r3 != NULL) {
                grPu_803E6C0C[var_r30].x08 =
                    Ground_801C3FA4(temp_r3, grPu_803E6C0C[var_r30].x04);
            }
        }
    }
}

void grPura_80213030(Ground_GObj* arg0)
{
    UNUSED u32 unused1;
    Point3d spC;
    u32 var_r30;

    for (var_r30 = 0; var_r30 < 0x2A; var_r30++) {
        if (grPu_803E6C0C[var_r30].x08 != NULL) {
            lb_8000B1CC(grPu_803E6C0C[var_r30].x08, NULL, &spC);
            mpVtxSetPos(grPu_803E6C0C[var_r30].x00, spC.x, spC.y);
        }
    }
    mpJointUpdateBounding(0);
    mpJointUpdateBounding(9);
    mpJointUpdateBounding(0x18);
    mpJointUpdateBounding(5);
}

DynamicsDesc* grPura_802130C0(enum_t arg0)
{
    return false;
}

bool grPura_802130C8(Vec3* a, int num, HSD_JObj* joint)
{
    return true;
}

void fn_802130D0(HSD_GObj* arg0, int arg1)
{
    PAD_STACK(8);
    HSD_MObjSetToonTextureImage(&grPu_803E7620);
    grDisplay_801C5DB0(arg0, arg1);
    HSD_MObjSetToonTextureImage(0);
}

void grPura_80213128(HSD_DObj* dobj)
{
    HSD_DObj* iter;
    HSD_DObj* next;
    HSD_DObj* next2;

    if ((next = dobj->next) != NULL) {
        if ((next2 = next->next) != NULL) {
            if (next2->next != NULL) {
                grPura_80213128(next2->next);
            }
            for (iter = next2; iter != NULL; iter = iter->next) {
                grPura_80213224(iter);
            }
            if (next2->mobj != NULL) {
                HSD_MObjCompileTev(next2->mobj);
            }
        }
        for (iter = next; iter != NULL; iter = iter->next) {
            if (iter != NULL) {
                HSD_MObjCompileTev(iter->mobj);
            }
        }
        if (next->mobj != NULL) {
            HSD_MObjCompileTev(next->mobj);
        }
    }
    for (iter = dobj; iter != NULL; iter = iter->next) {
        if (iter != NULL) {
            HSD_MObjCompileTev(iter->mobj);
        }
    }
    if (dobj->mobj != NULL) {
        HSD_MObjCompileTev(dobj->mobj);
    }
}

void grPura_80213224(HSD_DObj* dobj)
{
    if (dobj != 0) {
        HSD_MObjCompileTev(dobj->mobj);
    }
}

void grPura_80213250(HSD_JObj* jobj)
{
    HSD_JObj* child = jobj->child;
    HSD_DObj* dobj;
    HSD_DObj* iter;

    if (child != NULL) {
        if (child->child != NULL) {
            grPura_80213250(child->child);
        }
        if (child->next != NULL) {
            grPura_80213250(child->next);
        }
        if (union_type_dobj(child)) {
            dobj = child->u.dobj;
            if (dobj != NULL) {
                if (dobj->next != NULL) {
                    grPura_80213128(dobj->next);
                }
                for (iter = dobj; iter != NULL; iter = iter->next) {
                    grPura_80213224(iter);
                }
                if (dobj->mobj != NULL) {
                    HSD_MObjCompileTev(dobj->mobj);
                }
            }
        }
    }

    child = jobj->next;
    if (child != NULL) {
        if (child->child != NULL) {
            grPura_80213250(child->child);
        }
        if (child->next != NULL) {
            grPura_80213250(child->next);
        }
        if (union_type_dobj(child)) {
            dobj = child->u.dobj;
            if (dobj != NULL) {
                if (dobj->next != NULL) {
                    grPura_80213128(dobj->next);
                }
                for (iter = dobj; iter != NULL; iter = iter->next) {
                    grPura_80213224(iter);
                }
                if (dobj->mobj != NULL) {
                    HSD_MObjCompileTev(dobj->mobj);
                }
            }
        }
    }

    if (union_type_dobj(jobj)) {
        dobj = jobj->u.dobj;
        if (dobj != NULL) {
            if (dobj->next != NULL) {
                grPura_80213128(dobj->next);
            }
            for (iter = dobj; iter != NULL; iter = iter->next) {
                grPura_80213224(iter);
            }
            if (dobj->mobj != NULL) {
                HSD_MObjCompileTev(dobj->mobj);
            }
        }
    }
}
