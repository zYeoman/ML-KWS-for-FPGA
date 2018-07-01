#
# Makefile
# Yongwen Zhuang, 2018-06-28 13:48
#

TARGET = kws
TARGET_HW = kws_hw
LIBS = -lm
CC = g++
CFLAGS = -Wall

.PHONY: default all clean

default: $(TARGET) $(TARGET_HW)
all: default

OBJECTS = cnn.o mfcc.o fft.o test_cnn.o
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

$(TARGET_HW): 
	$(CC) test_cnn_hw.cpp -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

# vim:ft=make
#
