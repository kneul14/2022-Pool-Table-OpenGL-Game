#include "StartScreen.h"

StartScreen::StartScreen()
{
}

StartScreen::~StartScreen()
{
}

void StartScreen::InitialiseStartScreen(unsigned int vao, unsigned int vbo)
{
	static Vertex startVertices[] =       //START SCREEN
	{
		{vec4(100.0, -50.0, -80, 1.0), vec2(1.0, 1.0)},    //bottom right
		{vec4(100.0, 70.0, -80, 1.0), vec2(1.0, 0.0)},  //top right
		{vec4(-100.0, -50.0, -80, 1.0), vec2(0.0, 1.0)},   //bottom left
		{vec4(-100.0, 70.0, -80, 1.0), vec2(0.0, 0.0)}, //top left
	};
	startVAO = vao;
	startVBO = vbo;
	glBindVertexArray(startVAO);
	glBindBuffer(GL_ARRAY_BUFFER, startVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(startVertices), startVertices, GL_STATIC_DRAW);	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(startVertices[0]), 0);  //layout(location=0) in vec4 fieldCoords;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(startVertices[0]), (void*)(sizeof(startVertices[0].coords)));  //layout(location=1) in vec2 fieldTexCoords;
	glEnableVertexAttribArray(1);
}

void StartScreen::Initialise(unsigned int programId, unsigned int textureID)
{
	std::string startTextures[] =
	{
		"Textures/PoolTableLogo.bmp",
		//"Models/redBall.JPG"
	};

	glActiveTexture(GL_TEXTURE0 + textureID - 1);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	int width, height;

	unsigned char* data = SOIL_load_image(startTextures[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	unsigned int startTexLoc = glGetUniformLocation(programId, "startTex");
	glUniform1i(startTexLoc, textureID - 1); //send texture to shader
}

void StartScreen::Draw(unsigned int programId)
{
	glBindVertexArray(startVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glBindVertexArray(0);
}

void StartScreen::SetViewMatrix(unsigned int modelViewMatLoc, glm::mat4 modelViewMat)
{
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}
