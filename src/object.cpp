#include "object.h"

#include <glm/gtx/norm.hpp>

bool Sphere::ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info) const
{
    glm::vec3 oc = ray.origin - m_position;
    float a = glm::length2(ray.direction);
    float half_b = dot(oc, ray.direction);
    float c = glm::length2(oc) - m_radius * m_radius;
    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0.0f)
        return false;

    float sqrtd = glm::sqrt(discriminant);

    // find the nearest distance that lies in the acceptable range.
    info.dist = (-half_b - sqrtd) / a;
    if (info.dist < dist_min || info.dist > dist_max)
    {
        info.dist = (-half_b + sqrtd) / a;
        if (info.dist < dist_min || info.dist > dist_max)
            return false;
    }

    info.point = ray.at(info.dist);
    glm::vec3 outward_normal = (info.point - m_position) / m_radius;
    info.set_face_normal(ray, outward_normal);
    info.material = m_material.get();
    return true;
}
