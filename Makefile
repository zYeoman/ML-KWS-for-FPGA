#
# Makefile
# Yongwen Zhuang, 2018-06-28 13:48
#

all: mfcc.cpp mfcc.h cnn.cpp cnn.h timer.h test_cnn.cpp kws
	./kws

kws: mfcc.cpp mfcc.h cnn.cpp cnn.h timer.h test_cnn.cpp
	g++ cnn.cpp mfcc.cpp test_cnn.cpp -o kws -O3

# vim:ft=make
#
