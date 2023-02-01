#pragma once
#include "GameObject.h"
#include "Cue.h"

class Player : public GameObject
{
private:
	float speed;
	float mass;
	glm::vec3 position;

public:
	Cue* cuePTR;
	void Draw();
	void CheckForInput();
	void Update(float);
	Player(float, float, glm::vec3);
	~Player();
	Particle* GetParticle() { return particlePTR; }
	Cue* GetCue() { return cuePTR; }
};