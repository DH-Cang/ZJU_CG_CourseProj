#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Material {
    //材质颜色光照
    glm::vec4 Ka;
    //漫反射
    glm::vec4 Kd;
    //镜反射
    glm::vec4 Ks;
    float shininess;
};
