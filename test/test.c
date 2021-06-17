
// This file is a part of MRNIU/LUTF
// (https://github.com/MRNIU/LUTF).
//
// test.c for MRNIU/LUTF.

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "ch16.h"

int main(int    argc __attribute__((unused)),
         char **argv __attribute__((unused))) {
    msop_t *msop = (msop_t *)malloc(1248);
    get_msop(msop);
    uint16_t len = msop->header.len;
    printf("len: %d\n", len);
    assert(len == 1214);
    return 0;
}

#ifdef __cplusplus
}
#endif
