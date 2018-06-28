/**
 * @file test_cnn.cpp
 * @brief KWS test
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */

#include <stdio.h>
#include "cnn.h"
#include "timer.h"
#include "test.h"

#define DEBUG 0
#define D \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): \n", __FILE__, \
        __LINE__, __func__); } while (0)

typedef struct     // WAV stores wave file header
{
        int rId;
        int rLen;
        int wId;
        int fId;

        int fLen;

        short wFormatTag;
        short nChannels;
        int nSamplesPerSec;   // Sampling rate. This returns to FS variable
        int nAvgBytesPerSec;  // All other parameters are not used in processing
        short nBlockAlign;
        short wBitsPerSample;
        int dId;
        int wSampleLength;
}WAV;

int main(){
    int counter = 0;
    // Assign variables

    WAV header;
    int16_t buffer[16000];
    FILE *sourcefile;
    Timer my_timer("timer", false);
    for(int i = 0; i < TEST_NUM; i++) {
        my_timer.start();
        sourcefile=fopen(test_filename[i].c_str(),"rb");
        fread(&header,sizeof(WAV),1,sourcefile);
        fread(&buffer,sizeof(int16_t),16000,sourcefile);
        int16_t res;
        kws(buffer, &res);
        if(res == test_label[i]) {
            counter++;
        }
        fclose(sourcefile);
        my_timer.stop();
    }
    printf("Accuracy: %d/%d=%f\n", counter, TEST_NUM, counter/float(TEST_NUM));
    return 0;
}
