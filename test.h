#ifndef __KWS_TEST_H_
#define __KWS_TEST_H_

#include <string>

#define TEST_NUM 3081

const std::string test_filename[TEST_NUM] = {
    #include "test_filename.dat"
};

const int test_label[TEST_NUM] = {
    #include "test_label.dat"
};

#endif
