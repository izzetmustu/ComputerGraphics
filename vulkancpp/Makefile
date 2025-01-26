CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SRCS = main.cpp src/*.cpp 
INCLUDES = -Iinclude

buildCode: $(SRCS)
	mkdir -p build
	g++ $(CFLAGS) -o build/program $(SRCS) $(INCLUDES) $(LDFLAGS)

buildShaders: shaders/*.frag shaders/*.vert
	mkdir -p build/shaders
	glslc shaders/*.frag -o build/shaders/frag.spv
	glslc shaders/*.vert -o build/shaders/vert.spv

# build: buildShaders buildCode 
build: buildCode buildShaders
	
run: build/program 
	./build/program 

test: build run

clean:
	rm -rf build

.PHONY: run build clean test buildCode buildShaders
