# gcc main.c student.c -o main
CC = g++
CFLAGS = -Wall -g
LDFLAGS =

SRCS = lab7.cpp utils.cpp computer.cpp tablet.cpp computers.cpp
OBJS = $(SRCS:.c=.o)
TARGET = lab7

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
