# gcc main.c student.c -o main
CC = g++
CFLAGS = -Wall -g
LDFLAGS =

# SRCS = lab3.cpp computer.cpp
SRCS = lab3.cpp
OBJS = $(SRCS:.c=.o)
TARGET = lab3

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
