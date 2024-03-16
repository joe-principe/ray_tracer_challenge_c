#include <stdio.h>
#include <stdlib.h>

#include <cglm/cglm.h>

typedef struct ray ray;
typedef struct sphere sphere;
typedef struct intersection intersection;
typedef struct intersection_list intersection_list;
typedef void (*intersect_fp)(ray *, void *, intersection_list *);

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

struct intersection
{
    float t;
    void *object;
    intersect_fp intersect_func;
};

struct intersection_list
{
    int count;
    int length;
    intersection **intersections;
};

void ray_position(ray *r, float t, float *dest);
void sphere_intersect(ray *r, void *obj, intersection_list *il);
intersection_list *create_intersection_list(void);
void delete_intersection_list(intersection_list *il);
void intersection_list_add(intersection_list *il, intersection *it);
void intersection_list_remove(intersection_list *il);
intersection *create_intersection(void);
void delete_intersection(intersection *it);

int
main(void)
{
    ray r1 = {{0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 1.0f}};
    ray r2 = {{0.0f, 1.0f, -5.0f}, {0.0f, 0.0f, 1.0f}};
    ray r3 = {{0.0f, 2.0f, -5.0f}, {0.0f, 0.0f, 1.0f}};
    ray r4 = {GLM_VEC3_ZERO_INIT, {0.0f, 0.0f, 1.0f}};
    ray r5 = {{0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 1.0f}};
    sphere s = {1.0f, GLM_VEC3_ZERO_INIT};
    intersection_list *il = create_intersection_list();

    sphere_intersect(&r1, &s, il);
    printf("%.2f, %.2f\n", il->intersections[0]->t, il->intersections[1]->t);

    sphere_intersect(&r2, &s, il);
    printf("%.2f, %.2f\n", il->intersections[0]->t, il->intersections[1]->t);

    sphere_intersect(&r3, &s, il);
    printf("%.2f, %.2f\n", il->intersections[0]->t, il->intersections[1]->t);

    sphere_intersect(&r4, &s, il);
    printf("%.2f, %.2f\n", il->intersections[0]->t, il->intersections[1]->t);

    sphere_intersect(&r5, &s, il);
    printf("%.2f, %.2f\n", il->intersections[0]->t, il->intersections[1]->t);

    delete_intersection_list(il);

    return 0;
}

void
ray_position(ray *r, float t, float *dest)
{
    glm_vec3_scale(r->direction, t, dest);
    glm_vec3_add(r->origin, dest, dest);
}

void
sphere_intersect(ray *r, void *obj, intersection_list *il)
{
    float a, b, c;
    float discriminant;
    vec3 sphere_to_ray;
    sphere *s = (sphere *)obj;

    glm_vec3_sub(r->origin, s->origin, sphere_to_ray);

    a = glm_dot(r->direction, r->direction);
    b = 2.0f * glm_dot(r->direction, sphere_to_ray);
    c = glm_dot(sphere_to_ray, sphere_to_ray) - 1.0f;

    discriminant = b * b - 4 * a * c;

    if (discriminant == 0) {
        intersection_list_add(il, create_intersection());
        il->intersections[il->count]->t = (-b - sqrtf(discriminant))
                                          / (2.0f * a);
    } else if (discriminant > 0) {
        intersection_list_add(il, create_intersection());
        intersection_list_add(il, create_intersection());
        il->intersections[il->count]->t = (-b - sqrtf(discriminant))
                                          / (2.0f * a);
        il->intersections[il->count]->t = (-b + sqrtf(discriminant))
                                          / (2.0f * a);
    }
}

intersection_list *
create_intersection_list(void)
{
    intersection_list *il = malloc(sizeof(*il));

    if (il == NULL) {
        fprintf(stderr, "Error: Could not allocate enough memory for "
                "intersection list\n");
        exit(EXIT_FAILURE);
    }

    il->length = 8;
    il->count = 0;
    il->intersections = calloc(il->length, sizeof(il->intersections));

    if (il->intersections == NULL) {
        fprintf(stderr, "Error: Could not allocate enough memory for "
                "intersection list array\n");
        exit(EXIT_FAILURE);
    }

    return il;
}

void
delete_intersection_list(intersection_list *il)
{
    int i;

    for (i = 0; i < il->count; i++) {
        delete_intersection(il->intersections[i]);
    }

    free(il->intersections);
    il->intersections = NULL;

    free(il);
    il = NULL;
}

void
intersection_list_add(intersection_list *il, intersection *it)
{
    intersection *temp = NULL;

    if (il->count == il->length) {
        temp = realloc(il->intersections,
                       sizeof(il->intersections) * il->length * 2);

        if (temp == NULL) {
            fprintf(stderr, "Error: Could not reallocate memory for "
                    "intersection list array increase\n");
            exit(EXIT_FAILURE);
        }

        il->intersections = &temp;
        il->length *= 2;
    }

    il->intersections[il->count] = it;
    il->count++;
}

void
intersection_list_remove(intersection_list *il)
{
    intersection *temp = NULL;

    il->intersections[il->count] = NULL;
    il->count--;

    if (il->count == il->length / 4 && il->length > 8) {
        temp = realloc(il->intersections,
                       sizeof(il->intersections) * il->length * 2);

        if (temp == NULL) {
            fprintf(stderr, "Error: Could not reallocate memory for "
                    "intersection list array decrease\n");
            exit(EXIT_FAILURE);
        }

        il->intersections = &temp;
        il->length /= 2;
    }
}

intersection *
create_intersection(void)
{
    intersection *it = malloc(sizeof(*it));

    if (it == NULL) {
        fprintf(stderr, "Error: Could not allocate enough memory for "
                "intersection\n");
        exit(EXIT_FAILURE);
    }

    return it;
}

void
delete_intersection(intersection *it)
{
    free(it);
    it = NULL;
}
/* EOF */
