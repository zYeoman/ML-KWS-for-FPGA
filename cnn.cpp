/**
 * @file cnn.cpp
 * @brief Use CNN for key word spotting
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */

#include "cnn.h"
#include "model.h"

#define TESTING_SIZE 1


void kws(int16_t *data_in, int16_t *data_out){
#pragma HLS interface axis port=data_in
#pragma HLS interface axis port=data_out
#pragma HLS interface ap_ctrl_none port=return
    int16_t buffer[16000];
    float input[49*10];
    for (int i=0;i<16000;i++)
        buffer[i] = *data_in++;
    compute(buffer, input);
    float conv1[40*7*28];
    conv(conv1_w, conv1_b, input, conv1, 4, 10, 10, 49, 1, 28, false, 1, 1);
    bn(bn1_gamma, bn1_beta, bn1_mm, bn1_mv, conv1, 40, 7, 28);
    float conv2[16*4*30];
    conv(conv2_w, conv2_b, conv1, conv2, 4, 10, 7, 40, 28, 30, false, 2, 1);
    bn(bn2_gamma, bn2_beta, bn2_mm, bn2_mv, conv2, 16, 4, 30);
    // float flatten[576];
    // reshape(conv2, flatten);
    float linear[16];
    dense(linear_w, linear_b, conv2, linear, 1920, 16, false);
    float dense1[128];
    dense(fc1_w, fc1_b, linear, dense1, 16, 128, false);
    bn(bn3_gamma, bn3_beta, bn3_mm, bn3_mv, dense1, 1, 1, 128);
    float dense2[12];
    dense(fc2_w, fc2_b, dense1, dense2, 128, 12, false);
    float output_max = -1e10;
    int output_label = 0;
    for(int o = 0; o < 12; o++) {
        if(dense2[o] > output_max) {
            output_max = dense2[o];
            output_label = o;
        }
    }
    *data_out++=output_label;
}

int compute(int16_t* input, float* output) {
    for(int j=0;j<49;j++) {
        mfcc_compute(input+320*j, output+10*j);
    }
    return 0;
}

//weights: HWCN
//activations: HWC
void conv(const float* weight, const float* bias, float* data, float* output_data,
    int k_w, int k_h, int i_w, int i_h, int k_c, int o_c, bool relu, int h_s, int w_s) {
    int o_w = (int)floor((float)(i_w - k_w) / (float)w_s) + 1;
    int o_h = (int)floor((float)(i_h - k_h) / (float)h_s) + 1;
    int o_num = o_w*o_h*o_c;
    for(int i = 0; i < o_num; i++) {
        output_data[i] = 0;
    }
    for(int i_o_c = 0; i_o_c < o_c; i_o_c++) {
        for(int i_o_h = 0; i_o_h < o_h; i_o_h++) {
            for(int i_o_w = 0; i_o_w < o_w; i_o_w++) {
                for(int i_k_c = 0; i_k_c < k_c; i_k_c++) {
                    for(int i_k_h = 0; i_k_h < k_h; i_k_h++) {
                        for(int i_k_w = 0; i_k_w < k_w; i_k_w++) {
                            output_data[i_o_h*o_w*o_c + i_o_w*o_c + i_o_c]
                                += weight[i_k_h*k_w*k_c*o_c+i_k_w*k_c*o_c+i_k_c*o_c+i_o_c] *
                                data[((i_o_h)*h_s+i_k_h)*i_w*k_c+((i_o_w)*w_s+i_k_w)*k_c+i_k_c];
                        }
                    }
                }
                output_data[i_o_h*o_w*o_c + i_o_w*o_c + i_o_c] += bias[i_o_c];
            }
        }
    }
}

//weight:IO
void dense(const float* weight, const float* bias, float* data, float* output_data,
    int i_num, int o_num, bool relu) {
    for(int o = 0; o < o_num; o++) {
        output_data[o] = 0;
        for(int i = 0; i < i_num; i++) {
            output_data[o] += data[i] * weight[i*o_num+o];
        }
        output_data[o] += bias[o];
        if(relu && output_data[o] < 0) {
            output_data[o] = 0;
        }
    }
}

//activation:HWC
void bn(const float* gamma, const float* beta, const float* mm, const float* mv,
    float* data, const int h, const int w, const int c) {
    float epsilon = 0.001;
    for(int c_i = 0; c_i < c; c_i++) {
        for(int h_i = 0; h_i < h; h_i++) {
            for(int w_i = 0; w_i < w; w_i++) {
                int index = h_i*w*c+w_i*c+c_i;
                float value = gamma[c_i]*(data[index]
                 - mm[c_i])/sqrt(mv[c_i] + epsilon) + beta[c_i];
                data[index] = relu(value);
            }
        }
    }
}

float relu(float x) {
#pragma HLS inline
    return x>0 ? x : 0;
}
