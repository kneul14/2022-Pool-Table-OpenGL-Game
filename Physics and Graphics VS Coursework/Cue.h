#pragma once
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 

#include "soil/SOIL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Model.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Shader.h"
#include "vertex.h"

class Cue : public Model /*: public GameObject*/
{
protected:
	//physics stuff here
	//PhysicsBody* physicsBody;
	//SphereCollider* collider;
private:
	VertexWithAll* VerticesData;  //Data with vertices, normal, texCoords

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO; //for triangle indices buffer

	int NumVert;
	float speed;
	float mass;

	mat4 ModelMatrix;

	void CreateObject(const char*);

public:
	Cue(const char*, float, float, vec3, string);
	~Cue();

	void SetPosition(vec3 newPos);
	vec3 GetPosition(void);

	void SetIDs(unsigned int, unsigned int, unsigned int);
	void updateModelMatrix(unsigned int, float, float, vec3); //(MatrixLoc, camera offset,scale, Object offset Position);
	void Initialise(unsigned int programId, unsigned int textureID) override;

	void Setup();
	void Draw();
	void CheckForInput();
	Particle* GetParticle() { return particlePTR; }

	//SphereCollider* GetSphereCollider() { return sphereColliderPTR; }
	//void Update(float/*, glm::vec3 offset*/) override;
};