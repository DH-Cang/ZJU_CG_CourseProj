#pragma once
#include "application.h"
#include "model.h"
#include "camera.h"
#include "skybox.h"
#include "light.h"
#include "../basic_models/include/cube.h"
#include "../basic_models/include/square_pyramid.h"

class world : public Application {
public:
	world();

	~world() {};

private:

	void handleInput() override;

	void renderFrame() override;

    unique_ptr<PerspectiveCamera> camera;

	unique_ptr<SkyBox> skyBox;

	unique_ptr<SunLight> sunLight;

	unique_ptr<Shader> sunShader;
	
    unique_ptr<Model> sun;

	unique_ptr<Model> bunny;

	unique_ptr<Model> nanosuit;

	unique_ptr<Cube> cube;

	unique_ptr<Square_pyramid> square_pyramid;

    unique_ptr<Shader> defaultShader;

	unique_ptr<Shader> phongShader;

	unique_ptr<Shader> basicShader;



};

