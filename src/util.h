#pragma once

#include <cstdlib>
#include <ctime>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

struct Ray
{
    Ray() {}
    Ray(const glm::vec3& p_origin, const glm::vec3& p_direction)
        : origin(p_origin), direction(glm::normalize(p_direction))
    {
    }

    glm::vec3 at(float dist) const { return origin + dist * direction; }

    glm::vec3 origin = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};
};

class Material;

struct HitInfo
{
    glm::vec3 point;
    glm::vec3 normal;
    float dist;
    bool front_face;
    Material* material;

    void set_face_normal(const Ray& ray, const glm::vec3& outward_normal);
};

class Camera
{
public:
    // fov is vertical field-of-view in degrees
    void set_props(const glm::vec3& position, const glm::vec3& lookat, float fov,
                   float aspect_ratio, float apeture);

    Ray get_ray(float s, float t) const;

private:
    glm::vec3 m_position;
    glm::vec3 m_lower_left_corner;
    glm::vec3 m_horizontal;
    glm::vec3 m_vertical;
    glm::vec3 m_u, m_v;
    float m_lens_radius;
};

void write_image(const char* filename, int width, int height, glm::u8vec3* imagebuffer);

inline float random_float()
{
    static uint32_t seed = time(NULL);
    return rand_r(&seed) / float(RAND_MAX);
}

inline float random_float(float min, float max)
{
    return min + (max - min) * random_float();
}

inline glm::vec3 random_unit_vector()
{
    return glm::normalize(
        glm::vec3(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f)));
}

inline glm::vec3 random_unit_disk()
{
    while (true)
    {
        glm::vec3 point = {random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), 0.0f};
        if (glm::length2(point) < 1.0f)
            return point;
    }
}
