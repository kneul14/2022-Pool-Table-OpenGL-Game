#include "Player.h"
#include "Particle.h"

Player::Player(float s, float m, glm::vec3 pos) : GameObject(1.0f, 0.0f, 0, pos, tag)
{
	speed = s;
	mass = m;
	position = pos;
}

void Player::Draw()
{
}

void Player::CheckForInput()
{
	if (GameObject::specialKeys[GLUT_KEY_UP])
		particlePTR->AddForce(glm::vec3(-10.0f * speed, 0.0f, 0.0f)); //makes go up
	if (GameObject::specialKeys[GLUT_KEY_LEFT])
		particlePTR->AddForce(glm::vec3(0.0f, 0.0f, 10.0f * speed)); //makes go left
	if (GameObject::specialKeys[GLUT_KEY_RIGHT])
		particlePTR->AddForce(glm::vec3(0.0f, 0.0f, -10.0f * speed)); //makes go right
	if (GameObject::specialKeys[GLUT_KEY_DOWN])
		particlePTR->AddForce(glm::vec3(10.0f * speed, 0.0f, 0.0f)); //makes go down
}

void Player::Update(float deltaTime)
{
	//reset the forces for particle ->>>> //player->GetParticle()->ResetForce();
	CheckForInput();
	particlePTR->Update(deltaTime);
}

Player::~Player()
{
}
