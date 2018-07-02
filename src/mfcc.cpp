/**
 * @file mfcc.cpp
 * @brief MFCC feature extraction to match with TensorFlow MFCC Op
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */

#include "fft.h"
#include "mfcc.h"
#include <math.h>

void compute(const int16_t *input, float *output) {
  for (int j = 0; j < 49; j++) {
    mfcc_compute(input + 320 * j, output + 10 * j);
  }
}

void mfcc_compute(const int16_t *audio_data, float *mfcc_out) {
  floatc mfcc_frame[FILTER_LEN];
  floatc mfcc_tmp[FILTER_LEN];
  float mfcc_buffer[FILTER_LEN];
  float mel_energies[FILTER_LEN];

  // TensorFlow way of normalizing .wav data to (-1,1)
  for (int i = 0; i < FILTER_LEN; i++) {
#pragma HLS PIPELINE II = 1
    if (i < FRAME_LEN)
      mfcc_frame[i] = ((float)audio_data[i]) / (1 << 15) * window_func[i];
    else
      mfcc_frame[i] = 0;
  }

  FFT2(mfcc_frame, mfcc_tmp);
  // Convert to power spectrum
  // frame is stored as [real0, realN/2-1, real1, im1, real2, im2, ...]
  int32_t half_dim = FILTER_LEN / 2;
  for (int i = 0; i < half_dim; i++) {
#pragma HLS PIPELINE
    float re = mfcc_tmp[i].real() * mfcc_tmp[i].real();
    float im = mfcc_tmp[i].imag() * mfcc_tmp[i].imag();
    float add = re + im;
    mfcc_buffer[i] = sqrtf(add);
  }

  // Apply mel filterbanks
  for (int bin = 0; bin < NUM_FBANK_BINS; bin++) {
    float mel_energy = 0;
    uint8_t first_index = fbank_filter_first[bin];
    uint8_t last_index = fbank_filter_last[bin];
    int j = 0;
    for (int i = first_index; i <= last_index; i++) {
#pragma HLS PIPELINE II = 5
#pragma HLS LOOP_TRIPCOUNT min = 1 max = 30 avg = 8
      mel_energy += mfcc_buffer[i] * mel_fbank[bin * NUM_FBANK_MAXW + j++];
    }
    // avoid log of zero
    mel_energies[bin] = logf(mel_energy);
  }
  // Take DCT. Uses matrix mul.
  for (int i = 0; i < NUM_MFCC_FEATURES; i++) {
    float sum = 0.0;
    for (int j = 0; j < NUM_FBANK_BINS; j++) {
#pragma HLS PIPELINE
      sum += (float)(dct_matrix[i * NUM_FBANK_BINS + j] * mel_energies[j]);
    }
    mfcc_out[i] = sum;
  }
}
