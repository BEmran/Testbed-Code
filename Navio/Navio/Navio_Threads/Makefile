CXX ?= g++
NAVIO = ../Navio
INCLUDES = -I include -I $(NAVIO)

all:
	$(CXX) $(INCLUDES) Navio_Threads.cpp $(NAVIO)/PWM.cpp $(NAVIO)/Util.cpp -o Navio_Threads include/SamplingTime.cpp -lrt -lpthread

clean:
	rm -f Navio_Threads.cpp
