#include "math.h"

#include <platform.h>

#define EXP_MASK 0x7F800000
#define EXP_ZERO 0x3F800000 // hex representation of (float)0.0f
#define MANT_MASK 0x007FFFFF
#define SIGN_BIT 0x80000000
#define F32_BIT_CAST(raw_x) (*(float*) &raw_x)

typedef union {
    float f;
    u32 u;
} FloatU32;
#define F32_UNBIASED_EXPONENT(raw_x)                                          \
    (((raw_x & 0xFF800000) >> 23) - /*exponent bias*/ 127)
#define F32_HIGH_MANTISSA_BITS(raw_x)                                         \
    ((raw_x >> 16) & 0x7F) // 7 most significant mantissa bits
#define F32_LOW_MANTISSA_BITS(raw_x)                                          \
    (raw_x & 0xFFFF)   // remaining 16 mantissa bits
#define LN2 0.6931472f // natural logarithm of 2

/// logf Taylor series coefficients and IEEE specials, stored as u32 bit
/// patterns: the exact bytes are not reproducible from float-typed
/// initializers (MWCC folds 0.0/0.0 to 0xFFC00000, not 0x7FFFFFFF).
/// Original names of the two coefficients are unknown (symbols.txt
/// placeholders); __float_nan/__float_huge are the real MSL names.
const u32 MSL_Math_804DE1B0 = 0xBF000030; // -0.500003F (logf C0)
const u32 MSL_Math_804DE1B4 = 0x3EAAAA36; // 0.333329856F (logf C1)
const u32 __float_nan = 0x7FFFFFFF;
const u32 __float_huge = 0x7F800000;

/// Math lookup tables - defined in math_data.c
/// Keeping them in a separate compilation unit prevents the compiler from
/// hoisting array base addresses, which is needed to match the original code.
extern const float __ln_F[];
extern const float __one_over_F[];
extern const float __sincos_on_quadrant[];
extern const float __sincos_poly[];

/*
Computes the natural logarithm of x.

Algorithm:
- Decompose `x = 2^E * 1.M`, where `E` is the exponent, `M` are the mantissa
bits and `1.M` denotes the binary number with integer part 1 and fractional
bits `M`
- Observe that `ln(x) = ln(2)*E + ln(1.M)`. It remains to compute `ln(1.M)`.
- Choose a 7 bit mantissa `m` such that `1.m` best approximates `1.M`. Details:
  - Let `m` be the 7 most significant bits of `M`, then `1.m` already is a good
approximation. But when the 8'th most significant bit of `M` is set, we
increment `m` accordingly by 1 bit to get an equal or better approximation.
- Next approximate `ln(1.M) = ln(y + h)` with `y=1.m` and `h=1.M - 1.m` using a
Taylor expansion at `y`. Maths detour:
    - We have `ln(y + h) = ln(y) + h/y - (h/y)^2 / 2 + (h/y)^3 / 3 - ...`
    - So for `H := h/y` we have `ln(y + h) = ln(y) + H + H^2 * (-0.5 + H/3 -
...)`
    - I don't understand how this works, but the algorithm uses this
approximation formula: `ln(1.M) = ln(y + h) = ln(y) + H + H^2 * (H*0.333329856
- 0.500003)` (coefficient bit patterns 0x3EAAAA36 / 0xBF000030, byte-proven
from the target .sdata2)
- In the formula above, the algorithm computes `ln(y) = ln(1.m)` using a lookup
table, then computes `H = h * 1/y = h * 1/1.m` using another lookup table for
`1/1.m`.
*/
/// Frame layout is load-bearing (target frame 0x28; byte-matched):
/// - The switch head must be the INLINE expression (a named exp_bits local
///   plus the case-block raw_x cast-init would charge 12B of hidden frame
///   slots; with the inline head the case-local front-end-CSEs to a 4B
///   copy-class home at 0xc).
/// - Decl order [raw_fm, raw_fM, coef] maps top-down to fm@0x1c, fM@0x18,
///   coef@0x10-0x17 (homed scalars and aggregates share one lexical
///   top-down band below the 8B int->f64 conversion slot at 0x20).
/// - coef must stay a u32 ARRAY: array stores carry may-alias-with-global
///   scheduling edges that pin the target's store/load order at function
///   top; the address-taken raw_fm/raw_fM SCALAR pair keeps the int webs
///   at r6/r5/r7/r8 (deleting it shifts them all down one).
/// - H is the only named float (unhomed here); the other coefficients are
///   read inline. The (float) cast blocks fmadds contraction of
///   H + (H*H)*poly at zero frame cost.
float logf(float x)
{
    switch (*(u32*) &x & EXP_MASK) {
    default: /* x is normal */
    {
        u32 raw_fm;
        u32 raw_fM;
        u32 coef[2];
        u32 mant_bits;
        u32 raw_x;
        s32 E;
        s32 m;

        coef[0] = *(u32*) &MSL_Math_804DE1B0;
        raw_x = *(u32*) &x;
        coef[1] = *(u32*) &MSL_Math_804DE1B4;
        mant_bits = raw_x & MANT_MASK;
        E = F32_UNBIASED_EXPONENT(raw_x);
        m = F32_HIGH_MANTISSA_BITS(raw_x);

        if (F32_LOW_MANTISSA_BITS(raw_x)) {
            float H;

            raw_fm = (raw_x & 0x7F0000) | EXP_ZERO;
            raw_fM = mant_bits | EXP_ZERO;

            if (raw_x & 0x8000) {
                raw_fm += 0x10000;
                m += 1;
            }

            H = F32_BIT_CAST(raw_fM) - F32_BIT_CAST(raw_fm);
            H = H * __one_over_F[m];

            return (LN2 * E + __ln_F[m]) +
                   (H + (float) ((H * H) * (H * F32_BIT_CAST(coef[1]) +
                                            F32_BIT_CAST(coef[0]))));
        }
        return LN2 * E + __ln_F[m];
    }
    case EXP_MASK: {
        u32 raw_x = *(u32*) &x;
        if (raw_x & MANT_MASK) {
            return x;
        } else {
            return (raw_x & SIGN_BIT) ? F32_BIT_CAST(__float_nan)
                                      : F32_BIT_CAST(__float_huge);
        }
    }
    case 0:
        return -F32_BIT_CAST(__float_huge);
    }
}
/*
matching `__one_over_F[m]` (currently line 72 in the C code above)

target code:
r8 = array index
80: lis     r3,lbl_803B9154@ha // load array base pointer 0x803C0000-28848
8c: rlwinm  r4,r8,2,0,0x1d   // multiply index by 4 = sizeof(float)
90: addi    r0,r3,lbl_803B9154@l
98: add     r3,r0,r4         // r3 = base_pointer + 4*index
a4: lfs     f1,0(r3)         // deref the pointer

a8: lis     r3, 0x803C       // load array base pointer 0x803C0000-28848
c0: subi	r0, r3, 28848
cc: add     r3,r0,r4         // r3 = base_pointer + 4*index
e0: lfs     f1,0(r3)         // deref the pointer

my code:
r8 = array index
4:  lis     r3,...data.0@ha    // load array base pointer
10: addi    r5,r3,...data.0@l
80: rlwinm  r4,r8,2,0,0x1d     //  multiply index by 4 = sizeof(float)
90: add     r3,r5,r4           // r3 = base_pointer + 4*index
a0: lfs     f3, 0x204(r3)      // load __one_over_F[m]
ac: lfsx    f0,r5,r4           // load float32 from address r5+r4, = __ln_F[m]
*/
