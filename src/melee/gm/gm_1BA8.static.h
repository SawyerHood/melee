#include <melee/gm/types.h>
#include <melee/mn/types.h>

/// Defined in gm_1BA8.c (binary-proven .bss layout; gm_804979D8 is a
/// double-buffered MatchExitInfo pair: 0x4510 == 2 * 0x2288).
extern CSSData gm_80497758;
extern StartMeleeData gm_804978A0;
extern MatchExitInfo gm_804979D8[2];

struct gm_804D6900_t {
    int x0;
    struct gm_804D6900_x4_t {
        int x0;
        intptr_t x4;
    }* x4;
    StartMeleeRules* x8;
    struct {
        u8 unk0[0x16];
        u8 x16;
    }* xC;
    void* x10;
    s8* x14;
    s8* x18;
};

/// Defined in gm_1BA8.c (target exports it; was wrongly static).
extern struct gm_804D6900_t** gm_804D6900;
