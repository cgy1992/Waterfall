CXX = g++
CXXFLAGS = -c -std=c++11 -Wall -W
LDFLAGS = -lAntTweakBar -framework OpenGL -framework AppKit -lglfw3 -lglew -lfreeimage
INCPATH = -I./src -I./lib/glm -I./lib/glew-1.10.0
SOURCES = src/skybox_main.cpp src/tiny_obj_loader.cc src/model.cpp src/utils.cpp src/camera.cpp src/skybox.cpp src/particlesystem.cpp src/shaders.cpp src/texture.cpp src/waterfall.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = waterfall

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	rm -f src/*.o waterfall
