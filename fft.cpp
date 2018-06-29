#include "fft.h"
#ifdef HWFFT
void FFT(floatc in[FFT_LENGTH]) {
    floatc tmp[FFT_LENGTH];
    int i;
    config_t fft_config;
    status_t fft_status;

    // FFT IP
    hls::fft<config1>(in, tmp, &fft_status, &fft_config);
    for (i=0;i<FFT_LENGTH;i++)
        in[i] = tmp[i];
}

#else
float vecW_r[FFT_LENGTH/2] = {
#include "vecW_re.dat"
};
float vecW_i[FFT_LENGTH/2] = {
#include "vecW_im.dat"
};

void FFT(floatc vec[FFT_LENGTH])
{
  uint16_t ulIndex;
  uint16_t ulK;
  for(uint16_t p = 0; p < FFT_LENGTH; p++)
  {
    ulIndex = 0;
    ulK = 1;
    for(uint16_t j = 0; j < FFT_NFFT_MAX; j++)
      {
        ulIndex += (p&(1<<(FFT_NFFT_MAX - j - 1)))? ulK : 0;
        ulK *= 2;
      }

    if(ulIndex > p)
      {
        floatc c = vec[p];
        vec[p] = vec[ulIndex];
        vec[ulIndex] = c;
      }
  }

  uint16_t ulGroupLength = 1;
  uint16_t ulHalfLength = 0;
  floatc cw;
  floatc c1;
  floatc c2;
  for(uint16_t b = 0; b < FFT_NFFT_MAX; b++)
    {
      ulHalfLength = ulGroupLength;
      ulGroupLength *= 2;
      for(uint16_t j = 0; j < FFT_LENGTH; j += ulGroupLength)
        {
          for(uint16_t k = 0; k < ulHalfLength; k++)
            {
              uint16_t i1=(k*(512>>b)),i2=(j+k+ulHalfLength);
              cw = floatc(vecW_r[i1], vecW_i[i1])*vec[i2];
              c1 = vec[j + k] + cw;
              c2 = vec[j + k] - cw;
              vec[j + k] = c1;
              vec[j + k + ulHalfLength] = c2;
            }
        }
    }
}
#endif
