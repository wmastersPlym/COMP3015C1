#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform struct LightInfo {
    vec4 Position;  // Light position in eye coords.
    vec3 La;        // Ambient light intensity
    vec3 L;         // Diffuse and specular light intensity
} lights[3];

uniform struct MaterialInfo {
    vec3 Ka;            // Ambient reflectivity
    vec3 Kd;            // Diffuse reflectivity
    vec3 Ks;            // Specular reflectivity
    float Shininess;    // Specular shininess factor
} Material;


uniform vec4 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;


uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform vec3 cameraPos;

vec3 blinnPhong(int light, vec3 pos, vec3 n) {

    vec4 p = ModelViewMatrix * vec4(VertexPosition, 1.0f);
    vec3 s = normalize(vec3(lights[light].Position - p));
    float sDotN = max(dot(s,n), 0.0f);


    // Ambient
    vec3 ambient = lights[light].La * Material.Ka;

    // Diffuse
    vec3 diffuse = Material.Kd *sDotN;

    // Specular
    vec3 spec = vec3(0.0f);

    if(sDotN > 0.0f) {
        vec3 v = normalize(-pos.xyz);
        vec3 r = reflect(-s,n);
        spec = Material.Ks * pow(max(dot(r,v), 0.0f), Material.Shininess);
    }

    return ambient + lights[light].L * (diffuse + spec);
}


void main()
{
    vec3 n = normalize(NormalMatrix * VertexNormal);
    vec4 p = ModelViewMatrix * vec4(VertexPosition, 1.0f);


    LightIntensity = vec3(0.0f);
    for(int i=0; i<3; i++) {
        LightIntensity += blinnPhong(i, cameraPos, n);
    }


    gl_Position = vec4(VertexPosition,1.0) * MVP;

}
