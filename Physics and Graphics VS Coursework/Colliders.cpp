#include "Colliders.h"

void BaseCollider::SetPosition(vec3 pos, vec3 off)
{
	position = pos + off;
	offset = off;
}