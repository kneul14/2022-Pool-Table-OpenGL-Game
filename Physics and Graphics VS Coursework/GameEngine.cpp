#include "GameEngine.h"

vector<GameObject*> GameEngine::objects;
vector<CollisionData*> GameEngine::collsionData;
int GameEngine::oldTimeSinceStart;
int GameEngine::newTimeSinceStart;
Camera GameEngine::camera; //access to camera object..
GameObject GameEngine::GOCode;
BroadPhaseGrid* GameEngine::broadPhaseGridCode;
ForceGenerator GameEngine::forceGen;
Particle* GameEngine::particlePTR;
GameObject* GameEngine::RegObj;
bool GameEngine::showLogo = true;
bool GameEngine::isGamePlayable = false;
//int GameEngine::currentScore;

static enum object { FIELD, SKY, SPHERE, CUE, POOLTABLE, START, TREE }; // VAO ids.
static enum buffer { FIELD_VERTICES, SKY_VERTICES, SPHERE_VERTICES, CUE_VERTICES, POOLTABLE_VERTICES, START_VERTICES, TREE_VERTICES }; // VBO ids.

struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);
static mat3 normalMat = mat3(1.0);  //create normal matrix

static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);
// Front and back material properties.
static const Material sphereFandB =
{
	vec4(1.0, 1.0, 1.0, 1.0), // ambient reflection
	vec4(1.0, 1.0, 1.0, 1.0), // diffuse reflection
	vec4(10.0, 10.0, 10.0, 1.0), // specular reflection
	vec4(0.0, 0.0, 0.0, 1.0), // emit
	100.0f                    // shininess
};

static const Material cueFandB =
{
	vec4(1.0, 1.0, 1.0, 1.0), // ambient reflection
	vec4(1.0, 1.0, 1.0, 1.0), // diffuse reflection
	vec4(1.0, 1.0, 1.0, 1.0), // specular reflection
	vec4(0.0, 0.0, 0.0, 1.0), // emit
	50.0f                     // shininess
};

static const Light light0 =
{
	vec4(0.0, 0.0, 0.0, 1.0), // ambient
	vec4(1.0, 1.0, 1.0, 1.0), // specular
	vec4(1.0, 1.0, 1.0, 1.0), // diffuse
	vec4(1.0, 1.0, 0.0, 0.0) // direction 
};

static const Light light1 =
{
	vec4(0.0, 0.0, 0.0, 1.0), // ambient
	vec4(0.0, 0.0, 0.0, 1.0), // specular
	vec4(0.0, 0.0, 0.0, 1.0), // diffuse
	vec4(1.0, 1.0, 0.0, 0.0) // direction 
};

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
modelMatLoc,
viewMatLoc,
objectLoc,
sphereColour,
redBall,
yellowBall,
whiteBall,
blackBall,
grassTexLoc,  //for grass texture 
skyTexLoc,
woodTexLoc,
cueTexLoc,
startTexLoc,
sphereRedTexLoc,
buffer[10],    //VBO List
vao[10],	     //VAO List
texture[8];   //Texture List


std::vector<GameObject*> objects;  //change name later
GameObject* cue;
GameObject* poolTable;
GameObject* GO;
int startingScore = 0;
int currentScore;
static Skybox skybox;
static Field field;
static StartScreen startScreen;
static BillBoards tree;
static BillBoards tree2;
float d = 0.0; //Camera position

void GameEngine::StartEngine()
{
	std::cout << "Press escape to exit the game." << std::endl;
	glutMainLoop();
	startingScore = currentScore;
}

