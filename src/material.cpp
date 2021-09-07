#include "material.h"

#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>

bool near_zero(const glm::vec3& vector)
{
    constexpr float EPSILON = 0.00001f;
    return glm::length2(vector) < EPSILON * EPSILON;
}

bool DiffuseMaterial::scatter(const Ray& ray_in, const HitInfo& info, Ray& ray_scattered,
                              glm::vec3& attenuation) const
{
    glm::vec3 diffuse_direction = info.normal + glm::sphericalRand(1.0f);

    if (near_zero(diffuse_direction))
        diffuse_direction = info.normal;

    ray_scattered = Ray(info.point, diffuse_direction);
    attenuation = m_albedo;
    return true;
}

bool MetalMaterial::scatter(const Ray& ray_in, const HitInfo& info, Ray& ray_scattered,
                            glm::vec3& attenuation) const
{
    glm::vec3 reflect_direction = glm::reflect(ray_in.direction, info.normal);
    ray_scattered = Ray(info.point, reflect_direction);
    attenuation = m_albedo;
    return (glm::dot(reflect_direction, info.normal) > 0.0f);
}
