#include <core/engine.hpp>
#include <gamecore/camera.hpp>
#include <renderer/shader.hpp>
#include <common/shapes.hpp>
#include <gamecore/player.hpp>
#include <renderer/skybox.hpp>
#include <gamecore/enemy.hpp>
#include <renderer/mesh.hpp>
#include <renderer/model.hpp>
#include <renderer/particles.hpp>
#include <renderer/fontRenderer.hpp>
#include <renderer/spriteRenderer.hpp>
#include <renderer/texture2D.hpp>
#include <core/assetManager.hpp>
#include <gamecore/projectile.hpp>
#include <gamecore/entity.hpp>
#include <gamecore/entityManager.hpp>
#include <gameplay/enemy/eye.hpp>
#include <lib/stb_image.h>
#include <iostream>
#include <vector>
#include <algorithm> 
#include <string>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_impl_glfw.h>
#include <lib/imgui/imgui_impl_opengl3.h>
#include <core/debugWindow.hpp>


Engine::Engine() { }
Engine::~Engine() { }

void Engine::init()
{
    srand(time(0));

    Projectile::onSpawn = [this](std::unique_ptr<Projectile> newProj)
    {
        this->activeProjectiles.push_back(std::move(newProj));
    };

    if (!glfwInit())
    {
        std::cerr << "Failed to init glfw" << "\n";
        return;
    }

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "Rake", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "Failde to create window!" << "\n";
        glfwTerminate();
        return;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, 800, 600);

    fontRenderer = std::make_unique<FontRenderer>(width, height);
    fontRenderer->load("assets/fonts/arialmt.ttf", 48);

    AssetManager::loadAllTextures("assets/textures/UI");
    AssetManager::loadAllShaders("shaders/vertex", "shaders/fragments");
    AssetManager::loadAllModels("assets/models");

    player = std::make_unique<Player>(glm::vec3(0.f, 30.f, 0.f), glm::vec3(1.f, 2.f, 1.f), 4.25f, width, height);

    std::vector<std::string> skyboxFaces = {
        "assets/textures/skybox/right.png", "assets/textures/skybox/left.png",
        "assets/textures/skybox/top.png"  , "assets/textures/skybox/bottom.png",
        "assets/textures/skybox/front.png", "assets/textures/skybox/back.png"    
    };

    skybox = std::make_unique<Skybox>(skyboxFaces);

    line = std::make_unique<Line>();
    line->init();

    glfwSetWindowUserPointer(window, player->camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
    {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        {
            Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            if (cam)
                cam->mouse_callback(window, xpos, ypos);
        }
    });


    spriteRenderer = std::make_unique<SpriteRenderer>(&AssetManager::getShader("sprite"));

    level.reserve(9);

    level.push_back(std::make_unique<Cube>(glm::vec3(0.f), glm::vec3(0.8f, 0.6f, 0.3f), glm::vec3(40.f, 1.f, 40.f)));
    level.push_back(std::make_unique<Cube>(glm::vec3(20.f, 5.f, 20.f), glm::vec3(0.3f, 0.5f, 0.2f), glm::vec3(1.f, 10.f, 1.f)));
    level.push_back(std::make_unique<Cube>(glm::vec3(-20.f, 5.f, 20.f), glm::vec3(0.3f, 0.5f, 0.2f), glm::vec3(1.f, 10.f, 1.f)));
    level.push_back(std::make_unique<Cube>(glm::vec3(20.f, 5.f, -20.f), glm::vec3(0.3f, 0.5f, 0.2f), glm::vec3(1.f, 10.f, 1.f)));
    level.push_back(std::make_unique<Cube>(glm::vec3(-20.f, 5.f, -20.f), glm::vec3(0.3f, 0.5f, 0.2f), glm::vec3(1.f, 10.f, 1.f)));
    level.push_back(std::make_unique<Cube>(glm::vec3(0.f, 10.f, -5.f), glm::vec3(0.8f, 0.6f, 0.3f), glm::vec3(40.f, 1.f, 30.f)));
    level.push_back(std::make_unique<Cube>(glm::vec3(13.f, 3.f, 13.f), glm::vec3(0.8f, 0.6f, 0.3f), glm::vec3(5.f, 5.f, 5.f)));
    level.push_back(std::make_unique<Cube>(glm::vec3(18.f, 2.f, 18.f), glm::vec3(0.8f, 0.6f, 0.3f), glm::vec3(5.f, 3.f, 5.f)));
    level.push_back(std::make_unique<Cube>(glm::vec3(8.f, 3.f, 13.f), glm::vec3(0.8f, 0.6f, 0.3f), glm::vec3(5.f, 8.f, 5.f)));

    for (int i = 0; i <= 5; ++i)
    {
        float x = (rand() % 40) - 20;
        float z = (rand() % 40) - 20;
        glm::vec3 pos = glm::vec3(x, 0, z);
        enemies.push_back(EntityManager::spawnEnemy(EnemyType::EYE, glm::vec3(x, 0, z)));
    }

    // lights.push_back({ glm::vec3(18.f, 5.f, 18.f), 1.f, 0.022f, 0.0019f });
    // lights.push_back({ glm::vec3(-18.f, 5.f, 18.f), 1.f, 0.022f, 0.0019f });
    // lights.push_back({ glm::vec3(18.f, 5.f, -18.f), 1.f, 0.022f, 0.0019f });
    // lights.push_back({ glm::vec3(-18.f, 5.f, -18.f), 1.f, 0.022f, 0.0019f });
    lights.push_back(player->lampa);

    stbi_set_flip_vertically_on_load(true);

    float iconSizeW = 30.0f;
    float iconSizeH = 56.0f;
    float padding = 20.0f;

    glm::vec2 dashPos = glm::vec2(width - padding - iconSizeW, padding);  
    glm::vec2 crossChairPos = glm::vec2(width / 2.0f - 16.0f, height / 2.0f - 16.0f);

    uiElements.insert({ "cross_chair", UIElement("cross_chair", "CrossChair", crossChairPos, glm::vec2(16.f)) });
    uiElements.insert({ "hp_bar_bg", UIElement("hp_bar_bg", "hpbar_bg", glm::vec2(20.f), glm::vec2(200.f, 30.f) * 2.f) });
    uiElements.insert({ "hp_bar_front", UIElement("hp_bar_front", "hpbar_front", glm::vec2(20.f), glm::vec2(200.f, 30.f) * 2.f, glm::vec3(1.f), 0.f, 0.f, false) });
    uiElements.insert({ "dash_icon_back", UIElement("dash_icon_back", "dash_icon", dashPos, glm::vec2(iconSizeW, iconSizeH), glm::vec3(0.f), 0.f, 1.f, false) });
    uiElements.insert({ "dash_icon_front", UIElement("dash_icon_front", "dash_icon", dashPos, glm::vec2(iconSizeW, iconSizeH), glm::vec3(1.f), 0.f, 0.f, true) });

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark(); 
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplOpenGL3_Init("#version 130");

    particles = std::make_unique<ParticleGenerator>();

    particles->init(0.05f);

    deltaTime = 0.f;
    lastFrame = 0.f;

    debugWindow = std::make_unique<DebugWindow>();

    glEnable(GL_DEPTH_TEST);
}

