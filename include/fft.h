#ifndef FFT_H
#define FFT_H

#include "typedef.h"

const char FFT_NFFT_MAX = 10;
const unsigned int FFT_LENGTH = 1 << FFT_NFFT_MAX;

#ifdef __SYNTHESIS__
#include "hls_fft.h"
#include <complex>

// configurable params
const char FFT_INPUT_WIDTH = 32;
const char FFT_OUTPUT_WIDTH = 32;
const char FFT_CONFIG_WIDTH = 16;

struct config1 : hls::ip_fft::params_t {
  static const unsigned ordering_opt = hls::ip_fft::natural_order;
  static const unsigned config_width = FFT_CONFIG_WIDTH;
  static const unsigned phase_factor_width = 24;
};

typedef hls::ip_fft::config_t<config1> config_t;
typedef hls::ip_fft::status_t<config1> status_t;

void dummy_proc_fe(floatc in[FFT_LENGTH], floatc out[FFT_LENGTH]);

void dummy_proc_be(floatc in[FFT_LENGTH], floatc out[FFT_LENGTH]);

void FFT2(floatc in[FFT_LENGTH], floatc out[FFT_LENGTH]);
#else
#endif
void FFT2(floatc in[FFT_LENGTH], floatc out[FFT_LENGTH]);
void FFT(floatc in[FFT_LENGTH]);

#endif // FFT_H
