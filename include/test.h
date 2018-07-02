#ifndef TEST_H
#define TEST_H

#define TEST_NUM 3081

typedef struct // WAV stores wave file header
{
  int32_t rId;
  int32_t rLen;
  int32_t wId;
  int32_t fId;

  int32_t fLen;

  int16_t wFormatTag;
  int16_t nChannels;
  int32_t nSamplesPerSec;  // Sampling rate. This returns to FS variable
  int32_t nAvgBytesPerSec; // All other parameters are not used in processing
  int16_t nBlockAlign;
  int16_t wBitsPerSample;
  int32_t dId;
  int32_t wSampleLength;
} WAV;

#endif // TEST_H
