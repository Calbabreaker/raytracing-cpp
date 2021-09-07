#pragma once

#include "material.h"
#include "object.h"
#include "util.h"

#include <vector>

class Scene
{
public:
    Scene();

    void add_object(std::unique_ptr<Object>&& object)
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

    std::vector<std::unique_ptr<Object>> m_objects;

    FILE* m_image_file;
    const char* m_image_filename;
};
