#ifndef SPHERE_H
#define SPHERE_H

#include "../model.h"
#include "../material.h"
#include <math.h>

class Sphere : public Model {
public:
    glm::vec3 pos;
    glm::vec3 size;

    Material material;
    Sphere(Material material, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f))
        : material(material), pos(pos), size(size) {}

    void init() {

        unsigned int rings = 30;
        unsigned int sectors = 30;
        float radius = glm::length(size);
        float const R = 1.0f / (float)(rings - 1);
        float const S = 1.0f / (float)(sectors - 1);
        float noVertices = rings * sectors;
        float pi = 3.14159265358979323846f;
        float* vertices = new float[rings * sectors * 8];
        int count = 0;
        std::vector<unsigned int> indices;
        for (unsigned int r = 0; r < rings; r++) {
            for (unsigned int s = 0; s < sectors; s++) {
                float const y = (float)sin(-pi / 2.0f + pi * r * R);
                float const x = (float)cos(2.0f * pi * s * S) * (float)sin(pi * r * R);
                float const z = (float)sin(2.0f * pi * s * S) * (float)sin(pi * r * R);
                vertices[count++] = radius * x; //x
                vertices[count++] = radius * y;    // y
                vertices[count++] = radius * z;   // z

                vertices[count++] = x;   // nx
                vertices[count++] = y;   // ny
                vertices[count++] = z;   // nz

                vertices[count++] = s * S; //u
                vertices[count++] = r * R; //v
            }
        }
        count = 0;
        for (int i = 0; i < rings- 1; i++)
        {
            for (int j = 0; j < sectors - 1; j++)
            {
                int first = i * sectors + j;
                unsigned short qi[]{ first, first + sectors, first + 1, first + sectors + 1 };

                indices.insert(indices.end(), { qi[0], qi[2], qi[3], qi[0], qi[3], qi[1] });
            }
        }

        mesh = Mesh(Vertex::genList(vertices, 6*noVertices), indices);

    }
         void render(Shader shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        model = glm::scale(model, size);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-30.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setMat4("model", model);

        shader.set3Float("material.ambient", material.ambient);
        shader.set3Float("material.diffuse", material.diffuse);
        shader.set3Float("material.specular", material.specular);
        shader.setFloat("material.shininess", material.shininess);

        Model::render(shader);
    }

};



#endif