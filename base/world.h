#pragma once
#include "application.h"
#include "model.h"
#include "camera.h"
#include "skybox.h"
#include "light.h"
#include "../basic_models/include/cube.h"
#include "../basic_models/include/square_pyramid.h"
#include "../basic_models/include/prism.h"
#include "../basic_models/include/sphere.h"
#include "../basic_models/include/cylinder.h" 
#include "../basic_models/include/cone.h" 

#define BMP_Header_Length 54


class world : public Application {
public:
	world();

	~world() {};

private:

	void handleInput() override;

	void renderFrame() override;

	void CameraCollisionCheck(glm::vec3 &camera_pos, glm::vec3 move);

	bool setTarget = false;
	int state = 1; //游戏状态：0状态玩家不可进行上下移动，1状态玩家方可进行上下移动，按C切换
	bool init_collision_box = false;

	glm::vec3 target = { 0.0f, 0.0f, 0.0f };

	glm::vec3 viewDir = { 0.0f, 0.0f, 0.0f };

	std::vector<AABB> colli_box;

    unique_ptr<PerspectiveCamera> camera;

	unique_ptr<SkyBox> skyBox;

	unique_ptr<SunLight> sunLight;

	unique_ptr<Shader> sunShader;
	
    unique_ptr<Model> sun;

	unique_ptr<Model> bunny;

	unique_ptr<DynamicModel> posture;

	unique_ptr<Model> nanosuit;

	unique_ptr<Cube> cube;

	unique_ptr<Sphere> sphere;

	unique_ptr<Cylinder> cylinder;

	unique_ptr<Cone> cone;

	unique_ptr<Square_pyramid> square_pyramid;

	unique_ptr<Prism> prism;

    unique_ptr<Shader> defaultShader;

	unique_ptr<Shader> phongShader;

	unique_ptr<Shader> basicShader;

	unique_ptr<Shader> bunnyShader;

};

