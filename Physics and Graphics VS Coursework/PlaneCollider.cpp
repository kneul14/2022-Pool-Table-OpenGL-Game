#include "PlaneCollider.h"

PlaneCollider::PlaneCollider()
{
	v = ((position.x, position.y, position.z) - (planePoint.x, planePoint.y, planePoint.z));
}

PlaneCollider::PlaneCollider(glm::vec3 pp, glm::vec3 pos)
{
	planePoint = pp;
	position = pos;
}

PlaneCollider::~PlaneCollider()
{
}

void PlaneCollider::Draw()
{
}

bool PlaneCollider::PlaneCollideCheck(PlaneCollider& planecollider)
{
	//distance = glm::dot()
	//if (distance <= radiusAdded)
	//{
	//	return true;
	//}
	//else {
	//	return false;
	//}
	return false;
}
