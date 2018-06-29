#
# Makefile
# Yongwen Zhuang, 2018-06-28 13:48
#

TARGET = kws
LIBS = -lm
CC = g++
CFLAGS = -Wall -O3

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = cnn.o mfcc.o fft.o test_cnn.o
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

# vim:ft=make
#
