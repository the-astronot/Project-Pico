CXX=g++
CXXFLAGS=-std=c++1z -g
MAIN=compress
SRC_DIR=src
SRCS=$(wildcard $(SRC_DIR)/*.cpp)
INCLUDES= -I./include
OBJS=$(SRCS:.cpp=.o)

.PHONY: all clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	$(RM) $(MAIN) $(SRC_DIR)/*.o *~
