#version 330 core
// uniform sampler2D tex;

in vec2 texCoord;

uniform vec3 color;

out vec4 FragColor;

float sdBox( vec2 p )
{
  vec2 q = abs(p) - vec2(.5, .5);
  return length(max(q,0.0)) + min(max(q.x,q.y),0.0);
}

void main()
{
  float dist = sdBox(texCoord);
  FragColor = mix(vec4(color, 1.), vec4(0.), dist);
}
