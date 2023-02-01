/////////////////////////////////////////////////////////////////////////////////          
// CreateSphere.cpp
//
// Forward-compatible core GL 4.3 version 
//
// Interaction:
// Press the up and down arrow keys to move the viewpoint over the field.
//
//
#include <cmath>
#include <fstream>
#include <iostream>
#include "GameEngine.h"

using namespace std;
using namespace glm;

int oldTimeSinceStart;
int newTimeSinceStart;
Camera camera; //access to camera object..
//GameObject* sphere = new Sphere;
GameObject* player =new Player(1.0f, 1.0f, glm::vec3(0, 0, 0));
GameObject GOCode;
ForceGenerator forceGen;
Particle* particlePTR;
GameObject* RegObj;
GameEngine engine;

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:" << endl;
	cout << "Press the left and right arrow keys to move the viewpoint over the field." << endl;
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();

	engine.InitEngine(argc, argv, "Katherine's PoolGame", 500, 500);

	engine.AddGameObject(player);
	//engine.AddGameObject(sphere);
	
	engine.StartEngine();

	return 0;
}

// need to make camera local to the ball