#ifndef MODEL_CRNN_S_H
#define MODEL_CRNN_S_H

float crnn_conv1_w[48*10*4] = {
	#include "CRNN_S/conv1_weights.dat"
};

float crnn_conv1_b[48] = {
	#include "CRNN_S/conv1_bias.dat"
};

float crnn_gru1_gate_w[252*120] = {
	#include "CRNN_S/gru1_gates_weights.dat"
};

float crnn_gru1_gate_b[120] = {
	#include "CRNN_S/gru1_gates_bias.dat"
};

float crnn_gru1_cand_w[252*60] = {
	#include "CRNN_S/gru1_candidate_weights.dat"
};

float crnn_gru1_cand_b[60] = {
	#include "CRNN_S/gru1_candidate_bias.dat"
};

float crnn_gru2_gate_w[120*120] = {
	#include "CRNN_S/gru2_gates_weights.dat"
};

float crnn_gru2_gate_b[120] = {
	#include "CRNN_S/gru2_gates_bias.dat"
};

float crnn_gru2_cand_w[120*60] = {
	#include "CRNN_S/gru2_candidate_weights.dat"
};

float crnn_gru2_cand_b[60] = {
	#include "CRNN_S/gru2_candidate_bias.dat"
};

float crnn_fc1_w[60*84] = {
	#include "CRNN_S/fc1_weights.dat"
};

float crnn_fc1_b[84] = {
	#include "CRNN_S/fc1_bias.dat"
};

float crnn_fc2_w[84*12] = {
	#include "CRNN_S/fc2_weights.dat"
};

float crnn_fc2_b[12] = {
	#include "CRNN_S/fc2_bias.dat"
};

#endif
