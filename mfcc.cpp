/**
 * @file mfcc.cpp
 * @brief MFCC feature extraction to match with TensorFlow MFCC Op
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */


#include "mfcc.h"

float mfcc_frame[FILTER_LEN];
float mfcc_buffer[FILTER_LEN];
float mel_energies[NUM_FBANK_BINS];
float vecW_r[FILTER_LEN/2] = {
#include "vecW_re.dat"
};
float vecW_i[FILTER_LEN/2] = {
#include "vecW_im.dat"
};


void FFT(complex<float>*  vec)
{
  uint16_t ulIndex;
  uint16_t ulK;
  for(uint16_t p = 0; p < FILTER_LEN; p++)
  {
    ulIndex = 0;
    ulK = 1;
    for(uint16_t j = 0; j < FILTER_POW; j++)
      {
        ulIndex += (p&(1<<(FILTER_POW - j - 1)))? ulK : 0;
        ulK *= 2;
      }

    if(ulIndex > p)
      {
        complex<float> c = vec[p];
        vec[p] = vec[ulIndex];
        vec[ulIndex] = c;
      }
  }


  uint16_t ulGroupLength = 1;
  uint16_t ulHalfLength = 0;
  complex<float> cw;
  complex<float> c1;
  complex<float> c2;
  for(uint16_t b = 0; b < FILTER_POW; b++)
    {
      ulHalfLength = ulGroupLength;
      ulGroupLength *= 2;
      for(uint16_t j = 0; j < FILTER_LEN; j += ulGroupLength)
        {
          for(uint16_t k = 0; k < ulHalfLength; k++)
            {
              uint16_t i1=(k*(512>>b)),i2=(j+k+ulHalfLength);
              cw = complex<float>(vecW_r[i1], vecW_i[i1])*vec[i2];
              c1 = vec[j + k] + cw;
              c2 = vec[j + k] - cw;
              vec[j + k] = c1;
              vec[j + k + ulHalfLength] = c2;
            }
        }
    }
}

void mfcc_compute(const int16_t * audio_data, float* mfcc_out) {

  int32_t i, j, bin;

  //TensorFlow way of normalizing .wav data to (-1,1)
  for (i = 0; i < FRAME_LEN; i++) {
    mfcc_frame[i] = (float)audio_data[i]/(1<<15);
  }
  //Fill up remaining with zeros
  for (i = FRAME_LEN; i < FILTER_LEN; i++) {
    mfcc_frame[i] = 0;
  }

  for (i = 0; i < FRAME_LEN; i++) {
    mfcc_frame[i] *= window_func[i];
  }

  //Compute FFT
  complex<float> zero_padded[FILTER_LEN]={0};
  for (int i = 0; i < FRAME_LEN; ++i) {
    zero_padded[i] = mfcc_frame[i];
  }
  FFT(zero_padded);
  //Convert to power spectrum
  //frame is stored as [real0, realN/2-1, real1, im1, real2, im2, ...]
  int32_t half_dim = FILTER_LEN/2;
  float first_energy = zero_padded[0].real() * zero_padded[0].real(),
        last_energy =  zero_padded[half_dim-1].real() * zero_padded[half_dim-1].real();  // handle this special case
  for (i = 1; i < half_dim; i++) {
    float real = zero_padded[i].real(), im = zero_padded[i].imag();
    mfcc_buffer[i] = real*real + im*im;
  }
  mfcc_buffer[0] = first_energy;
  mfcc_buffer[half_dim] = last_energy;

  float sqrt_data;
  //Apply mel filterbanks
  for (bin = 0; bin < NUM_FBANK_BINS; bin++) {
    float mel_energy = 0;
    int32_t first_index = fbank_filter_first[bin];
    int32_t last_index = fbank_filter_last[bin];
    j = 0;
    for (i = first_index; i <= last_index; i++) {
      sqrt_data = sqrt(mfcc_buffer[i]);
      mel_energy += (sqrt_data) * mel_fbank[bin*NUM_FBANK_MAXW+j++];
    }
    mel_energies[bin] = mel_energy;

    //avoid log of zero
    if (mel_energy == 0.0)
      mel_energies[bin] = FLT_MIN;
  }
  //Take log
  for (bin = 0; bin < NUM_FBANK_BINS; bin++)
    mel_energies[bin] = logf(mel_energies[bin]);
  //Take DCT. Uses matrix mul.
  for (i = 0; i < NUM_MFCC_FEATURES; i++) {
    float sum = 0.0;
    for (j = 0; j < NUM_FBANK_BINS; j++) {
      sum += dct_matrix[i*NUM_FBANK_BINS+j] * mel_energies[j];
    }
    mfcc_out[i] = sum;
    //Input is Qx.mfcc_dec_bits (from quantization step)
    // sum *= (0x1<<mfcc_dec_bits);
    // sum = round(sum);
    // if(sum >= 127)
    //   mfcc_out[i] = 127;
    // else if(sum <= -128)
    //   mfcc_out[i] = -128;
    // else
    //   mfcc_out[i] = sum;
  }

}

