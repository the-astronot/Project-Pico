CXX=g++
CXXFLAGS=-std=c++1z -g
MAIN=pico
SRC_DIR=src
SRCS=$(wildcard $(SRC_DIR)/*.cpp)
INCLUDES= -I./include
OBJS=$(SRCS:.cpp=.o)


# Don't Look at This Too Hard
# It was not my finest hour
LINK=$(shell pwd)/config.txt
FWDSLASH=/
ESCAPESLASH=\/
WORKINGLINK=$(subst $(FWDSLASH),$(ESCAPESLASH),$(LINK))
#
#


.PHONY: all clean replace unreplace

all: replace $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

replace:
	sed -i 's/MAKEFILE_PLACEHOLDER/$(WORKINGLINK)/' src/main.cpp

unreplace:
	sed -i 's/$(WORKINGLINK)/MAKEFILE_PLACEHOLDER/' src/main.cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean: unreplace
	$(RM) $(MAIN) $(SRC_DIR)/*.o *~
