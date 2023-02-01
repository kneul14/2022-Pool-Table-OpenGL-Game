#include "PoolTable.h"
#include "OBJloader.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

PoolTable::PoolTable(const char* name, float s, float m, vec3 pos, string tag) : Model(name, s, m, pos, tag)
{
	speed = s;
	mass = m;
	position = pos;
	position = vec3(0);
	VAO = VBO = 0;
	VerticesData = NULL;
	NumVert = 0;
	CreateObject(name);

	//AABBCollider* top = new AABBCollider(vec3(6.5, 1, 1), particlePTR->position, vec3(0, -0.5f, -18.5), "Pool Table"); //top side 
	//AABBCollider* bottom = new AABBCollider(vec3(6.5, 1, 1), particlePTR->position, vec3(0, -0.5f, 18.5), "Pool Table"); //bottom side
	//AABBCollider* bLeft = new AABBCollider(vec3(1, 1, 24),  particlePTR->position, vec3(-9, -0.5f, 8.75), "Pool Table"); //bottom left
	//AABBCollider* bRight = new AABBCollider(vec3(1, 1, 7.5), particlePTR->position, vec3(9, -0.5f, 8.75), "Pool Table"); //bottom right
	//AABBCollider* tRight = new AABBCollider(vec3(1, 1, 7.5), particlePTR->position, vec3(9, -0.5f, -8.75), "Pool Table"); //top right
	//AABBCollider* tLeft = new AABBCollider(vec3(1, 1, 7.5), particlePTR->position, vec3(-9, -0.5f, -8.75), "Pool Table"); //top left

}

PoolTable::~PoolTable()
{
	free(VerticesData);
}

void PoolTable::CreateObject(const char* name)
{
	std::vector<VertexWithAll> mesh = loadOBJ(name); //Import OBJ file data into a vector array
	NumVert = mesh.size();

	this->VerticesData = new VertexWithAll[NumVert];
	for (size_t i = 0; i < NumVert; i++)
	{
		this->VerticesData[i] = mesh[i];
	}
}

void PoolTable::SetPosition(vec3 newPos)
{
	position = newPos;
}

vec3 PoolTable::GetPosition(void)
{
	return position;
}

void PoolTable::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}

void PoolTable::Setup()
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

void PoolTable::updateModelMatrix(unsigned int modelViewMatLoc, float d, float scale, vec3 offsetPos)
{
	ModelMatrix = mat4(1.0);
	ModelMatrix = glm::scale(ModelMatrix, vec3(scale, scale, scale));  //scale down the model
	ModelMatrix = glm::translate(ModelMatrix, offsetPos);
	ModelMatrix = glm::translate(ModelMatrix, GetPosition());
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader
}

void PoolTable::Initialise(unsigned int programId, unsigned int textureID)
{
	std::string PTTextures[] =
	{
		 "Models/PoolTable_Texture.bmp",
	};

	glActiveTexture(GL_TEXTURE0 + textureID - 1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height;

	unsigned char* data = SOIL_load_image(PTTextures[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	unsigned int woodTexLoc = glGetUniformLocation(programId, "woodTex");
	glUniform1i(woodTexLoc, textureID - 1); //send texture to shader
}

void PoolTable::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, NumVert);
}

//void PoolTable::Update(float deltaTime/*, glm::vec3 offset*/)
//{
//	//collider->Update(deltaTime, position, offset);
//}