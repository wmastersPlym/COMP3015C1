#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <sstream>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"


#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;


SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 30, 30) {}

void SceneBasic_Uniform::initScene()
{


    compile();
    glEnable(GL_DEPTH_TEST);


    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));

    view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    float x, z;

    for (int i = 0; i < 3; i++) {
        std::stringstream name;
        name << "Lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
    }
    

    


    
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/Blinn-Phong.vert");
		prog.compileShader("shader/Blinn-Phong.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    //angle += 0.01f;
	//update your angle here
    //if (angle > 360.0f) {
    //    angle -= 360.0f;
    //}
    
}

void SceneBasic_Uniform::render()
{
    model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.0f, 0.0f));

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    setMatrices();

    torus.render();
    
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0,0,w,h);

    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);


}

void SceneBasic_Uniform::setMatrices() {
    mat4 mv = model * view;

    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * view * model);
}