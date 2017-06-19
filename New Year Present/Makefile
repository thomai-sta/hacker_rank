CXX = g++
CXXFLAGS = -g -Wall
SOURCES = main.cpp
PROG = out.o

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SOURCES)
  $(CXX) $(CXXFLAGS) -o $(PROG) $(SOURCES) $(LIBS)

clean:
  rm -f *.o