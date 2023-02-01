#include <iostream>
#include "RigidBody2D.h"

void RigidBody2D::Update(float deltaTime)
{
	//ResetForce();

	/*Remember to speak to Ian an ask him if he can double check the variables and working outs below regarding the session 6 content*/

#pragma region Angular calcs

	aAcceleration = totalAngularForce / inertia;

	aVelocity = aVelocity + ((aAcceleration)* deltaTime);

	aVelocity *= pow(0.2f/* A NUMBER BETWEEN 0 and 1 */, deltaTime);

	orientation = orientation + (aVelocity.z * deltaTime);

#pragma endregion

#pragma region Linear calcs

	acceleration = totalLinearForce / mass; //force / mass = acceleration

	position = position + ((velocity)* deltaTime);

	velocity = velocity + ((acceleration)* deltaTime);

	velocity *= pow(0.2f/* A NUMBER BETWEEN 0 and 1 */, deltaTime); //works like wind or drag or smth  // The lower it is the stronger it is?
#pragma endregion

	std::cout << aVelocity.x << " " << aVelocity.y << " " << aVelocity.z << std::endl;
}

void RigidBody2D::ResetForce()
{
	totalLinearForce = glm::vec3(0, 0, 0);
	totalAngularForce = glm::vec3(0, 0, 0);
}

void RigidBody2D::AddForce(glm::vec3 force, glm::vec3 distance)
{
	totalLinearForce += force;
	totalAngularForce += glm::cross(distance, force); //This needs to have a format such as (distance.a x totalAngularForce.z) - (distance.z x totalAngularForce.y) = torque..
}

RigidBody2D::RigidBody2D()
{
}

RigidBody2D::RigidBody2D(float w, float l, float m, float i, float o, glm::vec3 pos)
{
	mass = m;
	inertia = i;
	width = w;
	length = l;
	//orientation = o;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	totalLinearForce = glm::vec3(0, 0, 0);
	position = pos;

	aVelocity = glm::vec3(0, 0, 0);
	aAcceleration = glm::vec3(0, 0, 0);
	totalAngularForce = glm::vec3(0, 0, 0);
	orientation = o = 0.0f;
}

RigidBody2D::~RigidBody2D()
{
}