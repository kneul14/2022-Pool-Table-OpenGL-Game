#pragma once
#include <glm/glm.hpp>

using namespace glm;

class GameObject;
class BaseCollider;

class CollisionData
{
public:
	float interPenetrationDepth = 0;
	vec3 collisonNormal;
	vec3 collisonPoint;

	GameObject* a;
	GameObject* b;

	BaseCollider* c;


	CollisionData(float, glm::vec3, glm::vec3);
	CollisionData();
	~CollisionData();
};

//The purpose of this script is so that when I have all the other collisions 
//in place I will be able to store their data externally and reference and 
//call depending on the apparent collision


//It's my understanding that every collision should store some kind of data 
// or have access to data.