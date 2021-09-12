#include "scene.h"
#include "glm/fwd.hpp"
#include "util.h"

#include <chrono>
#include <glm/gtc/random.hpp>
#include <ratio>

Scene::Scene() : m_camera()
{
    int n_cores = std::thread::hardware_concurrency();
    if (n_cores != 0)
        m_options.n_threads = n_cores;

    m_camera.set_props({-2, 2, 1}, {0, 0, -1}, 40.0f,
                       float(m_options.width) / float(m_options.height), 0.0f);
    m_options.width = 640;
    m_options.height = 360;
    m_options.samples_per_pixel = 32;
}

bool Scene::ray_cast(const Ray& ray, float dist_min, float dist_max, HitInfo& info)
{
    float closest_dist = dist_max;
    for (const auto& object : m_objects)
    {
        if (object->ray_intersect(ray, dist_min, closest_dist, info))
            closest_dist = info.dist;
    }

    return closest_dist != dist_max;
}

const glm::vec3 SKYBOX_COLOR_TOP = {0.5f, 0.7f, 1.0f};
const glm::vec3 SKYBOX_COLOR_BOTTOM = {1.0f, 1.0f, 1.0f};

glm::vec3 Scene::ray_color(const Ray& ray, int bounces_left)
{
    if (bounces_left == 0)
        return glm::vec3(0.0f, 0.0f, 0.0f);

    HitInfo info;
    if (ray_cast(ray, 0.01f, 1000.0f, info))
    {
        Ray ray_scattered;
        glm::vec3 attenuation;
        if (info.material->scatter(ray, info, ray_scattered, attenuation))
            return attenuation * ray_color(ray_scattered, bounces_left - 1);
        else
            return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    float color_scale = (ray.direction.y + 1.0f) / 2.0f;
    return glm::mix(SKYBOX_COLOR_BOTTOM, SKYBOX_COLOR_TOP, color_scale);
}

void Scene::render_tile(Tile tile)
{
    for (int y = tile.point1.y; y < tile.point2.y; y++)
    {
        for (int x = tile.point1.x; x < tile.point2.x; x++)
        {
            glm::vec3 color = {0.0f, 0.0f, 0.0f};
            for (int i = 0; i < m_options.samples_per_pixel; i++)
            {
                float u = (x + random_float()) / (m_options.width - 1);
                float v = (y + random_float()) / (m_options.height - 1);

                Ray ray = m_camera.get_ray(u, v);
                color += ray_color(ray, m_options.max_bounces);
            }

            // take the average of all the slightly differing rays in the sample (size of pixel)
            // also gamma correct for gamma=2
            color = glm::sqrt(color / float(m_options.samples_per_pixel));

            m_image_buffer[y * m_options.width + x] = glm::u8vec3(color * 255.0f);
        }
    }
}

void Scene::render_worker()
{
    while (m_current_tile_index < m_render_tiles.size())
    {
        Tile* tile;
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            tile = &m_render_tiles[m_current_tile_index];
            m_current_tile_index++;
            printf("\rRendering tile %i/%li at %i,%i ", m_current_tile_index, m_render_tiles.size(),
                   tile->point1.x, tile->point1.y);
            fflush(stdout);
        }

        render_tile(*tile);
    }
}

void Scene::start_render()
{
    auto start_time = std::chrono::high_resolution_clock::now();

    m_image_buffer = new glm::u8vec3[m_options.width * m_options.height];

    // create tiles
    for (int tile_x = 0; tile_x < m_options.width; tile_x += m_options.tile_width)
    {
        for (int tile_y = 0; tile_y < m_options.height; tile_y += m_options.tile_height)
        {
            glm::ivec2 p1 = {tile_x, tile_y};
            glm::ivec2 p2 = {glm::min(tile_x + m_options.tile_width, m_options.width),
                             glm::min(tile_y + m_options.tile_height, m_options.height)};

            m_render_tiles.emplace_back(Tile(p1, p2));
        }
    }

    m_current_tile_index = 0;

    // create threads
    for (int i = 0; i < m_options.n_threads; i++)
        m_threads.emplace_back(std::thread(&Scene::render_worker, this));

    printf("Initilized %i threads. Rendering...\n", m_options.n_threads);

    // wait for threads to finish
    for (std::thread& thread : m_threads)
        thread.join();

    printf("\nFinshed rendering. Saving image...\n");
    write_image(m_options.output_file, m_options.width, m_options.height, m_image_buffer);
    delete[] m_image_buffer;
    printf("Saved %s\n", m_options.output_file);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> delta = end_time - start_time;
    printf("Took %f seconds\n", delta.count() / 1000.0f);
}
