#include <cmath>
#include <fstream>
#include <iostream>
#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "soil/SOIL.h"

#include "camera.h"
#include "StartScreen.h"
#include "Field.h"
#include "BillBoards.h"
#include "GameObject.h"
#include "Particle.h"
#include "ForceGenerator.h"
#include "Player.h"
#include "RedBall.h"
#include "YellowBall.h"
#include "WhiteCueBall.h"
#include "BlackBall.h"
#include "SkyBox.h"
#include "Model.h"
#include "Cue.h"
#include "PoolTable.h"
#include "CollisionData.h"
#include "BroadPhaseGrid.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "shader.h"
#include "vertex.h"
#include "BGMusic.h"
#include <vector>
#include <glm/gtx/string_cast.hpp>

#pragma comment(lib, "glew32.lib") 
#endif

using namespace std;
using namespace glm;

class GameEngine
{
private:
	static vector<GameObject*> objects;
	static vector<CollisionData*> collsionData;
	static vector<CollisionData*> collisions;
	static int oldTimeSinceStart;
	static int newTimeSinceStart;

	void CleanUpEngine(); //Delete the objects afterwards.
	static void ResizeWindow(int w, int h); //The function to alter the window contents if the screen changes shape.
	static void DrawGame(void);
	static void DrawIntro(void);
	static void UpdateGame(void);
	static void ImpulseCalculator(CollisionData* collsionData);
	static void ScoreCalculator(CollisionData* collsionData);

public:
	static Camera camera; //access to camera object..
	static BGMusic bgMusic; //access to camera object..
	static GameObject GOCode;
	static BroadPhaseGrid* broadPhaseGridCode;
	static ForceGenerator forceGen;
	static Particle* particlePTR;
	static GameObject* RegObj;
	static bool showLogo;
	static bool isGamePlayable;
	//static int currentScore;
	GameObject* player = new Player(1.0f, 1.0f, glm::vec3(0, 0, 0));

	void InitEngine(int argc, char** argv, const char* windowTitle, int width, int height); //The function to set up/initialize Opengl and GLUT and the function to get things ready (but not run).
	void AddGameObject(GameObject*); //A way to add objects to the game world.
	void StartEngine(); //Start the game world.
	static void Setup();
};