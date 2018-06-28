#
# Makefile
# Yongwen Zhuang, 2018-06-28 13:48
#

all: mfcc gt.csv
	./mfcc 1.wav > tmp.csv
	diff tmp.csv gt.csv

mfcc: compute-mfcc.cc mfcc-official.cpp mfcc-official.h
	g++ compute-mfcc.cc mfcc-official.cpp -o mfcc --std=c++11


# vim:ft=make
#