void Engine::run()
{
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        input();
        update();
        render();
    }
}

void Engine::quit()
{
    level.clear();
    enemies.clear(); 
    activeProjectiles.clear();

    uiElements.clear();
    AssetManager::clear();
    
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::createEnemy()
{
    enemies.push_back(EntityManager::spawnEnemy(EnemyType::EYE, player.get()->position));
}

void Engine::input()
{
    static bool tabPressed = false;

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabPressed)
    {
        int currentMode = glfwGetInputMode(window, GLFW_CURSOR);
        if (currentMode == GLFW_CURSOR_DISABLED)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            player->camera->firstMouse = true; 
        }
        tabPressed = true; 
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    {
        tabPressed = false;
    }
        
    glfwGetFramebufferSize(window, &display_w, &display_h);
    width = static_cast<float>(display_w);
    height = static_cast<float>(display_h);
    glViewport(0, 0, display_w, display_h);
    player->camera->setSize(width, height);

    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) 
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    }
}

void Engine::update()
{
    ImGuiIO& io = ImGui::GetIO();
    UpdateContext context = { window, deltaTime, level, *player, enemies };

    player->update(context);

    for (auto& target: enemies)
    {
        if (!stopEnemyAI)
            target->update(context);
    }

    particles->update(deltaTime);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) 
    {
        if (!io.WantCaptureMouse)
        {
            if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) 
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                player->camera->firstMouse = true;
            }

            if (player->canShoot())
            {
                player->shoot(enemies, *line);
                player->resetShootTimer();
            }
        }
    }


    for (auto& proj : activeProjectiles)
    {
        if (!stopProjAI)
            proj->update(context);
        
        Projectile* p = dynamic_cast<Projectile*>(proj.get());

        for (auto& enemy: enemies)
        {
            Enemy* e = dynamic_cast<Enemy*>(enemy.get());
            if (proj->checkCollision(*enemy))
            {
                p->onHit(*e);
                break;
            }
        }
    }


    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const auto& p) 
        {
            return p->isDead;
        }
    ), enemies.end());

    activeProjectiles.erase(std::remove_if(activeProjectiles.begin(), activeProjectiles.end(),
        [](const auto& p) 
        {
            return p->isDead;
        }
    ), activeProjectiles.end());
}

