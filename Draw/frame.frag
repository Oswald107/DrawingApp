#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec3 Col;

uniform sampler2D screenTexture;

void main()
{ 
    FragColor = vec4(Col, 1.0);
    FragColor = texture(screenTexture, TexCoords);
}