/**
 * @file test_cnn.cpp
 * @brief KWS test
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "timer.h"
#include "test.h"

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
    // Open channels to the FPGA board.
    // These channels appear as files to the Linux OS
    int fdr = open("/dev/xillybus_read_32", O_RDONLY);
    int fdw = open("/dev/xillybus_write_32", O_WRONLY);

    // Check that the channels are correctly opened
    if ((fdr < 0) || (fdw < 0))
    {
      fprintf(stderr, "Failed to open Xillybus device channels\n");
      exit(-1);
    }

    WAV header;
    uint32_t buffer[8000];
    int32_t res;
    int nbytes;
    FILE *sourcefile;
    Timer my_timer("cnn FPGA", false);
    for(int i = 0; i < test_num; i++) {
        my_timer.start();
        sourcefile=fopen(test_filename[i].c_str(),"rb");
        fread(&header,sizeof(WAV),1,sourcefile);
        fread(&buffer,sizeof(int16_t),16000,sourcefile);
        nbytes = write(fdw, (void *)buffer, sizeof(buffer)); 
	//printf("%d\n",nbytes);
        assert(nbytes == sizeof(buffer));
        nbytes = read(fdr, (void *)&res, sizeof(int32_t));
	printf("%s GT:%d RES:%d\n", test_filename[i].c_str(), test_label[i], res);
        if(res == test_label[i]) {
            counter++;
        }
        fclose(sourcefile);
        my_timer.stop();
    }
    printf("Accuracy: %d/%d=%f\n", counter, test_num, counter/float(test_num));
    return 0;
}