void Engine::render()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    debugWindow->draw(this, deltaTime);

    Shader& skyShader = AssetManager::getShader("skybox");

    skyShader.use();
    skyShader.setFloat("time", (float)glfwGetTime());

    skybox->draw(player->camera->getView(), player->camera->getProjection());

    glm::mat4 view = player->camera->getView();
    glm::mat4 UIView = player->camera->getCleanView();
    glm::mat4 projection = player->camera->getProjection();

    std::vector<Entity*> entities;

    for (auto& target: enemies)
        entities.push_back(target.get());

    for (auto& proj: activeProjectiles)
        entities.push_back(proj.get());

    if (!lights.empty())
    {
        lights.back().position = player->lampa.position;
    }

    Shader& baseShader = AssetManager::getShader("base");

    baseShader.use();
    baseShader.setMat4("view", view);
    baseShader.setMat4("proj", projection);
    baseShader.setVec3("viewPos", player->camera->position);

    baseShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    baseShader.setVec3("dirLight.ambient",   glm::vec3(0.02f));
    baseShader.setVec3("dirLight.diffuse",   glm::vec3(0.15f));
    baseShader.setVec3("dirLight.specular",  glm::vec3(0.f));

    baseShader.setInt("activeLights", (int)lights.size());

    for (int i = 0; i < lights.size(); ++i)
    {
        std::string prefix = "lights[" + std::to_string(i) + "].";
        baseShader.setVec3(prefix + "position", lights[i].position);

        baseShader.setFloat(prefix + "constant",  1.f);
        baseShader.setFloat(prefix + "linear",  lights[i].linear);
        baseShader.setFloat(prefix + "quadratic", lights[i].quadratic);

        baseShader.setVec3(prefix + "ambient",   glm::vec3(0.05f));
        baseShader.setVec3(prefix + "diffuse",   glm::vec3(0.8f)); 
        baseShader.setVec3(prefix + "specular",  glm::vec3(0.f));
    }

    for (auto& cube: level)
    {
        cube->drawWithLight(baseShader, false);
    }

    if (showHitboxes)
    {
        for (auto& e : entities)
            e->drawHitbox(baseShader);
    } 

    Shader& meshShader = AssetManager::getShader("mesh");

    meshShader.use();
    meshShader.setMat4("view", view);
    meshShader.setMat4("proj", projection);
    meshShader.setBool("useTexture", true);
    meshShader.setVec3("viewPos", player->camera->position);

    meshShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    meshShader.setVec3("dirLight.ambient",   glm::vec3(0.02f));
    meshShader.setVec3("dirLight.diffuse",   glm::vec3(0.15f));
    meshShader.setVec3("dirLight.specular",  glm::vec3(0.f));

    meshShader.setInt("activeLights", (int)lights.size());

    for (int i = 0; i < lights.size(); ++i)
    {
        std::string prefix = "lights[" + std::to_string(i) + "].";
        meshShader.setVec3(prefix + "position",   lights[i].position);
        meshShader.setFloat(prefix + "constant",  1.f);
        meshShader.setFloat(prefix + "linear",    lights[i].linear);
        meshShader.setFloat(prefix + "quadratic", lights[i].quadratic);
        meshShader.setVec3(prefix + "ambient",    glm::vec3(0.05f));
        meshShader.setVec3(prefix + "diffuse",    glm::vec3(0.8f));
    }

    for (auto& e : entities)
    {
        e->draw(meshShader);
        // почему то, отрисовка проджектайлов работает плохо
        // если в дебаг меню включить show hitboxes проджектайлы, как и их хитбоксы, не будут прорисовываться
        // if (showHitboxes) e->drawHitbox(baseShader); 
    }

    meshShader.setMat4("view", UIView);

    player->drawWeapon(&meshShader, &AssetManager::getModel("agony"));

    line->draw(view, projection, AssetManager::getShader("ray"));

    Shader& particleShader = AssetManager::getShader("particle");

    particleShader.use();
    particleShader.setMat4("view", view);
    particleShader.setMat4("proj", projection);

    particles->draw(&particleShader, player->camera);

    drawUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Engine::drawUI()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    player->visualHealth = glm::mix(player->visualHealth, player->health, deltaTime * 5.0f);

    float hpPercent = player->visualHealth / 100.0f;
    if (hpPercent < 0) hpPercent = 0;

    float dashPercent = 1.0f - (player->dashCooldownTimer / player->dashCooldown);
    if (dashPercent > 1.f) dashPercent = 1.f;
    if (dashPercent < 0.f) dashPercent = 0.0f;

    glm::mat4 orthoProjection = player->camera->getOrthoProj();

    Shader& fontShader = AssetManager::getShader("font");

    Shader& spriteShader = AssetManager::getShader("sprite");

    spriteShader.use();
    spriteShader.setMat4("proj", orthoProjection);

    float iconSizeW = 30.0f;
    float iconSizeH = 56.0f;
    float padding = 20.0f;

    glm::vec2 dashPos = glm::vec2(width - padding - iconSizeW, padding);  
    glm::vec2 crossChairPos = glm::vec2(width / 2.0f - 16.0f, height / 2.0f - 16.0f);

    uiElements.at("hp_bar_front").fillAmount = hpPercent;
    uiElements.at("dash_icon_front").fillAmount = dashPercent;

    uiElements.at("dash_icon_back").pos = dashPos;
    uiElements.at("dash_icon_front").pos = dashPos;
    uiElements.at("cross_chair").pos = crossChairPos;


    for (auto& [name, element]: uiElements)
    {
        spriteRenderer->drawSprite(
            AssetManager::getTexture(element.textureName),
            element.pos,
            element.size,
            element.rotation,
            element.color,
            element.fillAmount,
            element.verticalFill
        );
    }

    fontShader.use();
    fontShader.setMat4("proj", orthoProjection);

    // fontRenderer->renderText(fontShader, "HP: " + std::to_string((int)player->health), 20.0f, 20.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}