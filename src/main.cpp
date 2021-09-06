#include "scene.h"

int main()
{
    Scene scene;
    scene.add_object(std::make_unique<SceneSphere>(glm::vec3(0, 0, -1), 0.5f));
    scene.render_image();
}
