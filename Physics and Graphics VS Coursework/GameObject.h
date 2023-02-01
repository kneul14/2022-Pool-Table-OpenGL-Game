#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "Particle.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include <map>

class GameObject
{
public:
	Particle* particlePTR;
	SphereCollider* sphereColliderPTR;
	AABBCollider* aabbColliderPTR;
	string tag = "";

	//keyboard input
	static std::map<char, bool> keys;
	static std::map<char, bool> specialKeys;

	GameObject(glm::vec3 pos, int score, string tag);

	//3D position of the object
	glm::vec3 position;
	glm::vec3 Colour;

	int ballScore;

	//OpenGL drawing function
	virtual void Draw(int location, int colourLoc);

	///physics update function
	virtual void Update(float deltaTime);

	//Get particle collider 
	Particle* GetParticle();
	//Get sphere collider 
	SphereCollider* GetSphereCollider();

	AABBCollider* GetAABBCollider();

	GameObject();
	GameObject(float m, float f, int score, glm::vec3 pos, string tag);
	~GameObject(); //this might need to be changed to a virtual function like draw and update
};

