#include "c_decimal.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

d2x init_2x() {
    d2x initial;
    for (int i = 0; i < 7; i++) {
        initial.bits[i] = 0;
    }
    initial.value_type = DEC_NORMAL_VALUE;
    return initial;
}

c_decimal init() {
    c_decimal initial_decimal;
    for (int i = 0; i < 4; i++) {
        initial_decimal.bits[i] = 0;
    }
    initial_decimal.value_type = DEC_NORMAL_VALUE;
    return initial_decimal;
}

int d2x_is_greater(d2x a, d2x b) {
    int res = FALSE;
    normalise_scale_2x(&a, &b);
    int non_equal_pos = -1;
    if (a.value_type == DEC_NAN || b.value_type == DEC_NAN) {
        res = FALSE;
    } else if (a.value_type == DEC_INFINITY && b.value_type != DEC_INFINITY) {
        res = TRUE;
    } else if (a.value_type == DEC_NEGATIVE_INFINITY) {
        res = FALSE;
    } else if (a.value_type == DEC_NORMAL_VALUE && b.value_type == DEC_INFINITY) {
        res = FALSE;
    } else if (a.value_type == DEC_NORMAL_VALUE && b.value_type == DEC_NEGATIVE_INFINITY) {
        res = TRUE;
    } else {
        // check for sign
        if (get_sign_2x(&a) == 1 && get_sign_2x(&b) == 1) {
            for (int i = LAST_2DX_BIT - 1; i >= 0 && non_equal_pos == -1; i--) {
                if (!!get_bit_2x(&a, i) != !!get_bit_2x(&b, i)) {
                    non_equal_pos = i;
                }
            }
            if (non_equal_pos != -1) {
                if (get_bit_2x(&b, non_equal_pos)) {
                    res = TRUE;
                }
            }
        } else if (get_sign_2x(&a) == 1 && get_sign_2x(&b) == 0) {
            res = FALSE;
        } else if (get_sign_2x(&a) == 0 && get_sign_2x(&b) == 1) {
            res = TRUE;
        } else {
            // Need find greater bit
            for (int i = LAST_2DX_BIT - 1; i >= 0 && non_equal_pos == -1; i--) {
                if (!!get_bit_2x(&a, i) != !!get_bit_2x(&b, i)) {
                    non_equal_pos = i;
                    res = get_bit_2x(&a, i);
                }
            }
            if (get_bit_2x(&a, non_equal_pos) && non_equal_pos != -1) {
                res = TRUE;
            }
        }
    }
    return res;
}

int is_less_specials(int a, int b, int *res) {
    int pass = 0;
    if (a == DEC_INFINITY) {
        *res = FALSE;
    } else if (a == DEC_NEGATIVE_INFINITY) {
        if (b != DEC_NEGATIVE_INFINITY) {
            *res = TRUE;
        } else {
            *res = FALSE;
        }
    } else if (b == DEC_NEGATIVE_INFINITY) {
        *res = FALSE;
    } else if (b == DEC_INFINITY) {
        *res = TRUE;
    } else {
        pass = 1;
    }
    return pass;
}

int d2x_is_less(d2x a, d2x b) {  // a < b
    int res = FALSE;
    normalise_scale_2x(&a, &b);

    if (a.value_type != DEC_NAN && b.value_type != DEC_NAN) {
        if (is_less_specials(a.value_type, b.value_type, &res) && d2x_is_equal(a, b) == FALSE) {
            // check for sign
            if (get_sign_2x(&a) == 1 && get_sign_2x(&b) == 0) {
                res = TRUE;
            } else if (get_sign_2x(&a) == 0 && get_sign_2x(&b) == 1) {
                res = FALSE;
            } else {
                if (is_zero_2x(&a)) {
                    set_sign_2x(&a, 0);
                }
                if (is_zero_2x(&b)) {
                    set_sign_2x(&b, 0);
                }
                int non_equal_pos = -1;
                // Find first != bit
                for (int i = LAST_2DX_BIT - 1; i >= 0 && non_equal_pos == -1; i--) {
                    if (!!get_bit_2x(&a, i) != !!get_bit_2x(&b, i)) {
                        non_equal_pos = i;
                    }
                }
                if (non_equal_pos != -1) {
                    if (get_bit_2x(&b, non_equal_pos)) {
                        res = TRUE;
                    }
                }
                if (get_sign_2x(&a)) {
                    if (res == TRUE) {
                        res = FALSE;
                    } else {
                        res = TRUE;
                    }
                }
            }
        }
    }
    return res;
}

int d2x_is_equal(d2x a, d2x b) {
    int res = FALSE;
    if (a.value_type != b.value_type) {
        res = FALSE;
    } else if (a.value_type == DEC_NAN && b.value_type == DEC_NAN) {
        res = FALSE;
    } else if (a.value_type == DEC_NEGATIVE_INFINITY && b.value_type == DEC_NEGATIVE_INFINITY) {
        res = TRUE;
    } else if (a.value_type == DEC_INFINITY && b.value_type == DEC_INFINITY) {
        res = TRUE;
    } else if (a.value_type == DEC_NORMAL_VALUE && b.value_type == DEC_NORMAL_VALUE) {
        normalise_scale_2x(&a, &b);
        int is_eq = 1;
        for (int i = LAST_2DX_BIT - 1; i >= 0 && is_eq == 1; i--) {
            if (!!get_bit_2x(&a, i) != !!get_bit_2x(&b, i)) {
                is_eq = 0;
            }
        }
        if (is_eq == 1 && get_sign_2x(&a) == get_sign_2x(&b)) {
            if (a.value_type == DEC_NORMAL_VALUE && b.value_type == DEC_NORMAL_VALUE) {
                res = TRUE;
            }
        }
        if (is_eq == 1) {
            if (get_sign_2x(&a) != get_sign_2x(&b)) {
                res = FALSE;
            }
        }
    }
    if (a.value_type != b.value_type) {
        res = FALSE;
    }
    return res;
}

