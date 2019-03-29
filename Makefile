build:
	g++ main.cpp -o main -lGLEW -lGL -lGLU -lglut

run:
	./main

all:
	make build 
	make run

clean:
	rm main