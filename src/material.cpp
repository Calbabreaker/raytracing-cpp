#include "material.h"

static bool near_zero(const glm::vec3& vector)
{
    constexpr float EPSILON = 0.00001f;
    return glm::length2(vector) < EPSILON * EPSILON;
}

// Uses Schlick's approximation for reflectance
static float reflectance(float cosine, float refraction_index)
{
    float r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * glm::pow((1 - cosine), 5);
}

bool DiffuseMaterial::scatter(const Ray& ray_in, const HitInfo& info, Ray& ray_scattered,
                              glm::vec3& attenuation) const
{
    glm::vec3 diffuse_direction = info.normal + random_unit_vector();

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
    ray_scattered = Ray(info.point, reflect_direction + m_fuzziness * random_unit_vector());
    attenuation = m_albedo;
    return (glm::dot(reflect_direction, info.normal) > 0.0f);
}

bool DielectricMaterial::scatter(const Ray& ray_in, const HitInfo& info, Ray& ray_scattered,
                                 glm::vec3& attenuation) const
{
    attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
    float refraction_ratio = info.front_face ? (1.0f / m_refraction_index) : m_refraction_index;

    float cos_theta = glm::min(glm::dot(-ray_in.direction, info.normal), 1.0f);
    float sin_theta = glm::sqrt(1.0f - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
    bool should_reflect = reflectance(cos_theta, refraction_ratio) > random_float();
    glm::vec3 direction = cannot_refract || should_reflect 
                              ? glm::reflect(ray_in.direction, info.normal)
                              : glm::refract(ray_in.direction, info.normal, refraction_ratio);

    ray_scattered = Ray(info.point, direction);
    return true;
}
