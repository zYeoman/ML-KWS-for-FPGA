#
# Makefile
# Yongwen Zhuang, 2018-06-28 13:48
#

TARGET = kws
TARGET_HW = kws_hw
LIBS = -lm
CC = g++
CFLAGS = -Wall -O3

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = cnn.o crnn.o mfcc.o fft.o
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

test:
	$(CC) $(CFLAGS) -c test.cpp -o test.o

test_q:
	$(CC) $(CFLAGS) -D QUAT -c test.cpp -o test_q.o

test_crnn:
	$(CC) $(CFLAGS) -D CRNN -c test.cpp -o test_crnn.o

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS) test test_q test_crnn
	$(CC) $(OBJECTS) test.o -Wall $(LIBS) -o kws
	$(CC) $(OBJECTS) test_q.o -Wall $(LIBS) -o kws_q
	$(CC) $(OBJECTS) test_crnn.o -Wall $(LIBS) -o kws_crnn

$(TARGET_HW): 
	$(CC) test_cnn_hw.cpp -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

# vim:ft=make
#
