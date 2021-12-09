#pragma once
#include "application.h"
#include "model.h"
#include "camera.h"

class test : public Application {
public:
	test();

	~test() {};

private:

	void handleInput() override;

	void renderFrame() override;

    std::unique_ptr<Shader> test_shader;
    std::unique_ptr<Model> tmp_model;
    std::unique_ptr<Camera> camera;
};

