#version 430 core

#define FIELD 0
#define SKY 1
#define SPHERE 2
#define CUE 3
#define POOLTABLE 4
#define START 5
#define TREE 6 

layout(location=0) in vec4 Coords;
layout(location=1) in vec2 TexCoords;
layout(location=2) in vec4 sphereCoords;
layout(location=3) in vec3 sphereNormals;

layout(location=4) in vec3 objCoords;
layout(location=5) in vec3 objNormals;
layout(location=6) in vec2 objTexCoords;
layout(location=7) in vec3 skyCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;
uniform float yPos;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform vec3 eyePos;

out vec2 texCoordsExport;
out vec3 normalExport;
out vec3 eyeExport;
out vec3 modelExport;
out vec3 SkytexCoordsExport;

vec4 coords;

void main(void)
{   
   if (object == FIELD)
   {
      coords = Coords;
      texCoordsExport = TexCoords;
   }
   if (object == SKY)
   {   
      SkytexCoordsExport = skyCoords;
      coords = vec4(skyCoords, 1.0);
   }
   if (object == SPHERE)
   {
      coords = sphereCoords;
      normalExport = sphereNormals;
      //InstanceID = gl_InstanceID;
      //coords = vec4(sphereCoords.x+(gl_InstanceID*12 - 18),sphereCoords.y,sphereCoords.z,sphereCoords.w);
      texCoordsExport = TexCoords;
   }

    if (object == CUE)
    {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
    }
    if (object == POOLTABLE)
    {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
    }
    if (object == START)
    {
      coords = Coords;
      texCoordsExport = TexCoords;
    }
    if (object == TREE)
    {
      coords = Coords;
      coords.y = 40.0 - Coords.y;
      texCoordsExport = TexCoords;
     mat4 modelView = viewMat* modelMat;
     // First colunm.
     modelView[0][0] = 1.0; 
     modelView[0][1] = 0.0; 
     modelView[0][2] = 0.0; 

     // Column 1:
     modelView[1][0] = 0;
     modelView[1][1] = 1;
     modelView[1][2] = 0;

     // Thrid colunm.
     modelView[2][0] = 0.0; 
     modelView[2][1] = 0.0; 
     modelView[2][2] = 1.0; 

     gl_Position = projMat * modelView * coords;
    }
   else{
   modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));
   eyeExport = eyePos;  

   gl_Position = projMat * viewMat* modelMat * coords;}
}
