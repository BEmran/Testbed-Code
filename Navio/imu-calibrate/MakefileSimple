CXX ?= g++ -std=c++11 
LIB = ../lib
all:
	$(MAKE) -C $(LIB) all
	$(CXX) -I $(LIB) -I $(LIB)/Navio -L $(LIB) main.cpp -o mian -lmynavio -lrt -lpthread

clean:
	rm -f main
