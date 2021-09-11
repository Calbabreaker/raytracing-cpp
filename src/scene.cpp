#include "scene.h"

#include <glm/gtc/random.hpp>

// TODO use cli arguments
Scene::Scene()
    : m_width(640), m_height(360), m_samples_per_pixel(32), m_max_bounces(50),
      m_camera({ -2,2,1 }, { 0,0,-1 }, 40.0f, float(m_width) / float(m_height)), m_image_filename("output.ppm")
{
    m_image_file = fopen(m_image_filename, "w");
    if (!m_image_file)
    {
        fprintf(stderr, "%s could not be opened for writing!\n", m_image_filename);
        exit(EXIT_FAILURE);
    }

    fprintf(m_image_file, "P3\n%i %i\n255\n", m_width, m_height);
}

bool Scene::ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info)
{
    float closest_dist = dist_max;
    for (const auto& object : m_objects)
    {
        if (object->ray_cast(ray, dist_min, closest_dist, info))
            closest_dist = info.dist;
    }

    return closest_dist != dist_max;
}

const glm::vec3 SKYBOX_COLOR_TOP = {0.5f, 0.7f, 1.0f};
const glm::vec3 SKYBOX_COLOR_BOTTOM = {1.0f, 1.0f, 1.0f};

glm::vec3 Scene::ray_color(const Ray& ray, int bounces_left)
{
    if (bounces_left == 0)
        return glm::vec3(0.0f, 0.0f, 0.0f);

    HitInfo info;
    if (ray_cast(ray, 0.01f, 1000.0f, info))
    {
        Ray ray_scattered;
        glm::vec3 attenuation;
        if (info.material->scatter(ray, info, ray_scattered, attenuation))
            return attenuation * ray_color(ray_scattered, bounces_left - 1);
        else
            return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    float color_scale = (ray.direction.y + 1.0f) / 2.0f;
    return glm::mix(SKYBOX_COLOR_BOTTOM, SKYBOX_COLOR_TOP, color_scale);
}

void Scene::render_image()
{
    for (int j = m_height - 1; j >= 0; j--)
    {
        printf("\rScanlines remaining: %i ", j + 1);
        fflush(stdout);
        for (int i = 0; i < m_width; i++)
        {
            glm::vec3 color = {0.0f, 0.0f, 0.0f};
            for (int s = 0; s < m_samples_per_pixel; s++)
            {
                float u = (i + glm::linearRand(0.0f, 1.0f)) / (m_width - 1);
                float v = (j + glm::linearRand(0.0f, 1.0f)) / (m_height - 1);

                Ray ray = m_camera.get_ray(u, v);
                color += ray_color(ray, m_max_bounces);
            }

            // take the average of all the slightly differing rays in the sample (size of pixel)
            // and also gamma correct for gamma=2
            color = glm::sqrt(color / float(m_samples_per_pixel));

            color = glm::clamp(color, 0.0f, 1.0f);
            fprintf(m_image_file, "%i %i %i\n", int(color.r * 255.0f), int(color.g * 255.0f),
                    int(color.b * 255.0f));
        }
    }

    printf("\nRendered %s\n", m_image_filename);
}
