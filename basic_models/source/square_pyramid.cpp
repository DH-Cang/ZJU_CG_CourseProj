#include "../include/square_pyramid.h"

Square_pyramid::Square_pyramid() {
    // set up material
    mat.Ka = glm::vec4(0.5f, 0.2f, 0.2f, 1.0f);
    mat.Kd = glm::vec4(0.5f, 0.2f, 0.2f, 1.0f);
    mat.Ks = glm::vec4(0.5f, 0.2f, 0.2f, 1.0f);
    mat.shininess = 5.0f;

    // set up OpenGL src
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

void Square_pyramid::Draw(Shader& shader) {
    glDepthFunc(GL_LESS);
    shader.use();
    shader.setVec4("material.ka", mat.Ka);
    shader.setVec4("material.kd", mat.Kd);
    shader.setVec4("material.ks", mat.Ks);
    shader.setFloat("material.shininess", mat.shininess);
    shader.setMat4("model", getModelMatrix());

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

Square_pyramid::~Square_pyramid() {
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}