int get_bit(c_decimal d, int i) {
    unsigned int mask = 1u << (i % 32);
    return d.bits[i/32] & mask;
}

int get_bit_2x(const d2x *d, int i) {
    unsigned int mask = 1u << (i % 32);
    return d -> bits[i/32] & mask;
}

void clear_bit(c_decimal *d, int i) {
    unsigned int mask = ~(1u << (i % 32));  // Inverted mask
    d -> bits[i/32] &= mask;
}

void clear_bit_2x(d2x *d, int i) {
    unsigned int mask = ~(1u << (i % 32));  // Inverted mask
    d -> bits[i/32] &= mask;
}

void set_bit(c_decimal *d, int i) {
    unsigned int mask = 1u << (i % 32);
    d -> bits[i/32] |= mask;
}

void set_bit_2x(d2x *d, int i) {
    unsigned int mask = 1u << (i % 32);
    d -> bits[i/32] |= mask;
}

void set_sign(c_decimal *d, int i) {
    if (i == 1) {
        unsigned int mask = 1u << 31;
        d->bits[SCALE] |= mask;
    } else if (i == 0) {
        unsigned int mask = ~(1u << 31);
        d->bits[SCALE] &= mask;
    }
}

void set_sign_2x(d2x *d, int i) {
    if (i == 1) {
        unsigned int mask = 1u << 31;
        d->bits[SCALE_2X] |= mask;
    } else if (i == 0) {
        unsigned int mask = ~(1u << 31);
        d->bits[SCALE_2X] &= mask;
    }
}

// Get sign stored in the last bit of bits[3]
int get_sign(c_decimal d) {
    unsigned int mask = 1u << 31;
    return !!(d.bits[SCALE] & mask);
}

int get_sign_2x(const d2x *d) {
    unsigned int mask = 1u << 31;
    return !!(d -> bits[SCALE_2X] & mask);
}

// Sets scale within bits[3]
int set_scale(c_decimal *d, int scale) {
    int code = SUCCESS;
    if (scale > SCALE_MAX) {
        code = CALCULATION_ERROR;
    } else {
        clear_scale(d);
        int c_bit = LAST_SCALE_BIT + LAST_DEC_BIT;  // LAST_DEC_BIT is used to get into bits[SCALE]
        for (unsigned int scale_mask = 0x80; scale_mask > 0;
            scale_mask = scale_mask >> 1) {
            if (!!(scale & scale_mask)) {
                set_bit(d, c_bit);
            }
            c_bit--;
        }
    }

    return code;
}

int set_scale_2x(d2x *d, int scale) {
    int code = SUCCESS;
    if (scale > 0xFF) {
        code = CALCULATION_ERROR;
    } else {
        clear_scale_2x(d);
        int c_bit = LAST_SCALE_BIT + LAST_2DX_BIT;  // LAST_2DX_BIT is used to get into bits[SCALE_2X]
        for (unsigned int scale_mask = 0x80; scale_mask > 0;
            scale_mask = scale_mask >> 1) {
            if (!!(scale & scale_mask)) {
                set_bit_2x(d, c_bit);
            }
            c_bit--;
        }
    }

    return code;
}

int get_scale(c_decimal d) {
    int out = 0;
    int out_mask = 1;
    int scale_mask = 0x10000;

    for (int i = 0; i < 8; i++) {
        if (!!(scale_mask & d.bits[SCALE])) {
            out |= out_mask;
        }
        out_mask = out_mask << 1;
        scale_mask = scale_mask << 1;
    }

    return out;
}

int get_scale_2x(const d2x *d) {
    int out = 0;
    int out_mask = 1;
    int scale_mask = 0x10000;

    for (int i = 0; i < 8; i++) {
        if (!!(scale_mask & d -> bits[SCALE_2X])) {
            out |= out_mask;
        }
        out_mask = out_mask << 1;
        scale_mask = scale_mask << 1;
    }

    return out;
}

void clear_scale(c_decimal *d) {
    int sign = 0;
    if (get_sign(*d)) {
        sign = 1;
    }
    d->bits[SCALE] = 0;
    if (sign) {
        set_sign(d, 1);
    }
}

void clear_scale_2x(d2x *d) {
    int sign = 0;
    if (get_sign_2x(d)) {
        sign = 1;
    }
    d->bits[SCALE_2X] = 0;
    if (sign) {
        set_sign_2x(d, 1);
    }
}

int from_int_to_decimal(int src, c_decimal *dst) {
    int code = SUCCESS;
    if (dst == NULL) {
        code = CALCULATION_ERROR;
    } else {
        *dst = init();
        if (src < 0) {
            set_sign(dst, 1);
            src *= -1;
        }
        dst->bits[LOW] = src;
    }

    return code;
}


int count_digits(double in) {
    int out = 0;
    while (in >= 1) {
        out++;
        in /= 10;
    }
    return out;
}

