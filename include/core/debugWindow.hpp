#pragma once

class GLFWwindow;
class Engine;

class DebugWindow
{
public:
    void init(GLFWwindow* window);
    void draw(Engine* engine, float dt);
};