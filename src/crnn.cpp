/**
 * @file crnn.cpp
 * @brief CRNN
 * @version 0.1
 * @date 2018-06-28
 */

#include "crnn.h"
#include "mfcc.h"
#include "model_crnn_s.h"

void kws_crnn(uint32_t *data_in, int32_t *data_out) {
#pragma HLS interface axis port = data_in depth = 128
#pragma HLS interface axis port = data_out depth = 1
#pragma HLS interface ap_ctrl_none port = return
  int16_t buffer[16000];
  float input[49 * 10];
  uint32_t tmp;
  for (int i = 0; i < 8000; i++) {
    tmp = *data_in++;
    buffer[2 * i] = (int16_t)(tmp & 0xFFFF);
    buffer[2 * i + 1] = (int16_t)(tmp >> 16);
  }
  compute(buffer, input);

  float conv1[20 * 4 * 48];
  conv(crnn_conv1_w, crnn_conv1_b, input, conv1, 4, 10, 10, 49, 1, 48, true, 2,
       2);
  float gru1[20 * 60];
  gru(crnn_gru1_gate_w, crnn_gru1_gate_b, crnn_gru1_cand_w, crnn_gru1_cand_b,
      conv1, gru1, 60, 192, 20);
  float gru2[20 * 60];
  gru(crnn_gru2_gate_w, crnn_gru2_gate_b, crnn_gru2_cand_w, crnn_gru2_cand_b,
      gru1, gru2, 60, 60, 20);
  float dense1[84];
  dense(crnn_fc1_w, crnn_fc1_b, gru2 + 19 * 60, dense1, 60, 84, true);
  float dense2[12];
  dense(crnn_fc2_w, crnn_fc2_b, dense1, dense2, 84, 12, false);
  float output_max = -1e10;
  int32_t output_label = 0;
  for (int o = 0; o < 12; o++) {
    if (dense2[o] > output_max) {
      output_max = dense2[o];
      output_label = o;
    }
  }
  *data_out++ = output_label;
}

void gru(const float *gate_weights, const float *gate_bias,
         const float *candidate_weights, const float *candidate_bias,
         float *data, float *output_data, int unit, int input, int length) {
  float state[unit];
  for (int i = 0; i < unit; i++) {
    state[i] = 0;
  }
  float r_t[unit];
  float z_t[unit];
  float h_t[unit];
  for (int l = 0; l < length; l++) {
    for (int o = 0; o < unit; o++) {
      r_t[o] = 0;
      z_t[o] = 0;
      for (int i = 0; i < unit; i++) {
        r_t[o] += state[i] * gate_weights[(i + input) * 2 * unit + o];
        z_t[o] += state[i] * gate_weights[(i + input) * 2 * unit + unit + o];
      }
      for (int i = 0; i < input; i++) {
        r_t[o] += data[i + l * input] * gate_weights[i * 2 * unit + o];
        z_t[o] += data[i + l * input] * gate_weights[i * 2 * unit + unit + o];
      }
      r_t[o] = 1. / (1. + exp(-(r_t[o] + gate_bias[o])));
      z_t[o] = 1. / (1. + exp(-(z_t[o] + gate_bias[unit + o])));
    }
    for (int o = 0; o < unit; o++) {
      h_t[o] = 0;
      for (int i = 0; i < unit; i++) {
        h_t[o] += state[i] * r_t[i] * candidate_weights[(i + input) * unit + o];
      }
      for (int i = 0; i < input; i++) {
        h_t[o] += data[i + l * input] * candidate_weights[i * unit + o];
      }
      h_t[o] += candidate_bias[o];
      h_t[o] = (1. - exp(-2 * h_t[o])) / (1. + exp(-2 * h_t[o]));
    }
    for (int o = 0; o < unit; o++) {
      state[o] = z_t[o] * state[o] + (1 - z_t[o]) * h_t[o];
      output_data[o + l * unit] = state[o];
    }
  }
}
