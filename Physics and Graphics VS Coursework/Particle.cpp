#include "Particle.h"

void Particle::Update(float deltaTime)
{
	if (isPotted == true) {
		hasGravity = true;
		isOutOfPlay = true;
	}
	if (hasGravity == true) {
		AddForce((glm::vec3(0.0f, -9.0f, 0.0f)) * GetMass()); //This is the gravity. 
	}

	//force = mass * acceleration

	acceleration = totalLinearForce / mass; //force / mass = acceleration

	//friction = 0.2f; //changed this to a global variable.
	velocity = velocity + ((acceleration)*deltaTime);
	velocity *= pow(friction, deltaTime); //works like wind or drag or smth  // The lower it is the stronger it is?

	position = position + ((velocity)*deltaTime);
}

Particle::Particle(float m, float f, glm::vec3 pos)
{
	//All play apart in Newtons Laws
	mass = m;
	friction = f;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	position = pos;
	totalLinearForce = glm::vec3(0, 0, 0);
}

Particle::~Particle()
{
}

float Particle::GetMass()
{
	return mass;
}

glm::vec3 Particle::GetAcceleration()
{
	return acceleration;
}

float Particle::GetFriction()
{
	return friction; /* A NUMBER BETWEEN 0 and 1 */
}

glm::vec3 Particle::GetVelocity()
{
	return velocity;
}

glm::vec3 Particle::GetTotalForce()
{
	return totalLinearForce;
}

void Particle::ResetForce()
{
	totalLinearForce = glm::vec3(0, 0, 0);
}

void Particle::AddForce(glm::vec3 force)
{
	totalLinearForce += force;
}
