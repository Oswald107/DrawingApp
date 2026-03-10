#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec3 Col;

uniform sampler2D screenTexture;

void main()
{ 
    vec4 color1 = vec4(Col, 1.0);
    vec4 color = texture(screenTexture, TexCoords);

    

    FragColor = color;
}