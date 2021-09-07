#include "scene.h"

int main()
{
    Scene scene;
    scene.add_object(std::make_unique<SceneSphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
    scene.add_object(std::make_unique<SceneSphere>(glm::vec3(0.0f, -101.0f, 0.0f), 100.0f));
    scene.render_image();
}
