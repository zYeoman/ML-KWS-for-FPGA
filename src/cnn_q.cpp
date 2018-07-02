/**
 * @file cnn_q.cpp
 * @brief Use CNN for key word spotting
 * @version 0.1
 * @date 2018-06-28
 */

#include "cnn_q.h"
#include "mfcc.h"
#include "model_cnn_q.h"

void kws_q(uint32_t *data_in, int32_t *data_out) {
#pragma HLS interface axis port = data_in depth = 128
#pragma HLS interface axis port = data_out depth = 1
#pragma HLS interface ap_ctrl_none port = return
  uint32_t tmp;
  int16_t buffer[16000];
  for (int i = 0; i < 8000; i++) {
    tmp = *data_in++;
    buffer[2 * i] = (int16_t)(tmp & 0xFFFF);
    buffer[2 * i + 1] = (int16_t)(tmp >> 16);
  }
  float finput[49 * 10];
  int8_t input[49 * 10];
  compute(buffer, finput);
  // Input is Qx.mfcc_dec_bits (from quantization step)
  for (int i = 0; i < 49 * 10; ++i) {
    float sum = round(4 * finput[i]);
    if (sum >= 127)
      input[i] = 127;
    else if (sum <= -128)
      input[i] = -128;
    else
      input[i] = (int8_t)sum;
  }
  int8_t conv1[40 * 7 * 28];
  conv_bn_q(conv1_w_q, conv1_b_q, bn1_m1_q, bn1_m2_q, input, conv1, 4, 10, 10,
            49, 1, 28, true, 1, 1, 2, 9, 10, 4, 5, 7);
  int8_t conv2[16 * 4 * 30];
  conv_bn_q(conv2_w_q, conv2_b_q, bn2_m1_q, bn2_m2_q, conv1, conv2, 4, 10, 7,
            40, 28, 30, true, 2, 1, 4, 8, 11, 4, 7, 7);
  int8_t linear[16];
  dense_q(linear_w_q, linear_b_q, conv2, linear, 1920, 16, false, 4, 2, 8, 8);
  int8_t dense1[128];
  dense_bn_q(fc1_w_q, fc1_b_q, linear, dense1, bn3_m1_q, bn3_m2_q, 4, 5, 16,
             128, true, 2, 2, 9, 11);
  int8_t dense2[12];
  dense_q(fc2_w_q, fc2_b_q, dense1, dense2, 128, 12, false, 2, 2, 8, 9);
  int8_t output_max = -128;
  int output_label = 0;
  for (int o = 0; o < 12; o++) {
    if (dense2[o] > output_max) {
      output_max = dense2[o];
      output_label = o;
    }
  }
  *data_out++ = output_label;
}

