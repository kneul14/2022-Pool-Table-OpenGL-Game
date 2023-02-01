#pragma once
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Particle.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "vertex.h"
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"

using namespace std;
using namespace glm;

class WhiteCueBall : public GameObject
{
private:

	int buffer, vao;
	unsigned int sphere_VAO; //VAO for the sphere object
	unsigned int sphere_Indices, sphere_Vertices; // VBO for the sphere object

	vec3 position; //The Sphere position in 3D world
	vec3 Colour; //The Sphere colour
	VertexWtihNormal* sphereVerticesNor;  //Sphere vertices data with normals
	unsigned int* sphereIndices;          //Sphere triangle indices 

	int stacks; // nunber of stacks
	int slices; // number of slices
	float radius;

	float speed;
	float mass;

	void CreateSpherewithNormal(); //The function creates vertex data and normals 

public:

	Camera camera;
	float d = 0.0f;

	int location;
	int colourLoc;

	const bool isOutOfPlay = false; //This ball always needs to be in play
	bool isPotted; //Take the correct ball out of commision

	WhiteCueBall();
	WhiteCueBall(float, float, vec3 newCol, vec3 newPos, string tag);
	~WhiteCueBall();

	void SetPosition(vec3 newPos); //set the 3D position of the sphere
	vec3 GetPosition(void);        //get the 3D position of the sphere    
	VertexWtihNormal* GetVerData(int&); //get vertex data and normals
	unsigned int* GetTriData(int&);     //get triangle index data

	void SendToGPU();
	void Draw(int location, int colourLoc);
	void CheckForInput();
	Particle* GetParticle() { return particlePTR; }
	SphereCollider* GetSphereCollider() { return sphereColliderPTR; }
	void Update(float) override;
};
