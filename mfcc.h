/*
 * Copyright (C) 2018 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __KWS_MFCC_H__
#define __KWS_MFCC_H__

#ifdef __SYNTHESIS__
#include "hls_math.h"
#else
#include <math.h>
#endif
#include <string.h>
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
