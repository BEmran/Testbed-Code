CXX ?= g++
NAVIO = include/Navio
INCLUDES = -I $(NAVIO)/..

all:
	$(CXX) $(INCLUDES) Navio_IMU.cpp -o Navio_IMU include/SamplingTime.cpp $(NAVIO)/MPU9250.cpp $(NAVIO)/LSM9DS1.cpp $(NAVIO)/Util.cpp -lrt -lpthread

clean:
	rm Navio_IMU.cpp
