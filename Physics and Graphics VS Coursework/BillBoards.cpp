#include "BillBoards.h"

BillBoards::BillBoards()
{
}

BillBoards::~BillBoards()
{
}

void BillBoards::InitialiseBillBoards(unsigned int vao, unsigned int vbo)
{
	static Vertex treeVertices[] =       //START SCREEN
	{
	{vec4(40.0, 0.0, -30.0, 1.0), vec2(1.0, 0.0)},
	{vec4(40.0, 40.0, -30.0, 1.0), vec2(1.0, 1.0)},
	{vec4(-00.0, 0.0, -30.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-00.0, 40.0, -30.0, 1.0), vec2(0.0, 1.0)}
	};
	BBVAO = vao;
	BBVBO = vbo;
	glBindVertexArray(BBVAO);
	glBindBuffer(GL_ARRAY_BUFFER, BBVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(treeVertices), treeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(treeVertices[0]), 0);  //layout(location=0) in vec4 fieldCoords;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(treeVertices[0]), (void*)(sizeof(treeVertices[0].coords)));  //layout(location=1) in vec2 fieldTexCoords;
	glEnableVertexAttribArray(1);
}

void BillBoards::InitialiseBillBoardsTwo(unsigned int vao, unsigned int vbo)
{
	static Vertex treeVertices2[] =       //START SCREEN
	{
	{vec4(00.0, 0.0, -30.0, 1.0), vec2(1.0, 0.0)},
	{vec4(00.0, 40.0, -30.0, 1.0), vec2(1.0, 1.0)},
	{vec4(-40.0, 0.0, -30.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-40.0, 40.0, -30.0, 1.0), vec2(0.0, 1.0)}
	};
	BBVAO = vao;
	BBVBO = vbo;
	glBindVertexArray(BBVAO);
	glBindBuffer(GL_ARRAY_BUFFER, BBVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(treeVertices2), treeVertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(treeVertices2[0]), 0);  //layout(location=0) in vec4 fieldCoords;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(treeVertices2[0]), (void*)(sizeof(treeVertices2[0].coords)));  //layout(location=1) in vec2 fieldTexCoords;
	glEnableVertexAttribArray(1);
}

std::string BBTextures[] =
{
	"Textures/trees.png",
	"Textures/tree1.png",
	"Textures/tree2.png",
};

void BillBoards::Initialise(unsigned int programId, unsigned int textureID)
{
	glActiveTexture(GL_TEXTURE0 + textureID - 1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height;

	unsigned char* data = SOIL_load_image(BBTextures[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	unsigned int BBTexLoc = glGetUniformLocation(programId, "BBTex");
	glUniform1i(BBTexLoc, textureID - 1); //send texture to shader
}

void BillBoards::InitialiseTwo(unsigned int programId, unsigned int textureID)
{
	glActiveTexture(GL_TEXTURE0 + textureID - 1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height;

	unsigned char* data = SOIL_load_image(BBTextures[1].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	unsigned int BBTexLoc = glGetUniformLocation(programId, "BBTex");
	glUniform1i(BBTexLoc, textureID - 1); //send texture to shader
}

void BillBoards::Draw(unsigned int programId)
{
	glBindVertexArray(BBVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void BillBoards::SetViewMatrix(unsigned int modelViewMatLoc, glm::mat4 modelViewMat)
{
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}
