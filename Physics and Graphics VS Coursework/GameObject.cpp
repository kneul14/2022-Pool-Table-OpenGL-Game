#include "GameObject.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

std::map<char, bool> GameObject::keys;
std::map<char, bool> GameObject::specialKeys;

GameObject::GameObject(glm::vec3 pos, int score, string tag)
{
    ballScore = score;
    position = pos;
    this->tag = tag;
}

GameObject::GameObject()
{
    glm::vec3 position;
}

GameObject::GameObject(float m, float f, int score, glm::vec3 pos, string tag)
{
    ballScore = score;
    particlePTR = new Particle(m, f, pos);
    this->tag = tag;
}

GameObject::~GameObject()
{
    delete particlePTR;
    delete sphereColliderPTR;
    delete aabbColliderPTR;
}

void GameObject::Draw(int location, int colourLoc)
{
    glm::mat4 modelMat = glm::mat4(1.0);
    modelMat = glm::translate(modelMat, position);

    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelMat));
    glUniform4fv(colourLoc, 1, glm::value_ptr(Colour));
}

Particle* GameObject::GetParticle()
{
    return particlePTR;
}

SphereCollider* GameObject::GetSphereCollider()
{
    return sphereColliderPTR;
}

AABBCollider* GameObject::GetAABBCollider()
{
    return aabbColliderPTR;
}

void GameObject::Update(float deltaTime)
{

}

