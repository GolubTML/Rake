#include <core/engine.hpp>

int main(int argc, char** argv)
{
    Engine engine(glm::vec3(20.f, 40.f, 0.f));

    engine.init();
    engine.run();
    engine.quit();

    return 0;
}