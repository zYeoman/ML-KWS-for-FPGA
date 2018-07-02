#
# Makefile
# Yongwen Zhuang, 2018-06-28 13:48
#

CC := clang++
SRCDIR := src
BUILDDIR := build
INCLUDEDIR := include
TARGET := kws
CFLAGS := -Wall -O3 -I./$(INCLUDEDIR)
LIBS := -lm
SRCEXT := cpp

.PHONY: default all clean

default: $(TARGET)
all: default

SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
HEADERS := $(wildcard $(INCLUDEDIR)/*.h)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	@echo " Linking... "
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $(TARGET)

clean:
	@echo " Cleaning... "
	-rm -rf $(BUILDDIR)
	-rm -rf $(TARGET)

# vim:ft=make
#
