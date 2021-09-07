#include "util.h"

#include <stdlib.h>

Camera::Camera(float aspect_ratio, float focal_length)
{
    float viewport_height = 2.0f;
    float viewport_width = viewport_height * aspect_ratio;

    m_horizontal = {viewport_width, 0.0f, 0.0f};
    m_vertical = {0.0f, viewport_height, 0.0f};
    m_lower_left_corner =
        m_origin - m_horizontal / 2.0f - m_vertical / 2.0f - glm::vec3(0, 0, focal_length);
}

Ray Camera::get_ray(float u, float v) const
{
    return Ray(m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin);
}

void HitInfo::set_face_normal(const Ray& ray, const glm::vec3& outward_normal)
{
    front_face = glm::dot(ray.direction, outward_normal) < 0.0f;
    normal = front_face ? outward_normal : -outward_normal;
}
