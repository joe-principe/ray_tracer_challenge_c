#include <stdio.h>

#include <cglm/cglm.h>

typedef struct ray ray;
typedef struct sphere sphere;

struct ray
{
    vec3 origin;
    vec3 direction;
};

struct sphere
{
    float radius;
    vec3 origin;
};

void ray_position(ray *r, float t, float *dest);
void intersect(ray *r, sphere *s, float *dest);

int
main(void)
{
    ray r1 = {{0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 1.0f}};
    ray r2 = {{0.0f, 1.0f, -5.0f}, {0.0f, 0.0f, 1.0f}};
    ray r3 = {{0.0f, 2.0f, -5.0f}, {0.0f, 0.0f, 1.0f}};
    ray r4 = {GLM_VEC3_ZERO_INIT, {0.0f, 0.0f, 1.0f}};
    ray r5 = {{0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 1.0f}};
    sphere s = {1.0f, GLM_VEC3_ZERO_INIT};
    vec2 intersections = GLM_VEC2_ZERO_INIT;

    intersect(&r1, &s, intersections);
    printf("%.2f, %.2f\n", intersections[0], intersections[1]);

    intersect(&r2, &s, intersections);
    printf("%.2f, %.2f\n", intersections[0], intersections[1]);

    intersect(&r3, &s, intersections);
    printf("%.2f, %.2f\n", intersections[0], intersections[1]);

    intersect(&r4, &s, intersections);
    printf("%.2f, %.2f\n", intersections[0], intersections[1]);

    intersect(&r5, &s, intersections);
    printf("%.2f, %.2f\n", intersections[0], intersections[1]);

    return 0;
}

void
ray_position(ray *r, float t, float *dest)
{
    glm_vec3_scale(r->direction, t, dest);
    glm_vec3_add(r->origin, dest, dest);
}

void
intersect(ray *r, sphere *s, float *dest)
{
    float a, b, c;
    float discriminant;
    vec3 sphere_to_ray;

    glm_vec3_sub(r->origin, s->origin, sphere_to_ray);

    a = glm_dot(r->direction, r->direction);
    b = 2.0f * glm_dot(r->direction, sphere_to_ray);
    c = glm_dot(sphere_to_ray, sphere_to_ray) - 1.0f;

    discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        glm_vec2_zero(dest);
        return;
    }

    dest[0] = (-b - sqrtf(discriminant)) / (2.0f * a);
    dest[1] = (-b + sqrtf(discriminant)) / (2.0f * a);
}
/* EOF */
