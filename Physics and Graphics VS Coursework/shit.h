#pragma once
#include <cmath>
#include <fstream>
#include <iostream>
#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "soil/SOIL.h"

#include "shader.h"
#include "vertex.h"
#include "sphere.h"
#include "Model.h"
#include "camera.h"
#include "GameObject.h"
#include "Particle.h"
#include "Player.h"
#include "ForceGenerator.h"
#include "SkyBox.h"
#include "PoolTable.h"
#include <vector>
#include <glm/gtx/string_cast.hpp>

#pragma comment(lib, "glew32.lib") 
#endif
class vectorthree
{
private:

	float x, y, z;


public:

	float GetX();
	float GetY();
	float GetZ();

	vec3 vector3();
	vec3 vector3(float _x, float _y, float _z);
	float DotProduct(vectorthree other);
	vec3 CrossProduct(vectorthree other);

};
