/**
 * @file test_cnn.cpp
 * @brief KWS test
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cnn.h"
#include "timer.h"
#include "test.h"

#define DEBUG 0
#define D \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): \n", __FILE__, \
        __LINE__, __func__); } while (0)

typedef struct     // WAV stores wave file header
{
        int32_t rId;
        int32_t rLen;
        int32_t wId;
        int32_t fId;

        int32_t fLen;

        int16_t wFormatTag;
        int16_t nChannels;
        int32_t nSamplesPerSec;   // Sampling rate. This returns to FS variable
        int32_t nAvgBytesPerSec;  // All other parameters are not used in processing
        int16_t nBlockAlign;
        int16_t wBitsPerSample;
        int32_t dId;
        int32_t wSampleLength;
}WAV;
int main(int argc, char *argv[]) {
    int counter = 0;
    int test_num = TEST_NUM;
    if (argc > 1)
        test_num = atoi(argv[1]);
    // Assign variables

    WAV header;
    uint32_t buffer[8000];
    int32_t res;
    FILE *sourcefile;
    Timer my_timer("timer", false);
    for(int i = 0; i < test_num; i++) {
        my_timer.start();
        sourcefile=fopen(test_filename[i].c_str(),"rb");
        if(!fread(&header,sizeof(WAV),1,sourcefile)) {
            printf("READ %s HEAD FAILURE\n", test_filename[i].c_str());
            return 1;
        }
        if(!fread(&buffer,sizeof(int16_t),16000,sourcefile)) {
            printf("READ %s CONTENT FAILURE\n", test_filename[i].c_str());
            return 1;
        }
        kws_q(buffer, &res);
        if(res == test_label[i]) {
            counter++;
        }
        fclose(sourcefile);
        my_timer.stop();
    }
    printf("Accuracy: %d/%d=%f\n", counter, test_num, counter/float(test_num));
    return 0;
}
