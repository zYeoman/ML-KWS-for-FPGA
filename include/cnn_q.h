/**
 * @file cnn_q.h
 * @brief Use CNN for key word spotting
 * @version 0.1
 * @date 2018-06-28
 */

#ifndef CNN_Q_H
#define CNN_Q_H

#include "typedef.h"
void kws_q(uint32_t *data_in, int32_t *data_out);
void conv_bn_q(const int8_t *weight, const int8_t *bias, const int8_t *bn_m1,
               const int8_t *bn_m2, int8_t *data, int8_t *output_data_q,
               int k_w, int k_h, int i_w, int i_h, int k_c, int o_c, bool relu,
               int h_s, int w_s, int input_q, int weight_q, int bias_q,
               int out_q, int m1_q, int m2_q);
void dense_q(const int8_t *weight, const int8_t *bias, int8_t *data,
             int8_t *output_data_q, int i_num, int o_num, bool relu,
             int input_q, int out_q, int weight_q, int bias_q);
void dense_bn_q(const int8_t *weight, const int8_t *bias, int8_t *data,
                int8_t *output_data_q, const int8_t *bn_m1, const int8_t *bn_m2,
                int m1_q, int m2_q, int i_num, int o_num, bool relu,
                int input_q, int out_q, int weight_q, int bias_q);
#endif // CNN_Q_H
