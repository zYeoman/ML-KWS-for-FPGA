# ML-KWS-for-FPGA

嵌入式计算系统及SoC软硬件协同设计课程大作业。

## Usage
1. Download [Google speech command dataset](http://download.tensorflow.org/data/speech_commands_v0.01.tar.gz)
2. Put the dataset outside this project, specifically, at `../speech/`
3. `git clone https://github.com/zYeoman/ML-KWS-for-FPGA`
4. CD in ML-KWS-for-FPGA and make and run kws

```bash
wget http://download.tensorflow.org/data/speech_commands_v0.01.tar.gz
tar xzvf speech_commands_v0.0.1.tar.gz speech
git clone https://github.com/zYeoman/ML-KWS-for-FPGA && cd ML-KWS-for-FPGA
make
# Run CNN model for the first 10 test.
./kws cnn 10
# Run CRNN model for all test
./kws crnn
# Run CNN_Q(Quantization CNN)
./kws cnn_q
```

## Hardware
In Vivado HLS, use `kws(uint32_t*, int32_t*)` or `kws_q` or `kws_crnn` as top function, `test.cpp` as test source.
In Vivado, Open [xillinux](http://xillybus.com/xillinux) blockdesign, and connect the IP core and Xillybus as shown belown.

![Connect](https://i.loli.net/2018/07/08/5b41ad2f2dd2b.png)

## Files

```
./bit           Bitstream
./Makefile      Makefile
./silence.wav   Testfile
./include       Header And Model Parameters
./src           Source
```
