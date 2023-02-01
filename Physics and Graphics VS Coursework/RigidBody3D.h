#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "GameObject.h"
#include "Particle.h"
#include "vertex.h"

using namespace glm;

class RigidBody3D : public GameObject
{
private:
	//Rotational force variables
	float width;
	float length;
	float height;
	mat3 inertia; //Moment of intertia
	mat3 inverseInertia;
	glm::vec3 aAcceleration; //Three number data needed
	glm::vec3 aVelocity; //Three number data needed
	glm::vec3 totalAngularForce; //Is the torque
	//glm::vec3 aForce; //creating torque

	//Linear force variables
	float mass; //Normally One number, but can contain decimal place values.
	float inverseMass;

	glm::vec3 acceleration; //Three number data needed
	glm::vec3 velocity; //Three number data needed
	glm::vec3 totalLinearForce; //Three number data needed

	float orientation;  
	glm::vec3 position; //Three number data needed
public:

	float GetOrientation();
	glm::vec3 GetPosition();

	void Update(float);
	void ResetForce();
	void AddForce(glm::vec3, glm::vec3);
	RigidBody3D();
	RigidBody3D(float, float, float, float, mat3, float, glm::vec3);
	~RigidBody3D();
};