int float_specials(float src, c_decimal *dst, int *code) {
    int pass = 0;
    if (dst != NULL) {
        *code = SUCCESS;
        if (src == INFINITY) {
            dst -> value_type = DEC_INFINITY;
        } else if (src == -INFINITY) {
            dst -> value_type = DEC_NEGATIVE_INFINITY;
        } else if (src != src) {
            dst -> value_type = DEC_NAN;
        } else if (fabs(src) == FLOAT_MAX) {
            for (int i = 0; i <= HIGH; i++) {
                dst -> bits[i] = -1;
            }
            if (src < 0) {
                set_sign(dst, 1);
            }
        } else if (fabs(src) > FLOAT_MAX) {
            *code = CALCULATION_ERROR;
            if (src > 0) {
                dst -> value_type = DEC_INFINITY;
            } else {
                dst -> value_type = DEC_NEGATIVE_INFINITY;
            }
        } else if (fabs(src) > FLOAT_MIN) {
            pass = 1;
        } else if (src != 0) {
            *code = CALCULATION_ERROR;
        }
    }
    return pass;
}

int from_float_to_decimal(float src, c_decimal *dst) {
    int code = CALCULATION_ERROR;
    if (dst != NULL) {
        *dst = init();
        if (float_specials(src, dst, &code)) {
            int digits = 0;
            int sign = 0;  // [0 == +] [1 == -]
            double tmp = src;
            int scale = 0;
            if (tmp < 0) {
                sign = 1;
                tmp *= -1;
            }
            // src *= 10 until we find first != 0 element
            if (fabs(src) < 1) {
                digits = 1;
                for (; !(int)tmp && scale < SCALE_MAX; tmp *= 10, scale++) {}
            } else {
                digits = count_digits(tmp);
            }
            if (fmod(tmp, 1) != 0) {
                for (int i = digits; i < 6 && scale < SCALE_MAX; i++) {
                    tmp *= 10;
                    scale++;
                }
            }
            tmp = trunc(tmp);
            int c_bit = 0;
            int bit_overflow = 0;
            while (tmp > 1 && !bit_overflow) {
                if (c_bit > LAST_DEC_BIT - 1) {
                    bit_overflow = 1;
                } else {
                    if (fmod(tmp, 2) != 0) {
                        set_bit(dst, c_bit);
                    }
                    tmp /= 2;
                    tmp = trunc(tmp);
                    c_bit++;
                }
            }
            if (tmp == 1) {
                if (c_bit < LAST_2DX_BIT && !bit_overflow) {
                    set_bit(dst, c_bit);
                } else {
                    bit_overflow = 1;
                }
            }
            if (!bit_overflow) {
                code = SUCCESS;
                d2x tmp_2x = decimal_to_2x(*dst);
                set_scale_2x(&tmp_2x, scale);
                if (scale > SCALE_MAX) {
                    decrease_scale_to_target_2x(&tmp_2x, SCALE_MAX);
                }
                *dst = fit_2x_into_decimal(tmp_2x);  // This removes trailing zeroes
                if (sign == 1) {
                    set_sign(dst, 1);
                }
            } else {
                code = CALCULATION_ERROR;
                *dst = init();
                if (sign) {
                    dst -> value_type = DEC_NEGATIVE_INFINITY;
                } else {
                    dst -> value_type = DEC_INFINITY;
                }
            }
        }
    }
    return code;
}

// Performs << on decimal
int dec_shift_2x(d2x *d) {
    int code = SUCCESS;
    if (!!(get_bit_2x(d, LAST_2DX_BIT - 1))) {
        code = CALCULATION_ERROR;
    } else {
        // starting point = last_bit - 1
        for (int i = LAST_2DX_BIT - 2; i >= 0; i--) {
            if (!!get_bit_2x(d, i)) {
                set_bit_2x(d, i + 1);
            } else {
                clear_bit_2x(d, i + 1);
            }
        }
        clear_bit_2x(d, 0);
    }

    return code;
}

void dec_shift_right_2x(d2x *d) {
    for (int i = 0; i <= LAST_2DX_BIT - 2; i++) {
        if (!!get_bit_2x(d, i + 1)) {
            set_bit_2x(d, i);
        } else {
            clear_bit_2x(d, i);
        }
    }
    clear_bit_2x(d, LAST_2DX_BIT - 1);
}

d2x multiply_2x(d2x a, d2x b, int *overflow) {
    d2x d = init_2x();
    int mask_overflow = 0;
    for (int i = 0; i < LAST_2DX_BIT; i++) {
        if (!!get_bit_2x(&b, i) && !*overflow) {
            if (!mask_overflow) {
                d = addition_2x(d, a, overflow);
            } else {
                *overflow = 1;
            }
        }
        mask_overflow = dec_shift_2x(&a);  // This will always overflow
    }
    return d;
}

int mul_special(c_decimal a, c_decimal b, c_decimal *out) {
    int pass = 0;
    if (a.value_type == DEC_INFINITY) {
        if (b.value_type == DEC_INFINITY) {
            out -> value_type = DEC_INFINITY;
        } else if (b.value_type == DEC_NEGATIVE_INFINITY) {
            out -> value_type = DEC_NEGATIVE_INFINITY;
        } else if (is_zero(b)) {
            out -> value_type = DEC_NAN;
        } else if (get_sign(b)) {
            out -> value_type = DEC_NEGATIVE_INFINITY;
        } else {
            out -> value_type = DEC_INFINITY;
        }
    } else if (a.value_type == DEC_NEGATIVE_INFINITY) {
        if (b.value_type == DEC_INFINITY) {
            out -> value_type = DEC_NEGATIVE_INFINITY;
        } else if (b.value_type == DEC_NEGATIVE_INFINITY) {
            out -> value_type = DEC_INFINITY;
        } else if (is_zero(b)) {
            out -> value_type = DEC_NAN;
        } else if (get_sign(b)) {
            out -> value_type = DEC_INFINITY;
        } else {
            out -> value_type = DEC_NEGATIVE_INFINITY;
        }
    } else {
        pass = 1;
    }
    return pass;
}

