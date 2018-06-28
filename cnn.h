/*
 * cnn.h
 * Copyright (C) 2018 Yongwen Zhuang <zeoman@163.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CNN_H
#define CNN_H

#include <stdint.h>

#ifdef __SYNTHESIS__
#include "hls_math.h"
#else
#include <math.h>
#endif

#include"mfcc.h"

void kws(int16_t* data_in, int16_t* data_out);
void bn(const float* gamma, const float* beta, const float* mm, const float* mv,
    float* data, const int h, const int w, const int c);
void dense(const float* weight, const float* bias, float* data, float* output_data,
    int i_num, int o_num, bool relu);
void conv(const float* weight, const float* bias, float* data, float* output_data,
    int k_w, int k_h, int i_w, int i_h, int k_c, int o_c, bool relu, int h_s, int w_s);
int compute(int16_t* input, float* output);
float relu(float x);
#endif /* !CNN_H */
