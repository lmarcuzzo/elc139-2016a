CXX=g++
#CXXFLAGS=-Wall -I. -O2 -DNDEBUG
CXXFLAGS=-Wall -I. -fopenmp
OBJS=Graph.o floyd_openmp.o
PROGS=floyd_openmp
floyd_openmp: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
all: $(PROGS)
clean:
	rm -f *.o $(PROGS)
