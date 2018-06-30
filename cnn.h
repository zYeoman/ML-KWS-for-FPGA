/**
 * @file cnn.h
 * @brief Use CNN for key word spotting
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */

#ifndef __KWS_CNN_H_
#define __KWS_CNN_H_

#include <math.h>

#include "typedef.h"
#include "mfcc.h"
void kws(uint32_t* data_in, int32_t* data_out);
void kws_q(uint32_t *data_in, int32_t *data_out);
void bn(const ffloat* gamma, const ffloat* beta, const ffloat* mm, const ffloat* mv,
    float* data, const int h, const int w, const int c);
void dense(const ffloat* weight, const ffloat* bias, float* data, float* output_data,
    int i_num, int o_num, bool relu);
void conv(const ffloat* weight, const ffloat* bias, float* data, float* output_data, int k_w, int k_h, int i_w, int i_h, int k_c, int o_c, bool relu, int h_s, int w_s);
void conv_bn_q(const int8_t* weight, const int8_t* bias, const int8_t* bn_m1, const int8_t* bn_m2, int8_t* data, int8_t* output_data_q, int k_w, int k_h, int i_w, int i_h, int k_c, int o_c, bool relu, int h_s, int w_s, int input_q, int weight_q, int bias_q, int out_q, int m1_q, int m2_q);
void dense_q(const int8_t* weight, const int8_t* bias, int8_t* data, int8_t* output_data_q,
    int i_num, int o_num, bool relu, int input_q, int out_q, int weight_q, int bias_q);
void dense_bn_q(const int8_t* weight, const int8_t* bias, int8_t* data, int8_t* output_data_q, const int8_t* bn_m1, const int8_t* bn_m2, int m1_q, int m2_q, int i_num, int o_num, bool relu, int input_q, int out_q, int weight_q, int bias_q);
int compute(int16_t* input, float* output);
float relu(float x);
#endif /* !CNN_H */
