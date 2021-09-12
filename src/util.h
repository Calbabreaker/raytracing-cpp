#pragma once

#include <glm/glm.hpp>
#include <stdio.h>

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
