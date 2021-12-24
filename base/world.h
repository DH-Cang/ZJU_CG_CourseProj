#pragma once
#include "application.h"
#include "model.h"
#include "camera.h"
#include "skybox.h"
#include "light.h"
#include "../basic_models/include/cube.h"
#define BMP_Header_Length 54

class world : public Application {
public:
	world();

	~world() {};

private:

	void handleInput() override;

	void renderFrame() override;

	bool setTarget = false;

	glm::vec3 target = { 0.0f, 0.0f, 0.0f };

	glm::vec3 viewDir = { 0.0f, 0.0f, 0.0f };

    unique_ptr<PerspectiveCamera> camera;

	unique_ptr<SkyBox> skyBox;

	unique_ptr<SunLight> sunLight;

	unique_ptr<Shader> sunShader;
	
    unique_ptr<Model> sun;

	unique_ptr<Model> bunny;

	unique_ptr<DynamicModel> posture;

	unique_ptr<Model> nanosuit;

	unique_ptr<Cube> cube;

    unique_ptr<Shader> defaultShader;

	unique_ptr<Shader> phongShader;

	unique_ptr<Shader> basicShader;

	unique_ptr<Shader> bunnyShader;

};

