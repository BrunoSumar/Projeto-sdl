#version 330 core
// uniform sampler2D tex;

in vec2 texCoord;

uniform vec3 Color;
uniform float Impacto;
uniform int Equipe;
uniform int Time;
uniform int Ocupado;

out vec4 FragColor;

vec2 rotate(vec2 v, float a) {
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, -s, s, c);
	return m * v;
}

float sdBox( vec2 p )
{
  p = abs(p - vec2(.5));
  return 2 * max(p.x, p.y);
}

float sdX(vec2 p, vec2 b){
  p = rotate(p - vec2(.5), 3.1415/4.);
  vec2 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,q.y),0.0);
}

void main()
{
  float dist = sdBox(texCoord);
  // float estado = max( Impacto - Time, 0.)/2.;
  float estado = Equipe == 2 ? 1.4 : .0;
  vec2 cross = vec2(.05, .3);

  float intensidade = Ocupado == 0 ? .5 : 1.;
  float x =  sdBox(texCoord)*2. - 1.7 + intensidade/5;
  // float x =  min( sdX(texCoord, estado * cross), sdX(texCoord, estado * cross.yx));

  FragColor = mix(vec4(Color, intensidade), vec4(Color, .0), step(.95, dist ) + step(.0, -x));
}
