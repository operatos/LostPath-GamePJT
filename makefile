SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.c)
CFLAGS = -g -Wall -lgthread-2.0 -w
TARGET = LostPath
CC = gcc

.PHONY: $(TARGET)

$(TARGET): $(OBJS)  
	$(CC) `pkg-config --cflags gtk+-2.0` -o $(TARGET) $(OBJS) `pkg-config --libs gtk+-2.0` $(CFLAGS)

clean:
	$(RM) $(OBJS) $(TARGET)

