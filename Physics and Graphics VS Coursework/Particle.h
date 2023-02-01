#pragma once
#include <glm/glm.hpp>

class Particle
{
private:

public:
    float mass; //Normally One number, but can contain decimal place values.
    glm::vec3 acceleration; //Three number data needed
    glm::vec3 velocity; //Three number data needed
    glm::vec3 totalLinearForce; //Three number data needed
    float friction = 0.3f; /* A NUMBER BETWEEN 0 and 1 */

    glm::vec3 position; //Three number data needed
    float time; //current time
    bool hasGravity /*= true*/;

    bool isOutOfPlay = false; //This ball always needs to be in play
    bool isPotted; //Take the correct ball out of commision

    Particle(float m, float f, glm::vec3 pos);
    ~Particle();
    void Update(float);
    float GetMass();
    float GetFriction();
    glm::vec3 GetAcceleration();
    glm::vec3 GetVelocity();
    glm::vec3 GetTotalForce();
    void ResetForce();
    void AddForce(glm::vec3);
};

