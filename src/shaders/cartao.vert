#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

vec3 distort(float dist, vec3 point){
  return (.5 - .5*dist) * point;
}

void main()
{
    vec4 ori = projection * view * model * vec4(vec3(0), 1.0);
    gl_Position = ori + vec4(distort(ori.z, aPos), 1.0);
    texCoord = texCord;
}