void GameEngine::UpdateGame(void)
{
	oldTimeSinceStart = newTimeSinceStart;
	newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);

	float deltaTime = (newTimeSinceStart - oldTimeSinceStart);
	deltaTime /= 1000.f;

	forceGen.Update(deltaTime);

	std::vector<CollisionData*> collisions;


	for (int i = 0; i < objects.size() - 1; ++i)
	{
		if (objects[i]->sphereColliderPTR != nullptr)
		{
			for (int a = 0; a < objects.size(); ++a)
			{
				bool isACollider = (objects[a]->sphereColliderPTR != nullptr && i != a);

				if (isACollider)
				{
					CollisionData* data = nullptr;
					data = new CollisionData;

					bool checked = objects[i]->sphereColliderPTR->SphereCollideCheck(objects[a]->sphereColliderPTR, data);
					if (checked)
					{
						data->a = objects[i];
						data->b = objects[a];
						data->c = objects[i]->sphereColliderPTR;
						collisions.push_back(data);
					}
						//delete data;
				}
			}
		}
		if (objects[i]->aabbColliderPTR != nullptr)
		{
			//std::cout << "has aabb collider" << std::endl;
			for (int a = 0; a < objects.size(); ++a)
			{
				bool isACollider = (objects[a]->sphereColliderPTR != nullptr && i != a);

				if (isACollider)
				{
					CollisionData* data = nullptr;
					data = new CollisionData;
					data->a = objects[i];
					data->b = objects[a];
					data->c = objects[i]->aabbColliderPTR;


					bool checked = objects[i]->aabbColliderPTR->SpherexAABBCollideCheck(objects[a]->sphereColliderPTR, data);
					if (checked)
					{
						collisions.push_back(data);
					}
						//delete data;
				}
			}
		}
	}

	for (int i = 0; i < collisions.size(); i++)
	{
		ImpulseCalculator(collisions[i]);
	}
	collisions.clear();

	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->Update(deltaTime);
	}

	Cue* dynamicCasted = dynamic_cast<Cue*>(cue);
	dynamicCasted->Update(deltaTime);

	// refresh screen 
	glutPostRedisplay();
}

void GameEngine::ImpulseCalculator(CollisionData* collsionData)
{
	float cor = glm::dot(-(collsionData->a->particlePTR->velocity - collsionData->b->particlePTR->velocity), collsionData->collisonNormal);
	cor *= (1 + 0.0f); //Adds coeficient of restitution

	float num = glm::dot(collsionData->collisonNormal, collsionData->collisonNormal);
	num *= (1 / collsionData->a->particlePTR->mass + 1 / collsionData->b->particlePTR->mass);

	float impulse = 0; //if this calculation isn't here and the normals are equal to 0 the GOs just disappear
	if (cor != 0 && num != 0)
		impulse = cor / num;;


	//Applies correct force for collision response
	if (strstr(collsionData->a->tag.c_str(), "Ball") || strstr(collsionData->a->tag.c_str(), "Cue"))
	{
		collsionData->a->particlePTR->velocity += (impulse / collsionData->a->particlePTR->mass) * collsionData->collisonNormal;;
		collsionData->a->particlePTR->position += (collsionData->interPenetrationDepth / 2.0f) * collsionData->collisonNormal;;
	}
	if (strstr(collsionData->b->tag.c_str(), "Ball") || strstr(collsionData->b->tag.c_str(), "Cue"))
	{
		collsionData->b->particlePTR->velocity -= (impulse / collsionData->b->particlePTR->mass) * collsionData->collisonNormal;;
		collsionData->b->particlePTR->position -= (collsionData->interPenetrationDepth / 2.0f) * collsionData->collisonNormal;;
	}	
	if (strstr(collsionData->a->tag.c_str(), "Pot"))	{
		
		collsionData->b->particlePTR->isPotted = true;
		ScoreCalculator(collsionData);

		//the balls are falling a little bit early (before the pocket) so I wanted to only make them fall in the positon of the pocket
		//collsionData->b->particlePTR->position = collsionData->a->sphereColliderPTR->offset;
	}
	else {
		collsionData->a->particlePTR->hasGravity = false;
		collsionData->b->particlePTR->hasGravity = false;
	}
}

void GameEngine::ScoreCalculator(CollisionData* collsionData)
{
	currentScore = currentScore + collsionData->b->ballScore;
	std::cout << "The score is  " << currentScore << std::endl;
}

