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
    MetalMaterial(const glm::vec3& albedo) : m_albedo(albedo){};

    bool scatter(const Ray& ray_in, const HitInfo& info, Ray& ray_scattered,
                 glm::vec3& attenuation) const override;

private:
    glm::vec3 m_albedo;
};
