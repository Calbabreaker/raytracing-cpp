#include "util.h"

#include <glm/gtc/random.hpp>
#include <stb_image_write.h>
#include <stdlib.h>

void Camera::set_props(const glm::vec3& position, const glm::vec3& lookat, float fov,
                       float aspect_ratio, float apeture)
{
    m_position = position;
    float h = glm::tan(glm::radians(fov) / 2.0f);
    float viewport_height = h * 2.0f;
    float viewport_width = viewport_height * aspect_ratio;

    constexpr glm::vec3 up = {0.0f, 1.0f, 0.0f};
    glm::vec3 w = m_position - lookat;
    float focus_dist = glm::length(w);
    w /= focus_dist;
    m_u = glm::normalize(glm::cross(up, w));
    m_v = glm::cross(w, m_u);

    m_horizontal = viewport_width * m_u * focus_dist;
    m_vertical = viewport_height * m_v * focus_dist;
    m_lower_left_corner = m_position - m_horizontal / 2.0f - m_vertical / 2.0f - focus_dist * w;

    m_lens_radius = apeture / 2.0f;
}

Ray Camera::get_ray(float s, float t) const
{
    glm::vec2 random_disk = glm::diskRand(m_lens_radius);
    glm::vec3 offset = m_u * random_disk.x + m_v * random_disk.y;

    return Ray(m_position + offset,
               m_lower_left_corner + s * m_horizontal + t * m_vertical - m_position - offset);
}

void HitInfo::set_face_normal(const Ray& ray, const glm::vec3& outward_normal)
{
    front_face = glm::dot(ray.direction, outward_normal) < 0.0f;
    normal = front_face ? outward_normal : -outward_normal;
}

void write_image(const char* filename, int width, int height, glm::u8vec3* imagebuffer)
{
    stbi_flip_vertically_on_write(1);
    stbi_write_png(filename, width, height, 3, imagebuffer, width * 3);
}