void GameEngine::Setup()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// Create shader program executable.
	vertexShaderId = setShader("vertex", "vertexShader.glsl");
	fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);

	//codes for OpenGL lighting
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1, &sphereFandB.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1, &sphereFandB.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1, &sphereFandB.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1, &sphereFandB.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), sphereFandB.shininess);

	glUniform4fv(glGetUniformLocation(programId, "cueFandB.ambRefl"), 1, &sphereFandB.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "cueFandB.difRefl"), 1, &sphereFandB.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "cueFandB.specRefl"), 1, &sphereFandB.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "cueFandB.emitCols"), 1, &sphereFandB.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "cueFandB.shininess"), sphereFandB.shininess);

	glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

	glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);


	///// Create a field  ///////////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[FIELD]);
	glGenBuffers(1, &buffer[FIELD_VERTICES]);
	field.InitialiseField(vao[FIELD], buffer[FIELD_VERTICES]);

	///// Create a Sky /////////////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[SKY]);
	glGenBuffers(1, &buffer[SKY_VERTICES]);
	skybox.InitialiseSkybox(vao[SKY], buffer[SKY_VERTICES]);

	/////Create BroadPhase? /////
	broadPhaseGridCode = new BroadPhaseGrid(1000, 1000, 1000, 10);
	broadPhaseGridCode->GetCell(-1,0,0);

	/////Create spheres /////  

	#pragma region Pool Ball Instances:) //(up, down, +left -right) 
	GameObject* RedBall1 = new RedBall(2.0f, 2.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0f, -0.5f, -6.0f), "Red Ball 1");
	GameObject* RedBall2 = new RedBall(2.0f, 2.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(-0.6, -0.5, -7), "Red Ball 2");
	GameObject* RedBall3 = new RedBall(2.0f, 2.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.1, -0.5, -8), "Red Ball 3");
	GameObject* RedBall4 = new RedBall(2.0f, 2.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.5, -0.5, -9), "Red Ball 4");
	GameObject* RedBall5 = new RedBall(2.0f, 2.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(-1.7, -0.5, -9), "Red Ball 5");
	GameObject* RedBall6 = new RedBall(2.0f, 2.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(2.2, -0.5, -10), "Red Ball 6");
	GameObject* RedBall7 = new RedBall(2.0f, 2.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(0, -0.5, -10), "Red Ball 7");

	GameObject* YellowBall1 = new YellowBall(2.0f, 2.0f, glm::vec3(1.0, 1.0, 0.0), glm::vec3(0.6, -0.5, -7), "Yellow Ball 1");
	GameObject* YellowBall2 = new YellowBall(2.0f, 2.0f, glm::vec3(1.0, 1.0, 0.0), glm::vec3(-1.1, -0.5, -8), "Yellow Ball 2");
	GameObject* YellowBall3 = new YellowBall(2.0f, 2.0f, glm::vec3(1.0, 1.0, 0.0), glm::vec3(1.6, -0.5, -9), "Yellow Ball 3");
	GameObject* YellowBall4 = new YellowBall(2.0f, 2.0f, glm::vec3(1.0, 1.0, 0.0), glm::vec3(-0.6, -0.5, -9), "Yellow Ball 4");
	GameObject* YellowBall5 = new YellowBall(2.0f, 2.0f, glm::vec3(1.0, 1.0, 0.0), glm::vec3(1.1, -0.5, -10), "Yellow Ball 5");
	GameObject* YellowBall6 = new YellowBall(2.0f, 2.0f, glm::vec3(1.0, 1.0, 0.0), glm::vec3(-1.1, -0.5, -10), "Yellow Ball 6");
	GameObject* YellowBall7 = new YellowBall(2.0f, 2.0f, glm::vec3(1.0, 1.0, 0.0), glm::vec3(-2.2, -0.5, -10), "Yellow Ball 7");

	GameObject* WhiteCueBall1 = new WhiteCueBall(2.0f, 2.0f, glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, -0.5, 8.5), "White Cue Ball 1");
	GameObject* BlackBall1 = new BlackBall(2.0f, 2.0f, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -0.5, -8), "Black Ball 1");

	GameObject* GO = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0),  "GO");
	GameObject* GO1 = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "GO");
	GameObject* GO2 = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "GO");
	GameObject* GO3 = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "GO");
	GameObject* GO4 = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "GO");
	GameObject* GO5 = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "GO");
	GameObject* GO6 = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "GO");

	GameObject* GOa = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "Pot");
	GameObject* GOb = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "Pot");
	GameObject* GOc = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "Pot");
	GameObject* GOd = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "Pot");
	GameObject* GOe = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "Pot");
	GameObject* GOf = new GameObject(2.0f, 0.0f, 0, glm::vec3(0.0, 0.0, 0.0), "Pot");
	
	cue = new Cue("Models/PoolCue.obj", 2.0f, 2.0f, vec3(0, 0, 9), "Cue");
	Cue* dynamicCasted = dynamic_cast<Cue*>(cue);
	poolTable = new PoolTable("Models/PoolTable.obj", 10.0f, 1.0f, vec3(0, 0, 0), "GameObject");

	#pragma region colliders
	RedBall1->sphereColliderPTR = new SphereCollider(0.5f, RedBall1->particlePTR->position, vec3(0, 0, 0), RedBall1->tag);
	RedBall2->sphereColliderPTR = new SphereCollider(0.5f, RedBall2->particlePTR->position, vec3(0, 0, 0), RedBall2->tag);
	RedBall3->sphereColliderPTR = new SphereCollider(0.5f, RedBall3->particlePTR->position, vec3(0, 0, 0), RedBall3->tag);
	RedBall4->sphereColliderPTR = new SphereCollider(0.5f, RedBall4->particlePTR->position, vec3(0, 0, 0), RedBall4->tag);
	RedBall5->sphereColliderPTR = new SphereCollider(0.5f, RedBall5->particlePTR->position, vec3(0, 0, 0), RedBall5->tag);
	RedBall6->sphereColliderPTR = new SphereCollider(0.5f, RedBall6->particlePTR->position, vec3(0, 0, 0), RedBall6->tag);
	RedBall7->sphereColliderPTR = new SphereCollider(0.5f, RedBall7->particlePTR->position, vec3(0, 0, 0), RedBall7->tag);

	YellowBall1->sphereColliderPTR = new SphereCollider(0.5f, YellowBall1->particlePTR->position, vec3(0, 0, 0), YellowBall1->tag);
	YellowBall2->sphereColliderPTR = new SphereCollider(0.5f, YellowBall2->particlePTR->position, vec3(0, 0, 0), YellowBall2->tag);
	YellowBall3->sphereColliderPTR = new SphereCollider(0.5f, YellowBall3->particlePTR->position, vec3(0, 0, 0), YellowBall3->tag);
	YellowBall4->sphereColliderPTR = new SphereCollider(0.5f, YellowBall4->particlePTR->position, vec3(0, 0, 0), YellowBall4->tag);
	YellowBall5->sphereColliderPTR = new SphereCollider(0.5f, YellowBall5->particlePTR->position, vec3(0, 0, 0), YellowBall5->tag);
	YellowBall6->sphereColliderPTR = new SphereCollider(0.5f, YellowBall6->particlePTR->position, vec3(0, 0, 0), YellowBall6->tag);
	YellowBall7->sphereColliderPTR = new SphereCollider(0.5f, YellowBall7->particlePTR->position, vec3(0, 0, 0), YellowBall7->tag);

	WhiteCueBall1->sphereColliderPTR = new SphereCollider(0.5f, WhiteCueBall1->particlePTR->position, vec3(0, 0, 0), WhiteCueBall1->tag);
	BlackBall1->sphereColliderPTR = new SphereCollider(0.5f, BlackBall1->particlePTR->position, vec3(0, 0, 0), BlackBall1->tag);

	cue->sphereColliderPTR = new SphereCollider(0.5f, cue->particlePTR->position, vec3(0, -0.5f, 0), cue->tag);

	#pragma region Table Colliders
	GO->aabbColliderPTR = new AABBCollider(vec3(7, 5, 2), vec3(0, 0, 0), vec3(0, -0.5f, -19.5), "Wall");	//top side 
	GO1->aabbColliderPTR = new AABBCollider(vec3(9, 5, 2), vec3(0, 0, 0), vec3(0, -0.5f, 20.0), "Wall");	//bottom side
	GO2->aabbColliderPTR = new AABBCollider(vec3(1, 5, 7.5), vec3(0, 0, 0), vec3(-9, -0.5f, 8.75), "Wall");	//bottom left
	GO3->aabbColliderPTR = new AABBCollider(vec3(1, 5, 7.5), vec3(0, 0, 0), vec3(9, -0.5f, 8.75), "Wall");	//bottom right
	GO4->aabbColliderPTR = new AABBCollider(vec3(1, 5, 7.5), vec3(0, 0, 0), vec3(9, -0.5f, -8.75), "Wall");	//top right
	GO5->aabbColliderPTR = new AABBCollider(vec3(1, 5, 7.5), vec3(0, 0, 0), vec3(-9, -0.5f, -8.75), "Wall");//top left

	GO6->aabbColliderPTR = new AABBCollider(vec3(24, 1, 24), vec3(0, 0, 0), vec3(0, -2.5f, 0), "floor");		//Table Collider

	GOa->sphereColliderPTR = new SphereCollider(2.0f, vec3(0, 0, 0), vec3(-9, -0.5f, -18.5), "Pot");			//top right 
	GOb->sphereColliderPTR = new SphereCollider(2.0f, vec3(0, 0, 0), vec3(9, -0.5f, -18.5), "Pot");			//top right 
	GOc->sphereColliderPTR = new SphereCollider(1.0f, vec3(0, 0, 0), vec3(9, -0.5f, 0), "Pot");				//mid left 
	GOd->sphereColliderPTR = new SphereCollider(1.0f, vec3(0, 0, 0), vec3(-9, -0.5f, 0), "Pot");			//mid right 
	GOe->sphereColliderPTR = new SphereCollider(2.0f, vec3(0, 0, 0), vec3(-9, -0.5f, 18.5), "Pot");			//bot right 
	GOf->sphereColliderPTR = new SphereCollider(2.0f, vec3(0, 0, 0), vec3(9, -0.5f, 18.5), "Pot");			//bot right
	//GOa->aabbColliderPTR = new AABBCollider(vec3(70.5, 20, 1), vec3(0, 0, 0), vec3(0, -0.5f, 21), "Wall");	//top right
	//GOb->aabbColliderPTR = new AABBCollider(vec3(1, 20, 70.5), vec3(0, 0, 0), vec3(11, -0.5f, 0), "Wall");	//top right
	#pragma endregion
