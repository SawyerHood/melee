#include "mnnamenew.h"

#include "baselib/debug.h"
#include "dolphin/gx/GXStruct.h"
#include "gm/gm_18A5.h"
#include "gm/gm_1A3F.h"
#include "gm/gmmain_lib.h"
#include "lb/lb_00B0.h"
#include "lb/lb_00F9.h"
#include "lb/lbarchive.h"
#include "lb/lbaudio_ax.h"
#include "lb/lbcardgame.h"
#include "lb/lblanguage.h"
#include "mn/mncharsel.h"
#include "mn/mnmain.h"
#include "mn/mnname.h"
#include "mn/types.h"
#include "sc/types.h"
#include "sysdolphin/baselib/gobj.h"
#include "sysdolphin/baselib/gobjgxlink.h"
#include "sysdolphin/baselib/gobjobject.h"
#include "sysdolphin/baselib/gobjplink.h"
#include "sysdolphin/baselib/gobjproc.h"
#include "sysdolphin/baselib/gobjuserdata.h"
#include "sysdolphin/baselib/jobj.h"
#include "sysdolphin/baselib/memory.h"
#include "sysdolphin/baselib/random.h"
#include "sysdolphin/baselib/sislib.h"

#include <dolphin/os.h>
typedef char* GlyphRow[4];

typedef struct MnNameNewDataLayout {
    AnimLoopSettings anim[3];
    u16 key_jobj_ids[8];
    char* x34[50];
    char* xFC[50];
    char* character_bytes[50];
    GlyphRow lower_glyphs[50];
    GlyphRow upper_glyphs[50];
} MnNameNewDataLayout;

extern volatile char mnNameNew_NullCharacter[1];
extern u8 mnNameNew_PortInUse;
extern char mnNameNew_CurrentNameText[0x10];
extern u8** AutoNamesList;
extern char** NotAllowedNamesList;
extern u8 mn_804D6BB4;
extern u8 mn_804D6BB5;
extern u8 mnNameNew_804D4F7C[8];
extern char mnNameNew_SpaceCharacter[2];
extern GlyphRow mnNameNew_803EDCE4[50];
extern void* mnNameNew_804A06F0[];
extern void* mnNameNew_804A0700[];
extern void* mnNameNew_804A0710[];
extern void* mnNameNew_804A0720[];
extern HSD_GObj* mnNameNew_804D6C08;

extern u16 mnNameNew_803EDA7C[];
extern StaticModelDesc MenMainBack_Top;
extern StaticModelDesc MenMainPanel_Top;
extern HSD_CObjDesc* MenMain_cam;
extern UNK_T MenMain_lights;
extern HSD_FogDesc* MenMain_fog;

/* === datarecon-w27: target .sdata 0x0..0x390 -- glyph cells + colors + misc, in
   target order; .bss/.sbss blocks; pools @327/@328 emit later from KeySetup === */
