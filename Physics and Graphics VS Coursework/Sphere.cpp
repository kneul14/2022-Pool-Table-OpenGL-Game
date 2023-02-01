#include <iostream>
#include "Sphere.h"
#include "soil/SOIL.h"

void Sphere::CreateSpherewithNormal(void)
{
	int count;
	count = 0;
	for (int i = 0; i <= stacks; ++i) {

		GLfloat V = i / (float)stacks;
		GLfloat phi = V * glm::pi <float>();

		// Loop Through Slices
		for (int j = 0; j <= slices; ++j) {

			GLfloat U = j / (float)slices;  //The parameter along all stacks, it range from 0 to 1
			GLfloat theta = U * (glm::pi <float>() * 2);  // The phi angle, it was converted to radian 

			// Calc The Vertex Positions
			GLfloat x = cosf(theta) * sinf(phi);
			GLfloat y = cosf(phi);
			GLfloat z = sinf(theta) * sinf(phi);

			sphereVerticesNor[count].coords = vec4(x * radius, y * radius + 2.0, z * radius, 1.0); //6.0 is just small tweak, you can remove it 
			sphereVerticesNor[count].normals = vec3(x, y, z); ///Sphere normals
			sphereVerticesNor[count].texcoords = vec2(U, V);
			count++;
		}
	}

	count = 0;
	// Calc The Index Positions
	for (int i = 0; i < slices * stacks + slices; ++i) {

		//The triangle index is explained in the lecture slides
		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i + slices;
		count++;

		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + 1;
		count++;
	}

	count = 0;
}

void Sphere::SendToGPU()
{
	//Create VAO and VBO
	glGenVertexArrays(1, &sphere_VAO);
	glGenBuffers(1, &sphere_Vertices);
	glGenBuffers(1, &sphere_Indices);

	//Binding VAO 
	glBindVertexArray(sphere_VAO);
	//Binding VBO
	glBindBuffer(GL_ARRAY_BUFFER, sphere_Vertices);
	//creates a new data store for the buffer object currently bound to GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWtihNormal) * 121, sphereVerticesNor, GL_STATIC_DRAW);
	//Binding Vertex array indices

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), (GLvoid*)sizeof(sphereVerticesNor[0].normals));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), (GLvoid*)offsetof(VertexWtihNormal, texcoords));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_Indices);
	//creates a new data store for the buffer object currently bound to GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 660, sphereIndices, GL_STATIC_DRAW);
}


void Sphere::Initialise(unsigned int programId, unsigned int textureID)
{
	//std::string sphereTextures[] =
	//{
	//	"Models/redBall.JPG",
	//};

	//glActiveTexture(GL_TEXTURE0 + textureID - 1);
	//glBindTexture(GL_TEXTURE_2D, textureID);

	//int width, height;

	//unsigned char* data = SOIL_load_image(sphereTextures[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//SOIL_free_image_data(data);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//unsigned int sphereRedTexLoc = glGetUniformLocation(programId, "sphereRedTex");
	//glUniform1i(sphereRedTexLoc, 0); //send texture to shader
}

Sphere::Sphere()
{
}

Sphere::Sphere(float s, float m, vec3 col, vec3 pos): GameObject(1.0f, 0.0f, 0, pos, tag)
{
	speed = s;
	mass = m;
	position = pos;
	Colour = col;
	//particlePTR->hasGravity = true; //Adds gravity to the balls
	stacks = 10; //number of stacks
	slices = 10; //number of slices
	radius = 0.5f; //made the radius smaller so that the blass are smaller in the scene and match the table properly.

	sphereVerticesNor = (VertexWtihNormal*)malloc(sizeof(VertexWtihNormal) * 121); //total number of vertices = (stacks+1)*(slices +1)
	sphereIndices = (unsigned int*)malloc(sizeof(unsigned int) * 660);
	CreateSpherewithNormal();
	SendToGPU();
}

Sphere::~Sphere()
{
	free(sphereVerticesNor);
	free(sphereIndices);
}

VertexWtihNormal* Sphere::GetVerData(int& verNum)
{
	verNum = 121;
	return sphereVerticesNor;
}

unsigned int* Sphere::GetTriData(int& triNum)
{
	triNum = 660;
	return sphereIndices;
}

void Sphere::SetPosition(vec3 newPos)
{
	position = newPos;
}

vec3 Sphere::GetPosition(void)
{
	return position;
}

void Sphere::Draw(int location, int colourLoc, unsigned int programId)
{	glm::mat4 modelMat = mat4(1.0);
	modelMat = glm::translate(modelMat, particlePTR->position);

	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelMat));
	glUniform4fv(colourLoc, 1, glm::value_ptr(Colour));

	glBindVertexArray(sphere_VAO);

	glDrawElements(GL_TRIANGLE_STRIP, 660, GL_UNSIGNED_INT, (void*)0);  ///use index array to control triangle drawing
	//glDrawElementsInstanced(GL_TRIANGLE_STRIP, 660, GL_UNSIGNED_INT, (void*)0, 4);  ///use index array to control triangle drawing
}

void Sphere::CheckForInput()
{
	//if (GameObject::specialKeys[GLUT_KEY_UP])
	//	particlePTR->AddForce(glm::vec3(-10.0f * speed, 0.0f, 0.0f)); //makes go up
	//if (GameObject::specialKeys[GLUT_KEY_LEFT])
	//	particlePTR->AddForce(glm::vec3(0.0f, 0.0f, 10.0f * speed)); //makes go left
	//	//std::cout << "Key pressed: " << glm::to_string(position) << " : " << std::endl;
	//if (GameObject::specialKeys[GLUT_KEY_RIGHT])
	//	particlePTR->AddForce(glm::vec3(0.0f, 0.0f, -10.0f * speed)); //makes go right
	//if (GameObject::specialKeys[GLUT_KEY_DOWN])
	//	particlePTR->AddForce(glm::vec3(10.0f * speed, 0.0f, 0.0f)); //makes go down
}

void Sphere::Update(float deltaTime)
{
	particlePTR->ResetForce();
	CheckForInput();
	particlePTR->Update(deltaTime);

	/*physicsBody->ResetTotalForces();

	float amount = 1.f * speed;

	if (GameObject::specialKeys[GLUT_KEY_UP] == true)
		physicsBody->AddForce(glm::vec3(0, 0, amount));
	if (GameObject::specialKeys[GLUT_KEY_DOWN] == true)
		physicsBody->AddForce(glm::vec3(0, 0, -amount));

	if (GameObject::specialKeys[GLUT_KEY_LEFT] == true)
		physicsBody->AddForce(glm::vec3(amount, 0, 0));
	if (GameObject::specialKeys[GLUT_KEY_RIGHT] == true)
		physicsBody->AddForce(glm::vec3(-amount, 0, 0));

	physicsBody->UpdateBody(deltaTime);*/
}
