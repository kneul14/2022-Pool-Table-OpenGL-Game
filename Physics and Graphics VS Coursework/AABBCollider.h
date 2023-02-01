#pragma once
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector> 
#include "vertex.h"
#include "Colliders.h"
#include "CollisionData.h"


using namespace std;
using namespace glm;

class SphereCollider;

class AABBCollider : public BaseCollider
{
private:
	vec3 size; //Half of the Block <- 0 -> 

public:
	AABBCollider();
	AABBCollider(glm::vec3, glm::vec3, glm::vec3, string);
	~AABBCollider();
	void Draw();
	bool AABBColliderCheck(AABBCollider& collider, CollisionData* data);
	bool SpherexAABBCollideCheck(SphereCollider* sphereCollider, CollisionData* data); //If the distance is less than the radius sum, the spheres are colliding
};

