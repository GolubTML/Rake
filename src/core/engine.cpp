#include <core/engine.hpp>
#include <core/camera.hpp>
#include <core/shader.hpp>
#include <core/shapes.hpp>
#include <core/player.hpp>
#include <core/skybox.hpp>
#include <iostream>
#include <vector>
#include <string>

Engine::Engine(glm::vec3 lightPos) : worldLight(lightPos) { }
Engine::~Engine() { }

void Engine::init()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to init glfw" << "\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(800, 600, "Rake", nullptr, nullptr);

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

    player = new Player(glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 2.f, 1.f), 2.5f);

    std::vector<std::string> skyboxFaces = {
        "textures/skybox/Daylight Box_Right.bmp", "textures/skybox/Daylight Box_Left.bmp",
        "textures/skybox/Daylight Box_Top.bmp"  , "textures/skybox/Daylight Box_Bottom.bmp",
        "textures/skybox/Daylight Box_Front.bmp", "textures/skybox/Daylight Box_Back.bmp"    
    };

    skybox = new Skybox(skyboxFaces);

    line = new Line();
    line->init();

    glfwSetWindowUserPointer(window, player->camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
    {
        Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        if (cam)
            cam->mouse_callback(window, xpos, ypos);
    });


    shaderProg = new Shader("shaders/vertex/vertex.glsl", "shaders/fragments/fragment.glsl");
    rayShader = new Shader("shaders/vertex/ray_v.glsl", "shaders/fragments/ray_f.glsl");

    worldPlane = new Cube(glm::vec3(0.f), glm::vec3(0.8f, 0.6f, 0.3f), glm::vec3(20.f, 1.f, 20.f));

    deltaTime = 0.f;
    lastFrame = 0.f;

    glEnable(GL_DEPTH_TEST);
}

void Engine::run()
{
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        skybox->draw(player->camera->getView(), player->camera->getProjection());
        
        shaderProg->use();
        
        player->update(window, deltaTime, worldPlane);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) player->shoot(worldPlane, *line);

        glm::mat4 view = player->camera->getView();
        glm::mat4 projection = player->camera->getProjection();

        shaderProg->setMat4("view", view);
        shaderProg->setMat4("proj", projection);

        worldPlane->drawWithLight(*shaderProg, false, worldLight);

        line->draw(view, projection, *rayShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Engine::quit()
{
    delete player;
    delete shaderProg;
    delete rayShader;
    delete line;
    delete worldPlane;
    delete skybox;
    
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Engine::processInput(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp, float dt)
{
    float speed = 2.5f * dt;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}