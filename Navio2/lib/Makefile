CXX = g++
CFLAGS = -Wno-psabi -c -I .

SRC=$(wildcard Navio2/*.cpp)
OBJECTS = $(SRC:.cpp=.o) 

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ $< 

all: $(OBJECTS)
	ar rcs libnavio.a $(OBJECTS)

clean:
	rm -f */*.o *.a
