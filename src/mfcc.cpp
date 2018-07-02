/**
 * @file mfcc.cpp
 * @brief MFCC feature extraction to match with TensorFlow MFCC Op
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */

#include "fft.h"
#include "mfcc.h"
#include <float.h>
#include <math.h>
#include <string.h>

float mfcc_frame[FILTER_LEN];
float mfcc_buffer[FILTER_LEN];
float mel_energies[NUM_FBANK_BINS];

void compute(const int16_t *input, float *output) {
  for (int j = 0; j < 49; j++) {
    mfcc_compute(input + 320 * j, output + 10 * j);
  }
}

void mfcc_compute(const int16_t *audio_data, float *mfcc_out) {

  int32_t i, j, bin;

  // TensorFlow way of normalizing .wav data to (-1,1)
  for (i = 0; i < FILTER_LEN; i++) {
    if (i < FRAME_LEN)
      mfcc_frame[i] = ((float)audio_data[i]) / (1 << 15);
    else
      mfcc_frame[i] = 0;
  }

  for (i = 0; i < FRAME_LEN; i++) {
    mfcc_frame[i] *= window_func[i];
  }

  // Compute FFT
  floatc zero_padded[FILTER_LEN];
  for (int i = 0; i < FILTER_LEN; ++i) {
    if (i < FRAME_LEN)
      zero_padded[i] = mfcc_frame[i];
    else
      zero_padded[i] = floatc(0, 0);
  }
  FFT(zero_padded);
  // Convert to power spectrum
  // frame is stored as [real0, realN/2-1, real1, im1, real2, im2, ...]
  int32_t half_dim = FILTER_LEN / 2;
  for (i = 0; i < half_dim; i++) {
    float real = zero_padded[i].real(), im = zero_padded[i].imag();
    mfcc_buffer[i] = real * real + im * im;
  }

  float sqrt_data;
  // Apply mel filterbanks
  for (bin = 0; bin < NUM_FBANK_BINS; bin++) {
    float mel_energy = 0;
    uint8_t first_index = fbank_filter_first[bin];
    uint8_t last_index = fbank_filter_last[bin];
    j = 0;
    for (i = first_index; i <= last_index; i++) {
      sqrt_data = sqrtf(mfcc_buffer[i]);
      mel_energy += (sqrt_data)*mel_fbank[bin * NUM_FBANK_MAXW + j++];
    }
    mel_energies[bin] = mel_energy;

    // avoid log of zero
    if (mel_energy == 0.0)
      mel_energies[bin] = FLT_MIN;
  }
  // Take log
  for (bin = 0; bin < NUM_FBANK_BINS; bin++)
    mel_energies[bin] = logf((float)mel_energies[bin]);
  // Take DCT. Uses matrix mul.
  for (i = 0; i < NUM_MFCC_FEATURES; i++) {
    float sum = 0.0;
    for (j = 0; j < NUM_FBANK_BINS; j++) {
      sum += dct_matrix[i * NUM_FBANK_BINS + j] * mel_energies[j];
    }
    mfcc_out[i] = sum;
  }
}
