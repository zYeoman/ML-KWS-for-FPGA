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

#define TESTING_SIZE 1
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

int main(int argc, char *argv[]){
    int counter = 0;
    // Assign variables
    int numCepstra = 10;
    int samplingRate = 16000;
    int winLength = 40;
    int frameShift = 20;
    int frame_len = winLength*samplingRate/1000;
    int frame_shift = frameShift*samplingRate/1000;

    WAV header;
    int16_t buffer[16000];
    FILE *sourcefile;
    sourcefile=fopen("1.wav","rb");
    fread(&header,sizeof(WAV),1,sourcefile);
    fread(&buffer,sizeof(int16_t),16000,sourcefile);
    Timer my_timer("timer", true);
    for(int i = 0; i < TESTING_SIZE; i++) {
        int16_t res;
        kws(buffer, &res);
        printf("Label: %d\n", res);
        /* if(output_label == testing_label[i]) { */
        /*     counter++; */
        /* } */
    }
    my_timer.stop();
    printf("Accuracy: %d/%d\n", counter, TESTING_SIZE);
    return 0;
}