c_decimal dec_mul(c_decimal a, c_decimal b) {
    c_decimal out = init();
    if (a.value_type == DEC_NAN || b.value_type == DEC_NAN) {
        out.value_type = DEC_NAN;
    } else if (mul_special(a, b, &out) && mul_special(b, a, &out)) {
        d2x a2x = decimal_to_2x(a);
        d2x b2x = decimal_to_2x(b);
        int scale = get_scale(a) + get_scale(b);
        int sign = 0;
        int overflow = 0;
        if (get_sign(a) != get_sign(b)) {
            sign = 1;
        }
        d2x tmp = multiply_2x(a2x, b2x, &overflow);
        if (!overflow) {
            set_scale_2x(&tmp, scale);
            if (sign) {
                set_sign_2x(&tmp, 1);
            }
            out = fit_2x_into_decimal(tmp);
        }
    }

    return out;
}

d2x addition_2x(d2x a, d2x b, int *overflow) {
    d2x sum = init_2x();
    int flag_add = 0;
    for (int i = 0; i < LAST_2DX_BIT; i++) {
        int bit_a = !!(get_bit_2x(&a, i));
        int bit_b = !!(get_bit_2x(&b, i));
        if (flag_add) {
            if (bit_a == 0 && bit_b == 0) {
                set_bit_2x(&sum, i);
                flag_add = 0;
            } else if (bit_a == 1 && bit_b == 1) {
                set_bit_2x(&sum, i);
            }
        } else {
            if (bit_a == 1 && bit_b == 1) {
                flag_add = 1;
            } else if (bit_a != bit_b) {
                set_bit_2x(&sum, i);
            }
        }
    }
    if (flag_add && a.value_type != ALLOW_OVERFLOW) {
        *overflow = 1;
        sum = init_2x();
    }
    return sum;
}

void set_decimal_inf(c_decimal *in, int sign) {
    if (sign) {
        in -> value_type = DEC_NEGATIVE_INFINITY;
    } else {
        in -> value_type = DEC_INFINITY;
    }
}

// a & b are value types
int add_specials(int a, int b, c_decimal *out) {
    int pass = 0;
    if (a == DEC_NAN || b == DEC_NAN) {
        out -> value_type = DEC_NAN;
    } else if (a == DEC_INFINITY) {
        if (b == DEC_NEGATIVE_INFINITY) {
            out -> value_type = DEC_NAN;
        } else {
            out -> value_type = DEC_INFINITY;
        }
    } else if (a == DEC_NEGATIVE_INFINITY) {
        if (b == DEC_INFINITY) {
            out -> value_type = DEC_NAN;
        } else {
            out -> value_type = DEC_NEGATIVE_INFINITY;
        }
    } else {
        pass = 1;
    }

    return pass;
}

c_decimal dec_add(c_decimal a, c_decimal b) {
    int overflow = 0;
    c_decimal out = init();
    if (add_specials(a.value_type, b.value_type, &out) && add_specials(b.value_type, a.value_type, &out)) {
        d2x a2x = decimal_to_2x(a);
        d2x b2x = decimal_to_2x(b);
        d2x tmp;
        normalise_scale_2x(&a2x, &b2x);
        if (get_sign_2x(&a2x) == get_sign_2x(&b2x)) {
            // Since -a - b == -(a + b)
            tmp = addition_2x(a2x, b2x, &overflow);
            if (get_sign_2x(&a2x)) {
                set_sign_2x(&tmp, 1);
            }
        } else {
            if (!get_sign_2x(&a2x)) {  // a - b
                tmp = substract_2x(a2x, b2x);
            } else {  // -a + b == b - a;
                tmp = substract_2x(b2x, a2x);
            }
        }
        if (overflow) {
            set_decimal_inf(&out, get_sign_2x(&tmp));
        } else {
            set_scale_2x(&tmp, get_scale_2x(&a2x));
            out = fit_2x_into_decimal(tmp);
        }
    }


    return out;
}

c_decimal invert_decimal(c_decimal in) {
    for (int i = 0; i <= 2; i++) {
        in.bits[i] = ~in.bits[i];
    }
    return in;
}

d2x invert_2x(d2x d) {
    for (int i = 0; i <= 5; i++) {
        d.bits[i] = ~d.bits[i];
    }
    return d;
}

// WARNING: Only use this function after processing signs of a and b
// This function assumes that a and b are both positive
d2x substract_2x(d2x a, d2x b) {
    d2x res;
    set_sign_2x(&a, 0);
    set_sign_2x(&b, 0);
    if (is_zero_2x(&b)) {
        res = a;
    } else if (is_zero_2x(&a)) {
        res = b;
        if (get_sign_2x(&b)) {
            set_sign_2x(&res, 0);
        } else {
            set_sign_2x(&res, 1);
        }
    } else {
        d2x one = init_2x();
        one.bits[0] = 1;
        int overflow = 0;

        int negative_res = 0;
        if (d2x_is_less(a, b) == TRUE) {
            negative_res = 1;
        }
        b = invert_2x(b);
        b = addition_2x(b, one, &overflow);
        b.value_type = ALLOW_OVERFLOW;
        a.value_type = ALLOW_OVERFLOW;
        // This one will always overflow due to invert and is normal for this algorithm
        res = addition_2x(a, b, &overflow);
        if (negative_res) {
            res = invert_2x(res);
            res = addition_2x(res, one, &overflow);
            set_sign_2x(&res, 1);
        }
    }

    return res;
}

