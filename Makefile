build:
	g++ main.cpp -o main -lGLEW -lGL -lGLU -lglut --std=c++11

run:
	./main

all:
	make build 
	make run

clean:
	rm main