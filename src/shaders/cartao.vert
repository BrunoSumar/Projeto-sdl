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
  mat4 pvm = projection * view * model;
  vec4 ori = pvm * vec4(vec3(0), 1.0);
  vec4 topo = pvm * vec4(0., 2., 0., 1.0);
  gl_Position = ori + vec4( distance(ori, topo) * aPos / 2., 1.0);
  // gl_Position = ori + vec4( aPos / 1., 1.0);

  texCoord = texCord;
}