#pragma endregion

#pragma endregion
	#pragma region PoolBall Push Backs
	objects.push_back(RedBall1);
	objects.push_back(RedBall2);
	objects.push_back(RedBall3);
	objects.push_back(RedBall4);
	objects.push_back(RedBall5);
	objects.push_back(RedBall6);
	objects.push_back(RedBall7);

	objects.push_back(YellowBall1);
	objects.push_back(YellowBall2);
	objects.push_back(YellowBall3);
	objects.push_back(YellowBall4);
	objects.push_back(YellowBall5);
	objects.push_back(YellowBall6);
	objects.push_back(YellowBall7);

	objects.push_back(BlackBall1);
	objects.push_back(WhiteCueBall1);
	objects.push_back(cue);
	objects.push_back(poolTable);
	objects.push_back(GO);
	objects.push_back(GO1);
	objects.push_back(GO2);
	objects.push_back(GO3);
	objects.push_back(GO4);
	objects.push_back(GO5);
	objects.push_back(GO6);

	objects.push_back(GOa);
	objects.push_back(GOb);
	objects.push_back(GOc);
	objects.push_back(GOd);
	objects.push_back(GOe);
	objects.push_back(GOf);

	//for (int i = 0; i < objects.size(); ++i)
	//{
	//	objects.push_back();
	//}

	//forceGen.AddObjectsToFunc(WhiteCueBall1);
