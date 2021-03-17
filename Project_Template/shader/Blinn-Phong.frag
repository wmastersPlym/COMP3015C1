#version 460

in vec3 LightIntensity;
layout (location = 0) out vec4 FragColour;

void main() {
    FragColour = vec4(LightIntensity, 1.0);
}
