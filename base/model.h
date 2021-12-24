#pragma once
#include "shader.h"
#include "mesh.h"
#include "object3d.h"
#include "manualtexture.h"
#include "light.h"
#include <vector>
#include <string>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model: public Object3D
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const& path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }
    Model() = default;
    ~Model() = default;

    // draws the model, and thus all its meshes
    virtual void Draw(Shader& shader);

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

class DynamicModel : public Object3D
{
public:
    DynamicModel(string const& fragpath, int amount, float fps);
    void Draw(Shader& shader, float time);
    void setPosition(glm::vec3 ps);
    void setRotation(glm::quat rt);
    void setScale(glm::vec3 sc);

private:
    glm::vec3 globalPosition = { 0.0f, 0.0f, 0.0f };
    glm::quat globalRotation = { 1.0f, 0.0f, 0.0f, 0.0f };
    glm::vec3 globalScale = { 1.0f, 1.0f, 1.0f };
    int amount;
    float dt;
    vector<Model> m;
};