#pragma endregion

	/////Create models /////

	glGenVertexArrays(1, &vao[CUE]);
	glGenBuffers(1, &buffer[CUE_VERTICES]);
	dynamicCasted->SetIDs(vao[CUE], buffer[CUE_VERTICES], 0);
	dynamicCasted->Setup();

	glGenVertexArrays(1, &vao[POOLTABLE]);
	glGenBuffers(1, &buffer[POOLTABLE_VERTICES]);
	PoolTable* dynamicCastedPoolTable = dynamic_cast<PoolTable*>(poolTable);
	dynamicCastedPoolTable->SetIDs(vao[POOLTABLE], buffer[POOLTABLE_VERTICES], 0);
	dynamicCastedPoolTable->Setup();

	///// Create a field  ///////////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[START]);
	glGenBuffers(1, &buffer[START_VERTICES]);
	startScreen.InitialiseStartScreen(vao[START], buffer[START_VERTICES]);

	///// Create a tree  ///////////

		//Create VAO and VBO
	glGenVertexArrays(1, &vao[TREE]);
	glGenBuffers(1, &buffer[TREE_VERTICES]);
	tree.InitialiseBillBoards(vao[TREE], buffer[TREE_VERTICES]);

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[TREE]);
	glGenBuffers(1, &buffer[TREE_VERTICES]);
	tree2.InitialiseBillBoardsTwo(vao[TREE], buffer[TREE_VERTICES]);

	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat");   //uniform mat4 projMat;
	projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 1000.0); /*perspective(radians(60.0), 1.0, 0.1, 1000.0); *///
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

	// Obtain modelview matrix uniform and object uniform locations.
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");   //uniform mat4 modelViewMat;
	modelMatLoc = glGetUniformLocation(programId, "modelMat");   //uniform mat4 modelViewMat;
	viewMatLoc = glGetUniformLocation(programId, "viewMat");
	objectLoc = glGetUniformLocation(programId, "object");  //uniform uint object;
	grassTexLoc = glGetUniformLocation(programId, "grassTex");
	sphereColour = glGetUniformLocation(programId, "sphereColour");
	redBall = glGetUniformLocation(programId, "redBall");
	yellowBall = glGetUniformLocation(programId, "yellowBall");
	whiteBall = glGetUniformLocation(programId, "whiteBall");
	blackBall = glGetUniformLocation(programId, "blackBall");
	skyTexLoc = glGetUniformLocation(programId, "skyTex");
	cueTexLoc = glGetUniformLocation(programId, "cueTex");

	// Load the images.
	std::string TexNames[] = {
		 "Textures/grass.bmp",
		 "Textures/top.jpg",
		 "Models/PoolTable_Texture.bmp",
		 "Textures/PoolTableLogo.bmp",
		 //"Models/redBall.JPG"
		 "Models/earth.JPG"
		 "Models/CueMaterial.bmp",
	};

	// Create texture ids.
	glGenTextures(8, texture);

	int width, height;
	unsigned char* data;

	///RED BALL TEXTURE///

	// Bind Table image.
	glActiveTexture(GL_TEXTURE0 + texture[6] - 1);
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	//load image data using SOIL library
	data = SOIL_load_image(TexNames[4].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	sphereRedTexLoc = glGetUniformLocation(programId, "sphereRedTex");
	glUniform1i(sphereRedTexLoc, texture[6] - 1); //send texture to shader	///RED BALL TEXTURE///

	///FIELD TEXTURE///
	field.Initialise(programId, texture[0]);
	///START TEXTURE/// 
	startScreen.Initialise(programId, texture[1]);
	///POOLTABLE TEXTURE///
	dynamicCastedPoolTable->Initialise(programId, texture[2]);
	///SKYBOX TEXTURE///
	skybox.InitialiseCubeMap(programId, texture[3]);
	///Cue TEXTURE///
	dynamicCasted->Initialise(programId, texture[4]);
	///SKYBOX TEXTURE///
	tree.Initialise(programId, texture[5]);
	tree2.InitialiseTwo(programId, texture[7]);

	glm::mat4 viewMat = camera.WhichCamera(d);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, value_ptr(viewMat));
}