c_decimal copy_decimal(c_decimal d) {
    c_decimal res = init();
    for (int i = 0; i <= SCALE; i++) {
        res.bits[i] = d.bits[i];
    }
    res.value_type = d.value_type;
    return res;
}

int sub_specials(c_decimal a, c_decimal b, c_decimal *out) {
    int pass = 0;

    if (a.value_type == DEC_INFINITY) {
        if (b.value_type == DEC_INFINITY) {
            out -> value_type = DEC_NAN;
        } else if (b.value_type == DEC_NEGATIVE_INFINITY) {
            out -> value_type = DEC_INFINITY;
        } else {
            out -> value_type = DEC_INFINITY;
        }
    } else if (a.value_type == DEC_NEGATIVE_INFINITY) {
        if (b.value_type == DEC_INFINITY) {
            out -> value_type = DEC_NEGATIVE_INFINITY;
        } else if (b.value_type == DEC_NEGATIVE_INFINITY) {
            out -> value_type = DEC_NAN;
        } else {
            out -> value_type = DEC_NEGATIVE_INFINITY;
        }
    } else if (b.value_type == DEC_INFINITY) {
        out -> value_type = DEC_NEGATIVE_INFINITY;
    } else if (b.value_type == DEC_NEGATIVE_INFINITY) {
        out -> value_type = DEC_INFINITY;
    } else if (is_zero(b)) {
        *out = copy_decimal(a);
    } else if (is_zero(a)) {
        *out = dec_negate(b);
    } else {
        pass = 1;
    }

    return pass;
}

c_decimal dec_sub(c_decimal a, c_decimal b) {
    c_decimal out = init();
    if (a.value_type != DEC_NAN && b.value_type != DEC_NAN) {
        if (sub_specials(a, b, &out)) {
            int overflow = 0;
            d2x a2x = decimal_to_2x(a);
            d2x b2x = decimal_to_2x(b);
            normalise_scale_2x(&a2x, &b2x);
            int scale = get_scale_2x(&a2x);
            d2x tmp;
            if (!get_sign(a) && get_sign(b)) {  // a - -b
                tmp = addition_2x(a2x, b2x, &overflow);
            } else if (get_sign(a) && get_sign(b)) {  // -a - -b
                tmp = substract_2x(a2x, b2x);
                set_sign_2x(&a2x, 0);
                if (d2x_is_greater(a2x, b2x) == TRUE) {
                    set_sign_2x(&tmp, 1);
                }
            } else if (get_sign(a) && !get_sign(b)) {
                tmp = addition_2x(a2x, b2x, &overflow);
                set_sign_2x(&tmp, 1);
            } else {
                tmp = substract_2x(a2x, b2x);
            }

            if (overflow) {
                set_decimal_inf(&out, get_sign_2x(&tmp));
            } else {
                set_scale_2x(&tmp, scale);
                out = fit_2x_into_decimal(tmp);
            }
        }
    } else {
        out.value_type = DEC_NAN;
    }

    return out;
}

int find_last_bit_2x(const d2x *d) {
    int last = -1;

    for (int i = LAST_2DX_BIT - 1; i >= 0 && last == -1; i--) {
        if (!!get_bit_2x(d, i)) {
            last = i;
        }
    }

    return last;
}

// Increases a scale of d2x to a specified value
d2x increase_scale_2x(d2x d, int target, int *overflow) {
    d2x tmp = d;
    d2x ten = init_2x();
    int sign = get_sign_2x(&d);
    ten.bits[LOW] = 10;
    int c_scale = get_scale_2x(&d);
    if (c_scale < target) {
        for (; c_scale < target && !*overflow; c_scale++) {
            tmp = multiply_2x(tmp, ten, overflow);
            if (!*overflow) {
                d = tmp;
            }
        }
        set_scale_2x(&d, target);
        if (sign) {
            set_sign_2x(&d, 1);
        }
    }

    return d;
}

int normalise_scale_2x(d2x *a, d2x *b) {
    int code = SUCCESS;
    int a_scale = get_scale_2x(a);
    int b_scale = get_scale_2x(b);
    if (a_scale != b_scale) {
        d2x tmp;
        int overflow = 0;
        if (a_scale < b_scale) {
            tmp = increase_scale_2x(*a, b_scale, &overflow);
            if (!overflow) {
                *a = tmp;
            } else {
                code = CALCULATION_ERROR;
            }
        } else {
            tmp = increase_scale_2x(*b, a_scale, &overflow);
            if (!overflow) {
                *b = tmp;
            } else {
                code = CALCULATION_ERROR;
            }
        }
    }
    return code;
}

d2x decimal_to_2x(c_decimal src) {
    d2x dst = init_2x();
    dst.bits[LOW] = src.bits[LOW];
    dst.bits[MID] = src.bits[MID];
    dst.bits[HIGH] = src.bits[HIGH];
    dst.bits[SCALE_2X] = src.bits[SCALE];
    dst.value_type = src.value_type;

    return dst;
}

// This function is supposed to be used only after clearing the 2x bits out of src
c_decimal d2x_to_decimal(const d2x *src) {
    c_decimal dst = init();
    dst.bits[LOW] = src -> bits[LOW];
    dst.bits[MID] = src -> bits[MID];
    dst.bits[HIGH] = src -> bits[HIGH];
    dst.bits[SCALE] = src -> bits[SCALE_2X];
    dst.value_type = src -> value_type;

    return dst;
}

