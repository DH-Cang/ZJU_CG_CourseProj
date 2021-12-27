#include "../include/cylinder.h"

Cylinder::Cylinder() {
    // set up material
    mat.Ka = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    mat.Kd = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    mat.Ks = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mat.shininess = 10.0f;

    down_vertices[0] = glm::vec3(0.0f, -1.0f, 0.0f);
    up_vertices[0] = glm::vec3(0.0f, 1.0f, 0.0f);

    float pi = 3.1415;
    int num = 362;
    for (int i = 1;i <= num;i++) {
        float angle = i * 2 * pi / 360;
        down_vertices[i] = glm::vec3(cos(angle), -1.0f, sin(angle));
        up_vertices[i] = glm::vec3(cos(angle), 1.0f, sin(angle));
    }

    around_vertices[0] = down_vertices[1];
    int n = 726;
    for (int i = 1;i <= n;i+=2) {
        around_vertices[i] = down_vertices[(i + 1) / 2];
        around_vertices[i + 1] = up_vertices[(i + 1) / 2];
    }







    // set up OpenGL src
    /*
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(up_vertices), &up_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    */

    // first
    glGenVertexArrays(1, vao + 0);
    glGenBuffers(1, vbo + 0);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(up_vertices), &up_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // second
    glGenVertexArrays(1, vao + 1);
    glGenBuffers(1, vbo + 1);

    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(down_vertices), &down_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // third
    glGenVertexArrays(1, vao + 2);
    glGenBuffers(1, vbo + 2);

    glBindVertexArray(vao[2]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(around_vertices), &around_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ...

}

void Cylinder::Draw(Shader& shader) {
    glDepthFunc(GL_LESS);
    shader.use();
    shader.setVec4("material.ka", mat.Ka);
    shader.setVec4("material.kd", mat.Kd);
    shader.setVec4("material.ks", mat.Ks);
    shader.setFloat("material.shininess", mat.shininess);
    shader.setMat4("model", getModelMatrix());

    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 362);
    glBindVertexArray(0);

    glBindVertexArray(vao[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 362);
    glBindVertexArray(0);

    glBindVertexArray(vao[2]);
    glDrawArrays(GL_TRIANGLE_STRIP, 1, 726);  // to be update
    glBindVertexArray(0);
}

Cylinder::~Cylinder() {
    // to be deconstruct
    if (vbo[0] != 0) {
        glDeleteBuffers(1, &vbo[0]);
        vbo[0] = 0;
    }

    if (vao[0] != 0) {
        glDeleteVertexArrays(1, &vao[0]);
        vao[0] = 0;
    }

    if (vbo[1] != 0) {
        glDeleteBuffers(1, &vbo[1]);
        vbo[1] = 0;
    }

    if (vao[1] != 0) {
        glDeleteVertexArrays(1, &vao[1]);
        vao[1] = 0;
    }

    if (vbo[1] != 0) {
        glDeleteBuffers(1, &vbo[1]);
        vbo[1] = 0;
    }

    if (vao[1] != 0) {
        glDeleteVertexArrays(1, &vao[1]);
        vao[1] = 0;
    }
}