f32 mnNameNew_804D4C10 = 10.0f; /* moved from below, de-static (idiom 139) */
u8 mnNameNew_804D4C14[4] = { 0x82, 0xA0, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C18[4] = { 0x82, 0xA2, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C1C[4] = { 0x82, 0xA4, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C20[4] = { 0x82, 0xA6, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C24[4] = { 0x82, 0xA8, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C28[4] = { 0x82, 0xA9, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C2C[4] = { 0x82, 0xAB, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C30[4] = { 0x82, 0xAD, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C34[4] = { 0x82, 0xAF, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C38[4] = { 0x82, 0xB1, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C3C[4] = { 0x82, 0xB3, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C40[4] = { 0x82, 0xB5, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C44[4] = { 0x82, 0xB7, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C48[4] = { 0x82, 0xB9, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C4C[4] = { 0x82, 0xBB, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C50[4] = { 0x82, 0xBD, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C54[4] = { 0x82, 0xBF, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C58[4] = { 0x82, 0xC2, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C5C[4] = { 0x82, 0xC4, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C60[4] = { 0x82, 0xC6, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C64[4] = { 0x82, 0xC8, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C68[4] = { 0x82, 0xC9, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C6C[4] = { 0x82, 0xCA, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C70[4] = { 0x82, 0xCB, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C74[4] = { 0x82, 0xCC, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C78[4] = { 0x82, 0xCD, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C7C[4] = { 0x82, 0xD0, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C80[4] = { 0x82, 0xD3, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C84[4] = { 0x82, 0xD6, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C88[4] = { 0x82, 0xD9, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C8C[4] = { 0x82, 0xDC, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C90[4] = { 0x82, 0xDD, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C94[4] = { 0x82, 0xDE, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C98[4] = { 0x82, 0xDF, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4C9C[4] = { 0x82, 0xE0, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CA0[4] = { 0x82, 0xE2, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CA4[4] = { 0x82, 0xE4, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CA8[4] = { 0x82, 0xE6, 0x00, 0x00 }; /* "...." */
char mnNameNew_SpaceCharacter[2] = { 0x81, 0x40 }; /* SJIS space; covers dtk-split lbl_804D4CAD */
u8 mnNameNew_804D4CB0[4] = { 0x82, 0xE7, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CB4[4] = { 0x82, 0xE8, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CB8[4] = { 0x82, 0xE9, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CBC[4] = { 0x82, 0xEA, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CC0[4] = { 0x82, 0xEB, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CC4[4] = { 0x82, 0xED, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CC8[4] = { 0x82, 0xF0, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CCC[4] = { 0x82, 0xF1, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4CD0[4] = { 0x81, 0x5B, 0x00, 0x00 }; /* ".[.." */
u8 mnNameNew_804D4CD4[4] = { 0x81, 0x42, 0x00, 0x00 }; /* ".B.." */
u8 mnNameNew_804D4CD8[4] = { 0x83, 0x41, 0x00, 0x00 }; /* ".A.." */
u8 mnNameNew_804D4CDC[4] = { 0x83, 0x43, 0x00, 0x00 }; /* ".C.." */
u8 mnNameNew_804D4CE0[4] = { 0x83, 0x45, 0x00, 0x00 }; /* ".E.." */
u8 mnNameNew_804D4CE4[4] = { 0x83, 0x47, 0x00, 0x00 }; /* ".G.." */
u8 mnNameNew_804D4CE8[4] = { 0x83, 0x49, 0x00, 0x00 }; /* ".I.." */
u8 mnNameNew_804D4CEC[4] = { 0x83, 0x4A, 0x00, 0x00 }; /* ".J.." */
u8 mnNameNew_804D4CF0[4] = { 0x83, 0x4C, 0x00, 0x00 }; /* ".L.." */
u8 mnNameNew_804D4CF4[4] = { 0x83, 0x4E, 0x00, 0x00 }; /* ".N.." */
u8 mnNameNew_804D4CF8[4] = { 0x83, 0x50, 0x00, 0x00 }; /* ".P.." */
u8 mnNameNew_804D4CFC[4] = { 0x83, 0x52, 0x00, 0x00 }; /* ".R.." */
u8 mnNameNew_804D4D00[4] = { 0x83, 0x54, 0x00, 0x00 }; /* ".T.." */
u8 mnNameNew_804D4D04[4] = { 0x83, 0x56, 0x00, 0x00 }; /* ".V.." */
u8 mnNameNew_804D4D08[4] = { 0x83, 0x58, 0x00, 0x00 }; /* ".X.." */
u8 mnNameNew_804D4D0C[4] = { 0x83, 0x5A, 0x00, 0x00 }; /* ".Z.." */
u8 mnNameNew_804D4D10[4] = { 0x83, 0x5C, 0x00, 0x00 }; /* ".\.." */
u8 mnNameNew_804D4D14[4] = { 0x83, 0x5E, 0x00, 0x00 }; /* ".^.." */
u8 mnNameNew_804D4D18[4] = { 0x83, 0x60, 0x00, 0x00 }; /* ".`.." */
u8 mnNameNew_804D4D1C[4] = { 0x83, 0x63, 0x00, 0x00 }; /* ".c.." */
u8 mnNameNew_804D4D20[4] = { 0x83, 0x65, 0x00, 0x00 }; /* ".e.." */
u8 mnNameNew_804D4D24[4] = { 0x83, 0x67, 0x00, 0x00 }; /* ".g.." */
u8 mnNameNew_804D4D28[4] = { 0x83, 0x69, 0x00, 0x00 }; /* ".i.." */
u8 mnNameNew_804D4D2C[4] = { 0x83, 0x6A, 0x00, 0x00 }; /* ".j.." */
u8 mnNameNew_804D4D30[4] = { 0x83, 0x6B, 0x00, 0x00 }; /* ".k.." */
u8 mnNameNew_804D4D34[4] = { 0x83, 0x6C, 0x00, 0x00 }; /* ".l.." */
u8 mnNameNew_804D4D38[4] = { 0x83, 0x6D, 0x00, 0x00 }; /* ".m.." */
u8 mnNameNew_804D4D3C[4] = { 0x83, 0x6E, 0x00, 0x00 }; /* ".n.." */
u8 mnNameNew_804D4D40[4] = { 0x83, 0x71, 0x00, 0x00 }; /* ".q.." */
u8 mnNameNew_804D4D44[4] = { 0x83, 0x74, 0x00, 0x00 }; /* ".t.." */
u8 mnNameNew_804D4D48[4] = { 0x83, 0x77, 0x00, 0x00 }; /* ".w.." */
u8 mnNameNew_804D4D4C[4] = { 0x83, 0x7A, 0x00, 0x00 }; /* ".z.." */
u8 mnNameNew_804D4D50[4] = { 0x83, 0x7D, 0x00, 0x00 }; /* ".}.." */
u8 mnNameNew_804D4D54[4] = { 0x83, 0x7E, 0x00, 0x00 }; /* ".~.." */
u8 mnNameNew_804D4D58[4] = { 0x83, 0x80, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D5C[4] = { 0x83, 0x81, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D60[4] = { 0x83, 0x82, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D64[4] = { 0x83, 0x84, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D68[4] = { 0x83, 0x86, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D6C[4] = { 0x83, 0x88, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D70[4] = { 0x83, 0x89, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D74[4] = { 0x83, 0x8A, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D78[4] = { 0x83, 0x8B, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D7C[4] = { 0x83, 0x8C, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D80[4] = { 0x83, 0x8D, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D84[4] = { 0x83, 0x8F, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D88[4] = { 0x83, 0x92, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D8C[4] = { 0x83, 0x93, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4D90[4] = { 0x82, 0x69, 0x00, 0x00 }; /* ".i.." */
u8 mnNameNew_804D4D94[4] = { 0x82, 0x73, 0x00, 0x00 }; /* ".s.." */
u8 mnNameNew_804D4D98[4] = { 0x82, 0x58, 0x00, 0x00 }; /* ".X.." */
u8 mnNameNew_804D4D9C[4] = { 0x81, 0x44, 0x00, 0x00 }; /* ".D.." */
u8 mnNameNew_804D4DA0[4] = { 0x82, 0x68, 0x00, 0x00 }; /* ".h.." */
u8 mnNameNew_804D4DA4[4] = { 0x82, 0x72, 0x00, 0x00 }; /* ".r.." */
u8 mnNameNew_804D4DA8[4] = { 0x82, 0x57, 0x00, 0x00 }; /* ".W.." */
u8 mnNameNew_804D4DAC[4] = { 0x81, 0x90, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4DB0[4] = { 0x82, 0x67, 0x00, 0x00 }; /* ".g.." */
u8 mnNameNew_804D4DB4[4] = { 0x82, 0x71, 0x00, 0x00 }; /* ".q.." */
u8 mnNameNew_804D4DB8[4] = { 0x82, 0x56, 0x00, 0x00 }; /* ".V.." */
u8 mnNameNew_804D4DBC[4] = { 0x81, 0x95, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4DC0[4] = { 0x82, 0x66, 0x00, 0x00 }; /* ".f.." */
u8 mnNameNew_804D4DC4[4] = { 0x82, 0x70, 0x00, 0x00 }; /* ".p.." */
u8 mnNameNew_804D4DC8[4] = { 0x82, 0x55, 0x00, 0x00 }; /* ".U.." */
u8 mnNameNew_804D4DCC[4] = { 0x81, 0x93, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4DD0[4] = { 0x82, 0x65, 0x00, 0x00 }; /* ".e.." */
u8 mnNameNew_804D4DD4[4] = { 0x82, 0x6F, 0x00, 0x00 }; /* ".o.." */
u8 mnNameNew_804D4DD8[4] = { 0x82, 0x79, 0x00, 0x00 }; /* ".y.." */
u8 mnNameNew_804D4DDC[4] = { 0x82, 0x54, 0x00, 0x00 }; /* ".T.." */
u8 mnNameNew_804D4DE0[4] = { 0x81, 0x97, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4DE4[4] = { 0x82, 0x64, 0x00, 0x00 }; /* ".d.." */
u8 mnNameNew_804D4DE8[4] = { 0x82, 0x6E, 0x00, 0x00 }; /* ".n.." */
u8 mnNameNew_804D4DEC[4] = { 0x82, 0x78, 0x00, 0x00 }; /* ".x.." */
u8 mnNameNew_804D4DF0[4] = { 0x82, 0x53, 0x00, 0x00 }; /* ".S.." */
u8 mnNameNew_804D4DF4[4] = { 0x81, 0x48, 0x00, 0x00 }; /* ".H.." */
u8 mnNameNew_804D4DF8[4] = { 0x82, 0x63, 0x00, 0x00 }; /* ".c.." */
u8 mnNameNew_804D4DFC[4] = { 0x82, 0x6D, 0x00, 0x00 }; /* ".m.." */
u8 mnNameNew_804D4E00[4] = { 0x82, 0x77, 0x00, 0x00 }; /* ".w.." */
u8 mnNameNew_804D4E04[4] = { 0x82, 0x52, 0x00, 0x00 }; /* ".R.." */
u8 mnNameNew_804D4E08[4] = { 0x81, 0x49, 0x00, 0x00 }; /* ".I.." */
u8 mnNameNew_804D4E0C[4] = { 0x82, 0x62, 0x00, 0x00 }; /* ".b.." */
u8 mnNameNew_804D4E10[4] = { 0x82, 0x6C, 0x00, 0x00 }; /* ".l.." */
u8 mnNameNew_804D4E14[4] = { 0x82, 0x76, 0x00, 0x00 }; /* ".v.." */
u8 mnNameNew_804D4E18[4] = { 0x82, 0x51, 0x00, 0x00 }; /* ".Q.." */
u8 mnNameNew_804D4E1C[4] = { 0x81, 0x81, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E20[4] = { 0x82, 0x61, 0x00, 0x00 }; /* ".a.." */
u8 mnNameNew_804D4E24[4] = { 0x82, 0x6B, 0x00, 0x00 }; /* ".k.." */
u8 mnNameNew_804D4E28[4] = { 0x82, 0x75, 0x00, 0x00 }; /* ".u.." */
u8 mnNameNew_804D4E2C[4] = { 0x82, 0x50, 0x00, 0x00 }; /* ".P.." */
u8 mnNameNew_804D4E30[4] = { 0x81, 0x7B, 0x00, 0x00 }; /* ".{.." */
u8 mnNameNew_804D4E34[4] = { 0x82, 0x60, 0x00, 0x00 }; /* ".`.." */
u8 mnNameNew_804D4E38[4] = { 0x82, 0x6A, 0x00, 0x00 }; /* ".j.." */
u8 mnNameNew_804D4E3C[4] = { 0x82, 0x74, 0x00, 0x00 }; /* ".t.." */
u8 mnNameNew_804D4E40[4] = { 0x82, 0x4F, 0x00, 0x00 }; /* ".O.." */
u8 mnNameNew_804D4E44[4] = { 0x81, 0x7C, 0x00, 0x00 }; /* ".|.." */
u8 mnNameNew_804D4E48[4] = { 0x82, 0x9F, 0x00, 0x00 }; /* "...." */
volatile char mnNameNew_NullCharacter[1] = { 0 }; /* braced-zero array -> .sdata @0x23c (scalar form lands .sbss, probed) */
u8 mnNameNew_804D4E50[4] = { 0x82, 0xA1, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E54[4] = { 0x82, 0xA3, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E58[4] = { 0x82, 0xA5, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E5C[4] = { 0x82, 0xA7, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E60[4] = { 0x82, 0xAA, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E64[4] = { 0x82, 0xAC, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E68[4] = { 0x82, 0xAE, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E6C[4] = { 0x82, 0xB0, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E70[4] = { 0x82, 0xB2, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E74[4] = { 0x82, 0xB4, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E78[4] = { 0x82, 0xB6, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E7C[4] = { 0x82, 0xB8, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E80[4] = { 0x82, 0xBA, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E84[4] = { 0x82, 0xBC, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E88[4] = { 0x82, 0xBE, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E8C[4] = { 0x82, 0xC0, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E90[4] = { 0x82, 0xC3, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E94[4] = { 0x82, 0xC1, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E98[4] = { 0x82, 0xC5, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4E9C[4] = { 0x82, 0xC7, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EA0[4] = { 0x82, 0xCE, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EA4[4] = { 0x82, 0xCF, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EA8[4] = { 0x82, 0xD1, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EAC[4] = { 0x82, 0xD2, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EB0[4] = { 0x82, 0xD4, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EB4[4] = { 0x82, 0xD5, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EB8[4] = { 0x82, 0xD7, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EBC[4] = { 0x82, 0xD8, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EC0[4] = { 0x82, 0xDA, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EC4[4] = { 0x82, 0xDB, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4EC8[4] = { 0x82, 0xE1, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4ECC[4] = { 0x82, 0xE3, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4ED0[4] = { 0x82, 0xE5, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4ED4[2] = { 0x20, 0x00 }; /* " ." */
u8 mnNameNew_804D4ED8[4] = { 0x83, 0x40, 0x00, 0x00 }; /* ".@.." */
u8 mnNameNew_804D4EDC[4] = { 0x83, 0x42, 0x00, 0x00 }; /* ".B.." */
u8 mnNameNew_804D4EE0[4] = { 0x83, 0x44, 0x00, 0x00 }; /* ".D.." */
u8 mnNameNew_804D4EE4[4] = { 0x83, 0x46, 0x00, 0x00 }; /* ".F.." */
u8 mnNameNew_804D4EE8[4] = { 0x83, 0x48, 0x00, 0x00 }; /* ".H.." */
u8 mnNameNew_804D4EEC[4] = { 0x83, 0x4B, 0x00, 0x00 }; /* ".K.." */
u8 mnNameNew_804D4EF0[4] = { 0x83, 0x4D, 0x00, 0x00 }; /* ".M.." */
u8 mnNameNew_804D4EF4[4] = { 0x83, 0x4F, 0x00, 0x00 }; /* ".O.." */
u8 mnNameNew_804D4EF8[4] = { 0x83, 0x51, 0x00, 0x00 }; /* ".Q.." */
u8 mnNameNew_804D4EFC[4] = { 0x83, 0x53, 0x00, 0x00 }; /* ".S.." */
u8 mnNameNew_804D4F00[4] = { 0x83, 0x55, 0x00, 0x00 }; /* ".U.." */
u8 mnNameNew_804D4F04[4] = { 0x83, 0x57, 0x00, 0x00 }; /* ".W.." */
u8 mnNameNew_804D4F08[4] = { 0x83, 0x59, 0x00, 0x00 }; /* ".Y.." */
u8 mnNameNew_804D4F0C[4] = { 0x83, 0x5B, 0x00, 0x00 }; /* ".[.." */
u8 mnNameNew_804D4F10[4] = { 0x83, 0x5D, 0x00, 0x00 }; /* ".].." */
u8 mnNameNew_804D4F14[4] = { 0x83, 0x5F, 0x00, 0x00 }; /* "._.." */
u8 mnNameNew_804D4F18[4] = { 0x83, 0x61, 0x00, 0x00 }; /* ".a.." */
u8 mnNameNew_804D4F1C[4] = { 0x83, 0x64, 0x00, 0x00 }; /* ".d.." */
u8 mnNameNew_804D4F20[4] = { 0x83, 0x62, 0x00, 0x00 }; /* ".b.." */
u8 mnNameNew_804D4F24[4] = { 0x83, 0x66, 0x00, 0x00 }; /* ".f.." */
u8 mnNameNew_804D4F28[4] = { 0x83, 0x68, 0x00, 0x00 }; /* ".h.." */
u8 mnNameNew_804D4F2C[4] = { 0x83, 0x6F, 0x00, 0x00 }; /* ".o.." */
u8 mnNameNew_804D4F30[4] = { 0x83, 0x70, 0x00, 0x00 }; /* ".p.." */
u8 mnNameNew_804D4F34[4] = { 0x83, 0x72, 0x00, 0x00 }; /* ".r.." */
u8 mnNameNew_804D4F38[4] = { 0x83, 0x73, 0x00, 0x00 }; /* ".s.." */
u8 mnNameNew_804D4F3C[4] = { 0x83, 0x75, 0x00, 0x00 }; /* ".u.." */
u8 mnNameNew_804D4F40[4] = { 0x83, 0x76, 0x00, 0x00 }; /* ".v.." */
u8 mnNameNew_804D4F44[4] = { 0x83, 0x78, 0x00, 0x00 }; /* ".x.." */
u8 mnNameNew_804D4F48[4] = { 0x83, 0x79, 0x00, 0x00 }; /* ".y.." */
u8 mnNameNew_804D4F4C[4] = { 0x83, 0x7B, 0x00, 0x00 }; /* ".{.." */
u8 mnNameNew_804D4F50[4] = { 0x83, 0x7C, 0x00, 0x00 }; /* ".|.." */
u8 mnNameNew_804D4F54[4] = { 0x83, 0x83, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4F58[4] = { 0x83, 0x85, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4F5C[4] = { 0x83, 0x87, 0x00, 0x00 }; /* "...." */
u8 mnNameNew_804D4F60[4] = { 0x81, 0x60, 0x00, 0x00 }; /* ".`.." */
u8 mnNameNew_804D4F64[4] = { 0x81, 0x41, 0x00, 0x00 }; /* ".A.." */
GXColor mnNameNew_804D4F68 = { 0, 0, 0, 0xFF }; /* moved, de-static */
GXColor mnNameNew_804D4F6C = { 0xA6, 0x81, 0x3D, 0xFF };
GXColor mnNameNew_804D4F70 = { 0, 0, 0, 0xFF };
GXColor mnNameNew_804D4F74 = { 0x74, 0x4F, 0x0B, 0xFF };
GXColor mnNameNew_804D4F78 = { 0, 0, 0, 0xFF };
u8 mnNameNew_804D4F7C[8] = { 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x48 }; /* "BCDEFGHH" */

/* .sbss (target 0x8); .bss quartet PROBE-REVERTED (see progress log) */
HSD_GObj* mnNameNew_804D6C08;
u8 mnNameNew_PortInUse;

AnimLoopSettings mnNameNew_803EDA58[3] = {
    { 0.0f, 19.0f, -0.1f },
    { 20.0f, 39.0f, -0.1f },
    { 0.0f, 10.0f, -0.1f },
};

extern char* mnNameNew_803EDA8C[50];
extern char* mnNameNew_803EDB54[50];
extern char* mnNameNew_803EDC1C[50];
extern GlyphRow mnNameNew_803EE004[50];
extern Vec3 mnNameNew_803EE324;

/* === datarecon-w27: .data 0x24..0x8d8 -- key-count header + 3 x char*[50] glyph
   pointer arrays (dtk merged into mnNameNew_803EDA7C) + GlyphRow[50] x2 (dtk
   mnNameNew_803EDCE4) + dtk-glommed Vec3 mnNameNew_803EE324 === */
u16 mnNameNew_803EDA7C[8] = { 3, 10, 11, 7, 9, 8, 5, 5 };
char* mnNameNew_803EDA8C[50] = {
    (char*) mnNameNew_804D4C14, (char*) mnNameNew_804D4C18,
    (char*) mnNameNew_804D4C1C, (char*) mnNameNew_804D4C20,
    (char*) mnNameNew_804D4C24, (char*) mnNameNew_804D4C28,
    (char*) mnNameNew_804D4C2C, (char*) mnNameNew_804D4C30,
    (char*) mnNameNew_804D4C34, (char*) mnNameNew_804D4C38,
    (char*) mnNameNew_804D4C3C, (char*) mnNameNew_804D4C40,
    (char*) mnNameNew_804D4C44, (char*) mnNameNew_804D4C48,
    (char*) mnNameNew_804D4C4C, (char*) mnNameNew_804D4C50,
    (char*) mnNameNew_804D4C54, (char*) mnNameNew_804D4C58,
    (char*) mnNameNew_804D4C5C, (char*) mnNameNew_804D4C60,
    (char*) mnNameNew_804D4C64, (char*) mnNameNew_804D4C68,
    (char*) mnNameNew_804D4C6C, (char*) mnNameNew_804D4C70,
    (char*) mnNameNew_804D4C74, (char*) mnNameNew_804D4C78,
    (char*) mnNameNew_804D4C7C, (char*) mnNameNew_804D4C80,
    (char*) mnNameNew_804D4C84, (char*) mnNameNew_804D4C88,
    (char*) mnNameNew_804D4C8C, (char*) mnNameNew_804D4C90,
    (char*) mnNameNew_804D4C94, (char*) mnNameNew_804D4C98,
    (char*) mnNameNew_804D4C9C, (char*) mnNameNew_804D4CA0,
    (char*) mnNameNew_804D4CA4, (char*) mnNameNew_804D4CA8,
    (char*) mnNameNew_SpaceCharacter, (char*) mnNameNew_SpaceCharacter,
    (char*) mnNameNew_804D4CB0, (char*) mnNameNew_804D4CB4,
    (char*) mnNameNew_804D4CB8, (char*) mnNameNew_804D4CBC,
    (char*) mnNameNew_804D4CC0, (char*) mnNameNew_804D4CC4,
    (char*) mnNameNew_804D4CC8, (char*) mnNameNew_804D4CCC,
    (char*) mnNameNew_804D4CD0, (char*) mnNameNew_804D4CD4
};
char* mnNameNew_803EDB54[50] = {
    (char*) mnNameNew_804D4CD8, (char*) mnNameNew_804D4CDC,
    (char*) mnNameNew_804D4CE0, (char*) mnNameNew_804D4CE4,
    (char*) mnNameNew_804D4CE8, (char*) mnNameNew_804D4CEC,
    (char*) mnNameNew_804D4CF0, (char*) mnNameNew_804D4CF4,
    (char*) mnNameNew_804D4CF8, (char*) mnNameNew_804D4CFC,
    (char*) mnNameNew_804D4D00, (char*) mnNameNew_804D4D04,
    (char*) mnNameNew_804D4D08, (char*) mnNameNew_804D4D0C,
    (char*) mnNameNew_804D4D10, (char*) mnNameNew_804D4D14,
    (char*) mnNameNew_804D4D18, (char*) mnNameNew_804D4D1C,
    (char*) mnNameNew_804D4D20, (char*) mnNameNew_804D4D24,
    (char*) mnNameNew_804D4D28, (char*) mnNameNew_804D4D2C,
    (char*) mnNameNew_804D4D30, (char*) mnNameNew_804D4D34,
    (char*) mnNameNew_804D4D38, (char*) mnNameNew_804D4D3C,
    (char*) mnNameNew_804D4D40, (char*) mnNameNew_804D4D44,
    (char*) mnNameNew_804D4D48, (char*) mnNameNew_804D4D4C,
    (char*) mnNameNew_804D4D50, (char*) mnNameNew_804D4D54,
    (char*) mnNameNew_804D4D58, (char*) mnNameNew_804D4D5C,
    (char*) mnNameNew_804D4D60, (char*) mnNameNew_804D4D64,
    (char*) mnNameNew_804D4D68, (char*) mnNameNew_804D4D6C,
    (char*) mnNameNew_SpaceCharacter, (char*) mnNameNew_SpaceCharacter,
    (char*) mnNameNew_804D4D70, (char*) mnNameNew_804D4D74,
    (char*) mnNameNew_804D4D78, (char*) mnNameNew_804D4D7C,
    (char*) mnNameNew_804D4D80, (char*) mnNameNew_804D4D84,
    (char*) mnNameNew_804D4D88, (char*) mnNameNew_804D4D8C,
    (char*) mnNameNew_804D4CD0, (char*) mnNameNew_804D4CD4
};
char* mnNameNew_803EDC1C[50] = {
    (char*) mnNameNew_804D4D90, (char*) mnNameNew_804D4D94,
    (char*) mnNameNew_SpaceCharacter, (char*) mnNameNew_804D4D98,
    (char*) mnNameNew_804D4D9C, (char*) mnNameNew_804D4DA0,
    (char*) mnNameNew_804D4DA4, (char*) mnNameNew_SpaceCharacter,
    (char*) mnNameNew_804D4DA8, (char*) mnNameNew_804D4DAC,
    (char*) mnNameNew_804D4DB0, (char*) mnNameNew_804D4DB4,
    (char*) mnNameNew_SpaceCharacter, (char*) mnNameNew_804D4DB8,
    (char*) mnNameNew_804D4DBC, (char*) mnNameNew_804D4DC0,
    (char*) mnNameNew_804D4DC4, (char*) mnNameNew_SpaceCharacter,
    (char*) mnNameNew_804D4DC8, (char*) mnNameNew_804D4DCC,
    (char*) mnNameNew_804D4DD0, (char*) mnNameNew_804D4DD4,
    (char*) mnNameNew_804D4DD8, (char*) mnNameNew_804D4DDC,
    (char*) mnNameNew_804D4DE0, (char*) mnNameNew_804D4DE4,
    (char*) mnNameNew_804D4DE8, (char*) mnNameNew_804D4DEC,
    (char*) mnNameNew_804D4DF0, (char*) mnNameNew_804D4DF4,
    (char*) mnNameNew_804D4DF8, (char*) mnNameNew_804D4DFC,
    (char*) mnNameNew_804D4E00, (char*) mnNameNew_804D4E04,
    (char*) mnNameNew_804D4E08, (char*) mnNameNew_804D4E0C,
    (char*) mnNameNew_804D4E10, (char*) mnNameNew_804D4E14,
    (char*) mnNameNew_804D4E18, (char*) mnNameNew_804D4E1C,
    (char*) mnNameNew_804D4E20, (char*) mnNameNew_804D4E24,
    (char*) mnNameNew_804D4E28, (char*) mnNameNew_804D4E2C,
    (char*) mnNameNew_804D4E30, (char*) mnNameNew_804D4E34,
    (char*) mnNameNew_804D4E38, (char*) mnNameNew_804D4E3C,
    (char*) mnNameNew_804D4E40, (char*) mnNameNew_804D4E44
};
GlyphRow mnNameNew_803EDCE4[50] = {
    { (char*) mnNameNew_804D4C14, (char*) mnNameNew_804D4E48, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C18, (char*) mnNameNew_804D4E50, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C1C, (char*) mnNameNew_804D4E54, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C20, (char*) mnNameNew_804D4E58, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C24, (char*) mnNameNew_804D4E5C, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C28, (char*) mnNameNew_804D4E60, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C2C, (char*) mnNameNew_804D4E64, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C30, (char*) mnNameNew_804D4E68, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C34, (char*) mnNameNew_804D4E6C, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C38, (char*) mnNameNew_804D4E70, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C3C, (char*) mnNameNew_804D4E74, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C40, (char*) mnNameNew_804D4E78, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C44, (char*) mnNameNew_804D4E7C, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C48, (char*) mnNameNew_804D4E80, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C4C, (char*) mnNameNew_804D4E84, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C50, (char*) mnNameNew_804D4E88, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C54, (char*) mnNameNew_804D4E8C, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C58, (char*) mnNameNew_804D4E90, (char*) mnNameNew_804D4E94, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4C5C, (char*) mnNameNew_804D4E98, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C60, (char*) mnNameNew_804D4E9C, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C64, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C68, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C6C, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C70, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C74, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C78, (char*) mnNameNew_804D4EA0, (char*) mnNameNew_804D4EA4, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4C7C, (char*) mnNameNew_804D4EA8, (char*) mnNameNew_804D4EAC, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4C80, (char*) mnNameNew_804D4EB0, (char*) mnNameNew_804D4EB4, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4C84, (char*) mnNameNew_804D4EB8, (char*) mnNameNew_804D4EBC, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4C88, (char*) mnNameNew_804D4EC0, (char*) mnNameNew_804D4EC4, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4C8C, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C90, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C94, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C98, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4C9C, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CA0, (char*) mnNameNew_804D4EC8, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CA4, (char*) mnNameNew_804D4ECC, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CA8, (char*) mnNameNew_804D4ED0, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_SpaceCharacter, (char*) mnNameNew_804D4ED4, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_SpaceCharacter, (char*) mnNameNew_804D4ED4, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CB0, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4CB4, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4CB8, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4CBC, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4CC0, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4CC4, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4CC8, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4CCC, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4CD0, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4CD4, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
};
GlyphRow mnNameNew_803EE004[50] = {
    { (char*) mnNameNew_804D4CD8, (char*) mnNameNew_804D4ED8, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CDC, (char*) mnNameNew_804D4EDC, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CE0, (char*) mnNameNew_804D4EE0, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CE4, (char*) mnNameNew_804D4EE4, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CE8, (char*) mnNameNew_804D4EE8, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CEC, (char*) mnNameNew_804D4EEC, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CF0, (char*) mnNameNew_804D4EF0, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CF4, (char*) mnNameNew_804D4EF4, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CF8, (char*) mnNameNew_804D4EF8, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4CFC, (char*) mnNameNew_804D4EFC, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D00, (char*) mnNameNew_804D4F00, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D04, (char*) mnNameNew_804D4F04, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D08, (char*) mnNameNew_804D4F08, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D0C, (char*) mnNameNew_804D4F0C, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D10, (char*) mnNameNew_804D4F10, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D14, (char*) mnNameNew_804D4F14, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D18, (char*) mnNameNew_804D4F18, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D1C, (char*) mnNameNew_804D4F1C, (char*) mnNameNew_804D4F20, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4D20, (char*) mnNameNew_804D4F24, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D24, (char*) mnNameNew_804D4F28, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D28, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D2C, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D30, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D34, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D38, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D3C, (char*) mnNameNew_804D4F2C, (char*) mnNameNew_804D4F30, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4D40, (char*) mnNameNew_804D4F34, (char*) mnNameNew_804D4F38, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4D44, (char*) mnNameNew_804D4F3C, (char*) mnNameNew_804D4F40, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4D48, (char*) mnNameNew_804D4F44, (char*) mnNameNew_804D4F48, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4D4C, (char*) mnNameNew_804D4F4C, (char*) mnNameNew_804D4F50, (char*) &mnNameNew_NullCharacter[0] },
    { (char*) mnNameNew_804D4D50, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D54, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D58, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D5C, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D60, (char*) &mnNameNew_NullCharacter[0], (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D64, (char*) mnNameNew_804D4F54, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D68, (char*) mnNameNew_804D4F58, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D6C, (char*) mnNameNew_804D4F5C, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_SpaceCharacter, (char*) mnNameNew_804D4ED4, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_SpaceCharacter, (char*) mnNameNew_804D4ED4, (char*) &mnNameNew_NullCharacter[0], NULL },
    { (char*) mnNameNew_804D4D70, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4D74, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4D78, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4D7C, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4D80, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4D84, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4D88, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4D8C, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4F60, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
    { (char*) mnNameNew_804D4F64, (char*) &mnNameNew_NullCharacter[0], NULL, NULL },
};
Vec3 mnNameNew_803EE324 = { -0.8f, 0.4f, 0.0f };

Vec3 mnNameNew_803EE330 = { -0.7f, 0.7f, 0.0f };

void mnNameNew_8023B0F8(HSD_GObj* arg0, u8 arg1)
{
    HSD_JObj* jobj;
    NameNewEntry* data;

    PAD_STACK(8);

    data = arg0->user_data;
    jobj = data->jobjs[10];
    if (arg1 == 0x33) {
        HSD_JObjReqAnimAll(jobj, 1.0f);
    } else if (data->mode == 0) {
        HSD_JObjReqAnimAll(jobj, 2.0f);
    } else {
        HSD_JObjReqAnimAll(jobj, 0.0f);
    }
    HSD_JObjAnimAll(jobj);
    jobj = data->jobjs[11];
    if (arg1 == 0x34) {
        HSD_JObjReqAnimAll(jobj, 1.0f);
    } else if (data->mode == 1) {
        HSD_JObjReqAnimAll(jobj, 2.0f);
    } else {
        HSD_JObjReqAnimAll(jobj, 0.0f);
    }
    HSD_JObjAnimAll(jobj);
    jobj = data->jobjs[7];
    if (arg1 == 0x35) {
        HSD_JObjReqAnimAll(jobj, 1.0f);
    } else if (data->mode == 2) {
        HSD_JObjReqAnimAll(jobj, 2.0f);
    } else {
        HSD_JObjReqAnimAll(jobj, 0.0f);
    }
    HSD_JObjAnimAll(jobj);
}

void mnNameNew_8023B224(u8 arg0)
{
    NameNewEntry* temp_r31;
    u8 temp_r30;

    PAD_STACK(8);

    temp_r31 = ((HSD_GObj*) mnNameNew_804D6C08)->user_data;
    temp_r30 = temp_r31->name_index;
    if (arg0 != 0) {
        lb_8001CE00();
    }
    if (gm_801A4310() == 0x1B) {
        HSD_SisLib_803A5E70();
        mn_8022EBDC();
        if (arg0 != 0) {
            gm_80190FE4((s32) temp_r30);
            return;
        }
        gm_80190FE4(0x78);
        return;
    }
    if (gm_801A4310() == 1) {
        mn_804D6BC8.cooldown = 5;
        if (arg0 != 0 && GetNameCount() > 0x18) {
            mnName_8023A9B4((u8) (temp_r31->name_index / 6));
            return;
        }
        mnName_8023A9B4(0U);
        return;
    }
    HSD_SisLib_803A5E70();
    mn_8022EBDC();
    mnCharSel_802640A0();
}

void mnNameNew_8023B314(NameNewEntry* arg0, s32 arg1)
{
    HSD_Text* text;
    u8 idx;
    s32 var_r4;

    var_r4 = arg1;
    if (var_r4 < 0x32) {
        switch (arg0->mode) {
        case 0:
            var_r4 = 0x33;
            break;
        case 1:
            var_r4 = 0x34;
            break;
        case 2:
            var_r4 = 0x35;
            break;
        }
    }
    text = arg0->desc_text;
    idx = mnNameNew_804D4F7C[var_r4 - 0x32];
    if (text != NULL) {
        if (text->sis_buffer == ((SIS**) HSD_SisLib_804D1124[0])[idx]) {
            return;
        }
        HSD_SisLib_803A5CC4(arg0->desc_text);
    }
    text = HSD_SisLib_803A5ACC(0, (s32) mn_804D6BB4, -9.5f, 9.1f, 17.0f,
                               364.68332f, 38.38772f);
    arg0->desc_text = text;
    text->font_size.x = 0.0521f;
    text->font_size.y = 0.0521f;
    HSD_SisLib_803A6368(text, (s32) idx);
}

/* datarecon-w27: 4F6C..4F78 moved to the .sdata block above (de-static, idiom 139).
   BF44/BF48 defs here = .sdata2 window between @237 (mnNameNew_8023B314) and @325
   (KeySetup); GXColor aggregate copies do not const-fold. */
const GXColor mnNameNew_804DBF44 = { 0xA6, 0x81, 0x3D, 0xFF };
const GXColor mnNameNew_804DBF48 = { 0x00, 0x00, 0x00, 0xFF };

s32 mnNameNew_KeySetup(NameNewEntry* arg0, u8 arg1)
{
    HSD_JObj* key_jobj;
    HSD_JObj* ref1;
    HSD_JObj* ref2;
    HSD_Text* text;
    Vec3 sp50;
    GXColor sp4C;
    GXColor sp48;
    GXColor sp44;
    char** str_table;
    f32 x_range;
    f32 y_range;
    f32 base_x;
    f32 base_y;
    f32 font_x;
    f32 col_x;
    s32 i;
    GXColor* color_ptr;

    FORCE_PAD_STACK(20);

    sp4C = mnNameNew_804DBF44;
    sp48 = mnNameNew_804DBF48;

    switch ((s32) arg1) {
    case 0:
        arg0->mode = 0;
        str_table = mnNameNew_803EDA8C;
        break;
    case 1:
        arg0->mode = 1;
        str_table = mnNameNew_803EDB54;
        break;
    case 2:
        arg0->mode = 2;
        str_table = mnNameNew_803EDC1C;
        break;
    }

    text = arg0->key_text;
    if (text != NULL) {
        HSD_SisLib_803A5CC4(text);
    }
    text = HSD_SisLib_803A6754(0, (s32) mn_804D6BB5);
    arg0->key_text = text;

    key_jobj = HSD_JObjGetChild(arg0->jobjs[16]);
    for (i = 0; i < 50; i++) {
        if (i == 0x2D) {
            break;
        }
        key_jobj = HSD_JObjGetNext(key_jobj);
    }

    lb_8000B1CC(key_jobj, &mnNameNew_803EE324, &sp50);
    text->pos_x = sp50.x;
    text->pos_y = -sp50.y;
    text->pos_z = sp50.z;
    text->font_size.x = 0.03f;
    text->font_size.y = 0.04f;
    text->text_color = mnNameNew_804D4F6C;

    ref1 = HSD_JObjGetChild(arg0->jobjs[16]);
    for (i = 0; i < 50; i++) {
        if (i == 0x28) {
            break;
        }
        ref1 = HSD_JObjGetNext(ref1);
    }

    base_x = HSD_JObjGetTranslationX(key_jobj);
    x_range = HSD_JObjGetTranslationX(ref1) - base_x;

    ref2 = HSD_JObjGetChild(arg0->jobjs[16]);
    for (i = 0; i < 50; i++) {
        if (i == 0x2E) {
            break;
        }
        ref2 = HSD_JObjGetNext(ref2);
    }

    base_y = HSD_JObjGetTranslationY(key_jobj);
    y_range = -(HSD_JObjGetTranslationY(ref2) - base_y);

    {
        GXColor* sp44_addr = &sp44;
        i = 0;
        for (; i < 0x32; i++) {
            font_x = text->font_size.x;
            col_x = (f32) (9 - (i / 5)) * x_range;
            HSD_SisLib_803A6B98(text, col_x / font_x,
                                ((f32) (i % 5) * y_range) / text->font_size.y,
                                str_table[i], font_x, col_x);
            if (i == (s32) mn_804A04F0.hovered_selection) {
                color_ptr = &sp48;
            } else {
                color_ptr = &sp4C;
            }
            sp44 = *color_ptr;
            HSD_SisLib_803A74F0(text, i, sp44_addr);
        }
    }

    return (s32) text;
}

s32 mnNameNew_8023BAA8(NameNewEntry* arg0, s32 arg1, u8 arg2)
{
    u8 mode;

    if (arg2 >= 0x32U && arg2 < 0x3AU) {
        if (arg1 & 1) {
            if (arg2 == 0x39) {
                return ((arg0->last_key_sel / 5) * 5) + 4;
            }
            if (arg2 > 0x32U) {
                return arg2 - 1;
            }
            return 0x38;
        }
        if (arg1 & 2) {
            if (arg2 == 0x39) {
                return (arg0->last_key_sel / 5) * 5;
            }
            if (arg2 == 0x38) {
                return 0x32;
            }
            return arg2 + 1;
        }
        if (arg1 & 4) {
            if (arg2 != 0x38 && arg2 != 0x39) {
                return arg0->last_key_sel % 5;
            }
        } else if ((arg1 & 8) && arg2 != 0x38 && arg2 != 0x39) {
            return (arg0->last_key_sel % 5) + 0x2D;
        }
    } else {
        if (arg1 & 1) {
            if ((arg2 % 5) != 0) {
                return arg2 - 1;
            }
            return 0x39;
        }
        if (arg1 & 2) {
            if ((s32) (arg2 % 5) == 4) {
                return 0x39;
            }
            return arg2 + 1;
        }
        if (arg1 & 4) {
            if ((s32) (arg2 / 5) < 9) {
                return arg2 + 5;
            }
            mode = arg0->mode;
            switch ((s32) mode) {
            case 0:
                return 0x33;
            case 1:
                return 0x34;
            case 2:
                return 0x35;
            }
        } else if (arg1 & 8) {
            if ((arg2 / 5) != 0) {
                return arg2 - 5;
            }
            mode = arg0->mode;
            switch ((s32) mode) {
            case 0:
                return 0x33;
            case 1:
                return 0x34;
            case 2:
                return 0x35;
            }
        }
    }
    return (s32) arg2;
}

s32 PickAutoName(HSD_GObj* arg0)
{
    NameNewEntry* data;
    u8* cur_text;
    u8* text;
    u8** names;
    u8** name_ptr;
    s32 count;
    s32 pick;
    s32 dup;
    s32 char_idx;
    s32 name_idx;
    u8 ch;
    u8 tmp;
    s8 null_ch;

    PAD_STACK(48);

    data = arg0->user_data;
    cur_text = (u8*) mnNameNew_CurrentNameText;

    do {
        dup = 0;
        do {
            null_ch = (s8) mnNameNew_NullCharacter[0];
            count = 0;
            names = AutoNamesList;
            while (null_ch != (s8) * *names) {
                names++;
                count++;
            }
            pick = HSD_Randi(count);
        } while (IsNameUnique((char*) AutoNamesList[pick]) != 0);

        if (pick == (s32) data->auto_history[0]) {
            dup = 1;
        } else if (pick == (s32) data->auto_history[1]) {
            dup = 1;
        } else if (pick == (s32) data->auto_history[2]) {
            dup = 1;
        } else if (pick == (s32) data->auto_history[3]) {
            dup = 1;
        } else if (pick == (s32) data->auto_history[4]) {
            dup = 1;
        }
    } while (dup != 0);

    name_idx = 0;
    char_idx = name_idx;
    cur_text[0] = mnNameNew_NullCharacter[0];
    text = cur_text;
    cur_text[3] = mnNameNew_NullCharacter[0];
    cur_text[6] = mnNameNew_NullCharacter[0];
    cur_text[9] = mnNameNew_NullCharacter[0];

    name_ptr = &AutoNamesList[pick];
    while ((s8) mnNameNew_NullCharacter[0] != (s8) (ch = (*name_ptr)[char_idx])) {
        text[0] = ch;
        text[1] = (*name_ptr)[char_idx + 1];
        text[2] = mnNameNew_NullCharacter[0];
        char_idx += 2;
        name_idx++;
        text += 3;
    }

    cur_text[name_idx * 3] = mnNameNew_NullCharacter[0];

    tmp = data->auto_history[0];
    data->auto_history[0] = (u8) pick;
    ch = data->auto_history[1];
    data->auto_history[1] = tmp;
    tmp = data->auto_history[2];
    data->auto_history[2] = ch;
    ch = data->auto_history[3];
    data->auto_history[3] = tmp;
    data->auto_history[4] = ch;

    return (s32) (s8) mnNameNew_NullCharacter[0];
}

#pragma push
#pragma dont_inline on
bool NameContainsOnlySpaces(void)
{
    char* text = mnNameNew_CurrentNameText;
    char null_char = mnNameNew_NullCharacter[0];
    char space0 = mnNameNew_SpaceCharacter[0];
    char* sp = mnNameNew_SpaceCharacter;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (null_char != text[0]) {
            if (space0 != text[0] || sp[1] != text[1]) {
                return false;
            }
        }
        text += 3;
    }
    return true;
}
#pragma pop

s32 WriteCharactersForNameAtIndex(u8 arg0, s32 arg1)
{
    s32 idx;
    struct NameTagData* nametag;
    u8 ch;
    u8* text;
    u8* ptr;
    s32 ret;

    nametag = GetPersistentNameData((s32) arg0);
    text = (u8*) mnNameNew_CurrentNameText;
    idx = 0;
    if ((s8) mnNameNew_NullCharacter[0] != (s8) *text) {
        ptr = text;
        while ((s8) mnNameNew_NullCharacter[0] != (s8) (ch = *ptr)) {
            nametag->namedata[idx] = (s8) ch;
            idx += 1;
            ptr += 1;
        }
        if ((s8) ch != (s8) * (text += 3)) {
            ptr = text;
            while ((s8) mnNameNew_NullCharacter[0] != (s8) (ch = *ptr)) {
                nametag->namedata[idx] = (s8) ch;
                idx += 1;
                ptr += 1;
            }
            if ((s8) ch != (s8) * (text += 3)) {
                ptr = text;
                while ((s8) mnNameNew_NullCharacter[0] != (s8) (ch = *ptr)) {
                    nametag->namedata[idx] = (s8) ch;
                    idx += 1;
                    ptr += 1;
                }
                if ((s8) ch != (s8) * (text += 3)) {
                    ptr = text;
                    while ((s8) mnNameNew_NullCharacter[0] != (s8) (ch = *ptr)) {
                        nametag->namedata[idx] = (s8) ch;
                        idx += 1;
                        ptr += 1;
                    }
                }
            }
        }
    }
    nametag->namedata[idx] = (s8) mnNameNew_NullCharacter[0];
    ret = GetRumbleSettingOfPort(arg1);
    nametag->x1A1 = ret;
    return ret;
}

inline char** AddCharacterToName_getGlyphs(GlyphRow* arg0, u8 arg1)
{
    return (char**) &arg0[arg1];
}

char* AddCharacterToName(char* arg0, u8 arg1, u8 arg2, u8 arg3)
{
    char ch;
    char* var_r4;
    char** table;
    MnNameNewDataLayout* layout;
    u32 temp;

    layout = (MnNameNewDataLayout*) mnNameNew_803EDA58;
    if ((s32) arg3 != 2) {
        if (((((s32) (temp = arg3)) < ((unsigned short) 2)) & 0xFFFFFFFF) &&
            (((s32) ((unsigned long) arg3)) >= 0))
        {
            char null;
            s32 idx;

            if ((u8) (arg1 - 0x30) <= 1U) {
                if ((arg2 % 2) != 0) {
                    table = AddCharacterToName_getGlyphs(layout->upper_glyphs,
                                                         arg1);
                } else {
                    table = AddCharacterToName_getGlyphs(layout->lower_glyphs,
                                                         arg1);
                }
            } else if ((arg3 == 0 && (arg2 % 2) == 0) ||
                       (arg3 == 1 && (arg2 % 2) != 0))
            {
                table =
                    AddCharacterToName_getGlyphs(layout->lower_glyphs, arg1);
            } else {
                table =
                    AddCharacterToName_getGlyphs(layout->upper_glyphs, arg1);
            }
            var_r4 = arg0;

            for (idx = layout->character_bytes[arg1][1] * 0;
                 (null = (mnNameNew_NullCharacter[0] & 0xFFFF) & 0xFFFF) !=
                 (ch = table[arg2 / 2][idx] & (0xFF & 0xFFu));
                 idx++)
            {
                var_r4[idx] = ch;
            }
            arg0[idx] = null;
        }
        return arg0;
    }
    arg0[0] = layout->character_bytes[arg1][0];
    arg0[1] = layout->character_bytes[arg1][1];
    arg0[2] = layout->character_bytes[arg1][2];
    return arg0;
}

void mnNameNew_GlyphVariantInput(void)
{
    NameNewEntry* data;
    u32 buttons;
    s32 count;
    u16 old_hover;
    u8 old_sel;
    u8 cur_pos;
    GlyphRow* table;
    s32 total;
    s8 null_ch;

    PAD_STACK(16);

    data = ((HSD_GObj*) mnNameNew_804D6C08)->user_data;
    buttons = mn_80229624((u32) mnNameNew_PortInUse);
    mn_804A04F0.buttons = buttons;
    count = 0;
    if (buttons & 0x200) {
        HSD_GObjPLink_80390228(data->variant_gobj);
        data->variant_gobj = NULL;
        AddCharacterToName(&mnNameNew_CurrentNameText[data->cursor_pos * 3],
                           mn_804A04F0.hovered_selection,
                           mn_804A04F0.confirmed_selection, data->mode);
        lbAudioAx_80024030(1);
        cur_pos = data->cursor_pos;
        old_hover = mn_804A04F0.hovered_selection;
        count = (s32) old_hover;
        if (cur_pos < 3U) {
            data->cursor_pos = (u8) (cur_pos + 1);
        } else {
            mn_804A04F0.hovered_selection = 0x39;
        }
        mnNameNew_8023CE4C();
        if (((count != 0x30) && (count != 0x31)) &&
            ((mn_804A04F0.confirmed_selection % 2) != 0))
        {
            if (data->mode == 0) {
                data->mode = 1;
            } else {
                data->mode = 0;
            }
            mnNameNew_KeySetup(data, data->mode);
            mnNameNew_8023B0F8((HSD_GObj*) mnNameNew_804D6C08,
                               (u8) mn_804A04F0.hovered_selection);
            mnNameNew_8023B314(data, (s32) mn_804A04F0.hovered_selection);
        }
    } else {
        if (buttons & 0x20) {
            HSD_GObjPLink_80390228(data->variant_gobj);
            data->variant_gobj = NULL;
            mnNameNew_8023CE4C();
            return;
        }
        null_ch = (s8) mnNameNew_NullCharacter[0];
        table = &mnNameNew_803EDCE4[data->x1 * 4];
        while (null_ch != (s8) * **table) {
            table++;
            count++;
        }
        total = (0, count * 2);
        old_sel = mn_804A04F0.confirmed_selection;
        if (buttons & 1) {
            if ((mn_804A04F0.confirmed_selection % 2) != 0) {
                mn_804A04F0.confirmed_selection -= 1;
            } else {
                mn_804A04F0.confirmed_selection += 1;
            }
        } else if (buttons & 2) {
            if ((mn_804A04F0.confirmed_selection % 2) != 0) {
                mn_804A04F0.confirmed_selection -= 1;
            } else {
                mn_804A04F0.confirmed_selection += 1;
            }
        } else if (buttons & 4) {
            if ((mn_804A04F0.confirmed_selection / 2) == 0) {
                mn_804A04F0.confirmed_selection =
                    (mn_804A04F0.confirmed_selection % 2) +
                    ((total / 2 - 1) * 2);
            } else {
                mn_804A04F0.confirmed_selection -= 2;
            }
        } else if (buttons & 8) {
            if ((mn_804A04F0.confirmed_selection / 2) == (total / 2 - 1)) {
                mn_804A04F0.confirmed_selection =
                    mn_804A04F0.confirmed_selection % 2;
            } else {
                mn_804A04F0.confirmed_selection += 2;
            }
        }
        if (old_sel != (u8) mn_804A04F0.confirmed_selection) {
            lbAudioAx_80024030(2);
        }
    }
}

void mnNameNew_MainInput(HSD_GObj* arg0)
{
    u8 sp24[16];
    char* name_text;
    NameNewEntry* data;
    MnNameNewDataLayout* layout;
    u32 buttons;
    s32 var_r29;
    s32 key_off;
    u8* key_char;
    s8 null_char;
    u8* src;
    u8* dest;
    s32 len;
    u8 cursor;
    s32 n;

    PAD_STACK(12);

    name_text = mnNameNew_CurrentNameText;
    data = ((HSD_GObj*) mnNameNew_804D6C08)->user_data;
    layout = (MnNameNewDataLayout*) mnNameNew_803EDA58;

    if (data->variant_gobj != NULL) {
        mnNameNew_GlyphVariantInput();
        return;
    }

    buttons = mn_80229624((u32) mnNameNew_PortInUse);
    ((s32*) &mn_804A04F0.buttons)[1] = buttons;
    ((s32*) &mn_804A04F0.buttons)[0] = 0;
    var_r29 = 0;

    if (buttons & 0x200) {
        u16 sel = mn_804A04F0.hovered_selection;
        if (sel < 0x32U) {
            if (data->mode != 2 && sel < 0x32U) {
                key_off = (((u8) sel) << 4) & 0xFF0;
                key_char = *(u8**) ((u8*) layout->lower_glyphs + key_off);
                if ((s8) mnNameNew_SpaceCharacter[0] == (s8) key_char[0] &&
                    (s8) mnNameNew_SpaceCharacter[1] == (s8) key_char[1])
                {
                    n = 1;
                } else {
                    n = 0;
                }
                if (n == 0) {
                    lbAudioAx_80024030(1);
                    mn_804A04F0.confirmed_selection = 0;
                    n = 0;
                    {
                        u16 sel2 = mn_804A04F0.hovered_selection;
                        u8** ptrs = (u8**) ((u8*) layout->lower_glyphs +
                                            ((((u8) sel2) << 4) & 0xFF0));
                        null_char = (s8) mnNameNew_NullCharacter[0];
                        while (null_char != (s8) *ptrs[0]) {
                            ptrs++;
                            n++;
                        }
                    }
                    data->variant_gobj =
                        (HSD_GObj*) mnNameNew_GlyphVariantSetup(
                            data, ((n * 2) & 0xFE),
                            mn_804A04F0.hovered_selection);
                    return;
                }
                cursor = data->cursor_pos;
                name_text[cursor * 3] = (char) mnNameNew_SpaceCharacter[0];
                name_text[cursor * 3 + 1] = (char) mnNameNew_SpaceCharacter[1];
                name_text[cursor * 3 + 2] = (char) mnNameNew_NullCharacter[0];
                lbAudioAx_80024030(1);
                if (data->cursor_pos < 3) {
                    data->cursor_pos = (u8) (data->cursor_pos + 1);
                } else {
                    mn_804A04F0.hovered_selection = 0x39;
                }
                mnNameNew_8023CE4C();
                return;
            }
            AddCharacterToName(&name_text[data->cursor_pos * 3],
                               mn_804A04F0.hovered_selection, 0U, data->mode);
            lbAudioAx_80024030(1);
            if (data->cursor_pos < 3) {
                data->cursor_pos = (u8) (data->cursor_pos + 1);
            } else {
                mn_804A04F0.hovered_selection = 0x39;
            }
            mnNameNew_8023CE4C();
            return;
        } else {
            switch (sel) {
            case 0x32:
                lbAudioAx_80024030(0);
                mnNameNew_8023B224(0U);
                return;

            case 0x33:
                if (data->mode != 0) {
                    lbAudioAx_80024030(1);
                    data->mode = 0;
                    mnNameNew_KeySetup(data, 0);
                    mnNameNew_8023B0F8((HSD_GObj*) mnNameNew_804D6C08,
                                       (u8) mn_804A04F0.hovered_selection);
                    return;
                }
                break;

            case 0x34:
                if (data->mode != 1) {
                    lbAudioAx_80024030(1);
                    data->mode = 1;
                    mnNameNew_KeySetup(data, 1);
                    mnNameNew_8023B0F8((HSD_GObj*) mnNameNew_804D6C08,
                                       (u8) mn_804A04F0.hovered_selection);
                    return;
                }
                break;

            case 0x35:
                if (data->mode != 2) {
                    lbAudioAx_80024030(1);
                    data->mode = 2;
                    mnNameNew_KeySetup(data, 2);
                    mnNameNew_8023B0F8((HSD_GObj*) mnNameNew_804D6C08,
                                       (u8) mn_804A04F0.hovered_selection);
                    return;
                }
                break;

            case 0x36:
                lbAudioAx_80024030(0);
                cursor = data->cursor_pos;
                {
                    char* slot = &name_text[cursor * 3];
                    if ((s8) mnNameNew_NullCharacter[0] != (s8) slot[0]) {
                        var_r29 = 1;
                    }
                    if (var_r29 != 0) {
                        slot[0] = (char) mnNameNew_NullCharacter[0];
                        mnNameNew_8023CE4C();
                        return;
                    }
                }
                if (cursor != 0) {
                    name_text[(u8) (cursor - 1) * 3] =
                        (char) mnNameNew_NullCharacter[0];
                    data->cursor_pos = (u8) (data->cursor_pos - 1);
                    mnNameNew_8023CE4C();
                    return;
                }
                lbAudioAx_80024030(3);
                return;

            case 0x37:
                lbAudioAx_80024030(1);
                PickAutoName((HSD_GObj*) mnNameNew_804D6C08);
                null_char = (s8) mnNameNew_NullCharacter[0];
                {
                    char* p = name_text;
                    if (null_char != (s8) *p) {
                        var_r29 = 1;
                        p += 3;
                        if (null_char != (s8) *p) {
                            var_r29 = 2;
                            p += 3;
                            if (null_char != (s8) *p) {
                                var_r29 = 3;
                                p += 3;
                                if (null_char != (s8) *p) {
                                    var_r29 = 4;
                                }
                            }
                        }
                    }
                }
                if (var_r29 == 4) {
                    data->cursor_pos = 3;
                } else {
                    data->cursor_pos = (u8) var_r29;
                }
                mnNameNew_8023CE4C();
                return;

            case 0x38:
            case 0x39:
                dest = sp24;
                len = 0;

                for (src = (u8*) name_text;
                     (s8) mnNameNew_NullCharacter[0] != (s8) *src;
                     src++, dest++, len++)
                {
                    *dest = *src;
                }

                for (src = (u8*) name_text + 3;
                     (s8) mnNameNew_NullCharacter[0] != (s8) *src;
                     src++, dest++, len++)
                {
                    *dest = *src;
                }

                for (src = (u8*) name_text + 6;
                     (s8) mnNameNew_NullCharacter[0] != (s8) *src;
                     src++, dest++, len++)
                {
                    *dest = *src;
                }

                for (src = (u8*) name_text + 9;
                     (s8) mnNameNew_NullCharacter[0] != (s8) *src;
                     src++, dest++, len++)
                {
                    *dest = *src;
                }

                null_char = (s8) mnNameNew_NullCharacter[0];
                sp24[len] = (u8) null_char;

                if ((s8) name_text[0] == null_char) {
                    n = 1;
                } else {
                    n = 0;
                }
                if (n == 0 && NameContainsOnlySpaces() == 0 &&
                    IsNameUnique((char*) sp24) == 0 &&
                    IsNameNotAllowed((char*) sp24) == 0)
                {
                    n = 1;
                } else {
                    n = 0;
                }
                if (n != 0) {
                    lbAudioAx_80024030(1);
                    CreateNameAtIndex((s32) data->name_index);
                    WriteCharactersForNameAtIndex(data->name_index,
                                                  (s32) mn_802295AC());
                    mnNameNew_8023B224(1U);
                    return;
                }
                lbAudioAx_80024030(3);
                return;
            }
        }
    } else {
        if (buttons & 0x100) {
            if ((u16) mn_804A04F0.hovered_selection == 0x38 ||
                (u16) mn_804A04F0.hovered_selection == 0x39)
            {
                dest = sp24;
                len = 0;

                for (src = (u8*) name_text;
                     (s8) mnNameNew_NullCharacter[0] != (s8) *src;
                     src++, dest++, len++)
                {
                    *dest = *src;
                }

                for (src = (u8*) name_text + 3;
                     (s8) mnNameNew_NullCharacter[0] != (s8) *src;
                     src++, dest++, len++)
                {
                    *dest = *src;
                }

                for (src = (u8*) name_text + 6;
                     (s8) mnNameNew_NullCharacter[0] != (s8) *src;
                     src++, dest++, len++)
                {
                    *dest = *src;
                }

                for (src = (u8*) name_text + 9;
                     (s8) mnNameNew_NullCharacter[0] != (s8) *src;
                     src++, dest++, len++)
                {
                    *dest = *src;
                }

                null_char = (s8) mnNameNew_NullCharacter[0];
                sp24[len] = (u8) null_char;

                if ((s8) name_text[0] == null_char) {
                    n = 1;
                } else {
                    n = 0;
                }
                if (n == 0 && NameContainsOnlySpaces() == 0 &&
                    IsNameUnique((char*) sp24) == 0 &&
                    IsNameNotAllowed((char*) sp24) == 0)
                {
                    n = 1;
                } else {
                    n = 0;
                }
                if (n != 0) {
                    lbAudioAx_80024030(1);
                    CreateNameAtIndex((s32) data->name_index);
                    WriteCharactersForNameAtIndex(data->name_index,
                                                  (s32) mn_802295AC());
                    mnNameNew_8023B224(1U);
                    return;
                }
                lbAudioAx_80024030(3);
                return;
            }
            mn_804A04F0.hovered_selection = 0x39;
            return;
        }
        if (buttons & 0xC0) {
            lbAudioAx_80024030(1);
            if (buttons & 0x40) {
                if (data->mode != 0) {
                    data->mode = (u8) (data->mode - 1);
                } else {
                    data->mode = 2;
                }
            } else {
                if (data->mode < 2) {
                    data->mode = (u8) (data->mode + 1);
                } else {
                    data->mode = 0;
                }
            }
            mnNameNew_KeySetup(data, data->mode);
            mnNameNew_8023B0F8((HSD_GObj*) mnNameNew_804D6C08,
                               (u8) mn_804A04F0.hovered_selection);
            mnNameNew_8023B314(data, (s32) mn_804A04F0.hovered_selection);
            return;
        }
        if (buttons & 0x20) {
            lbAudioAx_80024030(0);
            null_char = (s8) mnNameNew_NullCharacter[0];
            if ((s8) name_text[0] == null_char) {
                var_r29 = 1;
            }
            if (var_r29 != 0) {
                mnNameNew_8023B224(0U);
                return;
            }
            cursor = data->cursor_pos;
            if (null_char != (s8) name_text[cursor * 3]) {
                n = 1;
            } else {
                n = 0;
            }
            if (n != 0) {
                name_text[cursor * 3] = (char) mnNameNew_NullCharacter[0];
                mnNameNew_8023CE4C();
                return;
            }
            if (cursor != 0) {
                name_text[(u8) (cursor - 1) * 3] =
                    (char) mnNameNew_NullCharacter[0];
                data->cursor_pos = (u8) (data->cursor_pos - 1);
                mnNameNew_8023CE4C();
                return;
            }
            lbAudioAx_80024030(3);
            return;
        }
        {
            u8 new_sel = mnNameNew_8023BAA8(
                data, (s32) buttons, (u8) mn_804A04F0.hovered_selection);
            if ((s32) new_sel != (s32) mn_804A04F0.hovered_selection) {
                lbAudioAx_80024030(2);
                mn_804A04F0.hovered_selection = (u16) new_sel;
                if (new_sel < 0x32) {
                    data->last_key_sel = new_sel;
                }
            }
        }
    }
}

char mnNameNew_803EE35C[] = "Can't get user_data.\n";
char mnNameNew_803EE374[] = "mnnamenew.c";
char mnNameNew_803EE380[] = "user_data";
char mnNameNew_803EE38C[] = "MenMainBack_Top_joint";
char mnNameNew_803EE3A4[] = "MenMainBack_Top_animjoint";
char mnNameNew_803EE3C0[] = "MenMainBack_Top_matanim_joint";
char mnNameNew_803EE3E0[] = "MenMainBack_Top_shapeanim_joint";
char mnNameNew_803EE400[] = "ScMenMain_cam_int1_camera";
char mnNameNew_803EE41C[] = "ScMenMain_scene_lights";
char mnNameNew_803EE434[] = "ScMenMain_fog";
char mnNameNew_803EE444[] = "MenMainPanel_Top_joint";
char mnNameNew_803EE45C[] = "MenMainPanel_Top_animjoint";
char mnNameNew_803EE478[] = "MenMainPanel_Top_matanim_joint";
char mnNameNew_803EE498[] = "MenMainPanel_Top_shapeanim_joint";
char mnNameNew_803EE4BC[] = "MenMainConEtNw_Top_joint";
char mnNameNew_803EE4D8[] = "MenMainConEtNw_Top_animjoint";
char mnNameNew_803EE4F8[] = "MenMainConEtNw_Top_matanim_joint";
char mnNameNew_803EE51C[] = "MenMainConEtNw_Top_shapeanim_joint";
char mnNameNew_803EE540[] = "MenMainBaseEtNw_Top_joint";
char mnNameNew_803EE55C[] = "MenMainBaseEtNw_Top_animjoint";
char mnNameNew_803EE57C[] = "MenMainBaseEtNw_Top_matanim_joint";
char mnNameNew_803EE5A0[] = "MenMainBaseEtNw_Top_shapeanim_joint";
char mnNameNew_803EE5C4[] = "MenMainSubEtNw_Top_joint";
char mnNameNew_803EE5E0[] = "MenMainSubEtNw_Top_animjoint";
char mnNameNew_803EE600[] = "MenMainSubEtNw_Top_matanim_joint";
char mnNameNew_803EE624[] = "MenMainSubEtNw_Top_shapeanim_joint";
char mnNameNew_803EE648[] = "MenMainSbaseEtNw_Top_joint";
char mnNameNew_803EE664[] = "MenMainSbaseEtNw_Top_animjoint";
char mnNameNew_803EE684[] = "MenMainSbaseEtNw_Top_matanim_joint";
char mnNameNew_803EE6A8[] = "MenMainSbaseEtNw_Top_shapeanim_joint";
char mnNameNew_803EE6D0[] = "mnNameAutoNameUs";
char mnNameNew_803EE6E4[] = "mnNameRefuseNameUs";
char mnNameNew_803EE6F8[] = "mnNameAutoName";
char mnNameNew_803EE708[] = "mnNameRefuseName";

void mnNameNew_8023CE4C(void)
{
    GXColor sp20;
    Vec3 sp24;
    NameNewEntry* data;
    HSD_JObj* jobj_a;
    HSD_JObj* jobj_b;
    HSD_Text* old_text;
    float y_minus;
    f32 char_spacing;
    f32 first_x;
    u8* name_ptr;
    GXColor* sp20_ptr;
    s32 i;
    HSD_Text* text;

    data = ((HSD_GObj*) mnNameNew_804D6C08)->user_data;
    jobj_a = data->jobjs[14];
    jobj_b = data->jobjs[15];
    first_x = HSD_JObjGetTranslationX(jobj_a);
    char_spacing = HSD_JObjGetTranslationX(jobj_b) - first_x;
    if (data->name_disp_text != 0L) {
        HSD_SisLib_803A5CC4(data->name_disp_text);
    }
    text = HSD_SisLib_803A6754(0, mn_804D6BB5);
    lb_8000B1CC(jobj_a, &mnNameNew_803EE330, &sp24);
    name_ptr = (u8*) &mnNameNew_CurrentNameText;
    y_minus = -sp24.y;
    text->pos_x = sp24.x;
    sp20_ptr = &sp20;
    text->pos_y = y_minus;
    i = 0;
    text->pos_z = sp24.z;
    text->pos_z = sp24.z;
    text->font_size.x = 0.04f;
    text->font_size.y = 0.05f;
    text->text_color = mnNameNew_804D4F6C;
    for (; i < 4; i++) {
        if ((s8) mnNameNew_NullCharacter[0] == (s8) *name_ptr) {
            break;
        }
        HSD_SisLib_803A6B98(text, (char_spacing * (f32) i) / text->font_size.x,
                            0.0f, (const char*) name_ptr);
        sp20 = mnNameNew_804D4F78;
        HSD_SisLib_803A74F0(text, i, sp20_ptr);
        name_ptr += 3;
    }
    data->name_disp_text = text;
}

void fn_8023CFC8(HSD_GObj* arg0)
{
    HSD_JObj* jobj;
    HSD_JObj* root;
    u8 new_var;
    u8 old_sel;
    float one;
    s32 count;
    GlyphVariantEntry* data;
    u32 pad1;
    u32 pad2;
    GXColor sp30;
    GXColor sp2C;

    PAD_STACK(0x20);

    data = arg0->user_data;
    new_var = data->selection;
    old_sel = new_var;
    if (old_sel != (u8) mn_804A04F0.confirmed_selection) {
        root = data->jobjs[4];
        if (root == NULL) {
            jobj = NULL;
        } else {
            jobj = root->child;
        }
        for (count = old_sel; count > 0; count--) {
            if (jobj == NULL) {
                jobj = NULL;
            } else {
                jobj = jobj->next;
            }
        }
        root = jobj;
        HSD_JObjReqAnimAll(root, 0.0f);
        HSD_JObjAnimAll(root);
        sp30 = mnNameNew_804D4F74;
        HSD_SisLib_803A74F0(data->text, (s32) data->selection, &sp30);
        root = data->jobjs[4];
        new_var = mn_804A04F0.confirmed_selection;
        if (root == NULL) {
            jobj = NULL;
        } else {
            jobj = root->child;
        }
        for (count = (s32) new_var; count > 0; count--) {
            if (jobj == NULL) {
                jobj = NULL;
            } else {
                jobj = jobj->next;
            }
        }
        one = 1.0f;
        HSD_JObjReqAnimAll(jobj, one);
        HSD_JObjAnimAll(jobj);
        sp2C = mnNameNew_804D4F70;
        HSD_SisLib_803A74F0(data->text, (s32) mn_804A04F0.confirmed_selection,
                            &sp2C);
        data->selection = (u8) mn_804A04F0.confirmed_selection;
    }
}

void fn_8023D0F8(void* arg0)
{
    GlyphVariantEntry* entry = arg0;
    HSD_SisLib_803A5CC4(entry->text);
    HSD_Free(arg0);
}

s32 mnNameNew_8023D130(GlyphVariantEntry* arg0, u8 arg1, u8 arg2, s32 arg3)
{
    u8 _padA[8];
    Vec3 sp30;
    GXColor sp2C;
    HSD_JObj* jobj14;
    HSD_JObj* jobj18;
    HSD_JObj* jobj1C;
    HSD_Text* text;
    s32 i;
    char* str;
    GXColor* color_ptr;
    f32 x_range;
    f32 y_range;
    f32 font_x;
    f32 col_x;
    s32 temp;
    GlyphRow* table_upper;
    GlyphRow* table_lower;
    GXColor* sp2C_ptr;

    jobj14 = arg0->jobjs[4];
    text = HSD_SisLib_803A6754(0, (s32) mn_804D6BB4);
    jobj18 = arg0->jobjs[5];
    jobj1C = arg0->jobjs[6];
    lb_8000B1CC(jobj14, &mnNameNew_803EE324, &sp30);
    text->pos_x = sp30.x;
    text->pos_y = -sp30.y;
    text->pos_z = sp30.z;
    text->font_size.x = 0.03f;
    text->font_size.y = 0.04f;
    text->text_color = mnNameNew_804D4F6C;
    x_range =
        HSD_JObjGetTranslationX(jobj18) - HSD_JObjGetTranslationX(jobj14);
    y_range =
        -(HSD_JObjGetTranslationY(jobj1C) - HSD_JObjGetTranslationY(jobj14));
    temp = (arg3 * 4) & 0x3FC;
    table_upper = &mnNameNew_803EE004[temp];
    table_lower = &mnNameNew_803EDCE4[temp];
    sp2C_ptr = &sp2C;
    for (i = 0; i < (s32) arg1; i++) {
        if ((u8) (arg3 - 0x30) <= 1U) {
            if ((i % 2) != 0) {
                str = (char*) table_upper[i / 2];
            } else {
                str = (char*) table_lower[i / 2];
            }
        } else if ((arg2 == 0 && (i % 2) == 0) || (arg2 == 1 && (i % 2) != 0))
        {
            str = (char*) table_lower[i / 2];
        } else {
            str = (char*) table_upper[i / 2];
        }
        font_x = text->font_size.x;
        col_x = (f32) (i / 2) * x_range;
        HSD_SisLib_803A6B98(text, col_x / font_x,
                            ((f32) (i % 2) * y_range) / text->font_size.y, str,
                            font_x, col_x);
        if (i == (s32) mn_804A04F0.confirmed_selection) {
            color_ptr = &mnNameNew_804D4F70;
        } else {
            color_ptr = &mnNameNew_804D4F74;
        }
        sp2C = *color_ptr;
        HSD_SisLib_803A74F0(text, i, sp2C_ptr);
    }
    arg0->text = text;
    return (s32) text;
}

extern const Vec3 mnNameNew_803B8528;

s32 mnNameNew_GlyphVariantSetup(NameNewEntry* arg0, u16 arg1, u8 arg2)
{
    HSD_GObj* gobj;
    HSD_JObj* jobj;
    HSD_JObj* variant;
    HSD_JObj* key_jobj;
    HSD_JObj* ref_jobj;
    HSD_JObj* ref2;
    HSD_JObj* ref3;
    GlyphVariantEntry* user_data;
    Vec3 sp38;
    Vec3 sp2C;
    f32 dx;
    f32 dy;
    f32 base_x;
    f32 base_y;
    s32 i;

    gobj = GObj_Create(6U, 7U, 0x80U);
    jobj = HSD_JObjLoadJoint(mnNameNew_804A0710[0]);
    HSD_GObjObject_80390A70(gobj, HSD_GObj_804D7849, jobj);
    GObj_SetupGXLink(gobj, HSD_GObj_JObjCallback, 6U, 0x80U);
    HSD_GObj_SetupProc(gobj, fn_8023CFC8, 0U);
    HSD_JObjAddAnimAll(jobj, mnNameNew_804A0710[1], mnNameNew_804A0710[2],
                       mnNameNew_804A0710[3]);
    HSD_JObjReqAnimAll(jobj, (f32) arg1 * 0.5f);
    HSD_JObjAnimAll(jobj);

    user_data = HSD_MemAlloc(sizeof(GlyphVariantEntry));
    if (user_data == NULL) {
        HSD_ASSERTREPORT(0x5B4, user_data, "Can't get userdata\n");
    }
    GObj_InitUserData(gobj, 0U, fn_8023D0F8, user_data);

    user_data->selection = mn_804A04F0.confirmed_selection;
    i = 0;
    for (; i < 7; i++) {
        lb_80011E24(jobj, &user_data->jobjs[i], i, -1);
    }

    sp2C = mnNameNew_803B8528;

    if (arg2 >= 0x32U && arg2 < 0x3AU) {
        key_jobj = (arg0->jobjs[mnNameNew_803EDA7C[arg2 - 0x32]]);
    } else {
        key_jobj = HSD_JObjGetChild(arg0->jobjs[16]);
        for (i = 0; i < 50; i++) {
            if (i == (s32) arg2) {
                break;
            }
            key_jobj = HSD_JObjGetNext(key_jobj);
        }
    }

    lb_8000B1CC(key_jobj, &sp2C, &sp38);
    HSD_JObjSetTranslate(jobj, &sp38);

    ref_jobj = user_data->jobjs[4];
    ref2 = user_data->jobjs[5];
    ref3 = user_data->jobjs[6];

    base_x = HSD_JObjGetTranslationX(ref_jobj);
    dx = HSD_JObjGetTranslationX(ref2) - base_x;
    base_y = HSD_JObjGetTranslationY(ref_jobj);
    dy = HSD_JObjGetTranslationY(ref3) - base_y;

    i = 0;
    for (; i < (s32) arg1; i++) {
        variant = HSD_JObjLoadJoint(mnNameNew_804A0720[0]);
        HSD_JObjAddAnimAll(variant, mnNameNew_804A0720[1],
                           mnNameNew_804A0720[2], mnNameNew_804A0720[3]);
        HSD_JObjReqAnimAll(variant, (f32) (i == user_data->selection));
        HSD_JObjAnimAll(variant);
        HSD_JObjSetTranslateX(variant, dx * (f32) (i / 2));
        HSD_JObjSetTranslateY(variant, dy * (f32) (i % 2));
        HSD_JObjAddChild(ref_jobj, variant);
    }

    mnNameNew_8023D130(user_data, arg1, arg0->mode, arg2);
    return (s32) gobj;
}

s32 mnNameNew_8023DA08(NameNewEntry* arg0)
{
    s32 var_r29 = 1;

    if (mn_8022ED6C(arg0->jobjs[12], &mnNameNew_803EDA58[2]) <
        mnNameNew_803EDA58[2].end_frame)
    {
        var_r29 = 0;
    }
    if (mn_8022ED6C(arg0->jobjs[13], &mnNameNew_803EDA58[2]) <
        mnNameNew_803EDA58[2].end_frame)
    {
        var_r29 = 0;
    }
    if (mn_8022EFD8(arg0->jobjs[4], mnNameNew_803EDA58) <
        mnNameNew_803EDA58->end_frame)
    {
        var_r29 = 0;
    }
    if (mn_8022EFD8(arg0->jobjs[2], mnNameNew_803EDA58) <
        mnNameNew_803EDA58->end_frame)
    {
        var_r29 = 0;
    }
    if (mn_8022EFD8(arg0->jobjs[6], mnNameNew_803EDA58) <
        mnNameNew_803EDA58->end_frame)
    {
        var_r29 = 0;
    }
    return var_r29;
}

void fn_8023DAEC(HSD_GObj* arg0)
{
    HSD_Text* text;
    s32 var_r30;
    NameNewEntry* data;

    PAD_STACK(8);

    data = arg0->user_data;
    text = data->key_text;
    if (text != NULL) {
        HSD_SisLib_803A5CC4(text);
        data->key_text = NULL;
    }
    text = data->name_disp_text;
    if (text != NULL) {
        HSD_SisLib_803A5CC4(text);
        data->name_disp_text = NULL;
    }
    text = data->desc_text;
    if (text != NULL) {
        HSD_SisLib_803A5CC4(text);
        data->desc_text = NULL;
    }
    var_r30 = 1;
    if (mn_8022EFD8(data->jobjs[4], &mnNameNew_803EDA58[1]) <
        mnNameNew_803EDA58[1].end_frame)
    {
        var_r30 = 0;
    }
    if (mn_8022EFD8(data->jobjs[2], &mnNameNew_803EDA58[1]) <
        mnNameNew_803EDA58[1].end_frame)
    {
        var_r30 = 0;
    }
    if (mn_8022EFD8(data->jobjs[6], &mnNameNew_803EDA58[1]) <
        mnNameNew_803EDA58[1].end_frame)
    {
        var_r30 = 0;
    }
    if (var_r30 != 0 || (u8) mn_804A04F0.x10 == 1) {
        HSD_GObjPLink_80390228(arg0);
    }
}

/* datarecon-w27: 4C10/4F68 defs moved to the .sdata block at top */

#pragma push
#pragma dont_inline on
void fn_8023DBE8(HSD_GObj* arg0)
{
    NameNewEntry* data;
    HSD_JObj* jobj;
    HSD_JObj* parent;
    HSD_GObjProc* proc;
    GXColor sp2C;
    GXColor sp28;
    u8 cursor;
    u8 sel;
    s32 i;

    FORCE_PAD_STACK(32);

    data = arg0->user_data;

    if ((u8) mn_804A04F0.x10 != 1) {
        HSD_JObjSetFlagsAll(data->jobjs[16], 0x10U);
        HSD_JObjSetFlagsAll(data->jobjs[12], 0x10U);
        HSD_JObjSetFlagsAll(data->jobjs[13], 0x10U);
        HSD_GObjProc_8038FE24(HSD_GObj_804D7838);
        proc = HSD_GObj_SetupProc(arg0, fn_8023DAEC, 0U);
        proc->flags_3 = HSD_GObj_804D783C;
        return;
    }

    mnNameNew_8023DA08(data);
    cursor = data->x1;

    if ((s32) cursor != (s32) mn_804A04F0.hovered_selection) {
        if (cursor >= 0x32U && cursor < 0x3AU) {
            jobj = data->jobjs[mnNameNew_803EDA7C[cursor - 0x32]];
        } else {
            parent = data->jobjs[16];
            if (parent == NULL) {
                jobj = NULL;
            } else {
                jobj = parent->child;
            }
            for (i = 0; i < 50; i++) {
                if (i == (s32) cursor) {
                    break;
                }
                if (jobj == NULL) {
                    jobj = NULL;
                } else {
                    jobj = jobj->next;
                }
            }
        }
        HSD_JObjReqAnimAll(jobj, 0.0f);
        HSD_JObjAnimAll(jobj);
        if ((u8) data->x1 < 0x32U) {
            sp2C = mnNameNew_804D4F6C;
            HSD_SisLib_803A74F0(data->key_text, (s32) data->x1, &sp2C);
        }

        sel = (u8) mn_804A04F0.hovered_selection;
        if (sel >= 0x32U && sel < 0x3AU) {
            jobj = data->jobjs[mnNameNew_803EDA7C[sel - 0x32]];
        } else {
            parent = data->jobjs[16];
            if (parent == NULL) {
                jobj = NULL;
            } else {
                jobj = parent->child;
            }
            for (i = 0; i < 50; i++) {
                if (i == (s32) sel) {
                    break;
                }
                if (jobj == NULL) {
                    jobj = NULL;
                } else {
                    jobj = jobj->next;
                }
            }
        }
        HSD_JObjReqAnimAll(jobj, 1.0f);
        HSD_JObjAnimAll(jobj);
        if ((u16) mn_804A04F0.hovered_selection < 0x32U) {
            sp28 = mnNameNew_804D4F68;
            HSD_SisLib_803A74F0(data->key_text,
                                (s32) mn_804A04F0.hovered_selection, &sp28);
        }
        mnNameNew_8023B314(data, (s32) mn_804A04F0.hovered_selection);
        data->x1 = (u8) mn_804A04F0.hovered_selection;
        mnNameNew_8023B0F8(arg0, data->x1);
    }

    cursor = data->cursor_pos;
    if ((mnNameNew_804D4C10 + (f32) cursor) != mn_8022F298(data->jobjs[13])) {
        jobj = ((NameNewEntry*) arg0->user_data)->jobjs[13];
        HSD_JObjReqAnimAll(jobj, mnNameNew_804D4C10 + (f32) cursor);
        HSD_JObjAnimAll(jobj);
    }
}
#pragma pop

void mnNameNew_8023E0D8(NameNewEntry* arg0)
{
    MnNameNewDataLayout* layout;
    AnimLoopSettings* anim;
    HSD_JObj* jobj;
    u16* jobj_ids;
    s32 i;

    layout = (MnNameNewDataLayout*) mnNameNew_803EDA58;
    anim = layout->anim;
    jobj = arg0->jobjs[12];
    HSD_JObjReqAnim(jobj, anim[2].start_frame);
    HSD_JObjAnim(jobj);

    jobj = arg0->jobjs[13];
    HSD_JObjReqAnim(jobj, anim[2].start_frame);
    HSD_JObjAnim(jobj);

    jobj = arg0->jobjs[4];
    HSD_JObjReqAnim(jobj, anim[0].start_frame);
    HSD_JObjAnim(jobj);

    jobj = arg0->jobjs[5];
    HSD_JObjReqAnimAll(jobj, (f32) (arg0->x1 == 0x38));
    HSD_JObjAnimAll(jobj);

    jobj = arg0->jobjs[2];
    HSD_JObjReqAnim(jobj, anim[0].start_frame);
    HSD_JObjAnim(jobj);

    jobj = arg0->jobjs[5];
    HSD_JObjReqAnimAll(jobj, (f32) (arg0->x1 == 0x32));
    HSD_JObjAnimAll(jobj);

    jobj = arg0->jobjs[6];
    HSD_JObjReqAnim(jobj, anim[0].start_frame);
    HSD_JObjAnim(jobj);

    jobj_ids = layout->key_jobj_ids;
    for (i = 0x32; i < 0x3A; i++) {
        jobj = arg0->jobjs[jobj_ids[i - 0x32]];
        HSD_JObjReqAnimAll(jobj, (f32) (arg0->x1 == i));
        HSD_JObjAnimAll(jobj);
    }
}

s32 InitNameEntryUIState(NameNewEntry* arg0, s32 arg1)
{
    s32 result;
    s8 count;
    u8** names;
    s8 null_char;

    PAD_STACK(0x20);

    arg0->x1 = (count = (u8) mn_804A04F0.hovered_selection);
    result = lbLang_IsSavedLanguageUS();
    if (result) {
        arg0->mode = 2;
    } else {
        arg0->mode = 0;
    }
    count = 0;
    arg0->cursor_pos = 0;
    arg0->last_key_sel = arg0->x1;
    arg0->variant_gobj = 0;
    arg0->name_index = arg1;
    arg0->key_text = 0;
    arg0->name_disp_text = 0;
    arg0->desc_text = 0;
    null_char = (s8) mnNameNew_NullCharacter[0];
    names = AutoNamesList;
    while (null_char != (s8) * *names) {
        names++;
        count++;
    }
    arg0->auto_history[0] = count;
    arg0->auto_history[1] = count;
    arg0->auto_history[2] = count;
    arg0->auto_history[3] = count;
    arg0->auto_history[4] = count;
    return result;
}

void mnNameNew_8023E32C(s32 arg0)
{
    HSD_GObj* gobj;
    HSD_JObj* root_jobj;
    HSD_JObj* key_jobj;
    NameNewEntry* user_data;
    s32 i;
    s32 k;
    f32 x_range;
    f32 y_range;

    PAD_STACK(8);

    gobj = GObj_Create(6U, 7U, 0x80U);
    mnNameNew_804D6C08 = gobj;
    root_jobj = HSD_JObjLoadJoint(mnNameNew_804A06F0[0]);
    HSD_GObjObject_80390A70(gobj, HSD_GObj_804D7849, root_jobj);
    GObj_SetupGXLink(gobj, HSD_GObj_JObjCallback, 4U, 0x80U);
    HSD_GObj_SetupProc(gobj, (HSD_GObjEvent) fn_8023DBE8, 0U);
    HSD_JObjAddAnimAll(root_jobj, mnNameNew_804A06F0[1], mnNameNew_804A06F0[2],
                       mnNameNew_804A06F0[3]);
    HSD_JObjReqAnimAll(root_jobj, 0.0f);
    HSD_JObjAnimAll(root_jobj);
    user_data = HSD_MemAlloc(0x6C);
    if (user_data == NULL) {
        HSD_ASSERTREPORT(0x717U, user_data, "Can't get userdata\n");
    }
    GObj_InitUserData(gobj, 0U, HSD_Free, user_data);
    InitNameEntryUIState(user_data, arg0);
    for (i = 0; i < 0x13; i++) {
        lb_80011E24(root_jobj, &user_data->jobjs[i], i, -1);
    }
    mnNameNew_8023E0D8(user_data);
    for (k = 0; k < 0x32; k++) {
        key_jobj = HSD_JObjLoadJoint(mnNameNew_804A0700[0]);
        HSD_JObjAddAnimAll(key_jobj, mnNameNew_804A0700[1],
                           mnNameNew_804A0700[2], mnNameNew_804A0700[3]);
        HSD_JObjReqAnimAll(key_jobj, (f32) (user_data->x1 == (u8) k));
        HSD_JObjAnimAll(key_jobj);
        x_range = HSD_JObjGetTranslationX(user_data->jobjs[17]) -
                  HSD_JObjGetTranslationX(user_data->jobjs[16]);
        y_range = HSD_JObjGetTranslationY(user_data->jobjs[18]) -
                  HSD_JObjGetTranslationY(user_data->jobjs[16]);
        mnName_80239F5C(key_jobj, x_range * (f32) (k / 5));
        mnName_80239EBC(key_jobj, y_range * (f32) (k % 5));
        HSD_JObjAddChild(user_data->jobjs[16], key_jobj);
    }
    user_data->key_text =
        (HSD_Text*) mnNameNew_KeySetup(user_data, user_data->mode);
    mnNameNew_8023B314(user_data, (s32) user_data->x1);
    mnNameNew_8023B0F8(gobj, user_data->x1);
}

void mnNameNew_EnterFromMnName(UNK_T arg0)
{
    char unused[8];
    char* text = mnNameNew_CurrentNameText;

    mn_804A04F0.x10 = 1;
    if (lbLang_IsSavedLanguageUS()) {
        mn_804A04F0.hovered_selection = 0x2d;
    } else {
        mn_804A04F0.hovered_selection = 0;
    }
    mnNameNew_PortInUse = 4;
    text[0] = mnNameNew_NullCharacter[0];
    text[3] = mnNameNew_NullCharacter[0];
    text[6] = mnNameNew_NullCharacter[0];
    text[9] = mnNameNew_NullCharacter[0];
    mnNameNew_8023E32C((s32) arg0);
}

void mnNameNew_EnterFromMnCharSel(HSD_Archive* arg0, s32 arg1)
{
    u8 name_count;
    HSD_GObjProc* proc;
    char* text;
    s32 is_us;

    PAD_STACK(16);

    text = mnNameNew_CurrentNameText;

    mn_804D6BC8.cooldown = 0x14;
    mn_804D6BC8.x2 = 0;
    mn_804D6BC8.x4 = 0;
    mn_804A04F0.buttons = 0;
    mn_804A04F0.prev_menu = mn_804A04F0.cur_menu;
    mn_804A04F0.cur_menu = 0x12;

    if (lbLang_IsSavedLanguageUS()) {
        mn_804A04F0.hovered_selection = 0x2D;
    } else {
        mn_804A04F0.hovered_selection = 0;
    }

    mn_804A04F0.x10 = 1;
    HSD_SisLib_803A5E70();
    mnNameNew_PortInUse = arg1;

    lbArchive_LoadSections(
        arg0,

        // Background
        (void**) &MenMainBack_Top.joint, mnNameNew_803EE38C,
        &MenMainBack_Top.animjoint, mnNameNew_803EE3A4,
        &MenMainBack_Top.matanim_joint, mnNameNew_803EE3C0,
        &MenMainBack_Top.shapeanim_joint, mnNameNew_803EE3E0,

        // Scene
        &MenMain_cam, mnNameNew_803EE400, &MenMain_lights,
        mnNameNew_803EE41C, &MenMain_fog, mnNameNew_803EE434,

        // Panel
        &MenMainPanel_Top.joint, mnNameNew_803EE444,
        &MenMainPanel_Top.animjoint, mnNameNew_803EE45C,
        &MenMainPanel_Top.matanim_joint, mnNameNew_803EE478,
        &MenMainPanel_Top.shapeanim_joint, mnNameNew_803EE498,

        // Row 1
        &mnNameNew_804A06F0[0], mnNameNew_803EE4BC,
        &mnNameNew_804A06F0[1], mnNameNew_803EE4D8,
        &mnNameNew_804A06F0[2], mnNameNew_803EE4F8,
        &mnNameNew_804A06F0[3], mnNameNew_803EE51C,

        // Row 2
        &mnNameNew_804A0700[0], mnNameNew_803EE540,
        &mnNameNew_804A0700[1], mnNameNew_803EE55C,
        &mnNameNew_804A0700[2], mnNameNew_803EE57C,
        &mnNameNew_804A0700[3], mnNameNew_803EE5A0,

        // Row 3
        &mnNameNew_804A0710[0], mnNameNew_803EE5C4,
        &mnNameNew_804A0710[1], mnNameNew_803EE5E0,
        &mnNameNew_804A0710[2], mnNameNew_803EE600,
        &mnNameNew_804A0710[3], mnNameNew_803EE624,

        // Row 4
        &mnNameNew_804A0720[0], mnNameNew_803EE648,
        &mnNameNew_804A0720[1], mnNameNew_803EE664,
        &mnNameNew_804A0720[2], mnNameNew_803EE684,
        &mnNameNew_804A0720[3], mnNameNew_803EE6A8,

        NULL);

    is_us = lbLang_IsSavedLanguageUS();

    if (is_us) {
        lbArchive_LoadSections(arg0, (void**) &AutoNamesList,
                               mnNameNew_803EE6D0, &NotAllowedNamesList,
                               mnNameNew_803EE6E4, NULL);
    } else {
        lbArchive_LoadSections(arg0, (void**) &AutoNamesList, mnNameNew_803EE6F8,
                               &NotAllowedNamesList, mnNameNew_803EE708, NULL);
    }

    mn_8022C304();
    mn_8022BCF8();
    mn_8022BEDC(mn_8022BE34());
    mn_80229B2C();
    mn_80229DC0();

    name_count = (u8) GetNameCount();

    proc = HSD_GObj_SetupProc(GObj_Create(0U, 1U, 0x80U),
                              (HSD_GObjEvent) mnNameNew_MainInput, 0U);
    proc->flags_3 = HSD_GObj_804D783C;

    text[0] = mnNameNew_NullCharacter[0];
    text[3] = mnNameNew_NullCharacter[0];
    text[6] = mnNameNew_NullCharacter[0];
    text[9] = mnNameNew_NullCharacter[0];

    mnNameNew_8023E32C((s32) name_count);
    lbAudioAx_80023F28(gmMainLib_8015ECB0());
}

void mnNameNew_8023EA08(UNK_T arg0)
{
    mnNameNew_EnterFromMnCharSel((HSD_Archive*) arg0, 4);
}

/* datarecon-w27: .sdata tail 0x388 (after KeySetup pools @327/@328) */
u8 mnNameNew_804D4F98[8] = { 0 };

/* === datarecon-w27: .rodata (EOF def, after the line-1325 use) === */
const Vec3 mnNameNew_803B8528 = { -0.5f, 0.7f, 0.0f };

/* datarecon-w27 PARK NOTES: mnNameNew_803EE720/724 (8B zero .data tail; braced-zero
   array law would emit .sdata, C-unreachable from this TU -- ALIGNFIX-class residual).
   Ours-extra .rodata @241/@1010 (52B local-aggregate init images) = S1-structural. */
