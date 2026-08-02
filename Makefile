prog: main.cpp
	g++ main.cpp ./src/glad.c -I./include/ -o app -lSDL2 -ldl -I./glm-master/
	
clean:
	rm -f prog

