#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    unsigned int ID; // Programm id

    Shader() { }
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const; 
    void setVec3(const std::string& name, glm::vec3& value) const;
    void setMat4(const std::string& name, glm::mat4& value) const;

    std::string openFile(const char* path);
};