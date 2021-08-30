# release build - make DEBUG=0
# debug build (default) - make DEBUG=1

#make  24.36s user 1.42s system 99% cpu 25.946 total

DEBUG  ?= 1

CXX=clang++
RM=rm -f

PCH_I=./source/Project.h
PCH_O=$(PCH_I).pch
PCH_C=-include $(PCH_I)

CPPFLAGS=-std=c++17 -stdlib=libc++

LDFLAGS=-L./vendor/raylib
LDLIBS=-lc++ -lraylib -framework OpenGL -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT

ifeq ($(DEBUG), 1)
	CPPFLAGS +=-g -Wall -pedantic
	LDFLAGS +=-g
else
	CPPFLAGS += -O3 -DNDEBUG
endif

SRCS=$(shell find ./source -name "*.cpp")
OBJS=$(subst .cpp,.o,$(SRCS))

TEST_SRCS=$(shell find ./test -name "*.cpp")
TEST_OBJS=$(subst .cpp,.o,$(TEST_SRCS))

PCH_HEADERS=$(shell find ./source -name "*.h")

all: simon

simon: $(OBJS) ./main.cpp
	$(CXX) $(CPPFLAGS) $(PCH_C) $(LDFLAGS) -o simon $^ $(LDLIBS)

# pch compile rule
# make the pch depend on all header
# changing an header triggers a full rebuild
$(PCH_O): $(PCH_I) $(PCH_HEADERS)
	$(CXX) $(CPPFLAGS) -x c++-header $<  -o $@ 

%.o : %.cpp $(PCH_O) 
	$(CXX) -c $(CPPFLAGS) $(PCH_C) $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) $(TEST_OBJS)
	$(RM) $(PCH_O)
	$(RM) simon
	$(RM) -rf *.dSYM
	