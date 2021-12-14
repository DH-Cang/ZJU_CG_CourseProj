#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

class ManualTexture {
public:
	ManualTexture();

	virtual ~ManualTexture();

	virtual void bind() const = 0;

	virtual void unbind() const = 0;

protected:
	GLuint _handle = {};

	virtual void cleanup();
};

class Texture2D : public ManualTexture {
public:
	Texture2D(const std::string path);

	~Texture2D() = default;

	void bind() const override;

	virtual void unbind() const;

private:
	std::string _path;
};

class TextureCubemap : public ManualTexture {
public:
	TextureCubemap(const std::vector<std::string>& filenames);

	~TextureCubemap() = default;

	void bind() const override;

	void unbind() const override;

private:
	std::vector<std::string> _paths;
};