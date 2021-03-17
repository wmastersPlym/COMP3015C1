#version 460

in vec3 Position;
in vec3 Normal;

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

uniform vec3 cameraPos;

layout (location = 0) out vec4 FragColour;

vec3 blinnPhong(vec3 pos, vec3 n) {

    vec3 Pf = vec3(0.0f);
    for(int i=0; i< 3; i++) {
        vec3 s = normalize(vec3(lights[i].Position) - pos);
        float sDotN = max(dot(s,n), 0.0f);


        // Ambient
        vec3 ambient = lights[i].La * Material.Ka;

        // Diffuse
        vec3 diffuse = Material.Kd * sDotN;

        // Specular
        vec3 spec = vec3(0.0f);

        if(sDotN > 0.0f) {
            vec3 v = normalize(-pos.xyz);
            //r
            vec3 h = normalize (v + s);
            spec = Material.Ks * pow(max(dot(h,n), 0.0f), Material.Shininess);
        }

        Pf += ambient + lights[i].L * (diffuse + spec);
    }

    

    return Pf;
}


void main() {
    
    FragColour = vec4(blinnPhong(Position, normalize(Normal)), 1.0);
}
