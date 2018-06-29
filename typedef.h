#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <stdint.h>
#ifdef __SYNTHESIS__
#include "hls_half.h"
typedef half ffloat;
#else
typedef float ffloat;
#endif

#include <complex>
typedef std::complex<float> floatc;
#endif