void GameEngine::DrawIntro(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate and update modelview matrix.
	glm::mat4 viewMat = camera.WhichCamera(d); //sends Update Loc to the GPU
	camera.isCamera6 = true;
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, value_ptr(viewMat)); //The updated location has been sent to the GPU now

	// Draw field.
	glUniform1ui(objectLoc, START);  //if (object == FIELD)
	glBindVertexArray(vao[START]);
	if (showLogo)
	{
		startScreen.SetViewMatrix(modelMatLoc, glm::mat4(1.0f));
		startScreen.Draw(programId);
	}

	//Draw SkyBox
	glUniform1ui(objectLoc, SKY);  //if (object == SKYBOX)
	skybox.SetViewMatrix(modelMatLoc, glm::mat4(1.0f));
	skybox.Draw(programId);

	glutSwapBuffers();
	glutPostRedisplay();
}

void GameEngine::DrawGame(void)
{
	camera.isCamera6 = false;
	//if (isGamePlayable == true)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate and update modelview matrix.
		glm::mat4 viewMat = camera.WhichCamera(d); //sends Update Loc to the GPU
		glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, value_ptr(viewMat)); //The updated location has been sent to the GPU now

		// Draw field.
		glUniform1ui(objectLoc, FIELD);  //if (object == FIELD)
		field.SetViewMatrix(modelMatLoc, glm::mat4(1.0f));
		field.Draw(programId);

		// Draw sky.
		//Calculate and update modelview matrix.
		modelViewMat = mat4(1.0);
		modelViewMat = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0 + d, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

		//Draw SkyBox
		glUniform1ui(objectLoc, SKY);  //if (object == SKYBOX)
		skybox.SetViewMatrix(modelMatLoc, glm::mat4(1.0f));
		skybox.Draw(programId);

		// Draw sphere
		#pragma region PoolBalls //Balls Drawn

		for (int i = 0; i < objects.size(); ++i)
		{
			glUniform1ui(objectLoc, SPHERE);  //if (object == SPHERE)	 
			objects[i]->Draw(modelMatLoc, sphereColour);
		}