int check_2x_empty(const d2x *in) {
    int flag = 1;
    for (int i = LOW_2X; i <= HIGH_2X && flag; i++) {
        if (in -> bits[i] != 0) {
            flag = 0;
        }
    }
    return flag;
}

int is_zero(c_decimal in) {
    int pass = 1;
    for (int i = 0; i <= HIGH && pass; i++) {
        if (in.bits[i] != 0) {
            pass = 0;
        }
    }
    return pass;
}

int is_zero_2x(const d2x *in) {
    int pass = 1;
    for (int i = 0; i <= HIGH_2X && pass; i++) {
        if (in -> bits[i] != 0) {
            pass = 0;
        }
    }
    return pass;
}

int negate_specials(int type, c_decimal *a) {
    int pass = 0;
    if (type == DEC_NAN) {
        a -> value_type = DEC_NAN;
    } else if (type == DEC_INFINITY) {
        a -> value_type = DEC_NEGATIVE_INFINITY;
    } else if (type == DEC_NEGATIVE_INFINITY) {
        a -> value_type = DEC_INFINITY;
    } else {
        pass = 1;
    }
    return pass;
}

c_decimal dec_negate(c_decimal a) {
    if (negate_specials(a.value_type, &a)) {
        if (get_sign(a)) {
            set_sign(&a, 0);
        } else {
            set_sign(&a, 1);
        }
        a.value_type = DEC_NORMAL_VALUE;
    }
    return a;
}

void write_last_bit_2x(int bit, d2x *d) {
    dec_shift_2x(d);
    if (bit) {
        set_bit_2x(d, 0);
    }
}

void maximize_scale(d2x *d) {
    int scale = get_scale_2x(d);
    d2x tmp;
    d2x ten = init_2x();
    ten.bits[LOW] = 10;
    int overflow = 0;
    while (!overflow) {
        tmp = multiply_2x(*d, ten, &overflow);
        if (!overflow) {
            *d = tmp;
            scale++;
            set_scale_2x(d, scale);
        }
    }
}

// Algorithm used: (Page 12)
// https://www.cs.utah.edu/~rajeev/cs3810/slides/3810-08.pdf
d2x div_2x(d2x a, d2x b, d2x *res, int mode) {
    set_sign_2x(&a, 0);
    set_sign_2x(&b, 0);
    *res = init_2x();
    if (!is_zero_2x(&a)) {
        int initial_scale = 0;
        if (mode == NORM_DIV || mode == INT_DIV) {
            normalise_scale_2x(&a, &b);
        }
        if (mode == NORM_DIV) {
            initial_scale = get_scale_2x(&a);
            maximize_scale(&a);
        }
        int scale = get_scale_2x(&a);
        set_scale_2x(&a, 0);
        set_scale_2x(&b, 0);
        d2x tmp = b;
        int k = find_last_bit_2x(&a) - find_last_bit_2x(&b);
        for (int i = 0; i < k; i++) {
            dec_shift_2x(&tmp);
        }
        for (int i = 0; i <= k; i++) {
            if (d2x_is_less(a, tmp) == TRUE) {
                write_last_bit_2x(0, res);
            } else {
                a = substract_2x(a, tmp);
                write_last_bit_2x(1, res);
            }
            dec_shift_right_2x(&tmp);
        }

        if (mode == SCALE_DECREASE_DIV) {
            if (scale > 0) {
                scale--;
            }
        }
        set_scale_2x(res, scale - initial_scale);
        set_scale_2x(&a, scale - initial_scale);
        if (mode == NORM_DIV) {
            remove_trailing_zeroes_2x(res);
        }
    } else if (mode == SCALE_DECREASE_DIV && get_scale_2x(&a) != 0) {
        set_scale_2x(res, 0);
    }

    return a;  // a is a remainder at the end
}

c_decimal dec_div(c_decimal a, c_decimal b) {
    c_decimal out = init();
    c_decimal zero = init();
    c_decimal one = init();
    set_bit(&one, 0);
    c_decimal mone = init();
    set_bit(&mone, 0);
    set_sign(&mone, 1);
    if (a.value_type == DEC_NORMAL_VALUE && b.value_type == DEC_NORMAL_VALUE) {
        if (dec_is_not_equal(b, zero) == TRUE) {
            d2x ax = decimal_to_2x(a);
            d2x bx = decimal_to_2x(b);
            d2x res = init_2x();
            div_2x(ax, bx, &res, NORM_DIV);
            out = fit_2x_into_decimal(res);
            if (get_sign(a) == 1 && get_sign(b) == 1) {
                set_sign(&out, 0);
            } else if (get_sign(a) == 1 || get_sign(b) == 1) {
                set_sign(&out, 1);
            }
        } else if (dec_is_equal(a, zero) == dec_is_equal(b, zero)) {
            out.value_type = DEC_NAN;
        } else if (dec_is_greater_or_equal(a, one) == TRUE && dec_is_equal(b, zero) == TRUE) {
            out.value_type = DEC_INFINITY;
        } else if (dec_is_less_or_equal(a, mone) == TRUE && dec_is_equal(b, zero) == TRUE) {
            out.value_type = DEC_NEGATIVE_INFINITY;
        }
    } else if (a.value_type == DEC_NAN || b.value_type == DEC_NAN) {
        out.value_type = DEC_NAN;
    } else if (a.value_type == DEC_NORMAL_VALUE && b.value_type == DEC_INFINITY) {
        if (get_sign(a) == 0) {
            out.value_type = DEC_NORMAL_VALUE;
            set_sign(&out, 0);
        } else {
            out.value_type = DEC_NORMAL_VALUE;
            set_sign(&out, 1);
        }
    } else if (a.value_type == DEC_NORMAL_VALUE && b.value_type == DEC_NEGATIVE_INFINITY) {
        if (get_sign(a) == 1) {
            out.value_type = DEC_NORMAL_VALUE;
            set_sign(&out, 0);
        } else {
            out.value_type = DEC_NORMAL_VALUE;
            set_sign(&out, 1);
        }
    } else if (a.value_type == DEC_INFINITY && b.value_type == DEC_NORMAL_VALUE) {
        if (get_sign(b) == 0) {
            out.value_type = DEC_INFINITY;
        } else {
            out.value_type = DEC_NEGATIVE_INFINITY;
        }
    } else if (a.value_type == DEC_NEGATIVE_INFINITY && b.value_type == DEC_NORMAL_VALUE) {
        if (get_sign(b) == 0) {
            out.value_type = DEC_NEGATIVE_INFINITY;
        } else {
            out.value_type = DEC_INFINITY;
        }
    }
    return out;
}

