#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
  vec4 pos = projection * view * model * vec4(aPos, 1.0);
  pos.z -= .01;
  gl_Position = pos;
  texCoord = texCord;
}
