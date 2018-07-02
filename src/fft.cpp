#include "fft.h"
#ifdef __SYNTHESIS__
void dummy_proc_fe(floatc in[FFT_LENGTH], floatc out[FFT_LENGTH]) {
  int i;
  for (i = 0; i < FFT_LENGTH; i++)
    out[i] = in[i];
}

void dummy_proc_be(floatc in[FFT_LENGTH], floatc out[FFT_LENGTH]) {
  int i;
  for (i = 0; i < FFT_LENGTH; i++)
    out[i] = in[i];
}

void FFT2(floatc in[FFT_LENGTH], floatc out[FFT_LENGTH]) {
#pragma HLS data_pack variable = in
#pragma HLS dataflow
  floatc xn[FFT_LENGTH];
  floatc xk[FFT_LENGTH];
  config_t fft_config;
  status_t fft_status;
  dummy_proc_fe(in, xn);
  // FFT IP
  hls::fft<config1>(xn, xk, &fft_status, &fft_config);
  dummy_proc_be(xk, out);
}

void FFT(floatc in[FFT_LENGTH]) {
  floatc out[FFT_LENGTH];
  FFT2(in, out);
  for (int i = 0; i < FFT_LENGTH; i++)
    in[i] = out[i];
}
#else

float vecW_r[FFT_LENGTH / 2] = {
#include "vecW_re.dat"
};
float vecW_i[FFT_LENGTH / 2] = {
#include "vecW_im.dat"
};

void FFT2(floatc X[FFT_LENGTH], floatc Y[FFT_LENGTH]) {
  FFT(X);
  for (int i = 0; i < FFT_LENGTH; i++)
    Y[i] = X[i];
}

void FFT(floatc X[FFT_LENGTH]) {
  int Head, length, temp1, i, j, k;
  floatc tmp[2];
  floatc temp[FFT_LENGTH];
  float w_r, w_i;
  ffloat vec_r[FFT_LENGTH], vec_i[FFT_LENGTH];
  int index, input_1, input_2;
  for (i = 0; i < FFT_LENGTH; i++) {
    if (i < 512) {
      vec_r[i] = vecW_r[i];
      vec_i[i] = vecW_i[i];
    } else {
      vec_r[i] = -vecW_r[i - 512];
      vec_i[i] = -vecW_i[i - 512];
    }
  }

  for (i = 0; i < 10; i++) {
    temp1 = 1 << i;
    length = FFT_LENGTH / temp1;
    // j indicates number of sub-operations within specific stage
    for (j = 0; j < temp1; j++) {
      Head = length * j;
      {
        for (k = 0; k < (length / 2); k++) {
          index = k * (1 << i);
          // Exploring The symmetries of twiddle factors
          w_r = vec_r[index];
          w_i = vec_i[index];
          input_1 = k + Head;
          input_2 = input_1 + (length / 2);

          tmp[0] = X[input_1] + X[input_2];
          tmp[1] = (X[input_1] - X[input_2]) * floatc(w_r, w_i);
          X[input_1] = tmp[0];
          X[input_2] = tmp[1];
        }
      }
    }
  }
  // Reorder the bits after FFT computation
  for (i = 0; i < FFT_LENGTH; i++) {
    index = (i & 0x01) << 9 | (i & 0x02) << 7 | (i & 0x04) << 5 |
            (i & 0x08) << 3 | (i & 0x10) << 1 | (i & 0x20) >> 1 |
            (i & 0x40) >> 3 | (i & 0x80) >> 5 | (i & 0x100) >> 7 |
            (i & 0x200) >> 9; //| (i&0x400)>>10 ;
    temp[index] = X[i];
  }
  for (i = 0; i < FFT_LENGTH; i++) {
    X[i] = temp[i];
  }
}

#endif
