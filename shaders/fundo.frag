#version 330 core
uniform sampler2D tex;
uniform float Time;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
  float time = 6. * Time;

  vec2 uv = texCoord - .5;

  vec3 col = vec3(0);
  float s =.6;

  float a1 = 2.*3.1415-time*.02;
  float a2 = 3.1415/7.;
  vec2 dir = vec2(sin(a1),cos(a1));

  uv.y = abs(uv.y);
  uv.y -= .5;
  uv.x = -abs(uv.x);

  uv -= vec2(sin(a2),cos(a2))*min(0.,dot(uv+vec2(0,.5),vec2(sin(a2),cos(a2))))*2.;

  uv.x +=.5;

  for(int i=0;i<3;i++){
    s *= 3.;

    uv *= 3.;
    uv.x -= 1.5;

    uv.x = abs(uv.x);
    uv.x -= .5;
    uv -= dir*min(0.,dot(uv,dir))*2.;
  }

  float d = length(uv - vec2(clamp(uv.x, -1.,1.),0));
  d = smoothstep(.01*s, .0, d);
  col += texture(tex,-time*.03+uv/s).rgb;
  //col += vec3(d);

  FragColor = vec4(col,1.0);
}
