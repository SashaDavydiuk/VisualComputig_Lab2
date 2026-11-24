#version 330 core

uniform float hasVertCols;

// interpolated varyings
in vec3 vPosition;
in vec3 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

// output to framebuffer
out vec4 fragColor;

void main()
{
    vec4 color = vec4(1.0);

    if (hasVertCols == 1.0)
        color.rgb = vColor;

    fragColor = color;
}