#pragma endregion

		// Draw my Cue
		Cue* dynamicCasted = dynamic_cast<Cue*>(cue);
		vec3 pos = vec3(0.0, 0.0, 0.0f);
		dynamicCasted->updateModelMatrix(modelMatLoc, 0.0f, 1.0f, pos);
		glUniform1ui(objectLoc, CUE);  //if (object == cue)
		dynamicCasted->Draw();

		// Draw PoolTable
		PoolTable* dynamicCastedPoolTable = dynamic_cast<PoolTable*>(poolTable);
		pos = vec3(0.0, 0.0, 0.0f);
		dynamicCastedPoolTable->updateModelMatrix(modelMatLoc, 0.0f, 1.0f, pos);
		glUniform1ui(objectLoc, POOLTABLE);  //if (object == POOLTABLE)
		dynamicCastedPoolTable->Draw();

		// Draw field.
		glUniform1ui(objectLoc, TREE);  //if (object == FIELD)
		tree.SetViewMatrix(modelMatLoc, glm::mat4(1.0f));
		tree.Draw(programId);

		// Draw field.
		glUniform1ui(objectLoc, TREE);  //if (object == FIELD)
		tree2.SetViewMatrix(modelMatLoc, glm::mat4(1.0f));
		tree2.Draw(programId);

		glutSwapBuffers();
	}
}

void GameEngine::CleanUpEngine()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		delete objects[i];
	}
}

void GameEngine::AddGameObject(GameObject* object)
{
	objects.push_back(object);
}

