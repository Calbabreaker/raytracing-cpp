#include "scene.h"

#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>

void HitInfo::set_face_normal(const Ray& ray, const glm::vec3& outward_normal)
{
    front_face = glm::dot(ray.direction, outward_normal) < 0.0f;
    normal = front_face ? outward_normal : -outward_normal;
}

bool SceneSphere::ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info) const
{
    glm::vec3 oc = ray.origin - m_center;
    float a = glm::length2(ray.direction);
    float half_b = dot(oc, ray.direction);
    float c = glm::length2(oc) - m_radius * m_radius;
    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0.0f)
        return false;

    float sqrtd = glm::sqrt(discriminant);

    // find the nearest distance that lies in the acceptable range.
    float dist = (-half_b - sqrtd) / a;
    if (dist < dist_min || dist > dist_max)
    {
        dist = (-half_b + sqrtd) / a;
        if (dist < dist_min || dist > dist_max)
            return false;
    }

    info.dist = dist;
    info.point = ray.at(info.dist);
    glm::vec3 outward_normal = (info.point - m_center) / m_radius;
    info.set_face_normal(ray, outward_normal);
    return true;
}

Scene::Scene()
    : m_width(640), m_height(360), m_samples_per_pixel(32),
      m_camera(float(m_width) / float(m_height), 1.0f), m_image_filename("output.ppm")
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
    bool hit_anything = false;
    float closest_dist = dist_max;

    for (const auto& object : m_objects)
    {
        if (object->ray_cast(ray, dist_min, dist_max, info))
        {
            hit_anything = true;
            closest_dist = info.dist;
        }
    }

    return hit_anything;
}

constexpr glm::vec3 SKYBOX_COLOR_TOP = {0.5f, 0.7f, 1.0f};
constexpr glm::vec3 SKYBOX_COLOR_BOTTOM = {1.0f, 1.0f, 1.0f};

glm::vec3 Scene::ray_color(const Ray& ray)
{
    HitInfo info;
    if (ray_cast(ray, 0.0f, 1000.0f, info))
        return (info.normal + 1.0f) / 2.0f;

    float t = (ray.direction.y + 1.0f) / 2.0f;
    return glm::mix(SKYBOX_COLOR_BOTTOM, SKYBOX_COLOR_TOP, t);
}

void Scene::render_image()
{
    for (int j = m_height - 1; j >= 0; j--)
    {
        printf("Scanlines remaining: %i\n", j);
        for (int i = 0; i < m_width; i++)
        {
            // take the average of all the slightly differing rays in the sample (size of pixel)
            glm::vec3 color = {0.0f, 0.0f, 0.0f};
            for (int s = 0; s < m_samples_per_pixel; s++)
            {
                float u = (i + glm::linearRand(0.0f, 1.0f)) / (m_width - 1);
                float v = (j + glm::linearRand(0.0f, 1.0f)) / (m_height - 1);

                Ray ray = m_camera.get_ray(u, v);
                color += ray_color(ray);
            }

            color = glm::clamp(color / float(m_samples_per_pixel), 0.0f, 1.0f);
            fprintf(m_image_file, "%i %i %i\n", int(color.r * 255.0f), int(color.g * 255.0f),
                    int(color.b * 255.0f));
        }
    }

    printf("Rendered %s\n", m_image_filename);
}
