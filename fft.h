#ifndef __KWS_FFT_H__
#define __KWS_FFT_H__

#include "typedef.h"


const char FFT_NFFT_MAX                        = 10;
const int  FFT_LENGTH                          = 1 << FFT_NFFT_MAX;

#ifdef HWFFT
#include "hls_fft.h"

// configurable params
const char FFT_INPUT_WIDTH                     = 32;
const char FFT_OUTPUT_WIDTH                    = 32;
const char FFT_CONFIG_WIDTH                    = 16;

struct config1 : hls::ip_fft::params_t {
    static const unsigned ordering_opt = hls::ip_fft::natural_order;
    static const unsigned config_width = FFT_CONFIG_WIDTH;
};

typedef hls::ip_fft::config_t<config1> config_t;
typedef hls::ip_fft::status_t<config1> status_t;
#else

#endif

void FFT(floatc in[FFT_LENGTH]);

#endif
