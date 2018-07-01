/**
 * @file crnn.h
 * @brief CRNN
 * @version 0.1
 * @date 2018-06-28
 */

#ifndef __KWS_CRNN_H__
#define __KWS_CRNN_H__

#include <math.h>

#include "typedef.h"
#include "mfcc.h"
#include "cnn.h"
void kws_crnn(uint32_t* data_in, int32_t* data_out);
void gru(const float* gate_weights, const float* gate_bias, const float* candidate_weights, const float* candidate_bias, float* data, float* output_data, int unit, int input, int length);

#endif /* !CRNN_H */
