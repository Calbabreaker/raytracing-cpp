#pragma once

#include "material.h"
#include "util.h"

#include <memory>

class Object
{
public:
    Object(const glm::vec3& position, std::shared_ptr<Material> material)
        : m_position(position), m_material(material)
    {
    }

    // returns true if ray hits object
    virtual bool ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info) const = 0;

protected:
    glm::vec3 m_position;
    std::shared_ptr<Material> m_material;
};

class Sphere : public Object
{
public:
    Sphere(const glm::vec3& position, float radius, std::shared_ptr<Material> material)
        : Object(position, material), m_radius(radius)
    {
    }

    bool ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info) const override;

private:
    float m_radius;
};
