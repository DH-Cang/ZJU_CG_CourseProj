#pragma once
#include "../../base/object3d.h"
#include "../../base/material.h"
#include "../../base/shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


class Cylinder : public Object3D {
public:
    Cylinder();
    ~Cylinder();

    void Draw(Shader& shader);

private:
    // notice that the normal data hasn't been set

    struct Material mat;

    GLuint vao[4], vbo[4];
    glm::vec3 up_vertices[363];
    glm::vec3 down_vertices[363];
    glm::vec3 around_vertices[2180];
};
