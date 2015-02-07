#ifndef COMMON_H
#define COMMON_H

#include <cstddef>

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <memory>
using std::shared_ptr;

#include <chrono>
namespace chrono = std::chrono;

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;

#include <FreeImage.h>
#include <AntTweakBar.h>

#endif //COMMON_H