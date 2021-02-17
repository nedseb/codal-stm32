#pragma once
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
#endif
#if GCC_VERSION < 60300
  #error "GCC version 6.3 or higher is required"
#endif

#include "pins_arduino.h"