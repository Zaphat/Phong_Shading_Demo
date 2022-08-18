#ifndef PYRAMID_H
#define PYRAMID_H

#include "../model.h"
#include "../material.h"
class Pyramid : public Model {
public:
    glm::vec3 pos;
    glm::vec3 size;

    Material material;
    Pyramid(Material material, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f))
        : material(material), pos(pos), size(size) {}

    void init() {
        int noVertices = 18;

        float vertices[] = {
            // position                 normal              texcoord
            +0.5, -0.5, -0.5,   0.0,  0.0, -1.0,            0.0,0.0,
            -0.5, +0.5, -0.5,   0.0,  0.0, -1.0,            0.0,1.0,
            +0.5, +0.5, -0.5,   0.0,  0.0, -1.0,            1.0,0.0,
        
            +0.5, -0.5, -0.5,    0.0,  0.0, -1.0,           0.0,1.0,
            -0.5, -0.5, -0.5,    0.0,  0.0, -1.0,           1.0,1.0,
            -0.5, +0.5, -0.5,    0.0,  0.0, -1.0,           1.0,0.0,
    
            +0.5, -0.5, -0.5,     1.0,  0.0,  0.5,          0.0,0.0,
            +0.5, +0.5, -0.5,     1.0,  0.0,  0.5,          1.0,1.0,
            0.0,  0.0, +0.5,      1.0,  0.0,  0.5,         0.0,0.0,
    
            +0.5, +0.5, -0.5,       0.0,  1.0,  0.5,        1.0,0.0,
            -0.5, +0.5, -0.5,       0.0,  1.0,  0.5,         0.0,0.0,
            0.0,  0.0, +0.5,        0.0,  1.0,  0.5,         0.0,1.0,
            
            -0.5, +0.5, -0.5,       -1.0,  0.0,  0.5,       1.0,1.0,
            -0.5, -0.5, -0.5,       -1.0,  0.0,  0.5,       1.0,0.0,
            0.0,  0.0, +0.5,        -1.0,  0.0,  0.5,       0.0,0.0,
            
            -0.5, -0.5, -0.5,        0.0, -1.0,  0.5,       0.0,0.0,
            +0.5, -0.5, -0.5,        0.0, -1.0,  0.5,       1.0,0.0,
             0.0,  0.0, +0.5,        0.0, -1.0,  0.5,        0.0,1.0
        };

        std::vector<unsigned int> indices(noVertices);
        for (unsigned int i = 0; i < noVertices; i++) {
            indices[i] = noVertices-i-1;
        };

        mesh = Mesh(Vertex::genList(vertices, noVertices), indices);
    }

     
    void render(Shader shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,pos);
        model = glm::scale(model, size);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setMat4("model", model);

        shader.set3Float("material.ambient", material.ambient);
        shader.set3Float("material.diffuse", material.diffuse);
        shader.set3Float("material.specular", material.specular);
        shader.setFloat("material.shininess", material.shininess);

        Model::render(shader);
    }
};



#endif