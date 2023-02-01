#include <iostream>
#include "RigidBody3D.h"

float RigidBody3D::GetOrientation()
{
	return orientation;
}

glm::vec3 RigidBody3D::GetPosition()
{
	return position;
}

void RigidBody3D::Update(float deltaTime)
{
	#pragma region Linear calcs

	acceleration = totalLinearForce / mass; //force / mass = acceleration

	position = position + ((velocity)* deltaTime);

	velocity = velocity + ((acceleration)* deltaTime);

	velocity *= pow(0.2f/* A NUMBER BETWEEN 0 and 1 */, deltaTime); //works like wind or drag or smth  // The lower it is the stronger it is?
#pragma endregion

	#pragma region Angular calcs

	aAcceleration = totalAngularForce * inverseInertia;

	aVelocity = aVelocity + ((aAcceleration)* deltaTime);

	aVelocity *= pow(0.2f/* A NUMBER BETWEEN 0 and 1 */, deltaTime);

	orientation = orientation + (aVelocity.z * deltaTime);

#pragma endregion

	//std::cout << aVelocity.x << " " << aVelocity.y << " " << aVelocity.z << std::endl;
}

void RigidBody3D::ResetForce()
{
	totalLinearForce = glm::vec3(0, 0, 0);
	totalAngularForce = glm::vec3(0, 0, 0);
}

void RigidBody3D::AddForce(glm::vec3 force, glm::vec3 distance)
{
	totalLinearForce += force;
	totalAngularForce += glm::cross(distance, force); //This needs to have a format such as (distance.a x totalAngularForce.z) - (distance.z x totalAngularForce.y) = torque..	
}

RigidBody3D::RigidBody3D()
{
}

RigidBody3D::RigidBody3D(float w, float l, float h, float m, mat3 i, float o, glm::vec3 pos)
{
	mass = m;
	inertia = i;
	width = w;
	length = l;
	height = h;
	//orientation = o;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	totalLinearForce = glm::vec3(0, 0, 0);
	position = pos;

	inertia[0][0] = ((1.0f/12.0f)* mass *((height * height) + (width * width)));
	inertia[0][1] = 0.0;
	inertia[0][2] = 0.0;

	// Column 1:
	inertia[1][0] = 0;
	inertia[1][1] = ((1.0f/12.0f)* mass *((length * length) + (width * width)));
	inertia[1][2] = 0;

	// Thrid colunm.
	inertia[2][0] = 0.0;
	inertia[2][1] = 0.0;
	inertia[2][2] = ((1.0f / 12.0f) * mass *((height * height) + (length * length)));

	inverseInertia = glm::inverse(inertia);
	inverseMass = 1 / mass;

	aVelocity = glm::vec3(0, 0, 0);
	aAcceleration = glm::vec3(0, 0, 0);
	totalAngularForce = glm::vec3(0, 0, 0);
	orientation = o = 0.0f;
}

RigidBody3D::~RigidBody3D()
{
}