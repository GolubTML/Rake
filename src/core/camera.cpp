#include <core/camera.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, float w, float h, float f, float sens) : position(pos), size(w, h), fov(f), sensivity(sens)
{
    lastX = size.x / 2;
    lastY = size.y / 2;
}

glm::mat4 Camera::getView() const
{
    glm::mat4 view = glm::lookAt(position, position + front, up);
    view = glm::rotate(view, glm::radians(tilt), front);

    return view;
}

glm::mat4 Camera::getCleanView() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjection() const
{
    return glm::perspective(glm::radians(fov), size.x / size.y, 0.1f, 100.f);
}

glm::mat4 Camera::getOrthoProj() const
{
    return glm::ortho(0.f, size.x, 0.f, size.y);
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= 0.05f;
    yoffset *= 0.05f;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.f)
        pitch = 89.f;
    if (pitch < -89.f)
        pitch = -89.f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

void Camera::setSize(float w, float h)
{
    size = glm::vec2(w, h);
}