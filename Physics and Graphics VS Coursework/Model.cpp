#include "Model.h"
#include "OBJloader.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

Model::Model(const char* name, float s, float m, vec3 pos, string tag) : GameObject(1.0f, 0.2f, 0, pos, tag)
{
	speed = s;
	mass = m;
	position = pos;
	position = vec3(0);
	VAO = VBO = 0;
	VerticesData = NULL;
	NumVert = 0;
	CreateObject(name);
	RB3DPTR = new RigidBody3D(1.0f, 1.0f, 1.0f, m, inertia, 1.0f, pos);
}

Model::~Model()
{
	free(VerticesData);
	delete RB3DPTR;
	delete sphereColliderPTR;
}

void Model::CreateObject(const char* name)
{
	std::vector<VertexWithAll> mesh = loadOBJ(name); //Import OBJ file data into a vector array
	NumVert = mesh.size();

	this->VerticesData = new VertexWithAll[NumVert];
	for (size_t i = 0; i < NumVert; i++)
	{
		this->VerticesData[i] = mesh[i];
	}
}

void Model::SetPosition(vec3 newPos)
{
	position = newPos;
}

vec3 Model::GetPosition(void)
{
	return position;
}

void Model::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}

void Model::Setup()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWithAll) * NumVert, VerticesData, GL_STATIC_DRAW);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, position));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, normal));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, textcoord));
	glEnableVertexAttribArray(6);

}

void Model::updateModelMatrix(unsigned int modelViewMatLoc, float d, float scale, vec3 offsetPos)
{

	ModelMatrix = mat4(1.0);
	ModelMatrix = glm::scale(ModelMatrix, vec3(scale, scale, scale));  //scale down the model
	ModelMatrix = glm::translate(ModelMatrix, offsetPos);
	ModelMatrix = glm::translate(ModelMatrix, GetPosition());
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader

}

void Model::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, NumVert);
}

void Model::CheckForInput()
{
	if (camera.isCamera3 == true) {
		//looking down at the Table
		if (GameObject::keys['w'])
			particlePTR->AddForce(glm::vec3(-10.0f * speed, 0.0f, 0.0f)); //makes go up
		if (GameObject::keys['a'])
			particlePTR->AddForce(glm::vec3(0.0f, 0.0f, 10.0f * speed)); //makes go left
		if (GameObject::keys['d'])
			particlePTR->AddForce(glm::vec3(0.0f, 0.0f, -10.0f * speed)); //makes go right
		if (GameObject::keys['s'])
			particlePTR->AddForce(glm::vec3(10.0f * speed, 0.0f, 0.0f)); //makes go down
		if (GameObject::keys['q'])
			RB3DPTR->AddForce(glm::vec3(20, 0, 0), glm::vec3(0, 1, 0)); //rotates to right
		if (GameObject::keys['e'])
			RB3DPTR->AddForce(glm::vec3(-20, 0, 0), glm::vec3(0, 1, 0)); //rotates to left
		if (GameObject::keys['r']) {
			//particlePTR->position = vec3(0, 0, 9);
			particlePTR->isPotted = false; //Adds gravity to the balls
			particlePTR->hasGravity = false;
		}
	}
	else {
		if(posi)
		{
			particlePTR->position = vec3(0, 0, 9);//if enabled the cue stays in place when striken
		}
		
		//looking at the Table
		if (GameObject::keys['w']) {
			posi = false;
			particlePTR->AddForce(glm::vec3(0.0f, 0.0f, -10.0f * speed)); //makes go forward
		}
		if (GameObject::keys['a']) {
			posi = false;
			particlePTR->AddForce(glm::vec3(-10.0f * speed, 0.0f, 0.0f)); //makes go left
		}
		if (GameObject::keys['d']) {
			posi = false;
			particlePTR->AddForce(glm::vec3(10.0f * speed, 0.0f, 0.0f)); //makes go right
		}
		if (GameObject::keys['s']) {
			posi = false;
			particlePTR->AddForce(glm::vec3(0.0f, 0.0f, 10.0f * speed)); //makes go back	
		}
		if (GameObject::keys['q']) {
			posi = false;
			RB3DPTR->AddForce(glm::vec3(20, 0, 0), glm::vec3(0, 1, 0)); //rotates to right
		}
		if (GameObject::keys['e']) {
			posi = false;
			RB3DPTR->AddForce(glm::vec3(-20, 0, 0), glm::vec3(0, 1, 0)); //rotates to left
		}
		if (GameObject::keys['t']) {
			particlePTR->AddForce(glm::vec3(0.0f, 0.0f, forceToHitWith * speed)); //strikes ball)
			posi = true;
		}
		if (GameObject::keys['r']) { // reset the position
			posi = true;
			particlePTR->isPotted = false; //Adds gravity to the balls
			particlePTR->hasGravity = false;
		}
	}
}

void Mouse(int button, int state, int x, int y) {
	switch (button)
	{
		case GLUT_LEFT_BUTTON: //was going to use this to take a shot:(
			cout << "hi" << endl; 
			break;
		case GLUT_RIGHT_BUTTON:
			cout << "bye" << endl;
			break;
		default:
			break;
	}
}

void Model::Update(float deltaTime)
{
	if (particlePTR->isPotted == true)
	{
		particlePTR->hasGravity = true;
	}
	RB3DPTR->ResetForce();
	CheckForInput();
	glutMouseFunc(Mouse);
	RB3DPTR->Update(deltaTime);

	particlePTR->ResetForce();
	CheckForInput();
	particlePTR->Update(deltaTime);
	

	if (sphereColliderPTR != NULL) {
		sphereColliderPTR->position = particlePTR->position + sphereColliderPTR->offset;
	}
	if (aabbColliderPTR != NULL) {
		aabbColliderPTR->position = particlePTR->position + aabbColliderPTR->offset;
	}
}