#include "CollisionData.h"
#include "GameObject.h"
#include "SphereCollider.h"


CollisionData::CollisionData(float interPenetrationDepth, glm::vec3 collisonNormal, glm::vec3 collisonPoint)
{
	interPenetrationDepth = 0;
	collisonNormal = glm::vec3(0, 0, 0);
	collisonPoint = glm::vec3(0, 0, 0);
}

CollisionData::CollisionData()
{
}

CollisionData::~CollisionData()
{
}