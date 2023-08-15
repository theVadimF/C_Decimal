#ifndef DEV_TOOLS_H_
#define DEV_TOOLS_H_

#include "c_decimal.h"

void int_print_bin(int in);
void decimal_print_bin(c_decimal d);
void float_print_bin(float in);
void d2x_print_bin(d2x d);
void d2x_gen_hex(d2x d);
void decimal_gen_hex(c_decimal d);
void decimal_gen_test(c_decimal d);

#endif  // DEV_TOOLS_H_
