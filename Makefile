CXX=g++
CXXFLAGS=-std=c++1z -g
MAIN=compress.out
SRC_DIR=src
SRCS= bitf.cpp Character.cpp compress.cpp FileRead.cpp FileWrite.cpp leaff.cpp
FULLSRCS = $(foreach source,$(SRCS),$(wildcard $(SRC_DIR)/$(source)))
INCLUDES= -I./include
OBJS=$(FULLSRCS:.cpp=.o)

.PHONY: all clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	$(RM) $(MAIN) $(SRC_DIR)/*.o *~
