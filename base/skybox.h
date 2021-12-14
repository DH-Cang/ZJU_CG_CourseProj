#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "shader.h"
#include "manualtexture.h"

const std::vector<std::string> skyboxTexturePaths = {
"./data/skybox/RIGHT.bmp",
"./data/skybox/LEFT.bmp",
"./data/skybox/UP.bmp",
"./data/skybox/DOWN.bmp",
"./data/skybox/FRONT.bmp",
"./data/skybox/BACK.bmp"
};

class SkyBox {
public:
	SkyBox(const std::vector<std::string>& textureFilenames = skyboxTexturePaths);

	~SkyBox();

	void Draw(const glm::mat4& projection, const glm::mat4& view);

private:
	GLuint _vao = 0;
	GLuint _vbo = 0;

	std::unique_ptr<TextureCubemap> _texture;

	std::unique_ptr<Shader> _shader;



	void cleanup();
};