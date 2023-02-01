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
#include "Colliders.h"
#include <vector>

using namespace std;
using namespace glm;

class SphereCollider : public BaseCollider
{
private:
	//float distance; //Distance between both centers computed
	float radiusAdded; //Both radii of the spheres are added together
	vec3  x, y, z; // for working out sphere to aabb

public:
	float radius; //of the spheres
	vec3 offset;
	bool SphereCollideCheck(SphereCollider* collider, CollisionData* data); //If the distance is less than the radius sum, the spheres are colliding
	SphereCollider();
	SphereCollider(float, glm::vec3, vec3, string);
	~SphereCollider();
};