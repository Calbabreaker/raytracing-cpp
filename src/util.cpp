#include "util.h"

#include <stdlib.h>

Camera::Camera(const glm::vec3& position, const glm::vec3& lookat, float fov, float aspect_ratio)
    : m_position(position)
{

    float h = glm::tan(glm::radians(fov) / 2.0f);
    float viewport_height = h * 2.0f;
    float viewport_width = viewport_height * aspect_ratio;

    const glm::vec3 up = {0.0f, 1.0f, 0.0f};
    glm::vec3 w = glm::normalize(m_position - lookat);
    glm::vec3 u = glm::normalize(glm::cross(up, w));
    glm::vec3 v = glm::cross(w, u);

    m_horizontal = viewport_width * u;
    m_vertical = viewport_height * v;
    m_lower_left_corner = m_position - m_horizontal / 2.0f - m_vertical / 2.0f - w;
}

Ray Camera::get_ray(float u, float v) const
{
    return Ray(m_position, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_position);
}

void HitInfo::set_face_normal(const Ray& ray, const glm::vec3& outward_normal)
{
    front_face = glm::dot(ray.direction, outward_normal) < 0.0f;
    normal = front_face ? outward_normal : -outward_normal;
}
