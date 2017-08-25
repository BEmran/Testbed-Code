CXX ?= g++
INCLUDES = -I include

all:
	$(CXX) $(INCLUDES) Navio_Threads.cpp -o Navio_Threads include/SamplingTime.cpp -lrt -lpthread

clean:
	rm -f Navio_Threads.cpp
