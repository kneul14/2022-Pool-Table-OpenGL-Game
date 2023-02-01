#include "AABBCollider.h"
#include "SphereCollider.h"

AABBCollider::AABBCollider()
{
}

AABBCollider::AABBCollider(glm::vec3 s, glm::vec3 pos, glm::vec3 off, std::string tag)
{
	size = s;
	position = pos + off;
	offset = off;
	gameObjectTag = tag;
}

AABBCollider::~AABBCollider()
{
}

void AABBCollider::Draw()
{
}

bool AABBCollider::AABBColliderCheck(AABBCollider& collider, CollisionData* data)
{
	return false;
}

bool AABBCollider::SpherexAABBCollideCheck(SphereCollider* sphereCollider, CollisionData* data)
{
	float minX = position.x - size.x;
	float minY = position.y - size.y;
	float minZ = position.z - size.z;

	float maxX = position.x + size.x;
	float maxY = position.y + size.y;
	float maxZ = position.z + size.z;

	// get box closest point to sphere center by clamping
	float x = glm::max(minX, glm::min(sphereCollider->position.x, maxX));
	float y = glm::max(minY, glm::min(sphereCollider->position.y, maxY));
	float z = glm::max(minZ, glm::min(sphereCollider->position.z, maxZ));

	// checks if they're inside each other
	distance = glm::sqrt((x - sphereCollider->position.x) * (x - sphereCollider->position.x) +
		(y - sphereCollider->position.y) * (y - sphereCollider->position.y) +
		(z - sphereCollider->position.z) * (z - sphereCollider->position.z));

	if (distance <= sphereCollider->radius)
	{
		data->interPenetrationDepth = sphereCollider->radius - distance;

		data->collisonNormal = glm::normalize(position - sphereCollider->position);

		data->collisonPoint = position + (-data->collisonNormal * sphereCollider->radius) + data->interPenetrationDepth;

		//data->collisonPoint = position + data->collisonNormal * sphereCollider->radius;

		//std::cout << "There is a AABB-SPHERE collision on " + data->c->gameObjectTag + "!" << std::endl;
		return true;
	}
	//If not, then return false
	return false;
}

// reference to the AABB to Sphere colision info page:)
//https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
//https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/4collisiondetection/Physics%20-%20Collision%20Detection.pdf