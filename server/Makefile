TARGET = server
ifeq ($(OS),Windows_NT) 
	LIBS = -lWs2_32
	RMFILE = del
	MKDIR = md
else
	# LIBS = 
	RMFILE = rm -f
	MKDIR = mkdir
endif
CC = gcc
CFLAGS = -Wall -g

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-$(RMFILE) *.exe
	-$(RMFILE) *.o
	-$(RMFILE) $(TARGET)
