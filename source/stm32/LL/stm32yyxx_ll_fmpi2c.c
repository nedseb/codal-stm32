/* LL raised several warnings, ignore them */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifdef STM32F4xx
    #include "stm32f4xx_ll_fmpi2c.c"
#endif
#pragma GCC diagnostic pop
