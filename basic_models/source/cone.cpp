#include "../include/cone.h"
#include <math.h>

Cone::Cone() {
    // set up material
    mat.Ka = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    mat.Kd = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    mat.Ks = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mat.shininess = 10.0f;

    // set up OpenGL src
    vertices[0] = glm::vec3(0.0f, 1.0f, 0.0f);
    float pi = 3.1415;
    int num = 362;
    for (int i = 1;i <= num;i++) {
        float angle = i * 2 * pi / 360;
        vertices[i] = glm::vec3(cos(angle), -1.0f, sin(angle));
    }



    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Cone::Draw(Shader& shader) {
    glDepthFunc(GL_LESS);
    shader.use();
    shader.setVec4("material.ka", mat.Ka);
    shader.setVec4("material.kd", mat.Kd);
    shader.setVec4("material.ks", mat.Ks);
    shader.setFloat("material.shininess", mat.shininess);
    shader.setMat4("model", getModelMatrix());

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 362);
    glBindVertexArray(0);
}

Cone::~Cone() {
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}

