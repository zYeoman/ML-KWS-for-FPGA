#include <stdint.h>
#include <math.h>

#include "model.h"
#include "timer.h"
#include "mfcc-official.h"

#define TESTING_SIZE 1

int compute(MFCC& mfccComputer, char* filename, float* output, int frame_len=640, int frame_shift=320) {
    int j=0;
    int16_t* buffer = new int16_t[frame_len];
    WAV header;
    FILE *sourcefile;
    sourcefile=fopen(filename,"rb");
    fread(&header,sizeof(WAV),1,sourcefile);
    while(1) {
        if(fread(buffer,sizeof(int16_t),frame_len,sourcefile) == (size_t)frame_len)
            mfccComputer.mfcc_compute(buffer, output+10*j);
        else
            break;
        fseek(sourcefile, (frame_shift-frame_len)*sizeof(int16_t), SEEK_CUR);
        j++;
    }
    fclose(sourcefile);
    delete []buffer;
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
                if(relu && output_data[i_o_h*o_w*o_c + i_o_w*o_c + i_o_c] < 0) {
                    output_data[i_o_h*o_w*o_c + i_o_w*o_c + i_o_c] = 0;
                }
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
                data[index] = value>0?value:0;
            }
        }
    }
}
int main(int argc, char *argv[]){
    int counter = 0;
    // Assign variables
    int numCepstra = 10;
    int samplingRate = 16000;
    int winLength = 40;
    int frameShift = 20;
    int frame_len = winLength*samplingRate/1000;
    int frame_shift = frameShift*samplingRate/1000;

    MFCC mfccComputer (numCepstra, frame_len, 8);
    Timer my_timer("timer", true);
    for(int i = 0; i < TESTING_SIZE; i++) {
        float input[49*10];
        compute(mfccComputer, argv[1], input, frame_len, frame_shift);
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
        for(int o = 0; o < 10; o++) {
            if(dense2[o] > output_max) {
                output_max = dense2[o];
                output_label = o;
            }
        }
        printf("Label: %d\n", output_label);
        /* if(output_label == testing_label[i]) { */
        /*     counter++; */
        /* } */
    }
    my_timer.stop();
    printf("Accuracy: %d/%d\n", counter, TESTING_SIZE);
    return 0;
}
