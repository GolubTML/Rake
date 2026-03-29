#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_impl_glfw.h>
#include <lib/imgui/imgui_impl_opengl3.h>
#include <core/debugWindow.hpp>
#include <core/engine.hpp>
#include <core/player.hpp>
#include <core/particles.hpp>
#include <core/camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

void DebugWindow::init(GLFWwindow* window) { }

void DebugWindow::draw(Engine* engine, float dt) 
{
    ImGui::Begin("Debug mode");

    
    if (ImGui::CollapsingHeader("Performance"))
    {
        float fps = 1.f / dt;
        ImGui::Text("FPS: %.1f", fps);
        ImGui::Text("Enemies count: %d", (int)engine->enemies.size());
    }

    if (ImGui::CollapsingHeader("Player"))
    {
        ImGui::SliderFloat("Speed", &engine->player->speed, 1.f, 20.f);
        ImGui::SliderFloat("HP", &engine->player->health, 0.f, 100.f);
    }

    if (ImGui::CollapsingHeader("Light System"))
    {
        for (int i = 0; i < engine->lights.size(); ++i)
        {
            ImGui::PushID(i);
            std::string label = "Light " + std::to_string(i);
            if (ImGui::TreeNode(label.c_str()))
            {
                ImGui::DragFloat3("Pos", glm::value_ptr(engine->lights[i].position), 0.1f);
                // ImGui::ColorEdit3("Color", glm::value_ptr(engine->lights[i].color));
                ImGui::DragFloat("Linear", &engine->lights[i].linear);
                ImGui::DragFloat("Quadratic", &engine->lights[i].quadratic);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }

    if (ImGui::CollapsingHeader("Enemy"))
    {
        if (ImGui::Button("Spawn Enemy"))
            engine->createEnemy();

        ImGui::SameLine();

        if (ImGui::Button("Clear All"))
            engine->enemies.clear();

        ImGui::Checkbox("Show Enemy Hitboxes", &engine->showHitboxes);
        ImGui::Checkbox("Stop enemy's AI", &engine->stopAI);
    }

    if (ImGui::CollapsingHeader("Particles"))
    {
        if (ImGui::Button("Spawn explosion"))
            engine->particles->createExplosion(engine->player->position + engine->player->camera->front * 2.0f, glm::vec4(0.0f, 1.0f, 0.5f, 1.f), 50);
    }

    ImGui::End();
}