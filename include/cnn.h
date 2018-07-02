/**
 * @file cnn.h
 * @brief Use CNN for key word spotting
 * @version 0.1
 * @date 2018-06-28
 */

#ifndef CNN_H
#define CNN_H

#include "typedef.h"
void kws(uint32_t *data_in, int32_t *data_out);
void bn(const ffloat *gamma, const ffloat *beta, const ffloat *mm,
        const ffloat *mv, float *data, int h, int w, int c);
void dense(const ffloat *weight, const ffloat *bias, float *data,
           float *output_data, int i_num, int o_num, bool relu);
void conv(const ffloat *weight, const ffloat *bias, float *data,
          float *output_data, int k_w, int k_h, int i_w, int i_h, int k_c,
          int o_c, bool relu, int h_s, int w_s);
float relu(float x);
#endif // CNN_H
