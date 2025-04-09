#ifndef CUBE_H
#define CUBE_H

//Por David Perez, en base al tutorial del cubo

#include "hittable.h"
#include "rtweekend.h"
#include <cmath>
#include <algorithm>

inline vec3 rotate_y(const vec3& v, double angle) {
    auto cos_theta = cos(angle);
    auto sin_theta = sin(angle);
    return vec3(v.x() * cos_theta + v.z() * sin_theta,
                v.y(),
               -v.x() * sin_theta + v.z() * cos_theta);
}

class cube : public hittable {
  public:
    cube(const point3& cen, double s, shared_ptr<material> m)
        : center(cen), side(s), half(s * 0.5), mat(m) {}

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        const double theta = pi / 4;

        vec3 r_origin_local = rotate_y(r.origin() - center, -theta);
        vec3 r_direction_local = rotate_y(r.direction(), -theta);

        double tmin = -infinity;
        double tmax = infinity;

        {
            double invD = 1.0 / r_direction_local.x();
            double t0 = (-half - r_origin_local.x()) * invD;
            double t1 = ( half - r_origin_local.x()) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            tmin = fmax(tmin, t0);
            tmax = fmin(tmax, t1);
        }
        {
            double invD = 1.0 / r_direction_local.y();
            double t0 = (-half - r_origin_local.y()) * invD;
            double t1 = ( half - r_origin_local.y()) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            tmin = fmax(tmin, t0);
            tmax = fmin(tmax, t1);
        }
        {
            double invD = 1.0 / r_direction_local.z();
            double t0 = (-half - r_origin_local.z()) * invD;
            double t1 = ( half - r_origin_local.z()) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            tmin = fmax(tmin, t0);
            tmax = fmin(tmax, t1);
        }

        if (tmax <= tmin)
            return false;

        double t_hit = tmin;
        if (!ray_t.surrounds(t_hit)) {
            t_hit = tmax;
            if (!ray_t.surrounds(t_hit))
                return false;
        }

        vec3 hit_point_local = r_origin_local + t_hit * r_direction_local;

        vec3 local_normal(0, 0, 0);
        const double bias = 1e-4;
        if (fabs(hit_point_local.x() - half) < bias)
            local_normal = vec3(1, 0, 0);
        else if (fabs(hit_point_local.x() + half) < bias)
            local_normal = vec3(-1, 0, 0);
        else if (fabs(hit_point_local.y() - half) < bias)
            local_normal = vec3(0, 1, 0);
        else if (fabs(hit_point_local.y() + half) < bias)
            local_normal = vec3(0, -1, 0);
        else if (fabs(hit_point_local.z() - half) < bias)
            local_normal = vec3(0, 0, 1);
        else if (fabs(hit_point_local.z() + half) < bias)
            local_normal = vec3(0, 0, -1);
        else {
            double dx = half - fabs(hit_point_local.x());
            double dy = half - fabs(hit_point_local.y());
            double dz = half - fabs(hit_point_local.z());
            if (dx < dy && dx < dz)
                local_normal = (hit_point_local.x() > 0) ? vec3(1, 0, 0) : vec3(-1, 0, 0);
            else if (dy < dz)
                local_normal = (hit_point_local.y() > 0) ? vec3(0, 1, 0) : vec3(0, -1, 0);
            else
                local_normal = (hit_point_local.z() > 0) ? vec3(0, 0, 1) : vec3(0, 0, -1);
        }

        vec3 hit_point_world = rotate_y(hit_point_local, theta) + center;
        vec3 world_normal = rotate_y(local_normal, theta);

        rec.t = t_hit;
        rec.p = hit_point_world;
        rec.set_face_normal(r, world_normal);
        rec.mat = mat;
        return true;
    }

  private:
    point3 center;
    double side;
    double half;
    shared_ptr<material> mat;
};

#endif
