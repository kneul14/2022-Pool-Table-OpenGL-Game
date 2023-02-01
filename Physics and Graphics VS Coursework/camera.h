#pragma once
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include <map>
#include "shader.h"
#include "vertex.h"
#include "Sphere.h"

using namespace std;
using namespace glm;

class Camera
{
protected:
	//Sphere* SpherePTR;

public:
	//keyboard input
	static std::map<char, bool> keys;
	static std::map<char, bool> specialKeys;

    static bool isCamera1, isCamera2, isCamera3, isCamera4, isCamera5, isCamera6;
	//  bool isCamera1 = true, isCamera2 = false, isCamera3 = false, isCamera4 = false, isCamera5 = false, isCamera6 = false;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	#pragma region attempt at free moving camera
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	const float cameraSpeed = 0.05f; // adjust accordingly

	#pragma endregion 

	glm::vec3 spherePos;
	glm::quat rotation;


	float cameraYaw = 90.0f;
	float cameraPitch;

	//Sphere* GetSphere();
	Camera();
	~Camera();
    glm::mat4 WhichCamera(float);
	void UpdateCamera();
	void KeyInputCallback(unsigned char key, int x, int y);
};

