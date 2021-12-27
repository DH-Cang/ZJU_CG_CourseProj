#pragma once
#include "../../base/object3d.h"
#include "../../base/material.h"
#include "../../base/shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


class Cone : public Object3D {
public:
    Cone();
    ~Cone();

    void Draw(Shader& shader);

private:
    // notice that the normal data hasn't been set

    struct Material mat;

    GLuint vao, vbo;
    glm::vec3 vertices[363];
    
};
