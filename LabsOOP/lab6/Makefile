# gcc main.c student.c -o main
CC = g++
CFLAGS = -Wall -g
LDFLAGS =

SRCS = lab6.cpp utils.cpp computer.cpp computers.cpp
OBJS = $(SRCS:.c=.o)
TARGET = lab6

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
