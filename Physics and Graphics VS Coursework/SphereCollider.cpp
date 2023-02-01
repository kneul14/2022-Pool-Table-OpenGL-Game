#include "SphereCollider.h"
#include "AABBCollider.h"
#include "PlaneCollider.h"

bool SphereCollider::SphereCollideCheck(SphereCollider* collider, CollisionData* data)
{
	distance = sqrt(pow(position.x - collider->position.x, 2) + pow(position.y - collider->position.y, 2) + pow(position.z - collider->position.z, 2));
	radiusAdded = radius + collider->radius;

	if (distance <= radiusAdded)
	{
		data->interPenetrationDepth = radiusAdded - distance;
		data->collisonNormal = glm::normalize(position - collider->position);
		data->collisonPoint = position + data->collisonNormal * radius;
		//std::cout << "There is a collision!" << std::endl;
		return true;
	}
	else {
		//std::cout << "There is no collision!" << std::endl;
		return false;
	}

	return false;
}

SphereCollider::SphereCollider()
{

}

SphereCollider::SphereCollider(float r, glm::vec3 pos, glm::vec3 off, std::string tag)
{
	radius = r;
	position = pos + off;
	offset = off;
	gameObjectTag = tag;
}

SphereCollider::~SphereCollider()
{
}