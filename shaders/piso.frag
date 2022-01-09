#version 330 core
// uniform sampler2D tex;

in vec2 texCoord;

uniform vec3 color;

out vec4 FragColor;

float sdBox( vec2 p )
{
  p = abs(p - vec2(.5));
  return 2 * max(p.x, p.y);
}

void main()
{
  float dist = sdBox(texCoord);
  FragColor = mix(vec4(color, 0.1), vec4(color, 1.), smoothstep(.95, .97, dist));
  // FragColor = mix(vec4(color, 1.), vec4(0.), dist);
}
