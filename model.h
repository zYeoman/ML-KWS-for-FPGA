#ifndef MODEL_H
#define MODEL_H

//conv1:10*4*1*28 CNN_S
const float conv1_w[10*4*1*28] = { 
  #include "CNN_S/conv1_weights.dat" 
};

const float conv1_b[28] = { 
  #include "CNN_S/conv1_bias.dat" 
};

const float bn1_gamma[28] = { 
  #include "CNN_S/bn1_gamma.dat" 
};

const float bn1_beta[28] = { 
  #include "CNN_S/bn1_beta.dat" 
};

const float bn1_mm[28] = { 
  #include "CNN_S/bn1_moving_mean.dat" 
};

const float bn1_mv[28] = { 
  #include "CNN_S/bn1_moving_variance.dat" 
};

//conv2:10*4*28*30 CNN_S
const float conv2_w[10*4*28*30] = { 
  #include "CNN_S/conv2_weights.dat" 
};

const float conv2_b[30] = { 
  #include "CNN_S/conv2_bias.dat" 
};

const float bn2_gamma[30] = { 
  #include "CNN_S/bn2_gamma.dat" 
};

const float bn2_beta[30] = { 
  #include "CNN_S/bn2_beta.dat" 
};

const float bn2_mm[30] = { 
  #include "CNN_S/bn2_moving_mean.dat" 
};

const float bn2_mv[30] = { 
  #include "CNN_S/bn2_moving_variance.dat" 
};

//linear:1920*16 CNN_S
const float linear_w[1920*16] = { 
  #include "CNN_S/linear_weights.dat" 
};

const float linear_b[16] = { 
  #include "CNN_S/linear_bias.dat" 
};

//fc1:16*128 CNN_S
const float fc1_w[16*128] = { 
  #include "CNN_S/fc1_weights.dat" 
};

const float fc1_b[128] = { 
  #include "CNN_S/fc1_bias.dat" 
};

const float bn3_gamma[128] = { 
  #include "CNN_S/bn3_gamma.dat" 
};

const float bn3_beta[128] = { 
  #include "CNN_S/bn3_beta.dat" 
};

const float bn3_mm[128] = { 
  #include "CNN_S/bn3_moving_mean.dat" 
};

const float bn3_mv[128] = { 
  #include "CNN_S/bn3_moving_variance.dat" 
};

//fc2:128*12 CNN_S
const float fc2_w[128*12] = { 
  #include "CNN_S/fc2_weights.dat" 
};

const float fc2_b[12] = { 
  #include "CNN_S/fc2_bias.dat" 
};

#endif
