#pragma once
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "CollisionData.h"
#include "Particle.h"
#include "vertex.h"
#include <vector>
#include <string>

class BaseCollider
{
public:
	float distance; //Distance between collider and clostest point 
	vec3 offset;
	vec3 position; //position on axis
	std::string gameObjectTag = "";

public:
	void SetPosition(vec3, vec3);
};