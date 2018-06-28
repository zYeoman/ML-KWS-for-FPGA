#
# Makefile
# Yongwen Zhuang, 2018-06-28 13:48
#

all: mfcc
	./mfcc 1.wav

mfcc: mfcc-official.cpp mfcc-official.h main.cpp timer.h
	g++ main.cpp mfcc-official.cpp -o mfcc


# vim:ft=make
#
