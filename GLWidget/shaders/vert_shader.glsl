#version 330 core

// vertex attributes
in vec3 position;
in vec3 color;
in vec3 normal;
in vec2 texcoord;

// varyings to be interpolated
out vec3 vPosition;
out vec3 vColor;
out vec3 vNormal;
out vec2 vTexCoord;

void main()
{
    vColor = color;
    
    gl_Position = vec4(position, 1.0);
}
