#include "c_decimal.h"
#include "dev_tools.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void float_print_bin(float in) {
    typedef union {
        int i;
        float f;
    } u;
    u u1;
    u1.f = in;
    int_print_bin(u1.i);
}

void bin_logic(int in) {
    unsigned int mask = 0x80000000;
    int step = 0;
    while (mask > 0) {
        if (step == 4) {
            printf(" ");
            step = 0;
        }
        printf("%d", !!(in & mask));
        mask = mask >> 1;
        step++;
    }
}

void int_print_bin(int in) {
    bin_logic(in);
    printf("\n");
}

unsigned int int_to_unsigned(int in) {
    typedef union {
        int i;
        unsigned int ui;
    } u;
    u u1;
    u1.i = in;
    return u1.ui;
}

void decimal_print_bin(c_decimal d) {
    printf("LOW  : ");
    bin_logic(d.bits[LOW]);
    printf(" | %u\n", int_to_unsigned(d.bits[LOW]));
    printf("MID  : ");
    bin_logic(d.bits[MID]);
    printf(" | %u\n", int_to_unsigned(d.bits[MID]));
    printf("HIGH : ");
    bin_logic(d.bits[HIGH]);
    printf(" | %u\n", int_to_unsigned(d.bits[HIGH]));
    printf("SCALE: ");
    bin_logic(d.bits[SCALE]);
    printf(" | %d\n", get_scale(d));
    printf("TYPE:  %d\n", d.value_type);
}

void d2x_print_bin(d2x d) {
    printf("LOW     : ");
    bin_logic(d.bits[LOW]);
    printf(" | %u\n", int_to_unsigned(d.bits[LOW]));
    printf("MID     : ");
    bin_logic(d.bits[MID]);
    printf(" | %u\n", int_to_unsigned(d.bits[MID]));
    printf("HIGH    : ");
    bin_logic(d.bits[HIGH]);
    printf(" | %u\n", int_to_unsigned(d.bits[HIGH]));
    printf("LOW_2X  : ");
    bin_logic(d.bits[LOW_2X]);
    printf(" | %u\n", int_to_unsigned(d.bits[LOW_2X]));
    printf("MID_2X  : ");
    bin_logic(d.bits[MID_2X]);
    printf(" | %u\n", int_to_unsigned(d.bits[MID_2X]));
    printf("HIGH_2X : ");
    bin_logic(d.bits[HIGH_2X]);
    printf(" | %u\n", int_to_unsigned(d.bits[HIGH_2X]));
    printf("SCALE_2X: ");
    bin_logic(d.bits[SCALE_2X]);
    printf(" | %d\n", get_scale_2x(&d));
}

// Copy the result of this function into wolfram alpha to see the number stored in d2x
void d2x_gen_hex(d2x d) {
    char res[512];
    char tmp[512];
    snprintf(res, sizeof(res), "%08x", d.bits[LOW]);
    snprintf(tmp, sizeof(tmp), "%s", res);
    snprintf(res, sizeof(res), "%08x%s", d.bits[MID], tmp);
    snprintf(tmp, sizeof(tmp), "%s", res);
    snprintf(res, sizeof(res), "%08x%s", d.bits[HIGH], tmp);
    snprintf(tmp, sizeof(tmp), "%s", res);
    snprintf(res, sizeof(res), "%08x%s", d.bits[LOW_2X], tmp);
    snprintf(tmp, sizeof(tmp), "%s", res);
    snprintf(res, sizeof(res), "%08x%s", d.bits[MID_2X], tmp);
    snprintf(tmp, sizeof(tmp), "%s", res);
    snprintf(res, sizeof(res), "%08x%s", d.bits[HIGH_2X], tmp);
    printf("0x%s * 10^(-%d)\n", res, get_scale_2x(&d));
}

void decimal_gen_hex(c_decimal d) {
    char res[512];
    char tmp[512];
    snprintf(res, sizeof(res), "%08x", d.bits[LOW]);
    snprintf(tmp, sizeof(tmp), "%s", res);
    snprintf(res, sizeof(res), "%08x%s", d.bits[MID], tmp);
    snprintf(tmp, sizeof(tmp), "%s", res);
    snprintf(res, sizeof(res), "%08x%s", d.bits[HIGH], tmp);
    printf("0x%s * 10^(-%d)\n", res, get_scale(d));
}

void decimal_gen_test(c_decimal d) {
    printf("int og[4];\n");
    printf("og[LOW]   = 0x%x;\n", d.bits[LOW]);
    printf("og[MID]   = 0x%x;\n", d.bits[MID]);
    printf("og[HIGH]  = 0x%x;\n", d.bits[HIGH]);
    printf("og[SCALE] = 0x%x;\n", d.bits[SCALE]);
}
