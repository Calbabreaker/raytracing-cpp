#pragma once

#include "util.h"

#include <memory>
#include <vector>

class SceneObject
{
public:
    // returns true if ray hits object
    virtual bool ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info) const = 0;
};

class SceneSphere : public SceneObject
{
public:
    SceneSphere(const glm::vec3& center, float radius) : m_center(center), m_radius(radius) {}

    bool ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info) const override;

private:
    glm::vec3 m_center;
    float m_radius;
};

class Scene
{
public:
    Scene();

    void add_object(std::unique_ptr<SceneObject>&& object)
    {
        m_objects.emplace_back(std::move(object));
    };

    bool ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info);
    glm::vec3 ray_color(const Ray& ray, int bounces_left);
    void render_image();

private:
    int m_width;
    int m_height;
    int m_samples_per_pixel;
    int m_max_bounces;
    Camera m_camera;

    std::vector<std::unique_ptr<SceneObject>> m_objects;

    FILE* m_image_file;
    const char* m_image_filename;
};
