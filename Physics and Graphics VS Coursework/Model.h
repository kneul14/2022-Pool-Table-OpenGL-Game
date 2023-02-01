#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"
#include "GameObject.h"
#include "RigidBody3D.h"
#include "Camera.h"

using namespace glm;

class Model : public GameObject
{
private:
	VertexWithAll* VerticesData;  //Data with vertices, normal, texCoords

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO; //for triangle indices buffer

	int NumVert;
	float speed = 2.0f; //this also changes how quickly the cue rotates
	float mass;
	mat3 inertia;

	mat4 ModelMatrix;

	void CreateObject(const char*);

protected:
	RigidBody3D* RB3DPTR;

public:
	Camera camera;
	float d = 0.0f;
	float forceToHitWith = -1310.0f;

	bool posi;

	Model(const char*, float, float, vec3, string);
	~Model();

	void SetPosition(vec3 newPos);
	vec3 GetPosition(void);

	void SetIDs(unsigned int, unsigned int, unsigned int);
	void updateModelMatrix(unsigned int, float, float, vec3); //(MatrixLoc, camera offset,scale, Object offset Position);

	virtual void Initialise(unsigned int programId, unsigned int textureID) = 0;

	void Setup();
	void Draw();

	void CheckForInput();
	SphereCollider* GetSphereCollider() { return sphereColliderPTR; }
	RigidBody3D* GetRigidBody2D() { return RB3DPTR; }
	void Update(float);
};

