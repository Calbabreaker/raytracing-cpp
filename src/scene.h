#pragma once

#include "material.h"
#include "object.h"
#include "util.h"

#include <mutex>
#include <thread>
#include <vector>

struct SceneOptions
{
    int width = 1280;
    int height = 720;
    int samples_per_pixel = 32;
    int max_bounces = 50;
    int n_threads = 8;
    int tile_width = 128;
    int tile_height = 128;
    const char* output_file = "output.png";
};

struct Tile
{
    glm::ivec2 point1;
    glm::ivec2 point2;

    Tile(const glm::ivec2& p1, const glm::ivec2& p2) : point1(p1), point2(p2) {}
};

class Scene
{
public:
    Scene();

    void add_object(std::unique_ptr<Object>&& object)
    {
        m_objects.emplace_back(std::move(object));
    };

    void start_render();

private:
    bool ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info);
    glm::vec3 ray_color(const Ray& ray, int bounces_left);
    void render_tile(Tile tile);

    void render_worker();

private:
    Camera m_camera;
    SceneOptions m_options;

    glm::u8vec3* m_image_buffer;

    std::vector<std::thread> m_threads;
    std::vector<Tile> m_render_tiles;
    std::mutex m_mutex;
    int m_current_tile_index;

    std::vector<std::unique_ptr<Object>> m_objects;
};