void GameEngine::ResizeWindow(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void GameEngine::InitEngine(int argc, char** argv, const char* windowTitle, int width, int height) //Basically the Main
{
	//printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(350, 0);
	glutCreateWindow("Katherine's PoolGame");

	glutDisplayFunc(DrawIntro); //this was for the Graphics coursework
	//glutDisplayFunc(DrawGame); //this was for the Phyics scene
	glutReshapeFunc(ResizeWindow);
	glutKeyboardFunc([](unsigned char key, int x, int y)
		{
			GameObject::keys[key] = true;
			switch (key)
			{
			case 13:
				showLogo = !showLogo;
				isGamePlayable = true;
				glutDisplayFunc(DrawGame);
				break;
			case 27:
				exit(0);
				break;
			case 49:
				std::cout << "Camera 1" << std::endl;
				Camera::isCamera1 = true;
				Camera::isCamera2 = false;
				Camera::isCamera3 = false;
				Camera::isCamera4 = false;
				Camera::isCamera5 = false;
				break;
			case 50:
				std::cout << "Camera 2" << std::endl;
				Camera::isCamera1 = false;
				Camera::isCamera2 = true;
				Camera::isCamera3 = false;
				Camera::isCamera4 = false;
				Camera::isCamera5 = false;
				break;
			case 51:
				std::cout << "Camera 3" << std::endl;
				Camera::isCamera1 = false;
				Camera::isCamera2 = false;
				Camera::isCamera3 = true;
				Camera::isCamera4 = false;
				Camera::isCamera5 = false;
				break;
			case 52:
				std::cout << "Camera 4" << std::endl;
				Camera::isCamera1 = false;
				Camera::isCamera2 = false;
				Camera::isCamera3 = false;
				Camera::isCamera4 = true;
				Camera::isCamera5 = false;
				break;
			case 53:
				std::cout << "Camera 5" << std::endl;
				Camera::isCamera1 = false;
				Camera::isCamera2 = false;
				Camera::isCamera3 = false;
				Camera::isCamera4 = false;
				Camera::isCamera5 = true;
				break;
			case 119: //move forward
				if (Camera::isCamera4 == true)
				{
					std::cout << "Camera 4 + W" << std::endl;
					camera.cameraPos += camera.cameraSpeed * camera.cameraFront;
					camera.UpdateCamera();
				}
				break;
			case 83:  //move backward
				if (Camera::isCamera4 == true)
				{
					camera.cameraPos -= camera.cameraSpeed * camera.cameraFront;
					camera.UpdateCamera();
				}
				break;
			case 97:  //move left
				if (Camera::isCamera4 == true)
				{
					camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * camera.cameraSpeed;
					camera.UpdateCamera();
				}
				break;
			case 68:  //move right
				if (Camera::isCamera4 == true)
				{
					camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * camera.cameraSpeed;
					camera.UpdateCamera();
				}
				break;
			case 114:  //move right
				{					
					//particlePTR->hasGravity = false; //Adds gravity to the balls
				}
				break;
			default:
				break;
			}
		});
	glutKeyboardUpFunc([](unsigned char key, int x, int y)
		{
			GameObject::keys[key] = false;
		});
	glutSpecialFunc([](int key, int x, int y)
		{
			GameObject::specialKeys[key] = true;  ///will need setting to false check lesson 4
			switch (key)
			{
			case GLUT_KEY_UP:
				break;
			case GLUT_KEY_DOWN:
				break;
			case GLUT_KEY_LEFT:
				if (d > -50.0) d -= 0.1;
				break;
			case GLUT_KEY_RIGHT:
				if (d < 15.0) d += 0.1;
				break;
			default:
				break;
			}
			glutPostRedisplay();
		});
	glutSpecialUpFunc([](int key, int x, int y)
		{
			GameObject::specialKeys[key] = false;
		});
	glutIdleFunc(UpdateGame);

	glm::mat4 viewMat = camera.WhichCamera(d); //should change Camera positions
	camera.UpdateCamera();
	glewExperimental = GL_TRUE;
	glewInit();

	GameEngine::Setup();
}