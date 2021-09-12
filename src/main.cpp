#include "scene.h"

#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>

int main(int argc, char** argv)
{
    srand(time(0));

    Scene scene;

    auto material_ground = std::make_shared<DiffuseMaterial>(glm::vec3(0.0f, 0.7f, 0.5f));
    auto material_diffuse = std::make_shared<DiffuseMaterial>(glm::vec3(0.5f, 0.5f, 0.8f));
    auto material_metal1 = std::make_shared<MetalMaterial>(glm::vec3(0.5f, 0.4f, 0.2f), 0.0f);
    auto material_metal2 = std::make_shared<MetalMaterial>(glm::vec3(0.3f, 0.2f, 0.5f), 0.8f);
    auto material_dieletric = std::make_shared<DielectricMaterial>(1.5f);

    scene.add_object(
        std::make_unique<Sphere>(glm::vec3(0.0f, -100.5f, 0.0f), 100.0f, material_ground));
    scene.add_object(
        std::make_unique<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, material_diffuse));
    scene.add_object(
        std::make_unique<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, material_dieletric));
    scene.add_object(
        std::make_unique<Sphere>(glm::vec3(-0.5f, 0.0f, -2.0f), 0.5f, material_metal2));
    scene.add_object(std::make_unique<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, material_metal1));

    scene.start_render();
}