c_decimal dec_mod(c_decimal a, c_decimal b) {
    c_decimal out = init();
    if (a.value_type == DEC_NORMAL_VALUE && b.value_type == DEC_NORMAL_VALUE) {
        if (!is_zero(b)) {
            d2x ax = decimal_to_2x(a);
            d2x bx = decimal_to_2x(b);
            d2x res = decimal_to_2x(a);
            d2x result = div_2x(ax, bx, &res, INT_DIV);
            out = fit_2x_into_decimal(result);
            if (get_sign(a) == 1 && get_sign(b) == 1) {
                set_sign(&out, 1);
            } else if (get_sign(a) == 1 && get_sign(b) == 0) {
                set_sign(&out, 1);
            }
        } else {
            out.value_type = DEC_NAN;
        }
    } else if (a.value_type != DEC_NORMAL_VALUE) {
        out.value_type = DEC_NAN;
    } else if (b.value_type == DEC_NORMAL_VALUE && is_zero(b)) {
        out.value_type = DEC_NAN;
    } else if (a.value_type == DEC_NAN || b.value_type == DEC_NAN) {
        out.value_type = DEC_NAN;
    } else {
        out = a;
    }
    return out;
}

void add_int_2x(d2x *d, int i) {
    d2x num = init_2x();
    num.bits[LOW] = i;
    int overflow = 0;
    int scale = get_scale_2x(d);
    *d = addition_2x(*d, num, &overflow);
    set_scale_2x(d, scale);
}

void bank_round(d2x *d, d2x remainder) {
    if (remainder.bits[LOW] >= 5) {
        // Bankers rounding
        if (remainder.bits[LOW] == 5) {
            d2x tmp = init_2x();
            d2x divisor = init_2x();
            divisor.bits[LOW] = 2;
            remainder = div_2x(*d, divisor, &tmp, SCALE_DECREASE_DIV);
            if (remainder.bits[LOW] != 0) {
                add_int_2x(d, 1);
            }
        } else {
            add_int_2x(d, 1);
        }
    }
}

d2x decrease_scale_2x(d2x *d) {
    int scale = get_scale_2x(d);
    d2x remainder = init_2x();
    if (scale > 0) {
        int sign = get_sign_2x(d);
        d2x divisor = init_2x();
        divisor.bits[LOW] = 10;
        d2x res = init_2x();
        remainder = div_2x(*d, divisor, &res, SCALE_DECREASE_DIV);
        if (sign) {
            set_sign_2x(&res, 1);
        }
        *d = res;
    }

    return remainder;
}

void remove_trailing_zeroes_2x(d2x *d) {
    d2x ten = init_2x();
    ten.bits[LOW] = 10;
    d2x tmp = init_2x();
    int sign = get_sign_2x(d);
    d2x remainder = div_2x(*d, ten, &tmp, SCALE_DECREASE_DIV);
    while (remainder.bits[LOW] == 0 && get_scale_2x(d) > 0) {
        decrease_scale_2x(d);
        remainder = div_2x(*d, ten, &tmp, SCALE_DECREASE_DIV);
    }
    if (sign) {
        set_sign_2x(d, 1);
    }
}

d2x decrease_scale_to_target_2x(d2x *d, int target) {
    d2x remainder = init_2x();
    while (get_scale_2x(d) > target) {
        remainder = decrease_scale_2x(d);
    }
    return remainder;
}

c_decimal fit_2x_into_decimal(d2x d) {
    c_decimal res = init();
    if (d.value_type != DEC_NORMAL_VALUE && d.value_type != ALLOW_OVERFLOW) {
        res.value_type = d.value_type;
    } else {
        d2x remainder = init_2x();
        if (get_scale_2x(&d) > SCALE_MAX) {
            remainder = decrease_scale_to_target_2x(&d, SCALE_MAX);
        }
        while (!check_2x_empty(&d) && get_scale_2x(&d) > 0) {
            remainder = decrease_scale_2x(&d);
        }
        if (check_2x_empty(&d)) {
            bank_round(&d, remainder);
            remove_trailing_zeroes_2x(&d);
            res = d2x_to_decimal(&d);
            // Convert -0 into 0
            if (is_zero(res)) {
                set_sign(&res, 0);
            }
        } else {
            if (get_sign_2x(&d)) {
                res.value_type = DEC_NEGATIVE_INFINITY;
            } else {
                res.value_type = DEC_INFINITY;
            }
        }
    }

    return res;
}

