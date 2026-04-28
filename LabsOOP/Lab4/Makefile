CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET  = lab4
SRCS    = lab4.cpp computer.cpp computers.cpp utils.cpp
OBJS    = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