// weights: HWCN
// activations: HWC
void conv_bn_q(const int8_t *weight, const int8_t *bias, const int8_t *bn_m1,
               const int8_t *bn_m2, int8_t *data, int8_t *output_data_q,
               int k_w, int k_h, int i_w, int i_h, int k_c, int o_c, bool relu,
               int h_s, int w_s, int input_q, int weight_q, int bias_q,
               int out_q, int m1_q, int m2_q) {
  int o_w = (int)floor((float)(i_w - k_w) / (float)w_s) + 1;
  int o_h = (int)floor((float)(i_h - k_h) / (float)h_s) + 1;
  int o_num = o_w * o_h * o_c;
  int output_data[o_num];
  for (int i = 0; i < o_num; i++) {
    output_data[i] = 0;
  }
  for (int i_o_c = 0; i_o_c < o_c; i_o_c++) {
    for (int i_o_h = 0; i_o_h < o_h; i_o_h++) {
      for (int i_o_w = 0; i_o_w < o_w; i_o_w++) {
        for (int i_k_c = 0; i_k_c < k_c; i_k_c++) {
          for (int i_k_h = 0; i_k_h < k_h; i_k_h++) {
            for (int i_k_w = 0; i_k_w < k_w; i_k_w++) {
              output_data[i_o_h * o_w * o_c + i_o_w * o_c + i_o_c] +=
                  weight[i_k_h * k_w * k_c * o_c + i_k_w * k_c * o_c +
                         i_k_c * o_c + i_o_c] *
                  data[((i_o_h)*h_s + i_k_h) * i_w * k_c +
                       ((i_o_w)*w_s + i_k_w) * k_c + i_k_c];
            }
          }
        }
        output_data[i_o_h * o_w * o_c + i_o_w * o_c + i_o_c] +=
            ((input_q + weight_q - bias_q) > 0
                 ? (bias[i_o_c] << (input_q + weight_q - bias_q))
                 : (bias[i_o_c] >> (-input_q - weight_q + bias_q)));
        output_data[i_o_h * o_w * o_c + i_o_w * o_c + i_o_c] *= bn_m1[i_o_c];
        output_data[i_o_h * o_w * o_c + i_o_w * o_c + i_o_c] +=
            ((input_q + weight_q + m1_q - m2_q) > 0
                 ? (bn_m2[i_o_c] << (input_q + weight_q + m1_q - m2_q))
                 : (bn_m2[i_o_c] >> (-input_q - weight_q - m1_q + m2_q)));
        if (relu && output_data[i_o_h * o_w * o_c + i_o_w * o_c + i_o_c] < 0) {
          output_data[i_o_h * o_w * o_c + i_o_w * o_c + i_o_c] = 0;
        }
        output_data_q[i_o_h * o_w * o_c + i_o_w * o_c + i_o_c] =
            (out_q - input_q - weight_q - m1_q) > 0
                ? (output_data[i_o_h * o_w * o_c + i_o_w * o_c + i_o_c]
                   << (out_q - input_q - weight_q - m1_q))
                : (output_data[i_o_h * o_w * o_c + i_o_w * o_c + i_o_c] >>
                   (-out_q + input_q + weight_q + m1_q));
      }
    }
  }
}

// weight:IO
void dense_q(const int8_t *weight, const int8_t *bias, int8_t *data,
             int8_t *output_data_q, int i_num, int o_num, bool relu,
             int input_q, int out_q, int weight_q, int bias_q) {
  int output_data[o_num];
  for (int o = 0; o < o_num; o++) {
    output_data[o] = 0;
    for (int i = 0; i < i_num; i++) {
      output_data[o] += data[i] * weight[i * o_num + o];
    }
    output_data[o] += ((input_q + weight_q - bias_q) > 0
                           ? (bias[o] << (input_q + weight_q - bias_q))
                           : (bias[o] >> (-input_q - weight_q + bias_q)));
    if (relu && output_data[o] < 0) {
      output_data[o] = 0;
    }
    output_data_q[o] = (out_q - input_q - weight_q) > 0
                           ? (output_data[o] << (out_q - input_q - weight_q))
                           : (output_data[o] >> (-out_q + input_q + weight_q));
  }
}

// weight:IO
void dense_bn_q(const int8_t *weight, const int8_t *bias, int8_t *data,
                int8_t *output_data_q, const int8_t *bn_m1, const int8_t *bn_m2,
                int m1_q, int m2_q, int i_num, int o_num, bool relu,
                int input_q, int out_q, int weight_q, int bias_q) {
  int output_data[o_num];
  for (int o = 0; o < o_num; o++) {
    output_data[o] = 0;
    for (int i = 0; i < i_num; i++) {
      output_data[o] += data[i] * weight[i * o_num + o];
    }
    output_data[o] += ((input_q + weight_q - bias_q) > 0
                           ? (bias[o] << (input_q + weight_q - bias_q))
                           : (bias[o] >> (-input_q - weight_q + bias_q)));
    output_data[o] *= bn_m1[o];
    output_data[o] += ((input_q + weight_q + m1_q - m2_q) > 0
                           ? (bn_m2[o] << (input_q + weight_q + m1_q - m2_q))
                           : (bn_m2[o] >> (-input_q - weight_q - m1_q + m2_q)));
    if (relu && output_data[o] < 0) {
      output_data[o] = 0;
    }
    output_data_q[o] =
        (out_q - input_q - weight_q - m1_q) > 0
            ? (output_data[o] << (out_q - input_q - weight_q - m1_q))
            : (output_data[o] >> (-out_q + input_q + weight_q + m1_q));
  }
}
