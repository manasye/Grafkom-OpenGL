build:
	g++ main.cpp -o main -lGL -lGLU -lglut

run:
	./main

all:
	make build 
	make run

clean:
	rm main