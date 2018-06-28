/**
 * @file mfcc.h
 * @brief Extract MFCC feature from 16bit 16000 samplie rate 640 length wave.
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */
#ifndef __KWS_MFCC_H__
#define __KWS_MFCC_H__

#ifdef __SYNTHESIS__
#include "hls_math.h"
#else
#include <math.h>
#endif
#include <string.h>
#include <float.h>
#include <complex>
#include <vector>

using namespace std;

#define SAMP_FREQ 16000
#define NUM_FBANK_BINS 40
#define NUM_FBANK_MAXW 32
#define MEL_LOW_FREQ 20
#define MEL_HIGH_FREQ 4000
#define NUM_MFCC_FEATURES 10
#define FILTER_LEN 1024
#define FRAME_LEN 640

#define M_2PI 6.283185307179586476925286766559005

const float window_func[FRAME_LEN]={
    #include "hamm_window.dat"
};
const int32_t fbank_filter_first[NUM_FBANK_BINS]={
    #include "mel_first_index.dat"
};
const int32_t fbank_filter_last[NUM_FBANK_BINS]={
    #include "mel_last_index.dat"
};
const float mel_fbank[NUM_FBANK_BINS*NUM_FBANK_MAXW]={
    #include "mel_fbank.dat"
};
const float dct_matrix[NUM_FBANK_BINS*NUM_MFCC_FEATURES]={
    #include "dct_matrix.dat"
};

void mfcc_compute(const int16_t* data, float* mfcc_out);

#endif
