#ifndef C_DECIMAL_H_
#define C_DECIMAL_H_

#define FLOAT_MAX 79228162514264337593543950335.0
#define FLOAT_MIN 1e-28

#define TRUE 0
#define FALSE 1

#define SUCCESS 0
#define CALCULATION_ERROR 1

// !! Last bit is this -1 !!
#define LAST_DEC_BIT 96
#define LAST_2DX_BIT 192

#define LAST_SCALE_BIT 23
#define SCALE_MAX 28

// Division modes
#define INT_DIV 0
#define NORM_DIV 1
#define SCALE_DECREASE_DIV 2

typedef enum {
    LOW,
    MID,
    HIGH,
    SCALE,
    LOW_2X = 3,
    MID_2X = 4,
    HIGH_2X = 5,
    SCALE_2X = 6,
} index_decimal;

typedef enum {
    DEC_NORMAL_VALUE = 0,
    DEC_INFINITY = 1,
    DEC_NEGATIVE_INFINITY = 2,
    DEC_NAN = 3,
    ALLOW_OVERFLOW = 4
} value_type_t;

typedef struct {
    int bits[4];
    value_type_t value_type;
} c_decimal;

// Decimal, but 2x the size
typedef struct {
    int bits[7];
    value_type_t value_type;
} d2x;

// Arithmetic operators

c_decimal dec_add(c_decimal, c_decimal);
c_decimal dec_sub(c_decimal a, c_decimal b);
c_decimal dec_mul(c_decimal a, c_decimal b);
c_decimal dec_div(c_decimal, c_decimal);
c_decimal dec_mod(c_decimal, c_decimal);
c_decimal dec_negate(c_decimal a);

// Equals operators

int dec_is_less(c_decimal, c_decimal);
int dec_is_less_or_equal(c_decimal, c_decimal);
int dec_is_greater(c_decimal, c_decimal);
int dec_is_greater_or_equal(c_decimal, c_decimal);
int dec_is_equal(c_decimal, c_decimal);
int dec_is_not_equal(c_decimal, c_decimal);

// Converters

int from_int_to_decimal(int src, c_decimal *dst);
int from_float_to_decimal(float src, c_decimal *dst);
int from_decimal_to_int(c_decimal src, int *dst);
int from_decimal_to_float(c_decimal src, float *dst);

// Dev functions
int get_bit(c_decimal d, int i);
void set_bit(c_decimal *d, int i);
void clear_bit(c_decimal *d, int i);
void set_sign(c_decimal *d, int i);
int get_sign(c_decimal d);
int set_scale(c_decimal *d, int scale);
int get_scale(c_decimal d);
void clear_scale(c_decimal *d);

// Decimal2x processing functions
d2x init_2x();
int get_bit_2x(const d2x *d, int i);
void clear_bit_2x(d2x *d, int i);
void set_bit_2x(d2x *d, int i);
void set_sign_2x(d2x *d, int i);
int get_sign_2x(const d2x *d);
int set_scale_2x(d2x *d, int scale);
int get_scale_2x(const d2x *d);
void clear_scale_2x(d2x *d);
void set_decimal_inf(c_decimal *in, int sign);
int is_zero(c_decimal in);
d2x decrease_scale_2x(d2x *d);
void remove_trailing_zeroes_2x(d2x *d);
d2x decrease_scale_to_target_2x(d2x *d, int target);
c_decimal fit_2x_into_decimal(d2x d);
c_decimal init();

// Decimal2x operators
d2x addition_2x(d2x a, d2x b, int *overflow);
d2x multiply_2x(d2x a, d2x b, int *overflow);
d2x substract_2x(d2x a, d2x b);
d2x div_2x(d2x a, d2x b, d2x *res, int mode);

c_decimal dec_floor(c_decimal);
c_decimal dec_round(c_decimal);
c_decimal dec_truncate(c_decimal);

// Decimal2x Comparison operators
int d2x_is_less(d2x a, d2x b);

// Decimal2x extras
int dec_shift_2x(d2x *d);
void dec_shift_right_2x(d2x *d);
int normalise_scale_2x(d2x *a, d2x *b);
d2x decimal_to_2x(c_decimal src);
c_decimal d2x_to_decimal(const d2x *src);
int check_2x_empty(const d2x *in);
int is_zero_2x(const d2x *in);
void bank_round(d2x *d, d2x remainder);
int d2x_is_equal(d2x a, d2x b);

// Extra tools
int count_digits(double in);

#endif  // C_DECIMAL_H_
