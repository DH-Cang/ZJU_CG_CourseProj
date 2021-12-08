#include <iostream>
#include "shader.h"


/*
 * @brief constructor, take string as shader code to create opengl shader
 */
Shader::Shader(const char* vsCode, const char* fsCode) {
    createShaderProgram(vsCode, fsCode);
}

/*
 * @brief constructor, read shader code from file to create opengl shader
 */
Shader::Shader(const std::string& vsFilepath, const std::string& fsFilepath) {
    std::string vsCode = readFile(vsFilepath);
    std::string fsCode = readFile(fsFilepath);

    createShaderProgram(vsCode, fsCode);
}

/*
 * @brief constructor, read shader code from file to create opengl shader
 */
Shader::Shader(Shader&& shader) noexcept {
    _id = shader._id;
    shader._id = 0;
}

/*
 * @brief destructor
 */
Shader::~Shader() {
    if (_id > 0) {
        glDeleteProgram(_id);
    }
}

/*
 * @brief use current shader for object rendering
 */
void Shader::use() {
    glUseProgram(_id);
}

/*
 * @brief set bool uniform variable to shader
 * @param name name of the variable
 * @param value bool value to be pass to shader
 */
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), static_cast<int>(value));
}

/*
 * @brief set int uniform variable to shader
 * @param name name of the variable
 * @param value int value to be pass to shader
 */
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

/*
 * @brief set float uniform variable to shader
 * @param name name of the variable
 * @param value float value to be pass to shader
 */
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

/*
 * @brief set vec2 uniform variable to shader
 * @param name name of the variable
 * @param v2 vec2 to be pass to shader
 */
void Shader::setVec2(const std::string& name, const glm::vec2& v2) const {
    glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &v2[0]);
}


/*
 * @brief set vec3 uniform variable to shader
 * @param name name of the variable
 * @param v3 vec3 to be pass to shader
 */
void Shader::setVec3(const std::string& name, const glm::vec3& v3) const {
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &v3[0]);
}

/*
 * @brief set vec4 uniform variable to shader
 * @param name name of the variable
 * @param v4 vec4 to be pass to shader
 */
void Shader::setVec4(const std::string& name, const glm::vec4& v4) const {
    glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &v4[0]);
}

/*
 * @brief set mat3 uniform variable to shader
 * @param name name of the variable
 * @param value mat3 value to be pass to shader
 */
void Shader::setMat3(const std::string& name, const glm::mat3& mat3) const {
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &mat3[0][0]);
}

/*
 * @brief set mat4 uniform variable to shader
 * @param name name of the variable
 * @param value mat4 value to be pass to shader
 */
void Shader::setMat4(const std::string& name, const glm::mat4& mat4) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat4[0][0]);
}

/*
 * @brief read shader code from file
 * @param filepath path to the file
 * @return shader code in string
 */
std::string Shader::readFile(const std::string& filePath) {
    std::ifstream is;
    is.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        is.open(filePath);
        std::stringstream ss;
        ss << is.rdbuf();

        return ss.str();
    }
    catch (std::ifstream::failure& e) {
        throw std::runtime_error(std::string("read ") + filePath + "error: " + e.what());
    }
}

/*
 * @brief create a vertex / fragment shader
 * @param code shader code of the shader
 * @param shaderType type of the shader
 * @return the shader handle
 */
GLuint Shader::createShader(const std::string& code, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        throw std::runtime_error("create shader failure");
    }

    const char* codeBuf = code.c_str();
    glShaderSource(shader, 1, &codeBuf, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char buffer[1024];
        glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
        std::cerr << code << std::endl;
        throw std::runtime_error("compile error: \n" + std::string(buffer));
    }

    return shader;
}

/*
 * @brief create a shader program
 * @param vsCode vertex shader code
 * @param fsCode fragment shader code
 */
void Shader::createShaderProgram(const std::string& vsCode, const std::string& fsCode) {
    GLuint vs = 0, fs = 0;
    try {
        vs = createShader(vsCode, GL_VERTEX_SHADER);
        if (!vs) {
            throw std::runtime_error("create vertex shader failure");
        }

        fs = createShader(fsCode, GL_FRAGMENT_SHADER);
        if (!fs) {
            throw std::runtime_error("create fragment shader failure");
        }

        _id = glCreateProgram();
        if (_id == 0) {
            throw std::runtime_error("create shader program failure");
        }

        glAttachShader(_id, vs);
        glAttachShader(_id, fs);

        glLinkProgram(_id);

        GLint success;
        glGetProgramiv(_id, GL_LINK_STATUS, &success);
        if (!success) {
            char buffer[1024];
            glGetProgramInfoLog(_id, sizeof(buffer), NULL, buffer);
            throw std::runtime_error("link program error: " + std::string(buffer));
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    } catch (const std::exception& e) {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        if (_id) glDeleteProgram(_id);
        throw e;
    }
}