#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"

using namespace std;
using namespace glm;

std::map<char, bool> Camera::keys;
std::map<char, bool> Camera::specialKeys;

bool Camera::isCamera1 = true;
bool Camera::isCamera2 = false;
bool Camera::isCamera3 = false;
bool Camera::isCamera4 = false;
bool Camera::isCamera5 = false;
bool Camera::isCamera6 = false;

//Sphere* Camera::GetSphere()
//{
//	return SpherePTR;
//}

Camera::Camera()
{
	//spherePos = SpherePTR->GetPosition();
}

Camera::~Camera()
{

}

glm::mat4 Camera::WhichCamera(float d) // (eye_x, eye_y, eye_z) (center_x, center_y, center_z) (up_dx, up_dy, up_dz)
{
	mat4 viewMatrix;

	if (isCamera1 == true) //start
	{
		viewMatrix = lookAt(vec3(0.0, 15.0, 22.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, -1.0));
	}
	if (isCamera2 == true) //top forward
	{
		viewMatrix = lookAt(vec3(0.0, 25.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, -1.0));		
	}
	if (isCamera3 == true) //top side
	{
		viewMatrix = lookAt(vec3(0.0, 25.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0));
	}
	if (isCamera4 == true) //free cam
	{
		viewMatrix = lookAt(vec3(0.0, 25.0, 22.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, -1.0));
		//viewMatrix = lookAt(vec3(0.0, 20.0, 15.0), vec3(0.0 + d, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
	}
	if (isCamera5 == true) //reverse cam
	{
		//viewMatrix = glm::lookAt(glm::vec3(spherePos - 5.0f * glm::vec3(0, -1, 2) * glm::quat(rotation)),
		//glm::vec3(spherePos + cameraFront),
		//glm::vec3(cameraUp));

		viewMatrix = lookAt(vec3(0.0, 15.0, -22.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, -1.0));
	}
	if (isCamera6 == true) //Logo Camera 
	{
		viewMatrix = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
	}

	return viewMatrix;
}

void Camera::UpdateCamera()
{
}

void Camera::KeyInputCallback(unsigned char key, int x, int y)
{
}