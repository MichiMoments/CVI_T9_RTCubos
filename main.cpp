#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "cube.h"

int main() {
    hittable_list world;

    auto floor_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    double floor_side = 50.0;
    world.add(make_shared<cube>(point3(0, -25, 0), floor_side, floor_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> cube_material;

                if (choose_mat < 0.8) {
                    auto albedo = color::random() * color::random();
                    cube_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<cube>(center, 0.2, cube_material));
                } else if (choose_mat < 0.95) {
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    cube_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<cube>(center, 0.2, cube_material));
                } else {
                    cube_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<cube>(center, 0.2, cube_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<cube>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<cube>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<cube>(point3(4, 1, 0), 1.0, material3));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1400;
    cam.samples_per_pixel = 10;
    cam.max_depth = 20;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;
    cam.render(world);
}
