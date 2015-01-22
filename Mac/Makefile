CXX = g++
CXXFLAGS = -c -std=c++11 -Wall -W
LDFLAGS = -lAntTweakBar -framework OpenGL -lglfw3 -lglew
INCPATH = -I./src -I./lib/glm -I./lib/glew-1.10.0 -I./lib/FreeImage/include/FreeImage.h
SOURCES = src/main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = waterfall

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	rm -f src/*.o waterfall
