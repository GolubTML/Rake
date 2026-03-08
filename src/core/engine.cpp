#include <core/engine.hpp>
#include <core/camera.hpp>
#include <core/shader.hpp>
#include <core/shapes.hpp>
#include <core/player.hpp>
#include <core/skybox.hpp>
#include <core/enemy.hpp>
#include <iostream>
#include <vector>
#include <algorithm> 
#include <string>
#include <random>

Engine::Engine(glm::vec3 lightPos) : worldLight(lightPos) { }
Engine::~Engine() { }

void Engine::init()
{
    srand(time(0));

    if (!glfwInit())
    {
        std::cerr << "Failed to init glfw" << "\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "Rake", nullptr, nullptr);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (window == nullptr)
    {
        std::cerr << "Failde to create window!" << "\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, 800, 600);

    player = new Player(glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 2.f, 1.f), 2.5f, width, height);

    std::vector<std::string> skyboxFaces = {
        "textures/skybox/Daylight Box_Right.bmp", "textures/skybox/Daylight Box_Left.bmp",
        "textures/skybox/Daylight Box_Top.bmp"  , "textures/skybox/Daylight Box_Bottom.bmp",
        "textures/skybox/Daylight Box_Front.bmp", "textures/skybox/Daylight Box_Back.bmp"    
    };

    skybox = new Skybox(skyboxFaces);

    line = new Line();
    line->init();

    cross = {
        new Line(), new Line()
    };

    cross[0]->init();
    cross[1]->init();

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


    shaderProg = new Shader("shaders/vertex/vertex.glsl", "shaders/fragments/fragment.glsl");
    rayShader = new Shader("shaders/vertex/ray_v.glsl", "shaders/fragments/ray_f.glsl");

    worldPlane = new Cube(glm::vec3(0.f), glm::vec3(0.8f, 0.6f, 0.3f), glm::vec3(40.f, 1.f, 40.f));

    for (int i = 0; i <= 5; ++i)
    {
        float x = (rand() % 40) - 20;
        float z = (rand() % 40) - 20;
        enemies.push_back(new Enemy(glm::vec3(x, 1.0f, z), 100.f));
    }

    deltaTime = 0.f;
    lastFrame = 0.f;

    glEnable(GL_DEPTH_TEST);
}

void Engine::run()
{
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


        glfwGetFramebufferSize(window, &display_w, &display_h);
        width = static_cast<float>(display_w);
        height = static_cast<float>(display_h);
        glViewport(0, 0, display_w, display_h);
        player->camera->setSize(width, height);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        std::string title = "Rake | HP: " + std::to_string((int)player->health);
        glfwSetWindowTitle(window, title.c_str());

        skybox->draw(player->camera->getView(), player->camera->getProjection());
        
        shaderProg->use();
        
        player->update(window, deltaTime, worldPlane);

        for (auto target: enemies)
        {
            target->update(player, deltaTime);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) 
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

        glm::mat4 view = player->camera->getView();
        glm::mat4 projection = player->camera->getProjection();

        shaderProg->setMat4("view", view);
        shaderProg->setMat4("proj", projection);

        worldPlane->drawWithLight(*shaderProg, false, worldLight);

        for (auto target: enemies)
        {
            target->draw(shaderProg); // мне нужно тут избавиться от второго цикла, но как?
        }

        line->draw(view, projection, *rayShader);

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](Enemy* e)
            {
                if (e->health <= 0)
                {
                    delete e;
                    return true;
                }

                return false;
            }
        ), enemies.end());

        glm::vec3 camPos = player->camera->position;
        glm::vec3 camFront = player->camera->front;
        glm::vec3 camUp = glm::vec3(0, 1, 0); 
        glm::vec3 camRight = glm::normalize(glm::cross(camFront, camUp));
        camUp = glm::normalize(glm::cross(camRight, camFront));

        glm::vec3 center = camPos + camFront * 0.2f;

        float size = 0.002f;

        cross[0]->updatePoints(center - camRight * size, center + camRight * size);
        cross[0]->draw(view, projection, *rayShader);

        cross[1]->updatePoints(center - camUp * size, center + camUp * size);
        cross[1]->draw(view, projection, *rayShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Engine::quit()
{
    delete player;

    for (auto enemy: enemies)
        delete enemy;

    enemies.clear(); // во как надо

    delete shaderProg;
    delete rayShader;
    delete line;

    for (auto lines: cross)
        delete lines;

    cross.clear();

    delete worldPlane;
    delete skybox;
    
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}