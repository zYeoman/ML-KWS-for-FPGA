/**
 * @file kws_hw.cpp
 * @brief KWS hardware test
 * @author Yongwen Zhuang
 * @version 0.1
 * @date 2018-06-28
 */

#include "cnn_hw.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void kws_hw(uint32_t *input, int32_t *output) {
  static int fdr = open("/dev/xillybus_read_32", O_RDONLY);
  static int fdw = open("/dev/xillybus_write_32", O_WRONLY);

  // Check that the channels are correctly opened
  if ((fdr < 0) || (fdw < 0)) {
    fprintf(stderr, "Failed to open Xillybus device channels\n");
    exit(-1);
  }

  write(fdw, (void *)input, sizeof(input));
  read(fdr, (void *)&output, sizeof(int32_t));
}
