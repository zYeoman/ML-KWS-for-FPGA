#ifndef MODEL_CNN_Q_H
#define MODEL_CNN_Q_H

//conv1:10*4*1*28 CNN_S_Q
const int8_t conv1_w_q[10*4*1*28] = {
  #include "CNN_S_Q/conv1_weights_q.dat"
};

const int8_t conv1_b_q[28] = {
  #include "CNN_S_Q/conv1_bias_q.dat"
};

const int8_t bn1_m1_q[28] = {
  #include "CNN_S_Q/bn1_m1.dat"
};

const int8_t bn1_m2_q[28] = {
  #include "CNN_S_Q/bn1_m2.dat"
};

//conv2:10*4*28*30 CNN_S_Q
const int8_t conv2_w_q[10*4*28*30] = {
  #include "CNN_S_Q/conv2_weights_q.dat"
};

const int8_t conv2_b_q[30] = {
  #include "CNN_S_Q/conv2_bias_q.dat"
};

const int8_t bn2_m1_q[30] = {
  #include "CNN_S_Q/bn2_m1.dat"
};

const int8_t bn2_m2_q[30] = {
  #include "CNN_S_Q/bn2_m2.dat"
};

//linear:1920*16 CNN_S_Q
const int8_t linear_w_q[1920*16] = {
  #include "CNN_S_Q/linear_weights_q.dat"
};

const int8_t linear_b_q[16] = {
  #include "CNN_S_Q/linear_bias_q.dat"
};

//fc1:16*128 CNN_S_Q
const int8_t fc1_w_q[16*128] = {
  #include "CNN_S_Q/fc1_weights_q.dat"
};

const int8_t fc1_b_q[128] = {
  #include "CNN_S_Q/fc1_bias_q.dat"
};

const int8_t bn3_m1_q[128] = {
  #include "CNN_S_Q/bn3_m1.dat"
};

const int8_t bn3_m2_q[128] = {
  #include "CNN_S_Q/bn3_m2.dat"
};

//fc2:128*12 CNN_S_Q
const int8_t fc2_w_q[128*12] = {
  #include "CNN_S_Q/fc2_weights_q.dat"
};

const int8_t fc2_b_q[12] = {
  #include "CNN_S_Q/fc2_bias_q.dat"
};

#endif
