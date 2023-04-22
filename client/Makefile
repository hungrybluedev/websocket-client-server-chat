TARGET = client
ifeq ($(OS),Windows_NT) 
	LIBS = -lWs2_32
	RMFILE = rm
else
	# LIBS = 
	RMFILE = rm -f
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
	-$(RMFILE) *.o
	-$(RMFILE) $(TARGET)
	-$(RMFILE) *.exe