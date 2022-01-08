#version 330 core
uniform sampler2D tex;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
  vec4 texColor = texture(tex, texCoord);
  if(texColor.a < 0.0001)
    discard;
  FragColor = texColor;
}
