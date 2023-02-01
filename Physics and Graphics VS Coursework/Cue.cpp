#include "Cue.h"
#include "OBJloader.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

Cue::Cue(const char* name, float s, float m, vec3 pos, string tag) : Model(name, s, m, pos, tag)
{
	speed = s;
	mass = m;
	position = pos;
	VAO = VBO = 0;
	VerticesData = NULL;
	NumVert = 0;
	CreateObject(name);
}

Cue::~Cue()
{
	free(VerticesData);
	delete particlePTR;
	delete sphereColliderPTR;
}

void Cue::CreateObject(const char* name)
{
	std::vector<VertexWithAll> mesh = loadOBJ(name); //Import OBJ file data into a vector array
	NumVert = mesh.size();

	this->VerticesData = new VertexWithAll[NumVert];
	for (size_t i = 0; i < NumVert; i++)
	{
		this->VerticesData[i] = mesh[i];
	}
}

void Cue::SetPosition(vec3 newPos)
{
	position = newPos;
}

vec3 Cue::GetPosition(void)
{
	return particlePTR->position;
}

void Cue::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}

void Cue::updateModelMatrix(unsigned int modelViewMatLoc, float d, float scale, vec3 pos)
{
	ModelMatrix = mat4(1.0);
	ModelMatrix = glm::scale(ModelMatrix, vec3(scale, scale, scale));  //scale down the model
	ModelMatrix = glm::translate(ModelMatrix, pos);
	ModelMatrix = glm::translate(ModelMatrix, GetPosition());
	ModelMatrix = glm::rotate(ModelMatrix, (RB3DPTR->GetOrientation()), vec3(0, 1, 0)); //this really happens first
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix)); //send modelview matrix to the shader
}

void Cue::Setup()
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

void Cue::Initialise(unsigned int programId, unsigned int textureID)
{
	std::string CueTextures[] =
	{
		 "Models/CueMaterial.bmp",
	};

	glActiveTexture(GL_TEXTURE0 + textureID - 1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height;

	unsigned char* data = SOIL_load_image(CueTextures[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	unsigned int cueTexLoc = glGetUniformLocation(programId, "cueTex");
	glUniform1i(cueTexLoc, textureID - 1); //send texture to shader

}

void Cue::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, NumVert);
}
