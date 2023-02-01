#version 430 core

#define FIELD 0
#define SKY 1
#define SPHERE 2
#define CUE 3
#define POOLTABLE 4
#define START 5
#define TREE 6 

in vec2 texCoordsExport;
in vec3 normalExport;
in vec3 modelExport;
in vec3 eyeExport;
in vec3 SkytexCoordsExport;

struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};

struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};

uniform Light light0;
uniform vec4 globAmb;
uniform Material sphereFandB;
uniform Material cueFandB;

uniform sampler2D grassTex;
uniform sampler2D skyTex;
uniform sampler2D woodTex;
uniform sampler2D cueTex;
uniform sampler2D startTex;
uniform sampler2D BBTex;
//uniform sampler2D sphereTex;
//uniform sampler2D sphereRedTex;
//uniform sampler2D sphereYellowTex;
//uniform sampler2D sphereWhiteTex;
//uniform sampler2D sphereBlackTex;
uniform uint object;
uniform vec4 sphereColour;
uniform samplerCube skyboxTexture;

out vec4 colorsOut;

vec4 fieldTexColor, skyTexColor, woodTexColor, cueTexColor, skyBoxTexColor, startTexColor, sphereTexColor, sphereRedTexColor, sphereYellowTexColor, sphereWhiteTexColor, sphereBlackTexColor, BBTexColor;
vec3 normal, lightDirection;
vec3 eyeDirection;
vec3 halfway;
vec4 fAndBDif;
vec4 fAndBSpec;

void main(void)
{  
   fieldTexColor = texture(grassTex, texCoordsExport);
   skyTexColor = texture(skyTex, texCoordsExport);
   skyBoxTexColor = texture(skyboxTexture, SkytexCoordsExport);
   woodTexColor = texture(woodTex, texCoordsExport);
   startTexColor = texture(startTex, texCoordsExport);
   //sphereRedTexColor = texture(sphereRedTex, texCoordsExport);
   cueTexColor = texture(cueTex, texCoordsExport);
   BBTexColor = texture(BBTex, texCoordsExport);
   
   if (object == FIELD) colorsOut = fieldTexColor;
   if (object == START) colorsOut = startTexColor;
   if (object == SPHERE) 
	{
    normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); //diffuse and ambient
	//specular
	halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
	fAndBSpec = pow(max(dot(normal, halfway), 0.0f), sphereFandB.shininess) * light0.specCols * sphereFandB.specRefl;
	colorsOut =  sphereColour*vec4(vec3(min(fAndBDif + fAndBSpec, vec4(1.0))), 1.0);
   }
   if (object == CUE) {
    normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * cueFandB.difRefl); 
	halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
	fAndBSpec = pow(max(dot(normal, halfway), 0.0f), cueFandB.shininess) * light0.specCols * cueFandB.specRefl;
    colorsOut =  cueTexColor*vec4(vec3(min(fAndBDif + fAndBSpec, vec4(1.0))), 1.0);
   }

   if (object == POOLTABLE) {
    normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
	halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
	fAndBSpec = pow(max(dot(normal, halfway), 0.0f), sphereFandB.shininess) * light0.specCols * sphereFandB.specRefl;
    colorsOut =  woodTexColor*vec4(vec3(min(fAndBDif + fAndBSpec, vec4(1.0))), 1.0);
   }
   if (object == SKY) colorsOut = skyBoxTexColor; 
   if (object == TREE) {
      if(BBTexColor.a < 0.2)         discard;
      colorsOut = BBTexColor;
	  }
}