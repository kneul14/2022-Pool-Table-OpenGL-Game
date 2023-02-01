#pragma once
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "GameObject.h"
#include "Particle.h"
#include "vertex.h"
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

class Sphere : public GameObject
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

	//unsigned int VAO;
	//unsigned int VBO;
	//unsigned int IBO; //for triangle indices buffer

	//mat4 ModelMatrix;

	void CreateSpherewithNormal(); //The function creates vertex data and normals 
public:

	int location;
	int colourLoc;

	Sphere();
	Sphere(float, float, vec3 newCol, vec3 newPos);
	~Sphere();

	void SetPosition(vec3 newPos); //set the 3D position of the sphere
	vec3 GetPosition(void);        //get the 3D position of the sphere    
	VertexWtihNormal* GetVerData(int&); //get vertex data and normals
	unsigned int* GetTriData(int&);     //get triangle index data

	void SendToGPU();
	void Initialise(unsigned int programId, unsigned int textureID);
	void Draw(int location, int colourLoc, unsigned int programId);
	void CheckForInput();
	Particle* GetParticle() { return particlePTR; }
	//virtual void Setup();
	void Update(float) override;



	//void SetIDs(unsigned int, unsigned int, unsigned int); // (VAO, VBO, IBO) 
	//void updateModelMatrix(unsigned int, float);           //(unsigned int modelViewMatLoc,float d)
	//void Draw();
};

