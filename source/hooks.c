#include <stdint.h>

/**
 * Empty yield() hook.
 *
 * This function is intended to be used by library writers to build
 * libraries or sketches that supports cooperative threads.
 *
 * Its defined as a weak symbol and it can be redefined to implement a
 * real cooperative scheduler.
 */
static void __empty()
{
    // Empty
}
void yield(void) __attribute__((weak, alias("__empty")));

#ifdef DTR_TOGGLING_SEQ
/**
 * Empty dtr_toggling() hook.
 *
 * This function is intended to be used by library writers to build
 * libraries or sketches that require DTR toggling feature.
 *
 * Its defined as a weak symbol and it can be redefined to implement
 * task to achieve in this case.
 */
static void __empty_dtr_toggling(uint8_t* buf, uint32_t* len)
{
    (void)buf;
    (void)len;
}
void dtr_togglingHook(uint8_t* buf, uint32_t* len) __attribute__((weak, alias("__empty_dtr_toggling")));
#endif
