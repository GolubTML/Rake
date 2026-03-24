#pragma once
#include <glm/glm.hpp>

class GLFWwindow;

class Camera
{
public:
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    glm::vec2 size = glm::vec2(0.f);

    float fov = 45.f;
    float sensivity = 0.05f;
    float yaw = -90.f;
    float pitch = 0.0f;
    float tilt = 0.f;
    bool firstMouse = false;
    
    Camera() {}
    Camera(glm::vec3 pos, float w, float h, float f, float sens); // f - fov

    glm::mat4 getView() const;
    glm::mat4 getCleanView() const;
    glm::mat4 getProjection() const;

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void setSize(float w, float h);

private:
    float lastX;
    float lastY;
};