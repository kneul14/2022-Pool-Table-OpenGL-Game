#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameObject.h"

using namespace glm;

class RigidBody2D : public GameObject
{
private:
	//Rotational force variables
	float width;
	float length;
	float inertia; //Moment of intertia
	glm::vec3 aAcceleration; //Three number data needed
	glm::vec3 aVelocity; //Three number data needed
	glm::vec3 totalAngularForce; //Is the torque

	//Linear force variables
	float mass; //Normally One number, but can contain decimal place values.
	glm::vec3 acceleration; //Three number data needed
	glm::vec3 velocity; //Three number data needed
	glm::vec3 totalLinearForce; //Three number data needed

public:
	glm::vec3 position; //Three number data needed
	float orientation; //Three number data needed

	void Update(float);
	void ResetForce();
	void AddForce(glm::vec3, glm::vec3);
	RigidBody2D();
	RigidBody2D(float, float, float, float, float, glm::vec3);
	~RigidBody2D();
};

