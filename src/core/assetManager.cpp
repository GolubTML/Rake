#include <core/assetManager.hpp>
#include <renderer/texture2D.hpp>
#include <renderer/shader.hpp>
#include <renderer/model.hpp>
#include <iostream>
#include <filesystem>
#include <GL/gl.h>

std::unordered_map<std::string, std::unique_ptr<Texture2D>> AssetManager::textures;
std::unordered_map<std::string, std::unique_ptr<Shader>> AssetManager::shaders;
std::unordered_map<std::string, std::unique_ptr<Model>> AssetManager::models;

Texture2D* AssetManager::loadTexture(const std::string& path, const std::string& name, unsigned int filter)
{
    if (textures.find(name) != textures.end())
        return textures[name].get();

    auto tex = std::make_unique<Texture2D>(path, filter);
    Texture2D* ptr = tex.get();

    textures[name] = std::move(tex);
    return ptr;
}

Texture2D& AssetManager::getTexture(const std::string& name)
{
    if (textures.find(name) == textures.end())
        std::cerr << "Cannot find texture: " << name << "\n";

    return *textures[name];
}

void AssetManager::loadAllTextures(const std::string& folder)
{
    try
    {
        for (const auto& entry: std::filesystem::recursive_directory_iterator(folder))
        {
            if (entry.is_regular_file())
            {
                std::string ext = entry.path().extension().string();

                if (ext == ".png")
                {
                    std::string name = entry.path().stem().string();
                    std::string path = entry.path().string();

                    loadTexture(path, name, GL_NEAREST);
                }
            }
        }

        std::cout << "Total textures loaded: " << textures.size() << std::endl;
    }
    catch(const std::filesystem::filesystem_error& e)
    {
        std::cerr << "File system err: " << e.what() << '\n';
    }
}



Shader* AssetManager::loadShader(const std::string& v, const std::string& f, std::string name)
{
    shaders[name] = std::make_unique<Shader>(v.c_str(), f.c_str());
    return shaders[name].get();
}

Shader& AssetManager::getShader(const std::string& name)
{
    return *shaders[name];
}

void AssetManager::loadAllShaders(const std::string& vertexFolder, const std::string& fragmentFolder)
{
    try
    {
        for (const auto& entry: std::filesystem::recursive_directory_iterator(vertexFolder))
        {
            std::string vPath = entry.path().string();

            if (vPath.find("_v.glsl") != std::string::npos)
            {
                std::string baseName = entry.path().stem().string(); 
                baseName = baseName.substr(0, baseName.length() - 2);

                std::string fPath = fragmentFolder + "/" + baseName + "_f.glsl";

                if (std::filesystem::exists(fPath))
                {
                    loadShader(vPath, fPath, baseName);
                    // std::cout << "Load shader: " << baseName << "\n";
                }
                else 
                {
                    std::cerr << "There is no shader file! " << baseName << "\n";
                }
            }
        }

        std::cout << "Total shaders loaded: " << shaders.size() << std::endl;
    }
    catch(const std::filesystem::filesystem_error& e)
    {
        std::cerr << "File system err: " << e.what() << '\n';
    }
}

Model* AssetManager::loadModel(const std::string& path, const std::string& name)
{
    models[name] = std::make_unique<Model>(path.c_str());
    return models[name].get();
}

Model& AssetManager::getModel(const std::string& name)
{
    return *models[name];
}

void AssetManager::loadAllModels(const std::string& folder)
{
    try
    {
        for (const auto& entry: std::filesystem::recursive_directory_iterator(folder))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".obj")
            {
                std::string filePath = entry.path().string();

                std::string modelName = entry.path().stem().string();

                std::cout << "Found model: " << modelName << " at " << filePath << std::endl;

                loadModel(filePath, modelName);
            }
        }

        std::cout << "Total models loaded: " << models.size() << std::endl;
    }
    catch(const std::filesystem::filesystem_error& e)
    {
        std::cerr << "File system err: " << e.what() << '\n';
    }
}


void AssetManager::clear() 
{ 
    textures.clear();
    shaders.clear(); 
    models.clear();
}