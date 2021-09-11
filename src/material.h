#pragma once

#include "util.h"

class Material
{
public:
    // returns true if valid scatter
    virtual bool scatter(const Ray& ray_in, const HitInfo& info, Ray& ray_scattered,
                         glm::vec3& attenuation) const = 0;
};

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(const glm::vec3& albedo) : m_albedo(albedo) {}

    bool scatter(const Ray& ray_in, const HitInfo& info, Ray& ray_scattered,
                 glm::vec3& attenuation) const override;

private:
    glm::vec3 m_albedo;
};

class MetalMaterial : public Material
{
public:
    MetalMaterial(const glm::vec3& albedo, float fuzziness)
        : m_albedo(albedo), m_fuzziness(fuzziness){};

    bool scatter(const Ray& ray_in, const HitInfo& info, Ray& ray_scattered,
                 glm::vec3& attenuation) const override;

private:
    glm::vec3 m_albedo;
    float m_fuzziness;
};

class DielectricMaterial : public Material
{
public:
    DielectricMaterial(float refraction_index) : m_refraction_index(refraction_index) {}

    bool scatter(const Ray& ray_in, const HitInfo& info, Ray& ray_scattered,
                 glm::vec3& attenuation) const override;

private:
    float m_refraction_index;
};
