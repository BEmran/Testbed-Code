CXX = g++
CFLAGS =-std=c++11 -Wno-psabi -c -I . -I */.

SRC=$(wildcard Navio/*/*.cpp *.cpp)
OBJECTS = $(SRC:.cpp=.o) 

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ $< 

all: $(OBJECTS)
	ar rcs libmynavio.a $(OBJECTS)

clean:
	rm -f Navio/*/*.o *.a *.o
