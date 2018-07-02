/**
 * @file test_cnn.cpp
 * @brief KWS test
 * @version 0.1
 * @date 2018-06-28
 */

#include "cnn.h"
#include "cnn_hw.h"
#include "cnn_q.h"
#include "crnn.h"
#include "test.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *test_filename[TEST_NUM] = {
#include "test_filename.dat"
};

const int test_label[TEST_NUM] = {
#include "test_label.dat"
};

int main(int argc, char *argv[]) {
  int counter = 0;
  int test_num = TEST_NUM;
  char *model;
  void (*kws_p)(uint32_t *, int32_t *) = NULL;
  if (argc == 2) {
    if (isdigit(argv[1][0])) {
      test_num = atoi(argv[1]);
    } else
      model = argv[1];
  } else if (argc == 3) {
    test_num = atoi(argv[2]);
    model = argv[1];
  }
  if (!strcmp(model, "crnn")) {
    kws_p = &kws_crnn;
    printf("Calculate Use CRNN\n");
  } else if (!strcmp(model, "cnn_q")) {
    kws_p = &kws_q;
    printf("Calculate Use QUART CNN\n");
  } else if (!strcmp(model, "hw")) {
    kws_p = &kws_hw;
    printf("Calculate Use Hardware\n");
  } else {
    kws_p = &kws;
    printf("Calculate Use CNN\n");
  }

  // Assign variables

  WAV header;
  uint32_t buffer[8000];
  int32_t res;
  FILE *sourcefile;
  Timer my_timer("timer", false);
  for (int i = 0; i < test_num; i++) {
    my_timer.start();
    sourcefile = fopen(test_filename[i], "rb");
    if (!fread(&header, sizeof(WAV), 1, sourcefile)) {
      printf("READ %s HEAD FAILURE\n", test_filename[i]);
      return 1;
    }
    if (!fread(&buffer, sizeof(int16_t), 16000, sourcefile)) {
      printf("READ %s CONTENT FAILURE\n", test_filename[i]);
      return 1;
    }
    kws_p(buffer, &res);
    if (res == test_label[i]) {
      counter++;
    }
    fclose(sourcefile);
    my_timer.stop();
  }
  printf("Accuracy: %d/%d=%f\n", counter, test_num, counter / float(test_num));
  return 0;
}