// Another functions block

c_decimal dec_floor(c_decimal a) {
    c_decimal out = a;
    d2x out_2x = decimal_to_2x(out);

    if (a.value_type == DEC_NORMAL_VALUE) {
        if (get_scale(a) != 0) {
            if (get_sign(a) == 1) {
                while (get_scale_2x(&out_2x) > 1) {
                    decrease_scale_2x(&out_2x);
                }
                d2x ten = init_2x();
                ten.bits[LOW] = 10;
                d2x tmp = init_2x();
                d2x remainder = div_2x(out_2x, ten, &tmp, SCALE_DECREASE_DIV);
                out_2x = tmp;
                if (remainder.bits[LOW] > 0) {
                    d2x one = init_2x();
                    one.bits[LOW] = 1;
                    int overflow = 0;
                    out_2x = addition_2x(tmp, one, &overflow);
                }
                set_sign_2x(&out_2x, 1);
                out = fit_2x_into_decimal(out_2x);
            } else {
                out = dec_truncate(a);
            }
        }
    }

    return out;
}

c_decimal dec_round(c_decimal a) {
    c_decimal out = a;
    if (a.value_type == DEC_NORMAL_VALUE && get_scale(a) > 0) {
        d2x out_2x = decimal_to_2x(a);
        d2x ten = init_2x();
        ten.bits[LOW] = 10;
        int sign = get_sign(a);
        while (get_scale_2x(&out_2x) > 1) {
            decrease_scale_2x(&out_2x);
        }
        d2x tmp = init_2x();
        d2x remainder =  div_2x(out_2x, ten, &tmp, SCALE_DECREASE_DIV);
        if (remainder.bits[LOW] > 5) {
            add_int_2x(&tmp, 1);
        }
        out_2x = tmp;
        set_sign_2x(&out_2x, sign);
        out = fit_2x_into_decimal(out_2x);
    }


    return out;
}

c_decimal dec_truncate(c_decimal a) {
    c_decimal out = a;
    d2x out_2x = decimal_to_2x(out);

    if (a.value_type == DEC_NORMAL_VALUE) {
        while (get_scale_2x(&out_2x) > 0) {
            decrease_scale_2x(&out_2x);
        }
    }

    out = d2x_to_decimal(&out_2x);

    return out;
}

// converters block

int from_decimal_to_int(c_decimal src, int *dst) {
    int code = CALCULATION_ERROR;

    if (dst != NULL && src.value_type == DEC_NORMAL_VALUE) {
        int pass = 1;
        c_decimal tmp = dec_truncate(src);
        if (!!get_bit(tmp, 31)) {
            pass = 0;
        }
        for (int i = MID; i <= HIGH && pass; i++) {
            if (tmp.bits[i] != 0) {
                pass = 0;
            }
        }
        if (pass) {
            code = SUCCESS;
            *dst = tmp.bits[LOW];
            if (get_sign(src)) {
                *dst *= -1;
            }
        }
    }

    return code;
}

// equal operators

int dec_is_equal(c_decimal a, c_decimal b) {
    int res = FALSE;

    if (is_zero(a)) {
        set_sign(&a, 0);
    }

    if (is_zero(b)) {
        set_sign(&b, 0);
    }

    d2x d2x_a = decimal_to_2x(a);
    d2x d2x_b = decimal_to_2x(b);

    res = d2x_is_equal(d2x_a, d2x_b);

    return res;
}

int dec_is_not_equal(c_decimal a, c_decimal b) {
    int res = FALSE;
    if (dec_is_equal(a, b) == FALSE) {
        res = TRUE;
    }
    return res;
}

int dec_is_less(c_decimal a, c_decimal b) {
    int res = 0;

    d2x d2x_a = decimal_to_2x(a);
    d2x d2x_b = decimal_to_2x(b);

    res = d2x_is_less(d2x_a, d2x_b);

    return res;
}

int dec_is_less_or_equal(c_decimal a, c_decimal b) {
    int res = FALSE;
    if (dec_is_less(a, b) == TRUE || dec_is_equal(a, b) == TRUE) {
        res = TRUE;
    }
    return res;
}

int dec_is_greater(c_decimal a, c_decimal b) {
    int res = FALSE;

    d2x d2x_a = decimal_to_2x(a);
    d2x d2x_b = decimal_to_2x(b);

    res = d2x_is_greater(d2x_a, d2x_b);

    return res;
}

int dec_is_greater_or_equal(c_decimal a, c_decimal b) {
    int res = FALSE;
    if (dec_is_greater(a, b) == TRUE || dec_is_equal(a, b) == TRUE) {
        res = TRUE;
    }
    return res;
}

int from_decimal_to_float(c_decimal src, float *dst) {
    int code = SUCCESS;
    if (dst == NULL) {
        code = CALCULATION_ERROR;
    } else {
        if (src.value_type == DEC_NAN) {
            *dst = NAN;
        } else if (src.value_type == DEC_NEGATIVE_INFINITY) {
            *dst = -INFINITY;
        } else if (src.value_type == DEC_INFINITY) {
            *dst = INFINITY;
        } else {
            *dst = 0;
            for (int i = 0; i < LAST_DEC_BIT; i++) {
                if (!!get_bit(src, i)) {
                    *dst += pow(2, i);
                }
            }
            int scale = get_scale(src);
            *dst *= pow(10, -scale);
            if (get_sign(src) == 1) {
                *dst *= -1;
            }
        }
    }
    return code;
}

