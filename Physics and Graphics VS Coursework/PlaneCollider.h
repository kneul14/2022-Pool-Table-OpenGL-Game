#pragma once
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"
#include <vector> 

using namespace std;
using namespace glm;

class PlaneCollider
{
private:
	vec3 planeNormal; //of the plane
	vec3 planePoint; //random point on the plane
	float v; //obj pos minus planePoint
	float distance; //Distance between both centers computed
	glm::vec3 position; //obj position
	//float lengthAdded; //Both lengths of the planes and sphere added together
	bool isDistanceLess; //If the distance is less than the lengths sum, the planes are colliding

public:
	PlaneCollider();
	PlaneCollider(glm::vec3, glm::vec3);
	~PlaneCollider();
	void Draw();
	bool PlaneCollideCheck(PlaneCollider& collider);
};

