#pragma once

#include <unordered_map>
#include <string>
#include <memory>

class Texture2D;
class Shader;
class Model;

class AssetManager
{
public:
    static Texture2D* loadTexture(const std::string& path, const std::string& name, unsigned int filter);
    static Texture2D& getTexture(const std::string& name);
    static void loadAllTextures(const std::string& folder);
    
    static Shader* loadShader(const std::string& v, const std::string& f, std::string name);
    static Shader& getShader(const std::string& name);
    static void loadAllShaders(const std::string& vertexFolder, const std::string& fragmentFolder);

    static Model* loadModel(const std::string& path, const std::string& name);
    static Model& getModel(const std::string& name);
    static void loadAllModels(const std::string& folder);
    
    static void clear();

private:
    static std::unordered_map<std::string, std::unique_ptr<Texture2D>> textures;
    static std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
    static std::unordered_map<std::string, std::unique_ptr<Model>> models;
};