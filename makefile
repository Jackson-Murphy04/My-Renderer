OBJS    = build/main.o build/ppm.o
SOURCE  = src/main.cpp src/ppm.cpp
HEADER  = include/ppm.h
OUT     = build/run.exe
FLAGS   = -g -Wall
LFLAGS  = 
CC      = g++

all: build $(OUT)

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

build/main.o: src/main.cpp $(HEADER)
	$(CC) $(FLAGS) -c src/main.cpp -o build/main.o

build/ppm.o: src/ppm.cpp $(HEADER)
	$(CC) $(FLAGS) -c src/ppm.cpp -o build/ppm.o

clean:
	rm -f build/*.o $(OUT)